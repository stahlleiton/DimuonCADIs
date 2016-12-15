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

#include "../textPosition.h"

#endif

void v2TheoryOpen_pt(bool bSavePlots = true, 
                  float rangeYAxis    = 0.27,
                  float rangeXAxis    = 30,
                  const char* inputDir= "../macro_v2/outRoot", // the place where the input root files, with the histograms are
                  const char* figNamePrefix="v2TheoryOpen_pt")
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

  pgV2Low->SetName("pgV2Low");
  pgV2LowSyst->SetFillColorAlpha(kViolet-9,0.5);
  pgV2High->SetName("pgV2High");
  pgV2HighSyst->SetFillColorAlpha(kOrange-9,0.5);

  // -----------------------------------------------------------------------------------------
  // -------- D, B, NP Jpsi from Pol-Bernand (K=0.8)
  TGraphErrors *gD_K08      = new TGraphErrors(v2PB_numpoints, v2PB_xval, v2PB_K08_D, 0, 0);
  TGraphErrors *gB_K08      = new TGraphErrors(v2PB_numpoints, v2PB_xval, v2PB_K08_B, 0, 0);
  TGraphErrors *gNPJpsi_K08 = new TGraphErrors(v2PB_numpoints, v2PB_xval, v2PB_K08_NPJpsi, 0, 0);

  gD_K08->SetName("gD_K08");
  gD_K08->SetLineStyle(2); //fine dotted line
  gD_K08->SetLineColor(kBlue-7);
  gD_K08->SetLineWidth(3);
  gB_K08->SetName("gB_K08");
  gB_K08->SetLineStyle(7); //dashed line (9)
  gB_K08->SetLineColor(kBlue-4);
  gB_K08->SetLineWidth(3);
  gNPJpsi_K08->SetName("gNPJpsi_K08");
  gNPJpsi_K08->SetLineStyle(1); //Solid line
  gNPJpsi_K08->SetLineColor(kBlue+1);
  gNPJpsi_K08->SetLineWidth(3);

  // -------- D, B, NP Jpsi from Pol-Bernand (K=1.5)
  TGraphErrors *gD_K15      = new TGraphErrors(v2PB_numpoints, v2PB_xval, v2PB_K15_D, 0, 0);
  TGraphErrors *gB_K15      = new TGraphErrors(v2PB_numpoints, v2PB_xval, v2PB_K15_B, 0, 0);
  TGraphErrors *gNPJpsi_K15 = new TGraphErrors(v2PB_numpoints, v2PB_xval, v2PB_K15_NPJpsi, 0, 0);

  gD_K15->SetName("gD_K15");
  gD_K15->SetLineStyle(2); //fine dotted line
  gD_K15->SetLineColor(kGreen-6);
  gD_K15->SetLineWidth(3);
  gB_K15->SetName("gB_K15");
  gB_K15->SetLineStyle(7); //dashed line (9)
  gB_K15->SetLineColor(kGreen+2);
  gB_K15->SetLineWidth(3);
  gNPJpsi_K15->SetName("gNPJpsi_K15");
  gNPJpsi_K15->SetLineStyle(1); //Solid line
  gNPJpsi_K15->SetLineColor(kTeal+3);
  gNPJpsi_K15->SetLineWidth(3);


  //------------------------------------------------------------------------
  // drawing
  // put everything on one plot
  TH1D *phAxis_v2 = new TH1D("phAxis_v2",";p_{T} (GeV/c);v_{2}",1,0,rangeXAxis);  
  phAxis_v2->SetDirectory(0);
  phAxis_v2->GetXaxis()->CenterTitle(true);
  phAxis_v2->GetXaxis()->LabelsOption("h");
  phAxis_v2->GetYaxis()->SetRangeUser(-0.01,rangeYAxis); 
  phAxis_v2->GetYaxis()->SetTitleOffset(1.25);
 
  TCanvas *pcCombi = new TCanvas("pcCombi","pcCombi");
  phAxis_v2->Draw();
  CMS_lumi(pcCombi,12001000,0);
 
  gNPJpsi_K08->Draw("l");
//  gD_K08->Draw("l");
  gB_K08->Draw("l");
  gNPJpsi_K15->Draw("l");
