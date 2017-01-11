/*
Macro to calcualte the systm uncertainties for the Raa vs centrality: bin-by-bin and global uncert.
-- bin-to-bin: aa and pp systm, taa
-- global: lumi and hf gating and counting (each contribution in the numbersRaaSyst_2015.h file)

Input: root files produced by the readFitTable/makeHisto_raa_syst.C (check there the input file names)

Output: root file with the systm. histograms for Raa vs pT.
*/

#if !defined(__CINT__) || defined(__MAKECINT__)
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

#include "TROOT.h"
#include "TStyle.h"
#include "TSystem.h"

#include "TBox.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TFile.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
#include "TH1.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TMath.h"

#include "../macro_raa/dataBinning_2015.h"
#include "../macro_raa/filesRaa_2015.h"
#include "numbersRaaSyst_2015.h"

#include "../CMS_lumi.C"
#include "../tdrstyle.C"
#endif

void makeSyst_cent( bool bSavePlots     = 1,
                    bool bDoDebug         = 1, // prints numbers, numerator, denominator, to help figure out if things are read properly
                    int method            = 2, // 0: nominal (rms of same category variations)&&added in quadrature with non-correlated sourcesvariations; 1: max of each variation type, added in quadrature, 2: same as 0 but quadrature in fit variations
                    int systBoxType       = 0, // 0: (systLumi+systSelection+systTrack) box is separated ONLY in 2D raa plots, 1: (systLumi+systSelection+systTrack) box is separated into grey box from pp uncertainty, 2: (add all global uncertainty into 1 box)
                    const char* inputDir  = "../readFitTable", // the place where the input root files, with the histograms are
                    const char* outputDir = "histSyst")// where the output figures will be
{
  gSystem->mkdir(Form("./%s/",outputDir), kTRUE);
  gSystem->mkdir(Form("./%s/data",outputDir), kTRUE);// numbers into txt files
    
  // set the style
  setTDRStyle();
 
  // type of available comparisons:
  const char* system[2] = {"aa", "pp"};

  const int nInHist = 5;
  const char* yieldHistNames[nInHist] = {"cent", "y1624LowPtCent","y012Cent", "y1216Cent", "y1624Cent"};

  // input files: are in the filesRaa_2015.h
  // open the files with yields and do the math
  // nominal yields
  TFile *fYesWeighFile_aa   = new TFile(Form("%s/%s",inputDir,yieldHistFile_yesWeight_1[0]));
  TFile *fYesWeighFile_pp   = new TFile(Form("%s/%s",inputDir,yieldHistFile_yesWeight_1[1]));
  
  TFile *fNoWeighFile_aa = new TFile(Form("%s/%s",inputDir,yieldHistFile_noWeight_1[0]));
  TFile *fNoWeighFile_pp = new TFile(Form("%s/%s",inputDir,yieldHistFile_noWeight_1[1]));
  
  if (!fYesWeighFile_aa->IsOpen() || !fYesWeighFile_pp->IsOpen()) {
    cout << "One or more input files are missing" << endl;
    return ;
  }

  TH1F *phCorr_pr_pp;
  TH1F *phCorr_pr_aa;
  TH1F *phRaw_pr_pp;
  TH1F *phRaw_pr_aa;

  TH1F *phCorr_npr_pp;
  TH1F *phCorr_npr_aa;
  TH1F *phRaw_npr_pp;
  TH1F *phRaw_npr_aa;

  TH1F *phCorrVar_pr_pp;
  TH1F *phCorrVar_pr_aa;

  TH1F *phCorrVar_npr_pp;
  TH1F *phCorrVar_npr_aa;
 
  // For printing out numbers
  TH1F *phRaw_pr_pp_y024_pt6530_copy;
  TH1F *phRaw_pr_pp_y012_pt6530_copy;
  TH1F *phRaw_pr_pp_y1216_pt6530_copy;
  TH1F *phRaw_pr_pp_y1624_pt6530_copy;
  TH1F *phRaw_pr_pp_y1624_pt6530_pty_copy;
  TH1F *phRaw_pr_pp_y1624_pt365_copy;
  TH1F *phRaw_pr_aa_y024_pt6530_copy;
  TH1F *phRaw_pr_aa_y012_pt6530_copy;
  TH1F *phRaw_pr_aa_y1216_pt6530_copy;
  TH1F *phRaw_pr_aa_y1624_pt6530_copy;
  TH1F *phRaw_pr_aa_y1624_pt365_copy;

  TH1F *phRaw_npr_pp_y024_pt6530_copy;
  TH1F *phRaw_npr_pp_y012_pt6530_copy;
  TH1F *phRaw_npr_pp_y1216_pt6530_copy;
  TH1F *phRaw_npr_pp_y1624_pt6530_copy;
  TH1F *phRaw_npr_pp_y1624_pt365_copy;
  TH1F *phRaw_npr_aa_y024_pt6530_copy;
  TH1F *phRaw_npr_aa_y012_pt6530_copy;
  TH1F *phRaw_npr_aa_y1216_pt6530_copy;
  TH1F *phRaw_npr_aa_y1624_pt6530_copy;
  TH1F *phRaw_npr_aa_y1624_pt365_copy;

  TH1F *phCorr_pr_pp_y024_pt6530_copy;
  TH1F *phCorr_pr_pp_y012_pt6530_copy;
  TH1F *phCorr_pr_pp_y1216_pt6530_copy;
  TH1F *phCorr_pr_pp_y1624_pt6530_copy;
  TH1F *phCorr_pr_pp_y1624_pt365_copy;
  TH1F *phCorr_pr_aa_y024_pt6530_copy;
  TH1F *phCorr_pr_aa_y012_pt6530_copy;
  TH1F *phCorr_pr_aa_y1216_pt6530_copy;
  TH1F *phCorr_pr_aa_y1624_pt6530_copy;
  TH1F *phCorr_pr_aa_y1624_pt365_copy;

  TH1F *phCorr_npr_pp_y024_pt6530_copy;
  TH1F *phCorr_npr_pp_y012_pt6530_copy;
  TH1F *phCorr_npr_pp_y1216_pt6530_copy;
  TH1F *phCorr_npr_pp_y1624_pt6530_copy;
  TH1F *phCorr_npr_pp_y1624_pt365_copy;
  TH1F *phCorr_npr_aa_y024_pt6530_copy;
  TH1F *phCorr_npr_aa_y012_pt6530_copy;
  TH1F *phCorr_npr_aa_y1216_pt6530_copy;
  TH1F *phCorr_npr_aa_y1624_pt6530_copy;
  TH1F *phCorr_npr_aa_y1624_pt365_copy;
  
  // total uncertainty separately for pp and pbpb
  double systErrTotal_pr_pp_y024_pt6530_copy[100]={0};
  double systErrTotal_pr_pp_y012_pt6530_copy[100]={0};
  double systErrTotal_pr_pp_y1216_pt6530_copy[100]={0};
  double systErrTotal_pr_pp_y1624_pt6530_copy[100]={0};
  double systErrTotal_pr_pp_y1624_pt365_copy[100]={0};
  double systErrTotal_pr_aa_y024_pt6530_copy[100]={0};
  double systErrTotal_pr_aa_y012_pt6530_copy[100]={0};
  double systErrTotal_pr_aa_y1216_pt6530_copy[100]={0};
  double systErrTotal_pr_aa_y1624_pt6530_copy[100]={0};
  double systErrTotal_pr_aa_y1624_pt365_copy[100]={0};

  double systErrTotal_npr_pp_y024_pt6530_copy[100]={0};
  double systErrTotal_npr_pp_y012_pt6530_copy[100]={0};
  double systErrTotal_npr_pp_y1216_pt6530_copy[100]={0};
  double systErrTotal_npr_pp_y1624_pt6530_copy[100]={0};
  double systErrTotal_npr_pp_y1624_pt365_copy[100]={0};
  double systErrTotal_npr_aa_y024_pt6530_copy[100]={0};
  double systErrTotal_npr_aa_y012_pt6530_copy[100]={0};
  double systErrTotal_npr_aa_y1216_pt6530_copy[100]={0};
  double systErrTotal_npr_aa_y1624_pt6530_copy[100]={0};
  double systErrTotal_npr_aa_y1624_pt365_copy[100]={0};
  


  // If systBoxType==2, TBox lumi_* will have all global systematics. TBox globalpp_ will have nothing.
  // If systBoxType==0 or 1, TBox lumi_* will have lumi, evt sel, tracking systematics. TBox globalpp_ will have pp uncertainties.
  TBox *lumi_pr_y024_pt6530(0);
  TBox *lumi_pr_y012_pt6530(0);
  TBox *lumi_pr_y1216_pt6530(0);
  TBox *lumi_pr_y1624_pt6530(0);
  TBox *lumi_pr_y1624_pt6530_pty(0);
  TBox *lumi_pr_y1624_pt365(0);

  TBox *lumi_npr_y024_pt6530(0);
  TBox *lumi_npr_y012_pt6530(0);
  TBox *lumi_npr_y1216_pt6530(0);
  TBox *lumi_npr_y1624_pt6530(0);
  TBox *lumi_npr_y1624_pt6530_pty(0);
  TBox *lumi_npr_y1624_pt365(0);
  
  TBox *globalpp_pr_y024_pt6530(0);
  TBox *globalpp_pr_y012_pt6530(0);
  TBox *globalpp_pr_y1216_pt6530(0);
  TBox *globalpp_pr_y1624_pt6530(0);
  TBox *globalpp_pr_y1624_pt6530_pty(0);
  TBox *globalpp_pr_y1624_pt365(0);

  TBox *globalpp_npr_y024_pt6530(0);
  TBox *globalpp_npr_y012_pt6530(0);
  TBox *globalpp_npr_y1216_pt6530(0);
  TBox *globalpp_npr_y1624_pt6530(0);
  TBox *globalpp_npr_y1624_pt6530_pty(0);
  TBox *globalpp_npr_y1624_pt365(0);
  
  // Write systematics into a txt file
  string centbins_str[] = {"60100","5060","4550","4045","3540","3035","2530","2025","1520","1015","510","05"};
  string centbins_6bins_str[] = {"50100","4050","3040","2030","1020","010"};
  ofstream outputData_pr(Form("%s/data/raaSystUncert_cent_pr.dat",outputDir));
  if (!outputData_pr.good()) {cout << "######### Fail to open data/*.dat file.##################" << endl;}
  outputData_pr << "pT\t" << "rapidity\t" << "cent\t" << "Raa\t" << "Uncert_tot\t"
                << "contrib_IDtrg_stat_pp\t" << "contrib_IDtrg_syst_pp\t" << "contrib_IDtrg_pp\t"
                << "contrib_sta_stat_pp\t" << "contrib_sta_syst_pp\t" << "contrib_sta_pp\t"
                << "contrib_3d_pp\t" << "contrib_4d_pp\t" << "contrib_fit_pp\t" << "global_uncertainty_pp\t"
                << "contrib_IDtrg_stat_aa\t" << "contrib_IDtrg_syst_aa\t" << "contrib_IDtrg_aa\t"
                << "contrib_sta_stat_aa\t" << "contrib_sta_syst_aa\t" << "contrib_sta_aa\t"
                << "contrib_3d_aa\t" << "contrib_4d_aa\t" << "contrib_fit_aa\t" << "global_uncertainty_aa\n";
  ofstream outputData_npr(Form("%s/data/raaSystUncert_cent_npr.dat",outputDir));
  if (!outputData_npr.good()) {cout << "######### Fail to open data/*.dat file.##################" << endl;}
  outputData_npr << "pT\t" << "rapidity\t" << "cent\t" << "Raa\t" << "Uncert_tot\t"
                 << "contrib_IDtrg_stat_pp\t" << "contrib_IDtrg_syst_pp\t" << "contrib_IDtrg_pp\t"
                 << "contrib_sta_stat_pp\t" << "contrib_sta_syst_pp\t" << "contrib_sta_pp\t"
                 << "contrib_3d_pp\t" << "contrib_4d_pp\t" << "contrib_fit_pp\t" << "global_uncertainty_pp\t"
                 << "contrib_IDtrg_stat_aa\t" << "contrib_IDtrg_syst_aa\t" << "contrib_IDtrg_aa\t"
                 << "contrib_sta_stat_aa\t" << "contrib_sta_syst_aa\t" << "contrib_sta_aa\t"
                 << "contrib_3d_aa\t" << "contrib_4d_aa\t" << "contrib_fit_aa\t" << "global_uncertainty_aa\n";
    
  for(int ih=0; ih<nInHist;ih++)// for each kinematic range
  { 
    TString hist_pr(Form("phPrp_%s",yieldHistNames[ih]));
    TString hist_npr(Form("phNPrp_%s",yieldHistNames[ih]));
  
    cout <<"histogram input name: "<< hist_pr<<"\t"<<hist_npr<<endl; 
    
    // prompt histos
    phCorr_pr_pp = (TH1F*)fYesWeighFile_pp->Get(hist_pr);
    phCorr_pr_aa = (TH1F*)fYesWeighFile_aa->Get(hist_pr);
    phRaw_pr_pp  = (TH1F*)fNoWeighFile_pp->Get(hist_pr);
    phRaw_pr_aa  = (TH1F*)fNoWeighFile_aa->Get(hist_pr);
    
    // non-prompt histos
    phCorr_npr_pp = (TH1F*)fYesWeighFile_pp->Get(hist_npr);
    phCorr_npr_aa = (TH1F*)fYesWeighFile_aa->Get(hist_npr);
    phRaw_npr_pp  = (TH1F*)fNoWeighFile_pp->Get(hist_npr);
    phRaw_npr_aa  = (TH1F*)fNoWeighFile_aa->Get(hist_npr);
    
    switch(ih) {
      case 0: // high-pt, |y|<2.4
        phRaw_pr_pp_y024_pt6530_copy  = (TH1F*)phRaw_pr_pp->Clone();
        phRaw_pr_aa_y024_pt6530_copy  = (TH1F*)phRaw_pr_aa->Clone();
        phRaw_npr_pp_y024_pt6530_copy = (TH1F*)phRaw_npr_pp->Clone();
        phRaw_npr_aa_y024_pt6530_copy = (TH1F*)phRaw_npr_aa->Clone();
        phCorr_pr_pp_y024_pt6530_copy = (TH1F*)phCorr_pr_pp->Clone();
        phCorr_pr_aa_y024_pt6530_copy = (TH1F*)phCorr_pr_aa->Clone();
        phCorr_npr_pp_y024_pt6530_copy = (TH1F*)phCorr_npr_pp->Clone();
        phCorr_npr_aa_y024_pt6530_copy = (TH1F*)phCorr_npr_aa->Clone();
        break;
      case 1:// low-pt: 1.6<|y|<2.4
        phRaw_pr_pp_y1624_pt365_copy  = (TH1F*)phRaw_pr_pp->Clone();
        phRaw_pr_aa_y1624_pt365_copy  = (TH1F*)phRaw_pr_aa->Clone();
        phRaw_npr_pp_y1624_pt365_copy = (TH1F*)phRaw_npr_pp->Clone();
        phRaw_npr_aa_y1624_pt365_copy = (TH1F*)phRaw_npr_aa->Clone();
        phCorr_pr_pp_y1624_pt365_copy = (TH1F*)phCorr_pr_pp->Clone();
        phCorr_pr_aa_y1624_pt365_copy = (TH1F*)phCorr_pr_aa->Clone();
        phCorr_npr_pp_y1624_pt365_copy = (TH1F*)phCorr_npr_pp->Clone();
        phCorr_npr_aa_y1624_pt365_copy = (TH1F*)phCorr_npr_aa->Clone();
        break;
      case 2:// high-pt: |y|<1.2
        phRaw_pr_pp_y012_pt6530_copy  = (TH1F*)phRaw_pr_pp->Clone();
        phRaw_pr_aa_y012_pt6530_copy  = (TH1F*)phRaw_pr_aa->Clone();
        phRaw_npr_pp_y012_pt6530_copy = (TH1F*)phRaw_npr_pp->Clone();
        phRaw_npr_aa_y012_pt6530_copy = (TH1F*)phRaw_npr_aa->Clone();
        phCorr_pr_pp_y012_pt6530_copy = (TH1F*)phCorr_pr_pp->Clone();
        phCorr_pr_aa_y012_pt6530_copy = (TH1F*)phCorr_pr_aa->Clone();
        phCorr_npr_pp_y012_pt6530_copy = (TH1F*)phCorr_npr_pp->Clone();
        phCorr_npr_aa_y012_pt6530_copy = (TH1F*)phCorr_npr_aa->Clone();
        break;
      case 3:// high-pt: 1.2<|y|<1.6
        phRaw_pr_pp_y1216_pt6530_copy  = (TH1F*)phRaw_pr_pp->Clone();
        phRaw_pr_aa_y1216_pt6530_copy  = (TH1F*)phRaw_pr_aa->Clone();
        phRaw_npr_pp_y1216_pt6530_copy = (TH1F*)phRaw_npr_pp->Clone();
        phRaw_npr_aa_y1216_pt6530_copy = (TH1F*)phRaw_npr_aa->Clone();
        phCorr_pr_pp_y1216_pt6530_copy = (TH1F*)phCorr_pr_pp->Clone();
        phCorr_pr_aa_y1216_pt6530_copy = (TH1F*)phCorr_pr_aa->Clone();
        phCorr_npr_pp_y1216_pt6530_copy = (TH1F*)phCorr_npr_pp->Clone();
        phCorr_npr_aa_y1216_pt6530_copy = (TH1F*)phCorr_npr_aa->Clone();
        break;
      case 4:// high-pt: 1.6<|y|<2.4
        phRaw_pr_pp_y1624_pt6530_copy  = (TH1F*)phRaw_pr_pp->Clone();
        phRaw_pr_aa_y1624_pt6530_copy  = (TH1F*)phRaw_pr_aa->Clone();
        phRaw_npr_pp_y1624_pt6530_copy = (TH1F*)phRaw_npr_pp->Clone();
        phRaw_npr_aa_y1624_pt6530_copy = (TH1F*)phRaw_npr_aa->Clone();
        phCorr_pr_pp_y1624_pt6530_copy = (TH1F*)phCorr_pr_pp->Clone();
        phCorr_pr_aa_y1624_pt6530_copy = (TH1F*)phCorr_pr_aa->Clone();
        phCorr_npr_pp_y1624_pt6530_copy = (TH1F*)phCorr_npr_pp->Clone();
        phCorr_npr_aa_y1624_pt6530_copy = (TH1F*)phCorr_npr_aa->Clone();
        break;
    }

    unsigned int numBins = 0;
    if(ih==0) numBins = nBinsNpart12;
    if(ih!=0) numBins = nBinsNpart6;
      
    // high-pt and |y|<2.4
    double syst_fit_pr_aa[numBins][nFitVariations] ;
    double syst_eff4d_pr_aa[numBins][nEff4DVariations] ;
    double syst_effTnP_pr_aa[numBins][nEffTnPVariation];

    double syst_fit_pr_pp[numBins][nFitVariations]     ;
    double syst_eff4d_pr_pp[numBins][nEff4DVariations] ;
    double syst_effTnP_pr_pp[numBins][nEffTnPVariation];
  
    double syst_fit_npr_aa[nBinsNpart6][nFitVariations]    ;
    double syst_eff4d_npr_aa[nBinsNpart6][nEff4DVariations];
    double syst_effTnP_npr_aa[nBinsNpart6][nEffTnPVariation];

    double syst_fit_npr_pp[nBinsNpart6][nFitVariations]     ;
    double syst_eff4d_npr_pp[nBinsNpart6][nEff4DVariations] ;
    double syst_effTnP_npr_pp[nBinsNpart6][nEffTnPVariation];
  
    //high-pt and |y|<1.2
    // pr
    double syst_fit_pt6530y012_pr_aa[numBins][nFitVariations];
    double syst_eff4d_pt6530y012_pr_aa[numBins][nEff4DVariations];
    double syst_effTnP_pt6530y012_pr_aa[numBins][nEffTnPVariation];

    double syst_fit_pt6530y012_pr_pp[numBins][nFitVariations]; 
    double syst_eff4d_pt6530y012_pr_pp[numBins][nEff4DVariations];
    double syst_effTnP_pt6530y012_pr_pp[numBins][nEffTnPVariation];
    //npr
    double syst_fit_pt6530y012_npr_aa[numBins][nFitVariations]    ;
    double syst_eff4d_pt6530y012_npr_aa[numBins][nEff4DVariations];
    double syst_effTnP_pt6530y012_npr_aa[numBins][nEffTnPVariation];

    double syst_fit_pt6530y012_npr_pp[numBins][nFitVariations]     ;
    double syst_eff4d_pt6530y012_npr_pp[numBins][nEff4DVariations] ;
    double syst_effTnP_pt6530y012_npr_pp[numBins][nEffTnPVariation];

    //high-pt and 1.2<|y|<1.6
    // pr
    double syst_fit_pt6530y1216_pr_aa[numBins][nFitVariations];
    double syst_eff4d_pt6530y1216_pr_aa[numBins][nEff4DVariations];
    double syst_effTnP_pt6530y1216_pr_aa[numBins][nEffTnPVariation];

    double syst_fit_pt6530y1216_pr_pp[numBins][nFitVariations]; 
    double syst_eff4d_pt6530y1216_pr_pp[numBins][nEff4DVariations];
    double syst_effTnP_pt6530y1216_pr_pp[numBins][nEffTnPVariation];
    //npr
    double syst_fit_pt6530y1216_npr_aa[numBins][nFitVariations]    ;
    double syst_eff4d_pt6530y1216_npr_aa[numBins][nEff4DVariations];
    double syst_effTnP_pt6530y1216_npr_aa[numBins][nEffTnPVariation];

    double syst_fit_pt6530y1216_npr_pp[numBins][nFitVariations]     ;
    double syst_eff4d_pt6530y1216_npr_pp[numBins][nEff4DVariations] ;
    double syst_effTnP_pt6530y1216_npr_pp[numBins][nEffTnPVariation];

    //high-pt and 1.6|y|<2.4
    // pr
    double syst_fit_pt6530y1624_pr_aa[numBins][nFitVariations];
    double syst_eff4d_pt6530y1624_pr_aa[numBins][nEff4DVariations];
    double syst_effTnP_pt6530y1624_pr_aa[numBins][nEffTnPVariation];

    double syst_fit_pt6530y1624_pr_pp[numBins][nFitVariations]; 
    double syst_eff4d_pt6530y1624_pr_pp[numBins][nEff4DVariations];
    double syst_effTnP_pt6530y1624_pr_pp[numBins][nEffTnPVariation];
    //npr
    double syst_fit_pt6530y1624_npr_aa[numBins][nFitVariations]    ;
    double syst_eff4d_pt6530y1624_npr_aa[numBins][nEff4DVariations];
    double syst_effTnP_pt6530y1624_npr_aa[numBins][nEffTnPVariation];

    double syst_fit_pt6530y1624_npr_pp[numBins][nFitVariations]     ;
    double syst_eff4d_pt6530y1624_npr_pp[numBins][nEff4DVariations] ;
    double syst_effTnP_pt6530y1624_npr_pp[numBins][nEffTnPVariation];

    //low-pt and 1.6|y|<2.4
    // pr
    double syst_fit_pt365y1624_pr_aa[numBins][nFitVariations];
    double syst_eff4d_pt365y1624_pr_aa[numBins][nEff4DVariations];
    double syst_effTnP_pt365y1624_pr_aa[numBins][nEffTnPVariation];

    double syst_fit_pt365y1624_pr_pp[numBins][nFitVariations]; 
    double syst_eff4d_pt365y1624_pr_pp[numBins][nEff4DVariations];
    double syst_effTnP_pt365y1624_pr_pp[numBins][nEffTnPVariation];
    //npr
    double syst_fit_pt365y1624_npr_aa[numBins][nFitVariations]    ;
    double syst_eff4d_pt365y1624_npr_aa[numBins][nEff4DVariations];
    double syst_effTnP_pt365y1624_npr_aa[numBins][nEffTnPVariation];

    double syst_fit_pt365y1624_npr_pp[numBins][nFitVariations]     ;
    double syst_eff4d_pt365y1624_npr_pp[numBins][nEff4DVariations] ;
    double syst_effTnP_pt365y1624_npr_pp[numBins][nEffTnPVariation];

    cout <<"###################################################################"<<endl;
    if(bDoDebug) cout << "################ Kinematic region: " <<yieldHistNames[ih]<<endl;

    for(unsigned int ibin=1; ibin<=numBins; ibin++) {
      double fitContribution_pr_aa    = 0;
      double eff4dContribution_pr_aa  = 0;
      double efftnpContribution_pr_aa = 0; 

      double fitContribution_pr_pp    = 0;
      double eff4dContribution_pr_pp  = 0;
      double efftnpContribution_pr_pp = 0;

      double fitContribution_npr_aa    = 0;
      double eff4dContribution_npr_aa  = 0;
      double efftnpContribution_npr_aa = 0; 

      double fitContribution_npr_pp    = 0;
      double eff4dContribution_npr_pp  = 0;
      double efftnpContribution_npr_pp = 0;

      //high-pt |y|<1.2
      double fitContribution_pt6530y012_pr_aa    = 0;
      double eff4dContribution_pt6530y012_pr_aa  = 0;
      double efftnpContribution_pt6530y012_pr_aa = 0; 

      double fitContribution_pt6530y012_pr_pp    = 0;
      double eff4dContribution_pt6530y012_pr_pp  = 0;
      double efftnpContribution_pt6530y012_pr_pp = 0;

      double fitContribution_pt6530y012_npr_aa    = 0;
      double eff4dContribution_pt6530y012_npr_aa  = 0;
      double efftnpContribution_pt6530y012_npr_aa = 0; 

      double fitContribution_pt6530y012_npr_pp    = 0;
      double eff4dContribution_pt6530y012_npr_pp  = 0;
      double efftnpContribution_pt6530y012_npr_pp = 0;

      //high-pt 1.2<|y|<1.6
      double fitContribution_pt6530y1216_pr_aa    = 0;
      double eff4dContribution_pt6530y1216_pr_aa  = 0;
      double efftnpContribution_pt6530y1216_pr_aa = 0; 

      double fitContribution_pt6530y1216_pr_pp    = 0;
      double eff4dContribution_pt6530y1216_pr_pp  = 0;
      double efftnpContribution_pt6530y1216_pr_pp = 0;

      double fitContribution_pt6530y1216_npr_aa    = 0;
      double eff4dContribution_pt6530y1216_npr_aa  = 0;
      double efftnpContribution_pt6530y1216_npr_aa = 0; 

      double fitContribution_pt6530y1216_npr_pp    = 0;
      double eff4dContribution_pt6530y1216_npr_pp  = 0;
      double efftnpContribution_pt6530y1216_npr_pp = 0;

      //high-pt 1.6<|y|<2.4
      double fitContribution_pt6530y1624_pr_aa    = 0;
      double eff4dContribution_pt6530y1624_pr_aa  = 0;
      double efftnpContribution_pt6530y1624_pr_aa = 0; 

      double fitContribution_pt6530y1624_pr_pp    = 0;
      double eff4dContribution_pt6530y1624_pr_pp  = 0;
      double efftnpContribution_pt6530y1624_pr_pp = 0;

      double fitContribution_pt6530y1624_npr_aa    = 0;
      double eff4dContribution_pt6530y1624_npr_aa  = 0;
      double efftnpContribution_pt6530y1624_npr_aa = 0; 

      double fitContribution_pt6530y1624_npr_pp    = 0;
      double eff4dContribution_pt6530y1624_npr_pp  = 0;
      double efftnpContribution_pt6530y1624_npr_pp = 0;

      //low-pt 1.6<|y|<2.4
      double fitContribution_pt365y1624_pr_aa    = 0;
      double eff4dContribution_pt365y1624_pr_aa  = 0;
      double efftnpContribution_pt365y1624_pr_aa = 0; 

      double fitContribution_pt365y1624_pr_pp    = 0;
      double eff4dContribution_pt365y1624_pr_pp  = 0;
      double efftnpContribution_pt365y1624_pr_pp = 0;

      double fitContribution_pt365y1624_npr_aa    = 0;
      double eff4dContribution_pt365y1624_npr_aa  = 0;
      double efftnpContribution_pt365y1624_npr_aa = 0; 

      double fitContribution_pt365y1624_npr_pp    = 0;
      double eff4dContribution_pt365y1624_npr_pp  = 0;
      double efftnpContribution_pt365y1624_npr_pp = 0;

      //nominal prompt and non-prompt yield ratios
      const double yield_aa_pr  = phCorr_pr_aa->GetBinContent(ibin);
      const double yielderr_aa_pr  = phCorr_pr_aa->GetBinError(ibin);
      double yield_aa_npr=0, yielderr_aa_npr=0;
      if(ibin <= nBinsNpart6) { // out-of-range bins will be discarded for np
        yield_aa_npr = phCorr_npr_aa->GetBinContent(ibin);
        yielderr_aa_npr = phCorr_npr_aa->GetBinError(ibin);
      }
      
      // the pp yields are the same for all centrality bins
      const double yield_pp_pr  = phCorr_pr_pp->GetBinContent(1);
      const double yield_pp_npr = phCorr_npr_pp->GetBinContent(1);
      const double yielderr_pp_pr  = phCorr_pr_pp->GetBinContent(1);
      const double yielderr_pp_npr = phCorr_npr_pp->GetBinContent(1);

      const double scaleFactor      = ppLumi/nMbEvents;

      // Taa scale factor 
      double scale_cent       = 1./(adTaa12[ibin-1]*adDeltaCent12[ibin-1]);
      if(ih!=0) scale_cent = 1/(adTaa6[ibin-1]*adDeltaCent6[ibin-1]);
      double scale_cent6      = 0;
      if (ibin<=nBinsNpart6) {
        scale_cent6 = 1./(adTaa6[ibin-1]*adDeltaCent6[ibin-1]);
      }
      // taa relative uncertainty
      double taa6_relerr   = 0;
      const double taa12_relerr  = TMath::Power(adTaa12Err[ibin-1] / adTaa12[ibin-1],2); 
      if (ibin<=nBinsNpart6) {
        taa6_relerr = TMath::Power(adTaa6Err[ibin-1] / adTaa6[ibin-1],2);
      }

      if(bDoDebug) cout << "###### Taa uncert: bin6= " << taa6_relerr << "\tbin12= " << taa12_relerr << endl;

      double yieldRatio_pr  = yield_aa_pr/yield_pp_pr * scaleFactor * scale_cent;
      double yieldRatio_npr=0;
      if (ibin<=nBinsNpart6) {
        yieldRatio_npr = yield_aa_npr/yield_pp_npr * scaleFactor * scale_cent6;
      }
      
      if(bDoDebug) cout << "################ Bin " <<ibin << endl
                        << "  Nominal yields are (prompt_aa & nonPr_aa ; prompt_pp & nonPr_pp): " 
                        << yield_aa_pr<<"\t& "<<yield_aa_npr<<"\t; "<<yield_pp_pr<<"\t& "<<yield_pp_npr<<endl;

      // for each source of uncert, calc variation wrt nominal value
      for(int ivar=0; ivar<(nFitVariations+nEff4DVariations+nEffTnPVariation); ivar++) {
        cout <<"@@@@@@@ Variation = " << ibin << ": " << ivar << " / " << (nFitVariations+nEff4DVariations+nEffTnPVariation) <<endl; 
        char nameFile1[200], nameFile2[200];
      
        if(ivar<nFitVariations)
        {
          int ifile = ivar;
          sprintf(nameFile1,Form("%s/%s",inputDir,yieldHistFile_aa_systSgnBkg[ifile]));
          sprintf(nameFile2,Form("%s/%s",inputDir,yieldHistFile_pp_systSgnBkg[ifile]));
        }
    
        if( (ivar>=nFitVariations) && ivar<(nFitVariations+nEff4DVariations) )
        {
          int ifile = ivar-nFitVariations;
          sprintf(nameFile1,Form("%s/%s",inputDir,yieldHistFile_aa_syst4DCorr[ifile]));
          sprintf(nameFile2,Form("%s/%s",inputDir,yieldHistFile_pp_syst4DCorr[ifile]));
        }

        if( (ivar>=(nFitVariations+nEff4DVariations)) && (ivar < (nFitVariations+nEff4DVariations+nEffTnPVariation)) )
        {
          int ifile = ivar-nFitVariations-nEff4DVariations;
          sprintf(nameFile1,Form("%s/%s",inputDir,yieldHistFile_aa_systTnP[ifile]));
          sprintf(nameFile2,Form("%s/%s",inputDir,yieldHistFile_pp_systTnP[ifile]));
        }

        if(bDoDebug) cout << "Opened systematic files:\n pp: "<<nameFile2 << "\tAA: "<< nameFile1 <<endl;
        TFile *fVar_aa = new TFile(nameFile1);
        TFile *fVar_pp = new TFile(nameFile2);
        if (!fVar_aa->IsOpen() || !fVar_pp->IsOpen()) {
          cout << "One or more input files are missing" << endl;
          return ;
        }
        
        // prompt histos
        phCorrVar_pr_pp = (TH1F*)fVar_pp->Get(hist_pr);
        phCorrVar_pr_aa = (TH1F*)fVar_aa->Get(hist_pr);
        phCorrVar_pr_pp->SetDirectory(0);
        phCorrVar_pr_aa->SetDirectory(0);
        // non-prompt histos
        phCorrVar_npr_pp = (TH1F*)fVar_pp->Get(hist_npr);
        phCorrVar_npr_aa = (TH1F*)fVar_aa->Get(hist_npr);
        phCorrVar_npr_pp->SetDirectory(0);
        phCorrVar_npr_aa->SetDirectory(0);

        fVar_aa->Close();
        fVar_pp->Close();

        double yieldVar_aa_pr  = phCorrVar_pr_aa->GetBinContent(ibin);
        double yieldVar_aa_npr = 0;
        if(ibin <= nBinsNpart6) { // out-of-range bins will be discarded for np
          yieldVar_aa_npr = phCorrVar_npr_aa->GetBinContent(ibin);
        }
        
        // pp values are the same for each centrality, for a given centrality region
        double yieldVar_pp_pr  = phCorrVar_pr_pp->GetBinContent(1);
        double yieldVar_pp_npr = phCorrVar_npr_pp->GetBinContent(1);
        
        double relVar_aa_pr = (yield_aa_pr - yieldVar_aa_pr)/yield_aa_pr;
        double relVar_pp_pr = (yield_pp_pr - yieldVar_pp_pr)/yield_pp_pr; 

        double relVar_aa_npr = (yield_aa_npr - yieldVar_aa_npr)/yield_aa_npr;
        double relVar_pp_npr = (yield_pp_npr - yieldVar_pp_npr)/yield_pp_npr;

        if(bDoDebug) cout << "+++++++++++ Variation " << ivar << " yields are: "
                          <<yieldVar_aa_pr<<"\t& "<<yieldVar_aa_npr<<"\t;  "<<yieldVar_pp_pr<<"\t& "<<yieldVar_pp_npr<<endl;
        
        switch(ih){
        case 0: // high-pt, |y|<2.4
          // fit 
          if(ivar<nFitVariations)
          {
            prJpsi_cent[ibin-1]    = yieldRatio_pr;
            syst_fit_pr_aa[ibin-1][ivar]  = TMath::Power( relVar_aa_pr, 2 );
            syst_fit_pr_pp[ibin-1][ivar]  = TMath::Power( relVar_pp_pr, 2 );

            if(ibin<= nBinsNpart6) 
            {
              nonPrJpsi_cent[ibin-1] = yieldRatio_npr;
              syst_fit_npr_aa[ibin-1][ivar] = TMath::Power( relVar_aa_npr, 2 ) ;
              syst_fit_npr_pp[ibin-1][ivar] = TMath::Power( relVar_pp_npr, 2 ) ;
              if(bDoDebug) cout << "++++++++++++++++++++++ Delta yields: "
                                << syst_fit_pr_aa[ibin-1][ivar] << "\t& " << syst_fit_npr_aa[ibin-1][ivar] << "\t; "
                                << syst_fit_pr_pp[ibin-1][ivar] << "\t& " << syst_fit_npr_pp[ibin-1][ivar] << endl;
            } else {
              if(bDoDebug) cout << "++++++++++++++++++++++ Delta yields: "
                                << syst_fit_pr_aa[ibin-1][ivar] << "\t& " << "\t; "
                                << syst_fit_pr_pp[ibin-1][ivar] << "\t& " << endl;
            }
            if(method==1)//maximum
            {
              if( syst_fit_pr_aa[ibin-1][ivar] > fitContribution_pr_aa ) fitContribution_pr_aa = syst_fit_pr_aa[ibin-1][ivar];
              if( syst_fit_pr_pp[ibin-1][ivar] > fitContribution_pr_pp ) fitContribution_pr_pp = syst_fit_pr_pp[ibin-1][ivar];
              
              if(ibin<= nBinsNpart6) 
              {
                if( syst_fit_npr_aa[ibin-1][ivar] > fitContribution_npr_aa ) fitContribution_npr_aa = syst_fit_npr_aa[ibin-1][ivar];
                if( syst_fit_npr_pp[ibin-1][ivar] > fitContribution_npr_pp ) fitContribution_npr_pp = syst_fit_npr_pp[ibin-1][ivar];
              }
            }
            if(method==0 || method==2) // rms of all variations for fit systm
            {
              fitContribution_pr_aa += syst_fit_pr_aa[ibin-1][ivar];
              fitContribution_pr_pp += syst_fit_pr_pp[ibin-1][ivar];
              if(ibin<= nBinsNpart6) 
              {
                fitContribution_npr_aa += syst_fit_npr_aa[ibin-1][ivar]; 
                fitContribution_npr_pp += syst_fit_npr_pp[ibin-1][ivar];
              }
            }
 
            if (ibin<=nBinsNpart6) {
              if(bDoDebug) cout << "+++++++++++++++++++++++++++++++++ Fit contribution to systm: "
                                << fitContribution_pr_aa << "\t& " << fitContribution_npr_aa << "\t; "
                                << fitContribution_pr_pp << "\t& " << fitContribution_npr_pp << endl;
            } else {
              if(bDoDebug) cout << "+++++++++++++++++++++++++++++++++ Fit contribution to systm: "
                                << fitContribution_pr_aa << "\t& " << "\t; "
                                << fitContribution_pr_pp << "\t& " << endl;
            }
          }
          // 4d eff
          if(ivar>=nFitVariations && ivar<(nFitVariations+nEff4DVariations) )
          {
            int ifile = ivar-nFitVariations;
            syst_eff4d_pr_aa[ibin-1][ifile]  = TMath::Power( relVar_aa_pr,2 ) ;
            syst_eff4d_pr_pp[ibin-1][ifile]  = TMath::Power( relVar_pp_pr,2 ) ;
            
            if(ibin<= nBinsNpart6) 
            {
              syst_eff4d_npr_aa[ibin-1][ifile]  = TMath::Power( relVar_aa_npr,2 ) ;
              syst_eff4d_npr_pp[ibin-1][ifile]  = TMath::Power( relVar_pp_npr,2 ) ;
              if(bDoDebug) cout << "++++++++++++++++++++++ Delta yields: "
                                << syst_eff4d_pr_aa[ibin-1][ifile] << "\t& " << syst_eff4d_npr_aa[ibin-1][ifile] << "\t; "
                                << syst_eff4d_pr_pp[ibin-1][ifile] << "\t& " << syst_eff4d_npr_pp[ibin-1][ifile] << endl;
            } else {
              if(bDoDebug) cout << "++++++++++++++++++++++ Delta yields: "
                                << syst_eff4d_pr_aa[ibin-1][ifile] << "\t& " << "\t; "
                                << syst_eff4d_pr_pp[ibin-1][ifile] << "\t& " << endl;
            }

            if(method==1)//maximum
            {
              if( syst_eff4d_pr_aa[ibin-1][ifile] > eff4dContribution_pr_aa ) eff4dContribution_pr_aa = syst_eff4d_pr_aa[ibin-1][ifile];
              if( syst_eff4d_pr_pp[ibin-1][ifile] > eff4dContribution_pr_pp ) eff4dContribution_pr_pp = syst_eff4d_pr_pp[ibin-1][ifile];
              
              if(ibin<= nBinsNpart6) 
              {
                if( syst_eff4d_npr_aa[ibin-1][ifile] > eff4dContribution_npr_aa ) eff4dContribution_npr_aa = syst_eff4d_npr_aa[ibin-1][ifile];
                if( syst_eff4d_npr_pp[ibin-1][ifile] > eff4dContribution_npr_pp ) eff4dContribution_npr_pp = syst_eff4d_npr_pp[ibin-1][ifile];
              }
            }
            if(method==0 || method==2) // add in quadrature all independent variations
            {
              eff4dContribution_pr_aa += syst_eff4d_pr_aa[ibin-1][ifile];
              eff4dContribution_pr_pp += syst_eff4d_pr_pp[ibin-1][ifile];
                
              if(ibin<= nBinsNpart6) 
              {
                eff4dContribution_npr_aa += syst_eff4d_npr_aa[ibin-1][ifile];
                eff4dContribution_npr_pp += syst_eff4d_npr_pp[ibin-1][ifile];
              }
            }
            if (ibin<=nBinsNpart6) {
              if(bDoDebug) cout << "+++++++++++++++++++++++++++++++++ 4DEff Contribution to systm: "
                                << eff4dContribution_pr_aa << "\t& " << eff4dContribution_npr_aa << "\t; "
                                << eff4dContribution_pr_pp << "\t& " << eff4dContribution_npr_pp << endl;
            } else {
              if(bDoDebug) cout << "+++++++++++++++++++++++++++++++++ 4DEff Contribution to systm: "
                                << eff4dContribution_pr_aa << "\t& " << "\t; "
                                << eff4dContribution_pr_pp << "\t& " << endl;
            }
          }

          // TnP and 3d eff
          if( (ivar>=(nFitVariations+nEff4DVariations)) && (ivar < (nFitVariations+nEff4DVariations+nEffTnPVariation)) )
          {
            int ifile = ivar-nFitVariations-nEff4DVariations;
            syst_effTnP_pr_aa[ibin-1][ifile]  = TMath::Power( yieldVar_aa_pr,2 ) ;
            syst_effTnP_pr_pp[ibin-1][ifile]  = TMath::Power( yieldVar_pp_pr,2 ) ;
            
            if(ibin<= nBinsNpart6) 
            {
              syst_effTnP_npr_aa[ibin-1][ifile]  = TMath::Power( yieldVar_aa_npr,2 ) ;
              syst_effTnP_npr_pp[ibin-1][ifile]  = TMath::Power( yieldVar_pp_npr,2 ) ;
            }
            if (ibin<=nBinsNpart6) {
              if(bDoDebug) cout << "++++++++++++++++++++++ Delta yields: "
                                << syst_effTnP_pr_aa[ibin-1][ifile] << "\t& " << syst_effTnP_npr_aa[ibin-1][ifile] << "\t; "
                                << syst_effTnP_pr_pp[ibin-1][ifile] << "\t& " << syst_effTnP_npr_pp[ibin-1][ifile] << endl;
            } else {
              if(bDoDebug) cout << "++++++++++++++++++++++ Delta yields: "
                                << syst_effTnP_pr_aa[ibin-1][ifile] << "\t& " << "\t; "
                                << syst_effTnP_pr_pp[ibin-1][ifile] << "\t& " << endl;
            }

            if(method==1)//maximum
            {
              if( syst_effTnP_pr_aa[ibin-1][ifile] > efftnpContribution_pr_aa ) efftnpContribution_pr_aa = syst_effTnP_pr_aa[ibin-1][ifile];
              if( syst_effTnP_pr_pp[ibin-1][ifile] > efftnpContribution_pr_pp ) efftnpContribution_pr_pp = syst_effTnP_pr_pp[ibin-1][ifile];
                
              if(ibin<= nBinsNpart6) 
              {
                if( syst_effTnP_npr_aa[ibin-1][ifile] > efftnpContribution_npr_aa ) efftnpContribution_npr_aa = syst_effTnP_npr_aa[ibin-1][ifile];
                if( syst_effTnP_npr_pp[ibin-1][ifile] > efftnpContribution_npr_pp ) efftnpContribution_npr_pp = syst_effTnP_npr_pp[ibin-1][ifile];
              }
            }
            if(method==0 || method==2) // add in quadrature all independent variations
            {
              efftnpContribution_pr_aa += syst_effTnP_pr_aa[ibin-1][ifile];
              efftnpContribution_pr_pp += syst_effTnP_pr_pp[ibin-1][ifile];
                
              if(ibin<= nBinsNpart6) 
              {
                efftnpContribution_npr_aa += syst_effTnP_npr_aa[ibin-1][ifile];
                efftnpContribution_npr_pp += syst_effTnP_npr_pp[ibin-1][ifile];
              }
            }
            if (ibin<=nBinsNpart6) {
              if(bDoDebug) cout << "+++++++++++++++++++++++++++++++++ Contribution to systm: " << efftnpContribution_pr_aa << "\t& " << efftnpContribution_npr_aa << "\t; "
                                << efftnpContribution_pr_pp << "\t& " << efftnpContribution_npr_pp << endl;
            } else {
              if(bDoDebug) cout << "+++++++++++++++++++++++++++++++++ Contribution to systm: " << efftnpContribution_pr_aa << "\t& " << "\t; "
                                << efftnpContribution_pr_pp << "\t& " << endl;
            }
          }
          break;
          
        case 1:// low-pt: 1.6<|y|<2.4
          if (ibin > nBinsNpart6) break;
          prJpsi_pt365y1624_cent[ibin-1]    = yieldRatio_pr;
          nonPrJpsi_pt365y1624_cent[ibin-1] = yieldRatio_npr;
          
          if(ivar<nFitVariations)
          {
            syst_fit_pt365y1624_pr_aa[ibin-1][ivar]  = TMath::Power(relVar_aa_pr,2) ;
            syst_fit_pt365y1624_pr_pp[ibin-1][ivar]  = TMath::Power(relVar_pp_pr,2) ;
              
            syst_fit_pt365y1624_npr_aa[ibin-1][ivar]  = TMath::Power(relVar_aa_npr,2) ;
            syst_fit_pt365y1624_npr_pp[ibin-1][ivar]  = TMath::Power(relVar_pp_npr,2) ;
            if(bDoDebug) cout << "++++++++++++++++++++++ Delta yields: "
                              << syst_fit_pt365y1624_pr_aa[ibin-1][ivar] << "\t& " << syst_fit_pt365y1624_npr_aa[ibin-1][ivar] << "\t; "
                              << syst_fit_pt365y1624_pr_pp[ibin-1][ivar] << "\t& " << syst_fit_pt365y1624_npr_pp[ibin-1][ivar] << endl;
            if(method==1)//maximum
            {
              if( syst_fit_pt365y1624_pr_aa[ibin-1][ivar] > fitContribution_pt365y1624_pr_aa ) fitContribution_pt365y1624_pr_aa = syst_fit_pt365y1624_pr_aa[ibin-1][ivar];
              if( syst_fit_pt365y1624_pr_pp[ibin-1][ivar] > fitContribution_pt365y1624_pr_pp ) fitContribution_pt365y1624_pr_pp = syst_fit_pt365y1624_pr_pp[ibin-1][ivar];
                
              if( syst_fit_pt365y1624_npr_aa[ibin-1][ivar] > fitContribution_pt365y1624_npr_aa ) fitContribution_pt365y1624_npr_aa = syst_fit_pt365y1624_npr_aa[ibin-1][ivar];
              if( syst_fit_pt365y1624_npr_pp[ibin-1][ivar] > fitContribution_pt365y1624_npr_pp ) fitContribution_pt365y1624_npr_pp = syst_fit_pt365y1624_npr_pp[ibin-1][ivar];
            }
            if(method==0 || method==2) // rms of all variations for fit systm.
            {
              fitContribution_pt365y1624_pr_aa += syst_fit_pt365y1624_pr_aa[ibin-1][ivar];
              fitContribution_pt365y1624_pr_pp += syst_fit_pt365y1624_pr_pp[ibin-1][ivar];
                
              fitContribution_pt365y1624_npr_aa += syst_fit_pt365y1624_npr_aa[ibin-1][ivar];
              fitContribution_pt365y1624_npr_pp += syst_fit_pt365y1624_npr_pp[ibin-1][ivar];
            }
            if(bDoDebug) cout << "+++++++++++++++++++++++++++++++++ Fit contribution to systm: "
                              << fitContribution_pt365y1624_pr_aa << "\t& " << fitContribution_pt365y1624_npr_aa << "\t; "
                              << fitContribution_pt365y1624_pr_pp << "\t& " << fitContribution_pt365y1624_npr_pp << endl;
          }
          if(ivar>=nFitVariations && ivar<(nFitVariations+nEff4DVariations) )
          {
            int ifile = ivar-nFitVariations;
            syst_eff4d_pt365y1624_pr_aa[ibin-1][ifile]  = TMath::Power( relVar_aa_pr,2 ) ;
            syst_eff4d_pt365y1624_pr_pp[ibin-1][ifile]  = TMath::Power( relVar_pp_pr,2 ) ;
            
            syst_eff4d_pt365y1624_npr_aa[ibin-1][ifile]  = TMath::Power( relVar_aa_npr,2) ;
            syst_eff4d_pt365y1624_npr_pp[ibin-1][ifile]  = TMath::Power( relVar_pp_npr,2) ;
            if(bDoDebug) cout << "++++++++++++++++++++++ Delta yields: "
                              << syst_eff4d_pt365y1624_pr_aa[ibin-1][ifile] << "\t& " << syst_eff4d_pt365y1624_npr_aa[ibin-1][ifile] << "\t; "
                              << syst_eff4d_pt365y1624_pr_pp[ibin-1][ifile] << "\t& " << syst_eff4d_pt365y1624_npr_pp[ibin-1][ifile] << endl;
            
            if(method==1)//maximum
            {
              if( syst_eff4d_pt365y1624_pr_aa[ibin-1][ifile] > eff4dContribution_pt365y1624_pr_aa ) eff4dContribution_pt365y1624_pr_aa = syst_eff4d_pt365y1624_pr_aa[ibin-1][ifile];
              if( syst_eff4d_pt365y1624_pr_pp[ibin-1][ifile] > eff4dContribution_pt365y1624_pr_pp ) eff4dContribution_pt365y1624_pr_pp = syst_eff4d_pt365y1624_pr_pp[ibin-1][ifile];
              
              if( syst_eff4d_pt365y1624_npr_aa[ibin-1][ifile] > eff4dContribution_pt365y1624_npr_aa ) eff4dContribution_pt365y1624_npr_aa = syst_eff4d_pt365y1624_npr_aa[ibin-1][ifile];
              if( syst_eff4d_pt365y1624_npr_pp[ibin-1][ifile] > eff4dContribution_pt365y1624_npr_pp ) eff4dContribution_pt365y1624_npr_pp = syst_eff4d_pt365y1624_npr_pp[ibin-1][ifile];
            }
            if(method==0 || method==2) // add in quadrature all independent variations
            {
              eff4dContribution_pt365y1624_pr_aa += syst_eff4d_pt365y1624_pr_aa[ibin-1][ifile];
              eff4dContribution_pt365y1624_pr_pp += syst_eff4d_pt365y1624_pr_pp[ibin-1][ifile];
              
              eff4dContribution_pt365y1624_npr_aa += syst_eff4d_pt365y1624_npr_aa[ibin-1][ifile];
              eff4dContribution_pt365y1624_npr_pp += syst_eff4d_pt365y1624_npr_pp[ibin-1][ifile];
            }
            if(bDoDebug) cout << "+++++++++++++++++++++++++++++++++ Fit contribution to systm: "
                              << eff4dContribution_pt365y1624_pr_aa << "\t& " << eff4dContribution_pt365y1624_npr_aa << "\t; "
                              << eff4dContribution_pt365y1624_pr_pp << "\t& " << eff4dContribution_pt365y1624_npr_pp << endl;
          }
          if( (ivar>=(nFitVariations+nEff4DVariations)) && (ivar < (nFitVariations+nEff4DVariations+nEffTnPVariation)) )
          {
            int ifile = ivar-nFitVariations-nEff4DVariations;
            syst_effTnP_pt365y1624_pr_aa[ibin-1][ifile]  = TMath::Power( yieldVar_aa_pr,2 ) ;
            syst_effTnP_pt365y1624_pr_pp[ibin-1][ifile]  = TMath::Power( yieldVar_pp_pr,2 ) ;
              
            syst_effTnP_pt365y1624_npr_aa[ibin-1][ifile]  = TMath::Power( yieldVar_aa_npr,2) ;
            syst_effTnP_pt365y1624_npr_pp[ibin-1][ifile]  = TMath::Power( yieldVar_pp_npr,2) ;
            if(bDoDebug) cout << "++++++++++++++++++++++ Delta yields:  "
                              << syst_effTnP_pt365y1624_pr_aa[ibin-1][ifile] << "\t& " << syst_effTnP_pt365y1624_npr_aa[ibin-1][ifile] << "\t; "
                              << syst_effTnP_pt365y1624_pr_pp[ibin-1][ifile] << "\t& " << syst_effTnP_pt365y1624_npr_pp[ibin-1][ifile] << endl;
              
            if(method==1)//maximum
            {
              if( syst_effTnP_pt365y1624_pr_aa[ibin-1][ifile] > efftnpContribution_pt365y1624_pr_aa ) efftnpContribution_pt365y1624_pr_aa = syst_effTnP_pt365y1624_pr_aa[ibin-1][ifile];
              if( syst_effTnP_pt365y1624_pr_pp[ibin-1][ifile] > efftnpContribution_pt365y1624_pr_pp ) efftnpContribution_pt365y1624_pr_pp = syst_effTnP_pt365y1624_pr_pp[ibin-1][ifile];
              
              if( syst_effTnP_pt365y1624_npr_aa[ibin-1][ifile] > efftnpContribution_pt365y1624_npr_aa ) efftnpContribution_pt365y1624_npr_aa = syst_effTnP_pt365y1624_npr_aa[ibin-1][ifile];
              if( syst_effTnP_pt365y1624_npr_pp[ibin-1][ifile] > efftnpContribution_pt365y1624_npr_pp ) efftnpContribution_pt365y1624_npr_pp = syst_effTnP_pt365y1624_npr_pp[ibin-1][ifile];
            }
            if(method==0 || method==2) // add in quadrature all independent variations
            {
              efftnpContribution_pt365y1624_pr_aa += syst_effTnP_pt365y1624_pr_aa[ibin-1][ifile];
              efftnpContribution_pt365y1624_pr_pp += syst_effTnP_pt365y1624_pr_pp[ibin-1][ifile];
                
              efftnpContribution_pt365y1624_npr_aa += syst_effTnP_pt365y1624_npr_aa[ibin-1][ifile];
              efftnpContribution_pt365y1624_npr_pp += syst_effTnP_pt365y1624_npr_pp[ibin-1][ifile];
            }
            if(bDoDebug) cout << "+++++++++++++++++++++++++++++++++ Fit contribution to systm: "
                              << efftnpContribution_pt365y1624_pr_aa << "\t& " << efftnpContribution_pt365y1624_npr_aa << "\t; "
                              << efftnpContribution_pt365y1624_pr_pp << "\t& " << efftnpContribution_pt365y1624_npr_pp << endl;
          }
          break;
          
         case 2:// high-pt: |y|<1.2
          if (ibin > nBinsNpart6) break;
          prJpsi_pt6530y012_cent[ibin-1]    = yieldRatio_pr;
          nonPrJpsi_pt6530y012_cent[ibin-1] = yieldRatio_npr;
          
          if(ivar<nFitVariations)
          {
            syst_fit_pt6530y012_pr_aa[ibin-1][ivar]  = TMath::Power( relVar_aa_pr ,2 ) ;
            syst_fit_pt6530y012_pr_pp[ibin-1][ivar]  = TMath::Power( relVar_pp_pr ,2) ;
              
            syst_fit_pt6530y012_npr_aa[ibin-1][ivar]  = TMath::Power( relVar_aa_npr ,2) ;
            syst_fit_pt6530y012_npr_pp[ibin-1][ivar]  = TMath::Power( relVar_pp_npr ,2) ;
            if(bDoDebug) cout << "++++++++++++++++++++++ Delta yields: "
                              << syst_fit_pt6530y012_pr_aa[ibin-1][ivar] << "\t& " << syst_fit_pt6530y012_npr_aa[ibin-1][ivar] << "\t; "
                              << syst_fit_pt6530y012_pr_pp[ibin-1][ivar] << "\t& " << syst_fit_pt6530y012_npr_pp[ibin-1][ivar] << endl;
              
            if(method==1)//maximum
            {
              if( syst_fit_pt6530y012_pr_aa[ibin-1][ivar] > fitContribution_pt6530y012_pr_aa ) fitContribution_pt6530y012_pr_aa = syst_fit_pt6530y012_pr_aa[ibin-1][ivar];
              if( syst_fit_pt6530y012_pr_pp[ibin-1][ivar] > fitContribution_pt6530y012_pr_pp ) fitContribution_pt6530y012_pr_pp = syst_fit_pt6530y012_pr_pp[ibin-1][ivar];
                
              if( syst_fit_pt6530y012_npr_aa[ibin-1][ivar] > fitContribution_pt6530y012_npr_aa ) fitContribution_pt6530y012_npr_aa = syst_fit_pt6530y012_npr_aa[ibin-1][ivar];
              if( syst_fit_pt6530y012_npr_pp[ibin-1][ivar] > fitContribution_pt6530y012_npr_pp ) fitContribution_pt6530y012_npr_pp = syst_fit_pt6530y012_npr_pp[ibin-1][ivar];
            }
            if(method==0 || method==2) // rms of all variations for fit systm.
            {
              fitContribution_pt6530y012_pr_aa += syst_fit_pt6530y012_pr_aa[ibin-1][ivar];
              fitContribution_pt6530y012_pr_pp += syst_fit_pt6530y012_pr_pp[ibin-1][ivar];
                
              fitContribution_pt6530y012_npr_aa += syst_fit_pt6530y012_npr_aa[ibin-1][ivar];
              fitContribution_pt6530y012_npr_pp += syst_fit_pt6530y012_npr_pp[ibin-1][ivar];
            }
            if(bDoDebug) cout << "+++++++++++++++++++++++++++++++++ Fit contribution to systm: "
                              << fitContribution_pt6530y012_pr_aa << "\t& " << fitContribution_pt6530y012_npr_aa << "\t; "
                              << fitContribution_pt6530y012_pr_pp << "\t& " << fitContribution_pt6530y012_npr_pp << endl;
          }
          if(ivar>=nFitVariations && ivar<(nFitVariations+nEff4DVariations) )
          {
            int ifile = ivar-nFitVariations;
            syst_eff4d_pt6530y012_pr_aa[ibin-1][ifile]  = TMath::Power( relVar_aa_pr,2 ) ;
            syst_eff4d_pt6530y012_pr_pp[ibin-1][ifile]  = TMath::Power( relVar_pp_pr ,2 ) ;
            
            syst_eff4d_pt6530y012_npr_aa[ibin-1][ifile]  = TMath::Power( relVar_aa_npr ,2) ;
            syst_eff4d_pt6530y012_npr_pp[ibin-1][ifile]  = TMath::Power( relVar_pp_npr ,2) ;
            if(bDoDebug) cout << "++++++++++++++++++++++ Delta yields: "
                              << syst_eff4d_pt6530y012_pr_aa[ibin-1][ifile] << "\t& " << syst_eff4d_pt6530y012_npr_aa[ibin-1][ifile] << "\t; "
                              << syst_eff4d_pt6530y012_pr_pp[ibin-1][ifile] << "\t& " << syst_eff4d_pt6530y012_npr_pp[ibin-1][ifile] << endl;
            
            if(method==1) //maximum
            {
              if( syst_eff4d_pt6530y012_pr_aa[ibin-1][ifile] > eff4dContribution_pt6530y012_pr_aa ) eff4dContribution_pt6530y012_pr_aa = syst_eff4d_pt6530y012_pr_aa[ibin-1][ifile];
              if( syst_eff4d_pt6530y012_pr_pp[ibin-1][ifile] > eff4dContribution_pt6530y012_pr_pp ) eff4dContribution_pt6530y012_pr_pp = syst_eff4d_pt6530y012_pr_pp[ibin-1][ifile];
              
              if( syst_eff4d_pt6530y012_npr_aa[ibin-1][ifile] > eff4dContribution_pt6530y012_npr_aa ) eff4dContribution_pt6530y012_npr_aa = syst_eff4d_pt6530y012_npr_aa[ibin-1][ifile];
              if( syst_eff4d_pt6530y012_npr_pp[ibin-1][ifile] > eff4dContribution_pt6530y012_npr_pp ) eff4dContribution_pt6530y012_npr_pp = syst_eff4d_pt6530y012_npr_pp[ibin-1][ifile];
            }
            if(method==0 || method==2) // add in quadrature all independent variations
            {
              eff4dContribution_pt6530y012_pr_aa += syst_eff4d_pt6530y012_pr_aa[ibin-1][ifile];
              eff4dContribution_pt6530y012_pr_pp += syst_eff4d_pt6530y012_pr_pp[ibin-1][ifile];
              
              eff4dContribution_pt6530y012_npr_aa += syst_eff4d_pt6530y012_npr_aa[ibin-1][ifile];
              eff4dContribution_pt6530y012_npr_pp += syst_eff4d_pt6530y012_npr_pp[ibin-1][ifile];
            }
            if(bDoDebug) cout << "+++++++++++++++++++++++++++++++++ Fit contribution to systm: "
                              << eff4dContribution_pt6530y012_pr_aa << "\t& " << eff4dContribution_pt6530y012_npr_aa << "\t; "
                              << eff4dContribution_pt6530y012_pr_pp << "\t& " << eff4dContribution_pt6530y012_npr_pp << endl;
          }
          if( (ivar>=(nFitVariations+nEff4DVariations)) && (ivar < (nFitVariations+nEff4DVariations+nEffTnPVariation)) )
          {
            int ifile = ivar-nFitVariations-nEff4DVariations;
            syst_effTnP_pt6530y012_pr_aa[ibin-1][ifile]  = TMath::Power( yieldVar_aa_pr ,2 ) ;
            syst_effTnP_pt6530y012_pr_pp[ibin-1][ifile]  = TMath::Power( yieldVar_pp_pr ,2 ) ;
              
            syst_effTnP_pt6530y012_npr_aa[ibin-1][ifile]  = TMath::Power( yieldVar_aa_npr ,2) ;
            syst_effTnP_pt6530y012_npr_pp[ibin-1][ifile]  = TMath::Power( yieldVar_pp_npr ,2) ;
            if(bDoDebug) cout << "++++++++++++++++++++++ Delta yields: "
                              << syst_effTnP_pt6530y012_pr_aa[ibin-1][ifile] << "\t& " << syst_effTnP_pt6530y012_npr_aa[ibin-1][ifile] << "\t; "
                              << syst_effTnP_pt6530y012_pr_pp[ibin-1][ifile] << "\t& " << syst_effTnP_pt6530y012_npr_pp[ibin-1][ifile] << endl;
              
            if(method==1)//maximum
            {
              if( syst_effTnP_pt6530y012_pr_aa[ibin-1][ifile] > efftnpContribution_pt6530y012_pr_aa ) efftnpContribution_pt6530y012_pr_aa = syst_effTnP_pt6530y012_pr_aa[ibin-1][ifile];
              if( syst_effTnP_pt6530y012_pr_pp[ibin-1][ifile] > efftnpContribution_pt6530y012_pr_pp ) efftnpContribution_pt6530y012_pr_pp = syst_effTnP_pt6530y012_pr_pp[ibin-1][ifile];
              
              if( syst_effTnP_pt6530y012_npr_aa[ibin-1][ifile] > efftnpContribution_pt6530y012_npr_aa ) efftnpContribution_pt6530y012_npr_aa = syst_effTnP_pt6530y012_npr_aa[ibin-1][ifile];
              if( syst_effTnP_pt6530y012_npr_pp[ibin-1][ifile] > efftnpContribution_pt6530y012_npr_pp ) efftnpContribution_pt6530y012_npr_pp = syst_effTnP_pt6530y012_npr_pp[ibin-1][ifile];
            }
            if(method==0 || method==2) // add in quadrature all independent variations
            {
              efftnpContribution_pt6530y012_pr_aa += syst_effTnP_pt6530y012_pr_aa[ibin-1][ifile];
              efftnpContribution_pt6530y012_pr_pp += syst_effTnP_pt6530y012_pr_pp[ibin-1][ifile];
                
              efftnpContribution_pt6530y012_npr_aa += syst_effTnP_pt6530y012_npr_aa[ibin-1][ifile];
              efftnpContribution_pt6530y012_npr_pp += syst_effTnP_pt6530y012_npr_pp[ibin-1][ifile];
            }
            if(bDoDebug) cout << "+++++++++++++++++++++++++++++++++ Fit contribution to systm: "
                              << efftnpContribution_pt6530y012_pr_aa << "\t& " << efftnpContribution_pt6530y012_npr_aa << "\t; "
                              << efftnpContribution_pt6530y012_pr_pp << "\t& " << efftnpContribution_pt6530y012_npr_pp << endl;
          }
          break;

          case 3:// high-pt: 1.2<|y|<1.6
          if (ibin > nBinsNpart6) break;
          prJpsi_pt6530y1216_cent[ibin-1]    = yieldRatio_pr;
          nonPrJpsi_pt6530y1216_cent[ibin-1] = yieldRatio_npr;
          
          if(ivar<nFitVariations)
          {
            syst_fit_pt6530y1216_pr_aa[ibin-1][ivar]  = TMath::Power( relVar_aa_pr ,2 ) ;
            syst_fit_pt6530y1216_pr_pp[ibin-1][ivar]  = TMath::Power( relVar_pp_pr ,2) ;
              
            syst_fit_pt6530y1216_npr_aa[ibin-1][ivar]  = TMath::Power( relVar_aa_npr ,2) ;
            syst_fit_pt6530y1216_npr_pp[ibin-1][ivar]  = TMath::Power( relVar_pp_npr ,2) ;
            if(bDoDebug) cout << "++++++++++++++++++++++ Delta yields: "
                              << syst_fit_pt6530y1216_pr_aa[ibin-1][ivar] << "\t& " << syst_fit_pt6530y1216_npr_aa[ibin-1][ivar] << "\t; "
                              << syst_fit_pt6530y1216_pr_pp[ibin-1][ivar] << "\t& " << syst_fit_pt6530y1216_npr_pp[ibin-1][ivar] << endl;
              
            if(method==1)//maximum
            {
              if( syst_fit_pt6530y1216_pr_aa[ibin-1][ivar] > fitContribution_pt6530y1216_pr_aa ) fitContribution_pt6530y1216_pr_aa = syst_fit_pt6530y1216_pr_aa[ibin-1][ivar];
              if( syst_fit_pt6530y1216_pr_pp[ibin-1][ivar] > fitContribution_pt6530y1216_pr_pp ) fitContribution_pt6530y1216_pr_pp = syst_fit_pt6530y1216_pr_pp[ibin-1][ivar];
                
              if( syst_fit_pt6530y1216_npr_aa[ibin-1][ivar] > fitContribution_pt6530y1216_npr_aa ) fitContribution_pt6530y1216_npr_aa = syst_fit_pt6530y1216_npr_aa[ibin-1][ivar];
              if( syst_fit_pt6530y1216_npr_pp[ibin-1][ivar] > fitContribution_pt6530y1216_npr_pp ) fitContribution_pt6530y1216_npr_pp = syst_fit_pt6530y1216_npr_pp[ibin-1][ivar];
            }
            if(method==0 || method==2) // rms of all variations for fit systm.
            {
              fitContribution_pt6530y1216_pr_aa += syst_fit_pt6530y1216_pr_aa[ibin-1][ivar];
              fitContribution_pt6530y1216_pr_pp += syst_fit_pt6530y1216_pr_pp[ibin-1][ivar];
                
              fitContribution_pt6530y1216_npr_aa += syst_fit_pt6530y1216_npr_aa[ibin-1][ivar];
              fitContribution_pt6530y1216_npr_pp += syst_fit_pt6530y1216_npr_pp[ibin-1][ivar];
            }
            if(bDoDebug) cout << "+++++++++++++++++++++++++++++++++ Fit contribution to systm: "
                              << fitContribution_pt6530y1216_pr_aa << "\t& " << fitContribution_pt6530y1216_npr_aa << "\t; "
                              << fitContribution_pt6530y1216_pr_pp << "\t& " << fitContribution_pt6530y1216_npr_pp << endl;
          }
          if(ivar>=nFitVariations && ivar<(nFitVariations+nEff4DVariations) )
          {
            int ifile = ivar-nFitVariations;
            syst_eff4d_pt6530y1216_pr_aa[ibin-1][ifile]  = TMath::Power( relVar_aa_pr,2 ) ;
            syst_eff4d_pt6530y1216_pr_pp[ibin-1][ifile]  = TMath::Power( relVar_pp_pr ,2 ) ;
            
            syst_eff4d_pt6530y1216_npr_aa[ibin-1][ifile]  = TMath::Power( relVar_aa_npr ,2) ;
            syst_eff4d_pt6530y1216_npr_pp[ibin-1][ifile]  = TMath::Power( relVar_pp_npr ,2) ;
            if(bDoDebug) cout << "++++++++++++++++++++++ Delta yields: "
                              << syst_eff4d_pt6530y1216_pr_aa[ibin-1][ifile] << "\t& " << syst_eff4d_pt6530y1216_npr_aa[ibin-1][ifile] << "\t; "
                              << syst_eff4d_pt6530y1216_pr_pp[ibin-1][ifile] << "\t& " << syst_eff4d_pt6530y1216_npr_pp[ibin-1][ifile] << endl;
            
            if(method==1)//maximum
            {
              if( syst_eff4d_pt6530y1216_pr_aa[ibin-1][ifile] > eff4dContribution_pt6530y1216_pr_aa ) eff4dContribution_pt6530y1216_pr_aa = syst_eff4d_pt6530y1216_pr_aa[ibin-1][ifile];
              if( syst_eff4d_pt6530y1216_pr_pp[ibin-1][ifile] > eff4dContribution_pt6530y1216_pr_pp ) eff4dContribution_pt6530y1216_pr_pp = syst_eff4d_pt6530y1216_pr_pp[ibin-1][ifile];
              
              if( syst_eff4d_pt6530y1216_npr_aa[ibin-1][ifile] > eff4dContribution_pt6530y1216_npr_aa ) eff4dContribution_pt6530y1216_npr_aa = syst_eff4d_pt6530y1216_npr_aa[ibin-1][ifile];
              if( syst_eff4d_pt6530y1216_npr_pp[ibin-1][ifile] > eff4dContribution_pt6530y1216_npr_pp ) eff4dContribution_pt6530y1216_npr_pp = syst_eff4d_pt6530y1216_npr_pp[ibin-1][ifile];
            }
            if(method==0 || method==2) // add in quadrature all independent variations
            {
              eff4dContribution_pt6530y1216_pr_aa += syst_eff4d_pt6530y1216_pr_aa[ibin-1][ifile];
              eff4dContribution_pt6530y1216_pr_pp += syst_eff4d_pt6530y1216_pr_pp[ibin-1][ifile];
              
              eff4dContribution_pt6530y1216_npr_aa += syst_eff4d_pt6530y1216_npr_aa[ibin-1][ifile];
              eff4dContribution_pt6530y1216_npr_pp += syst_eff4d_pt6530y1216_npr_pp[ibin-1][ifile];
            }
            if(bDoDebug) cout << "+++++++++++++++++++++++++++++++++ Fit contribution to systm: "
                              << eff4dContribution_pt6530y1216_pr_aa << "\t& " << eff4dContribution_pt6530y1216_npr_aa << "\t; "
                              << eff4dContribution_pt6530y1216_pr_pp << "\t& " << eff4dContribution_pt6530y1216_npr_pp << endl;
          }
          if( (ivar>=(nFitVariations+nEff4DVariations)) && (ivar < (nFitVariations+nEff4DVariations+nEffTnPVariation)) )
          {
            int ifile = ivar-nFitVariations-nEff4DVariations; 
            syst_effTnP_pt6530y1216_pr_aa[ibin-1][ifile]  = TMath::Power( yieldVar_aa_pr ,2 ) ;
            syst_effTnP_pt6530y1216_pr_pp[ibin-1][ifile]  = TMath::Power( yieldVar_pp_pr ,2 ) ;
              
            syst_effTnP_pt6530y1216_npr_aa[ibin-1][ifile]  = TMath::Power( yieldVar_aa_npr ,2) ;
            syst_effTnP_pt6530y1216_npr_pp[ibin-1][ifile]  = TMath::Power( yieldVar_pp_npr ,2) ;
            if(bDoDebug) cout << "++++++++++++++++++++++ Delta yields: "
                              << syst_effTnP_pt6530y1216_pr_aa[ibin-1][ifile] << "\t& " << syst_effTnP_pt6530y1216_npr_aa[ibin-1][ifile] << "\t; "
                              << syst_effTnP_pt6530y1216_pr_pp[ibin-1][ifile] << "\t& " << syst_effTnP_pt6530y1216_npr_pp[ibin-1][ifile] << endl;
              
            if(method==1)//maximum
            {
              if( syst_effTnP_pt6530y1216_pr_aa[ibin-1][ifile] > efftnpContribution_pt6530y1216_pr_aa ) efftnpContribution_pt6530y1216_pr_aa = syst_effTnP_pt6530y1216_pr_aa[ibin-1][ifile];
              if( syst_effTnP_pt6530y1216_pr_pp[ibin-1][ifile] > efftnpContribution_pt6530y1216_pr_pp ) efftnpContribution_pt6530y1216_pr_pp = syst_effTnP_pt6530y1216_pr_pp[ibin-1][ifile];
              
              if( syst_effTnP_pt6530y1216_npr_aa[ibin-1][ifile] > efftnpContribution_pt6530y1216_npr_aa ) efftnpContribution_pt6530y1216_npr_aa = syst_effTnP_pt6530y1216_npr_aa[ibin-1][ifile];
              if( syst_effTnP_pt6530y1216_npr_pp[ibin-1][ifile] > efftnpContribution_pt6530y1216_npr_pp ) efftnpContribution_pt6530y1216_npr_pp = syst_effTnP_pt6530y1216_npr_pp[ibin-1][ifile];
            }
            if(method==0 || method==2) // add in quadrature all independent variations
            {
              efftnpContribution_pt6530y1216_pr_aa += syst_effTnP_pt6530y1216_pr_aa[ibin-1][ifile];
              efftnpContribution_pt6530y1216_pr_pp += syst_effTnP_pt6530y1216_pr_pp[ibin-1][ifile];
                
              efftnpContribution_pt6530y1216_npr_aa += syst_effTnP_pt6530y1216_npr_aa[ibin-1][ifile];
              efftnpContribution_pt6530y1216_npr_pp += syst_effTnP_pt6530y1216_npr_pp[ibin-1][ifile];
            }
            if(bDoDebug) cout << "+++++++++++++++++++++++++++++++++ Fit contribution to systm: "
                              << efftnpContribution_pt6530y1216_pr_aa << "\t& " << efftnpContribution_pt6530y1216_npr_aa << "\t; "
                              << efftnpContribution_pt6530y1216_pr_pp << "\t& " << efftnpContribution_pt6530y1216_npr_pp << endl;
          }
          break;

        case 4:// high-pt: 1.6<|y|<2.4
          if (ibin > nBinsNpart6) break;
          prJpsi_pt6530y1624_cent[ibin-1]    = yieldRatio_pr;
          nonPrJpsi_pt6530y1624_cent[ibin-1] = yieldRatio_npr;
          
          if(ivar<nFitVariations)
          {
            syst_fit_pt6530y1624_pr_aa[ibin-1][ivar]  = TMath::Power( relVar_aa_pr ,2 ) ;
            syst_fit_pt6530y1624_pr_pp[ibin-1][ivar]  = TMath::Power( relVar_pp_pr ,2) ;
              
            syst_fit_pt6530y1624_npr_aa[ibin-1][ivar]  = TMath::Power( relVar_aa_npr ,2) ;
            syst_fit_pt6530y1624_npr_pp[ibin-1][ivar]  = TMath::Power( relVar_pp_npr ,2) ;
            if(bDoDebug) cout << "++++++++++++++++++++++ Delta yields: "
                              << syst_fit_pt6530y1624_pr_aa[ibin-1][ivar] << "\t& " << syst_fit_pt6530y1624_npr_aa[ibin-1][ivar] << "\t; "
                              << syst_fit_pt6530y1624_pr_pp[ibin-1][ivar] << "\t& " << syst_fit_pt6530y1624_npr_pp[ibin-1][ivar] << endl;

            if(method==1)//maximum
            {
              if( syst_fit_pt6530y1624_pr_aa[ibin-1][ivar] > fitContribution_pt6530y1624_pr_aa ) fitContribution_pt6530y1624_pr_aa = syst_fit_pt6530y1624_pr_aa[ibin-1][ivar];
              if( syst_fit_pt6530y1624_pr_pp[ibin-1][ivar] > fitContribution_pt6530y1624_pr_pp ) fitContribution_pt6530y1624_pr_pp = syst_fit_pt6530y1624_pr_pp[ibin-1][ivar];
                
              if( syst_fit_pt6530y1624_npr_aa[ibin-1][ivar] > fitContribution_pt6530y1624_npr_aa ) fitContribution_pt6530y1624_npr_aa = syst_fit_pt6530y1624_npr_aa[ibin-1][ivar];
              if( syst_fit_pt6530y1624_npr_pp[ibin-1][ivar] > fitContribution_pt6530y1624_npr_pp ) fitContribution_pt6530y1624_npr_pp = syst_fit_pt6530y1624_npr_pp[ibin-1][ivar];
            }
            if(method==0 || method==2) // rms of all variations for fit systm.
            {
              fitContribution_pt6530y1624_pr_aa += syst_fit_pt6530y1624_pr_aa[ibin-1][ivar];
              fitContribution_pt6530y1624_pr_pp += syst_fit_pt6530y1624_pr_pp[ibin-1][ivar];
                
              fitContribution_pt6530y1624_npr_aa += syst_fit_pt6530y1624_npr_aa[ibin-1][ivar];
              fitContribution_pt6530y1624_npr_pp += syst_fit_pt6530y1624_npr_pp[ibin-1][ivar];
            }
            if(bDoDebug) cout << "+++++++++++++++++++++++++++++++++ Fit contribution to systm: "
                              << fitContribution_pt6530y1624_pr_aa << "\t& " << fitContribution_pt6530y1624_npr_aa << "\t; "
                              << fitContribution_pt6530y1624_pr_pp << "\t& " << fitContribution_pt6530y1624_npr_pp << endl;
          }
          if(ivar>=nFitVariations && ivar<(nFitVariations+nEff4DVariations) )
          {
            int ifile = ivar-nFitVariations;
            syst_eff4d_pt6530y1624_pr_aa[ibin-1][ifile]  = TMath::Power( relVar_aa_pr,2 ) ;
            syst_eff4d_pt6530y1624_pr_pp[ibin-1][ifile]  = TMath::Power( relVar_pp_pr ,2 ) ;
            
            syst_eff4d_pt6530y1624_npr_aa[ibin-1][ifile]  = TMath::Power( relVar_aa_npr ,2) ;
            syst_eff4d_pt6530y1624_npr_pp[ibin-1][ifile]  = TMath::Power( relVar_pp_npr ,2) ;
            if(bDoDebug) cout << "++++++++++++++++++++++ Delta yields: "
                              << syst_eff4d_pt6530y1624_pr_aa[ibin-1][ifile] << "\t& " << syst_eff4d_pt6530y1624_npr_aa[ibin-1][ifile] << "\t; "
                              << syst_eff4d_pt6530y1624_pr_pp[ibin-1][ifile] << "\t& " << syst_eff4d_pt6530y1624_npr_pp[ibin-1][ifile] << endl;
            
            if(method==1)//maximum
            {
              if( syst_eff4d_pt6530y1624_pr_aa[ibin-1][ifile] > eff4dContribution_pt6530y1624_pr_aa ) eff4dContribution_pt6530y1624_pr_aa = syst_eff4d_pt6530y1624_pr_aa[ibin-1][ifile];
              if( syst_eff4d_pt6530y1624_pr_pp[ibin-1][ifile] > eff4dContribution_pt6530y1624_pr_pp ) eff4dContribution_pt6530y1624_pr_pp = syst_eff4d_pt6530y1624_pr_pp[ibin-1][ifile];
              
              if( syst_eff4d_pt6530y1624_npr_aa[ibin-1][ifile] > eff4dContribution_pt6530y1624_npr_aa ) eff4dContribution_pt6530y1624_npr_aa = syst_eff4d_pt6530y1624_npr_aa[ibin-1][ifile];
              if( syst_eff4d_pt6530y1624_npr_pp[ibin-1][ifile] > eff4dContribution_pt6530y1624_npr_pp ) eff4dContribution_pt6530y1624_npr_pp = syst_eff4d_pt6530y1624_npr_pp[ibin-1][ifile];
            }
            if(method==0 || method==2) // add in quadrature all independent variations
            {
              eff4dContribution_pt6530y1624_pr_aa += syst_eff4d_pt6530y1624_pr_aa[ibin-1][ifile];
              eff4dContribution_pt6530y1624_pr_pp += syst_eff4d_pt6530y1624_pr_pp[ibin-1][ifile];
              
              eff4dContribution_pt6530y1624_npr_aa += syst_eff4d_pt6530y1624_npr_aa[ibin-1][ifile];
              eff4dContribution_pt6530y1624_npr_pp += syst_eff4d_pt6530y1624_npr_pp[ibin-1][ifile];
            }
            if(bDoDebug) cout << "+++++++++++++++++++++++++++++++++ Fit contribution to systm: "
                              << eff4dContribution_pt6530y1624_pr_aa << "\t& " << eff4dContribution_pt6530y1624_npr_aa << "\t; "
                              << eff4dContribution_pt6530y1624_pr_pp << "\t& " << eff4dContribution_pt6530y1624_npr_pp << endl;
          }
          if( (ivar>=(nFitVariations+nEff4DVariations)) && (ivar < (nFitVariations+nEff4DVariations+nEffTnPVariation)) )
          {
            int ifile = ivar-nFitVariations-nEff4DVariations;
            syst_effTnP_pt6530y1624_pr_aa[ibin-1][ifile]  = TMath::Power( yieldVar_aa_pr ,2 ) ;
            syst_effTnP_pt6530y1624_pr_pp[ibin-1][ifile]  = TMath::Power( yieldVar_pp_pr ,2 ) ;
              
            syst_effTnP_pt6530y1624_npr_aa[ibin-1][ifile]  = TMath::Power( yieldVar_aa_npr ,2) ;
            syst_effTnP_pt6530y1624_npr_pp[ibin-1][ifile]  = TMath::Power( yieldVar_pp_npr ,2) ;
            if(bDoDebug) cout << "++++++++++++++++++++++ Delta yields: "
                              << syst_effTnP_pt6530y1624_pr_aa[ibin-1][ifile] << "\t& " << syst_effTnP_pt6530y1624_npr_aa[ibin-1][ifile] << "\t; "
                              << syst_effTnP_pt6530y1624_pr_pp[ibin-1][ifile] << "\t& " << syst_effTnP_pt6530y1624_npr_pp[ibin-1][ifile] << endl;

            if(method==1)//maximum
            {
              if( syst_effTnP_pt6530y1624_pr_aa[ibin-1][ifile] > efftnpContribution_pt6530y1624_pr_aa ) efftnpContribution_pt6530y1624_pr_aa = syst_effTnP_pt6530y1624_pr_aa[ibin-1][ifile];
              if( syst_effTnP_pt6530y1624_pr_pp[ibin-1][ifile] > efftnpContribution_pt6530y1624_pr_pp ) efftnpContribution_pt6530y1624_pr_pp = syst_effTnP_pt6530y1624_pr_pp[ibin-1][ifile];
              
              if( syst_effTnP_pt6530y1624_npr_aa[ibin-1][ifile] > efftnpContribution_pt6530y1624_npr_aa ) efftnpContribution_pt6530y1624_npr_aa = syst_effTnP_pt6530y1624_npr_aa[ibin-1][ifile];
              if( syst_effTnP_pt6530y1624_npr_pp[ibin-1][ifile] > efftnpContribution_pt6530y1624_npr_pp ) efftnpContribution_pt6530y1624_npr_pp = syst_effTnP_pt6530y1624_npr_pp[ibin-1][ifile];
            }
            if(method==0 || method==2)// add in quadrature all independent variations
            {
              efftnpContribution_pt6530y1624_pr_aa += syst_effTnP_pt6530y1624_pr_aa[ibin-1][ifile];
              efftnpContribution_pt6530y1624_pr_pp += syst_effTnP_pt6530y1624_pr_pp[ibin-1][ifile];
                
              efftnpContribution_pt6530y1624_npr_aa += syst_effTnP_pt6530y1624_npr_aa[ibin-1][ifile];
              efftnpContribution_pt6530y1624_npr_pp += syst_effTnP_pt6530y1624_npr_pp[ibin-1][ifile];
            }
            if(bDoDebug) cout << "+++++++++++++++++++++++++++++++++ Fit contribution to systm: "
                              << efftnpContribution_pt6530y1624_pr_aa << "\t& " << efftnpContribution_pt6530y1624_npr_aa << "\t; "
                              << efftnpContribution_pt6530y1624_pr_pp << "\t& " << efftnpContribution_pt6530y1624_npr_pp << endl;
          }
          break;

        }//switch ih==kinematic range 
      }// fit variation ivar (fit, 4dEff, tnp)
      
      double yieldSyst_pr_aa  = 0;
      double yieldSyst_npr_aa = 0;
      double yieldSyst_pr_pp  = 0;
      double yieldSyst_npr_pp = 0;

      double globalSyst_pr    = 0;
      double globalSyst_npr   = 0;
      double globalSyst_pp_pr    = 0;
      double globalSyst_pp_npr   = 0;

      double systLumi      = 0;
      double systSelection = 0;
      double systTrack  = 0;
      for (int iglb=0; iglb<2; iglb++)
      {
        systLumi      += TMath::Power(systLumis[iglb],2);
        systSelection += TMath::Power(systEventSelection[iglb],2);
        systTrack  += TMath::Power(systTracking[iglb],2);
      }
    
      // normalization for rms of the fit variations
      double rms_fitContribNorm = nFitVariations;
      double dRelErrRaw_pr_pp=0, dRelErrRaw_np_pp=0;
      if (method==1 || method==2) rms_fitContribNorm = 1;
      switch(ih){
        case 0:// high-pt , |y|<2.4
          yieldSyst_pr_aa  = (fitContribution_pr_aa/rms_fitContribNorm  + eff4dContribution_pr_aa  + efftnpContribution_pr_aa);
          yieldSyst_npr_aa = (fitContribution_npr_aa/rms_fitContribNorm + eff4dContribution_npr_aa + efftnpContribution_npr_aa);

          yieldSyst_pr_pp  = (fitContribution_pr_pp/rms_fitContribNorm  + eff4dContribution_pr_pp  + efftnpContribution_pr_pp);
          yieldSyst_npr_pp = (fitContribution_npr_pp/rms_fitContribNorm + eff4dContribution_npr_pp + efftnpContribution_npr_pp);

          systErrTotal_pr_aa_y024_pt6530_copy[ibin-1] = phCorr_pr_aa->GetBinContent(ibin) * TMath::Sqrt( yieldSyst_pr_aa );
          systErrTotal_pr_pp_y024_pt6530_copy[ibin-1] = phCorr_pr_pp->GetBinContent(ibin) * TMath::Sqrt( yieldSyst_pr_pp );
          if(ibin <= nBinsNpart6) { // out-of-range bins will be discarded for np
            systErrTotal_npr_aa_y024_pt6530_copy[ibin-1] = phCorr_npr_aa->GetBinContent(ibin) * TMath::Sqrt( yieldSyst_npr_aa );
            systErrTotal_npr_pp_y024_pt6530_copy[ibin-1] = phCorr_npr_pp->GetBinContent(ibin) * TMath::Sqrt( yieldSyst_npr_pp );
          }

          dRelErrRaw_pr_pp  = phRaw_pr_pp->GetBinError(ibin)/phRaw_pr_pp->GetBinContent(ibin);
          dRelErrRaw_np_pp  = phRaw_npr_pp->GetBinError(ibin)/phRaw_npr_pp->GetBinContent(ibin);

          prJpsiErrSyst_cent[ibin-1]    = yieldRatio_pr  * TMath::Sqrt(yieldSyst_pr_aa  + taa12_relerr);
          if(ibin <= nBinsNpart6) { // out-of-range bins will be discarded for np
            nonPrJpsiErrSyst_cent[ibin-1] = yieldRatio_npr * TMath::Sqrt(yieldSyst_npr_aa + taa6_relerr); 

            cout << "\t##### check #####\n";
            cout << yieldSyst_npr_aa << " " << yieldSyst_npr_pp << endl;
            cout << phCorr_npr_aa->GetBinContent(ibin) << " " << phCorr_npr_pp->GetBinContent(ibin) << endl;
            cout << systErrTotal_npr_aa_y024_pt6530_copy[ibin-1] << " " << systErrTotal_npr_pp_y024_pt6530_copy[ibin-1] << endl;
            cout << taa6_relerr << " " << TMath::Sqrt(yieldSyst_npr_aa + taa6_relerr) <<  " " << yieldRatio_npr << endl;
            cout << nonPrJpsiErrSyst_cent[ibin-1] <<endl;
            cout << endl;

          }
        
          if(ibin == 1) { // global syst is same for all bins, don't need to repeat
            if (systBoxType == 0 || systBoxType == 2) {
              globalSyst_pr  = TMath::Sqrt(yieldSyst_pr_pp+systLumi+systSelection+systTrack + pow(dRelErrRaw_pr_pp,2));
              globalSyst_npr = TMath::Sqrt(yieldSyst_npr_pp+systLumi+systSelection+systTrack + pow(dRelErrRaw_np_pp,2));

              lumi_pr_y024_pt6530  = new TBox(375,1-globalSyst_pr,400.0,1+globalSyst_pr);
              lumi_npr_y024_pt6530 = new TBox(375,1-globalSyst_npr,400.0,1+globalSyst_npr);
            } else if (systBoxType == 1) {
              cout << "\t\tglobalSyst_pr and _npr with all 4 parts " << endl;
              cout << "\t\t" << TMath::Sqrt(yieldSyst_pr_pp+systLumi+systSelection+systTrack) << "\t"<< TMath::Sqrt(yieldSyst_npr_pp+systLumi+systSelection+systTrack) << endl;
              cout << "\t\t" << TMath::Sqrt(yieldSyst_pr_pp) << "\t"<< TMath::Sqrt(yieldSyst_npr_pp) << endl;
              cout << "\t\t" << TMath::Sqrt(systLumi+systSelection+systTrack) << "\t"<< TMath::Sqrt(systLumi+systSelection+systTrack) << endl;
              cout << "\t\t" << 1-TMath::Sqrt(yieldSyst_pr_pp + pow(dRelErrRaw_pr_pp,2)) << "\t"<< 1-TMath::Sqrt(yieldSyst_npr_pp + pow(dRelErrRaw_np_pp,2)) << endl;
              cout << "\t\t" << 1+TMath::Sqrt(yieldSyst_pr_pp + pow(dRelErrRaw_pr_pp,2)) << "\t"<< 1+TMath::Sqrt(yieldSyst_npr_pp + pow(dRelErrRaw_np_pp,2)) << endl;
              cout << "\t\t" << 1-TMath::Sqrt(systLumi+systSelection+systTrack) << "\t"<< 1-TMath::Sqrt(systLumi+systSelection+systTrack) << endl;
              cout << "\t\t" << 1+TMath::Sqrt(systLumi+systSelection+systTrack) << "\t"<< 1+TMath::Sqrt(systLumi+systSelection+systTrack) << endl;

              globalSyst_pr = TMath::Sqrt(systLumi+systSelection+systTrack);
              globalSyst_npr = TMath::Sqrt(systLumi+systSelection+systTrack);
              globalSyst_pp_pr = TMath::Sqrt(yieldSyst_pr_pp + pow(dRelErrRaw_pr_pp,2));
              globalSyst_pp_npr = TMath::Sqrt(yieldSyst_npr_pp + pow(dRelErrRaw_np_pp,2));

              lumi_pr_y024_pt6530  = new TBox(375,1-globalSyst_pr,400.0,1+globalSyst_pr);
              lumi_npr_y024_pt6530 = new TBox(375,1-globalSyst_npr,400.0,1+globalSyst_npr);
              globalpp_pr_y024_pt6530  = new TBox(375,1-globalSyst_pp_pr,400.0,1+globalSyst_pp_pr);
              globalpp_npr_y024_pt6530 = new TBox(375,1-globalSyst_pp_npr,400.0,1+globalSyst_pp_npr);
            }
          }
          
          outputData_pr << "65300\t" << "0024\t" << centbins_str[ibin-1] << "\t" << yieldRatio_pr << "\t" << prJpsiErrSyst_cent[ibin-1] << "\t"
            << TMath::Sqrt(syst_effTnP_pr_pp[ibin-1][1]) << "\t" << TMath::Sqrt(syst_effTnP_pr_pp[ibin-1][2]) << "\t" << TMath::Sqrt(syst_effTnP_pr_pp[ibin-1][1]+syst_effTnP_pr_pp[ibin-1][2]) << "\t"
            << TMath::Sqrt(syst_effTnP_pr_pp[ibin-1][3]) << "\t" << TMath::Sqrt(syst_effTnP_pr_pp[ibin-1][4]) << "\t" << TMath::Sqrt(syst_effTnP_pr_pp[ibin-1][3]+syst_effTnP_pr_pp[ibin-1][4]) << "\t"
            << TMath::Sqrt(syst_effTnP_pr_pp[ibin-1][0]) << "\t" << TMath::Sqrt(syst_eff4d_pr_pp[ibin-1][0]) << "\t"
            << TMath::Sqrt(fitContribution_pr_pp/rms_fitContribNorm) << "\t"
            << TMath::Sqrt(TMath::Power(systLumis[0],2) + TMath::Power(systEventSelection[0],2) + TMath::Power(systTracking[0],2)) << "\t"
            << TMath::Sqrt(syst_effTnP_pr_aa[ibin-1][1]) << "\t" << TMath::Sqrt(syst_effTnP_pr_aa[ibin-1][2]) << "\t" << TMath::Sqrt(syst_effTnP_pr_aa[ibin-1][1]+syst_effTnP_pr_aa[ibin-1][2]) << "\t"
            << TMath::Sqrt(syst_effTnP_pr_aa[ibin-1][3]) << "\t" << TMath::Sqrt(syst_effTnP_pr_aa[ibin-1][4]) << "\t" << TMath::Sqrt(syst_effTnP_pr_aa[ibin-1][3]+syst_effTnP_pr_aa[ibin-1][4]) << "\t"
            << TMath::Sqrt(syst_effTnP_pr_aa[ibin-1][0]) << "\t" << TMath::Sqrt(syst_eff4d_pr_aa[ibin-1][0]) << "\t"
            << TMath::Sqrt(fitContribution_pr_aa/rms_fitContribNorm) << "\t"
            << TMath::Sqrt(TMath::Power(systLumis[1],2) + TMath::Power(systEventSelection[1],2) + TMath::Power(systTracking[1],2)) << endl;
          if(ibin <= nBinsNpart6) { // out-of-range bins will be discarded for np
            outputData_npr << "65300\t" << "0024\t" << centbins_6bins_str[ibin-1] << "\t" << yieldRatio_npr << "\t" << nonPrJpsiErrSyst_cent[ibin-1] << "\t"
              << TMath::Sqrt(syst_effTnP_npr_pp[ibin-1][1]) << "\t" << TMath::Sqrt(syst_effTnP_npr_pp[ibin-1][2]) << "\t" << TMath::Sqrt(syst_effTnP_npr_pp[ibin-1][1]+syst_effTnP_npr_pp[ibin-1][2]) << "\t"
              << TMath::Sqrt(syst_effTnP_npr_pp[ibin-1][3]) << "\t" << TMath::Sqrt(syst_effTnP_npr_pp[ibin-1][4]) << "\t" << TMath::Sqrt(syst_effTnP_npr_pp[ibin-1][3]+syst_effTnP_npr_pp[ibin-1][4]) << "\t"
              << TMath::Sqrt(syst_effTnP_npr_pp[ibin-1][0]) << "\t" << TMath::Sqrt(syst_eff4d_npr_pp[ibin-1][0]) << "\t"
              << TMath::Sqrt(fitContribution_npr_pp/rms_fitContribNorm) << "\t"
              << TMath::Sqrt(TMath::Power(systLumis[0],2) + TMath::Power(systEventSelection[0],2) + TMath::Power(systTracking[0],2)) << "\t"
              << TMath::Sqrt(syst_effTnP_npr_aa[ibin-1][1]) << "\t" << TMath::Sqrt(syst_effTnP_npr_aa[ibin-1][2]) << "\t" << TMath::Sqrt(syst_effTnP_npr_aa[ibin-1][1]+syst_effTnP_npr_aa[ibin-1][2]) << "\t"
              << TMath::Sqrt(syst_effTnP_npr_aa[ibin-1][3]) << "\t" << TMath::Sqrt(syst_effTnP_npr_aa[ibin-1][4]) << "\t" << TMath::Sqrt(syst_effTnP_npr_aa[ibin-1][3]+syst_effTnP_npr_aa[ibin-1][4]) << "\t"
              << TMath::Sqrt(syst_effTnP_npr_aa[ibin-1][0]) << "\t" << TMath::Sqrt(syst_eff4d_npr_aa[ibin-1][0]) << "\t"
              << TMath::Sqrt(fitContribution_npr_aa/rms_fitContribNorm) << "\t"
              << TMath::Sqrt(TMath::Power(systLumis[1],2) + TMath::Power(systEventSelection[1],2) + TMath::Power(systTracking[1],2)) << endl;
          }
          if(bDoDebug)
          {
            cout <<"---------------------------------------------------------------"<<endl;
            cout << "Ingredients to the total systm. uncertainty"<<endl;
            cout <<"Prompt yields systematics: aa & pp"<<endl;
            cout <<"fitContribution: "<<fitContribution_pr_aa<<"\t"<<fitContribution_pr_pp<<endl;
            cout <<"eff4dContribution: "<<eff4dContribution_pr_aa<<"\t"<<eff4dContribution_pr_pp<<endl;
            cout <<"efftnpContribution: "<<efftnpContribution_pr_aa<<"\t"<<efftnpContribution_pr_pp<<endl;
            cout <<"yields: "<<yield_aa_pr<<"\t"<<yield_pp_pr<<endl;
            cout <<"yield ratio: "<<yieldRatio_pr<<endl;
            cout <<"Total: "<<prJpsiErrSyst_cent[ibin-1]<<endl;
          }
          break;
        
        case 1:// low=pt, 1.6<|y|<2.4
          if (ibin > nBinsNpart6) break;
          yieldSyst_pr_aa  = (fitContribution_pt365y1624_pr_aa/rms_fitContribNorm+eff4dContribution_pt365y1624_pr_aa+efftnpContribution_pt365y1624_pr_aa);
          yieldSyst_npr_aa = (fitContribution_pt365y1624_npr_aa/rms_fitContribNorm+eff4dContribution_pt365y1624_npr_aa+efftnpContribution_pt365y1624_npr_aa);

          yieldSyst_pr_pp  = (fitContribution_pt365y1624_pr_pp/rms_fitContribNorm+eff4dContribution_pt365y1624_pr_pp+efftnpContribution_pt365y1624_pr_pp);
          yieldSyst_npr_pp = (fitContribution_pt365y1624_npr_pp/rms_fitContribNorm+eff4dContribution_pt365y1624_npr_pp+efftnpContribution_pt365y1624_npr_pp);

          systErrTotal_pr_aa_y1624_pt365_copy[ibin-1] = phCorr_pr_aa->GetBinContent(ibin) * TMath::Sqrt( yieldSyst_pr_aa );
          systErrTotal_pr_pp_y1624_pt365_copy[ibin-1] = phCorr_pr_pp->GetBinContent(ibin) * TMath::Sqrt( yieldSyst_pr_pp );
          systErrTotal_npr_aa_y1624_pt365_copy[ibin-1] = phCorr_npr_aa->GetBinContent(ibin) * TMath::Sqrt( yieldSyst_npr_aa );
          systErrTotal_npr_pp_y1624_pt365_copy[ibin-1] = phCorr_npr_pp->GetBinContent(ibin) * TMath::Sqrt( yieldSyst_npr_pp );

          dRelErrRaw_pr_pp  = phRaw_pr_pp->GetBinError(ibin)/phRaw_pr_pp->GetBinContent(ibin);
          dRelErrRaw_np_pp  = phRaw_npr_pp->GetBinError(ibin)/phRaw_npr_pp->GetBinContent(ibin);

          prJpsiErrSyst_pt365y1624_cent[ibin-1]    = yieldRatio_pr  * TMath::Sqrt(yieldSyst_pr_aa  + taa6_relerr );
          nonPrJpsiErrSyst_pt365y1624_cent[ibin-1] = yieldRatio_npr * TMath::Sqrt(yieldSyst_npr_aa + taa6_relerr ); 
        
          if(ibin == 1) { // global syst is same for all bins, don't need to repeat
            if (systBoxType == 2) {
              globalSyst_pr  = TMath::Sqrt(yieldSyst_pr_pp+systLumi+systSelection+systTrack + pow(dRelErrRaw_pr_pp,2));
              globalSyst_npr = TMath::Sqrt(yieldSyst_npr_pp+systLumi+systSelection+systTrack + pow(dRelErrRaw_np_pp,2));

              lumi_pr_y1624_pt365  = new TBox(375,1-globalSyst_pr,400.0,1+globalSyst_pr);
              lumi_npr_y1624_pt365 = new TBox(375,1-globalSyst_npr,400.0,1+globalSyst_npr);
            } else if (systBoxType == 0 || systBoxType == 1) {
              globalSyst_pr = TMath::Sqrt(systLumi+systSelection+systTrack);
              globalSyst_npr = TMath::Sqrt(systLumi+systSelection+systTrack);
              globalSyst_pp_pr = TMath::Sqrt(yieldSyst_pr_pp + pow(dRelErrRaw_pr_pp,2));
              globalSyst_pp_npr = TMath::Sqrt(yieldSyst_npr_pp + pow(dRelErrRaw_np_pp,2));

              lumi_pr_y1624_pt365  = new TBox(375,1-globalSyst_pr,400.0,1+globalSyst_pr);
              lumi_npr_y1624_pt365 = new TBox(375,1-globalSyst_npr,400.0,1+globalSyst_npr);
              globalpp_pr_y1624_pt365  = new TBox(375,1-globalSyst_pp_pr,400.0,1+globalSyst_pp_pr);
              globalpp_npr_y1624_pt365 = new TBox(375,1-globalSyst_pp_npr,400.0,1+globalSyst_pp_npr);

              cout << "\t\tglobalSyst y1624pt365 _pr and _npr with all 4 parts " << endl;
              cout << "\t\t" << TMath::Sqrt(yieldSyst_pr_pp+systLumi+systSelection+systTrack) << "\t"<< TMath::Sqrt(yieldSyst_npr_pp+systLumi+systSelection+systTrack) << endl;
              cout << "\t\t" << TMath::Sqrt(yieldSyst_pr_pp) << "\t"<< TMath::Sqrt(yieldSyst_npr_pp) << endl;
              cout << "\t\t" << TMath::Sqrt(systLumi+systSelection+systTrack) << "\t"<< TMath::Sqrt(systLumi+systSelection+systTrack) << endl;
              cout << "\t\t" << 1-globalSyst_pp_pr  << "\t"<< 1-globalSyst_pp_npr  << endl;
              cout << "\t\t" << 1+globalSyst_pp_pr  << "\t"<< 1+globalSyst_pp_npr  << endl;
              cout << "\t\t" << 1-globalSyst_pr  << "\t"<< 1-globalSyst_npr  << endl;
              cout << "\t\t" << 1+globalSyst_pr  << "\t"<< 1+globalSyst_npr  << endl;

            }
          }

          outputData_pr << "3065\t" << "1624\t" << centbins_6bins_str[ibin-1] << "\t" << yieldRatio_pr << "\t" << prJpsiErrSyst_pt365y1624_cent[ibin-1] << "\t"
              << TMath::Sqrt(syst_effTnP_pt365y1624_pr_pp[ibin-1][1]) << "\t" << TMath::Sqrt(syst_effTnP_pt365y1624_pr_pp[ibin-1][2]) << "\t" << TMath::Sqrt(syst_effTnP_pt365y1624_pr_pp[ibin-1][1]+syst_effTnP_pt365y1624_pr_pp[ibin-1][2]) << "\t"
              << TMath::Sqrt(syst_effTnP_pt365y1624_pr_pp[ibin-1][3]) << "\t" << TMath::Sqrt(syst_effTnP_pt365y1624_pr_pp[ibin-1][4]) << "\t" << TMath::Sqrt(syst_effTnP_pt365y1624_pr_pp[ibin-1][3]+syst_effTnP_pt365y1624_pr_pp[ibin-1][4]) << "\t"
              << TMath::Sqrt(syst_effTnP_pt365y1624_pr_pp[ibin-1][0]) << "\t" << TMath::Sqrt(syst_eff4d_pt365y1624_pr_pp[ibin-1][0]) << "\t"
              << TMath::Sqrt(fitContribution_pt365y1624_pr_pp/rms_fitContribNorm) << "\t"
              << TMath::Sqrt(TMath::Power(systLumis[0],2) + TMath::Power(systEventSelection[0],2) + TMath::Power(systTracking[0],2)) << "\t"
              << TMath::Sqrt(syst_effTnP_pt365y1624_pr_aa[ibin-1][1]) << "\t" << TMath::Sqrt(syst_effTnP_pt365y1624_pr_aa[ibin-1][2]) << "\t" << TMath::Sqrt(syst_effTnP_pt365y1624_pr_aa[ibin-1][1]+syst_effTnP_pt365y1624_pr_aa[ibin-1][2]) << "\t"
              << TMath::Sqrt(syst_effTnP_pt365y1624_pr_aa[ibin-1][3]) << "\t" << TMath::Sqrt(syst_effTnP_pt365y1624_pr_aa[ibin-1][4]) << "\t" << TMath::Sqrt(syst_effTnP_pt365y1624_pr_aa[ibin-1][3]+syst_effTnP_pt365y1624_pr_aa[ibin-1][4]) << "\t"
              << TMath::Sqrt(syst_effTnP_pt365y1624_pr_aa[ibin-1][0]) << "\t" << TMath::Sqrt(syst_eff4d_pt365y1624_pr_aa[ibin-1][0]) << "\t"
              << TMath::Sqrt(fitContribution_pt365y1624_pr_aa/rms_fitContribNorm) << "\t"
              << TMath::Sqrt(TMath::Power(systLumis[1],2) + TMath::Power(systEventSelection[1],2) + TMath::Power(systTracking[1],2)) << endl;
          outputData_npr << "3065\t" << "1624\t" << centbins_6bins_str[ibin-1] << "\t" << yieldRatio_npr << "\t" << nonPrJpsiErrSyst_pt365y1624_cent[ibin-1] << "\t"
              << TMath::Sqrt(syst_effTnP_pt365y1624_npr_pp[ibin-1][1]) << "\t" << TMath::Sqrt(syst_effTnP_pt365y1624_npr_pp[ibin-1][2]) << "\t" << TMath::Sqrt(syst_effTnP_pt365y1624_npr_pp[ibin-1][1]+syst_effTnP_pt365y1624_npr_pp[ibin-1][2]) << "\t"
              << TMath::Sqrt(syst_effTnP_pt365y1624_npr_pp[ibin-1][3]) << "\t" << TMath::Sqrt(syst_effTnP_pt365y1624_npr_pp[ibin-1][4]) << "\t" << TMath::Sqrt(syst_effTnP_pt365y1624_npr_pp[ibin-1][3]+syst_effTnP_pt365y1624_npr_pp[ibin-1][4]) << "\t"
              << TMath::Sqrt(syst_effTnP_pt365y1624_npr_pp[ibin-1][0]) << "\t" << TMath::Sqrt(syst_eff4d_pt365y1624_npr_pp[ibin-1][0]) << "\t"
              << TMath::Sqrt(fitContribution_pt365y1624_npr_pp/rms_fitContribNorm) << "\t"
              << TMath::Sqrt(TMath::Power(systLumis[0],2) + TMath::Power(systEventSelection[0],2) + TMath::Power(systTracking[0],2)) << "\t"
              << TMath::Sqrt(syst_effTnP_pt365y1624_npr_aa[ibin-1][1]) << "\t" << TMath::Sqrt(syst_effTnP_pt365y1624_npr_aa[ibin-1][2]) << "\t" << TMath::Sqrt(syst_effTnP_pt365y1624_npr_aa[ibin-1][1]+syst_effTnP_pt365y1624_npr_aa[ibin-1][2]) << "\t"
              << TMath::Sqrt(syst_effTnP_pt365y1624_npr_aa[ibin-1][3]) << "\t" << TMath::Sqrt(syst_effTnP_pt365y1624_npr_aa[ibin-1][4]) << "\t" << TMath::Sqrt(syst_effTnP_pt365y1624_npr_aa[ibin-1][3]+syst_effTnP_pt365y1624_npr_aa[ibin-1][4]) << "\t"
              << TMath::Sqrt(syst_effTnP_pt365y1624_npr_aa[ibin-1][0]) << "\t" << TMath::Sqrt(syst_eff4d_pt365y1624_npr_aa[ibin-1][0]) << "\t"
              << TMath::Sqrt(fitContribution_pt365y1624_npr_aa/rms_fitContribNorm) << "\t"
              << TMath::Sqrt(TMath::Power(systLumis[1],2) + TMath::Power(systEventSelection[1],2) + TMath::Power(systTracking[1],2)) << endl;
          if(bDoDebug)
          {
            cout <<"---------------------------------------------------------------"<<endl;
            cout <<"Ingredients to the total systm. uncertainty"<<endl;
            cout <<"Prompt yields systematics: aa & pp"<<endl;
            cout <<"fitContribution: "<<fitContribution_pt365y1624_pr_aa<<"\t"<<fitContribution_pt365y1624_pr_pp<<endl;
            cout <<"eff4dContribution: "<<eff4dContribution_pt365y1624_pr_aa<<"\t"<<eff4dContribution_pt365y1624_pr_pp<<endl;
            cout <<"efftnpContribution: "<<efftnpContribution_pt365y1624_pr_aa<<"\t"<<efftnpContribution_pt365y1624_pr_pp<<endl;
            cout <<"fitContribution: (np) "<<fitContribution_pt365y1624_npr_aa<<"\t"<<fitContribution_pt365y1624_npr_pp<<endl;
            cout <<"eff4dContribution: (np) "<<eff4dContribution_pt365y1624_npr_aa<<"\t"<<eff4dContribution_pt365y1624_npr_pp<<endl;
            cout <<"efftnpContribution: (np) "<<efftnpContribution_pt365y1624_npr_aa<<"\t"<<efftnpContribution_pt365y1624_npr_pp<<endl;
            cout <<"yields: "<<yield_aa_pr<<"\t"<<yield_pp_pr<<"\t"<<yield_aa_npr<<"\t"<<yield_pp_npr<<endl;
            cout <<"yield ratio: "<<yieldRatio_pr<<"\t"<<yieldRatio_npr<<endl;
            cout <<"taa6_relerr: "<<taa6_relerr<<endl;
            cout <<"Total: (pr) "<<prJpsiErrSyst_pt365y1624_cent[ibin-1]<<"\t and (nonPr) "<<nonPrJpsiErrSyst_pt365y1624_cent[ibin-1]<<endl;
          }
          break;
        
        case 2:// high=pt, |y|<1.2
          if (ibin > nBinsNpart6) break;
          yieldSyst_pr_aa  = (fitContribution_pt6530y012_pr_aa/rms_fitContribNorm+eff4dContribution_pt6530y012_pr_aa+efftnpContribution_pt6530y012_pr_aa);
          yieldSyst_npr_aa = (fitContribution_pt6530y012_npr_aa/rms_fitContribNorm+eff4dContribution_pt6530y012_npr_aa+efftnpContribution_pt6530y012_npr_aa);

          yieldSyst_pr_pp  = (fitContribution_pt6530y012_pr_pp/rms_fitContribNorm+eff4dContribution_pt6530y012_pr_pp+efftnpContribution_pt6530y012_pr_pp);
          yieldSyst_npr_pp = (fitContribution_pt6530y012_npr_pp/rms_fitContribNorm+eff4dContribution_pt6530y012_npr_pp+efftnpContribution_pt6530y012_npr_pp);

          systErrTotal_pr_aa_y012_pt6530_copy[ibin-1] = phCorr_pr_aa->GetBinContent(ibin) * TMath::Sqrt( yieldSyst_pr_aa );
          systErrTotal_pr_pp_y012_pt6530_copy[ibin-1] = phCorr_pr_pp->GetBinContent(ibin) * TMath::Sqrt( yieldSyst_pr_pp );
          systErrTotal_npr_aa_y012_pt6530_copy[ibin-1] = phCorr_npr_aa->GetBinContent(ibin) * TMath::Sqrt( yieldSyst_npr_aa );
          systErrTotal_npr_pp_y012_pt6530_copy[ibin-1] = phCorr_npr_pp->GetBinContent(ibin) * TMath::Sqrt( yieldSyst_npr_pp );

          dRelErrRaw_pr_pp  = phRaw_pr_pp->GetBinError(ibin)/phRaw_pr_pp->GetBinContent(ibin);
          dRelErrRaw_np_pp  = phRaw_npr_pp->GetBinError(ibin)/phRaw_npr_pp->GetBinContent(ibin);

          prJpsiErrSyst_pt6530y012_cent[ibin-1]    = yieldRatio_pr  * TMath::Sqrt(yieldSyst_pr_aa  + taa6_relerr );
          nonPrJpsiErrSyst_pt6530y012_cent[ibin-1] = yieldRatio_npr * TMath::Sqrt(yieldSyst_npr_aa + taa6_relerr ); 
        
          if(ibin == 1) { // global syst is same for all bins, don't need to repeat
            if (systBoxType == 2) {
              globalSyst_pr  = TMath::Sqrt(yieldSyst_pr_pp+systLumi+systSelection+systTrack + pow(dRelErrRaw_pr_pp,2));
              globalSyst_npr = TMath::Sqrt(yieldSyst_npr_pp+systLumi+systSelection+systTrack + pow(dRelErrRaw_np_pp,2));

              lumi_pr_y012_pt6530  = new TBox(325,1-globalSyst_pr,350.0,1+globalSyst_pr);
              lumi_npr_y012_pt6530 = new TBox(325,1-globalSyst_npr,350.0,1+globalSyst_npr);
            } else if (systBoxType == 0 || systBoxType == 1) {
              globalSyst_pr = TMath::Sqrt(systLumi+systSelection+systTrack);
              globalSyst_npr = TMath::Sqrt(systLumi+systSelection+systTrack);
              globalSyst_pp_pr = TMath::Sqrt(yieldSyst_pr_pp + pow(dRelErrRaw_pr_pp,2));
              globalSyst_pp_npr = TMath::Sqrt(yieldSyst_npr_pp + pow(dRelErrRaw_np_pp,2));

              lumi_pr_y012_pt6530  = new TBox(325,1-globalSyst_pr,400.0,1+globalSyst_pr);
              lumi_npr_y012_pt6530 = new TBox(325,1-globalSyst_npr,400.0,1+globalSyst_npr);
              globalpp_pr_y012_pt6530  = new TBox(325,1-globalSyst_pp_pr,350,1+globalSyst_pp_pr);
              globalpp_npr_y012_pt6530 = new TBox(325,1-globalSyst_pp_npr,350.0,1+globalSyst_pp_npr);
              
              cout << "\t\tglobalSyst y012pt6530 _pr and _npr with all 4 parts " << endl;
              cout << "\t\t" << TMath::Sqrt(yieldSyst_pr_pp+systLumi+systSelection+systTrack) << "\t"<< TMath::Sqrt(yieldSyst_npr_pp+systLumi+systSelection+systTrack) << endl;
              cout << "\t\t" << TMath::Sqrt(yieldSyst_pr_pp) << "\t"<< TMath::Sqrt(yieldSyst_npr_pp) << endl;
              cout << "\t\t" << TMath::Sqrt(systLumi+systSelection+systTrack) << "\t"<< TMath::Sqrt(systLumi+systSelection+systTrack) << endl;
              cout << "\t\t" << 1-globalSyst_pp_pr  << "\t"<< 1-globalSyst_pp_npr  << endl;
              cout << "\t\t" << 1+globalSyst_pp_pr  << "\t"<< 1+globalSyst_pp_npr  << endl;
              cout << "\t\t" << 1-globalSyst_pr  << "\t"<< 1-globalSyst_npr  << endl;
              cout << "\t\t" << 1+globalSyst_pr  << "\t"<< 1+globalSyst_npr  << endl;
            }
          }
          
          outputData_pr << "65300\t" << "012\t" << centbins_6bins_str[ibin-1] << "\t" << yieldRatio_pr << "\t" << prJpsiErrSyst_pt6530y012_cent[ibin-1] << "\t"
              << TMath::Sqrt(syst_effTnP_pt6530y012_pr_pp[ibin-1][1]) << "\t" << TMath::Sqrt(syst_effTnP_pt6530y012_pr_pp[ibin-1][2]) << "\t" << TMath::Sqrt(syst_effTnP_pt6530y012_pr_pp[ibin-1][1]+syst_effTnP_pt6530y012_pr_pp[ibin-1][2]) << "\t"
              << TMath::Sqrt(syst_effTnP_pt6530y012_pr_pp[ibin-1][3]) << "\t" << TMath::Sqrt(syst_effTnP_pt6530y012_pr_pp[ibin-1][4]) << "\t" << TMath::Sqrt(syst_effTnP_pt6530y012_pr_pp[ibin-1][3]+syst_effTnP_pt6530y012_pr_pp[ibin-1][4]) << "\t"
              << TMath::Sqrt(syst_effTnP_pt6530y012_pr_pp[ibin-1][0]) << "\t" << TMath::Sqrt(syst_eff4d_pt6530y012_pr_pp[ibin-1][0]) << "\t"
              << TMath::Sqrt(fitContribution_pt6530y012_pr_pp/rms_fitContribNorm) << "\t"
              << TMath::Sqrt(TMath::Power(systLumis[0],2) + TMath::Power(systEventSelection[0],2) + TMath::Power(systTracking[0],2)) << "\t"
              << TMath::Sqrt(syst_effTnP_pt6530y012_pr_aa[ibin-1][1]) << "\t" << TMath::Sqrt(syst_effTnP_pt6530y012_pr_aa[ibin-1][2]) << "\t" << TMath::Sqrt(syst_effTnP_pt6530y012_pr_aa[ibin-1][1]+syst_effTnP_pt6530y012_pr_aa[ibin-1][2]) << "\t"
              << TMath::Sqrt(syst_effTnP_pt6530y012_pr_aa[ibin-1][3]) << "\t" << TMath::Sqrt(syst_effTnP_pt6530y012_pr_aa[ibin-1][4]) << "\t" << TMath::Sqrt(syst_effTnP_pt6530y012_pr_aa[ibin-1][3]+syst_effTnP_pt6530y012_pr_aa[ibin-1][4]) << "\t"
              << TMath::Sqrt(syst_effTnP_pt6530y012_pr_aa[ibin-1][0]) << "\t" << TMath::Sqrt(syst_eff4d_pt6530y012_pr_aa[ibin-1][0]) << "\t"
              << TMath::Sqrt(fitContribution_pt6530y012_pr_aa/rms_fitContribNorm) << "\t"
              << TMath::Sqrt(TMath::Power(systLumis[1],2) + TMath::Power(systEventSelection[1],2) + TMath::Power(systTracking[1],2)) << endl;
          outputData_npr << "65300\t" << "012\t" << centbins_6bins_str[ibin-1] << "\t" << yieldRatio_npr << "\t" << nonPrJpsiErrSyst_pt6530y012_cent[ibin-1] << "\t"
              << TMath::Sqrt(syst_effTnP_pt6530y012_npr_pp[ibin-1][1]) << "\t" << TMath::Sqrt(syst_effTnP_pt6530y012_npr_pp[ibin-1][2]) << "\t" << TMath::Sqrt(syst_effTnP_pt6530y012_npr_pp[ibin-1][1]+syst_effTnP_pt6530y012_npr_pp[ibin-1][2]) << "\t"
              << TMath::Sqrt(syst_effTnP_pt6530y012_npr_pp[ibin-1][3]) << "\t" << TMath::Sqrt(syst_effTnP_pt6530y012_npr_pp[ibin-1][4]) << "\t" << TMath::Sqrt(syst_effTnP_pt6530y012_npr_pp[ibin-1][3]+syst_effTnP_pt6530y012_npr_pp[ibin-1][4]) << "\t"
              << TMath::Sqrt(syst_effTnP_pt6530y012_npr_pp[ibin-1][0]) << "\t" << TMath::Sqrt(syst_eff4d_pt6530y012_npr_pp[ibin-1][0]) << "\t"
              << TMath::Sqrt(fitContribution_pt6530y012_npr_pp/rms_fitContribNorm) << "\t"
              << TMath::Sqrt(TMath::Power(systLumis[0],2) + TMath::Power(systEventSelection[0],2) + TMath::Power(systTracking[0],2)) << "\t"
              << TMath::Sqrt(syst_effTnP_pt6530y012_npr_aa[ibin-1][1]) << "\t" << TMath::Sqrt(syst_effTnP_pt6530y012_npr_aa[ibin-1][2]) << "\t" << TMath::Sqrt(syst_effTnP_pt6530y012_npr_aa[ibin-1][1]+syst_effTnP_pt6530y012_npr_aa[ibin-1][2]) << "\t"
              << TMath::Sqrt(syst_effTnP_pt6530y012_npr_aa[ibin-1][3]) << "\t" << TMath::Sqrt(syst_effTnP_pt6530y012_npr_aa[ibin-1][4]) << "\t" << TMath::Sqrt(syst_effTnP_pt6530y012_npr_aa[ibin-1][3]+syst_effTnP_pt6530y012_npr_aa[ibin-1][4]) << "\t"
              << TMath::Sqrt(syst_effTnP_pt6530y012_npr_aa[ibin-1][0]) << "\t" << TMath::Sqrt(syst_eff4d_pt6530y012_npr_aa[ibin-1][0]) << "\t"
              << TMath::Sqrt(fitContribution_pt6530y012_npr_aa/rms_fitContribNorm) << "\t"
              << TMath::Sqrt(TMath::Power(systLumis[1],2) + TMath::Power(systEventSelection[1],2) + TMath::Power(systTracking[1],2)) << endl;
          if(bDoDebug)
          {
            cout <<"---------------------------------------------------------------"<<endl;
            cout << "Ingredients to the total systm. uncertainty"<<endl;
            cout <<"Prompt yields systematics: aa & pp"<<endl;
            cout <<"fitContribution: "<<fitContribution_pt6530y012_pr_aa<<"\t"<<fitContribution_pt6530y012_pr_pp<<endl;
            cout <<"eff4dContribution: "<<eff4dContribution_pt6530y012_pr_aa<<"\t"<<eff4dContribution_pt6530y012_pr_pp<<endl;
            cout <<"efftnpContribution: "<<efftnpContribution_pt6530y012_pr_aa<<"\t"<<efftnpContribution_pt6530y012_pr_pp<<endl;
            cout <<"yields: "<<yield_aa_pr<<"\t"<<yield_pp_pr<<endl;
            cout <<"yield ratio: "<<yieldRatio_pr<<endl;
            cout <<"Total: "<<prJpsiErrSyst_pt6530y012_cent[ibin-1]<<endl;
          }
          break;

        case 3:// high=pt, 1.2<|y|<1.6
          if (ibin > nBinsNpart6) break;
          yieldSyst_pr_aa  = (fitContribution_pt6530y1216_pr_aa/rms_fitContribNorm+eff4dContribution_pt6530y1216_pr_aa+efftnpContribution_pt6530y1216_pr_aa);
          yieldSyst_npr_aa = (fitContribution_pt6530y1216_npr_aa/rms_fitContribNorm+eff4dContribution_pt6530y1216_npr_aa+efftnpContribution_pt6530y1216_npr_aa);

          yieldSyst_pr_pp  = (fitContribution_pt6530y1216_pr_pp/rms_fitContribNorm+eff4dContribution_pt6530y1216_pr_pp+efftnpContribution_pt6530y1216_pr_pp);
          yieldSyst_npr_pp = (fitContribution_pt6530y1216_npr_pp/rms_fitContribNorm+eff4dContribution_pt6530y1216_npr_pp+efftnpContribution_pt6530y1216_npr_pp);

          systErrTotal_pr_aa_y1216_pt6530_copy[ibin-1] = phCorr_pr_aa->GetBinContent(ibin) * TMath::Sqrt( yieldSyst_pr_aa );
          systErrTotal_pr_pp_y1216_pt6530_copy[ibin-1] = phCorr_pr_pp->GetBinContent(ibin) * TMath::Sqrt( yieldSyst_pr_pp );
          systErrTotal_npr_aa_y1216_pt6530_copy[ibin-1] = phCorr_npr_aa->GetBinContent(ibin) * TMath::Sqrt( yieldSyst_npr_aa );
          systErrTotal_npr_pp_y1216_pt6530_copy[ibin-1] = phCorr_npr_pp->GetBinContent(ibin) * TMath::Sqrt( yieldSyst_npr_pp );

          dRelErrRaw_pr_pp  = phRaw_pr_pp->GetBinError(ibin)/phRaw_pr_pp->GetBinContent(ibin);
          dRelErrRaw_np_pp  = phRaw_npr_pp->GetBinError(ibin)/phRaw_npr_pp->GetBinContent(ibin);

          prJpsiErrSyst_pt6530y1216_cent[ibin-1]    = yieldRatio_pr  * TMath::Sqrt(yieldSyst_pr_aa  + taa6_relerr);
          nonPrJpsiErrSyst_pt6530y1216_cent[ibin-1] = yieldRatio_npr * TMath::Sqrt(yieldSyst_npr_aa + taa6_relerr); 
      
          if(ibin == 1) { // global syst is same for all bins, don't need to repeat
            if (systBoxType == 2) {
              globalSyst_pr  = TMath::Sqrt(yieldSyst_pr_pp+systLumi+systSelection+systTrack + pow(dRelErrRaw_pr_pp,2));
              globalSyst_npr = TMath::Sqrt(yieldSyst_npr_pp+systLumi+systSelection+systTrack + pow(dRelErrRaw_np_pp,2));

              lumi_pr_y1216_pt6530  = new TBox(350,1-globalSyst_pr,375,1+globalSyst_pr);
              lumi_npr_y1216_pt6530 = new TBox(350,1-globalSyst_npr,375,1+globalSyst_npr);
            } else if (systBoxType == 0 || systBoxType == 1) {
              globalSyst_pr = TMath::Sqrt(systLumi+systSelection+systTrack);
              globalSyst_npr = TMath::Sqrt(systLumi+systSelection+systTrack);
              globalSyst_pp_pr = TMath::Sqrt(yieldSyst_pr_pp + pow(dRelErrRaw_pr_pp,2));
              globalSyst_pp_npr = TMath::Sqrt(yieldSyst_npr_pp + pow(dRelErrRaw_np_pp,2));

              lumi_pr_y1216_pt6530  = new TBox(350,1-globalSyst_pr,375,1+globalSyst_pr);
              lumi_npr_y1216_pt6530 = new TBox(350,1-globalSyst_npr,375,1+globalSyst_npr);
              globalpp_pr_y1216_pt6530  = new TBox(350,1-globalSyst_pp_pr,375,1+globalSyst_pp_pr);
              globalpp_npr_y1216_pt6530 = new TBox(350,1-globalSyst_pp_npr,375,1+globalSyst_pp_npr);

              cout << "\t\tglobalSyst y1216pt6530 _pr and _npr with all 4 parts " << endl;
              cout << "\t\t" << TMath::Sqrt(yieldSyst_pr_pp+systLumi+systSelection+systTrack) << "\t"<< TMath::Sqrt(yieldSyst_npr_pp+systLumi+systSelection+systTrack) << endl;
              cout << "\t\t" << TMath::Sqrt(yieldSyst_pr_pp) << "\t"<< TMath::Sqrt(yieldSyst_npr_pp) << endl;
              cout << "\t\t" << TMath::Sqrt(systLumi+systSelection+systTrack) << "\t"<< TMath::Sqrt(systLumi+systSelection+systTrack) << endl;
              cout << "\t\t" << 1-globalSyst_pp_pr  << "\t"<< 1-globalSyst_pp_npr  << endl;
              cout << "\t\t" << 1+globalSyst_pp_pr  << "\t"<< 1+globalSyst_pp_npr  << endl;
              cout << "\t\t" << 1-globalSyst_pr  << "\t"<< 1-globalSyst_npr  << endl;
              cout << "\t\t" << 1+globalSyst_pr  << "\t"<< 1+globalSyst_npr  << endl;
            }
          }

          outputData_pr << "65300\t" << "1216\t" << centbins_6bins_str[ibin-1] << "\t" << yieldRatio_pr << "\t" << prJpsiErrSyst_pt6530y1216_cent[ibin-1] << "\t"
              << TMath::Sqrt(syst_effTnP_pt6530y1216_pr_pp[ibin-1][1]) << "\t" << TMath::Sqrt(syst_effTnP_pt6530y1216_pr_pp[ibin-1][2]) << "\t" << TMath::Sqrt(syst_effTnP_pt6530y1216_pr_pp[ibin-1][1]+syst_effTnP_pt6530y1216_pr_pp[ibin-1][2]) << "\t"
              << TMath::Sqrt(syst_effTnP_pt6530y1216_pr_pp[ibin-1][3]) << "\t" << TMath::Sqrt(syst_effTnP_pt6530y1216_pr_pp[ibin-1][4]) << "\t" << TMath::Sqrt(syst_effTnP_pt6530y1216_pr_pp[ibin-1][3]+syst_effTnP_pt6530y1216_pr_pp[ibin-1][4]) << "\t"
              << TMath::Sqrt(syst_effTnP_pt6530y1216_pr_pp[ibin-1][0]) << "\t" << TMath::Sqrt(syst_eff4d_pt6530y1216_pr_pp[ibin-1][0]) << "\t"
              << TMath::Sqrt(fitContribution_pt6530y1216_pr_pp/rms_fitContribNorm) << "\t"
              << TMath::Sqrt(TMath::Power(systLumis[0],2) + TMath::Power(systEventSelection[0],2) + TMath::Power(systTracking[0],2)) << "\t"
              << TMath::Sqrt(syst_effTnP_pt6530y1216_pr_aa[ibin-1][1]) << "\t" << TMath::Sqrt(syst_effTnP_pt6530y1216_pr_aa[ibin-1][2]) << "\t" << TMath::Sqrt(syst_effTnP_pt6530y1216_pr_aa[ibin-1][1]+syst_effTnP_pt6530y1216_pr_aa[ibin-1][2]) << "\t"
              << TMath::Sqrt(syst_effTnP_pt6530y1216_pr_aa[ibin-1][3]) << "\t" << TMath::Sqrt(syst_effTnP_pt6530y1216_pr_aa[ibin-1][4]) << "\t" << TMath::Sqrt(syst_effTnP_pt6530y1216_pr_aa[ibin-1][3]+syst_effTnP_pt6530y1216_pr_aa[ibin-1][4]) << "\t"
              << TMath::Sqrt(syst_effTnP_pt6530y1216_pr_aa[ibin-1][0]) << "\t" << TMath::Sqrt(syst_eff4d_pt6530y1216_pr_aa[ibin-1][0]) << "\t"
              << TMath::Sqrt(fitContribution_pt6530y1216_pr_aa/rms_fitContribNorm) << "\t"
              << TMath::Sqrt(TMath::Power(systLumis[1],2) + TMath::Power(systEventSelection[1],2) + TMath::Power(systTracking[1],2)) << endl;
          outputData_npr << "65300\t" << "1216\t" << centbins_6bins_str[ibin-1] << "\t" << yieldRatio_npr << "\t" << nonPrJpsiErrSyst_pt6530y1216_cent[ibin-1] << "\t"
              << TMath::Sqrt(syst_effTnP_pt6530y1216_npr_pp[ibin-1][1]) << "\t" << TMath::Sqrt(syst_effTnP_pt6530y1216_npr_pp[ibin-1][2]) << "\t" << TMath::Sqrt(syst_effTnP_pt6530y1216_npr_pp[ibin-1][1]+syst_effTnP_pt6530y1216_npr_pp[ibin-1][2]) << "\t"
              << TMath::Sqrt(syst_effTnP_pt6530y1216_npr_pp[ibin-1][3]) << "\t" << TMath::Sqrt(syst_effTnP_pt6530y1216_npr_pp[ibin-1][4]) << "\t" << TMath::Sqrt(syst_effTnP_pt6530y1216_npr_pp[ibin-1][3]+syst_effTnP_pt6530y1216_npr_pp[ibin-1][4]) << "\t"
              << TMath::Sqrt(syst_effTnP_pt6530y1216_npr_pp[ibin-1][0]) << "\t" << TMath::Sqrt(syst_eff4d_pt6530y1216_npr_pp[ibin-1][0]) << "\t"
              << TMath::Sqrt(fitContribution_pt6530y1216_npr_pp/rms_fitContribNorm) << "\t"
              << TMath::Sqrt(TMath::Power(systLumis[0],2) + TMath::Power(systEventSelection[0],2) + TMath::Power(systTracking[0],2)) << "\t"
              << TMath::Sqrt(syst_effTnP_pt6530y1216_npr_aa[ibin-1][1]) << "\t" << TMath::Sqrt(syst_effTnP_pt6530y1216_npr_aa[ibin-1][2]) << "\t" << TMath::Sqrt(syst_effTnP_pt6530y1216_npr_aa[ibin-1][1]+syst_effTnP_pt6530y1216_npr_aa[ibin-1][2]) << "\t"
              << TMath::Sqrt(syst_effTnP_pt6530y1216_npr_aa[ibin-1][3]) << "\t" << TMath::Sqrt(syst_effTnP_pt6530y1216_npr_aa[ibin-1][4]) << "\t" << TMath::Sqrt(syst_effTnP_pt6530y1216_npr_aa[ibin-1][3]+syst_effTnP_pt6530y1216_npr_aa[ibin-1][4]) << "\t"
              << TMath::Sqrt(syst_effTnP_pt6530y1216_npr_aa[ibin-1][0]) << "\t" << TMath::Sqrt(syst_eff4d_pt6530y1216_npr_aa[ibin-1][0]) << "\t"
              << TMath::Sqrt(fitContribution_pt6530y1216_npr_aa/rms_fitContribNorm) << "\t"
              << TMath::Sqrt(TMath::Power(systLumis[1],2) + TMath::Power(systEventSelection[1],2) + TMath::Power(systTracking[1],2)) << endl;
          if(bDoDebug)
          {
            cout <<"---------------------------------------------------------------"<<endl;
            cout << "Ingredients to the total systm. uncertainty"<<endl;
            cout <<"Prompt yields systematics: aa & pp"<<endl;
            cout <<"fitContribution: "<<fitContribution_pt6530y1216_pr_aa<<"\t"<<fitContribution_pt6530y1216_pr_pp<<endl;
            cout <<"eff4dContribution: "<<eff4dContribution_pt6530y1216_pr_aa<<"\t"<<eff4dContribution_pt6530y1216_pr_pp<<endl;
            cout <<"efftnpContribution: "<<efftnpContribution_pt6530y1216_pr_aa<<"\t"<<efftnpContribution_pt6530y1216_pr_pp<<endl;
            cout <<"yields: "<<yield_aa_pr<<"\t"<<yield_pp_pr<<endl;
            cout <<"yield ratio: "<<yieldRatio_pr<<endl;
            cout <<"Total: "<<prJpsiErrSyst_pt6530y1216_cent[ibin-1]<<endl;
          }
          break;

        case 4:// high=pt, 1.6<|y|<2.4
          if (ibin > nBinsNpart6) break;
          yieldSyst_pr_aa  = (fitContribution_pt6530y1624_pr_aa/rms_fitContribNorm+eff4dContribution_pt6530y1624_pr_aa+efftnpContribution_pt6530y1624_pr_aa);
          yieldSyst_npr_aa = (fitContribution_pt6530y1624_npr_aa/rms_fitContribNorm+eff4dContribution_pt6530y1624_npr_aa+efftnpContribution_pt6530y1624_npr_aa);

          yieldSyst_pr_pp  = (fitContribution_pt6530y1624_pr_pp/rms_fitContribNorm+eff4dContribution_pt6530y1624_pr_pp+efftnpContribution_pt6530y1624_pr_pp);
          yieldSyst_npr_pp = (fitContribution_pt6530y1624_npr_pp/rms_fitContribNorm+eff4dContribution_pt6530y1624_npr_pp+efftnpContribution_pt6530y1624_npr_pp);

          systErrTotal_pr_aa_y1624_pt6530_copy[ibin-1] = phCorr_pr_aa->GetBinContent(ibin) * TMath::Sqrt( yieldSyst_pr_aa );
          systErrTotal_pr_pp_y1624_pt6530_copy[ibin-1] = phCorr_pr_pp->GetBinContent(ibin) * TMath::Sqrt( yieldSyst_pr_pp );
          systErrTotal_npr_aa_y1624_pt6530_copy[ibin-1] = phCorr_npr_aa->GetBinContent(ibin) * TMath::Sqrt( yieldSyst_npr_aa );
          systErrTotal_npr_pp_y1624_pt6530_copy[ibin-1] = phCorr_npr_pp->GetBinContent(ibin) * TMath::Sqrt( yieldSyst_npr_pp );

          dRelErrRaw_pr_pp  = phRaw_pr_pp->GetBinError(ibin)/phRaw_pr_pp->GetBinContent(ibin);
          dRelErrRaw_np_pp  = phRaw_npr_pp->GetBinError(ibin)/phRaw_npr_pp->GetBinContent(ibin);

          prJpsiErrSyst_pt6530y1624_cent[ibin-1]    = yieldRatio_pr  * TMath::Sqrt(yieldSyst_pr_aa+taa6_relerr);
          nonPrJpsiErrSyst_pt6530y1624_cent[ibin-1] = yieldRatio_npr * TMath::Sqrt(yieldSyst_npr_aa+taa6_relerr); 
         
          if(ibin == 1) { // global syst is same for all bins, don't need to repeat
            if (systBoxType == 2) {
              globalSyst_pr  = TMath::Sqrt(yieldSyst_pr_pp+systLumi+systSelection+systTrack + pow(dRelErrRaw_pr_pp,2));
              globalSyst_npr = TMath::Sqrt(yieldSyst_npr_pp+systLumi+systSelection+systTrack + pow(dRelErrRaw_np_pp,2));

              lumi_pr_y1624_pt6530  = new TBox(375,1-globalSyst_pr,400.0,1+globalSyst_pr);
              lumi_npr_y1624_pt6530 = new TBox(375,1-globalSyst_npr,400.0,1+globalSyst_npr);
              
              lumi_pr_y1624_pt6530_pty  = new TBox(350,1-globalSyst_pr,375,1+globalSyst_pr);
              lumi_npr_y1624_pt6530_pty = new TBox(350,1-globalSyst_npr,375.0,1+globalSyst_npr);
            } else if (systBoxType == 0 || systBoxType == 1) {
              globalSyst_pr = TMath::Sqrt(systLumi+systSelection+systTrack);
              globalSyst_npr = TMath::Sqrt(systLumi+systSelection+systTrack);
              globalSyst_pp_pr = TMath::Sqrt(yieldSyst_pr_pp + pow(dRelErrRaw_pr_pp,2));
              globalSyst_pp_npr = TMath::Sqrt(yieldSyst_npr_pp + pow(dRelErrRaw_np_pp,2));

              lumi_pr_y1624_pt6530  = new TBox(375,1-globalSyst_pr,400.0,1+globalSyst_pr);
              lumi_npr_y1624_pt6530 = new TBox(375,1-globalSyst_npr,400.0,1+globalSyst_npr);
              lumi_pr_y1624_pt6530_pty  = new TBox(350,1-globalSyst_pr,400,1+globalSyst_pr);
              lumi_npr_y1624_pt6530_pty = new TBox(350,1-globalSyst_npr,400,1+globalSyst_npr);

              globalpp_pr_y1624_pt6530  = new TBox(375,1-globalSyst_pp_pr,400.0,1+globalSyst_pp_pr);
              globalpp_npr_y1624_pt6530 = new TBox(375,1-globalSyst_pp_npr,400.0,1+globalSyst_pp_npr);
              globalpp_pr_y1624_pt6530_pty  = new TBox(350,1-globalSyst_pp_pr,375,1+globalSyst_pp_pr);
              globalpp_npr_y1624_pt6530_pty = new TBox(350,1-globalSyst_pp_npr,375.0,1+globalSyst_pp_npr);

              cout << "\t\tglobalSyst y1624pt6530 _pr and _npr with all 4 parts " << endl;
              cout << "\t\t" << TMath::Sqrt(yieldSyst_pr_pp+systLumi+systSelection+systTrack) << "\t"<< TMath::Sqrt(yieldSyst_npr_pp+systLumi+systSelection+systTrack) << endl;
              cout << "\t\t" << TMath::Sqrt(yieldSyst_pr_pp) << "\t"<< TMath::Sqrt(yieldSyst_npr_pp) << endl;
              cout << "\t\t" << TMath::Sqrt(systLumi+systSelection+systTrack) << "\t"<< TMath::Sqrt(systLumi+systSelection+systTrack) << endl;
              cout << "\t\t" << 1-globalSyst_pp_pr  << "\t"<< 1-globalSyst_pp_npr  << endl;
              cout << "\t\t" << 1+globalSyst_pp_pr  << "\t"<< 1+globalSyst_pp_npr  << endl;
              cout << "\t\t" << 1-globalSyst_pr  << "\t"<< 1-globalSyst_npr  << endl;
              cout << "\t\t" << 1+globalSyst_pr  << "\t"<< 1+globalSyst_npr  << endl;
            }
          }

          outputData_pr << "65300\t" << "1624\t" << centbins_6bins_str[ibin-1] << "\t" << yieldRatio_pr << "\t" << prJpsiErrSyst_pt6530y1624_cent[ibin-1] << "\t"
              << TMath::Sqrt(syst_effTnP_pt6530y1624_pr_pp[ibin-1][1]) << "\t" << TMath::Sqrt(syst_effTnP_pt6530y1624_pr_pp[ibin-1][2]) << "\t" << TMath::Sqrt(syst_effTnP_pt6530y1624_pr_pp[ibin-1][1]+syst_effTnP_pt6530y1624_pr_pp[ibin-1][2]) << "\t"
              << TMath::Sqrt(syst_effTnP_pt6530y1624_pr_pp[ibin-1][3]) << "\t" << TMath::Sqrt(syst_effTnP_pt6530y1624_pr_pp[ibin-1][4]) << "\t" << TMath::Sqrt(syst_effTnP_pt6530y1624_pr_pp[ibin-1][3]+syst_effTnP_pt6530y1624_pr_pp[ibin-1][4]) << "\t"
              << TMath::Sqrt(syst_effTnP_pt6530y1624_pr_pp[ibin-1][0]) << "\t" << TMath::Sqrt(syst_eff4d_pt6530y1624_pr_pp[ibin-1][0]) << "\t"
              << TMath::Sqrt(fitContribution_pt6530y1624_pr_pp/rms_fitContribNorm) << "\t"
              << TMath::Sqrt(TMath::Power(systLumis[0],2) + TMath::Power(systEventSelection[0],2) + TMath::Power(systTracking[0],2)) << "\t"
              << TMath::Sqrt(syst_effTnP_pt6530y1624_pr_aa[ibin-1][1]) << "\t" << TMath::Sqrt(syst_effTnP_pt6530y1624_pr_aa[ibin-1][2]) << "\t" << TMath::Sqrt(syst_effTnP_pt6530y1624_pr_aa[ibin-1][1]+syst_effTnP_pt6530y1624_pr_aa[ibin-1][2]) << "\t"
              << TMath::Sqrt(syst_effTnP_pt6530y1624_pr_aa[ibin-1][3]) << "\t" << TMath::Sqrt(syst_effTnP_pt6530y1624_pr_aa[ibin-1][4]) << "\t" << TMath::Sqrt(syst_effTnP_pt6530y1624_pr_aa[ibin-1][3]+syst_effTnP_pt6530y1624_pr_aa[ibin-1][4]) << "\t"
              << TMath::Sqrt(syst_effTnP_pt6530y1624_pr_aa[ibin-1][0]) << "\t" << TMath::Sqrt(syst_eff4d_pt6530y1624_pr_aa[ibin-1][0]) << "\t"
              << TMath::Sqrt(fitContribution_pt6530y1624_pr_aa/rms_fitContribNorm) << "\t"
              << TMath::Sqrt(TMath::Power(systLumis[1],2) + TMath::Power(systEventSelection[1],2) + TMath::Power(systTracking[1],2)) << endl;
          outputData_npr << "65300\t" << "1624\t" << centbins_6bins_str[ibin-1] << "\t" << yieldRatio_npr << "\t" << nonPrJpsiErrSyst_pt6530y1624_cent[ibin-1] << "\t"
              << TMath::Sqrt(syst_effTnP_pt6530y1624_npr_pp[ibin-1][1]) << "\t" << TMath::Sqrt(syst_effTnP_pt6530y1624_npr_pp[ibin-1][2]) << "\t" << TMath::Sqrt(syst_effTnP_pt6530y1624_npr_pp[ibin-1][1]+syst_effTnP_pt6530y1624_npr_pp[ibin-1][2]) << "\t"
              << TMath::Sqrt(syst_effTnP_pt6530y1624_npr_pp[ibin-1][3]) << "\t" << TMath::Sqrt(syst_effTnP_pt6530y1624_npr_pp[ibin-1][4]) << "\t" << TMath::Sqrt(syst_effTnP_pt6530y1624_npr_pp[ibin-1][3]+syst_effTnP_pt6530y1624_npr_pp[ibin-1][4]) << "\t"
              << TMath::Sqrt(syst_effTnP_pt6530y1624_npr_pp[ibin-1][0]) << "\t" << TMath::Sqrt(syst_eff4d_pt6530y1624_npr_pp[ibin-1][0]) << "\t"
              << TMath::Sqrt(fitContribution_pt6530y1624_npr_pp/rms_fitContribNorm) << "\t"
              << TMath::Sqrt(TMath::Power(systLumis[0],2) + TMath::Power(systEventSelection[0],2) + TMath::Power(systTracking[0],2)) << "\t"
              << TMath::Sqrt(syst_effTnP_pt6530y1624_npr_aa[ibin-1][1]) << "\t" << TMath::Sqrt(syst_effTnP_pt6530y1624_npr_aa[ibin-1][2]) << "\t" << TMath::Sqrt(syst_effTnP_pt6530y1624_npr_aa[ibin-1][1]+syst_effTnP_pt6530y1624_npr_aa[ibin-1][2]) << "\t"
              << TMath::Sqrt(syst_effTnP_pt6530y1624_npr_aa[ibin-1][3]) << "\t" << TMath::Sqrt(syst_effTnP_pt6530y1624_npr_aa[ibin-1][4]) << "\t" << TMath::Sqrt(syst_effTnP_pt6530y1624_npr_aa[ibin-1][3]+syst_effTnP_pt6530y1624_npr_aa[ibin-1][4]) << "\t"
              << TMath::Sqrt(syst_effTnP_pt6530y1624_npr_aa[ibin-1][0]) << "\t" << TMath::Sqrt(syst_eff4d_pt6530y1624_npr_aa[ibin-1][0]) << "\t"
              << TMath::Sqrt(fitContribution_pt6530y1624_npr_aa/rms_fitContribNorm) << "\t"
              << TMath::Sqrt(TMath::Power(systLumis[1],2) + TMath::Power(systEventSelection[1],2) + TMath::Power(systTracking[1],2)) << endl;
          if(bDoDebug)
          {
            cout <<"---------------------------------------------------------------"<<endl;
            cout << "Ingredients to the total systm. uncertainty"<<endl;
            cout <<"Prompt yields systematics: aa & pp"<<endl;
            cout <<"fitContribution: "<<fitContribution_pt6530y1624_pr_aa<<"\t"<<fitContribution_pt6530y1624_pr_pp<<endl;
            cout <<"eff4dContribution: "<<eff4dContribution_pt6530y1624_pr_aa<<"\t"<<eff4dContribution_pt6530y1624_pr_pp<<endl;
            cout <<"efftnpContribution: "<<efftnpContribution_pt6530y1624_pr_aa<<"\t"<<efftnpContribution_pt6530y1624_pr_pp<<endl;
            cout <<"yields: "<<yield_aa_pr<<"\t"<<yield_pp_pr<<endl;
            cout <<"yield ratio: "<<yieldRatio_pr<<endl;
            cout <<"Total: "<<prJpsiErrSyst_pt6530y1624_cent[ibin-1]<<endl;
          }
          break;
      }//switch end
      cout << endl;
    }//loop end: for(int ibin=1; ibin<=numBins; ibin++): 7 for highpt, 3 for low-pt, ...
    outputData_pr << endl;
    outputData_npr << endl;
  }//loop end: for(int ih=0; ih<nInHist;ih++) for each kinematic range (high-pt, low=pt, mb, etC)

  outputData_pr.close();
  outputData_npr.close();
  

  cout << endl;
  cout << phCorr_pr_aa_y024_pt6530_copy->GetName() << endl;
  cout << "Yield prompt PbPb : PbPberr : pp : pperr \n";
  for (int ibin=1; ibin<=phCorr_pr_aa_y024_pt6530_copy->GetNbinsX(); ibin++) {
    double dRelErrRaw_pr_pp  = phRaw_pr_pp_y024_pt6530_copy->GetBinError(ibin)/phRaw_pr_pp_y024_pt6530_copy->GetBinContent(ibin);
    double dRelErrRaw_pr_aa  = phRaw_pr_aa_y024_pt6530_copy->GetBinError(ibin)/phRaw_pr_aa_y024_pt6530_copy->GetBinContent(ibin);
    dRelErrRaw_pr_pp *= phCorr_pr_pp_y024_pt6530_copy->GetBinContent(ibin);
    dRelErrRaw_pr_aa *= phCorr_pr_aa_y024_pt6530_copy->GetBinContent(ibin);

    printf("%.2f$\\pm$%.2f$\\pm$%.2f  &  ",phCorr_pr_aa_y024_pt6530_copy->GetBinContent(ibin),dRelErrRaw_pr_aa,systErrTotal_pr_aa_y024_pt6530_copy[ibin-1]);
    printf("%.2f$\\pm$%.2f$\\pm$%.2f  \\\\",phCorr_pr_pp_y024_pt6530_copy->GetBinContent(ibin),dRelErrRaw_pr_pp,systErrTotal_pr_pp_y024_pt6530_copy[ibin-1]);
    cout << endl;
  }
  cout << phCorr_pr_aa_y012_pt6530_copy->GetName() << endl;
  cout << "Yield prompt PbPb : PbPberr : pp : pperr \n";
  for (int ibin=1; ibin<=phCorr_pr_aa_y012_pt6530_copy->GetNbinsX(); ibin++) {
    double dRelErrRaw_pr_pp  = phRaw_pr_pp_y012_pt6530_copy->GetBinError(ibin)/phRaw_pr_pp_y012_pt6530_copy->GetBinContent(ibin);
    double dRelErrRaw_pr_aa  = phRaw_pr_aa_y012_pt6530_copy->GetBinError(ibin)/phRaw_pr_aa_y012_pt6530_copy->GetBinContent(ibin);
    dRelErrRaw_pr_pp *= phCorr_pr_pp_y012_pt6530_copy->GetBinContent(ibin);
    dRelErrRaw_pr_aa *= phCorr_pr_aa_y012_pt6530_copy->GetBinContent(ibin);

    printf("%.2f$\\pm$%.2f$\\pm$%.2f  &  ",phCorr_pr_aa_y012_pt6530_copy->GetBinContent(ibin),dRelErrRaw_pr_aa,systErrTotal_pr_aa_y012_pt6530_copy[ibin-1]);
    printf("%.2f$\\pm$%.2f$\\pm$%.2f  \\\\",phCorr_pr_pp_y012_pt6530_copy->GetBinContent(ibin),dRelErrRaw_pr_pp,systErrTotal_pr_pp_y012_pt6530_copy[ibin-1]);
    cout << endl;
  }
  cout << phCorr_pr_aa_y1216_pt6530_copy->GetName() << endl;
  cout << "Yield prompt PbPb : PbPberr : pp : pperr \n";
  for (int ibin=1; ibin<=phCorr_pr_aa_y1216_pt6530_copy->GetNbinsX(); ibin++) {
    double dRelErrRaw_pr_pp  = phRaw_pr_pp_y1216_pt6530_copy->GetBinError(ibin)/phRaw_pr_pp_y1216_pt6530_copy->GetBinContent(ibin);
    double dRelErrRaw_pr_aa  = phRaw_pr_aa_y1216_pt6530_copy->GetBinError(ibin)/phRaw_pr_aa_y1216_pt6530_copy->GetBinContent(ibin);
    dRelErrRaw_pr_pp *= phCorr_pr_pp_y1216_pt6530_copy->GetBinContent(ibin);
    dRelErrRaw_pr_aa *= phCorr_pr_aa_y1216_pt6530_copy->GetBinContent(ibin);

    printf("%.2f$\\pm$%.2f$\\pm$%.2f  &  ",phCorr_pr_aa_y1216_pt6530_copy->GetBinContent(ibin),dRelErrRaw_pr_aa,systErrTotal_pr_aa_y1216_pt6530_copy[ibin-1]);
    printf("%.2f$\\pm$%.2f$\\pm$%.2f  \\\\",phCorr_pr_pp_y1216_pt6530_copy->GetBinContent(ibin),dRelErrRaw_pr_pp,systErrTotal_pr_pp_y1216_pt6530_copy[ibin-1]);
    cout << endl;
  }
  cout << phCorr_pr_aa_y1624_pt6530_copy->GetName() << endl;
  cout << "Yield prompt PbPb : PbPberr : pp : pperr \n";
  for (int ibin=1; ibin<=phCorr_pr_aa_y1624_pt6530_copy->GetNbinsX(); ibin++) {
    double dRelErrRaw_pr_pp  = phRaw_pr_pp_y1624_pt6530_copy->GetBinError(ibin)/phRaw_pr_pp_y1624_pt6530_copy->GetBinContent(ibin);
    double dRelErrRaw_pr_aa  = phRaw_pr_aa_y1624_pt6530_copy->GetBinError(ibin)/phRaw_pr_aa_y1624_pt6530_copy->GetBinContent(ibin);
    dRelErrRaw_pr_pp *= phCorr_pr_pp_y1624_pt6530_copy->GetBinContent(ibin);
    dRelErrRaw_pr_aa *= phCorr_pr_aa_y1624_pt6530_copy->GetBinContent(ibin);

    printf("%.2f$\\pm$%.2f$\\pm$%.2f  &  ",phCorr_pr_aa_y1624_pt6530_copy->GetBinContent(ibin),dRelErrRaw_pr_aa,systErrTotal_pr_aa_y1624_pt6530_copy[ibin-1]);
    printf("%.2f$\\pm$%.2f$\\pm$%.2f  \\\\",phCorr_pr_pp_y1624_pt6530_copy->GetBinContent(ibin),dRelErrRaw_pr_pp,systErrTotal_pr_pp_y1624_pt6530_copy[ibin-1]);
    cout << endl;
  }
  cout << phCorr_pr_aa_y1624_pt365_copy->GetName() << endl;
  cout << "Yield prompt PbPb : PbPberr : pp : pperr \n";
  for (int ibin=1; ibin<=phCorr_pr_aa_y1624_pt365_copy->GetNbinsX(); ibin++) {
    double dRelErrRaw_pr_pp  = phRaw_pr_pp_y1624_pt365_copy->GetBinError(ibin)/phRaw_pr_pp_y1624_pt365_copy->GetBinContent(ibin);
    double dRelErrRaw_pr_aa  = phRaw_pr_aa_y1624_pt365_copy->GetBinError(ibin)/phRaw_pr_aa_y1624_pt365_copy->GetBinContent(ibin);
    dRelErrRaw_pr_pp *= phCorr_pr_pp_y1624_pt365_copy->GetBinContent(ibin);
    dRelErrRaw_pr_aa *= phCorr_pr_aa_y1624_pt365_copy->GetBinContent(ibin);

    printf("%.2f$\\pm$%.2f$\\pm$%.2f  &  ",phCorr_pr_aa_y1624_pt365_copy->GetBinContent(ibin),dRelErrRaw_pr_aa,systErrTotal_pr_aa_y1624_pt365_copy[ibin-1]);
    printf("%.2f$\\pm$%.2f$\\pm$%.2f  \\\\",phCorr_pr_pp_y1624_pt365_copy->GetBinContent(ibin),dRelErrRaw_pr_pp,systErrTotal_pr_pp_y1624_pt365_copy[ibin-1]);
    cout << endl;
  }


  cout << endl;
  cout << phCorr_npr_aa_y024_pt6530_copy->GetName() << endl;
  cout << "Yield non-prompt PbPb : PbPberr : pp : pperr \n";
  for (int ibin=1; ibin<=phCorr_npr_aa_y024_pt6530_copy->GetNbinsX(); ibin++) {
    double dRelErrRaw_np_pp  = phRaw_npr_pp_y024_pt6530_copy->GetBinError(ibin)/phRaw_npr_pp_y024_pt6530_copy->GetBinContent(ibin);
    double dRelErrRaw_np_aa  = phRaw_npr_aa_y024_pt6530_copy->GetBinError(ibin)/phRaw_npr_aa_y024_pt6530_copy->GetBinContent(ibin);
    dRelErrRaw_np_pp *= phCorr_npr_pp_y024_pt6530_copy->GetBinContent(ibin);
    dRelErrRaw_np_aa *= phCorr_npr_aa_y024_pt6530_copy->GetBinContent(ibin);

    printf("%.2f$\\pm$%.2f$\\pm$%.2f  &  ",phCorr_npr_aa_y024_pt6530_copy->GetBinContent(ibin),dRelErrRaw_np_aa,systErrTotal_npr_aa_y024_pt6530_copy[ibin-1]);
    printf("%.2f$\\pm$%.2f$\\pm$%.2f  \\\\",phCorr_npr_pp_y024_pt6530_copy->GetBinContent(ibin),dRelErrRaw_np_pp,systErrTotal_npr_pp_y024_pt6530_copy[ibin-1]);
    cout << endl;
  }
  cout << phCorr_npr_aa_y012_pt6530_copy->GetName() << endl;
  cout << "Yield non-prompt PbPb : PbPberr : pp : pperr \n";
  for (int ibin=1; ibin<=phCorr_npr_aa_y012_pt6530_copy->GetNbinsX(); ibin++) {
    double dRelErrRaw_np_pp  = phRaw_npr_pp_y012_pt6530_copy->GetBinError(ibin)/phRaw_npr_pp_y012_pt6530_copy->GetBinContent(ibin);
    double dRelErrRaw_np_aa  = phRaw_npr_aa_y012_pt6530_copy->GetBinError(ibin)/phRaw_npr_aa_y012_pt6530_copy->GetBinContent(ibin);
    dRelErrRaw_np_pp *= phCorr_npr_pp_y012_pt6530_copy->GetBinContent(ibin);
    dRelErrRaw_np_aa *= phCorr_npr_aa_y012_pt6530_copy->GetBinContent(ibin);

    printf("%.2f$\\pm$%.2f$\\pm$%.2f  &  ",phCorr_npr_aa_y012_pt6530_copy->GetBinContent(ibin),dRelErrRaw_np_aa,systErrTotal_npr_aa_y012_pt6530_copy[ibin-1]);
    printf("%.2f$\\pm$%.2f$\\pm$%.2f  \\\\",phCorr_npr_pp_y012_pt6530_copy->GetBinContent(ibin),dRelErrRaw_np_pp,systErrTotal_npr_pp_y012_pt6530_copy[ibin-1]);
    cout << endl;
  }
  cout << phCorr_npr_aa_y1216_pt6530_copy->GetName() << endl;
  cout << "Yield non-prompt PbPb : PbPberr : pp : pperr \n";
  for (int ibin=1; ibin<=phCorr_npr_aa_y1216_pt6530_copy->GetNbinsX(); ibin++) {
    double dRelErrRaw_np_pp  = phRaw_npr_pp_y1216_pt6530_copy->GetBinError(ibin)/phRaw_npr_pp_y1216_pt6530_copy->GetBinContent(ibin);
    double dRelErrRaw_np_aa  = phRaw_npr_aa_y1216_pt6530_copy->GetBinError(ibin)/phRaw_npr_aa_y1216_pt6530_copy->GetBinContent(ibin);
    dRelErrRaw_np_pp *= phCorr_npr_pp_y1216_pt6530_copy->GetBinContent(ibin);
    dRelErrRaw_np_aa *= phCorr_npr_aa_y1216_pt6530_copy->GetBinContent(ibin);

    printf("%.2f$\\pm$%.2f$\\pm$%.2f  &  ",phCorr_npr_aa_y1216_pt6530_copy->GetBinContent(ibin),dRelErrRaw_np_aa,systErrTotal_npr_aa_y1216_pt6530_copy[ibin-1]);
    printf("%.2f$\\pm$%.2f$\\pm$%.2f  \\\\",phCorr_npr_pp_y1216_pt6530_copy->GetBinContent(ibin),dRelErrRaw_np_pp,systErrTotal_npr_pp_y1216_pt6530_copy[ibin-1]);
    cout << endl;
  }
  cout << phCorr_npr_aa_y1624_pt6530_copy->GetName() << endl;
  cout << "Yield non-prompt PbPb : PbPberr : pp : pperr \n";
  for (int ibin=1; ibin<=phCorr_npr_aa_y1624_pt6530_copy->GetNbinsX(); ibin++) {
    double dRelErrRaw_np_pp  = phRaw_npr_pp_y1624_pt6530_copy->GetBinError(ibin)/phRaw_npr_pp_y1624_pt6530_copy->GetBinContent(ibin);
    double dRelErrRaw_np_aa  = phRaw_npr_aa_y1624_pt6530_copy->GetBinError(ibin)/phRaw_npr_aa_y1624_pt6530_copy->GetBinContent(ibin);
    dRelErrRaw_np_pp *= phCorr_npr_pp_y1624_pt6530_copy->GetBinContent(ibin);
    dRelErrRaw_np_aa *= phCorr_npr_aa_y1624_pt6530_copy->GetBinContent(ibin);

    printf("%.2f$\\pm$%.2f$\\pm$%.2f  &  ",phCorr_npr_aa_y1624_pt6530_copy->GetBinContent(ibin),dRelErrRaw_np_aa,systErrTotal_npr_aa_y1624_pt6530_copy[ibin-1]);
    printf("%.2f$\\pm$%.2f$\\pm$%.2f  \\\\",phCorr_npr_pp_y1624_pt6530_copy->GetBinContent(ibin),dRelErrRaw_np_pp,systErrTotal_npr_pp_y1624_pt6530_copy[ibin-1]);
    cout << endl;
  }
  cout << phCorr_npr_aa_y1624_pt365_copy->GetName() << endl;
  cout << "Yield non-prompt PbPb : PbPberr : pp : pperr \n";
  for (int ibin=1; ibin<=phCorr_npr_aa_y1624_pt365_copy->GetNbinsX(); ibin++) {
    double dRelErrRaw_np_pp  = phRaw_npr_pp_y1624_pt365_copy->GetBinError(ibin)/phRaw_npr_pp_y1624_pt365_copy->GetBinContent(ibin);
    double dRelErrRaw_np_aa  = phRaw_npr_aa_y1624_pt365_copy->GetBinError(ibin)/phRaw_npr_aa_y1624_pt365_copy->GetBinContent(ibin);
    dRelErrRaw_np_pp *= phCorr_npr_pp_y1624_pt365_copy->GetBinContent(ibin);
    dRelErrRaw_np_aa *= phCorr_npr_aa_y1624_pt365_copy->GetBinContent(ibin);

    printf("%.2f$\\pm$%.2f$\\pm$%.2f  &  ",phCorr_npr_aa_y1624_pt365_copy->GetBinContent(ibin),dRelErrRaw_np_aa,systErrTotal_npr_aa_y1624_pt365_copy[ibin-1]);
    printf("%.2f$\\pm$%.2f$\\pm$%.2f  \\\\",phCorr_npr_pp_y1624_pt365_copy->GetBinContent(ibin),dRelErrRaw_np_pp,systErrTotal_npr_pp_y1624_pt365_copy[ibin-1]);
    cout << endl;
  }


  cout << endl;
  cout << phCorr_pr_aa_y024_pt6530_copy->GetName() << endl;
  cout << "Yield prompt PbPb : PbPberr : pp : pperr \n";
  for (int ibin=1; ibin<=phCorr_pr_aa_y024_pt6530_copy->GetNbinsX(); ibin++) {
    double dRelErrRaw_pr_pp  = phRaw_pr_pp_y024_pt6530_copy->GetBinError(ibin)/phRaw_pr_pp_y024_pt6530_copy->GetBinContent(ibin);
    double dRelErrRaw_pr_aa  = phRaw_pr_aa_y024_pt6530_copy->GetBinError(ibin)/phRaw_pr_aa_y024_pt6530_copy->GetBinContent(ibin);
    dRelErrRaw_pr_pp *= phCorr_pr_pp_y024_pt6530_copy->GetBinContent(ibin);
    dRelErrRaw_pr_aa *= phCorr_pr_aa_y024_pt6530_copy->GetBinContent(ibin);

    printf("%.2f\t%.2f\t%.2f\t",phCorr_pr_aa_y024_pt6530_copy->GetBinContent(ibin),dRelErrRaw_pr_aa,systErrTotal_pr_aa_y024_pt6530_copy[ibin-1]);
    printf("%.2f\t%.2f\t%.2f\t",phCorr_pr_pp_y024_pt6530_copy->GetBinContent(ibin),dRelErrRaw_pr_pp,systErrTotal_pr_pp_y024_pt6530_copy[ibin-1]);
    cout << endl;
  }
  cout << phCorr_pr_aa_y012_pt6530_copy->GetName() << endl;
  cout << "Yield prompt PbPb : PbPberr : pp : pperr \n";
  for (int ibin=1; ibin<=phCorr_pr_aa_y012_pt6530_copy->GetNbinsX(); ibin++) {
    double dRelErrRaw_pr_pp  = phRaw_pr_pp_y012_pt6530_copy->GetBinError(ibin)/phRaw_pr_pp_y012_pt6530_copy->GetBinContent(ibin);
    double dRelErrRaw_pr_aa  = phRaw_pr_aa_y012_pt6530_copy->GetBinError(ibin)/phRaw_pr_aa_y012_pt6530_copy->GetBinContent(ibin);
    dRelErrRaw_pr_pp *= phCorr_pr_pp_y012_pt6530_copy->GetBinContent(ibin);
    dRelErrRaw_pr_aa *= phCorr_pr_aa_y012_pt6530_copy->GetBinContent(ibin);

    printf("%.2f\t%.2f\t%.2f\t",phCorr_pr_aa_y012_pt6530_copy->GetBinContent(ibin),dRelErrRaw_pr_aa,systErrTotal_pr_aa_y012_pt6530_copy[ibin-1]);
    printf("%.2f\t%.2f\t%.2f\t",phCorr_pr_pp_y012_pt6530_copy->GetBinContent(ibin),dRelErrRaw_pr_pp,systErrTotal_pr_pp_y012_pt6530_copy[ibin-1]);
    cout << endl;
  }
  cout << phCorr_pr_aa_y1216_pt6530_copy->GetName() << endl;
  cout << "Yield prompt PbPb : PbPberr : pp : pperr \n";
  for (int ibin=1; ibin<=phCorr_pr_aa_y1216_pt6530_copy->GetNbinsX(); ibin++) {
    double dRelErrRaw_pr_pp  = phRaw_pr_pp_y1216_pt6530_copy->GetBinError(ibin)/phRaw_pr_pp_y1216_pt6530_copy->GetBinContent(ibin);
    double dRelErrRaw_pr_aa  = phRaw_pr_aa_y1216_pt6530_copy->GetBinError(ibin)/phRaw_pr_aa_y1216_pt6530_copy->GetBinContent(ibin);
    dRelErrRaw_pr_pp *= phCorr_pr_pp_y1216_pt6530_copy->GetBinContent(ibin);
    dRelErrRaw_pr_aa *= phCorr_pr_aa_y1216_pt6530_copy->GetBinContent(ibin);

    printf("%.2f\t%.2f\t%.2f\t",phCorr_pr_aa_y1216_pt6530_copy->GetBinContent(ibin),dRelErrRaw_pr_aa,systErrTotal_pr_aa_y1216_pt6530_copy[ibin-1]);
    printf("%.2f\t%.2f\t%.2f\t",phCorr_pr_pp_y1216_pt6530_copy->GetBinContent(ibin),dRelErrRaw_pr_pp,systErrTotal_pr_pp_y1216_pt6530_copy[ibin-1]);
    cout << endl;
  }
  cout << phCorr_pr_aa_y1624_pt6530_copy->GetName() << endl;
  cout << "Yield prompt PbPb : PbPberr : pp : pperr \n";
  for (int ibin=1; ibin<=phCorr_pr_aa_y1624_pt6530_copy->GetNbinsX(); ibin++) {
    double dRelErrRaw_pr_pp  = phRaw_pr_pp_y1624_pt6530_copy->GetBinError(ibin)/phRaw_pr_pp_y1624_pt6530_copy->GetBinContent(ibin);
    double dRelErrRaw_pr_aa  = phRaw_pr_aa_y1624_pt6530_copy->GetBinError(ibin)/phRaw_pr_aa_y1624_pt6530_copy->GetBinContent(ibin);
    dRelErrRaw_pr_pp *= phCorr_pr_pp_y1624_pt6530_copy->GetBinContent(ibin);
    dRelErrRaw_pr_aa *= phCorr_pr_aa_y1624_pt6530_copy->GetBinContent(ibin);

    printf("%.2f\t%.2f\t%.2f\t",phCorr_pr_aa_y1624_pt6530_copy->GetBinContent(ibin),dRelErrRaw_pr_aa,systErrTotal_pr_aa_y1624_pt6530_copy[ibin-1]);
    printf("%.2f\t%.2f\t%.2f\t",phCorr_pr_pp_y1624_pt6530_copy->GetBinContent(ibin),dRelErrRaw_pr_pp,systErrTotal_pr_pp_y1624_pt6530_copy[ibin-1]);
    cout << endl;
  }
  cout << phCorr_pr_aa_y1624_pt365_copy->GetName() << endl;
  cout << "Yield prompt PbPb : PbPberr : pp : pperr \n";
  for (int ibin=1; ibin<=phCorr_pr_aa_y1624_pt365_copy->GetNbinsX(); ibin++) {
    double dRelErrRaw_pr_pp  = phRaw_pr_pp_y1624_pt365_copy->GetBinError(ibin)/phRaw_pr_pp_y1624_pt365_copy->GetBinContent(ibin);
    double dRelErrRaw_pr_aa  = phRaw_pr_aa_y1624_pt365_copy->GetBinError(ibin)/phRaw_pr_aa_y1624_pt365_copy->GetBinContent(ibin);
    dRelErrRaw_pr_pp *= phCorr_pr_pp_y1624_pt365_copy->GetBinContent(ibin);
    dRelErrRaw_pr_aa *= phCorr_pr_aa_y1624_pt365_copy->GetBinContent(ibin);

    printf("%.2f\t%.2f\t%.2f\t",phCorr_pr_aa_y1624_pt365_copy->GetBinContent(ibin),dRelErrRaw_pr_aa,systErrTotal_pr_aa_y1624_pt365_copy[ibin-1]);
    printf("%.2f\t%.2f\t%.2f\t",phCorr_pr_pp_y1624_pt365_copy->GetBinContent(ibin),dRelErrRaw_pr_pp,systErrTotal_pr_pp_y1624_pt365_copy[ibin-1]);
    cout << endl;
  }


  cout << endl;
  cout << phCorr_npr_aa_y024_pt6530_copy->GetName() << endl;
  cout << "Yield non-prompt PbPb : PbPberr : pp : pperr \n";
  for (int ibin=1; ibin<=phCorr_npr_aa_y024_pt6530_copy->GetNbinsX(); ibin++) {
    double dRelErrRaw_np_pp  = phRaw_npr_pp_y024_pt6530_copy->GetBinError(ibin)/phRaw_npr_pp_y024_pt6530_copy->GetBinContent(ibin);
    double dRelErrRaw_np_aa  = phRaw_npr_aa_y024_pt6530_copy->GetBinError(ibin)/phRaw_npr_aa_y024_pt6530_copy->GetBinContent(ibin);
    dRelErrRaw_np_pp *= phCorr_npr_pp_y024_pt6530_copy->GetBinContent(ibin);
    dRelErrRaw_np_aa *= phCorr_npr_aa_y024_pt6530_copy->GetBinContent(ibin);

    printf("%.2f\t%.2f\t%.2f\t",phCorr_npr_aa_y024_pt6530_copy->GetBinContent(ibin),dRelErrRaw_np_aa,systErrTotal_npr_aa_y024_pt6530_copy[ibin-1]);
    printf("%.2f\t%.2f\t%.2f\t",phCorr_npr_pp_y024_pt6530_copy->GetBinContent(ibin),dRelErrRaw_np_pp,systErrTotal_npr_pp_y024_pt6530_copy[ibin-1]);
    cout << endl;
  }
  cout << phCorr_npr_aa_y012_pt6530_copy->GetName() << endl;
  cout << "Yield non-prompt PbPb : PbPberr : pp : pperr \n";
  for (int ibin=1; ibin<=phCorr_npr_aa_y012_pt6530_copy->GetNbinsX(); ibin++) {
    double dRelErrRaw_np_pp  = phRaw_npr_pp_y012_pt6530_copy->GetBinError(ibin)/phRaw_npr_pp_y012_pt6530_copy->GetBinContent(ibin);
    double dRelErrRaw_np_aa  = phRaw_npr_aa_y012_pt6530_copy->GetBinError(ibin)/phRaw_npr_aa_y012_pt6530_copy->GetBinContent(ibin);
    dRelErrRaw_np_pp *= phCorr_npr_pp_y012_pt6530_copy->GetBinContent(ibin);
    dRelErrRaw_np_aa *= phCorr_npr_aa_y012_pt6530_copy->GetBinContent(ibin);

    printf("%.2f\t%.2f\t%.2f\t",phCorr_npr_aa_y012_pt6530_copy->GetBinContent(ibin),dRelErrRaw_np_aa,systErrTotal_npr_aa_y012_pt6530_copy[ibin-1]);
    printf("%.2f\t%.2f\t%.2f\t",phCorr_npr_pp_y012_pt6530_copy->GetBinContent(ibin),dRelErrRaw_np_pp,systErrTotal_npr_pp_y012_pt6530_copy[ibin-1]);
    cout << endl;
  }
  cout << phCorr_npr_aa_y1216_pt6530_copy->GetName() << endl;
  cout << "Yield non-prompt PbPb : PbPberr : pp : pperr \n";
  for (int ibin=1; ibin<=phCorr_npr_aa_y1216_pt6530_copy->GetNbinsX(); ibin++) {
    double dRelErrRaw_np_pp  = phRaw_npr_pp_y1216_pt6530_copy->GetBinError(ibin)/phRaw_npr_pp_y1216_pt6530_copy->GetBinContent(ibin);
    double dRelErrRaw_np_aa  = phRaw_npr_aa_y1216_pt6530_copy->GetBinError(ibin)/phRaw_npr_aa_y1216_pt6530_copy->GetBinContent(ibin);
    dRelErrRaw_np_pp *= phCorr_npr_pp_y1216_pt6530_copy->GetBinContent(ibin);
    dRelErrRaw_np_aa *= phCorr_npr_aa_y1216_pt6530_copy->GetBinContent(ibin);

    printf("%.2f\t%.2f\t%.2f\t",phCorr_npr_aa_y1216_pt6530_copy->GetBinContent(ibin),dRelErrRaw_np_aa,systErrTotal_npr_aa_y1216_pt6530_copy[ibin-1]);
    printf("%.2f\t%.2f\t%.2f\t",phCorr_npr_pp_y1216_pt6530_copy->GetBinContent(ibin),dRelErrRaw_np_pp,systErrTotal_npr_pp_y1216_pt6530_copy[ibin-1]);
    cout << endl;
  }
  cout << phCorr_npr_aa_y1624_pt6530_copy->GetName() << endl;
  cout << "Yield non-prompt PbPb : PbPberr : pp : pperr \n";
  for (int ibin=1; ibin<=phCorr_npr_aa_y1624_pt6530_copy->GetNbinsX(); ibin++) {
    double dRelErrRaw_np_pp  = phRaw_npr_pp_y1624_pt6530_copy->GetBinError(ibin)/phRaw_npr_pp_y1624_pt6530_copy->GetBinContent(ibin);
    double dRelErrRaw_np_aa  = phRaw_npr_aa_y1624_pt6530_copy->GetBinError(ibin)/phRaw_npr_aa_y1624_pt6530_copy->GetBinContent(ibin);
    dRelErrRaw_np_pp *= phCorr_npr_pp_y1624_pt6530_copy->GetBinContent(ibin);
    dRelErrRaw_np_aa *= phCorr_npr_aa_y1624_pt6530_copy->GetBinContent(ibin);

    printf("%.2f\t%.2f\t%.2f\t",phCorr_npr_aa_y1624_pt6530_copy->GetBinContent(ibin),dRelErrRaw_np_aa,systErrTotal_npr_aa_y1624_pt6530_copy[ibin-1]);
    printf("%.2f\t%.2f\t%.2f\t",phCorr_npr_pp_y1624_pt6530_copy->GetBinContent(ibin),dRelErrRaw_np_pp,systErrTotal_npr_pp_y1624_pt6530_copy[ibin-1]);
    cout << endl;
  }
  cout << phCorr_npr_aa_y1624_pt365_copy->GetName() << endl;
  cout << "Yield non-prompt PbPb : PbPberr : pp : pperr \n";
  for (int ibin=1; ibin<=phCorr_npr_aa_y1624_pt365_copy->GetNbinsX(); ibin++) {
    double dRelErrRaw_np_pp  = phRaw_npr_pp_y1624_pt365_copy->GetBinError(ibin)/phRaw_npr_pp_y1624_pt365_copy->GetBinContent(ibin);
    double dRelErrRaw_np_aa  = phRaw_npr_aa_y1624_pt365_copy->GetBinError(ibin)/phRaw_npr_aa_y1624_pt365_copy->GetBinContent(ibin);
    dRelErrRaw_np_pp *= phCorr_npr_pp_y1624_pt365_copy->GetBinContent(ibin);
    dRelErrRaw_np_aa *= phCorr_npr_aa_y1624_pt365_copy->GetBinContent(ibin);

    printf("%.2f\t%.2f\t%.2f\t",phCorr_npr_aa_y1624_pt365_copy->GetBinContent(ibin),dRelErrRaw_np_aa,systErrTotal_npr_aa_y1624_pt365_copy[ibin-1]);
    printf("%.2f\t%.2f\t%.2f\t",phCorr_npr_pp_y1624_pt365_copy->GetBinContent(ibin),dRelErrRaw_np_pp,systErrTotal_npr_pp_y1624_pt365_copy[ibin-1]);
    cout << endl;
  }





  // ***** //Drawing
  // pr
  cout << " nBinsNpart12 : " << nBinsNpart12 << " prJpsiErrSyst_cent[0] "<< prJpsiErrSyst_cent[0]<< endl;
  for (unsigned int ibin=0; ibin<nBinsNpart12; ibin++) {
    cout << "prJpsiErrSyst_cent[" << ibin << "] " << prJpsiErrSyst_cent[ibin] << endl ;
  }
  cout << " nBinsNpart6 : " << nBinsNpart6 << " nonPrJpsiErrSyst_pt365y1624_cent[0] "<< nonPrJpsiErrSyst_pt365y1624_cent[0]<< endl;
  for (unsigned int ibin=0; ibin<nBinsNpart6; ibin++) {
    cout << "nonPrJpsiErrSyst_pt365y1624_cent[" << ibin << "] " << nonPrJpsiErrSyst_pt365y1624_cent[ibin] << endl ;
  }
  TGraphErrors *gPrJpsiSyst             = new TGraphErrors(nBinsNpart12, binsNpart12, prJpsi_cent, binsNpart12X, prJpsiErrSyst_cent);
  TGraphErrors *gPrJpsiSyst_pt6530y012  = new TGraphErrors(nBinsNpart6, binsNpart6_shiftMinus, prJpsi_pt6530y012_cent, binsNpart6X, prJpsiErrSyst_pt6530y012_cent);
  TGraphErrors *gPrJpsiSyst_pt6530y1216 = new TGraphErrors(nBinsNpart6, binsNpart6, prJpsi_pt6530y1216_cent, binsNpart6X, prJpsiErrSyst_pt6530y1216_cent);
  TGraphErrors *gPrJpsiSyst_pt6530y1624 = new TGraphErrors(nBinsNpart6, binsNpart6_shiftPlus, prJpsi_pt6530y1624_cent, binsNpart6X, prJpsiErrSyst_pt6530y1624_cent);
  TGraphErrors *gPrJpsiSyst_pt365y1624  = new TGraphErrors(nBinsNpart6, binsNpart6,    prJpsi_pt365y1624_cent, binsNpart6X, prJpsiErrSyst_pt365y1624_cent);
 
  // nonPr
  TGraphErrors *gNonPrJpsiSyst             = new TGraphErrors(nBinsNpart6, binsNpart6, nonPrJpsi_cent, binsNpart6X, nonPrJpsiErrSyst_cent);
  TGraphErrors *gNonPrJpsiSyst_pt6530y012  = new TGraphErrors(nBinsNpart6, binsNpart6_shiftMinus, nonPrJpsi_pt6530y012_cent, binsNpart6X,   nonPrJpsiErrSyst_pt6530y012_cent);
  TGraphErrors *gNonPrJpsiSyst_pt6530y1216 = new TGraphErrors(nBinsNpart6, binsNpart6, nonPrJpsi_pt6530y1216_cent, binsNpart6X,    nonPrJpsiErrSyst_pt6530y1216_cent);
  TGraphErrors *gNonPrJpsiSyst_pt6530y1624 = new TGraphErrors(nBinsNpart6, binsNpart6_shiftPlus, nonPrJpsi_pt6530y1624_cent, binsNpart6X,    nonPrJpsiErrSyst_pt6530y1624_cent);
  TGraphErrors *gNonPrJpsiSyst_pt365y1624  = new TGraphErrors(nBinsNpart6, binsNpart6, nonPrJpsi_pt365y1624_cent, binsNpart6X, nonPrJpsiErrSyst_pt365y1624_cent);

  //-------------------------------------------------------------------
  // **************** marker colors
  //stat boxes
  gPrJpsiSyst->SetFillColor(kRed-9);
  gPrJpsiSyst_pt6530y012->SetFillColor(kAzure-9);
  gPrJpsiSyst_pt6530y1216->SetFillColor(kRed-9);
  gPrJpsiSyst_pt6530y1624->SetFillColor(kGreen-9); 
  gPrJpsiSyst_pt365y1624->SetFillColor(kViolet-9);

  // non-pr
  gNonPrJpsiSyst->SetFillColor(kOrange-9);
  gNonPrJpsiSyst_pt6530y012->SetFillColor(kAzure-9);
  gNonPrJpsiSyst_pt6530y1216->SetFillColor(kRed-9);
  gNonPrJpsiSyst_pt6530y1624->SetFillColor(kGreen-9);
  gNonPrJpsiSyst_pt365y1624->SetFillColor(kViolet-9);

  //----------------------------------------------------------------------
  // global uncertainty boxes

  if (systBoxType == 0 || systBoxType==2) { 
    lumi_pr_y024_pt6530->SetFillColor(kGray+1);
    if (systBoxType == 2) {
      lumi_pr_y012_pt6530->SetFillColor(kAzure-9);
      lumi_pr_y1216_pt6530->SetFillColor(kRed-9);
      lumi_pr_y1624_pt6530->SetFillColor(kGreen-9);
      lumi_pr_y1624_pt6530_pty->SetFillColor(kGreen-9);
      lumi_pr_y1624_pt365->SetFillColor(kViolet-9);
    }

    lumi_npr_y024_pt6530->SetFillColor(kGray+1);
    if (systBoxType == 2) {
      lumi_npr_y012_pt6530->SetFillColor(kAzure-9);
      lumi_npr_y1216_pt6530->SetFillColor(kRed-9);
      lumi_npr_y1624_pt6530->SetFillColor(kGreen-9);
      lumi_npr_y1624_pt6530_pty->SetFillColor(kGreen-9);
      lumi_npr_y1624_pt365->SetFillColor(kViolet-9);
    }
  }
  if (systBoxType == 0 || systBoxType == 1) {
    if (systBoxType == 1) lumi_pr_y024_pt6530->SetLineColor(kBlack);
    lumi_pr_y012_pt6530->SetLineColor(kBlack);
    lumi_pr_y1216_pt6530->SetLineColor(kBlack);
    lumi_pr_y1624_pt6530->SetLineColor(kBlack);
    lumi_pr_y1624_pt6530_pty->SetLineColor(kBlack);
    lumi_pr_y1624_pt365->SetLineColor(kBlack);

    if (systBoxType == 1) lumi_npr_y024_pt6530->SetLineColor(kBlack);
    lumi_npr_y012_pt6530->SetLineColor(kBlack);
    lumi_npr_y1216_pt6530->SetLineColor(kBlack);
    lumi_npr_y1624_pt6530->SetLineColor(kBlack);
    lumi_npr_y1624_pt6530_pty->SetLineColor(kBlack);
    lumi_npr_y1624_pt365->SetLineColor(kBlack);

    if (systBoxType == 1) lumi_pr_y024_pt6530->SetLineWidth(2);
    lumi_pr_y012_pt6530->SetLineWidth(2);
    lumi_pr_y1216_pt6530->SetLineWidth(2);
    lumi_pr_y1624_pt6530->SetLineWidth(2);
    lumi_pr_y1624_pt6530_pty->SetLineWidth(2);
    lumi_pr_y1624_pt365->SetLineWidth(2);

    if (systBoxType == 1) lumi_npr_y024_pt6530->SetLineWidth(2);
    lumi_npr_y012_pt6530->SetLineWidth(2);
    lumi_npr_y1216_pt6530->SetLineWidth(2);
    lumi_npr_y1624_pt6530->SetLineWidth(2);
    lumi_npr_y1624_pt6530_pty->SetLineWidth(2);
    lumi_npr_y1624_pt365->SetLineWidth(2);
    
    if (systBoxType == 1) lumi_pr_y024_pt6530->SetFillStyle(4000);
    lumi_pr_y012_pt6530->SetFillStyle(4000);
    lumi_pr_y1216_pt6530->SetFillStyle(4000);
    lumi_pr_y1624_pt6530->SetFillStyle(4000);
    lumi_pr_y1624_pt6530_pty->SetFillStyle(4000);
    lumi_pr_y1624_pt365->SetFillStyle(4000);

    if (systBoxType == 1) lumi_npr_y024_pt6530->SetFillStyle(4000);
    lumi_npr_y012_pt6530->SetFillStyle(4000);
    lumi_npr_y1216_pt6530->SetFillStyle(4000);
    lumi_npr_y1624_pt6530->SetFillStyle(4000);
    lumi_npr_y1624_pt6530_pty->SetFillStyle(4000);
    lumi_npr_y1624_pt365->SetFillStyle(4000);

    if (systBoxType == 1) globalpp_pr_y024_pt6530->SetFillColor(kRed-9);
    globalpp_pr_y012_pt6530->SetFillColor(kAzure-9);
    globalpp_pr_y1216_pt6530->SetFillColor(kRed-9);
    globalpp_pr_y1624_pt6530->SetFillColor(kGreen-9);
    globalpp_pr_y1624_pt6530_pty->SetFillColor(kGreen-9);
    globalpp_pr_y1624_pt365->SetFillColor(kViolet-9);

    if (systBoxType == 1) globalpp_npr_y024_pt6530->SetFillColor(kOrange-9);
    globalpp_npr_y012_pt6530->SetFillColor(kAzure-9);
    globalpp_npr_y1216_pt6530->SetFillColor(kRed-9);
    globalpp_npr_y1624_pt6530->SetFillColor(kGreen-9);
    globalpp_npr_y1624_pt6530_pty->SetFillColor(kGreen-9);
    globalpp_npr_y1624_pt365->SetFillColor(kViolet-9);
  }

  //-------------------------------------------
  TF1 *f4 = new TF1("f4","1",0,400);
  f4->SetLineWidth(1);
  f4->GetXaxis()->SetTitle("N_{part}");
  f4->GetYaxis()->SetTitle("R_{AA}");
  f4->GetYaxis()->SetRangeUser(0.0,1.5);
  f4->GetXaxis()->CenterTitle(kTRUE);

  TLegend *leg11a = new TLegend(0.6,0.5,0.85,0.65);
  leg11a->SetFillStyle(0);
  leg11a->SetFillColor(0);
  leg11a->SetBorderSize(0);
  leg11a->SetMargin(0.2);
  leg11a->SetTextSize(0.04);

  leg11a->AddEntry(gPrJpsiSyst_pt6530y012,"|y|<1.2","f");
  leg11a->AddEntry(gPrJpsiSyst_pt6530y1216,"1.2<|y|<1.6","f");
  leg11a->AddEntry(gPrJpsiSyst_pt6530y1624,"1.6<|y|<2.4","f");

  TLegend *leg11b = new TLegend(0.5,0.52,0.8,0.65);
  leg11b->SetFillStyle(0);
  leg11b->SetFillColor(0);
  leg11b->SetBorderSize(0);
  leg11b->SetMargin(0.2);
  leg11b->SetTextSize(0.04);
  leg11b->AddEntry(gPrJpsiSyst_pt6530y1624,"6.5<p_{T}<30 GeV/c","f");
  leg11b->AddEntry(gPrJpsiSyst_pt365y1624,"3<p_{T}<6.5 GeV/c","f");

  TLegend *leg21a = new TLegend(0.6,0.5,0.85,0.65);
  leg21a->SetFillStyle(0);
  leg21a->SetFillColor(0);
  leg21a->SetBorderSize(0);
  leg21a->SetMargin(0.2);
  leg21a->SetTextSize(0.04);
  leg21a->AddEntry(gNonPrJpsiSyst_pt6530y012,"|y|<1.2","f");
  leg21a->AddEntry(gNonPrJpsiSyst_pt6530y1216,"1.2<|y|<1.6","f");
  leg21a->AddEntry(gNonPrJpsiSyst_pt6530y1624,"1.6<|y|<2.4","f");

  TLegend *leg21b = new TLegend(0.55,0.5,0.8,0.65);
  leg21b->SetFillStyle(0);
  leg21b->SetFillColor(0);
  leg21b->SetBorderSize(0);
  leg21b->SetMargin(0.2);
  leg21b->SetTextSize(0.04);
  leg21b->AddEntry(gNonPrJpsiSyst_pt6530y1624,"6.5<p_{T}<30 GeV/c","f");
  leg21b->AddEntry(gNonPrJpsiSyst_pt365y1624, "3<p_{T}<6.5 GeV/c","f");

  //---------------- general stuff
  TLatex *lPr = new TLatex(20,1.35,"Prompt J/#psi");
  lPr->SetTextFont(42);
  lPr->SetTextSize(0.05);

  TLatex *lNpr = new TLatex(20,1.35,"Non-prompt J/#psi");
  lNpr->SetTextFont(42);
  lNpr->SetTextSize(0.05);

  TLatex *ly = new TLatex(190.0,1.30,"|y| < 2.4");
  ly->SetTextFont(42);
  ly->SetTextSize(0.05);

  TLatex *lyfwd = new TLatex(190.0,0.85,"1.6 < |y| < 2.4");
  lyfwd->SetTextFont(42);
  lyfwd->SetTextSize(0.05);

  TLatex *lpt = new TLatex(190.0,0.81,"6.5 < p_{T} < 30 GeV/c");
  lpt->SetTextFont(42);
  lpt->SetTextSize(0.05);

  TLatex *lpthigh = new TLatex(190.0,1.15,"6.5 < p_{T} < 30 GeV/c");
  lpthigh->SetTextFont(42);
  lpthigh->SetTextSize(0.05);


  // ##################################################### pr plots
  TCanvas *c1 = new TCanvas("c1","c1");
  // general stuff
  f4->Draw();// axis 
  lumi_pr_y024_pt6530->Draw();
  f4->Draw("same");

  lPr->Draw();
  ly->Draw();
  lpthigh->Draw();
 
  CMS_lumi(c1,103,33);
    
  gPrJpsiSyst->Draw("2");
  c1->SetTitle(" ");
  
  //------------------- (pt, y) dependence
  TCanvas *c11a = new TCanvas("c11a","c11a");
  f4->Draw();// axis 
  lumi_pr_y012_pt6530->Draw();
  lumi_pr_y1216_pt6530->Draw();
  lumi_pr_y1624_pt6530->Draw();
  f4->Draw("same");

  lPr->Draw();
  lpt->Draw();
  leg11a->Draw();
  CMS_lumi(c11a,103,33);
 
  gPrJpsiSyst_pt6530y012->Draw("2");
  gPrJpsiSyst_pt6530y1216->Draw("2");
  gPrJpsiSyst_pt6530y1624->Draw("2");

  gPad->RedrawAxis();

  TCanvas *c11b = new TCanvas("c11b","c11b");
  f4->Draw();
  lumi_pr_y1624_pt6530_pty->Draw();
  lumi_pr_y1624_pt365->Draw();
  f4->Draw("same");

  lPr->Draw();
  lyfwd->Draw();
  CMS_lumi(c11b,103,33);
  leg11b->Draw();
  
  gPrJpsiSyst_pt6530y1624->Draw("2");
  gPrJpsiSyst_pt365y1624->Draw("2");
  
  gPad->RedrawAxis();

  //   // ############################################## non-pr
  //   // ############################################## non-pr
  //   // ############################################## non-pr
  TCanvas *c2 = new TCanvas("c2","c2");
  // general stuff
  f4->Draw();
  lumi_npr_y024_pt6530->Draw();
  f4->Draw("same");

  lNpr->Draw();
  ly->Draw();
  lpthigh->Draw();
  CMS_lumi(c2,103,33);

  gNonPrJpsiSyst->Draw("2");
  gPad->RedrawAxis();

  //------------------- (pt, y) dependence
  TCanvas *c21a = new TCanvas("c21a","c21a");
  f4->Draw();
  lumi_npr_y012_pt6530->Draw();
  lumi_npr_y1216_pt6530->Draw();
  lumi_npr_y1624_pt6530->Draw();
  f4->Draw("same");

  lNpr->Draw();
  lpt->Draw();
  leg21a->Draw();
  CMS_lumi(c21a,103,33);

  gNonPrJpsiSyst_pt6530y012->Draw("2");
  gNonPrJpsiSyst_pt6530y1216->Draw("2");
  gNonPrJpsiSyst_pt6530y1624->Draw("2");
   
  gPad->RedrawAxis();

  // pT dependence
  TCanvas *c21b = new TCanvas("c21b","c21b");
  f4->Draw();
  lumi_npr_y1624_pt6530_pty->Draw();
  lumi_npr_y1624_pt365->Draw();
  f4->Draw("same");

  lNpr->Draw();
  lyfwd->Draw(0);
  leg21b->Draw();
  CMS_lumi(c21b,103,33);

  gNonPrJpsiSyst_pt6530y1624->Draw("2");
  gNonPrJpsiSyst_pt365y1624->Draw("2");
    
  gPad->RedrawAxis();
    
  //------------------ write the output file with the Graphs of systematic uncertainties
  TFile *pfOutput = new TFile(Form("%s/raaSystUncert_cent.root",outputDir),"RECREATE");
 
  gPrJpsiSyst->Write("gPrJpsiSyst");             
  gPrJpsiSyst_pt6530y012->Write("gPrJpsiSyst_pt6530y012");
  gPrJpsiSyst_pt6530y1216->Write("gPrJpsiSyst_pt6530y1216");
  gPrJpsiSyst_pt6530y1624->Write("gPrJpsiSyst_pt6530y1624");
  gPrJpsiSyst_pt365y1624->Write("gPrJpsiSyst_pt365y1624");
 
  // nonPr
  gNonPrJpsiSyst->Write("gNonPrJpsiSyst");         
  gNonPrJpsiSyst_pt6530y012->Write("gNonPrJpsiSyst_pt6530y012");
  gNonPrJpsiSyst_pt6530y1216->Write("gNonPrJpsiSyst_pt6530y1216"); 
  gNonPrJpsiSyst_pt6530y1624->Write("gNonPrJpsiSyst_pt6530y1624"); 
  gNonPrJpsiSyst_pt365y1624->Write("gNonPrJpsiSyst_pt365y1624");  

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
  
  if (systBoxType == 1)
    globalpp_pr_y024_pt6530->Write("globalpp_pr_y024_pt6530");
  globalpp_pr_y012_pt6530->Write("globalpp_pr_y012_pt6530");
  globalpp_pr_y1216_pt6530->Write("globalpp_pr_y1216_pt6530");
  globalpp_pr_y1624_pt6530->Write("globalpp_pr_y1624_pt6530");
  globalpp_pr_y1624_pt6530_pty->Write("globalpp_pr_y1624_pt6530_pty");
  globalpp_pr_y1624_pt365->Write("globalpp_pr_y1624_pt365");

  if (systBoxType == 1)
    globalpp_npr_y024_pt6530->Write("globalpp_npr_y024_pt6530");
  globalpp_npr_y012_pt6530->Write("globalpp_npr_y012_pt6530");
  globalpp_npr_y1216_pt6530->Write("globalpp_npr_y1216_pt6530");
  globalpp_npr_y1624_pt6530->Write("globalpp_npr_y1624_pt6530");
  globalpp_npr_y1624_pt6530_pty->Write("globalpp_npr_y1624_pt6530_pty");
  globalpp_npr_y1624_pt365->Write("globalpp_npr_y1624_pt365");
  
  if(bSavePlots)
  {
    c1->Write();
    c11a->Write();
    c11b->Write();
    c2->Write();
    c21a->Write();
    c21b->Write();
  }

  pfOutput->Write();                          
}
