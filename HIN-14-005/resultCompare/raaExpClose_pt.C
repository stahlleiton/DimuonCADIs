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
void raaExpClose_pt(const char* inputDir = "../macro_raa/outRoot", // the place where the input root files, with the histograms are
                          bool bSavePlots   = true,
                          bool bDoLowPtForw = true
                          )
{
   // set the style
  gSystem->mkdir(Form("./figs/png"), kTRUE);
  gSystem->mkdir(Form("./figs/pdf"), kTRUE);
  setTDRStyle();
  // gStyle->SetCanvasPreferGL(1);
  
  // read CMS graphs
  TFile *pfRaaCms_pt   = new TFile(Form("%s/makeRaa_pt.root",inputDir));
  // ##################### HIGH PT ############################
  TGraphErrors *pgCms     = (TGraphErrors *)pfRaaCms_pt->Get("gPrJpsi");
  TGraphErrors *pgCmsP    = (TGraphErrors *)pfRaaCms_pt->Get("gPrJpsiP");
  TGraphErrors *pgCmsSyst = (TGraphErrors *)pfRaaCms_pt->Get("gPrJpsiSyst");
  pgCms->SetFillColorAlpha(kRed-9,0.5);
  pgCms->SetName("gPrJpsi");
  pgCmsSyst->SetFillColorAlpha(kRed-9,0.5);
  
  TBox *lumi = (TBox*)pfRaaCms_pt->Get("lumi");
  lumi->SetFillColor(kRed-9);
  lumi->SetFillStyle(1001);
  lumi->SetX1(29.25); lumi->SetX2(30);
  TBox *lumi_lowpt = (TBox*)lumi->Clone("lumi_lowpt");
  lumi_lowpt->SetFillColor(kViolet-9);
  lumi_lowpt->SetFillStyle(1001);
  lumi_lowpt->SetX1(28.5); lumi_lowpt->SetX2(29.25);

  // ##################### LOW PT ############################
  TGraphErrors *pgCms_lowpt     = (TGraphErrors *)pfRaaCms_pt->Get("gPrJpsi_pt365y1624");
  TGraphErrors *pgCmsP_lowpt    = (TGraphErrors *)pfRaaCms_pt->Get("gPrJpsiP_pt365y1624");
  TGraphErrors *pgCmsSyst_lowpt = (TGraphErrors *)pfRaaCms_pt->Get("gPrJpsiSyst_pt365y1624");
  pgCms_lowpt->SetFillColorAlpha(kViolet-9,0.5);
  pgCms_lowpt->SetName("gPrJpsi_pt365y1624");
  pgCmsSyst_lowpt->SetFillColorAlpha(kViolet-9,0.5);

  //-------------------------------------------------------------------- 
  // *********** alice points:
  //  TGraphAsymmErrors(n,x,y,exl,exh,eyl,eyh);
  TGraphAsymmErrors *pgRaaAlice_cent090    = new TGraphAsymmErrors(raaJpsiAliceCent090_numpoints,
                                                                   raaJpsiAliceCent090_xval, raaJpsiAliceCent090_yval,
                                                                   raaJpsiAliceCent090_xerrminus, raaJpsiAliceCent090_xerrminus, 
                                                                   raaJpsiAliceCent090_yerrminus,raaJpsiAliceCent090_yerrminus);
  TGraphAsymmErrors *pgRaaAliceStat_cent090    = new TGraphAsymmErrors(raaJpsiAliceCent090_numpoints,
                                                                   raaJpsiAliceCent090_xval, raaJpsiAliceCent090_yval,
                                                                   raaJpsiAliceCent090_xerrminus, raaJpsiAliceCent090_xerrminus, 
                                                                   raaJpsiAliceCent090_ystatminus,raaJpsiAliceCent090_ystatminus);
  pgRaaAlice_cent090->SetName("pgRaaAlice_cent090");
  pgRaaAlice_cent090->SetMarkerStyle(20);
  pgRaaAlice_cent090->SetMarkerSize(1.2);
  pgRaaAlice_cent090->SetFillColor(kGray);

  // systmatic boxes
  // correlated, open
  pgRaaAliceStat_cent090->SetMarkerStyle(20);
  pgRaaAliceStat_cent090->SetMarkerSize(1.2);
  pgRaaAliceStat_cent090->SetLineColor(1);
  pgRaaAliceStat_cent090->SetFillColor(kGray);
//  pgRaaAliceStat_cent090->SetFillStyle(0);

  //---------------------------------------------------------
  TLine *line = new TLine(0.,1,30,1);
  line->SetLineStyle(1);
  line->SetLineWidth(1);

  TCanvas *pc = new TCanvas("pc","pc");

  TF1 *f4 = new TF1("f4","-1",0,30);
  //  f4->SetLineWidth(1);
  f4->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  f4->GetYaxis()->SetTitle("R_{AA}");
  f4->GetYaxis()->SetRangeUser(0.0,1.5);
  f4->GetXaxis()->CenterTitle(kTRUE);

  f4->Draw();
  lumi->Draw();
  lumi_lowpt->Draw();
  pc->Update();

  pgRaaAlice_cent090->Draw("2");
  pgCmsSyst->Draw("2");
  pgCmsP->Draw("P");
  pgCms->Draw("P");
  if(bDoLowPtForw)
  {
    pgCmsSyst_lowpt->Draw("2");
    pgCmsP_lowpt->Draw("P");
    pgCms_lowpt->Draw("P");
  }
  
  pgRaaAliceStat_cent090->Draw("P");

  // additional info
  CMS_lumi(pc,12003000,0);

  TLegend *leg_pt = new TLegend(0.2,0.77,0.7,0.88,NULL,"brNDC");
  leg_pt->SetBorderSize(0);
  leg_pt->SetTextFont(132);
  leg_pt->SetTextSize(ltxSetTextSize3);
  leg_pt->SetLineColor(1);
  leg_pt->SetLineStyle(1);
  leg_pt->SetLineWidth(1);
  leg_pt->SetFillColor(19);
  leg_pt->SetFillStyle(0);

  TLegendEntry *entry_pt;
  entry_pt=leg_pt->AddEntry("raab","Prompt J/#psi","");
  entry_pt->SetTextFont(132);
  entry_pt->SetTextSize(ltxSetTextSize3);
  if(bDoLowPtForw)
  {
    entry_pt=leg_pt->AddEntry("gPrJpsi_pt365y1624", "1.6 < |y| < 2.4","p");
    entry_pt->SetTextFont(42);
    entry_pt->SetTextSize(entrySize);
  }
  entry_pt=leg_pt->AddEntry("gPrJpsi", "|y| < 2.4","p");
  entry_pt->SetTextFont(42);
  entry_pt->SetTextSize(entrySize);

  TLegend *leg_alice_pt = new TLegend(0.2,0.68,0.7,0.75,NULL,"brNDC");
  leg_alice_pt->SetBorderSize(0);
  leg_alice_pt->SetTextFont(132);
  leg_alice_pt->SetTextSize(ltxSetTextSize3);
  leg_alice_pt->SetLineColor(1);
  leg_alice_pt->SetLineStyle(1);
  leg_alice_pt->SetLineWidth(1);
  leg_alice_pt->SetFillColor(19);
  leg_alice_pt->SetFillStyle(0);

  TLegendEntry *entry_alice_pt=leg_alice_pt->AddEntry("pgRaaAlice_cent090","Inclusive J/#psi (ALICE)","");
  entry_alice_pt->SetTextFont(132);
  entry_alice_pt->SetTextSize(ltxSetTextSize3);

  entry_alice_pt=leg_alice_pt->AddEntry("pgRaaAlice_cent090","2.5 < y < 4, Cent. 0-90%","p");
  entry_alice_pt->SetTextFont(42);
  entry_alice_pt->SetTextSize(entrySize);

  TLatex *lat = new TLatex();
  lat->SetNDC();
  lat->SetTextFont(42);
  lat->SetTextSize(ltxSetTextSize2);
  lat->DrawLatex(0.65,0.82,"Cent. 0-100%");

  leg_pt->Draw();
  leg_alice_pt->Draw();
  line->Draw();
  gPad->RedrawAxis();

  if(bSavePlots)
  {
    pc->SaveAs(Form("figs/pdf/raaExpClose_pt.pdf"));
    pc->SaveAs(Form("figs/png/raaExpClose_pt.png"));
  }
  
}


