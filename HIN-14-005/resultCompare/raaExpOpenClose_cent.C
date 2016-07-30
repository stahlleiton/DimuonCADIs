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

#endif
void raaExpOpenClose_cent(const char* inputDir      = "../macro_raa/outRoot", // the place where the input root files, with the histograms are
			  bool bSavePlots = true
			  )
{
   // set the style
  gSystem->mkdir(Form("./figs/png"), kTRUE);
  gSystem->mkdir(Form("./figs/pdf"), kTRUE);
  setTDRStyle();
  // gStyle->SetCanvasPreferGL(1);
  
  // read CMS graphs
  TFile *pfRaaCms_cent   = new TFile(Form("%s/makeRaa_cent.root",inputDir));

  // ##################### HIGH PT ############################
  
  TGraphErrors *pgCms     = (TGraphErrors *)pfRaaCms_cent->Get("gPrJpsi");
  TGraphErrors *pgCmsP    = (TGraphErrors *)pfRaaCms_cent->Get("gPrJpsiP");
  TGraphErrors *pgCmsSyst = (TGraphErrors *)pfRaaCms_cent->Get("gPrJpsiSyst");
  TBox *lumi = (TBox*)pfRaaCms_cent->Get("lumi_pr_y024_pt6530");
  lumi->SetFillColor(kRed-9);


  //-------------------------------------------------------------------- 
  // *********** alice points:
  //  TGraphAsymmErrors(n,x,y,exl,exh,eyl,eyh);
  TGraphAsymmErrors *pgAlice_dpt612         = new TGraphAsymmErrors(nNpartBins5_alice,
								    npartBins5_alice, raaAlice_cent_dpt612,
								    npartBinsErr5_alice, npartBinsErr5_alice, 
								    raaAliceStat_cent_dpt612,raaAliceStat_cent_dpt612);
  TGraphAsymmErrors *pgAliceSyst_dpt612     = new TGraphAsymmErrors(nNpartBins5_alice,
								    npartBins5_alice, raaAlice_cent_dpt612, 
								    npartSystX5_alice, npartSystX5_alice,
								    raaAliceSystLow_cent_dpt612,raaAliceSystHigh_cent_dpt612);
  TGraphAsymmErrors *pgAliceSystCorr_dpt612 = new TGraphAsymmErrors(nNpartBins5_alice,
								    npartBins5_alice, raaAlice_cent_dpt612,
								    npartSystX5_alice, npartSystX5_alice,
								    raaAliceSystCorrLow_cent_dpt612,raaAliceSystCorrHigh_cent_dpt612);
  
  pgAlice_dpt612->SetMarkerStyle(25);
  pgAlice_dpt612->SetMarkerSize(1.2);
  // systmatic boxes
  // uncorrelated, open
  pgAliceSyst_dpt612->SetLineColor(1);
  pgAliceSyst_dpt612->SetFillColor(0);
  pgAliceSyst_dpt612->SetFillStyle(0);

  // correlated, filled
  pgAliceSystCorr_dpt612->SetLineColor(1);
  pgAliceSystCorr_dpt612->SetFillColor(kGray);
  // pgAliceSystCorr_dpt816->SetFillStyle(0);

   //---------------------------------------------------------
  TLine *line = new TLine(0.,1,400,1);
  line->SetLineStyle(1);
  line->SetLineWidth(1);

  TCanvas *pc = new TCanvas("pc","pc");

  TF1 *f4 = new TF1("f4","",0,400);
  //  f4->SetLineWidth(1);
  f4->GetXaxis()->SetTitle("N_{part}");
  f4->GetYaxis()->SetTitle("R_{AA}");
  f4->GetYaxis()->SetRangeUser(0.0,1.5);
  f4->GetXaxis()->CenterTitle(kTRUE);

 
  f4->Draw();
  lumi->Draw();
  pc->Update();

  pgAliceSystCorr_dpt612->Draw("2");
  pgCmsSyst->Draw("2");
  pgAliceSyst_dpt612->Draw("2");

  
  pgCmsP->Draw("P");
  pgCms->Draw("P");
  
  pgAlice_dpt612->Draw("P");

  // additional info
  CMS_lumi(pc,14005000,33);

  TLegend *leg_cent = new TLegend(0.2,0.78,0.7,0.88,NULL,"brNDC");
  leg_cent->SetBorderSize(0);
  leg_cent->SetTextFont(62);
  leg_cent->SetTextSize(0.03);
  leg_cent->SetLineColor(1);
  leg_cent->SetLineStyle(1);
  leg_cent->SetLineWidth(1);
  leg_cent->SetFillColor(19);
  leg_cent->SetFillStyle(0);

  TLegendEntry *entry_cent;
  entry_cent=leg_cent->AddEntry("raab","Closed charm: prompt J/#psi (CMS)","pf");
  entry_cent->SetFillColor(kRed-9);
  entry_cent->SetFillStyle(1001);
  entry_cent->SetLineColor(1);
  entry_cent->SetLineStyle(1);
  entry_cent->SetLineWidth(1);
  entry_cent->SetMarkerStyle(21);
  entry_cent->SetMarkerColor(kRed+2);
  entry_cent->SetMarkerSize(1.);
  entry_cent->SetTextSize(0.035);
  entry_cent=leg_cent->AddEntry("raabjpsi", "6.5<p_{T}<30 GeV/c,|y|<2.4","");
  entry_cent->SetTextFont(42);
  entry_cent->SetTextSize(0.03);


  TLegend *leg_alice_cent = new TLegend(0.2,0.67,0.7,0.77,NULL,"brNDC");
  leg_alice_cent->SetBorderSize(0);
  leg_alice_cent->SetTextFont(62);
  leg_alice_cent->SetTextSize(0.03);
  leg_alice_cent->SetLineColor(1);
  leg_alice_cent->SetLineStyle(1);
  leg_alice_cent->SetLineWidth(1);
  leg_alice_cent->SetFillColor(19);
  leg_alice_cent->SetFillStyle(0);
  TLegendEntry *entry_alice_cent=leg_alice_cent->AddEntry("pgAlice_dpt816","Open charm: prompt D (ALICE)","fP");
  entry_alice_cent->SetFillStyle(1001);
  entry_alice_cent->SetLineColor(1);
  entry_alice_cent->SetLineStyle(1);
  entry_alice_cent->SetLineWidth(1);
  entry_alice_cent->SetTextSize(0.035);
  entry_alice_cent->SetMarkerStyle(25);
  entry_alice_cent->SetMarkerSize(1);
  entry_alice_cent=leg_alice_cent->AddEntry("alicewhat","6<p_{T}<12 GeV/c, |y|<0.5","");
  entry_alice_cent->SetTextFont(42);
  entry_alice_cent->SetTextSize(0.03);
  

  leg_cent->Draw();
  leg_alice_cent->Draw();
  line->Draw();
  gPad->RedrawAxis();

  if(bSavePlots)
    {
      pc->SaveAs("figs/pdf/raaExpOpenClose_cent.pdf");
      pc->SaveAs("figs/png/raaExpOpenClose_cent.png");
    }
  // ##################### LOW PT ############################
  // ##################### LOW PT ############################
  // ##################### LOW PT ############################
  // ##################### LOW PT ############################
  // ##################### LOW PT ############################
  
  TGraphErrors *pgCms_low     = (TGraphErrors *)pfRaaCms_cent->Get("gPrJpsi_pt365y1624");
  TGraphErrors *pgCmsP_low    = (TGraphErrors *)pfRaaCms_cent->Get("gPrJpsiP_pt365y1624");
  TGraphErrors *pgCmsSyst_low = (TGraphErrors *)pfRaaCms_cent->Get("gPrJpsiSyst_pt365y1624");
  pgCmsSyst_low->SetFillColorAlpha(kViolet-9, 0.4);

  TBox *lumi_low = (TBox*)pfRaaCms_cent->Get("lumi_pr_y1624_pt365");
  

  //-------------------------------------------------------------------- 
  // *********** alice points:
  //  TGraphAsymmErrors(n,x,y,exl,exh,eyl,eyh);
  TGraphAsymmErrors *pgAlice_dpt25        = new TGraphAsymmErrors(nNpartBins5_alice,
								  npartBins5_alice, raaAlice_cent_dpt25,
								  npartBinsErr5_alice, npartBinsErr5_alice, 
								  raaAliceStat_cent_dpt25,raaAliceStat_cent_dpt25);
  TGraphAsymmErrors *pgAliceSyst_dpt25     = new TGraphAsymmErrors(nNpartBins5_alice,
								    npartBins5_alice, raaAlice_cent_dpt25, 
								    npartSystX5_alice, npartSystX5_alice,
								    raaAliceSystLow_cent_dpt612,raaAliceSystHigh_cent_dpt25);
  TGraphAsymmErrors *pgAliceSystCorr_dpt25 = new TGraphAsymmErrors(nNpartBins5_alice,
								    npartBins5_alice, raaAlice_cent_dpt25,
								    npartSystX5_alice, npartSystX5_alice,
								    raaAliceSystCorrLow_cent_dpt25,raaAliceSystCorrHigh_cent_dpt25);
  
  pgAlice_dpt25->SetMarkerStyle(25);
  pgAlice_dpt25->SetMarkerSize(1.2);

  // systmatic boxes
  // uncorrelated, open
  pgAliceSyst_dpt25->SetLineColor(1);
  pgAliceSyst_dpt25->SetFillColor(0);
  pgAliceSyst_dpt25->SetFillStyle(0);

  // correlated, filled
  pgAliceSystCorr_dpt25->SetLineColor(1);
  pgAliceSystCorr_dpt25->SetFillColor(kGray);
 
  //---------------------------------------------------------
  TCanvas *pc_low = new TCanvas("pc_low","pc_low");

  f4->Draw();
  lumi_low->Draw();
  pc_low->Update();

  pgAliceSystCorr_dpt25->Draw("2");
  pgCmsSyst_low->Draw("2");
  pgAliceSyst_dpt25->Draw("2");
  
  pgCmsP_low->Draw("P");
  pgCms_low->Draw("P");
  
  pgAlice_dpt25->Draw("P");

  // additional info
  // additional info
  CMS_lumi(pc_low,14005000,33);

  TLegend *leg2_cent = new TLegend(0.2,0.78,0.7,0.88,NULL,"brNDC");
  leg2_cent->SetBorderSize(0);
  leg2_cent->SetTextFont(62);
  leg2_cent->SetTextSize(0.03);
  leg2_cent->SetLineColor(1);
  leg2_cent->SetLineStyle(1);
  leg2_cent->SetLineWidth(1);
  leg2_cent->SetFillColor(19);
  leg2_cent->SetFillStyle(0);

  TLegendEntry *entry2_cent;
  entry2_cent=leg2_cent->AddEntry("raalow","Closed charm: prompt J/#psi (CMS)","pf");
  entry2_cent->SetTextSize(0.035);
  entry2_cent->SetLineColor(1);
  entry2_cent->SetLineStyle(1);
  entry2_cent->SetLineWidth(1);
  entry2_cent->SetFillColor(kViolet-9);
  entry2_cent->SetFillStyle(1001);
  entry2_cent->SetMarkerStyle(kFullCross);
  entry2_cent->SetMarkerColor(kViolet+2);
  entry2_cent->SetMarkerSize(1.7);
  entry2_cent=leg2_cent->AddEntry("raalowjpsi", "3<p_{T}<6.5 GeV/c,1.6<|y|<2.4","");
  entry2_cent->SetTextFont(42);
  entry2_cent->SetTextSize(0.03);


  TLegend *leg2_alice_cent = new TLegend(0.2,0.67,0.7,0.77,NULL,"brNDC");
  leg2_alice_cent->SetBorderSize(0);
  leg2_alice_cent->SetTextFont(62);
  leg2_alice_cent->SetTextSize(0.03);
  leg2_alice_cent->SetLineColor(1);
  leg2_alice_cent->SetLineStyle(1);
  leg2_alice_cent->SetLineWidth(1);
  leg2_alice_cent->SetFillColor(19);
  leg2_alice_cent->SetFillStyle(0);
  TLegendEntry *entry2_alice_cent=leg2_alice_cent->AddEntry("pgAlice_dpt25","Open charm: prompt D^{0} (ALICE)","fP");
  entry2_alice_cent->SetTextSize(0.035);
  entry2_alice_cent->SetLineColor(1);
  entry2_alice_cent->SetLineStyle(1);
  entry2_alice_cent->SetLineWidth(1);
  entry2_alice_cent->SetFillStyle(1001);
  entry2_alice_cent->SetMarkerStyle(25);
  entry2_alice_cent->SetMarkerSize(1.2);
  entry2_alice_cent=leg2_alice_cent->AddEntry("alicelow","2<p_{T}<5 GeV/c, |y|<0.5","");
  entry2_alice_cent->SetTextFont(42);
  entry2_alice_cent->SetTextSize(0.03);

  leg2_cent->Draw();
  leg2_alice_cent->Draw();
  line->Draw();

  gPad->RedrawAxis();

  if(bSavePlots)
    {   
      pc->SaveAs("figs/pdf/raaExpOpenClosePt25_cent.pdf");
      pc->SaveAs("figs/png/raaExpOpenClosePt25_cent.png");
    }
  
  
}


