#if !defined(__CINT__) || defined(__MAKECINT__)
#include <iostream>
#include <fstream>

#include <sstream>
#include <string>
#include <vector>

#include <Riostream.h>
#include "TROOT.h"

#include "TCanvas.h"
#include "TColor.h"
#include "TF1.h"
#include "TFile.h"
#include "TH1.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TMath.h"
#include "TSystem.h"
#include "TStyle.h"

#include "dats/data_cms2016.h"
#include "dats/data_others2016.h"

#include "../CMS_lumi.C"
#include "../tdrstyle.C"

#endif

void v2Exp_pt(bool bSavePlots     = false, 
	      float rangeYAxis    = 0.5,
	      float rangeXAxis    = 20,
	      const char* inputDir      = "../macro_v2/outRoot", // the place where the input root files, with the histograms are
	      const char* figNamePrefix="v2Exp_pt")
{
  gSystem->mkdir(Form("./figs/png"), kTRUE);
  gSystem->mkdir(Form("./figs/pdf"), kTRUE);
  setTDRStyle();
  
 // read CMS graphs
  TFile *pfV2Cms_cent   = new TFile(Form("%s/Prp_v2_pt_plotter.root",inputDir));
  
  TGraphAsymmErrors *pgV2Low  = (TGraphAsymmErrors *)pfV2Cms_cent->Get("pgV2_low");
  TGraphErrors *pgV2LowSyst   = (TGraphErrors *)pfV2Cms_cent->Get("pgV2_low_sys");
  TGraphErrors *pgV2LowP      = (TGraphErrors *)pfV2Cms_cent->Get("pgV2_low_cont");

  TGraphAsymmErrors *pgV2High = (TGraphAsymmErrors *)pfV2Cms_cent->Get("pgV2");
  TGraphErrors *pgV2HighSyst  = (TGraphErrors *)pfV2Cms_cent->Get("pgV2_sys");
  TGraphErrors *pgV2HighP     = (TGraphErrors *)pfV2Cms_cent->Get("pgV2_cont");
  

  // -----------------------------------------------------------------------------------------
  // ----- charged hadrons
 TGraphErrors *gChar = new TGraphErrors(19, pTChar, v2Char, chxerr, v2CharSt);
 TGraphErrors *gChar2 = new TGraphErrors(19, pTChar, v2Char, chxerr, v2CharSt2);
 TGraphErrors *gCharSys = new TGraphErrors(19, pTChar, v2Char, chxerr2, v2CharSys);

 gChar->SetMarkerStyle(20);
 gChar->SetMarkerColor(kAzure+1);
 gChar->SetLineColor(kAzure+1);
 gChar->SetMarkerSize(1.3);
 gChar2->SetMarkerStyle(24);
 gChar2->SetMarkerColor(kBlue+2);
 gChar2->SetLineColor(kBlue+2);
 gChar2->SetMarkerSize(1.3);

 gCharSys->SetFillColor(kAzure+9);

  //----------- D from ALICE
  TGraphErrors *pgAlice          = new TGraphErrors(6, v2AliceX_pt, v2Alice_pt, v2AliceXl, v2AliceStat_pt);
  TGraphErrors *pgAlice2         = new TGraphErrors(6, v2AliceX_pt, v2Alice_pt, v2AliceXl, v2AliceStat_pt);
  TGraphErrors *pgAliceSys       = new TGraphErrors(6, v2AliceX_pt, v2Alice_pt, v2AliceSyst_pt, v2AliceSyst_pt);
  TGraphErrors *pgAliceSys2      = new TGraphErrors(6, v2AliceX_pt, v2Alice_pt, v2AliceXl2, v2AliceXl2);
  TGraphAsymmErrors *pgAliceSysB = new TGraphAsymmErrors(6, v2AliceX_pt, v2Alice_pt, v2AliceXl2, v2AliceXl2, v2AliceSystBLow_pt, v2AliceSystBHigh_pt);

 
  pgAlice->SetMarkerStyle(27);
  pgAlice->SetMarkerColor(kGray+2);
  pgAlice->SetLineColor(kGray+2);
  pgAlice->SetMarkerSize(1.8);

  pgAlice2->SetMarkerStyle(33);
  pgAlice2->SetMarkerColor(kWhite);
  pgAlice2->SetLineColor(kGray+2);
  pgAlice2->SetMarkerSize(1.8);
  
  pgAliceSys->SetFillStyle(0);
  pgAliceSys->SetMarkerStyle(27);
  pgAliceSys->SetMarkerColor(kGray+2);
  pgAliceSys->SetLineColor(kGray+2);
  pgAliceSys->SetMarkerSize(1.7);

  pgAliceSysB->SetFillColor(kGray);

  // drawing
 TLatex lx; 
 lx.SetNDC();
 lx.SetTextFont(42);
 lx.SetTextAlign(11);
  //------------------------------------------------------------------------
  // put everything on one plot
 TH1D *phAxis_v2 = new TH1D("phAxis_v2",";p_{T} GeV/c;v_{2}",1,0,rangeXAxis);  
 phAxis_v2->SetDirectory(0);
 phAxis_v2->GetXaxis()->CenterTitle(true);
 phAxis_v2->GetXaxis()->LabelsOption("h");
 phAxis_v2->GetYaxis()->SetRangeUser(0,rangeYAxis); 
 phAxis_v2->GetYaxis()->SetTitleOffset(1.25);

 TCanvas *pcCombi = new TCanvas("pcCombi","pcCombi");
 phAxis_v2->Draw();
 CMS_lumi(pcCombi,101,33);

 
 pgAliceSysB->Draw("2");
 pgAliceSys->Draw("2");
 pgAlice->Draw("pz");

 gCharSys->Draw("2");
 gChar->Draw("pz");
 gChar2->Draw("p");

 pgV2LowSyst->Draw("2");
 pgV2Low->Draw("PZ");
 pgV2LowP->Draw("P");

 pgV2HighSyst->Draw("2");
 pgV2High->Draw("PZ");
 pgV2HighP->Draw("P");
 
 
 // legends ----
 TLegend *pl = new TLegend(0.15, 0.6, 0.5, 0.9);
 pl->SetTextFont(42);
 pl->SetBorderSize(0);
 pl->SetTextSize(0.03);
 pl->SetLineColor(1);
 pl->SetLineStyle(1);
 pl->SetLineWidth(1);
 pl->SetFillColor(19);
 pl->SetFillStyle(0);

  pl->SetTextSize(0.03);
  pl->AddEntry(gChar,"h^{#pm}, |#eta|<0.8","PL");
  pl->AddEntry(pgV2Low,"Close charm: Prompt J/#psi","");
  pl->AddEntry(pgV2Low,"   1.6<|y|<2.4","PL");
  pl->AddEntry(pgV2High,"   |y|<2.4","PL");
  pl->AddEntry(pgAlice,"Open charm: D (ALICE)","P");
  pl->AddEntry(pgAlice,"   |y|<0.8, 30-50%","");
  pl->Draw("same");

  TLatex *lt1 = new TLatex();
  lt1->SetNDC();
  lt1->SetTextFont(42);
  lt1->SetTextSize(0.04);
  lt1->DrawLatex(0.7,0.77,"Cent. 10-60%");

 gPad->RedrawAxis();
      
 if(bSavePlots)
   {
     pcCombi->SaveAs(Form("%s.pdf",figNamePrefix));
     pcCombi->SaveAs(Form("%s.png",figNamePrefix));  
   }

  
  return;
}