//  gD_K15->Draw("l");
  gB_K15->Draw("l");
 
  pgV2LowSyst->Draw("2");
  pgV2Low->Draw("PZ");
  pgV2LowP->Draw("P");
 
  pgV2HighSyst->Draw("2");
  pgV2High->Draw("PZ");
  pgV2HighP->Draw("P");
  
  // --------- legends ----
  TLegend *leg = new TLegend(0.2,0.77,0.7,0.89,NULL,"brNDC");
  leg->SetBorderSize(0);
  leg->SetMargin(0.2);
  leg->SetTextFont(132);
  leg->SetTextSize(ltxSetTextSize3);
  leg->SetLineColor(1);
  leg->SetLineStyle(1);
  leg->SetLineWidth(1);
  leg->SetFillColor(19);
  leg->SetFillStyle(0);
 
  TLegendEntry *entry, *entry11;
  entry=leg->AddEntry("cmspr","Open beauty: nonprompt J/#psi","");
  entry->SetTextFont(132);
  entry->SetTextSize(ltxSetTextSize3);
 
  entry=leg->AddEntry("pgV2Low", "1.6 < |y| < 2.4","p");
  entry->SetTextFont(42);
  entry->SetTextSize(entrySize);
 
  entry=leg->AddEntry("pgV2High", "|y| < 2.4","P");
  entry->SetTextFont(42);
  entry->SetTextSize(entrySize);
 
  //--------------------------- 
//  TLegend *leg_K = new TLegend(0.23,0.55,0.7,0.76,NULL,"brNDC"); // with 6 entries
  TLegend *leg_K = new TLegend(0.23,0.65,0.7,0.77,NULL,"brNDC"); //0.62
  leg_K->SetNColumns(2); // insertion order is top-left -> top-right -> bottom-left -> bottom-right
  leg_K->SetColumnSeparation(0.05);
  leg_K->SetMargin(0.32);
  leg_K->SetBorderSize(0);
//  leg_K->SetTextFont(132);
//  leg_K->SetTextSize(ltxSetTextSize3);
  leg_K->SetLineColor(1);
  leg_K->SetLineStyle(1);
  leg_K->SetLineWidth(1);
  leg_K->SetFillColor(19);
  leg_K->SetFillStyle(0);
  
  TLegendEntry *entry_K;
  entry_K=leg_K->AddEntry("K08K15","MC@sHQ + EPOS (Cent. 20-60%, |y| < 1)",""); 
  entry_K->SetTextFont(42);
  entry_K->SetTextSize(entrySize);//-0.005);
  entry_K=leg_K->AddEntry("K08K15","",""); // empty column for a alignment
//  entry_K=leg_K->AddEntry("gD_K08","D (K=0.8)","L");
//  entry_K->SetLineWidth(3);
//  entry_K->SetTextFont(42);
//  entry_K->SetTextSize(entrySize);
//  entry_K=leg_K->AddEntry("gD_K15","D (K=1.5)","L");
//  entry_K->SetLineWidth(3);
//  entry_K->SetTextFont(42);
//  entry_K->SetTextSize(entrySize);

  entry_K=leg_K->AddEntry("gB_K08","B (K=0.8)","L");
  entry_K->SetLineWidth(3);
  entry_K->SetTextFont(42);
  entry_K->SetTextSize(entrySize);//-0.005);
  entry_K=leg_K->AddEntry("gB_K15","B (K=1.5)","L");
  entry_K->SetLineWidth(3);
  entry_K->SetTextFont(42);
  entry_K->SetTextSize(entrySize);//-0.005);

  entry_K=leg_K->AddEntry("gNPJpsi_K08","NP J/#psi (K=0.8)","L");
  entry_K->SetLineWidth(3);
  entry_K->SetTextFont(42);
  entry_K->SetTextSize(entrySize);//-0.005);
  entry_K=leg_K->AddEntry("gNPJpsi_K15","NP J/#psi (K=1.5)","L");
  entry_K->SetLineWidth(3);
  entry_K->SetTextFont(42);
  entry_K->SetTextSize(entrySize);//-0.005);
 
  TLatex *lat = new TLatex();
  lat->SetNDC();
  lat->SetTextFont(42);
  lat->SetTextSize(ltxSetTextSize2);
  lat->DrawLatex(0.63,0.52,"Cent. 10-60%");

  leg->Draw();
  leg_K->Draw();
  gPad->RedrawAxis();
  pcCombi->Update();

  if(bSavePlots)
  {
    pcCombi->SaveAs(Form("figs/pdf/%s.pdf",figNamePrefix));
    pcCombi->SaveAs(Form("figs/png/%s.png",figNamePrefix));  
  }
  
  return;
}

