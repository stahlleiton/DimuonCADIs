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
void raaTheoryOpen_cent(const char* inputDir = "../macro_raa/outRoot", // the place where the input root files, with the histograms are
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
  // *********** Theory curves with filled area
  TGraph *pgRaaNPJpsi_TAMU       = new TGraph(2*raaNPJpsi_TAMU_numpoints);
  pgRaaNPJpsi_TAMU->SetName("pgRaaNPJpsi_TAMU");
  for (int i=0; i<raaNPJpsi_TAMU_numpoints; i++) {
    pgRaaNPJpsi_TAMU->SetPoint(i,raaNPJpsi_TAMU_npart[i],raaNPJpsi_TAMU_yhigh[i]);
    pgRaaNPJpsi_TAMU->SetPoint(raaNPJpsi_TAMU_numpoints+i,
        raaNPJpsi_TAMU_npart[raaNPJpsi_TAMU_numpoints-i-1],raaNPJpsi_TAMU_ylow[raaNPJpsi_TAMU_numpoints-i-1]);
  }
  TGraph *pgRaaNPJpsi_TAMU_withcmass       = new TGraph(2*raaNPJpsi_TAMU_numpoints_withcmass);
  pgRaaNPJpsi_TAMU_withcmass->SetName("pgRaaNPJpsi_TAMU_withcmass");
  for (int i=0; i<raaNPJpsi_TAMU_numpoints_withcmass; i++) {
    pgRaaNPJpsi_TAMU_withcmass->SetPoint(i,raaNPJpsi_TAMU_npart_withcmass[i],raaNPJpsi_TAMU_yhigh_withcmass[i]);
    pgRaaNPJpsi_TAMU_withcmass->SetPoint(raaNPJpsi_TAMU_numpoints_withcmass+i,
        raaNPJpsi_TAMU_npart_withcmass[raaNPJpsi_TAMU_numpoints_withcmass-i-1],raaNPJpsi_TAMU_ylow_withcmass[raaNPJpsi_TAMU_numpoints_withcmass-i-1]);
  }

  TGraph *pgRaaNPJpsi_Djou       = new TGraph(2*raaNPJpsi_Djou_numpoints);
  pgRaaNPJpsi_Djou->SetName("pgRaaNPJpsi_Djou");
  for (int i=0; i<raaNPJpsi_Djou_numpoints; i++) {
    pgRaaNPJpsi_Djou->SetPoint(i,raaNPJpsi_Djou_npart[i],raaNPJpsi_Djou_yhigh[i]);
    pgRaaNPJpsi_Djou->SetPoint(raaNPJpsi_Djou_numpoints+i,
        raaNPJpsi_Djou_npart[raaNPJpsi_Djou_numpoints-i-1],raaNPJpsi_Djou_ylow[raaNPJpsi_Djou_numpoints-i-1]);
  }
  TGraph *pgRaaNPJpsi_Djou_withcmass       = new TGraph(2*raaNPJpsi_Djou_numpoints_withcmass);
  pgRaaNPJpsi_Djou_withcmass->SetName("pgRaaNPJpsi_Djou_withcmass");
  for (int i=0; i<raaNPJpsi_Djou_numpoints_withcmass; i++) {
    pgRaaNPJpsi_Djou_withcmass->SetPoint(i,raaNPJpsi_Djou_npart_withcmass[i],raaNPJpsi_Djou_yhigh_withcmass[i]);
    pgRaaNPJpsi_Djou_withcmass->SetPoint(raaNPJpsi_Djou_numpoints_withcmass+i,
        raaNPJpsi_Djou_npart_withcmass[raaNPJpsi_Djou_numpoints_withcmass-i-1],raaNPJpsi_Djou_ylow_withcmass[raaNPJpsi_Djou_numpoints_withcmass-i-1]);
  }

  // *********** Theory curves with line
  TGraph *pgRaaNPJpsi_buzatti               = new TGraph(raaNPJpsi_buzatti_numpoints,raaNPJpsi_buzatti_npart,raaNPJpsi_buzatti);
  pgRaaNPJpsi_buzatti->SetName("pgRaaNPJpsi_buzatti");
  TGraph *pgRaaNPJpsi_mcatshq               = new TGraph(raaNPJpsi_mcatshq_numpoints,raaNPJpsi_mcatshq_npart,raaNPJpsi_mcatshq);
  pgRaaNPJpsi_mcatshq->SetName("pgRaaNPJpsi_mcatshq");
  TGraph *pgRaaNPJpsi_mcatshq_withcmass     = new TGraph(raaNPJpsi_mcatshq_numpoints,raaNPJpsi_mcatshq_npart,raaNPJpsi_mcatshq_withcmass);
  pgRaaNPJpsi_mcatshq_withcmass->SetName("pgRaaNPJpsi_mcatshq_withcmass");
  TGraph *pgRaaNPJpsi_mcatshq_rad           = new TGraph(raaNPJpsi_mcatshq_rad_numpoints,raaNPJpsi_mcatshq_rad_npart,raaNPJpsi_mcatshq_rad);
  pgRaaNPJpsi_mcatshq_rad->SetName("pgRaaNPJpsi_mcatshq_rad");
  TGraph *pgRaaNPJpsi_mcatshq_rad_withcmass = new TGraph(raaNPJpsi_mcatshq_rad_numpoints,raaNPJpsi_mcatshq_rad_npart,raaNPJpsi_mcatshq_rad_withcmass);
  pgRaaNPJpsi_mcatshq_rad_withcmass->SetName("pgRaaNPJpsi_mcatshq_rad_withcmass");

  // Style for filled graphs
  pgRaaNPJpsi_TAMU->SetFillColorAlpha(kRed+1,0.7);
  pgRaaNPJpsi_TAMU->SetFillStyle(1001);
  pgRaaNPJpsi_TAMU_withcmass->SetFillColor(kOrange+1);
  pgRaaNPJpsi_TAMU_withcmass->SetFillStyle(1001);

  pgRaaNPJpsi_Djou->SetFillColor(kViolet-9);
  pgRaaNPJpsi_Djou->SetFillStyle(1001);
  pgRaaNPJpsi_Djou_withcmass->SetFillColor(kViolet-8);
  pgRaaNPJpsi_Djou_withcmass->SetFillStyle(1001);

  // Style for line graphs
  pgRaaNPJpsi_buzatti->SetLineColor(kRed);
  pgRaaNPJpsi_mcatshq->SetLineColor(kTeal+4);
//  pgRaaNPJpsi_mcatshq_withcmass->SetLineColor(kViolet);
  pgRaaNPJpsi_mcatshq_rad->SetLineColor(kTeal+3);
//  pgRaaNPJpsi_mcatshq_rad_withcmass->SetLineColor(kViolet-6);
  
  pgRaaNPJpsi_buzatti->SetLineStyle(1);
  pgRaaNPJpsi_mcatshq->SetLineStyle(1);
  pgRaaNPJpsi_mcatshq_withcmass->SetLineStyle(7);
  pgRaaNPJpsi_mcatshq_rad->SetLineStyle(1);
  pgRaaNPJpsi_mcatshq_rad_withcmass->SetLineStyle(7);
  
  pgRaaNPJpsi_buzatti->SetLineWidth(4);
  pgRaaNPJpsi_mcatshq->SetLineWidth(4);
  pgRaaNPJpsi_mcatshq_withcmass->SetLineWidth(4);
  pgRaaNPJpsi_mcatshq_rad->SetLineWidth(4);
  pgRaaNPJpsi_mcatshq_rad_withcmass->SetLineWidth(4);


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
  
  pgCmsSyst->Draw("2"); // for drawing x-axis
  pgRaaNPJpsi_Djou->Draw("f");
//  pgRaaNPJpsi_Djou_withcmass->Draw("f");
  pgRaaNPJpsi_TAMU->Draw("f");
//  pgRaaNPJpsi_TAMU_withcmass->Draw("f");

  pgRaaNPJpsi_mcatshq->Draw("c");
//  pgRaaNPJpsi_mcatshq_withcmass->Draw("c");
  pgRaaNPJpsi_mcatshq_rad->Draw("c");
//  pgRaaNPJpsi_mcatshq_rad_withcmass->Draw("c");
//  pgRaaNPJpsi_buzatti->Draw("c");

  pgCmsSyst->Draw("2");
  pgCmsP->Draw("P");
  pgCms->Draw("P");

  // additional info
  CMS_lumi(pc,12014000,0);

  TLegend *leg_cent = new TLegend(0.23,0.81,0.83,0.89,NULL,"brNDC");
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
  entry_cent=leg_cent->AddEntry("raab","Nonprompt J/#psi","");
  entry_cent->SetTextFont(132);
  entry_cent->SetTextSize(ltxSetTextSize3);
  
  if(bDoSameYHighPt)
    entry_cent=leg_cent->AddEntry("pgCms", "6.5 < p_{T} < 30 GeV/c, |y| < 1.2","p");
  else
    entry_cent=leg_cent->AddEntry("pgCms", "6.5 < p_{T} < 30 GeV/c, |y| < 2.4","p");
  
  entry_cent->SetTextFont(42);
  entry_cent->SetTextSize(entrySize);

  TLegend *leg_theory_cent = new TLegend(0.23,0.665,0.83,0.795,NULL,"brNDC"); //this area is for 5 theory curves into 1 canvas
//  TLegend *leg_theory_cent = new TLegend(0.23,0.66,0.83,0.82,NULL,"brNDC"); //this area is for 6 theory curves into 1 canvas
//  TLegend *leg_theory_cent = new TLegend(0.23,0.62,0.83,0.82,NULL,"brNDC"); //this area is for all 8 theory curves into 1 canvas
  //TLegend *leg_theory_cent = new TLegend(0.2,0.67,0.83,0.81,NULL,"brNDC"); //this area is for 4 theory curves into 1 canvas
  leg_theory_cent->SetMargin(0.12);
  leg_theory_cent->SetBorderSize(0);
  leg_theory_cent->SetTextFont(132);
  leg_theory_cent->SetTextSize(0.028);
  leg_theory_cent->SetLineColor(1);
  leg_theory_cent->SetLineStyle(1);
  leg_theory_cent->SetLineWidth(1);
  leg_theory_cent->SetFillColor(10);
  leg_theory_cent->SetFillStyle(1001);
  
  TLegendEntry *entry_theory_cent;
//  entry_theory_cent=leg_theory_cent->AddEntry("pgRaaNPJpsi_buzatti","Buzatti: CUJET preliminary (y~0)","l");
//  entry_theory_cent->SetTextFont(42);
  entry_theory_cent=leg_theory_cent->AddEntry("pgRaaNPJpsi_mcatshq","MC@sHQ+EPOS: standard (0-100%, |y| < 1)","l");
  entry_theory_cent->SetTextFont(42);
//  entry_theory_cent=leg_theory_cent->AddEntry("pgRaaNPJpsi_mcatshq_withcmass","MC@sHQ+EPOS: c mass (|y| < 1)","l");
//  entry_theory_cent->SetTextFont(42);
  entry_theory_cent=leg_theory_cent->AddEntry("pgRaaNPJpsi_mcatshq_rad","MC@sHQ+EPOS+rad+LPM: standard (0-100%, |y| < 1)","l");
  entry_theory_cent->SetTextFont(42);
//  entry_theory_cent=leg_theory_cent->AddEntry("pgRaaNPJpsi_mcatshq_rad_withcmass","MC@sHQ+EPOS+rad+LPM: c mass (|y| < 1)","l");
//  entry_theory_cent->SetTextFont(42);
  entry_theory_cent=leg_theory_cent->AddEntry("pgRaaNPJpsi_TAMU","TAMU: standard","f");
  entry_theory_cent->SetTextFont(42);
//  entry_theory_cent=leg_theory_cent->AddEntry("pgRaaNPJpsi_TAMU_withcmass","TAMU: c mass","f");
//  entry_theory_cent->SetTextFont(42);
  entry_theory_cent=leg_theory_cent->AddEntry("pgRaaNPJpsi_Djou","Djordjevic et al: standard","f");
  entry_theory_cent->SetTextFont(42);
//  entry_theory_cent=leg_theory_cent->AddEntry("pgRaaNPJpsi_Djou_withcmass","Djordjevic et al: c mass","f");
//  entry_theory_cent->SetTextFont(42);

  line->Draw();
  leg_cent->Draw();
  leg_theory_cent->Draw();
  gPad->RedrawAxis();
  if(bSavePlots)
  {
    pc->SaveAs(Form("figs/pdf/raaTheoryOpen_cent_sameY%d.pdf",bDoSameYHighPt));
    pc->SaveAs(Form("figs/png/raaTheoryOpen_cent_sameY%d.png",bDoSameYHighPt));
  }
}


