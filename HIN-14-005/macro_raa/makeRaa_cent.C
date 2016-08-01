/*
Macro to plot the Efficiency vs centrality, for all the analysis bins.(in the traditional way, yield x correction, and the weighted way)

Input: root files produced by the readFitTable/makeHisto_raa.C (check there the input file names)

Output: the Raa vs cent.

 */
#if !defined(__CINT__) || defined(__MAKECINT__)
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>

#include "TROOT.h"
#include "TSystem.h"
#include "TStyle.h"
#include "TMath.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
#include "TPaveStats.h"
#include "TLatex.h"
#include "TLegend.h"
#include <TLegendEntry.h>

#include "dataBinning_2015.h"
#include "filesRaa_2015.h"

#include "../CMS_lumi.C"
#include "../tdrstyle.C"
#include "../textPosition.h"

#endif

using namespace std;

void makeRaa_cent( bool bSavePlots           = 1,
		   bool bAddCent             = 1,
		   bool bSaveRoot            = 1,
		   bool bDoDebug             = 0, // adds some numbers, numerator, denominator, to help figure out if things are read properly
		   bool bAddLumi             = 1, // add the lumi boxes at raa=1
		   int  whichSample          = 1,//0: no TnP corrections; 1: w/ TnP corr on Data; 2: w/ TnP corr on MC; 3: lxy w/ TnP on MC
		   const char* inputDir      = "../readFitTable", // the place where the input root files, with the histograms are
		   const char* outputDir     = "figs",
		   const char* outputRootDir = "outRoot")// where the output figures will be
{
  gSystem->mkdir(Form("./%s/png",outputDir), kTRUE);
  gSystem->mkdir(Form("./%s/pdf",outputDir), kTRUE);

  // set the style
  setTDRStyle();
  // gStyle->SetCanvasPreferGL(1);

  // type of available comparisons:
  const char* sample[4] = {"noTnP","dataTnP","mcTnP","lxyTnP"};

  const unsigned int nInHist = 5;
  const char* yieldHistNames[nInHist] = {"cent","y012Cent", "y1216Cent", "y1624Cent", "y1624LowPtCent"};

  // input files: are in the filesRaa_2015.h

  // open the files with yields and do the math
  TFile *fYesWeighFile_aa   = new TFile(Form("%s/%s",inputDir,yieldHistFile_yesWeight_1[0]));
  TFile *fYesWeighFile_pp   = new TFile(Form("%s/%s",inputDir,yieldHistFile_yesWeight_1[1]));
  
  TFile *fNoWeighFile_aa = new TFile(Form("%s/%s",inputDir,yieldHistFile_noWeight_1[0]));
  TFile *fNoWeighFile_pp = new TFile(Form("%s/%s",inputDir,yieldHistFile_noWeight_1[1]));

  TFile *fEffFile_aa = new TFile(Form("%s/%s",inputDir,effHistFile[0]));
  TFile *fEffFile_pp = new TFile(Form("%s/%s",inputDir,effHistFile[1]));

  switch(whichSample){
  case 0:
    cout << "You are making Raa, with NOT TnP corrections whatsoever!"<<endl;
    fYesWeighFile_aa   = new TFile(Form("%s/%s",inputDir,yieldHistFile_yesWeight_0[0]));
    fYesWeighFile_pp   = new TFile(Form("%s/%s",inputDir,yieldHistFile_yesWeight_0[1]));
  
    fNoWeighFile_aa = new TFile(Form("%s/%s",inputDir,yieldHistFile_noWeight_0[0]));
    fNoWeighFile_pp = new TFile(Form("%s/%s",inputDir,yieldHistFile_noWeight_0[1]));

    fEffFile_aa = new TFile(Form("%s/%s",inputDir,effHistFile_noTnP[0]));
    fEffFile_pp = new TFile(Form("%s/%s",inputDir,effHistFile_noTnP[1]));
    break;

  case 2:
    cout << "You are making Raa, with TnP corrections applied on MC!"<<endl;
    fYesWeighFile_aa   = new TFile(Form("%s/%s",inputDir,yieldHistFile_yesWeight_2[0]));
    fYesWeighFile_pp   = new TFile(Form("%s/%s",inputDir,yieldHistFile_yesWeight_2[1]));
  
    fNoWeighFile_aa = new TFile(Form("%s/%s",inputDir,yieldHistFile_noWeight_2[0]));
    fNoWeighFile_pp = new TFile(Form("%s/%s",inputDir,yieldHistFile_noWeight_2[1]));
    break;

  case 3: 
    cout << "You are making Raa, with Lxy and TnP corrections applie on MC!"<<endl;
    fYesWeighFile_aa   = new TFile(Form("%s/%s",inputDir,yieldHistFile_yesWeight_3[0]));
    fYesWeighFile_pp   = new TFile(Form("%s/%s",inputDir,yieldHistFile_yesWeight_3[1]));
  
    fNoWeighFile_aa = new TFile(Form("%s/%s",inputDir,yieldHistFile_noWeight_3[0]));
    fNoWeighFile_pp = new TFile(Form("%s/%s",inputDir,yieldHistFile_noWeight_3[1]));
    break;

  case 1:
  default:
    cout<<" You are doing Raa Nominal: TnP on data!"<<endl;
    break;
  }

  if (!fYesWeighFile_aa->IsOpen() || !fYesWeighFile_pp->IsOpen()|| !fNoWeighFile_aa->IsOpen() || !fNoWeighFile_pp->IsOpen() || !fEffFile_aa->IsOpen() || !fEffFile_pp->IsOpen()) {
    cout << "One or more input files are missing" << endl;
    return ;
  }

  TH1F *phRaw_pr_pp; 
  TH1F *phCorr_pr_pp;
  TH1F *phEff_pr_pp;
  TH1F *phRaw_pr_aa; 
  TH1F *phCorr_pr_aa;
  TH1F *phEff_pr_aa;

  TH1F *phRaw_npr_pp; 
  TH1F *phCorr_npr_pp;
  TH1F *phEff_npr_pp;
  TH1F *phRaw_npr_aa; 
  TH1F *phCorr_npr_aa;
  TH1F *phEff_npr_aa;
  
  for(unsigned int ih=0; ih<nInHist;ih++) // for each kinematic range
  {
    TString hist_pr(Form("phPrp_%s",yieldHistNames[ih]));
    TString hist_npr(Form("phNPrp_%s",yieldHistNames[ih]));
    
    cout<<"histogram input name: "<< hist_pr<<"\t"<<hist_npr<<endl; 

    // prompt histos
    phRaw_pr_pp  = (TH1F*)fNoWeighFile_pp->Get(hist_pr);
    phCorr_pr_pp = (TH1F*)fYesWeighFile_pp->Get(hist_pr);
    phRaw_pr_aa  = (TH1F*)fNoWeighFile_aa->Get(hist_pr);
    phCorr_pr_aa = (TH1F*)fYesWeighFile_aa->Get(hist_pr);
    
    // non-prompt histos
    phRaw_npr_pp  = (TH1F*)fNoWeighFile_pp->Get(hist_npr);
    phCorr_npr_pp = (TH1F*)fYesWeighFile_pp->Get(hist_npr);
    phRaw_npr_aa  = (TH1F*)fNoWeighFile_aa->Get(hist_npr);
    phCorr_npr_aa = (TH1F*)fYesWeighFile_aa->Get(hist_npr);

    // efficiency histos
    phEff_pr_pp  = (TH1F*)fEffFile_pp->Get(hist_pr);
    phEff_npr_pp = (TH1F*)fEffFile_pp->Get(hist_npr);
    phEff_pr_aa  = (TH1F*)fEffFile_aa->Get(hist_pr);
    phEff_npr_aa = (TH1F*)fEffFile_aa->Get(hist_npr);

    double scaleFactor = ppLumi/nMbEvents;

    unsigned int numBins = 0;
    if(ih==0) numBins = nBinsNpart12;
    if(ih==4) numBins = nBinsNpart6;
    if(ih==1 || ih==2 || ih==3) numBins = nBinsNpart6;

    for (unsigned int ibin=1; ibin<=numBins; ibin++)
    {
      cout << endl << "ih " << ih << ", ibin " << ibin << "/" << numBins << endl;
      
      double raa_pr=0, raaErr_pr=0, raa_npr=0, raaErr_npr=0;
      double scale_cent = 1;
      double scale_cent_np = 1;

      if(ih==0)
      {
        scale_cent = 1/(adTaa12[ibin-1]*adDeltaCent12[ibin-1]);
        scale_cent_np = 1/(adTaa6[ibin-1]*adDeltaCent6[ibin-1]);
      }
      if(ih==4)          scale_cent = 1/(adTaa6[ibin-1]*adDeltaCent6[ibin-1]);
      if(ih!=0 && ih!=4) scale_cent = 1/(adTaa6[ibin-1]*adDeltaCent6[ibin-1]);
      //prompt
      double dRelErrRaw_pr_pp  = phRaw_pr_pp->GetBinError(ibin)/phRaw_pr_pp->GetBinContent(ibin);
      double dRelErrRaw_pr_aa  = phRaw_pr_aa->GetBinError(ibin)/phRaw_pr_aa->GetBinContent(ibin);
      double yieldRatio_pr     = phCorr_pr_aa->GetBinContent(ibin)/phCorr_pr_pp->GetBinContent(ibin);
    
      raa_pr      =  yieldRatio_pr * scaleFactor * scale_cent;
      raaErr_pr   = TMath::Sqrt(TMath::Power(dRelErrRaw_pr_pp,2)+TMath::Power(dRelErrRaw_pr_aa,2))*raa_pr;

      //non-prompt
      // get the rel uncert from the raw sample
      double dRelErrRaw_npr_pp  = phRaw_npr_pp->GetBinError(ibin)/phRaw_npr_pp->GetBinContent(ibin);
      double dRelErrRaw_npr_aa  = phRaw_npr_aa->GetBinError(ibin)/phRaw_npr_aa->GetBinContent(ibin);
      double yieldRatio_npr     = phCorr_npr_aa->GetBinContent(ibin)/phCorr_npr_pp->GetBinContent(ibin);
    
      if(ih==0)raa_npr= yieldRatio_npr * scaleFactor * scale_cent_np;// the 1D nonPr has 6 bins only
      else raa_npr    = yieldRatio_npr * scaleFactor * scale_cent;
      raaErr_npr = TMath::Sqrt(TMath::Power(dRelErrRaw_npr_pp,2)+TMath::Power(dRelErrRaw_npr_aa,2))*raa_npr;

      // Check bin content
      if (ibin <= 6) {
        cout << "adTaa12\tadDeltaCent12\tadTaa6\tadDeltaCent6\n"
          << adTaa12[ibin-1] << "\t" << adDeltaCent12[ibin-1] << "\t"
          << adTaa6[ibin-1] << "\t" << adDeltaCent6[ibin-1] << endl;
        cout << "scale_cent_np\tscale_cent\tscaleFactor\tyieldRatio_pr\traa_pr\n"
          << scale_cent_np << "\t" << scale_cent << "\t" 
          << scaleFactor << "\t" << yieldRatio_pr << "\t" << raa_pr << endl;
      } else {
        cout << "adTaa12\tadDeltaCent12\n"
          << adTaa12[ibin-1] << "\t" << adDeltaCent12[ibin-1] << endl;
        cout << "scale_cent\tscaleFactor\tyieldRatio_pr\traa_pr\n"
          << scale_cent << "\t" << scaleFactor << "\t" << yieldRatio_pr << "\t" << raa_pr << endl;
      }

     
      // fill the corresponding array
      switch(ih) {
      case 0:
        prJpsi_cent[ibin-1]    = raa_pr;
        prJpsiErr_cent[ibin-1] = raaErr_pr;
        if(bDoDebug)
        {
          cout<<"weight_pr_aa = "<<phEff_pr_aa->GetBinContent(ibin)<<"\t weight_pr_pp = "<<phEff_pr_pp->GetBinContent(ibin)<<endl;
          cout<<"yield_pr_aa "<<phCorr_pr_aa->GetBinContent(ibin)<<"\t yield_pr_pp "<<phCorr_pr_pp->GetBinContent(ibin)<<endl;
        
          cout<<"pr_aa= "<<phRaw_pr_aa->GetBinContent(ibin)/phEff_pr_aa->GetBinContent(ibin)<<"\t pr_pp= "<<phRaw_pr_pp->GetBinContent(ibin)/phEff_pr_pp->GetBinContent(ibin)<<endl;
	  cout<<setprecision(2);
          cout<<"!!!!! raa = "<<prJpsi_cent[ibin-1]<<endl;
          
          // cout<<"Scale_Cent= "<<scale_cent<<endl;
        }

        nonPrJpsi_cent[ibin-1]    = raa_npr;
        nonPrJpsiErr_cent[ibin-1] = raaErr_npr;
        break;
        
      case 1:
        prJpsi_pt6530y012_cent[ibin-1]        = raa_pr;
        prJpsiErr_pt6530y012_cent[ibin-1]     = raaErr_pr;

        nonPrJpsi_pt6530y012_cent[ibin-1]     = raa_npr;
        nonPrJpsiErr_pt6530y012_cent[ibin-1]  = raaErr_npr;
        break;
        
      case 2:
        prJpsi_pt6530y1216_cent[ibin-1]       = raa_pr;
        prJpsiErr_pt6530y1216_cent[ibin-1]    = raaErr_pr;

        nonPrJpsi_pt6530y1216_cent[ibin-1]    = raa_npr;
        nonPrJpsiErr_pt6530y1216_cent[ibin-1] = raaErr_npr;
        break;
        
      case 3:
        prJpsi_pt6530y1624_cent[ibin-1]       = raa_pr;
        prJpsiErr_cent[ibin-1]                = raaErr_pr;

        nonPrJpsi_pt6530y1624_cent[ibin-1]    = raa_npr;
        nonPrJpsiErr_pt6530y1624_cent[ibin-1] = raaErr_npr;
        break;
       
      case 4:
        prJpsi_pt365y1624_cent[ibin-1]        = raa_pr;
        prJpsiErr_pt365y1624_cent[ibin-1]     = raaErr_pr;

        nonPrJpsi_pt365y1624_cent[ibin-1]     = raa_npr;
        nonPrJpsiErr_pt365y1624_cent[ibin-1]  = raaErr_npr;
        break;
      }
    }//loop end: for(int ibin=1; ibin<=numBins; ibin++)
  
  }//loop end: for(int ih=0; ih<nInHist;ih++)


  // LOADING SYSTEMATICS
  TFile *systFile = new TFile("../calcSyst/histSyst/raaSystUncert_cent.root","read");

  // ***** //Drawing
  // pr
  TGraphErrors *gPrJpsi     = new TGraphErrors(nBinsNpart12, binsNpart12, prJpsi_cent, binsNpart12Err, prJpsiErr_cent);
  TGraphErrors *gPrJpsiP    = new TGraphErrors(nBinsNpart12, binsNpart12, prJpsi_cent, binsNpart12Err, prJpsiErr_cent);
  TGraphErrors *gPrJpsiSyst = (TGraphErrors *)systFile->Get("gPrJpsiSyst");
 
  TGraphErrors *gPrJpsi_pt6530y012     = new TGraphErrors(nBinsNpart6, binsNpart6_shiftMinus, prJpsi_pt6530y012_cent, binsNpart6Err, prJpsiErr_pt6530y012_cent);
  TGraphErrors *gPrJpsiP_pt6530y012    = new TGraphErrors(nBinsNpart6, binsNpart6_shiftMinus, prJpsi_pt6530y012_cent, binsNpart6Err, prJpsiErr_pt6530y012_cent);
  TGraphErrors *gPrJpsiSyst_pt6530y012 = (TGraphErrors *)systFile->Get("gPrJpsiSyst_pt6530y012");

  TGraphErrors *gPrJpsi_pt6530y1216     = new TGraphErrors(nBinsNpart6, binsNpart6, prJpsi_pt6530y1216_cent, binsNpart6Err, prJpsiErr_pt6530y1216_cent);
  TGraphErrors *gPrJpsiP_pt6530y1216    = new TGraphErrors(nBinsNpart6, binsNpart6, prJpsi_pt6530y1216_cent, binsNpart6Err, prJpsiErr_pt6530y1216_cent);
  TGraphErrors *gPrJpsiSyst_pt6530y1216 = (TGraphErrors *)systFile->Get("gPrJpsiSyst_pt6530y1216");

  TGraphErrors *gPrJpsi_pt6530y1624     = new TGraphErrors(nBinsNpart6, binsNpart6_shiftPlus, prJpsi_pt6530y1624_cent, binsNpart6Err, prJpsiErr_pt6530y1624_cent);
  TGraphErrors *gPrJpsiP_pt6530y1624    = new TGraphErrors(nBinsNpart6, binsNpart6_shiftPlus, prJpsi_pt6530y1624_cent, binsNpart6Err, prJpsiErr_pt6530y1624_cent);
  TGraphErrors *gPrJpsiSyst_pt6530y1624 = (TGraphErrors *)systFile->Get("gPrJpsiSyst_pt6530y1624");

  TGraphErrors *gPrJpsi_pt365y1624     = new TGraphErrors(nBinsNpart6, binsNpart6, prJpsi_pt365y1624_cent, binsNpart6Err, prJpsiErr_pt365y1624_cent);
  TGraphErrors *gPrJpsiP_pt365y1624    = new TGraphErrors(nBinsNpart6, binsNpart6, prJpsi_pt365y1624_cent, binsNpart6Err, prJpsiErr_pt365y1624_cent);
  TGraphErrors *gPrJpsiSyst_pt365y1624 = (TGraphErrors *)systFile->Get("gPrJpsiSyst_pt365y1624");
 
  // nonPr
  TGraphErrors *gNonPrJpsi     = new TGraphErrors(nBinsNpart6, binsNpart6, nonPrJpsi_cent, binsNpart6Err, nonPrJpsiErr_cent);
  TGraphErrors *gNonPrJpsiP    = new TGraphErrors(nBinsNpart6, binsNpart6, nonPrJpsi_cent, binsNpart6Err, nonPrJpsiErr_cent);
  TGraphErrors *gNonPrJpsiSyst = (TGraphErrors*)systFile->Get("gNonPrJpsiSyst");

  TGraphErrors *gNonPrJpsi_pt6530y012     = new TGraphErrors(nBinsNpart6, binsNpart6_shiftMinus,nonPrJpsi_pt6530y012_cent, binsNpart6Err, nonPrJpsiErr_pt6530y012_cent);
  TGraphErrors *gNonPrJpsiP_pt6530y012    = new TGraphErrors(nBinsNpart6, binsNpart6_shiftMinus,nonPrJpsi_pt6530y012_cent, binsNpart6Err, nonPrJpsiErr_pt6530y012_cent);
  TGraphErrors *gNonPrJpsiSyst_pt6530y012 = (TGraphErrors*)systFile->Get("gNonPrJpsiSyst_pt6530y012");

  TGraphErrors *gNonPrJpsi_pt6530y1216     = new TGraphErrors(nBinsNpart6, binsNpart6, nonPrJpsi_pt6530y1216_cent, binsNpart6Err, nonPrJpsiErr_pt6530y1216_cent);
  TGraphErrors *gNonPrJpsiP_pt6530y1216    = new TGraphErrors(nBinsNpart6, binsNpart6, nonPrJpsi_pt6530y1216_cent, binsNpart6Err, nonPrJpsiErr_pt6530y1216_cent);
  TGraphErrors *gNonPrJpsiSyst_pt6530y1216 = (TGraphErrors*)systFile->Get("gNonPrJpsiSyst_pt6530y1216");
 
  TGraphErrors *gNonPrJpsi_pt6530y1624     = new TGraphErrors(nBinsNpart6, binsNpart6_shiftPlus, nonPrJpsi_pt6530y1624_cent, binsNpart6Err, nonPrJpsiErr_pt6530y1624_cent);
  TGraphErrors *gNonPrJpsiP_pt6530y1624    = new TGraphErrors(nBinsNpart6, binsNpart6_shiftPlus, nonPrJpsi_pt6530y1624_cent, binsNpart6Err, nonPrJpsiErr_pt6530y1624_cent);
  TGraphErrors *gNonPrJpsiSyst_pt6530y1624 = (TGraphErrors*)systFile->Get("gNonPrJpsiSyst_pt6530y1624");

  TGraphErrors *gNonPrJpsi_pt365y1624     = new TGraphErrors(nBinsNpart6, binsNpart6, nonPrJpsi_pt365y1624_cent, binsNpart6Err, nonPrJpsiErr_pt365y1624_cent);
  TGraphErrors *gNonPrJpsiP_pt365y1624    = new TGraphErrors(nBinsNpart6, binsNpart6, nonPrJpsi_pt365y1624_cent, binsNpart6Err, nonPrJpsiErr_pt365y1624_cent);
  TGraphErrors *gNonPrJpsiSyst_pt365y1624 = (TGraphErrors*)systFile->Get("gNonPrJpsiSyst_pt365y1624");


  //-------------------------------------------------------------------
  // **************** marker colors
  //prompt
  gPrJpsi->SetMarkerColor(kRed);
  gPrJpsi_pt6530y012->SetMarkerColor(kAzure+7);
  gPrJpsi_pt6530y1216->SetMarkerColor(kRed);
  gPrJpsi_pt6530y1624->SetMarkerColor(kGreen+2);   
  gPrJpsi_pt365y1624->SetMarkerColor(kViolet+2);
  
  // non-prompt
  gNonPrJpsi->SetMarkerColor(kOrange+2);
  gNonPrJpsi_pt6530y012->SetMarkerColor(kAzure+7);
  gNonPrJpsi_pt6530y1216->SetMarkerColor(kRed);
  gNonPrJpsi_pt6530y1624->SetMarkerColor(kGreen+2);  
  gNonPrJpsi_pt365y1624->SetMarkerColor(kViolet+2);
  
  //--------- marker style  
  // pr
  gPrJpsi->SetMarkerStyle(21);
  gPrJpsiP->SetMarkerStyle(25);

  gPrJpsi_pt6530y012->SetMarkerStyle(20);
  gPrJpsi_pt6530y1216->SetMarkerStyle(21);
  gPrJpsi_pt6530y1624->SetMarkerStyle(33);
  gPrJpsi_pt365y1624->SetMarkerStyle(34);

  gPrJpsiP_pt6530y012->SetMarkerStyle(24);
  gPrJpsiP_pt6530y1216->SetMarkerStyle(25);
  gPrJpsiP_pt6530y1624->SetMarkerStyle(27);
  gPrJpsiP_pt365y1624->SetMarkerStyle(28);

  // non-pr
  gNonPrJpsi->SetMarkerStyle(29);
  gNonPrJpsiP->SetMarkerStyle(30);
  
  gNonPrJpsi_pt6530y012->SetMarkerStyle(20);
  gNonPrJpsi_pt6530y1216->SetMarkerStyle(21);
  gNonPrJpsi_pt6530y1624->SetMarkerStyle(33);
  gNonPrJpsi_pt365y1624->SetMarkerStyle(34);

  gNonPrJpsiP_pt6530y012->SetMarkerStyle(24);
  gNonPrJpsiP_pt6530y1216->SetMarkerStyle(25);
  gNonPrJpsiP_pt6530y1624->SetMarkerStyle(27);
  gNonPrJpsiP_pt365y1624->SetMarkerStyle(28);


  // ************** contour
  // pr
  gPrJpsiP->SetMarkerColor(kBlack);
  gPrJpsiP_pt6530y012->SetMarkerColor(kBlack);
  gPrJpsiP_pt6530y1216->SetMarkerColor(kBlack);
  gPrJpsiP_pt6530y1624->SetMarkerColor(kBlack);
  gPrJpsiP_pt365y1624->SetMarkerColor(kBlack);

  // nonPr
  gNonPrJpsiP->SetMarkerColor(kBlack);
  gNonPrJpsiP_pt6530y012->SetMarkerColor(kBlack);
  gNonPrJpsiP_pt6530y1216->SetMarkerColor(kBlack);
  gNonPrJpsiP_pt6530y1624->SetMarkerColor(kBlack);
  gNonPrJpsiP_pt365y1624->SetMarkerColor(kBlack);

  // marker size
  // pr
  gPrJpsi->SetMarkerSize(1.2);
  gPrJpsiP->SetMarkerSize(1.2);

  gPrJpsi_pt6530y012->SetMarkerSize(1.2);
  gPrJpsi_pt6530y1216->SetMarkerSize(1.2);
  gPrJpsi_pt6530y1624->SetMarkerSize(2.0);
  gPrJpsi_pt365y1624->SetMarkerSize(1.7);

  gPrJpsiP_pt6530y012->SetMarkerSize(1.2);
  gPrJpsiP_pt6530y1216->SetMarkerSize(1.2);
  gPrJpsiP_pt6530y1624->SetMarkerSize(2.0);
  gPrJpsiP_pt365y1624->SetMarkerSize(1.7);
  
  // nonPr
  gNonPrJpsi->SetMarkerSize(2.0);
  gNonPrJpsiP->SetMarkerSize(2.0);

  gNonPrJpsi_pt6530y012->SetMarkerSize(1.2);
  gNonPrJpsi_pt6530y1216->SetMarkerSize(1.2);
  gNonPrJpsi_pt6530y1624->SetMarkerSize(2.0);
  gNonPrJpsi_pt365y1624->SetMarkerSize(1.7);
  
  gNonPrJpsiP_pt6530y012->SetMarkerSize(1.2);
  gNonPrJpsiP_pt6530y1216->SetMarkerSize(1.2);
  gNonPrJpsiP_pt6530y1624->SetMarkerSize(2.0);
  gNonPrJpsiP_pt365y1624->SetMarkerSize(1.7);

  //syst boxes
  gPrJpsiSyst->SetFillColorAlpha(kRed-9,0.5);
  gPrJpsiSyst_pt6530y012->SetFillColorAlpha(kAzure-9, 0.5);
  gPrJpsiSyst_pt6530y1216->SetFillColorAlpha(kRed-9, 0.5);
  gPrJpsiSyst_pt6530y1624->SetFillColorAlpha(kGreen-9, 0.5); 
  gPrJpsiSyst_pt365y1624->SetFillColorAlpha(kViolet-9, 0.5);

  // non-pr
  gNonPrJpsiSyst->SetFillColorAlpha(kOrange-9,0.5);
  gNonPrJpsiSyst_pt6530y012->SetFillColorAlpha(kAzure-9, 0.5);
  gNonPrJpsiSyst_pt6530y1216->SetFillColorAlpha(kRed-9, 0.5);
  gNonPrJpsiSyst_pt6530y1624->SetFillColorAlpha(kGreen-9, 0.5);
  gNonPrJpsiSyst_pt365y1624->SetFillColorAlpha(kViolet-9, 0.5);

  //-------------------------------------------
  TF1 *f4 = new TF1("f4","1",0,400);
  f4->SetLineWidth(1);
  f4->SetLineStyle(1);
  f4->SetLineColor(1);
  f4->GetXaxis()->SetTitle("N_{part}");
  f4->GetYaxis()->SetTitle("R_{AA}");
  f4->GetYaxis()->SetRangeUser(0.0,1.5);
  f4->GetXaxis()->CenterTitle(kTRUE);

  // pp lumi + pp stat+pp syst
  TBox *lumi_pr_y024_pt6530  = (TBox*)systFile->Get("lumi_pr_y024_pt6530");
  TBox *lumi_pr_y012_pt6530  = (TBox*)systFile->Get("lumi_pr_y012_pt6530");
  TBox *lumi_pr_y1216_pt6530 = (TBox*)systFile->Get("lumi_pr_y1216_pt6530");
  TBox *lumi_pr_y1624_pt6530 = (TBox*)systFile->Get("lumi_pr_y1624_pt6530");
  TBox *lumi_pr_y1624_pt6530_pty = (TBox*)systFile->Get("lumi_pr_y1624_pt6530_pty");
  TBox *lumi_pr_y1624_pt365      = (TBox*)systFile->Get("lumi_pr_y1624_pt365");

  TBox *lumi_npr_y024_pt6530  = (TBox*)systFile->Get("lumi_npr_y024_pt6530");
  TBox *lumi_npr_y012_pt6530  = (TBox*)systFile->Get("lumi_npr_y012_pt6530");
  TBox *lumi_npr_y1216_pt6530 = (TBox*)systFile->Get("lumi_npr_y1216_pt6530");
  TBox *lumi_npr_y1624_pt6530 = (TBox*)systFile->Get("lumi_npr_y1624_pt6530");
  TBox *lumi_npr_y1624_pt6530_pty = (TBox*)systFile->Get("lumi_npr_y1624_pt6530_pty");
  TBox *lumi_npr_y1624_pt365      = (TBox*)systFile->Get("lumi_npr_y1624_pt365");

  //----- colors for boxes
  lumi_pr_y024_pt6530->SetFillColor(kRed-9);
  lumi_pr_y012_pt6530->SetFillColor(kAzure-9);
  lumi_pr_y1216_pt6530->SetFillColor(kRed-9);
  lumi_pr_y1624_pt6530->SetFillColor(kGreen-9);
  lumi_pr_y1624_pt6530_pty->SetFillColor(kGreen-9);
  lumi_pr_y1624_pt365->SetFillColor(kViolet-9);

  lumi_npr_y024_pt6530->SetFillColor(kOrange-9);
  lumi_npr_y012_pt6530->SetFillColor(kAzure-9);
  lumi_npr_y1216_pt6530->SetFillColor(kRed-9);
  lumi_npr_y1624_pt6530->SetFillColor(kGreen-9);
  lumi_npr_y1624_pt6530_pty->SetFillColor(kGreen-9);
  lumi_npr_y1624_pt365->SetFillColor(kViolet-9);

  //---------------- general stuff
  TLatex *lat = new TLatex();
  lat->SetNDC();
  lat->SetTextFont(42);

  
  // ##################################################### pr plots
  TCanvas *c1 = new TCanvas("c1","c1");
  // general stuff
  f4->Draw();// axis
  if(bAddLumi) 
  {
    lumi_pr_y024_pt6530->Draw();
    f4->Draw("same");
  }
  lat->SetTextSize(ltxSetTextSize1);
  lat->SetTextFont(22);
  lat->DrawLatex(ltxText_xStart,ltxText_yStart,"Prompt J/#psi");
 
  lat->SetTextFont(42);
  lat->SetTextSize(ltxSetTextSize2);
  lat->DrawLatex(ltxText_xUp,ltxText_yUp,"#splitline{6.5 < p_{T} < 30 GeV/c}{|y| < 2.4}");

  CMS_lumi(c1,12014000,0);

  gPrJpsiSyst->Draw("2");
  gPrJpsi->Draw("P");
  gPrJpsiP->Draw("P");

  // ****** centrality notation on plot ******
  if(bAddCent)
    {
      lat->SetTextFont(42);
      lat->SetTextSize(0.025);
      lat->DrawLatex(0.89,0.29,"0-5%");
      lat->DrawLatex(0.82,0.3,"5-10%");
      lat->DrawLatex(0.71,0.31,"10-15%");
      lat->DrawLatex(0.62,0.33,"15-20%");
      lat->DrawLatex(0.55,0.37,"20-25%");
      lat->DrawLatex(0.49,0.4,"25-30%");
      lat->DrawLatex(0.44,0.42,"30-35%");
      lat->DrawLatex(0.38,0.44,"35-40%");
      lat->DrawLatex(0.34,0.47,"40-45%");
      lat->DrawLatex(0.3,0.49,"45-50%");
      lat->DrawLatex(0.25,0.57,"50-60%");
      lat->DrawLatex(0.17,0.59,"60-100%");
    }
  c1->SetTitle(" ");
  c1->Update();
  if(bSavePlots)
  {
    c1->SaveAs(Form("%s/pdf/PrJpsi_vsCent_%s_addCent%d.pdf",outputDir,sample[whichSample],bAddCent));
    c1->SaveAs(Form("%s/png/PrJpsi_vsCent_%s_addCent%d.png",outputDir,sample[whichSample],bAddCent));
    c1->SaveAs(Form("%s/PrJpsi_vsCent_%s.root",outputDir,sample[whichSample]));
  }

  //------------------- (pt, y) dependence
  TCanvas *c11a = new TCanvas("c11a","c11a");
  f4->Draw();
  if(bAddLumi)
  {
    lumi_pr_y012_pt6530->Draw();
    lumi_pr_y1216_pt6530->Draw();
    lumi_pr_y1624_pt6530->Draw();
    f4->Draw("same");
  }
  CMS_lumi(c11a,12014000,0);
  
  lat->SetTextSize(ltxSetTextSize1);
  lat->SetTextFont(22);
  lat->DrawLatex(ltxText_xStart,ltxText_yStart,"Prompt J/#psi");
  lat->SetTextFont(42);
  lat->SetTextSize(ltxSetTextSize2);
  lat->DrawLatex(ltxText_xDown,ltxText_yDown,"6.5 < p_{T} < 30 GeV/c");
 
  gPrJpsiSyst_pt6530y1216->Draw("2");
  gPrJpsiSyst_pt6530y012->Draw("2");
  gPrJpsiSyst_pt6530y1624->Draw("2");
  gPrJpsi_pt6530y012->Draw("P");
  gPrJpsi_pt6530y1216->Draw("P");
  gPrJpsi_pt6530y1624->Draw("P");
  gPrJpsiP_pt6530y012->Draw("P");
  gPrJpsiP_pt6530y1216->Draw("P");
  gPrJpsiP_pt6530y1624->Draw("P");

  //------------
  TLegend *leg11a = new TLegend(leg2DRaa_xStart,leg2DRaaY_yStart,leg2DRaa_xEnd,leg2DRaaY_yEnd);
  leg11a->SetBorderSize(0);
  leg11a->SetTextFont(22);
  leg11a->SetTextSize(ltxSetTextSize2);
  leg11a->SetLineColor(1);
  leg11a->SetLineStyle(1);
  leg11a->SetLineWidth(1);
  leg11a->SetFillColor(19);
  leg11a->SetFillStyle(0);
  
  TLegendEntry *le11a1 = leg11a->AddEntry("gPrJpsi_pt6530y012","|y|<1.2","P");
  le11a1->SetTextColor(kAzure+7);
  le11a1->SetFillStyle(1001);
  le11a1->SetFillColor(kAzure-9);
  le11a1->SetMarkerStyle(20);
  le11a1->SetMarkerColor(kAzure+2);
  le11a1->SetMarkerSize(1.2);


  TLegendEntry *le11a2 = leg11a->AddEntry("gPrJpsi_pt6530y1216","1.2<|y|<1.6","P");
  le11a2->SetTextColor(kRed);
  le11a2->SetFillStyle(1001);
  le11a2->SetFillColor(kRed-9);
  le11a2->SetMarkerStyle(21);
  le11a2->SetMarkerColor(kRed);
  le11a2->SetMarkerSize(1.2);

  TLegendEntry *le11a3 = leg11a->AddEntry("gPrJpsi_pt6530y1624","1.6<|y|<2.4","P");
  le11a3->SetTextColor(kGreen+2);
  le11a3->SetFillStyle(1001);
  le11a3->SetFillColor(kGreen-9);
  le11a3->SetMarkerStyle(33);
  le11a3->SetMarkerColor(kGreen+2);
  le11a3->SetMarkerSize(1.5);

  leg11a->Draw();
  gPad->RedrawAxis();
  c11a->Update();
  // ---------------------------
  TCanvas *c11b = new TCanvas("c11b","c11b");
  f4->Draw();
  if(bAddLumi)
  {
    lumi_pr_y1624_pt6530_pty->Draw();
    lumi_pr_y1624_pt365->Draw();
    f4->Draw("same");
  }
  CMS_lumi(c11b,12014000,0);

  lat->SetTextSize(ltxSetTextSize1);
  lat->SetTextFont(22);
  lat->DrawLatex(ltxText_xStart,ltxText_yStart,"Prompt J/#psi");
  lat->SetTextFont(42);
  lat->SetTextSize(ltxSetTextSize2);
  lat->DrawLatex(ltxText_xDown,ltxText_yDown,"1.6 < |y| < 2.4");
 
  gPrJpsiSyst_pt365y1624->Draw("2");
  gPrJpsiSyst_pt6530y1624->Draw("2");
  gPrJpsi_pt6530y1624->Draw("P");
  gPrJpsi_pt365y1624->Draw("P");
  gPrJpsiP_pt6530y1624->Draw("P");
  gPrJpsiP_pt365y1624->Draw("P");

  TLegend *leg21b = new TLegend(leg2DRaa_xStart,leg2DRaaPt_yStart,leg2DRaa_xEnd,leg2DRaaPt_yEnd);
  leg21b->SetBorderSize(0);
  leg21b->SetTextFont(22);
  leg21b->SetTextSize(ltxSetTextSize2);
  leg21b->SetLineColor(1);
  leg21b->SetLineStyle(1);
  leg21b->SetLineWidth(1);
  leg21b->SetFillColor(19);
  leg21b->SetFillStyle(0);
  TLegendEntry *leg21b2 = leg21b->AddEntry("gNonPrJpsi_pt365y1624", "3 < p_{T} < 6.5 GeV/c","P");
  leg21b2->SetTextColor(kViolet+2);
  leg21b2->SetFillStyle(1001);
  leg21b2->SetFillColor(kViolet-9);
  leg21b2->SetMarkerStyle(34);
  leg21b2->SetMarkerColor(kViolet+2);
  leg21b2->SetMarkerSize(1.7);
  
  TLegendEntry *leg21b1 = leg21b->AddEntry("gNonPrJpsi_pt6530y1624","6.5 < p_{T} < 30 GeV/c","P");
  leg21b1->SetTextColor(kGreen+2);
  leg21b1->SetFillStyle(1001);
  leg21b1->SetFillColor(kGreen-9);
  leg21b1->SetMarkerStyle(33);
  leg21b1->SetMarkerColor(kGreen+2);
  leg21b1->SetMarkerSize(2);

  leg21b->Draw();
  gPad->RedrawAxis();
  c11b->Update();
  
  if(bSavePlots)
  {

    c11a->SaveAs(Form("%s/pdf/PrJpsi_vsCent_yHighPt_%s.pdf",outputDir,sample[whichSample]));
    c11a->SaveAs(Form("%s/png/PrJpsi_vsCent_yHighPt_%s.png",outputDir,sample[whichSample]));
    c11a->SaveAs(Form("%s/PrJpsi_vsCent_yHighPt_%s.root",outputDir,sample[whichSample]));

    c11b->SaveAs(Form("%s/pdf/PrJpsi_vsCent_yLowPt_%s.pdf",outputDir,sample[whichSample]));
    c11b->SaveAs(Form("%s/png/PrJpsi_vsCent_yLowPt_%s.png",outputDir,sample[whichSample]));
    c11b->SaveAs(Form("%s/PrJpsi_vsCent_yLowPt_%s.root",outputDir,sample[whichSample]));

  }
  
  //   // ############################################## non-pr
  //   // ############################################## non-pr
  //   // ############################################## non-pr
  TCanvas *c2 = new TCanvas("c2","c2");
  f4->Draw();
  // general stuff
  if(bAddLumi) 
  {
    lumi_npr_y024_pt6530->Draw();
    f4->Draw("same");
  }
   lat->SetTextSize(ltxSetTextSize1);
  lat->SetTextFont(22);
  lat->DrawLatex(ltxText_xStart,ltxText_yStart,"Non-prompt J/#psi");
 
  lat->SetTextFont(42);
  lat->SetTextSize(ltxSetTextSize2);
  lat->DrawLatex(ltxText_xUp,ltxText_yUp,"#splitline{6.5 < p_{T} < 30 GeV/c}{|y| < 2.4}");
  CMS_lumi(c2,12014000,0);

  gNonPrJpsiSyst->Draw("2");
  gNonPrJpsi->Draw("P");
  gNonPrJpsiP->Draw("P");

  if(bAddCent)
    {
      lat->SetTextFont(42);
      lat->SetTextSize(0.025);
      lat->DrawLatex(0.83,0.33,"0-10%");
      lat->DrawLatex(0.65,0.39,"10-20%");
      lat->DrawLatex(0.51,0.43,"20-30%");
      lat->DrawLatex(0.4,0.45,"30-40%");
      lat->DrawLatex(0.31,0.55,"40-50%");
      lat->DrawLatex(0.19,0.56,"50-100%");
    }
   
  gPad->RedrawAxis();
  c2->Update();
  
  if(bSavePlots)
  {
    c2->SaveAs(Form("%s/pdf/nonPrJpsi_vsCent_%s_addCent%d.pdf",outputDir,sample[whichSample],bAddCent));
    c2->SaveAs(Form("%s/png/nonPrJpsi_vsCent_%s_addCent%d.png",outputDir,sample[whichSample],bAddCent));
    c2->SaveAs(Form("%s/nonPrJpsi_vsCent_%s.root",outputDir,sample[whichSample]));
  }
  
 
  //------------------- (pt, y) dependence
  TCanvas *c21a = new TCanvas("c21a","c21a");
  f4->Draw();
  if(bAddLumi)
  {
    lumi_npr_y012_pt6530->Draw();
    lumi_npr_y1216_pt6530->Draw();
    lumi_npr_y1624_pt6530->Draw();
    f4->Draw("same");
  }
  
  lat->SetTextSize(ltxSetTextSize1);
  lat->SetTextFont(22);
  lat->DrawLatex(ltxText_xStart,ltxText_yStart,"Non-prompt J/#psi");
  lat->SetTextFont(42);
  lat->SetTextSize(ltxSetTextSize2);
  lat->DrawLatex(ltxText_xDown,ltxText_yDown,"6.5 < p_{T} < 30 GeV/c");
  CMS_lumi(c21a,12014000,0);

  gNonPrJpsiSyst_pt6530y012->Draw("2");
  gNonPrJpsiSyst_pt6530y1216->Draw("2");
  gNonPrJpsiSyst_pt6530y1624->Draw("2");
   
  gNonPrJpsi_pt6530y1216->Draw("P");
  gNonPrJpsi_pt6530y012->Draw("P");
  gNonPrJpsi_pt6530y1624->Draw("P");
 
  gNonPrJpsiP_pt6530y1216->Draw("P");
  gNonPrJpsiP_pt6530y012->Draw("P");
  gNonPrJpsiP_pt6530y1624->Draw("P");

  //----------------------
  TLegend *leg11b = new TLegend(leg2DRaa_xStart,leg2DRaaY_yStart,leg2DRaa_xEnd,leg2DRaaY_yEnd);
  leg11b->SetBorderSize(0);
  leg11b->SetTextFont(22);
  leg11b->SetTextSize(ltxSetTextSize2);
  leg11b->SetLineColor(1);
  leg11b->SetLineStyle(1);
  leg11b->SetLineWidth(1);
  leg11b->SetFillColor(19);
  leg11b->SetFillStyle(0);
  
  TLegendEntry *leg21a1 = leg11b->AddEntry("gNonPrJpsi_pt6530y012","|y|<1.2","P");
  leg21a1->SetTextColor(kAzure+7);
  leg21a1->SetFillStyle(1001);
  leg21a1->SetFillColor(kAzure-9);
  leg21a1->SetMarkerStyle(20);
  leg21a1->SetMarkerColor(kAzure+2);
  leg21a1->SetMarkerSize(1.2);


  TLegendEntry *leg21a2 = leg11b->AddEntry("gNonPrJpsi_pt6530y1216","1.2<|y|<1.6","P");
  leg21a2->SetTextColor(kRed);
  leg21a2->SetFillStyle(1001);
  leg21a2->SetFillColor(kRed-9);
  leg21a2->SetMarkerStyle(21);
  leg21a2->SetMarkerColor(kRed);
  leg21a2->SetMarkerSize(1.2);

  TLegendEntry *leg21a3 = leg11b->AddEntry("gNonPrJpsi_pt6530y1624","1.6<|y|<2.4","P");
  leg21a3->SetTextColor(kGreen+2);
  leg21a3->SetFillStyle(1001);
  leg21a3->SetFillColor(kGreen-9);
  leg21a3->SetMarkerStyle(33);
  leg21a3->SetMarkerColor(kGreen+2);
  leg21a3->SetMarkerSize(1.5);

  gPad->RedrawAxis();
  leg11b->Draw();
  c21a->Update();
  
  // pT dependence
  TCanvas *c21b = new TCanvas("c21b","c21b");
  f4->Draw();
  if(bAddLumi)
  {
    lumi_npr_y1624_pt6530_pty->Draw();
    lumi_npr_y1624_pt365->Draw();
    f4->Draw("same");
  }
  CMS_lumi(c21b,12014000,0);
  lat->SetTextSize(ltxSetTextSize1);
  lat->SetTextFont(22);
  lat->DrawLatex(ltxText_xStart,ltxText_yStart,"Non-prompt J/#psi");
  lat->SetTextFont(42);
  lat->SetTextSize(ltxSetTextSize2);
  lat->DrawLatex(ltxText_xDown,ltxText_yDown,"1.6 < |y| < 2.4");
 
  
  gNonPrJpsiSyst_pt6530y1624->Draw("2");
  gNonPrJpsiSyst_pt365y1624->Draw("2");
    
  gNonPrJpsi_pt6530y1624->Draw("P");
  gNonPrJpsi_pt365y1624->Draw("P");

  gNonPrJpsiP_pt6530y1624->Draw("P");
  gNonPrJpsiP_pt365y1624->Draw("P");

  //------------
  TLegend *leg22b = new TLegend(leg2DRaa_xStart,leg2DRaaPt_yStart,leg2DRaa_xEnd,leg2DRaaPt_yEnd);
  leg22b->SetBorderSize(0);
  leg22b->SetTextFont(22);
  leg22b->SetTextSize(ltxSetTextSize2);
  leg22b->SetLineColor(1);
  leg22b->SetLineStyle(1);
  leg22b->SetLineWidth(1);
  leg22b->SetFillColor(19);
  leg22b->SetFillStyle(0);
  TLegendEntry *leg22b2 = leg22b->AddEntry("gNonPrJpsi_pt365y1624", "3<p_{T}<6.5 GeV/c","P");
  leg22b2->SetTextColor(kViolet+2);
  leg22b2->SetFillStyle(1001);
  leg22b2->SetFillColor(kViolet-9);
  leg22b2->SetMarkerStyle(34);
  leg22b2->SetMarkerColor(kViolet+2);
  leg22b2->SetMarkerSize(1.7);
  
  TLegendEntry *leg22b3 = leg22b->AddEntry("gNonPrJpsi_pt6530y1624","6.5<p_{T}<30 GeV/c","P");
  leg22b3->SetTextColor(kGreen+2);
  leg22b3->SetFillStyle(1001);
  leg22b3->SetFillColor(kGreen-9);
  leg22b3->SetMarkerStyle(33);
  leg22b3->SetMarkerColor(kGreen+2);
  leg22b3->SetMarkerSize(2);

  leg22b->Draw();
  gPad->RedrawAxis();
  c21b->Update();
  
  if(bSavePlots)
  {
    c21a->SaveAs(Form("%s/pdf/NonPrJpsi_vsCent_yHighPt_%s.pdf",outputDir,sample[whichSample]));
    c21a->SaveAs(Form("%s/png/NonPrJpsi_vsCent_yHighPt_%s.png",outputDir,sample[whichSample]));
    c21a->SaveAs(Form("%s/NonPrJpsi_vsCent_yHighPt_%s.root",outputDir,sample[whichSample]));
    
    c21b->SaveAs(Form("%s/pdf/NonPrJpsi_vsCent_yLowPt_%s.pdf",outputDir,sample[whichSample]));
    c21b->SaveAs(Form("%s/png/NonPrJpsi_vsCent_yLowPt_%s.png",outputDir,sample[whichSample]));
    c21b->SaveAs(Form("%s/NonPrJpsi_vsCent_yLowPt_%s.root",outputDir,sample[whichSample]));
  }
  
  //==========================================================
  if(bSaveRoot)
    {
      TFile *pfOutput = new TFile(Form("%s/makeRaa_cent.root",outputRootDir),"RECREATE");

      lumi_pr_y024_pt6530->Write("lumi_pr_y024_pt6530");
      lumi_pr_y012_pt6530->Write("lumi_pr_y012_pt6530");
      lumi_pr_y1216_pt6530->Write("lumi_pr_y1216_pt6530");
      lumi_pr_y1624_pt6530->Write("lumi_pr_y1624_pt6530");
    
      lumi_pr_y1624_pt6530_pty->Write("lumi_pr_y1624_pt6530_pty");
      lumi_pr_y1624_pt365->Write("lumi_pr_y1624_pt365");

      lumi_npr_y024_pt6530->Write("lumi_npr_y024_pt6530");
	  
      lumi_npr_y012_pt6530->Write("lumi_npr_y012_pt6530");
      lumi_npr_y1216_pt6530->Write("lumi_npr_y1216_pt6530");
      lumi_npr_y1624_pt6530->Write("lumi_npr_y1624_pt6530");
    
      lumi_npr_y1624_pt6530_pty->Write("lumi_npr_y1624_pt6530_pty");
      lumi_npr_y1624_pt365->Write("lumi_npr_y1624_pt365");
    
      // PROMPT
      gPrJpsiSyst->Write("gPrJpsiSyst");
      gPrJpsi->Write("gPrJpsi");
      gPrJpsiP->Write("gPrJpsiP");
  
      gPrJpsiSyst_pt6530y1216->Write("gPrJpsiSyst_pt6530y1216");
      gPrJpsiSyst_pt6530y012->Write("gPrJpsiSyst_pt6530y012");
      gPrJpsiSyst_pt6530y1624->Write("gPrJpsiSyst_pt6530y1624");
      
      gPrJpsi_pt6530y012->Write("gPrJpsi_pt6530y012");
      gPrJpsi_pt6530y1216->Write("gPrJpsi_pt6530y1216");
      gPrJpsi_pt6530y1624->Write("gPrJpsi_pt6530y1624");
      
      gPrJpsiP_pt6530y012->Write("gPrJpsiP_pt6530y012");
      gPrJpsiP_pt6530y1216->Write("gPrJpsiP_pt6530y1216");
      gPrJpsiP_pt6530y1624->Write("gPrJpsiP_pt6530y1624");
      
      gPrJpsiSyst_pt365y1624->Write("gPrJpsiSyst_pt365y1624");
      gPrJpsiSyst_pt6530y1624->Write("gPrJpsiSyst_pt6530y1624");
      
      gPrJpsi_pt6530y1624->Write("gPrJpsi_pt6530y1624");
      gPrJpsi_pt365y1624->Write("gPrJpsi_pt365y1624");
      
      gPrJpsiP_pt6530y1624->Write("gPrJpsiP_pt6530y1624");
      gPrJpsiP_pt365y1624->Write("gPrJpsiP_pt365y1624");
  
      //NONPROMPT
      gNonPrJpsiSyst->Write("gNonPrJpsiSyst");
      gNonPrJpsi->Write("gNonPrJpsi");
      gNonPrJpsiP->Write("gNonPrJpsiP");
  
      gNonPrJpsiSyst_pt6530y012->Write("gNonPrJpsiSyst_pt6530y012");
      gNonPrJpsiSyst_pt6530y1216->Write("gNonPrJpsiSyst_pt6530y1216");
      gNonPrJpsiSyst_pt6530y1624->Write("gNonPrJpsiSyst_pt6530y1624");
      
      gNonPrJpsi_pt6530y1216->Write("gNonPrJpsi_pt6530y1216");
      gNonPrJpsi_pt6530y012->Write("gNonPrJpsi_pt6530y012");
      gNonPrJpsi_pt6530y1624->Write("gNonPrJpsi_pt6530y1624");
      
      gNonPrJpsiP_pt6530y1216->Write("gNonPrJpsiP_pt6530y1216");
      gNonPrJpsiP_pt6530y012->Write("gNonPrJpsiP_pt6530y012");
      gNonPrJpsiP_pt6530y1624->Write("gNonPrJpsiP_pt6530y1624");
      
      gNonPrJpsiSyst_pt6530y1624->Write("gNonPrJpsiSyst_pt6530y1624");
      gNonPrJpsiSyst_pt365y1624->Write("gNonPrJpsiSyst_pt365y1624");
    
      gNonPrJpsi_pt6530y1624->Write("gNonPrJpsi_pt6530y1624");
      gNonPrJpsi_pt365y1624->Write("gNonPrJpsi_pt365y1624");

      gNonPrJpsiP_pt6530y1624->Write("gNonPrJpsiP_pt6530y1624");
      gNonPrJpsiP_pt365y1624->Write("gNonPrJpsiP_pt365y1624");

      pfOutput->Write(); 
  
    }
  
  systFile->Close();
}
