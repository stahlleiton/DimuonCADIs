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
#include "TLegendEntry.h"
#include "TMath.h"
#include "TSystem.h"
#include "TStyle.h"

#include "dats/data_cms2016.h"
#include "dats/data_others2016.h"

#include "../CMS_lumi.C"
#include "../tdrstyle.C"
#include "../textPosition.h"

#endif

void v2ExpClose_pt(bool bSavePlots     = true, 
	      float rangeYAxis    = 0.6,
	      float rangeXAxis    = 20,
              bool  bDrawCh       = true,
	      const char* inputDir      = "../macro_v2/outRoot", // the place where the input root files, with the histograms are
	      const char* figNamePrefix="v2ExpClose_pt")
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

  pgV2Low->SetName("pgV2Low");
  pgV2LowSyst->SetFillColorAlpha(kViolet-9,0.5);
  pgV2High->SetName("pgV2High");
  pgV2HighSyst->SetFillColorAlpha(kRed-9,0.5);

  // -----------------------------------------------------------------------------------------
  // ----- charged hadrons
  TGraphErrors *gChar    = new TGraphErrors(19, pTChar, v2Char, chxerr, v2CharSt);
  TGraphErrors *gChar2   = new TGraphErrors(19, pTChar, v2Char, chxerr, v2CharSt2);
  TGraphErrors *gCharSys = new TGraphErrors(19, pTChar, v2Char, chxerr2, v2CharSys);
 
  gChar->SetName("gChar");
  gChar->SetMarkerStyle(20);
  gChar->SetMarkerColor(kTeal+3);
  gChar->SetLineColor(kTeal+3);
  gChar->SetMarkerSize(1.3);
  gChar2->SetMarkerStyle(24);
  gChar2->SetMarkerColor(kTeal+4);
  gChar2->SetLineColor(kTeal+4);
  gChar2->SetMarkerSize(1.3);
 
  gCharSys->SetFillColor(kTeal-9);
 
  //----------- D from ALICE
  TGraphErrors *pgAlice          = new TGraphErrors(6, v2AliceX_pt, v2Alice_pt, v2AliceXl, v2AliceStat_pt);
  TGraphErrors *pgAliceSys       = new TGraphErrors(6, v2AliceX_pt, v2Alice_pt, v2AliceXl, v2AliceSyst_pt);
  TGraphAsymmErrors *pgAliceSysB = new TGraphAsymmErrors(6, v2AliceX_pt, v2Alice_pt, v2AliceXl2, v2AliceXl2, v2AliceSystBLow_pt, v2AliceSystBHigh_pt);

 
  pgAlice->SetName("pgAlice");
  pgAlice->SetMarkerStyle(kOpenSquare);
  pgAlice->SetMarkerColor(kGray+2);
  pgAlice->SetLineColor(kGray+2);
  pgAlice->SetMarkerSize(1.0);
  
  pgAliceSys->SetFillStyle(0);
  pgAliceSys->SetMarkerColor(kGray+2);
  pgAliceSys->SetLineColor(kGray+2);
  pgAliceSys->SetMarkerSize(1.7);

  pgAliceSysB->SetFillColor(kGray);

  // drawing
  //------------------------------------------------------------------------
  // put everything on one plot
  TH1D *phAxis_v2 = new TH1D("phAxis_v2",";p_{T} (GeV/c);v_{2}",1,0,rangeXAxis);  
  phAxis_v2->SetDirectory(0);
  phAxis_v2->GetXaxis()->CenterTitle(true);
  phAxis_v2->GetXaxis()->LabelsOption("h");
  phAxis_v2->GetYaxis()->SetRangeUser(0,rangeYAxis); 
  phAxis_v2->GetYaxis()->SetTitleOffset(1.25);
 
  TCanvas *pcCombi = new TCanvas("pcCombi","pcCombi");
  phAxis_v2->Draw();
  CMS_lumi(pcCombi,12014000,0);
 
  pgAliceSysB->Draw("2");
  pgAliceSys->Draw("2");
  pgAlice->Draw("pz");
 
  if (bDrawCh) {
    gCharSys->Draw("2");
    gChar->Draw("pz");
    gChar2->Draw("p");
  }
 
  pgV2LowSyst->Draw("2");
  pgV2Low->Draw("PZ");
  pgV2LowP->Draw("P");
 
  pgV2HighSyst->Draw("2");
  pgV2High->Draw("PZ");
  pgV2HighP->Draw("P");
  
  // --------- legends ----
  TLegend *leg = new TLegend(0.2,0.77,0.7,0.89,NULL,"brNDC"); // at top center
  leg->SetBorderSize(0);
  leg->SetTextFont(132);
  leg->SetTextSize(0.03);
  leg->SetLineColor(1);
  leg->SetLineStyle(1);
  leg->SetLineWidth(1);
  leg->SetFillColor(19);
  leg->SetFillStyle(0);
 
  TLegendEntry *entry, *entry11;
  entry=leg->AddEntry("cmspr","Hidden charm: prompt J/#psi","");
  entry->SetTextFont(132);
  entry->SetTextSize(ltxSetTextSize3);
 
  entry=leg->AddEntry("pgV2Low", "1.6 < |y| < 2.4","p");
  entry->SetTextFont(42);
  entry->SetTextSize(entrySize);
 
  entry=leg->AddEntry("pgV2High", "|y| < 2.4","P");
  entry->SetTextFont(42);
  entry->SetTextSize(entrySize);
 
  TLegend *leg1 = new TLegend(0.2,0.685,0.7,0.765,NULL,"brNDC");
  leg1->SetBorderSize(0);
  leg1->SetTextFont(132);
  leg1->SetTextSize(ltxSetTextSize3);
 
  if (bDrawCh) {
    TLegendEntry *entry1;
    entry1=leg1->AddEntry("hpm","Charged hadron","");
    entry1->SetTextFont(132);
    entry1->SetTextSize(ltxSetTextSize3);
    entry1=leg1->AddEntry("gChar","|#eta| < 0.8","P");
    entry1->SetTextFont(42);
    entry1->SetTextSize(entrySize);
  }
 
  //--------------------------- 
  TLegend *leg_alice;
  if (bDrawCh) {
    leg_alice = new TLegend(0.2,0.60,0.7,0.68,NULL,"brNDC");
  } else {
    leg_alice = new TLegend(0.2,0.685,0.7,0.765,NULL,"brNDC");
  }
  leg_alice->SetBorderSize(0);
  leg_alice->SetTextFont(132);
  leg_alice->SetLineColor(1);
  leg_alice->SetLineStyle(1);
  leg_alice->SetLineWidth(1);
  leg_alice->SetFillColor(19);
  leg_alice->SetFillStyle(0);
 
  TLegendEntry *entry_alice=leg_alice->AddEntry("pgAlice","Open charm: prompt D (ALICE)","");
  entry_alice->SetTextFont(132);
  entry_alice->SetTextSize(ltxSetTextSize3);
  entry_alice=leg_alice->AddEntry("pgAlice","|y| < 0.8, Cent. 30-50\%","P");
  entry_alice->SetTextFont(42);
  entry_alice->SetTextSize(entrySize);
 
  TLatex *lat = new TLatex();
  lat->SetNDC();
  lat->SetTextFont(42);
  lat->SetTextSize(ltxSetTextSize2);
  if (bDrawCh) lat->DrawLatex(0.63,0.52,"Cent. 10-60%");
  else lat->DrawLatex(0.63,0.58,"Cent. 10-60%");
 
  leg->Draw();
  if (bDrawCh) leg1->Draw();
  leg_alice->Draw();
 
  gPad->RedrawAxis();
       
  if(bSavePlots)
  {
    pcCombi->SaveAs(Form("figs/pdf/%s_RaaCh%d.pdf",figNamePrefix,bDrawCh));
    pcCombi->SaveAs(Form("figs/png/%s_RaaCh%d.png",figNamePrefix,bDrawCh));
  }
 
   
  return;
}

