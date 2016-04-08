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

void plotSyst_pt( bool bSavePlots       = 1,
                  bool bDoDebug         = 1, // prints numbers, numerator, denominator, to help figure out if things are read properly
                  const char* inputDir  = "../readFitTable", // the place where the input root files, with the histograms are
                  const char* outputDir = "histPlotSyst")// where the output figures will be
{
  gSystem->mkdir(Form("./%s/png",outputDir), kTRUE);
  gSystem->mkdir(Form("./%s/pdf",outputDir), kTRUE);
  // set the style
  setTDRStyle();
 
  // type of available comparisons:
  const char* system[2] = {"aa", "pp"};

  const int nInHist = 4;
  const char* yieldHistNames[nInHist] = {"pt", "ptLow", "ptLow_mb", "mb"}; // ptLow_mb is MB integrated in, 3-6.5, 6.5-30, and 3-30

  // input files: are in the filesRaa_2015.h
  // open the files with yields and do the math
  // nominal yields
  TFile *fYesWeighFile_aa   = new TFile(Form("%s/%s",inputDir,yieldHistFile_yesWeight_1[0]));
  TFile *fYesWeighFile_pp   = new TFile(Form("%s/%s",inputDir,yieldHistFile_yesWeight_1[1]));
  
  if (!fYesWeighFile_aa->IsOpen() || !fYesWeighFile_pp->IsOpen()) {
    cout << "One or more input files are missing" << endl;
    return ;
  }

  TH1F *phCorr_pr_pp;
  TH1F *phCorr_pr_aa;

  TH1F *phCorr_npr_pp;
  TH1F *phCorr_npr_aa;

  TH1F *phCorrVar_pr_pp;
  TH1F *phCorrVar_pr_aa;

  TH1F *phCorrVar_npr_pp;
  TH1F *phCorrVar_npr_aa;
 
  // ----------------- general drawing area setup
  //---axes
  TF1 *f4 = new TF1("f4","0",0,30);
  f4->SetLineWidth(1);
  f4->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  f4->GetYaxis()->SetTitle(" (nominal-variation)/nominal [%]");
  f4->GetYaxis()->SetRangeUser(-10,10);
  f4->GetXaxis()->CenterTitle(kTRUE);
  f4->GetXaxis()->SetRangeUser(6.5,30);
 
  //---------------------------------------------
  for(int ih=0; ih<nInHist;ih++)// for each kinematic range
  { 
    f4->GetYaxis()->SetRangeUser(-10,10);
    //---------------- laex and shit
    TLatex *ly     = new TLatex(20.0,-8.5,"|y| < 2.4");
    ly->SetTextFont(42);
    ly->SetTextSize(0.05);
    TLatex *lcent = new TLatex(20,-7,"Cent. 0-100%");
    lcent->SetTextFont(42);
    lcent->SetTextSize(0.05);

    TLatex *lPr = new TLatex(7,8,"Prompt J/#psi");
    lPr->SetTextFont(42);
    lPr->SetTextSize(0.05);
    
    TLatex *lNpr = new TLatex(7, 8,"Non-prompt J/#psi");
    lNpr->SetTextFont(42);
    lNpr->SetTextSize(0.05);

    if(ih==1) 
    {
      f4->GetXaxis()->SetRangeUser(3,6.5);
      f4->GetYaxis()->SetRangeUser(-40,40);
      lPr   = new TLatex(3.3,30,"Prompt J/#psi");
      lNpr  = new TLatex(3.3,30,"Non-prompt J/#psi");
      lcent = new TLatex(5.,-25,"Cent. 0-100%");
      ly    = new TLatex(5.,-30,"1.6 < |y| < 2.4");

    }
    if(ih==2) 
    {
      f4->GetXaxis()->SetRangeUser(3,30);
      ly     = new TLatex(5.,-9,"1.6 < |y| < 2.4");
    }
    // -------- drawing setting, for each kinematic region
    TCanvas *c1_pp = new TCanvas("c1_pp","c1_pp", 800,1000);
    c1_pp->Divide(2,2);
    c1_pp->cd(1); f4->Draw();
    c1_pp->cd(2); f4->Draw();
    c1_pp->cd(3); f4->Draw();
    c1_pp->cd(4); f4->Draw();

    TCanvas *c1_aa = new TCanvas("c1_aa","c1_aa", 800,1200);
    c1_aa->Divide(2,2);
    c1_aa->cd(1); f4->Draw();
    c1_aa->cd(2); f4->Draw();
    c1_aa->cd(3); f4->Draw();
    c1_aa->cd(4); f4->Draw();


    // legend for fit variation  
    TLegend *leg2a = new TLegend(0.65,0.6,0.8,0.9);
    leg2a->SetFillStyle(0);
    leg2a->SetFillColor(0);
    leg2a->SetBorderSize(0);
    leg2a->SetMargin(0.2);
    leg2a->SetTextSize(0.04);

    // legend for rest of variations
    TLegend *leg2b = new TLegend(0.7,0.65,0.8,0.9);
    leg2b->SetFillStyle(0);
    leg2b->SetFillColor(0);
    leg2b->SetBorderSize(0);
    leg2b->SetMargin(0.2);
    leg2b->SetTextSize(0.04);

    //------------------------------------------------------------
    TString hist_pr(Form("phPrp_%s",yieldHistNames[ih]));
    TString hist_npr(Form("phNPrp_%s",yieldHistNames[ih]));

    cout <<"histogram input name: "<< hist_pr<<"\t"<<hist_npr<<endl; 
  
    // prompt histos
    phCorr_pr_pp = (TH1F*)fYesWeighFile_pp->Get(hist_pr);
    phCorr_pr_aa = (TH1F*)fYesWeighFile_aa->Get(hist_pr);
  
    // non-prompt histos
    phCorr_npr_pp = (TH1F*)fYesWeighFile_pp->Get(hist_npr);
    phCorr_npr_aa = (TH1F*)fYesWeighFile_aa->Get(hist_npr);
  
    int numBins = 0;
    if(ih==0) numBins = nBinsPt; // high-pt bins
    if(ih==1 || ih==2) numBins = nBinsPt3;// mb pt fwd bins; 3-6.5, 6.5-30, 3-30
    if(ih==3) numBins = nBinsMB;// mb bin=1

    double relVar_pr_aa[numBins];
    double relVar_pr_pp[numBins];

    double relVar_npr_aa[numBins];
    double relVar_npr_pp[numBins];

    cout <<"###################################################################"<<endl;
    if(bDoDebug) cout << "################ Kinematic region: " <<yieldHistNames[ih]<<endl;
    // for each source of uncert, calc variation wrt nominal value
    for(int ivar=0; ivar<(nFitVariations+nEff4DVariations+nEffTnPVariation); ivar++)
    {
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
      
      for(int ibin=1; ibin<=numBins; ibin++)
      {
        //nominal prompt and non-prompt yield ratios
        const double yield_aa_pr  = phCorr_pr_aa->GetBinContent(ibin);
        const double yield_aa_npr = phCorr_npr_aa->GetBinContent(ibin);
      
        const double yield_pp_pr  = phCorr_pr_pp->GetBinContent(ibin);
        const double yield_pp_npr = phCorr_npr_pp->GetBinContent(ibin);
    
        if(bDoDebug) cout << "################ Bin " <<ibin << endl
        << "  Nominal yields are (prompt_aa & nonPr_aa ; prompt_pp & nonPr_pp): " 
        << yield_aa_pr<<"\t& "<<yield_aa_npr<<"\t; "<<yield_pp_pr<<"\t& "<<yield_pp_npr<<endl;
    
        double yieldVar_aa_pr  = phCorrVar_pr_aa->GetBinContent(ibin);
        double yieldVar_aa_npr = phCorrVar_npr_aa->GetBinContent(ibin);
    
        double yieldVar_pp_pr  = phCorrVar_pr_pp->GetBinContent(ibin);
        double yieldVar_pp_npr = phCorrVar_npr_pp->GetBinContent(ibin);
        
        if(bDoDebug) cout << "+++++++++++ Variation " << ivar << " yields are: "
        <<yieldVar_aa_pr<<"\t& "<<yieldVar_aa_npr<<"\t;  "<<yieldVar_pp_pr<<"\t& "<<yieldVar_pp_npr<<endl;
        
        double relVar_aa_pr = 100*( (yield_aa_pr - yieldVar_aa_pr)/yield_aa_pr );
        double relVar_pp_pr = 100*( (yield_pp_pr - yieldVar_pp_pr)/yield_pp_pr ); 
        
        double relVar_aa_npr = 100*( (yield_aa_npr - yieldVar_aa_npr)/yield_aa_npr );
        double relVar_pp_npr = 100*( (yield_pp_npr - yieldVar_pp_npr)/yield_pp_npr );

        if((ivar>=(nFitVariations+nEff4DVariations)) && (ivar < (nFitVariations+nEff4DVariations+nEffTnPVariation)))
        {
          // the TnP and 3D uncertainties, already come as relative uncertainties
          relVar_aa_pr = 100*yieldVar_aa_pr;
          relVar_pp_pr = 100*yieldVar_pp_pr; 

          relVar_aa_npr = 100*yieldVar_aa_npr;
          relVar_pp_npr = 100*yieldVar_pp_npr;

        }

        int bin = ibin-1;
        if (numBins==1) bin = 0;

        relVar_pr_aa[bin]  = relVar_aa_pr;
        relVar_pr_pp[bin]  = relVar_pp_pr;

        relVar_npr_aa[bin] = relVar_aa_npr;
        relVar_npr_pp[bin] = relVar_pp_npr;
      
      }//loop end: for(int ibin=1; ibin<=numBins; ibin++): 7 for highpt, 3 for low-pt, ...
      for(int ib=0; ib<numBins; ib++)
      {
        cout << "##### pp prompt relUncert.Vectors["<<ib<<"] = "<< relVar_pr_pp[ib]<<endl;
        cout << "##### aa prompt relUncert.Vectors["<<ib<<"] = "<< relVar_pr_aa[ib]<<endl;
        cout << "to be placed at this x: "<<binsPt[ib] <<endl;
      }
      // high-pt bins
      TGraph *gPrJpsiSyst_aa;
      TGraph *gNPrJpsiSyst_aa;
      TGraph *gPrJpsiSyst_pp;
      TGraph *gNPrJpsiSyst_pp;

      // low-pt bins
      if(ih==1 || ih==2) { // mb pt fwd bins; 3-6.5, 6.5-30, 3-30
        gPrJpsiSyst_aa  = new TGraph(numBins, binsPt3, relVar_pr_aa);
        gNPrJpsiSyst_aa = new TGraph(numBins, binsPt3, relVar_npr_aa);
        gPrJpsiSyst_pp  = new TGraph(numBins, binsPt3, relVar_pr_pp);
        gNPrJpsiSyst_pp = new TGraph(numBins, binsPt3, relVar_npr_pp); 
      } else if(ih==3) { //mb
        gPrJpsiSyst_aa  = new TGraph(numBins, binsPtMB, relVar_pr_aa);
        gNPrJpsiSyst_aa = new TGraph(numBins, binsPtMB, relVar_npr_aa);
        gPrJpsiSyst_pp  = new TGraph(numBins, binsPtMB, relVar_pr_pp);
        gNPrJpsiSyst_pp = new TGraph(numBins, binsPtMB, relVar_npr_pp); 
      } else {
        gPrJpsiSyst_aa  = new TGraph(numBins, binsPt, relVar_pr_aa);
        gNPrJpsiSyst_aa = new TGraph(numBins, binsPt, relVar_npr_aa);
        gPrJpsiSyst_pp  = new TGraph(numBins, binsPt, relVar_pr_pp);
        gNPrJpsiSyst_pp = new TGraph(numBins, binsPt, relVar_npr_pp);
      }

      //-------------------------------------------------------------------------------------------------------
      // set marker style
      gPrJpsiSyst_aa->SetMarkerStyle(20+ivar);
      gNPrJpsiSyst_aa->SetMarkerStyle(20+ivar);

      gPrJpsiSyst_pp->SetMarkerStyle(20+ivar);
      gNPrJpsiSyst_pp->SetMarkerStyle(20+ivar);

      gPrJpsiSyst_aa->SetMarkerSize(1.2);
      gNPrJpsiSyst_aa->SetMarkerSize(1.2);
      gPrJpsiSyst_pp->SetMarkerSize(1.2);
      gNPrJpsiSyst_pp->SetMarkerSize(1.2);

      // fill the pp Canvas
      if(ivar<nFitVariations)
      {  
        c1_pp->cd(1);
        leg2a->AddEntry(gPrJpsiSyst_pp,Form("%s",legendSyst[ivar]),"P");
        gPrJpsiSyst_pp->Draw("P");
        
        c1_pp->cd(3);
        gNPrJpsiSyst_pp->Draw("P");

        c1_aa->cd(1);
        gPrJpsiSyst_aa->Draw("P");

        c1_aa->cd(3);
        gNPrJpsiSyst_aa->Draw("P");
      }  
      else
      {
        c1_pp->cd(2);
        leg2b->AddEntry( gPrJpsiSyst_pp,Form("%s",legendSyst[ivar]),"P");
        gPrJpsiSyst_pp->Draw("P");
       
        c1_pp->cd(4);
        gNPrJpsiSyst_pp->Draw("P");

        c1_aa->cd(2);
        gPrJpsiSyst_aa->Draw("P");

        c1_aa->cd(4);
        gNPrJpsiSyst_aa->Draw("P");

      }
      c1_pp->Update();
      c1_aa->Update();
    }// fit variation ivar (fit, 4dEff, tnp)

    c1_pp->cd(1); lPr->Draw(); lcent->Draw(); ly->Draw(); leg2a->Draw(); c1_pp->Update();
    c1_aa->cd(1); lPr->Draw(); lcent->Draw(); ly->Draw(); leg2a->Draw(); c1_aa->Update();

    c1_pp->cd(2); leg2b->Draw(); c1_pp->Update();
    c1_aa->cd(2); leg2b->Draw(); c1_aa->Update();

    c1_pp->cd(3); lNpr->Draw(); c1_pp->Update();
    c1_aa->cd(3); lNpr->Draw(); c1_aa->Update();
  

    //-------------------------------- save all canvases for each ih kinematic case
    if(bSavePlots)
    {
      c1_pp->SaveAs(Form("%s/pdf/pp_relSystVar_%s.pdf",outputDir,yieldHistNames[ih]));
      c1_pp->SaveAs(Form("%s/png/pp_relSystVar_%s.png",outputDir,yieldHistNames[ih]));

      c1_aa->SaveAs(Form("%s/pdf/aa_relSystVar_%s.pdf",outputDir,yieldHistNames[ih]));
      c1_aa->SaveAs(Form("%s/png/aa_relSystVar_%s.png",outputDir,yieldHistNames[ih]));
    }

  }//loop end: for(int ih=0; ih<nInHist;ih++) for each kinematic range (high-pt, low=pt, mb, etC)
  
}
