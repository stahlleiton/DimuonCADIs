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
void raaExpOpen_pt(const char* inputDir = "../macro_raa/outRoot", // the place where the input root files, with the histograms are
                   bool bSavePlots      = true,
                   bool bDrawCh         = true,
                   double xMax          = 30 // x-axis range limit (ALICE D has x-range maximum value at 36, CMS has it at 30)
                  )
{
  // set the style
  gSystem->mkdir(Form("./figs/png"), kTRUE);
  gSystem->mkdir(Form("./figs/pdf"), kTRUE);
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

  //-------------------------------------------------------------------- 
  // *********** CMS Charged particle RAA
  TGraphAsymmErrors *p8165_d5x1y1 = new TGraphAsymmErrors(p8165_d5x1y1_numpoints, p8165_d5x1y1_xval, p8165_d5x1y1_yval, p8165_d5x1y1_xerrminus, p8165_d5x1y1_xerrplus, p8165_d5x1y1_ystatminus, p8165_d5x1y1_ystatplus);
  TGraphAsymmErrors *p8165_d5x1y1_syst = new TGraphAsymmErrors(p8165_d5x1y1_numpoints, p8165_d5x1y1_xval, p8165_d5x1y1_yval, p8165_d5x1y1_xerrminus, p8165_d5x1y1_xerrplus, p8165_d5x1y1_yerrminus, p8165_d5x1y1_yerrplus);
  p8165_d5x1y1->SetName("cms_ch_raa");
  // X-axis errors will be discarded
  for (int i=0; i<p8165_d5x1y1_numpoints; i++) {
    p8165_d5x1y1->SetPointEXlow(i,0);
    p8165_d5x1y1->SetPointEXhigh(i,0);
    p8165_d5x1y1_syst->SetPointEXlow(i,0.5);
    p8165_d5x1y1_syst->SetPointEXhigh(i,0.5);
  }
   
  // *********** ALICE D RAA vs. pT
  TGraphAsymmErrors *p9059_d15x1y1 = new TGraphAsymmErrors(p9059_d15x1y1_numpoints, p9059_d15x1y1_xval, p9059_d15x1y1_yval, p9059_d15x1y1_xerrminus, p9059_d15x1y1_xerrplus, p9059_d15x1y1_yerrminus, p9059_d15x1y1_yerrplus);
  TGraphAsymmErrors *p9059_d15x1y1_syst = new TGraphAsymmErrors(p9059_d15x1y1_numpoints, p9059_d15x1y1_xval, p9059_d15x1y1_yval, p9059_d15x1y1_xerrminus, p9059_d15x1y1_xerrplus, p9059_d15x1y1_yerrminus, p9059_d15x1y1_yerrplus);
  p9059_d15x1y1->SetName("alice_d_raa");
  // X-axis errors will be discarded
  for (int i=0; i<p9059_d15x1y1_numpoints; i++) {
    p9059_d15x1y1->SetPointEXlow(i,0);
    p9059_d15x1y1->SetPointEXhigh(i,0);
    p9059_d15x1y1_syst->SetPointEXlow(i,0.5);
    p9059_d15x1y1_syst->SetPointEXhigh(i,0.5);
    if (xMax<=30 && (i+1)==p9059_d15x1y1_numpoints) {
      double x,y;
      p9059_d15x1y1->GetPoint(i,x,y);
      p9059_d15x1y1->SetPoint(i,x+5,y);
      p9059_d15x1y1_syst->GetPoint(i,x,y);
      p9059_d15x1y1_syst->SetPoint(i,x+5,y);
    }
  }

  // Style for graphs
  p9059_d15x1y1->SetMarkerStyle(kOpenSquare);
  p9059_d15x1y1->SetMarkerSize(1.3);
  p9059_d15x1y1->SetMarkerColor(kBlack);
  p9059_d15x1y1->SetLineColor(kBlack);
  p9059_d15x1y1_syst->SetFillColorAlpha(kGray+1,0.5);

  p8165_d5x1y1->SetMarkerStyle(20);
  p8165_d5x1y1->SetMarkerSize(1.3);
  p8165_d5x1y1->SetMarkerColor(kTeal+3);
  p8165_d5x1y1->SetLineColor(kTeal+4);
  p8165_d5x1y1_syst->SetFillColorAlpha(kTeal-1,0.5);



  //---------------------------------------------------------
  TLine *line = new TLine(0.,1,xMax,1);
  line->SetLineStyle(1);
  line->SetLineWidth(1);

  TCanvas *pc = new TCanvas("pc","pc");

  TF1 *f4 = new TF1("f4","1",0,xMax);
  f4->SetLineWidth(1);
  f4->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  f4->GetYaxis()->SetTitle("R_{AA}");
  f4->GetXaxis()->SetRangeUser(0.0,xMax);
  f4->GetYaxis()->SetRangeUser(0.0,1.5);
  f4->GetXaxis()->CenterTitle(kTRUE);
 
  f4->Draw();
  lumi->Draw();
  lumi_lowpt->Draw();
  pc->Update();

  pgCmsSyst->Draw("2"); // for drawing x-axis

  p9059_d15x1y1_syst->Draw("2");
  if (bDrawCh) p8165_d5x1y1_syst->Draw("2");
  p9059_d15x1y1->Draw("pz");
  if (bDrawCh) p8165_d5x1y1->Draw("pz");

  pgCmsSyst->Draw("2");
  pgCmsP->Draw("P");
  pgCms->Draw("P");
  pgCmsSyst_lowpt->Draw("2");
  pgCmsP_lowpt->Draw("P");
  pgCms_lowpt->Draw("P");

  // additional info
  CMS_lumi(pc,12003000,0);

  TLegend *leg_cent = new TLegend(0.28,0.77,0.86,0.89,NULL,"brNDC"); // at top center
  leg_cent->SetMargin(0.12);
  leg_cent->SetBorderSize(0);
  leg_cent->SetTextFont(132);
  leg_cent->SetTextSize(0.03);
  leg_cent->SetLineColor(1);
  leg_cent->SetLineStyle(1);
  leg_cent->SetLineWidth(1);
  leg_cent->SetFillColor(19);
  leg_cent->SetFillStyle(0);

  TLegendEntry *entry_cent;
  entry_cent=leg_cent->AddEntry("raab","Open beauty: Nonprompt J/#psi","");
  entry_cent->SetTextFont(132);
  entry_cent->SetTextSize(ltxSetTextSize3);
  entry_cent=leg_cent->AddEntry("gNonPrJpsi_pt365y1624", "1.6 < |y| < 2.4","p");
  entry_cent->SetTextFont(42);
  entry_cent->SetTextSize(entrySize);
  entry_cent=leg_cent->AddEntry("gNonPrJpsi", "|y| < 2.4","p");
  entry_cent->SetTextFont(42);
  entry_cent->SetTextSize(entrySize);

  TLegend *leg_ch = new TLegend(0.28,0.685,0.86,0.765,NULL,"brNDC");
  leg_ch->SetMargin(0.12);
  leg_ch->SetBorderSize(0);
  leg_ch->SetTextFont(132);
  leg_ch->SetTextSize(0.03);
  leg_ch->SetLineColor(1);
  leg_ch->SetLineStyle(1);
  leg_ch->SetLineWidth(1);
  leg_ch->SetFillColor(19);
  leg_ch->SetFillStyle(0);

  if (bDrawCh) {
    TLegendEntry *entry_ch;
    entry_ch=leg_ch->AddEntry("cms_ch","Charged hadron","");
    entry_ch->SetTextSize(ltxSetTextSize3);
    entry_ch->SetTextFont(132);
    entry_ch=leg_ch->AddEntry("cms_ch_raa","|#eta| < 1, Cent. 0-5%","p");
    entry_ch->SetTextSize(entrySize);
    entry_ch->SetTextFont(42);
  }
  
  TLegend *leg_alice;
  if (bDrawCh) {
    leg_alice = new TLegend(0.28,0.60,0.86,0.68,NULL,"brNDC");
  } else {
    leg_alice = new TLegend(0.28,0.685,0.86,0.765,NULL,"brNDC");
  }
  leg_alice->SetMargin(0.12);
  leg_alice->SetBorderSize(0);
  leg_alice->SetTextFont(132);
  leg_alice->SetTextSize(0.03);
  leg_alice->SetLineColor(1);
  leg_alice->SetLineStyle(1);
  leg_alice->SetLineWidth(1);
  leg_alice->SetFillColor(kWhite);
  leg_alice->SetFillStyle(1000);

  TLegendEntry *entry_alice;
  entry_alice=leg_alice->AddEntry("alice_d","Open charm: Prompt D (ALICE)","");
  entry_alice->SetTextSize(ltxSetTextSize3);
  entry_alice->SetTextFont(132);
  entry_alice=leg_alice->AddEntry("alice_d_raa","|y| < 0.5, Cent. 0-10%","p");
  entry_alice->SetTextSize(entrySize);
  entry_alice->SetTextFont(42);

  TLatex *lat = new TLatex();
  lat->SetNDC();
  lat->SetTextFont(42);
  lat->SetTextSize(ltxSetTextSize2);
  if (bDrawCh) lat->DrawLatex(0.63,0.53,"Cent. 0-100%");
  else lat->DrawLatex(0.63,0.58,"Cent. 0-100%");

  line->Draw();
  leg_cent->Draw();
  if (bDrawCh) leg_ch->Draw();
  leg_alice->Draw();
  gPad->RedrawAxis();

  if(bSavePlots)
  {
    pc->SaveAs(Form("figs/pdf/raaExpOpen_pt_RaaCh%d.pdf",bDrawCh));
    pc->SaveAs(Form("figs/png/raaExpOpen_pt_RaaCh%d.png",bDrawCh));
  }
}


