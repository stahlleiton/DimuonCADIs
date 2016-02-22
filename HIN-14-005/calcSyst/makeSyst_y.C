/*
Macro to calcualte the systm uncertainties for the Raa vs pt: bin-by-bin and global uncert.

Input: root files produced by the readFitTable/makeHisto_raa_syst.C (check there the input file names)

Output: root file with the systm. histograms for Raa vs pT.

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

#include "../macro_raa/dataBinning_2015.h"
#include "../macro_raa/filesRaa_2015.h"
#include "numbersRaaSyst_2015.h"

#include "../CMS_lumi.C"
#include "../tdrstyle.C"
#endif

void makeSyst_y( bool bSavePlots        = 1,
                 bool bDoDebug         = 1, // prints numbers, numerator, denominator, to help figure out if things are read properly
                 int method            = 1, // 0: rms of all variations; 1: max of each variation type, added in quadrature
                 const char* inputDir  = "../readFitTable", // the place where the input root files, with the histograms are
                 const char* outputDir = "histSyst")// where the output figures will be
{
  gSystem->mkdir(Form("./%s/",outputDir), kTRUE);
  // set the style
  setTDRStyle();
 
  // type of available comparisons:
  const char* system[2] = {"aa", "pp"};

  const int nInHist = 2;
  const char* yieldHistNames[nInHist] = {"y", "y_mb"};

  // input files: are in the filesRaa_2015.h
  TH1F *phCorr_pr_pp;
  TH1F *phCorr_pr_aa;

  TH1F *phCorr_npr_pp;
  TH1F *phCorr_npr_aa;

  TH1F *phCorrVar_pr_pp;
  TH1F *phCorrVar_pr_aa;

  TH1F *phCorrVar_npr_pp;
  TH1F *phCorrVar_npr_aa;

  for(int ih=0; ih<nInHist;ih++)// for each kinematic rangea
  { 
    // open the files with yields and do the math
    // nominal yields
    TFile *fYesWeighFile_aa   = new TFile(Form("%s/%s",inputDir,yieldHistFile_yesWeight_1[0]));
    TFile *fYesWeighFile_pp   = new TFile(Form("%s/%s",inputDir,yieldHistFile_yesWeight_1[1]));

    if (!fYesWeighFile_aa->IsOpen() || !fYesWeighFile_pp->IsOpen()) {
      cout << "One or more input files are missing" << endl;
      return ;
    }

    TString hist_pr(Form("phPrp_%s",yieldHistNames[ih]));
    TString hist_npr(Form("phNPrp_%s",yieldHistNames[ih]));
  
    cout <<"histogram input name: "<< hist_pr<<"\t"<<hist_npr<<endl; 
    
    // prompt histos
    phCorr_pr_pp = (TH1F*)fYesWeighFile_pp->Get(hist_pr);
    phCorr_pr_aa = (TH1F*)fYesWeighFile_aa->Get(hist_pr);
    phCorr_pr_pp->SetDirectory(0);
    phCorr_pr_aa->SetDirectory(0);

    // non-prompt histos
    phCorr_npr_pp = (TH1F*)fYesWeighFile_pp->Get(hist_npr);
    phCorr_npr_aa = (TH1F*)fYesWeighFile_aa->Get(hist_npr);
    phCorr_npr_pp->SetDirectory(0);
    phCorr_npr_aa->SetDirectory(0);

    fYesWeighFile_aa->Close();
    fYesWeighFile_pp->Close();

    int numBins = 0;
    if(ih==0) numBins = nBinsY;
    if(ih==1) numBins = nBinsY3;

    // high-pt, minbias
    float syst_fit_pr_aa[numBins][nFitVariations] ;
    float syst_eff4d_pr_aa[numBins][nEff4DVariations] ;
    float syst_effTnP_pr_aa[numBins][nEffTnPVariation];

    float syst_fit_pr_pp[numBins][nFitVariations]     ;
    float syst_eff4d_pr_pp[numBins][nEff4DVariations] ;
    float syst_effTnP_pr_pp[numBins][nEffTnPVariation];
  
    float syst_fit_npr_aa[numBins][nFitVariations]    ;
    float syst_eff4d_npr_aa[numBins][nEff4DVariations];
    float syst_effTnP_npr_aa[numBins][nEffTnPVariation];

    float syst_fit_npr_pp[numBins][nFitVariations]     ;
    float syst_eff4d_npr_pp[numBins][nEff4DVariations] ;
    float syst_effTnP_npr_pp[numBins][nEffTnPVariation];
  
    // rapidity dependence integrated, x-check; for mb
    float syst_fit_mb_pr_aa[numBins][nFitVariations]      ;
    float syst_eff4d_mb_pr_aa[numBins][nEff4DVariations]  ;
    float syst_effTnP_mb_pr_aa[numBins][nEffTnPVariation] ;
  
    float syst_fit_mb_pr_pp[numBins][nFitVariations]      ;
    float syst_eff4d_mb_pr_pp[numBins][nEff4DVariations]  ;
    float syst_effTnP_mb_pr_pp[numBins][nEffTnPVariation] ;
  
    float syst_fit_mb_npr_aa[numBins][nFitVariations]      ;
    float syst_eff4d_mb_npr_aa[numBins][nEff4DVariations]  ;
    float syst_effTnP_mb_npr_aa[numBins][nEffTnPVariation] ;
  
    float syst_fit_mb_npr_pp[numBins][nFitVariations]      ;
    float syst_eff4d_mb_npr_pp[numBins][nEff4DVariations]  ;
    float syst_effTnP_mb_npr_pp[numBins][nEffTnPVariation] ;

    cout <<"###################################################################"<<endl;
    if(bDoDebug) cout << "################ Kinematic region: " <<yieldHistNames[ih]<<endl;

    for(int ibin=1; ibin<=numBins; ibin++)
    {
      float fitContribution_pr_aa    = 0;
      float eff4dContribution_pr_aa  = 0;
      float efftnpContribution_pr_aa = 0; 

      float fitContribution_pr_pp    = 0;
      float eff4dContribution_pr_pp  = 0;
      float efftnpContribution_pr_pp = 0;

      float fitContribution_npr_aa    = 0;
      float eff4dContribution_npr_aa  = 0;
      float efftnpContribution_npr_aa = 0; 

      float fitContribution_npr_pp    = 0;
      float eff4dContribution_npr_pp  = 0;
      float efftnpContribution_npr_pp = 0;


      float fitContribution_mb_pr_aa    = 0;
      float eff4dContribution_mb_pr_aa  = 0;
      float efftnpContribution_mb_pr_aa = 0; 

      float fitContribution_mb_pr_pp    = 0;
      float eff4dContribution_mb_pr_pp  = 0;
      float efftnpContribution_mb_pr_pp = 0;
        
      float fitContribution_mb_npr_aa    = 0;
      float eff4dContribution_mb_npr_aa  = 0;
      float efftnpContribution_mb_npr_aa = 0; 

      float fitContribution_mb_npr_pp    = 0;
      float eff4dContribution_mb_npr_pp  = 0;
      float efftnpContribution_mb_npr_pp = 0;

      //nominal prompt and non-prompt yield ratios
      float yield_aa_pr  = phCorr_pr_aa->GetBinContent(ibin);
      float yield_aa_npr = phCorr_npr_aa->GetBinContent(ibin);
      
      float yield_pp_pr  = phCorr_pr_pp->GetBinContent(ibin);
      float yield_pp_npr = phCorr_npr_pp->GetBinContent(ibin);


      float scaleFactor    = ppLumi/nMbEvents;
      float scale_cent     = 1/(adTaaMB[0]*adDeltaCentMB[0]);
      float yieldRatio_pr  = yield_aa_pr/yield_pp_pr   * scaleFactor * scale_cent;
      float yieldRatio_npr = yield_aa_npr/yield_pp_npr * scaleFactor * scale_cent;
      
      if(bDoDebug) cout << "################ Bin " <<ibin << endl
                        << "  Nominal yields are (prompt_aa & nonPr_aa ; prompt_pp & nonPr_pp): " 
                        << yield_aa_pr<<"\t& "<<yield_aa_npr<<"\t; "<<yield_pp_pr<<"\t& "<<yield_pp_npr<<endl;

      // for each source of uncert, calc variation wrt nominal value
      for(int ivar=0; ivar<(nFitVariations+nEff4DVariations+nEffTnPVariation); ivar++) {
        cout <<"@@@@@@@ Variation = " << ivar <<endl; 
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

        if(bDoDebug) cout << "Opened systematic files:\n pp: "<<nameFile2 << "\t AA: "<< nameFile1 <<endl;
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
        
        const float yieldVar_aa_pr  = phCorrVar_pr_aa->GetBinContent(ibin);
        const float yieldVar_aa_npr = phCorrVar_npr_aa->GetBinContent(ibin);

        const float yieldVar_pp_pr  = phCorrVar_pr_pp->GetBinContent(ibin);
        const float yieldVar_pp_npr = phCorrVar_npr_pp->GetBinContent(ibin);

        const double relVar_aa_pr = (yield_aa_pr - yieldVar_aa_pr)/yield_aa_pr;
        const double relVar_pp_pr = (yield_pp_pr - yieldVar_pp_pr)/yield_pp_pr; 

        const double relVar_aa_npr = (yield_aa_npr - yieldVar_aa_npr)/yield_aa_npr;
        const double relVar_pp_npr = (yield_pp_npr - yieldVar_pp_npr)/yield_pp_npr;

        if(bDoDebug) cout << "+++++++++++ Variation " << ivar << " yields are: "
                          <<yieldVar_aa_pr<<"\t& "<<yieldVar_aa_npr<<"\t;  "<<yieldVar_pp_pr<<"\t& "<<yieldVar_pp_npr<<endl;
      
        switch(ih){
          case 0: // minbias, high-pt
            prJpsi_y[ibin-1]      = yieldRatio_pr;
            nonPrJpsi_y[ibin-1]   = yieldRatio_npr;
            // fit 
            if(ivar<nFitVariations)
            {
              syst_fit_pr_aa[ibin-1][ivar]  = TMath::Power( relVar_aa_pr,2 );
              syst_fit_pr_pp[ibin-1][ivar]  = TMath::Power( relVar_pp_pr,2 );

              syst_fit_npr_aa[ibin-1][ivar] = TMath::Power( relVar_aa_npr,2 ) ;
              syst_fit_npr_pp[ibin-1][ivar] = TMath::Power( relVar_pp_npr,2 ) ;
              
              if(bDoDebug) cout << "++++++++++++++++++++++ Delta yields: "
                                << syst_fit_pr_aa[ibin-1][ivar] << "\t& " << syst_fit_npr_aa[ibin-1][ivar] << "\t; "
                                << syst_fit_pr_pp[ibin-1][ivar] << "\t& " << syst_fit_npr_pp[ibin-1][ivar] << endl;
              if(method==1)//maximum
              {
                if( syst_fit_pr_aa[ibin-1][ivar] > fitContribution_pr_aa ) fitContribution_pr_aa = syst_fit_pr_aa[ibin-1][ivar];
                if( syst_fit_pr_pp[ibin-1][ivar] > fitContribution_pr_pp ) fitContribution_pr_pp = syst_fit_pr_pp[ibin-1][ivar];
                  
                if( syst_fit_npr_aa[ibin-1][ivar] > fitContribution_npr_aa ) fitContribution_npr_aa = syst_fit_npr_aa[ibin-1][ivar];
                if( syst_fit_npr_pp[ibin-1][ivar] > fitContribution_npr_pp ) fitContribution_npr_pp = syst_fit_npr_pp[ibin-1][ivar];
                  
              }
              if(method==0) // rms of all variations
              {
                fitContribution_pr_aa += syst_fit_pr_aa[ibin-1][ivar];
                fitContribution_pr_pp += syst_fit_pr_pp[ibin-1][ivar];
                
                fitContribution_npr_aa += syst_fit_npr_aa[ibin-1][ivar];
                fitContribution_npr_pp += syst_fit_npr_pp[ibin-1][ivar];
              }
              if(bDoDebug) cout << "+++++++++++++++++++++++++++++++++ Fit contribution to systm: "
                                << fitContribution_pr_aa << "\t& " << fitContribution_npr_aa << "\t; "
                                << fitContribution_pr_pp << "\t& " << fitContribution_npr_pp << endl;
            }
            // 4d eff
            if(ivar>=nFitVariations && ivar<(nFitVariations+nEff4DVariations) )
            {
              int ifile = ivar-nFitVariations;
              syst_eff4d_pr_aa[ibin-1][ifile]  = TMath::Power( relVar_aa_pr,2 ) ;
              syst_eff4d_pr_pp[ibin-1][ifile]  = TMath::Power( relVar_pp_pr,2 ) ;
              
              syst_eff4d_npr_aa[ibin-1][ifile]  = TMath::Power( relVar_aa_npr,2 ) ;
              syst_eff4d_npr_pp[ibin-1][ifile]  = TMath::Power( relVar_pp_npr,2 ) ;
              
              if(bDoDebug) cout << "++++++++++++++++++++++ Delta yields: "
                                << syst_eff4d_pr_aa[ibin-1][ifile] << "\t& " << syst_eff4d_npr_aa[ibin-1][ifile] << "\t; "
                                << syst_eff4d_pr_pp[ibin-1][ifile] << "\t& " << syst_eff4d_npr_pp[ibin-1][ifile] << endl;

              if(method==1)//maximum
              {
                if( syst_eff4d_pr_aa[ibin-1][ifile] > eff4dContribution_pr_aa ) eff4dContribution_pr_aa = syst_eff4d_pr_aa[ibin-1][ifile];
                if( syst_eff4d_pr_pp[ibin-1][ifile] > eff4dContribution_pr_pp ) eff4dContribution_pr_pp = syst_eff4d_pr_pp[ibin-1][ifile];
                
                if( syst_eff4d_npr_aa[ibin-1][ifile] > eff4dContribution_npr_aa ) eff4dContribution_npr_aa = syst_eff4d_npr_aa[ibin-1][ifile];
                if( syst_eff4d_npr_pp[ibin-1][ifile] > eff4dContribution_npr_pp ) eff4dContribution_npr_pp = syst_eff4d_npr_pp[ibin-1][ifile];
              }
              if(method==0) // rms of all variations
              {
                eff4dContribution_pr_aa += syst_eff4d_pr_aa[ibin-1][ifile];
                eff4dContribution_pr_pp += syst_eff4d_pr_pp[ibin-1][ifile];
                  
                eff4dContribution_npr_aa += syst_eff4d_npr_aa[ibin-1][ifile];
                eff4dContribution_npr_pp += syst_eff4d_npr_pp[ibin-1][ifile];
              }
              if(bDoDebug) cout << "+++++++++++++++++++++++++++++++++ 4DEff Contribution to systm: "
                                << eff4dContribution_pr_aa << "\t& " << eff4dContribution_npr_aa << "\t; "
                                << eff4dContribution_pr_pp << "\t& " << eff4dContribution_npr_pp << endl;
            }

            // TnP and 3d eff
            if( (ivar>=(nFitVariations+nEff4DVariations)) && (ivar < (nFitVariations+nEff4DVariations+nEffTnPVariation)) )
            {
              int ifile = ivar-nFitVariations-nEff4DVariations;
              syst_effTnP_pr_aa[ibin-1][ifile]  = TMath::Power( yieldVar_aa_pr,2 ) ;
              syst_effTnP_pr_pp[ibin-1][ifile]  = TMath::Power( yieldVar_pp_pr,2 ) ;
              
              syst_effTnP_npr_aa[ibin-1][ifile] = TMath::Power( yieldVar_aa_npr,2 ) ;
              syst_effTnP_npr_pp[ibin-1][ifile] = TMath::Power( yieldVar_pp_npr,2 ) ;

              if(bDoDebug) cout << "++++++++++++++++++++++ Delta yields: "
                                << syst_effTnP_pr_aa[ibin-1][ifile] << "\t& " << syst_effTnP_npr_aa[ibin-1][ifile] << "\t; "
                                << syst_effTnP_pr_pp[ibin-1][ifile] << "\t& " << syst_effTnP_npr_pp[ibin-1][ifile] << endl;

              if(method==1)//maximum
              {
                if( syst_effTnP_pr_aa[ibin-1][ifile] > efftnpContribution_pr_aa ) efftnpContribution_pr_aa = syst_effTnP_pr_aa[ibin-1][ifile];
                if( syst_effTnP_pr_pp[ibin-1][ifile] > efftnpContribution_pr_pp ) efftnpContribution_pr_pp = syst_effTnP_pr_pp[ibin-1][ifile];
                  
                if( syst_effTnP_npr_aa[ibin-1][ifile] > efftnpContribution_npr_aa ) efftnpContribution_npr_aa = syst_effTnP_npr_aa[ibin-1][ifile];
                if( syst_effTnP_npr_pp[ibin-1][ifile] > efftnpContribution_npr_pp ) efftnpContribution_npr_pp = syst_effTnP_npr_pp[ibin-1][ifile];
              }
              if(method==0) // rms of all variations
              {
                efftnpContribution_pr_aa += syst_effTnP_pr_aa[ibin-1][ifile];
                efftnpContribution_pr_pp += syst_effTnP_pr_pp[ibin-1][ifile];
                  
                efftnpContribution_npr_aa += syst_effTnP_npr_aa[ibin-1][ifile];
                efftnpContribution_npr_pp += syst_effTnP_npr_pp[ibin-1][ifile];
              }
              if(bDoDebug) cout << "+++++++++++++++++++++++++++++++++ Contribution to systm: " << efftnpContribution_pr_aa << "\t& " << efftnpContribution_npr_aa << "\t; "
                                << efftnpContribution_pr_pp << "\t& " << efftnpContribution_npr_pp << endl;
            }
            break;
            
          case 1:// fwd, low-pt: 3bins
            prJpsi_y_y[ibin-1]    = yieldRatio_pr;
            nonPrJpsi_y_y[ibin-1] = yieldRatio_npr;
            if(ivar<nFitVariations)
            {
              syst_fit_mb_pr_aa[ibin-1][ivar]  = TMath::Power( relVar_aa_pr ,2 ) ;
              syst_fit_mb_pr_pp[ibin-1][ivar]  = TMath::Power( relVar_pp_pr ,2) ;
                
              syst_fit_mb_npr_aa[ibin-1][ivar]  = TMath::Power( relVar_aa_npr ,2) ;
              syst_fit_mb_npr_pp[ibin-1][ivar]  = TMath::Power( relVar_pp_npr ,2) ;

              if(bDoDebug) cout << "++++++++++++++++++++++ Delta yields: "
                                << syst_fit_mb_pr_aa[ibin-1][ivar] << "\t& " << syst_fit_mb_npr_aa[ibin-1][ivar] << "\t; "
                                << syst_fit_mb_pr_pp[ibin-1][ivar] << "\t& " << syst_fit_mb_npr_pp[ibin-1][ivar] << endl;
                
              if(method==1)//maximum
              {
                if( syst_fit_mb_pr_aa[ibin-1][ivar] > fitContribution_mb_pr_aa ) fitContribution_mb_pr_aa = syst_fit_mb_pr_aa[ibin-1][ivar];
                if( syst_fit_mb_pr_pp[ibin-1][ivar] > fitContribution_mb_pr_pp ) fitContribution_mb_pr_pp = syst_fit_mb_pr_pp[ibin-1][ivar];
                  
                if( syst_fit_mb_npr_aa[ibin-1][ivar] > fitContribution_mb_npr_aa ) fitContribution_mb_npr_aa = syst_fit_mb_npr_aa[ibin-1][ivar];
                if( syst_fit_mb_npr_pp[ibin-1][ivar] > fitContribution_mb_npr_pp ) fitContribution_mb_npr_pp = syst_fit_mb_npr_pp[ibin-1][ivar];
              }
              if(method==0) // rms of all variations
              {
                fitContribution_mb_pr_aa += syst_fit_mb_pr_aa[ibin-1][ivar];
                fitContribution_mb_pr_pp += syst_fit_mb_pr_pp[ibin-1][ivar];
                  
                fitContribution_mb_npr_aa += syst_fit_mb_npr_aa[ibin-1][ivar];
                fitContribution_mb_npr_pp += syst_fit_mb_npr_pp[ibin-1][ivar];
              }
              if(bDoDebug) cout << "+++++++++++++++++++++++++++++++++ Fit contribution to systm: "
                                << fitContribution_mb_pr_aa << "\t& " << fitContribution_mb_npr_aa << "\t; "
                                << fitContribution_mb_pr_pp << "\t& " << fitContribution_mb_npr_pp << endl;
            }
            if(ivar>=nFitVariations && ivar<(nFitVariations+nEff4DVariations) )
            {
              int ifile = ivar-nFitVariations;
              syst_eff4d_mb_pr_aa[ibin-1][ifile]  = TMath::Power( relVar_aa_pr,2 ) ;
              syst_eff4d_mb_pr_pp[ibin-1][ifile]  = TMath::Power( relVar_pp_pr ,2 ) ;
              
              syst_eff4d_mb_npr_aa[ibin-1][ifile]  = TMath::Power( relVar_aa_npr ,2) ;
              syst_eff4d_mb_npr_pp[ibin-1][ifile]  = TMath::Power( relVar_pp_npr ,2) ;
              
              if(bDoDebug) cout << "++++++++++++++++++++++ Delta yields: "
                                << syst_eff4d_mb_pr_aa[ibin-1][ifile] << "\t& " << syst_eff4d_mb_npr_aa[ibin-1][ifile] << "\t; "
                                << syst_eff4d_mb_pr_pp[ibin-1][ifile] << "\t& " << syst_eff4d_mb_npr_pp[ibin-1][ifile] << endl;

              if(method==1)//maximum
              {
                if( syst_eff4d_mb_pr_aa[ibin-1][ifile] > eff4dContribution_mb_pr_aa ) eff4dContribution_mb_pr_aa = syst_eff4d_mb_pr_aa[ibin-1][ifile];
                if( syst_eff4d_mb_pr_pp[ibin-1][ifile] > eff4dContribution_mb_pr_pp ) eff4dContribution_mb_pr_pp = syst_eff4d_mb_pr_pp[ibin-1][ifile];
                
                if( syst_eff4d_mb_npr_aa[ibin-1][ifile] > eff4dContribution_mb_npr_aa ) eff4dContribution_mb_npr_aa = syst_eff4d_mb_npr_aa[ibin-1][ifile];
                if( syst_eff4d_mb_npr_pp[ibin-1][ifile] > eff4dContribution_mb_npr_pp ) eff4dContribution_mb_npr_pp = syst_eff4d_mb_npr_pp[ibin-1][ifile];
              }
              if(method==0) // rms of all variations
              {
                eff4dContribution_mb_pr_aa += syst_eff4d_mb_pr_aa[ibin-1][ifile];
                eff4dContribution_mb_pr_pp += syst_eff4d_mb_pr_pp[ibin-1][ifile];
                
                eff4dContribution_mb_npr_aa += syst_eff4d_mb_npr_aa[ibin-1][ifile];
                eff4dContribution_mb_npr_pp += syst_eff4d_mb_npr_pp[ibin-1][ifile];
              }
              if(bDoDebug) cout << "+++++++++++++++++++++++++++++++++ Fit contribution to systm: "
                                << eff4dContribution_mb_pr_aa << "\t& " << eff4dContribution_mb_npr_aa << "\t; "
                                << eff4dContribution_mb_pr_pp << "\t& " << eff4dContribution_mb_npr_pp << endl;
            }
            if( (ivar>=(nFitVariations+nEff4DVariations)) && (ivar < (nFitVariations+nEff4DVariations+nEffTnPVariation)) )
            {
              int ifile = ivar-nFitVariations-nEff4DVariations;
              
              syst_effTnP_mb_pr_aa[ibin-1][ifile]  = TMath::Power( yieldVar_aa_pr ,2 ) ;
              syst_effTnP_mb_pr_pp[ibin-1][ifile]  = TMath::Power( yieldVar_pp_pr ,2 ) ;
                
              syst_effTnP_mb_npr_aa[ibin-1][ifile]  = TMath::Power( yieldVar_aa_npr ,2) ;
              syst_effTnP_mb_npr_pp[ibin-1][ifile]  = TMath::Power( yieldVar_pp_npr ,2) ;
              if(bDoDebug) cout << "++++++++++++++++++++++ Delta yields:  "
                                << syst_effTnP_mb_pr_aa[ibin-1][ifile] << "\t& " << syst_effTnP_mb_npr_aa[ibin-1][ifile] << "\t; "
                                << syst_effTnP_mb_pr_pp[ibin-1][ifile] << "\t& " << syst_effTnP_mb_npr_pp[ibin-1][ifile] << endl;
                            
              if(method==1)//maximum
              {
                if( syst_effTnP_mb_pr_aa[ibin-1][ifile] > efftnpContribution_mb_pr_aa ) {efftnpContribution_mb_pr_aa = syst_effTnP_mb_pr_aa[ibin-1][ifile]; }
                if( syst_effTnP_mb_pr_pp[ibin-1][ifile] > efftnpContribution_mb_pr_pp ) {efftnpContribution_mb_pr_pp = syst_effTnP_mb_pr_pp[ibin-1][ifile];}
                
                if( syst_effTnP_mb_npr_aa[ibin-1][ifile] > efftnpContribution_mb_npr_aa ) {efftnpContribution_mb_npr_aa = syst_effTnP_mb_npr_aa[ibin-1][ifile];}
                if( syst_effTnP_mb_npr_pp[ibin-1][ifile] > efftnpContribution_mb_npr_pp ) {efftnpContribution_mb_npr_pp = syst_effTnP_mb_npr_pp[ibin-1][ifile];}
              }
              if(method==0) // rms of all variations
              {
                efftnpContribution_mb_pr_aa += syst_effTnP_mb_pr_aa[ibin-1][ifile];
                efftnpContribution_mb_pr_pp += syst_effTnP_mb_pr_pp[ibin-1][ifile];
                  
                efftnpContribution_mb_npr_aa += syst_effTnP_mb_npr_aa[ibin-1][ifile];
                efftnpContribution_mb_npr_pp += syst_effTnP_mb_npr_pp[ibin-1][ifile];
              }
              if(bDoDebug) cout << "+++++++++++++++++++++++++++++++++ Fit contribution to systm: "
                                << efftnpContribution_mb_pr_aa << "\t& " << efftnpContribution_mb_npr_aa << "\t; "
                                << efftnpContribution_mb_pr_pp << "\t& " << efftnpContribution_mb_npr_pp << endl;
            }
            break;
        }//switch ih==kinematic range 
      }// fit variation ivar (fit, 4dEff, tnp)
        
      switch(ih) {
        case 0:
          prJpsiErrSyst_y[ibin-1] = yieldRatio_pr * TMath::Sqrt((fitContribution_pr_aa+eff4dContribution_pr_aa+efftnpContribution_pr_aa)+
                                                                (fitContribution_pr_pp+eff4dContribution_pr_pp+efftnpContribution_pr_pp));
          nonPrJpsiErrSyst_y[ibin-1] = yieldRatio_npr * TMath::Sqrt((fitContribution_npr_aa+eff4dContribution_npr_aa+efftnpContribution_npr_aa) +
                                                                    (fitContribution_npr_pp+eff4dContribution_npr_pp+efftnpContribution_npr_pp));  
          if(method==0)
          {   
            prJpsiErrSyst_y[ibin-1]    *= 1./TMath::Sqrt(nFitVariations+nEff4DVariations+nEffTnPVariation); 
            nonPrJpsiErrSyst_y[ibin-1] *= 1./TMath::Sqrt(nFitVariations+nEff4DVariations+nEffTnPVariation); 
          }
          if(bDoDebug)
          {
            cout <<"---------------------------------------------------------------"<<endl;
            cout << "Ingredients to the total systm. uncertainty"<<endl;
            cout <<"Prompt yields systematics: aa & pp"<<endl;
            cout <<"fitContribution: "<<fitContribution_pr_aa<<"\t "<<fitContribution_pr_pp<<endl;
            cout <<"eff4dContribution: "<<eff4dContribution_pr_aa<<"\t"<<eff4dContribution_pr_pp<<endl;
            cout <<"efftnpContribution: "<<efftnpContribution_pr_aa<<"\t"<<efftnpContribution_pr_pp<<endl;
            cout <<"yields: "<<yield_aa_pr<<"\t"<<yield_pp_pr<<endl;
            cout <<"yield ratio: "<<yieldRatio_pr<<endl;
            cout <<"Total: "<<prJpsiErrSyst_y[ibin-1]<<endl;
          }
          break;

        case 1:
          prJpsiErrSyst_y_y[ibin-1] = yieldRatio_pr * TMath::Sqrt((fitContribution_mb_pr_aa+eff4dContribution_mb_pr_aa+efftnpContribution_mb_pr_aa) +
                                                                  (fitContribution_mb_pr_pp+eff4dContribution_mb_pr_pp+efftnpContribution_mb_pr_pp));
          nonPrJpsiErrSyst_y_y[ibin-1] = yieldRatio_npr * TMath::Sqrt((fitContribution_mb_npr_aa+eff4dContribution_mb_npr_aa+efftnpContribution_mb_npr_aa) +
                                                                      (fitContribution_mb_npr_pp+eff4dContribution_mb_npr_pp+efftnpContribution_mb_npr_pp));  
          if(method==0)
          {   
            prJpsiErrSyst_y_y[ibin-1]    *= 1./TMath::Sqrt(nFitVariations+nEff4DVariations+nEffTnPVariation); 
            nonPrJpsiErrSyst_y_y[ibin-1] *= 1./TMath::Sqrt(nFitVariations+nEff4DVariations+nEffTnPVariation); 
          }
          if(bDoDebug)
          {
            cout <<"---------------------------------------------------------------"<<endl;
            cout << "Ingredients to the total systm. uncertainty"<<endl;
            cout <<"Prompt yields systematics: aa & pp"<<endl;
            cout <<"fitContribution: "<<fitContribution_mb_pr_aa<<"\t "<<fitContribution_mb_pr_pp<<endl;
            cout <<"eff4dContribution: "<<eff4dContribution_mb_pr_aa<<"\t"<<eff4dContribution_mb_pr_pp<<endl;
            cout <<"efftnpContribution: "<<efftnpContribution_mb_pr_aa<<"\t"<<efftnpContribution_mb_pr_pp<<endl;
            cout <<"yields: "<<yield_aa_pr<<"\t"<<yield_pp_pr<<endl;
            cout <<"yield ratio: "<<yieldRatio_pr<<endl;
            cout <<"Total: "<<prJpsiErrSyst_y_y[ibin-1]<<endl;
          }
          break;
      
        default:
          break;
         
      } //switch end
           
    }//loop end: for(int ibin=1; ibin<=numBins; ibin++):
  }//loop end: for(int ih=0; ih<nInHist;ih++) for each kinematic range 

  // ***** //Drawing
  // pr
  TGraphErrors *gPrJpsiSyst = new TGraphErrors(nBinsY, binsY, prJpsi_y, binsYX, prJpsiErrSyst_y);
  TGraphErrors *gPrJpsiSyst_y_y = new TGraphErrors(nBinsY3, binsY3, prJpsi_y_y, binsY3X,   prJpsiErrSyst_y_y);
 
  // nonPr   
  TGraphErrors *gNonPrJpsiSyst = new TGraphErrors(nBinsY, binsY, nonPrJpsi_y, binsYX, nonPrJpsiErrSyst_y);
  TGraphErrors *gNonPrJpsiSyst_y_y = new TGraphErrors(nBinsY3, binsY3, nonPrJpsi_y_y, binsY3X,   nonPrJpsiErrSyst_y_y);
  //-------------------------------------------------------------------
  //stat boxes
  gPrJpsiSyst->SetFillColor(kRed-9);
  gPrJpsiSyst_y_y->SetFillColor(kViolet-9);

  // non-pr
  gNonPrJpsiSyst->SetFillColor(kOrange-9);
  gNonPrJpsiSyst_y_y->SetFillColor(kViolet-9);
  //------------------------------- luminosity calcualtion
  double systLumi      = 0;
  double systSelection = 0;
  for (int iglb=0; iglb<2; iglb++)
  {
    systLumi      += TMath::Power(systLumis[iglb],2);
    systSelection += TMath::Power(systEventSelection[iglb],2);
  }
  double globalSyst  = TMath::Sqrt(systLumi+systSelection);
  TBox *lumi = new TBox(2.33,1-globalSyst,2.4,1+globalSyst);
  lumi->SetFillColor(kGray+1);

  //-------------------------------------------
  TF1 *f4 = new TF1("f4","1",0,2.4);
  f4->SetLineWidth(1);
  f4->GetXaxis()->SetTitle("|y|");
  f4->GetXaxis()->SetNdivisions(-6);
  f4->GetYaxis()->SetTitle("R_{AA}");
  f4->GetYaxis()->SetRangeUser(0.0,1.5);
  f4->GetXaxis()->CenterTitle(kTRUE);

  //---------------- general stuff
  TLatex *lPr = new TLatex(0.2,1.35,"Prompt J/#psi");
  lPr->SetTextFont(42);
  lPr->SetTextSize(0.05);

  TLatex *lNpr = new TLatex(0.2,1.35,"Non-prompt J/#psi");
  lNpr->SetTextFont(42);
  lNpr->SetTextSize(0.05);

  TLatex *lpt = new TLatex(0.12,0.075,"6.5 < p_{T} < 30 GeV/c");
  lpt->SetTextFont(42);
  lpt->SetTextSize(0.05);
  TLatex *lcent = new TLatex(19,0.85,"Cent. 0-100%");
  lcent->SetTextFont(42);
  lcent->SetTextSize(0.05);

  // ##################################################### pr plots
  TCanvas *c1 = new TCanvas("c1","c1");
  // general stuff
  f4->Draw();// axis

  CMS_lumi(c1,103,33);
  lPr->Draw();
  lcent->Draw();
  lpt->Draw();

  gPrJpsiSyst->Draw("2");
  
  //-------------------minbias dependence
  TCanvas *c11b = new TCanvas("c11b","c11b");
  f4->Draw();
  lumi->Draw();
  f4->Draw("same");

  lPr->Draw();
  lcent->Draw();
  lpt->Draw();
  CMS_lumi(c11b,103,33);

  gPrJpsiSyst_y_y->Draw("2");
   
  gPad->RedrawAxis();
  
  // ############################################## non-pr 
  // ############################################## non-pr
  TCanvas *c2 = new TCanvas("c2","c2");
  // general stuff
  f4->Draw();
  lumi->Draw();
  f4->Draw("same");

  CMS_lumi(c2,103,33);
  lNpr->Draw();
  lcent->Draw();
  lpt->Draw();

  gNonPrJpsiSyst->Draw("2");

  //-------------------minbias dependence
  TCanvas *c22b = new TCanvas("c22b","c22b");
  f4->Draw();
  lumi->Draw();
  f4->Draw("same");

  CMS_lumi(c22b,103,33);
  lNpr->Draw();
  lcent->Draw();
  lpt->Draw();

  gNonPrJpsiSyst_y_y->Draw("2");
  gPad->RedrawAxis();

  //------------------------------------------------------------------------------------- 
  //------------------ write the output file with the Graphs of systematic uncertainties
  TFile *pfOutput = new TFile(Form("%s/raaSystUncert_y.root",outputDir),"RECREATE");

  gPrJpsiSyst->Write("gPrJpsiSyst");
  gPrJpsiSyst_y_y->Write("gPrJpsiSyst_y_y");        

  // nonPr   
  gNonPrJpsiSyst->Write("gNonPrJpsiSyst");        
  gNonPrJpsiSyst_y_y->Write("gNonPrJpsiSyst_y_y");     

  lumi->Write("lumi");

  if(bSavePlots)
  {
    c1->Write();
    c11b->Write();
    c2->Write();
    c22b->Write();
  }

  pfOutput->Write();
  

 
}
