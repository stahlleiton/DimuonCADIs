#if !defined(__CINT__) || defined(__MAKECINT__)
#include <iostream>
#include <fstream>

#include <sstream>
#include <string>
#include <vector>

#include <Riostream.h>
#include "TROOT.h"

#include <Riostream.h>
#include <TSystem.h>
#include <TProfile.h>
#include <TBrowser.h>
#include <TROOT.h>
#include <TGraph.h>
#include <TNtuple.h>
#include <TString.h>
#include <TFile.h>
#include <TF1.h>
#include <TMath.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TLegendEntry.h>
#include <TStyle.h>
#include <TLatex.h>
#include <TInterpreter.h>
#include <TGraphAsymmErrors.h>
#include <TGraphErrors.h>

#include "dats/data_cms2016.h"
#include "dats/data_others2016.h"

#include "../CMS_lumi.C"
#include "../tdrstyle.C"

#endif

void v2ExpOpen_pt(bool bSavePlots = false, 
		  float rangeYAxis    = 0.6,
		  float rangeXAxis    = 20,
		  const char* inputDir= "../macro_v2/outRoot", // the place where the input root files, with the histograms are
		  const char* figNamePrefix="v2ExpOpen_pt")
{
  gSystem->mkdir(Form("./figs/png"), kTRUE);
  gSystem->mkdir(Form("./figs/pdf"), kTRUE);
  setTDRStyle();
  //  gStyle->SetCanvasPreferGL(1);
 // read CMS graphs
  TFile *pfV2Cms_cent   = new TFile(Form("%s/NPrp_v2_pt_plotter.root",inputDir));
  
  TGraphAsymmErrors *pgV2Low  = (TGraphAsymmErrors *)pfV2Cms_cent->Get("pgV2_low");
  TGraphErrors *pgV2LowSyst   = (TGraphErrors *)pfV2Cms_cent->Get("pgV2_low_sys");
  TGraphErrors *pgV2LowP      = (TGraphErrors *)pfV2Cms_cent->Get("pgV2_low_cont");

  TGraphAsymmErrors *pgV2High = (TGraphAsymmErrors *)pfV2Cms_cent->Get("pgV2");
  TGraphErrors *pgV2HighSyst  = (TGraphErrors *)pfV2Cms_cent->Get("pgV2_sys");
  TGraphErrors *pgV2HighP     = (TGraphErrors *)pfV2Cms_cent->Get("pgV2_cont");
  
  pgV2LowSyst->SetFillColorAlpha(kViolet-9,0.5);
  pgV2HighSyst->SetFillColorAlpha(kOrange-9,0.5);

  // -----------------------------------------------------------------------------------------
  // ----- charged hadrons
 TGraphErrors *gChar    = new TGraphErrors(19, pTChar, v2Char, chxerr, v2CharSt);
 TGraphErrors *gChar2   = new TGraphErrors(19, pTChar, v2Char, chxerr, v2CharSt2);
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
  TGraphErrors *pgAliceSys       = new TGraphErrors(6, v2AliceX_pt, v2Alice_pt, v2AliceXl, v2AliceSyst_pt);
  TGraphAsymmErrors *pgAliceSysB = new TGraphAsymmErrors(6, v2AliceX_pt, v2Alice_pt, v2AliceXl2, v2AliceXl2, v2AliceSystBLow_pt, v2AliceSystBHigh_pt);

 
  pgAlice->SetMarkerStyle(27);
  pgAlice->SetMarkerColor(kGray+2);
  pgAlice->SetLineColor(kGray+2);
  pgAlice->SetMarkerSize(1.8);
  
  pgAliceSys->SetFillStyle(0);
  pgAliceSys->SetMarkerStyle(27);
  pgAliceSys->SetMarkerColor(kGray+2);
  pgAliceSys->SetLineColor(kGray+2);
  pgAliceSys->SetMarkerSize(1.7);

  pgAliceSysB->SetFillColor(kGray);

  // drawing
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
 CMS_lumi(pcCombi,120015000,33);

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
 
  // --------- legends ----
 TLegend *leg = new TLegend(0.2,0.77,0.7,0.89,NULL,"brNDC");
 leg->SetBorderSize(0);
 leg->SetTextFont(62);
 leg->SetTextSize(0.03);
 leg->SetLineColor(1);
 leg->SetLineStyle(1);
 leg->SetLineWidth(1);
 leg->SetFillColor(19);
 leg->SetFillStyle(0);

 TLegendEntry *entry, *entry11;
 entry=leg->AddEntry("cmspr","Open beuty: non-prompt J/#psi (CMS)","");
 entry->SetTextSize(0.035);
 entry->SetFillStyle(1001);
 entry->SetLineColor(1);
 entry->SetLineStyle(1);
 entry->SetLineWidth(1);

 entry=leg->AddEntry("pgV2Low", "1.6<|y|<2.4","pf");
 entry->SetTextFont(42);
 entry->SetTextSize(0.03);
 entry->SetMarkerStyle(34);
 entry->SetMarkerColor(kViolet+2);
 entry->SetMarkerSize(1.7);
 entry->SetFillColor(kViolet-9);
 entry->SetFillStyle(1001);


 entry=leg->AddEntry("pgV2High", "|y|<2.4","Pf");
 entry->SetTextFont(42);
 entry->SetTextSize(0.03);
 entry->SetMarkerStyle(29);
 entry->SetMarkerColor(kOrange+2);
 entry->SetMarkerSize(1.7);
 entry->SetFillColor(kOrange-9);
 entry->SetFillStyle(1001);

 //--------------------------- 
 TLegend *leg_alice = new TLegend(0.2,0.68,0.7,0.77,NULL,"brNDC");
 leg_alice->SetBorderSize(0);
 leg_alice->SetTextFont(62);
 leg_alice->SetTextSize(0.03);
 leg_alice->SetLineColor(1);
 leg_alice->SetLineStyle(1);
 leg_alice->SetLineWidth(1);
 leg_alice->SetFillColor(19);
 leg_alice->SetFillStyle(0);
 TLegendEntry *entry_alice=leg_alice->AddEntry("pgAlice","Open charm: prompt D (ALICE)","");
 entry_alice->SetLineColor(1);
 entry_alice->SetLineStyle(1);
 entry_alice->SetLineWidth(1);
 entry_alice=leg_alice->AddEntry("pgAlice","|y|<0.8, Cent. 30-50\%","Pf");
 entry_alice->SetMarkerStyle(27);
 entry_alice->SetMarkerColor(kGray+2);
 entry_alice->SetMarkerSize(1.7);
 entry_alice->SetFillStyle(1001);
 entry_alice->SetTextFont(42);
 entry_alice->SetTextSize(0.03);


 TLegend *leg1 = new TLegend(0.2,0.61,0.7,0.68,NULL,"brNDC");
 leg1->SetBorderSize(0);
 leg1->SetTextFont(62);
 leg1->SetTextSize(0.03);
 leg1->SetLineColor(1);
 leg1->SetLineStyle(1);
 leg1->SetLineWidth(1);
 leg1->SetFillColor(19);
 leg1->SetFillStyle(0);
 TLegendEntry *entry1;
 entry1=leg1->AddEntry("hpm","Charged hadrons (CMS)","");
 entry1->SetLineColor(1);
 entry1->SetLineStyle(1);
 entry1->SetLineWidth(1);
 entry1->SetTextSize(0.035);
 entry1=leg1->AddEntry("gChar","|#eta|<0.8","Pf");
 entry1->SetMarkerStyle(20);
 entry1->SetMarkerColor(kAzure+2);
 entry1->SetMarkerSize(1.2);
 entry1->SetFillStyle(1001);
 entry1->SetFillColor(kAzure-9);
 entry1->SetTextFont(42);
 entry1->SetTextSize(0.03);


 leg->Draw();
 leg_alice->Draw();
 leg1->Draw();

 gPad->RedrawAxis();
 pcCombi->Update();
 if(bSavePlots)
   {
     pcCombi->SaveAs(Form("figs/pdf/%s.pdf",figNamePrefix));
     pcCombi->SaveAs(Form("figs/png/%s.png",figNamePrefix));  
   }

  
  return;
}

