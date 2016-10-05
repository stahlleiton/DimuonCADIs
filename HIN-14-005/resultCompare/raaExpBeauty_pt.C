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
#include "dats/data_others5tev.h"

#include "../CMS_lumi.C"
#include "../tdrstyle.C"
#include "../textPosition.h"

#endif
void raaExpBeauty_pt(const char* inputDir = "../macro_raa/outRoot", // the place where the input root files, with the histograms are
                     bool bSavePlots = false,
		     bool bDrawB     = true,
		     bool bDrawAtlas = false,
                     double xMax     = 50 
		     )
{
  // set the style
  gSystem->mkdir(Form("./figsHP/png"), kTRUE);
  gSystem->mkdir(Form("./figsHP/pdf"), kTRUE);
  setTDRStyle();
  
  // read CMS graphs
  TFile *pgRaaCms_pt   = new TFile(Form("%s/makeRaa_pt.root",inputDir));
  // ##################### HIGH PT ############################
  TGraphErrors *pgCms     = (TGraphErrors *)pgRaaCms_pt->Get("gNonPrJpsi");
  TGraphErrors *pgCmsP    = (TGraphErrors *)pgRaaCms_pt->Get("gNonPrJpsiP");
  TGraphErrors *pgCmsSyst = (TGraphErrors *)pgRaaCms_pt->Get("gNonPrJpsiSyst");
  pgCmsSyst->SetFillColorAlpha(kOrange-9,0.5);
  pgCms->SetName("gNonPrJpsi");

  TBox *lumi = (TBox*)pgRaaCms_pt->Get("lumi");
  lumi->SetFillColor(kOrange-9);
  lumi->SetFillStyle(1001);
  lumi->SetX1(xMax-0.75); lumi->SetX2(xMax);
  TBox *lumi_lowpt = (TBox*)lumi->Clone("lumi_lowpt");
  lumi_lowpt->SetFillColor(kViolet-9);
  lumi_lowpt->SetFillStyle(1001);
  lumi_lowpt->SetX1(xMax-1.5); lumi_lowpt->SetX2(xMax-0.75);

  // ##################### LOW PT ############################
  TGraphErrors *pgCms_lowpt     = (TGraphErrors *)pgRaaCms_pt->Get("gNonPrJpsi_pt365y1624");
  TGraphErrors *pgCmsP_lowpt    = (TGraphErrors *)pgRaaCms_pt->Get("gNonPrJpsiP_pt365y1624");
  TGraphErrors *pgCmsSyst_lowpt = (TGraphErrors *)pgRaaCms_pt->Get("gNonPrJpsiSyst_pt365y1624");
  pgCmsSyst_lowpt->SetFillColorAlpha(kViolet-9,0.5);
  pgCms_lowpt->SetName("gNonPrJpsi_pt365y1624");
  

  //---------------------------------------------------------------------
  // *********** CMS B@5TeV
  TFile *pfRaaCms_B5tev             = new TFile(Form("/afs/cern.ch/work/m/miheejo/public/HIN-14-005/plots/resultCompare/raa_B_5tev.root"));
  TH1 *phCmsB5Tev                   = (TH1D *)pfRaaCms_B5tev->Get("hNuclearModification");
  phCmsB5Tev->SetDirectory(0);
  TGraphAsymmErrors *pgCmsB5TeVSyst = (TGraphAsymmErrors *)pfRaaCms_B5tev->Get("gNuclearModification");
  pgCmsB5TeVSyst->SetFillColorAlpha(kBlue,0.2);
  phCmsB5Tev->SetMarkerStyle(21);
  phCmsB5Tev->SetMarkerSize(1.5);
  phCmsB5Tev->SetMarkerColor(kBlue);

  // 15% global uncertainty
  TBox *lumiB5TeV = new TBox(47.5,0.85,48.5,1.15);
  lumiB5TeV->SetFillColor(kBlue);
  
  //--------------------------------------------------------------------
  //=====================================================================
  // *********** ATLAS 5TeV nonprompt
  TGraphErrors *pgRaa_atlas     = new TGraphErrors(nPtBins_atlas, atlasRaa_pt, atlasRaa, atlasRaa_ptErr, atlasRaa_stat);
  TGraphErrors *pgRaaSyst_atlas = new TGraphErrors(nPtBins_atlas, atlasRaa_pt, atlasRaa, atlasRaa_ptErrSyst, atlasRaa_syst);
  TGraphErrors *pgRaaGlb_atlas  = new TGraphErrors(nPtBins_atlas, atlasRaa_pt, atlasRaa, atlasRaa_ptErrSyst, atlasRaa_glb);
  pgRaa_atlas->SetMarkerStyle(25);
  pgRaa_atlas->SetMarkerSize(1.3);
  pgRaa_atlas->SetMarkerColor(1);
 
  pgRaa_atlas->SetLineColor(kBlack);
  pgRaaSyst_atlas->SetFillStyle(0);
  pgRaaSyst_atlas->SetLineColor(1);
  pgRaaGlb_atlas->SetFillColorAlpha(kGray+1,0.5);
 
  //---------------------------------------------------------
  TLine *line = new TLine(0.,1,xMax,1);
  line->SetLineStyle(1);
  line->SetLineWidth(1);

  TCanvas *pc = new TCanvas("pc","pc");
  pc->cd();
  TF1 *f4 = new TF1("f4","1",0,xMax);
  f4->SetLineWidth(1);
  f4->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  if(bDrawB)f4->GetXaxis()->SetTitle("p_{T} or p_{T}^{B} (GeV/c)");
  f4->GetYaxis()->SetTitle("R_{AA}");
  f4->GetXaxis()->SetRangeUser(0.0,xMax);
  if(!bDrawB)f4->GetXaxis()->SetRangeUser(0.0,40);
  f4->GetYaxis()->SetRangeUser(0.0,1.5);
  f4->GetXaxis()->CenterTitle(kTRUE);
 
  f4->Draw();
  // CMS_lumi(pc,12014000,0);
  
  lumi->Draw();
  lumi_lowpt->Draw();
 
   if(bDrawB)
    {
      lumiB5TeV->Draw();
      pgCmsB5TeVSyst->Draw("2");
    }
   
  
  if(bDrawAtlas)
    {
      pgRaaGlb_atlas->Draw("2");
      pgRaaSyst_atlas->Draw("2");
      pgRaa_atlas->Draw("P");
    }
  if(bDrawB)
    {
      phCmsB5Tev->Draw("same");
    }
   
  pgCmsSyst->Draw("2");
  pgCmsP->Draw("P");
  pgCms->Draw("P");
  pgCmsSyst_lowpt->Draw("2");
  pgCmsP_lowpt->Draw("P");
  pgCms_lowpt->Draw("P");

 
  TLegend *leg_cent = new TLegend(0.2,0.65,0.4,0.89,NULL,"brNDC");
  if(!bDrawAtlas)leg_cent = new TLegend(0.2,0.73,0.4,0.89,NULL,"brNDC");
  leg_cent->SetMargin(0.12);
  leg_cent->SetBorderSize(0);
  leg_cent->SetTextFont(132);
  leg_cent->SetTextSize(0.03);
  leg_cent->SetLineColor(1);
  leg_cent->SetLineStyle(1);
  leg_cent->SetLineWidth(1);
  leg_cent->SetFillColor(19);
  leg_cent->SetFillStyle(0);

  float titleSize = 0.06;
  float subEntrySize = 0.025;
  TLegendEntry *entry_cent;
  entry_cent=leg_cent->AddEntry("raab","Nonprompt J/#psi","");
  entry_cent->SetTextFont(132);
  entry_cent->SetTextSize(titleSize);
  entry_cent=leg_cent->AddEntry("gNonPrJpsi_pt365y1624", "CMS: 2.76 TeV","");
  entry_cent->SetTextFont(22);
  entry_cent->SetTextSize(entrySize);
  entry_cent=leg_cent->AddEntry("gNonPrJpsi_pt365y1624", "1.6 < |y| < 2.4, 3 < p_{T} < 6.5 GeV/c","p");
  entry_cent->SetMarkerSize(1.1);
  entry_cent->SetTextFont(42);
  entry_cent->SetTextSize(subEntrySize);
  entry_cent=leg_cent->AddEntry("gNonPrJpsi", "|y| < 2.4, 6.5 < p_{T} < 30 GeV/c","p");
  entry_cent->SetMarkerSize(1.1);
  entry_cent->SetTextFont(42);
  entry_cent->SetTextSize(subEntrySize);
  if(bDrawAtlas)
    {
      //  entry_cent=leg_cent->AddEntry("pgRaa_atlas", " ","");
      entry_cent=leg_cent->AddEntry("pgRaa_atlas", "ATLAS Preliminary: 5 TeV","");
      entry_cent->SetTextFont(22);
      entry_cent->SetTextSize(entrySize);
      entry_cent=leg_cent->AddEntry("pgRaa_atlas", "|y| < 2, 9 < p_{T} < 40 GeV/c, Cent. 0-80%","p");
      entry_cent->SetTextFont(42);
      entry_cent->SetTextSize(subEntrySize);
      entry_cent->SetMarkerStyle(25);
      // entry_cent=leg_cent->AddEntry("pgRaa_atlas", "Cent. 0-80%","");
      // entry_cent->SetTextFont(42);
      // entry_cent->SetTextSize(subEntrySize);

    }
  TLegend *leg_b = new TLegend(0.58,0.77,0.9,0.89,NULL,"brNDC");
  leg_b->SetMargin(0.12);
  leg_b->SetBorderSize(0);
  leg_b->SetTextFont(132);
  leg_b->SetTextSize(0.03);
  leg_b->SetLineColor(1);
  leg_b->SetLineStyle(1);
  leg_b->SetLineWidth(1);
  leg_b->SetFillColor(19);
  leg_b->SetFillStyle(0);

  if (bDrawB) {
    TLegendEntry *entry_braa;
    entry_braa=leg_b->AddEntry("cms_b","B","");
    entry_braa->SetTextSize(titleSize);
    entry_braa->SetTextFont(132);
    entry_cent=leg_b->AddEntry("phCmsB5Tev", "CMS Preliminary: 5 TeV","");
    entry_cent->SetTextFont(22);
    entry_cent->SetTextSize(entrySize);
    entry_braa=leg_b->AddEntry("phCmsB5Tev","|y| < 2.4, 7 < p_{T} < 50 GeV/c","P");
    entry_braa->SetMarkerStyle(21);
    entry_braa->SetMarkerColor(kBlue);
    entry_braa->SetMarkerSize(1.2);
    entry_braa->SetTextSize(subEntrySize);
    entry_braa->SetTextFont(42);
  }
  TLatex *lat = new TLatex();
  lat->SetNDC();
  lat->SetTextFont(42);
  lat->SetTextSize(ltxSetTextSize2);
  lat->DrawLatex(0.2,0.15,"Cent. 0-100%");

  line->Draw();
  leg_cent->Draw();
  if (bDrawB) leg_b->Draw();
  gPad->RedrawAxis();

  if(bSavePlots)
  {
    pc->SaveAs(Form("figsHP/pdf/raaExpBeauty_pt_RaaB%d_RaaAtlas%d.pdf",bDrawB,bDrawAtlas));
    pc->SaveAs(Form("figsHP/png/raaExpBeauty_pt_RaaB%d_RaaAtlas%d.png",bDrawB,bDrawAtlas));
  }
}


