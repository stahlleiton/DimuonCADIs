/*
Macro to plot the Efficiency vs centrality, for all the analysis bins.(in the traditional way, yield*correction, and the weighted way)

Input: root files produced by the readFitTable/makeHisto_raa.C (check there the input file names)

Output: the Raa vs pt.

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
#include "TLegendEntry.h"

#include "dataBinning_2015.h"
#include "filesRaa_2015.h"

#include "../CMS_lumi.C"
#include "../tdrstyle.C"
#include "../textPosition.h"

#endif

void makeRaa_pt( bool bSavePlots           = 1,
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
 
  // type of available comparisons:
  const char* sample[4] = {"noTnP","dataTnP","mcTnP","lxyTnP"};

  const unsigned int nInHist = 4;
  const char* yieldHistNames[nInHist] = {"pt", "ptLow", "ptLow_mb", "mb"};

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
    double scale_cent = 1/(adTaaMB[0]*adDeltaCentMB[0]);
  
    unsigned int numBins = 0;
    if(ih==0) numBins = nBinsPt;
    if(ih==1 || ih==2) numBins = nBinsPt3;
    if(ih==3) numBins = nBinsMB;

    for(unsigned int ibin=1; ibin<=numBins; ibin++)
    {
      cout << endl << "ih " << ih << ", ibin " << ibin << "/" << numBins << endl;
      
      double raa_pr=0, raaErr_pr=0, raa_npr=0, raaErr_npr=0;

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
    
      raa_npr    = yieldRatio_npr * scaleFactor * scale_cent;
      raaErr_npr = TMath::Sqrt(TMath::Power(dRelErrRaw_npr_pp,2)+TMath::Power(dRelErrRaw_npr_aa,2))*raa_npr;
     
      // fill the corresponding array
      switch(ih){
      case 0:
        prJpsi_pt[ibin-1]    = raa_pr;
        prJpsiErr_pt[ibin-1] = raaErr_pr;
              
        nonPrJpsi_pt[ibin-1]    = raa_npr;
        nonPrJpsiErr_pt[ibin-1] = raaErr_npr;
        break;

      case 1:
        prJpsi_pt365y1624_pt[ibin-1]        = raa_pr;
        prJpsiErr_pt365y1624_pt[ibin-1]     = raaErr_pr;

        nonPrJpsi_pt365y1624_pt[ibin-1]     = raa_npr;
        nonPrJpsiErr_pt365y1624_pt[ibin-1]  = raaErr_npr;
        break;

      case 2:
        prJpsi_y1624MB_pt[ibin-1]        = raa_pr;
        prJpsiErr_y1624MB_pt[ibin-1]     = raaErr_pr;

        nonPrJpsi_y1624MB_pt[ibin-1]     = raa_npr;
        nonPrJpsiErr_y1624MB_pt[ibin-1]  = raaErr_npr;

        if(bDoDebug)
        {
          cout<<"yield_npr_aa "<<phCorr_npr_aa->GetBinContent(ibin)<<"\t yield_pr_pp "<<phCorr_npr_pp->GetBinContent(ibin)<<endl;
          cout<<setprecision(2);
          cout<<"!!!!! raa_pr = "<<raa_pr<<"\t raa_npr= "<<raa_npr<<endl;
        }
        break;
      case 3:
        // mb
        prJpsi_mb[0]     = raa_pr;
        prJpsiErr_mb[0]  = raaErr_pr;
        
        nonPrJpsi_mb[0]     = raa_npr;
        nonPrJpsiErr_mb[0]  = raaErr_npr;
        break;
      }
    }//loop end: for(int ibin=1; ibin<=numBins; ibin++)
  }//loop end: for(int ih=0; ih<nInHist;ih++)

  // LOADING SYSTEMATICS
  TFile *systFile = new TFile("../calcSyst/histSyst/raaSystUncert_pt.root","read");

  // ***** //Drawing
  // pr
  TGraphErrors *gPrJpsi                   = new TGraphErrors(nBinsPt, binsPt, prJpsi_pt, binsPtErr, prJpsiErr_pt);
  TGraphErrors *gPrJpsiP                  = new TGraphErrors(nBinsPt, binsPt, prJpsi_pt, binsPtErr, prJpsiErr_pt);
  TGraphErrors *gPrJpsiSyst               = (TGraphErrors *)systFile->Get("gPrJpsiSyst");

  TGraphErrors *gPrJpsi_mb                = new TGraphErrors(nBinsMB, binsPtMB, prJpsi_pt, binsPtMBErr, prJpsiErr_pt);
  TGraphErrors *gPrJpsiP_mb               = new TGraphErrors(nBinsMB, binsPtMB, prJpsi_pt, binsPtMBErr, prJpsiErr_pt);
  TGraphErrors *gPrJpsiSyst_mb            = new TGraphErrors(nBinsMB, binsPtMB, prJpsi_pt, binsPtMBX  , prJpsiErrSyst_pt);
 
  TGraphErrors *gPrJpsi_y1624_mb          = new TGraphErrors(nBinsPt3, binsPt3_mb, prJpsi_y1624MB_pt, binsPt3Err_mb, prJpsiErr_y1624MB_pt);
  TGraphErrors *gPrJpsiP_y1624_mb         = new TGraphErrors(nBinsPt3, binsPt3_mb, prJpsi_y1624MB_pt, binsPt3Err_mb, prJpsiErr_y1624MB_pt);
  TGraphErrors *gPrJpsiSyst_y1624_mb      = new TGraphErrors(nBinsPt3, binsPt3_mb, prJpsi_y1624MB_pt, binsPt3X_mb,   prJpsiErrSyst_y1624MB_pt);

  TGraphErrors *gPrJpsi_pt365y1624        = new TGraphErrors(nBinsPt3, binsPt3, prJpsi_pt365y1624_pt, binsPt3Err, prJpsiErr_pt365y1624_pt);
  TGraphErrors *gPrJpsiP_pt365y1624       = new TGraphErrors(nBinsPt3, binsPt3, prJpsi_pt365y1624_pt, binsPt3Err, prJpsiErr_pt365y1624_pt);
  TGraphErrors *gPrJpsiSyst_pt365y1624    = (TGraphErrors*)systFile->Get("gPrJpsiSyst_pt365y1624");
 
  // nonPr   
  TGraphErrors *gNonPrJpsi                = new TGraphErrors(nBinsPt, binsPt, nonPrJpsi_pt, binsPtErr, nonPrJpsiErr_pt);
  TGraphErrors *gNonPrJpsiP               = new TGraphErrors(nBinsPt, binsPt, nonPrJpsi_pt, binsPtErr, nonPrJpsiErr_pt);
  TGraphErrors *gNonPrJpsiSyst            = (TGraphErrors*)systFile->Get("gNonPrJpsiSyst");

  TGraphErrors *gNonPrJpsi_mb             = new TGraphErrors(nBinsMB, binsPtMB, nonPrJpsi_pt, binsPtMBErr, nonPrJpsiErr_pt);
  TGraphErrors *gNonPrJpsiP_mb            = new TGraphErrors(nBinsMB, binsPtMB, nonPrJpsi_pt, binsPtMBErr, nonPrJpsiErr_pt);
  TGraphErrors *gNonPrJpsiSyst_mb         = new TGraphErrors(nBinsMB, binsPtMB, nonPrJpsi_pt, binsPtMBX, nonPrJpsiErrSyst_pt);

  TGraphErrors *gNonPrJpsi_y1624_mb       = new TGraphErrors(nBinsPt3, binsPt3_mb, nonPrJpsi_y1624MB_pt, binsPt3Err_mb, nonPrJpsiErr_y1624MB_pt);
  TGraphErrors *gNonPrJpsiP_y1624_mb      = new TGraphErrors(nBinsPt3, binsPt3_mb, nonPrJpsi_y1624MB_pt, binsPt3Err_mb, nonPrJpsiErr_y1624MB_pt);
  TGraphErrors *gNonPrJpsiSyst_y1624_mb   = new TGraphErrors(nBinsPt3, binsPt3_mb, nonPrJpsi_y1624MB_pt, binsPt3X_mb,   nonPrJpsiErrSyst_y1624MB_pt);

  TGraphErrors *gNonPrJpsi_pt365y1624     = new TGraphErrors(nBinsPt3, binsPt3, nonPrJpsi_pt365y1624_pt, binsPt3Err, nonPrJpsiErr_pt365y1624_pt);
  TGraphErrors *gNonPrJpsiP_pt365y1624    = new TGraphErrors(nBinsPt3, binsPt3, nonPrJpsi_pt365y1624_pt, binsPt3Err, nonPrJpsiErr_pt365y1624_pt);
  TGraphErrors *gNonPrJpsiSyst_pt365y1624 = (TGraphErrors*)systFile->Get("gNonPrJpsiSyst_pt365y1624");

  //-------------------------------------------------------------------
  // **************** marker colors
  //prompt
  gPrJpsi->SetMarkerColor(kRed);
  gPrJpsi_pt365y1624->SetMarkerColor(kViolet+2);
    
  // non-prompt
  gNonPrJpsi->SetMarkerColor(kOrange+2);
  gNonPrJpsi_pt365y1624->SetMarkerColor(kViolet+2);

  //mnbias colors
  gPrJpsi_mb->SetMarkerColor(kCyan+2);
  gNonPrJpsi_mb->SetMarkerColor(kCyan+2);

  gPrJpsi_y1624_mb->SetMarkerColor(kBlue-4);
  gNonPrJpsi_y1624_mb->SetMarkerColor(kBlue-4);

  //--------- marker style  
  // pr
  gPrJpsi->SetMarkerStyle(21);
  gPrJpsiP->SetMarkerStyle(25);

  gPrJpsi_pt365y1624->SetMarkerStyle(34);
  gPrJpsiP_pt365y1624->SetMarkerStyle(28);

  // non-pr
  gNonPrJpsi->SetMarkerStyle(29);
  gNonPrJpsiP->SetMarkerStyle(30);
  
  gNonPrJpsi_pt365y1624->SetMarkerStyle(34);
  gNonPrJpsiP_pt365y1624->SetMarkerStyle(28);

  //mb
  gPrJpsi_mb->SetMarkerStyle(28);
  gPrJpsi_y1624_mb->SetMarkerStyle(21);   
  
  gNonPrJpsi_mb->SetMarkerStyle(28);
  gNonPrJpsi_y1624_mb->SetMarkerStyle(21);


  // ************** contour
  // pr
  gPrJpsiP->SetMarkerColor(kBlack);
  gPrJpsiP_pt365y1624->SetMarkerColor(kBlack);

  // nonPr
  gNonPrJpsiP->SetMarkerColor(kBlack);
  gNonPrJpsiP_pt365y1624->SetMarkerColor(kBlack);


  // marker size
  // pr
  gPrJpsi->SetMarkerSize(1.2);
  gPrJpsiP->SetMarkerSize(1.2);

  gPrJpsiP_pt365y1624->SetMarkerSize(1.7);
  gPrJpsi_pt365y1624->SetMarkerSize(1.7);

  gPrJpsi_mb->SetMarkerSize(1.5);
  gNonPrJpsi_y1624_mb->SetMarkerSize(1.5);

  // nonPr
  gNonPrJpsi->SetMarkerSize(2.0);
  gNonPrJpsiP->SetMarkerSize(2.0);
  
  gNonPrJpsiP_pt365y1624->SetMarkerSize(1.7);
  gNonPrJpsi_pt365y1624->SetMarkerSize(1.7);


  //stat boxes
  gPrJpsiSyst->SetFillColorAlpha(kRed-9,0.5);
  gPrJpsiSyst_pt365y1624->SetFillColorAlpha(kViolet-9,0.5);

  // non-pr
  gNonPrJpsiSyst->SetFillColorAlpha(kOrange-9,0.5);
  gNonPrJpsiSyst_pt365y1624->SetFillColorAlpha(kViolet-9,0.5);

  //mb
  // gPrJpsiP_mb->SetFillColor(kBlue-9);
  // gNonPrJpsiP_y1624_mb->SetFillColor(kBlue-9);
  //-------------------------------------------
  TF1 *f4 = new TF1("f4","1",0,30);
  f4->SetLineWidth(1);
  f4->SetLineColor(1);
  f4->SetLineStyle(1);
  f4->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  f4->GetYaxis()->SetTitle("R_{AA}");
  f4->GetYaxis()->SetRangeUser(0.0,1.5);
  f4->GetXaxis()->CenterTitle(kTRUE);

  TBox *lumi = (TBox*)systFile->Get("lumi");

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
    lumi->Draw();
    f4->Draw("same");
  }
 
  CMS_lumi(c1,12014000,0);
  lat->SetTextSize(ltxSetTextSize1);
  lat->SetTextFont(22);
  lat->DrawLatex(ltxText_xStart,ltxText_yStart,"Prompt J/#psi");
 
  lat->SetTextFont(42);
  lat->SetTextSize(ltxSetTextSize2);
  lat->DrawLatex(ltxCentOnly_x,ltxCentOnly_y,"Cent. 0-100%");

  
  gPrJpsiSyst->Draw("2");
  gPrJpsi->Draw("P");
  gPrJpsiP->Draw("P");

  gPrJpsiSyst_pt365y1624->Draw("2");
  gPrJpsi_pt365y1624->Draw("P");
  gPrJpsiP_pt365y1624->Draw("P");

  //------ the legend
  TLegend *leg11a = new TLegend(legRaaPt_xLowStart,legRaaPt_y,legRaaPt_xLowEnd,legRaaPt_y,NULL,"brNDC");
  leg11a->SetBorderSize(0);
  leg11a->SetTextFont(22);
  leg11a->SetTextSize(ltxSetTextSize2);
  leg11a->SetTextColor(kViolet+2);
  leg11a->SetLineColor(1);
  leg11a->SetLineStyle(1);
  leg11a->SetLineWidth(1);
  leg11a->SetFillColor(19);
  leg11a->SetFillStyle(0);
  TLegendEntry *entry1 = leg11a->AddEntry("gPrJpsi_pt365y1624","1.6 < |y| < 2.4","Pf");
  entry1->SetMarkerStyle(34);
  entry1->SetMarkerColor(kViolet+2);
  entry1->SetMarkerSize(1.7);
  entry1->SetFillStyle(1001);
  entry1->SetFillColor(kViolet-9);
 
  TLegend *leg11a_1 = new TLegend(legRaaPt_xHighStart,legRaaPt_y,legRaaPt_xHighEnd,legRaaPt_y,NULL,"brNDC");
  leg11a_1->SetBorderSize(0);
  leg11a_1->SetTextFont(22);
  leg11a_1->SetTextSize(ltxSetTextSize2);
  leg11a_1->SetTextColor(kRed+2);

  leg11a_1->SetLineColor(1);
  leg11a_1->SetLineStyle(1);
  leg11a_1->SetLineWidth(1);
  leg11a_1->SetFillColor(19);
  leg11a_1->SetFillStyle(0);
  TLegendEntry *entry2 = leg11a_1->AddEntry("gPrJpsi","|y|<2.4","Pf");
  entry2->SetMarkerStyle(21);
  entry2->SetMarkerColor(kRed+2);
  entry2->SetMarkerSize(1.2);
  entry2->SetFillStyle(1001);
  entry2->SetFillColor(kRed-9);
  
  leg11a->Draw();
  leg11a_1->Draw();
  c1->Update();
  if(bSavePlots)
  {
    c1->SaveAs(Form("%s/pdf/PrJpsi_vsPt_%s.pdf",outputDir,sample[whichSample]));
    c1->SaveAs(Form("%s/png/PrJpsi_vsPt_%s.png",outputDir,sample[whichSample]));
  }

  //-------------------minbias dependence
  TCanvas *c11b = new TCanvas("c11b","c11b");
  f4->Draw();
  if(bAddLumi)
  {
    lumi->Draw();
    f4->Draw("same");
  }
  CMS_lumi(c11b,12014000,0);
  lat->SetTextSize(ltxSetTextSize1);
  lat->SetTextFont(22);
  lat->DrawLatex(ltxText_xStart,ltxText_yStart,"Prompt J/#psi");
 
  lat->SetTextFont(42);
  lat->SetTextSize(ltxSetTextSize2);
  lat->DrawLatex(ltxCentOnly_x,ltxCentOnly_y,"Cent. 0-100%");

  
  gPrJpsiSyst_mb->Draw("2");
  gPrJpsi_mb->Draw("P");
  gPrJpsiSyst_y1624_mb->Draw("2");
  gPrJpsi_y1624_mb->Draw("P");

  TLegend *leg11b = new TLegend(0.65,0.5,0.8,0.63);
  leg11b->SetFillStyle(0);
  leg11b->SetFillColor(0);
  leg11b->SetBorderSize(0);
  leg11b->SetMargin(0.2);
  leg11b->SetTextSize(ltxSetTextSize2);
  
  leg11b->AddEntry(gPrJpsi_mb,"|y|<2.4","P");
  leg11b->AddEntry(gPrJpsi_y1624_mb,"1.6<|y|<2.4","P");
  leg11b->Draw();
  gPad->RedrawAxis();

  c11b->Update();
  if(bSavePlots)
  {
    c11b->SaveAs(Form("%s/pdf/PrJpsi_vsPt_mb_%s.pdf",outputDir,sample[whichSample]));
    c11b->SaveAs(Form("%s/png/PrJpsi_vsPt_mb_%s.png",outputDir,sample[whichSample]));
  }
  
  //   // ############################################## non-pr 
  //   // ############################################## non-pr
  //   // ############################################## non-pr
  TCanvas *c2 = new TCanvas("c2","c2");
  // general stuff
  f4->Draw();// axis
  if(bAddLumi) 
  {
    lumi->Draw();
    f4->Draw("same");
  }
 
  CMS_lumi(c2,12014000,0);
  lat->SetTextSize(ltxSetTextSize1);
  lat->SetTextFont(22);
  lat->DrawLatex(ltxText_xStart,ltxText_yStart,"Non-prompt J/#psi");
 
  lat->SetTextFont(42);
  lat->SetTextSize(ltxSetTextSize2);
  lat->DrawLatex(ltxCentOnly_x,ltxCentOnly_y,"Cent. 0-100%");


  gNonPrJpsiSyst->Draw("2");
  gNonPrJpsi->Draw("P");
  gNonPrJpsiP->Draw("P");

  gNonPrJpsiSyst_pt365y1624->Draw("2");
  gNonPrJpsi_pt365y1624->Draw("P");
  gNonPrJpsiP_pt365y1624->Draw("P");

  //-------- legends
   TLegend *leg22a = new TLegend(legRaaPt_xLowStart,legRaaPt_y,legRaaPt_xLowEnd,legRaaPt_y,NULL,"brNDC");
  leg22a->SetBorderSize(0);
  leg22a->SetTextFont(22);
  leg22a->SetTextSize(ltxSetTextSize2);
  leg22a->SetTextColor(kViolet+2);
  leg22a->SetLineColor(1);
  leg22a->SetLineStyle(1);
  leg22a->SetLineWidth(1);
  leg22a->SetFillColor(19);
  leg22a->SetFillStyle(0);
  TLegendEntry *entry3 = leg22a->AddEntry("gNonPrJpsi_pt365y1624","1.6 < |y| < 2.4","Pf");
  entry3->SetMarkerStyle(34);
  entry3->SetMarkerColor(kViolet+2);
  entry3->SetMarkerSize(1.7);
  entry3->SetFillStyle(1001);
  entry3->SetFillColor(kViolet-9);
 
  TLegend *leg22a_1 = new TLegend(legRaaPt_xHighStart,legRaaPt_y,legRaaPt_xHighEnd,legRaaPt_y,NULL,"brNDC");
  leg22a_1->SetBorderSize(0);
  leg22a_1->SetTextFont(22);
  leg22a_1->SetTextSize(ltxSetTextSize2);
  leg22a_1->SetTextColor(kOrange+2);

  leg22a_1->SetLineColor(1);
  leg22a_1->SetLineStyle(1);
  leg22a_1->SetLineWidth(1);
  leg22a_1->SetFillColor(19);
  leg22a_1->SetFillStyle(0);
  TLegendEntry *entry4 = leg22a_1->AddEntry("gNonPrJpsi","|y|<2.4","Pf");
  entry4->SetMarkerStyle(34);
  entry4->SetMarkerColor(kOrange+2);
  entry4->SetMarkerSize(1.7);
  entry4->SetFillStyle(1001);
  entry4->SetFillColor(kOrange-9);

  leg22a->Draw();
  leg22a_1->Draw();
  gPad->RedrawAxis();
  c2->Update();
  //-----------------------

  if(bSavePlots)
  {
    c2->SaveAs(Form("%s/pdf/nonPrJpsi_vsPt_%s.pdf",outputDir,sample[whichSample]));
    c2->SaveAs(Form("%s/png/nonPrJpsi_vsPt_%s.png",outputDir,sample[whichSample]));
  }
 
  //-------------------minbias dependence
  TCanvas *c22b = new TCanvas("c22b","c22b");
  f4->Draw();
  if(bAddLumi)
  {
    lumi->Draw();
    f4->Draw("same");
  }
  CMS_lumi(c22b,12014000,0);
 lat->SetTextSize(ltxSetTextSize1);
  lat->SetTextFont(22);
  lat->DrawLatex(ltxText_xStart,ltxText_yStart,"Non-rompt J/#psi");
 
  lat->SetTextFont(42);
  lat->SetTextSize(ltxSetTextSize2);
  lat->DrawLatex(ltxCentOnly_x,ltxCentOnly_y,"#splitline{Cent. 0-100%}{ }");

   
  gNonPrJpsiSyst_mb->Draw("2");
  gNonPrJpsi_mb->Draw("P");
  gNonPrJpsiSyst_y1624_mb->Draw("2");
  gNonPrJpsi_y1624_mb->Draw("P");

  TLegend *leg22b = new TLegend(0.65,0.52,0.8,0.65);
  leg22b->SetFillStyle(0);
  leg22b->SetFillColor(0);
  leg22b->SetBorderSize(0);
  leg22b->SetMargin(0.2);
  leg22b->SetTextSize(ltxSetTextSize2);
  leg22b->AddEntry(gNonPrJpsi_mb,"|y|<2.4","P");
  leg22b->AddEntry(gNonPrJpsi_y1624_mb,"1.6<|y|<2.4","P");
  leg22b->Draw();
    
  gPad->RedrawAxis();
  c22b->Update();
  if(bSavePlots)
  {
    c22b->SaveAs(Form("%s/pdf/nonPrJpsi_vsPt_mb_%s.pdf",outputDir,sample[whichSample]));
    c22b->SaveAs(Form("%s/png/nonPrJpsi_vsPt_mb_%s.png",outputDir,sample[whichSample]));
  }
  
  systFile->Close();
  //----------------------------------------------------
  if(bSaveRoot)
    {
      TFile *pfOutput = new TFile(Form("%s/makeRaa_pt.root",outputRootDir),"RECREATE");

      lumi->Write("lumi");
      // PROMPT
      //integrated values
      gPrJpsiSyst_mb->Write("gPrJpsiSyst_mb");
      gPrJpsi_mb->Write("gPrJpsi_mb");
      gPrJpsiSyst_y1624_mb->Write("gPrJpsiSyst_y1624_mb");
      gPrJpsi_y1624_mb->Write("gPrJpsi_y1624_mb");

      // pt dependence
      gPrJpsiSyst->Write("gPrJpsiSyst");
      gPrJpsi->Write("gPrJpsi");
      gPrJpsiP->Write("gPrJpsiP");

      gPrJpsiSyst_pt365y1624->Write("gPrJpsiSyst_pt365y1624");
      gPrJpsi_pt365y1624->Write("gPrJpsi_pt365y1624");
      gPrJpsiP_pt365y1624->Write("gPrJpsiP_pt365y1624");

      //NONPROMPT
      gNonPrJpsiSyst->Write("gNonPrJpsiSyst");
      gNonPrJpsi->Write("gNonPrJpsi");
      gNonPrJpsiP->Write("gNonPrJpsiP");

      gNonPrJpsiSyst_pt365y1624->Write("gNonPrJpsiSyst_pt365y1624");
      gNonPrJpsi_pt365y1624->Write("gNonPrJpsi_pt365y1624");
      gNonPrJpsiP_pt365y1624->Write("gNonPrJpsiP_pt365y1624");
      
      gNonPrJpsiSyst_mb->Write("gNonPrJpsiSyst_mb");
      gNonPrJpsi_mb->Write("gNonPrJpsi_mb");
 
      gNonPrJpsiSyst_y1624_mb->Write("gNonPrJpsiSyst_y1624_mb");
      gNonPrJpsi_y1624_mb->Write("gNonPrJpsi_y1624_mb");

      pfOutput->Write(); 
  
    }
  

}
