#if !defined(__CINT__) || defined(__MAKECINT__)
#include <Riostream.h>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "TROOT.h"
#include "TStyle.h"

#include "TAttMarker.h"
#include "TFile.h"
#include "TBox.h"
#include "TCanvas.h"
#include "TColor.h"
#include "TF1.h"
#include "TH1.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TLegendEntry.h"
#include "TLine.h"
#include "TSystem.h"


#include "dats/data_cms2016.h"
#include "dats/data_others2016.h"

#include "../CMS_lumi.C"
#include "../tdrstyle.C"
#include "../textPosition.h"

#endif
void raaExpOpen_cent(const char* inputDir = "../macro_raa/outRoot", // the place where the input root files, with the histograms are
                     bool bSavePlots      = true,
                     bool bDoSameYHighPt  = false
                     )
{
   // set the style
  gSystem->mkdir(Form("./figs/png"), kTRUE);
  gSystem->mkdir(Form("./figs/pdf"), kTRUE);
  setTDRStyle();
  
  // read CMS graphs
  TFile *pfRaaCms_cent   = new TFile(Form("%s/makeRaa_cent.root",inputDir));

  TGraphErrors *pgCms     = (TGraphErrors *)pfRaaCms_cent->Get("gNonPrJpsi");
  TGraphErrors *pgCmsP    = (TGraphErrors *)pfRaaCms_cent->Get("gNonPrJpsiP");
  TGraphErrors *pgCmsSyst = (TGraphErrors *)pfRaaCms_cent->Get("gNonPrJpsiSyst");
  TBox *lumi = (TBox*)pfRaaCms_cent->Get("lumi_npr_y024_pt6530");
  lumi->SetFillColor(kOrange-9);
  lumi->SetFillStyle(1001);
  pgCmsSyst->SetFillColorAlpha(kOrange-9,0.5);

  if(bDoSameYHighPt)
  {
    pgCms     = (TGraphErrors *)pfRaaCms_cent->Get("gNonPrJpsi_pt6530y012");
    pgCmsP    = (TGraphErrors *)pfRaaCms_cent->Get("gNonPrJpsiP_pt6530y012");
    pgCmsSyst = (TGraphErrors *)pfRaaCms_cent->Get("gNonPrJpsiSyst_pt6530y012");
    lumi      = (TBox*)pfRaaCms_cent->Get("lumi_npr_y012_pt6530");
    lumi->SetFillColor(kAzure-9);
    lumi->SetFillStyle(1001);
    pgCms->SetFillColorAlpha(kAzure-9,0.5);
    pgCmsSyst->SetFillColorAlpha(kAzure-9,0.5);

  }
  pgCms->SetName("pgCms");
  lumi->SetX1(375); lumi->SetX2(400);
   
  //-------------------------------------------------------------------- 
  // *********** alice points
  //  TGraphAsymmErrors(n,x,y,exl,exh,eyl,eyh);
  TGraphAsymmErrors *pgAlice_dpt816         = new TGraphAsymmErrors(nNpartBins_alice,
                                                                    npartBins_alice, raaAlice_cent_dpt816,
                                                                    npartBinsErr_alice, npartBinsErr_alice, 
                                                                    raaAliceStat_cent_dpt816,raaAliceStat_cent_dpt816);
  TGraphAsymmErrors *pgAliceSyst_dpt816     = new TGraphAsymmErrors(nNpartBins_alice,
                                                                    npartBins_alice, raaAlice_cent_dpt816, 
                                                                    npartSystX_alice, npartSystX_alice,
                                                                    raaAliceSystLow_cent_dpt816,raaAliceSystHigh_cent_dpt816);
  TGraphAsymmErrors *pgAliceSystCorr_dpt816 = new TGraphAsymmErrors(nNpartBins_alice,
                                                                    npartBins_alice, raaAlice_cent_dpt816,
                                                                    npartSystX_alice, npartSystX_alice,
                                                                    raaAliceSystCorrLow_cent_dpt816,raaAliceSystCorrHigh_cent_dpt816);
  pgAlice_dpt816->SetName("pgAlice_dpt816");
  
  pgAlice_dpt816->SetMarkerStyle(25);
  pgAlice_dpt816->SetMarkerSize(1.2);
  // systmatic boxes
  // uncorrelated, open
  pgAliceSyst_dpt816->SetLineColor(1);
  pgAliceSyst_dpt816->SetFillColor(0);
  pgAliceSyst_dpt816->SetFillStyle(0);

  // correlated, filled
  pgAliceSystCorr_dpt816->SetLineColor(1);
  pgAliceSystCorr_dpt816->SetFillColor(kGray);
  // pgAliceSystCorr_dpt816->SetFillStyle(0);

   //---------------------------------------------------------
  TLine *line = new TLine(0.,1,400,1);
  line->SetLineStyle(1);
  line->SetLineWidth(1);

  TCanvas *pc = new TCanvas("pc","pc");

  TF1 *f4 = new TF1("f4","1",0,400);
  f4->SetLineWidth(1);
  f4->GetXaxis()->SetTitle("N_{part}");
  f4->GetYaxis()->SetTitle("R_{AA}");
  f4->GetYaxis()->SetRangeUser(0.0,1.5);
  f4->GetXaxis()->CenterTitle(kTRUE);
 
  f4->Draw();
  lumi->Draw();
  pc->Update();

  pgAliceSystCorr_dpt816->Draw("2");
  pgCmsSyst->Draw("2");
  pgAliceSyst_dpt816->Draw("2");
  
  pgCmsP->Draw("P");
  pgCms->Draw("P");
  
  pgAlice_dpt816->Draw("P");

  // additional info
  CMS_lumi(pc,12014000,0);

  TLegend *leg_cent = new TLegend(0.2,0.78,0.7,0.88,NULL,"brNDC");
  leg_cent->SetBorderSize(0);
  leg_cent->SetTextFont(132);
  leg_cent->SetTextSize(0.03);
  leg_cent->SetLineColor(1);
  leg_cent->SetLineStyle(1);
  leg_cent->SetLineWidth(1);
  leg_cent->SetFillColor(19);
  leg_cent->SetFillStyle(0);

  TLegendEntry *entry_cent;
  entry_cent=leg_cent->AddEntry("pgCms","Open beauty: nonprompt J/#psi","");
  entry_cent->SetTextFont(132);
  entry_cent->SetTextSize(ltxSetTextSize3);

  if(bDoSameYHighPt)
    entry_cent=leg_cent->AddEntry("pgCms", "6.5 < p_{T} < 30 GeV/c, |y| < 1.2","p");
  else
    entry_cent=leg_cent->AddEntry("pgCms", "6.5 < p_{T} < 30 GeV/c, |y| < 2.4","p");
  
  entry_cent->SetTextFont(42);
  entry_cent->SetTextSize(entrySize);

  TLegend *leg_alice_cent = new TLegend(0.2,0.67,0.7,0.77,NULL,"brNDC");
  leg_alice_cent->SetBorderSize(0);
  leg_alice_cent->SetTextFont(132);
  leg_alice_cent->SetTextSize(0.03);
  leg_alice_cent->SetLineColor(1);
  leg_alice_cent->SetLineStyle(1);
  leg_alice_cent->SetLineWidth(1);
  leg_alice_cent->SetFillColor(19);
  leg_alice_cent->SetFillStyle(0);
  
  TLegendEntry *entry_alice_cent=leg_alice_cent->AddEntry("pgAlice_dpt816","Open charm: prompt D (ALICE)","");
  entry_alice_cent->SetTextFont(132);
  entry_alice_cent->SetTextSize(ltxSetTextSize3);
  entry_alice_cent=leg_alice_cent->AddEntry("pgAlice_dpt816","8 < p_{T} < 16 GeV/c, |y| < 0.5","p");
  entry_alice_cent->SetTextFont(42);
  entry_alice_cent->SetTextSize(entrySize);

  leg_cent->Draw();
  leg_alice_cent->Draw();
  line->Draw();
  gPad->RedrawAxis();
  if(bSavePlots)
  {
    pc->SaveAs(Form("figs/pdf/raaExpOpen_cent_sameY%d.pdf",bDoSameYHighPt));
    pc->SaveAs(Form("figs/png/raaExpOpen_cent_sameY%d.png",bDoSameYHighPt));
  }
}


