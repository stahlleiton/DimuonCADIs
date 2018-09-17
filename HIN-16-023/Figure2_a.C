#include "tdrstyle.C"
#include "TSystem.h"
void Figure2_a()
{
//=========Macro generated from canvas: c1/c1
//=========  (Mon Sep 17 15:36:17 2018) by ROOT version 6.14/04
   setTDRStyle();
   TCanvas *c1 = new TCanvas("c1", "c1",0,0,600,600);
  gPad->SetLeftMargin(0.23);
  gPad->SetBottomMargin(0.16);
  gPad->SetRightMargin(0.06);
  gPad->SetTopMargin(0.1);
   gStyle->SetOptFit(1);
   gStyle->SetOptStat(0);
   gStyle->SetOptTitle(0);
   c1->Range(-9.71831,-6.297297,32.53521,1.810811);
   c1->SetFillColor(0);
   c1->SetBorderMode(0);
   c1->SetBorderSize(2);
   c1->SetLogy();
   c1->SetTickx(1);
   c1->SetTicky(1);
   c1->SetLeftMargin(0.23);
   c1->SetRightMargin(0.06);
   c1->SetBottomMargin(0.16);
   c1->SetFrameFillStyle(0);
   c1->SetFrameBorderMode(0);
   c1->SetFrameFillStyle(0);
   c1->SetFrameBorderMode(0);
   
   Double_t gCrossSection_pt_PP_0_fx1001[6] = {
   1,
   3,
   5,
   7.5,
   10.5,
   21};
   Double_t gCrossSection_pt_PP_0_fy1001[6] = {
   0.1019321,
   0.1931985,
   0.1763474,
   0.09310412,
   0.0384924,
   0.004588989};
   Double_t gCrossSection_pt_PP_0_fex1001[6] = {
   1,
   1,
   1,
   1.5,
   1.5,
   9};
   Double_t gCrossSection_pt_PP_0_fey1001[6] = {
   0.01562026,
   0.01173399,
   0.01055219,
   0.005103429,
   0.001050312,
   0.0001900465};
   TGraphErrors *gre = new TGraphErrors(6,gCrossSection_pt_PP_0_fx1001,gCrossSection_pt_PP_0_fy1001,gCrossSection_pt_PP_0_fex1001,gCrossSection_pt_PP_0_fey1001);
   gre->SetName("gCrossSection_pt_PP_0");
   gre->SetTitle("cs_vs_pt_PP");


   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = 1181;
   color = new TColor(ci, 1, 0.8, 0.8, " ", 0.5);
   gre->SetFillColor(ci);

   ci = TColor::GetColor("#990033");
   gre->SetLineColor(ci);
   gre->SetMarkerStyle(20);
   
   TH1F *Graph_gCrossSection_pt_PP_01001 = new TH1F("Graph_gCrossSection_pt_PP_01001","cs_vs_pt_PP",100,0,30);
   Graph_gCrossSection_pt_PP_01001->SetMinimum(1e-05);
   Graph_gCrossSection_pt_PP_01001->SetMaximum(10);
   Graph_gCrossSection_pt_PP_01001->SetDirectory(0);
   Graph_gCrossSection_pt_PP_01001->SetStats(0);
   Graph_gCrossSection_pt_PP_01001->SetLineStyle(0);
   Graph_gCrossSection_pt_PP_01001->SetMarkerStyle(20);
   Graph_gCrossSection_pt_PP_01001->GetXaxis()->SetTitle("p_{T} (GeV)");
   Graph_gCrossSection_pt_PP_01001->GetXaxis()->CenterTitle(true);
   Graph_gCrossSection_pt_PP_01001->GetXaxis()->SetLabelFont(42);
   Graph_gCrossSection_pt_PP_01001->GetXaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_pt_PP_01001->GetXaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_pt_PP_01001->GetXaxis()->SetTitleSize(0.06);
   Graph_gCrossSection_pt_PP_01001->GetXaxis()->SetTitleFont(42);
   Graph_gCrossSection_pt_PP_01001->GetYaxis()->SetTitle("B #frac{d^{2}#sigma}{dy dp_{T}} (nb/ GeV)");
   Graph_gCrossSection_pt_PP_01001->GetYaxis()->CenterTitle(true);
   Graph_gCrossSection_pt_PP_01001->GetYaxis()->SetLabelFont(42);
   Graph_gCrossSection_pt_PP_01001->GetYaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_pt_PP_01001->GetYaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_pt_PP_01001->GetYaxis()->SetTitleSize(0.045);
   Graph_gCrossSection_pt_PP_01001->GetYaxis()->SetTitleOffset(2);
   Graph_gCrossSection_pt_PP_01001->GetYaxis()->SetTitleFont(42);
   Graph_gCrossSection_pt_PP_01001->GetZaxis()->SetLabelFont(42);
   Graph_gCrossSection_pt_PP_01001->GetZaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_pt_PP_01001->GetZaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_pt_PP_01001->GetZaxis()->SetTitleSize(0.06);
   Graph_gCrossSection_pt_PP_01001->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_gCrossSection_pt_PP_01001);
   
   gre->Draw("a5");
   
   Double_t gCrossSection_pt_PP_1_fx1002[3] = {
   2,
   6.5,
   19.5};
   Double_t gCrossSection_pt_PP_1_fy1002[3] = {
   0.03141797,
   0.03228289,
   0.003183686};
   Double_t gCrossSection_pt_PP_1_fex1002[3] = {
   2,
   2.5,
   10.5};
   Double_t gCrossSection_pt_PP_1_fey1002[3] = {
   0.008006395,
   0.001949977,
   0.0001084223};
   gre = new TGraphErrors(3,gCrossSection_pt_PP_1_fx1002,gCrossSection_pt_PP_1_fy1002,gCrossSection_pt_PP_1_fex1002,gCrossSection_pt_PP_1_fey1002);
   gre->SetName("gCrossSection_pt_PP_1");
   gre->SetTitle("cs_vs_pt_PP");

   ci = 1182;
   color = new TColor(ci, 0.8, 0.8, 1, " ", 0.5);
   gre->SetFillColor(ci);

   ci = TColor::GetColor("#3333cc");
   gre->SetLineColor(ci);
   gre->SetMarkerStyle(20);
   
   TH1F *Graph_gCrossSection_pt_PP_11002 = new TH1F("Graph_gCrossSection_pt_PP_11002","cs_vs_pt_PP",100,0,33);
   Graph_gCrossSection_pt_PP_11002->SetMinimum(0.002767737);
   Graph_gCrossSection_pt_PP_11002->SetMaximum(0.04305928);
   Graph_gCrossSection_pt_PP_11002->SetDirectory(0);
   Graph_gCrossSection_pt_PP_11002->SetStats(0);
   Graph_gCrossSection_pt_PP_11002->SetLineStyle(0);
   Graph_gCrossSection_pt_PP_11002->SetMarkerStyle(20);
   Graph_gCrossSection_pt_PP_11002->GetXaxis()->SetLabelFont(42);
   Graph_gCrossSection_pt_PP_11002->GetXaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_pt_PP_11002->GetXaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_pt_PP_11002->GetXaxis()->SetTitleSize(0.06);
   Graph_gCrossSection_pt_PP_11002->GetXaxis()->SetTitleOffset(0.9);
   Graph_gCrossSection_pt_PP_11002->GetXaxis()->SetTitleFont(42);
   Graph_gCrossSection_pt_PP_11002->GetYaxis()->SetLabelFont(42);
   Graph_gCrossSection_pt_PP_11002->GetYaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_pt_PP_11002->GetYaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_pt_PP_11002->GetYaxis()->SetTitleSize(0.06);
   Graph_gCrossSection_pt_PP_11002->GetYaxis()->SetTitleOffset(1.25);
   Graph_gCrossSection_pt_PP_11002->GetYaxis()->SetTitleFont(42);
   Graph_gCrossSection_pt_PP_11002->GetZaxis()->SetLabelFont(42);
   Graph_gCrossSection_pt_PP_11002->GetZaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_pt_PP_11002->GetZaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_pt_PP_11002->GetZaxis()->SetTitleSize(0.06);
   Graph_gCrossSection_pt_PP_11002->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_gCrossSection_pt_PP_11002);
   
   gre->Draw("5");
   
   Double_t gCrossSection_pt_PP_2_fx1003[2] = {
   3,
   18};
   Double_t gCrossSection_pt_PP_2_fy1003[2] = {
   0.01443302,
   0.003360191};
   Double_t gCrossSection_pt_PP_2_fex1003[2] = {
   3,
   12};
   Double_t gCrossSection_pt_PP_2_fey1003[2] = {
   0.0007224115,
   0.0001381983};
   gre = new TGraphErrors(2,gCrossSection_pt_PP_2_fx1003,gCrossSection_pt_PP_2_fy1003,gCrossSection_pt_PP_2_fex1003,gCrossSection_pt_PP_2_fey1003);
   gre->SetName("gCrossSection_pt_PP_2");
   gre->SetTitle("cs_vs_pt_PP");

   ci = 1183;
   color = new TColor(ci, 0.8, 1, 0.8, " ", 0.5);
   gre->SetFillColor(ci);

   ci = TColor::GetColor("#009900");
   gre->SetLineColor(ci);
   gre->SetMarkerStyle(20);
   
   TH1F *Graph_gCrossSection_pt_PP_21003 = new TH1F("Graph_gCrossSection_pt_PP_21003","cs_vs_pt_PP",100,0,33);
   Graph_gCrossSection_pt_PP_21003->SetMinimum(0.002028648);
   Graph_gCrossSection_pt_PP_21003->SetMaximum(0.01634878);
   Graph_gCrossSection_pt_PP_21003->SetDirectory(0);
   Graph_gCrossSection_pt_PP_21003->SetStats(0);
   Graph_gCrossSection_pt_PP_21003->SetLineStyle(0);
   Graph_gCrossSection_pt_PP_21003->SetMarkerStyle(20);
   Graph_gCrossSection_pt_PP_21003->GetXaxis()->SetLabelFont(42);
   Graph_gCrossSection_pt_PP_21003->GetXaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_pt_PP_21003->GetXaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_pt_PP_21003->GetXaxis()->SetTitleSize(0.06);
   Graph_gCrossSection_pt_PP_21003->GetXaxis()->SetTitleOffset(0.9);
   Graph_gCrossSection_pt_PP_21003->GetXaxis()->SetTitleFont(42);
   Graph_gCrossSection_pt_PP_21003->GetYaxis()->SetLabelFont(42);
   Graph_gCrossSection_pt_PP_21003->GetYaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_pt_PP_21003->GetYaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_pt_PP_21003->GetYaxis()->SetTitleSize(0.06);
   Graph_gCrossSection_pt_PP_21003->GetYaxis()->SetTitleOffset(1.25);
   Graph_gCrossSection_pt_PP_21003->GetYaxis()->SetTitleFont(42);
   Graph_gCrossSection_pt_PP_21003->GetZaxis()->SetLabelFont(42);
   Graph_gCrossSection_pt_PP_21003->GetZaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_pt_PP_21003->GetZaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_pt_PP_21003->GetZaxis()->SetTitleSize(0.06);
   Graph_gCrossSection_pt_PP_21003->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_gCrossSection_pt_PP_21003);
   
   gre->Draw("5");
   
   Double_t gCrossSection_pt_PP_0_fx1004[6] = {
   1,
   3,
   5,
   7.5,
   10.5,
   21};
   Double_t gCrossSection_pt_PP_0_fy1004[6] = {
   0.1019321,
   0.1931985,
   0.1763474,
   0.09310412,
   0.0384924,
   0.004588989};
   Double_t gCrossSection_pt_PP_0_fex1004[6] = {
   0,
   0,
   0,
   0,
   0,
   0};
   Double_t gCrossSection_pt_PP_0_fey1004[6] = {
   0.002064104,
   0.0056473,
   0.004011357,
   0.001872668,
   0.0008224874,
   8.847451e-05};
   gre = new TGraphErrors(6,gCrossSection_pt_PP_0_fx1004,gCrossSection_pt_PP_0_fy1004,gCrossSection_pt_PP_0_fex1004,gCrossSection_pt_PP_0_fey1004);
   gre->SetName("gCrossSection_pt_PP_0");
   gre->SetTitle("cs_vs_pt_PP");

   ci = TColor::GetColor("#990033");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#990033");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetMarkerSize(1.5);
   
   TH1F *Graph_gCrossSection_pt_PP_01004 = new TH1F("Graph_gCrossSection_pt_PP_01004","cs_vs_pt_PP",100,0,23);
   Graph_gCrossSection_pt_PP_01004->SetMinimum(0.004050463);
   Graph_gCrossSection_pt_PP_01004->SetMaximum(0.2182804);
   Graph_gCrossSection_pt_PP_01004->SetDirectory(0);
   Graph_gCrossSection_pt_PP_01004->SetStats(0);
   Graph_gCrossSection_pt_PP_01004->SetLineStyle(0);
   Graph_gCrossSection_pt_PP_01004->SetMarkerStyle(20);
   Graph_gCrossSection_pt_PP_01004->GetXaxis()->SetLabelFont(42);
   Graph_gCrossSection_pt_PP_01004->GetXaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_pt_PP_01004->GetXaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_pt_PP_01004->GetXaxis()->SetTitleSize(0.06);
   Graph_gCrossSection_pt_PP_01004->GetXaxis()->SetTitleOffset(0.9);
   Graph_gCrossSection_pt_PP_01004->GetXaxis()->SetTitleFont(42);
   Graph_gCrossSection_pt_PP_01004->GetYaxis()->SetLabelFont(42);
   Graph_gCrossSection_pt_PP_01004->GetYaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_pt_PP_01004->GetYaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_pt_PP_01004->GetYaxis()->SetTitleSize(0.06);
   Graph_gCrossSection_pt_PP_01004->GetYaxis()->SetTitleOffset(1.25);
   Graph_gCrossSection_pt_PP_01004->GetYaxis()->SetTitleFont(42);
   Graph_gCrossSection_pt_PP_01004->GetZaxis()->SetLabelFont(42);
   Graph_gCrossSection_pt_PP_01004->GetZaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_pt_PP_01004->GetZaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_pt_PP_01004->GetZaxis()->SetTitleSize(0.06);
   Graph_gCrossSection_pt_PP_01004->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_gCrossSection_pt_PP_01004);
   
   gre->Draw("p");
   
   auto gre1 = (TGraphErrors*)gre;
   Double_t gCrossSection_pt_PP_1_fx1005[3] = {
   2,
   6.5,
   19.5};
   Double_t gCrossSection_pt_PP_1_fy1005[3] = {
   0.03141797,
   0.03228289,
   0.003183686};
   Double_t gCrossSection_pt_PP_1_fex1005[3] = {
   0,
   0,
   0};
   Double_t gCrossSection_pt_PP_1_fey1005[3] = {
   0.001167282,
   0.0008744681,
   8.246174e-05};
   gre = new TGraphErrors(3,gCrossSection_pt_PP_1_fx1005,gCrossSection_pt_PP_1_fy1005,gCrossSection_pt_PP_1_fex1005,gCrossSection_pt_PP_1_fey1005);
   gre->SetName("gCrossSection_pt_PP_1");
   gre->SetTitle("cs_vs_pt_PP");

   ci = TColor::GetColor("#3333cc");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#3333cc");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(21);
   gre->SetMarkerSize(1.5);
   
   TH1F *Graph_gCrossSection_pt_PP_11005 = new TH1F("Graph_gCrossSection_pt_PP_11005","cs_vs_pt_PP",100,0.25,21.25);
   Graph_gCrossSection_pt_PP_11005->SetMinimum(9.561031e-05);
   Graph_gCrossSection_pt_PP_11005->SetMaximum(0.03616298);
   Graph_gCrossSection_pt_PP_11005->SetDirectory(0);
   Graph_gCrossSection_pt_PP_11005->SetStats(0);
   Graph_gCrossSection_pt_PP_11005->SetLineStyle(0);
   Graph_gCrossSection_pt_PP_11005->SetMarkerStyle(20);
   Graph_gCrossSection_pt_PP_11005->GetXaxis()->SetLabelFont(42);
   Graph_gCrossSection_pt_PP_11005->GetXaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_pt_PP_11005->GetXaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_pt_PP_11005->GetXaxis()->SetTitleSize(0.06);
   Graph_gCrossSection_pt_PP_11005->GetXaxis()->SetTitleOffset(0.9);
   Graph_gCrossSection_pt_PP_11005->GetXaxis()->SetTitleFont(42);
   Graph_gCrossSection_pt_PP_11005->GetYaxis()->SetLabelFont(42);
   Graph_gCrossSection_pt_PP_11005->GetYaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_pt_PP_11005->GetYaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_pt_PP_11005->GetYaxis()->SetTitleSize(0.06);
   Graph_gCrossSection_pt_PP_11005->GetYaxis()->SetTitleOffset(1.25);
   Graph_gCrossSection_pt_PP_11005->GetYaxis()->SetTitleFont(42);
   Graph_gCrossSection_pt_PP_11005->GetZaxis()->SetLabelFont(42);
   Graph_gCrossSection_pt_PP_11005->GetZaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_pt_PP_11005->GetZaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_pt_PP_11005->GetZaxis()->SetTitleSize(0.06);
   Graph_gCrossSection_pt_PP_11005->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_gCrossSection_pt_PP_11005);
   
   gre->Draw("p");
   auto gre2 = (TGraphErrors*)gre;
   
   Double_t gCrossSection_pt_PP_2_fx1006[2] = {
   3,
   18};
   Double_t gCrossSection_pt_PP_2_fy1006[2] = {
   0.01443302,
   0.003360191};
   Double_t gCrossSection_pt_PP_2_fex1006[2] = {
   0,
   0};
   Double_t gCrossSection_pt_PP_2_fey1006[2] = {
   0.0007224115,
   0.0001381983};
   gre = new TGraphErrors(2,gCrossSection_pt_PP_2_fx1006,gCrossSection_pt_PP_2_fy1006,gCrossSection_pt_PP_2_fex1006,gCrossSection_pt_PP_2_fey1006);
   gre->SetName("gCrossSection_pt_PP_2");
   gre->SetTitle("cs_vs_pt_PP");

   ci = TColor::GetColor("#009900");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#009900");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(33);
   gre->SetMarkerSize(2.6);
   
   TH1F *Graph_gCrossSection_pt_PP_21006 = new TH1F("Graph_gCrossSection_pt_PP_21006","cs_vs_pt_PP",100,1.5,19.5);
   Graph_gCrossSection_pt_PP_21006->SetMinimum(0.002028648);
   Graph_gCrossSection_pt_PP_21006->SetMaximum(0.01634878);
   Graph_gCrossSection_pt_PP_21006->SetDirectory(0);
   Graph_gCrossSection_pt_PP_21006->SetStats(0);
   Graph_gCrossSection_pt_PP_21006->SetLineStyle(0);
   Graph_gCrossSection_pt_PP_21006->SetMarkerStyle(20);
   Graph_gCrossSection_pt_PP_21006->GetXaxis()->SetLabelFont(42);
   Graph_gCrossSection_pt_PP_21006->GetXaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_pt_PP_21006->GetXaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_pt_PP_21006->GetXaxis()->SetTitleSize(0.06);
   Graph_gCrossSection_pt_PP_21006->GetXaxis()->SetTitleOffset(0.9);
   Graph_gCrossSection_pt_PP_21006->GetXaxis()->SetTitleFont(42);
   Graph_gCrossSection_pt_PP_21006->GetYaxis()->SetLabelFont(42);
   Graph_gCrossSection_pt_PP_21006->GetYaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_pt_PP_21006->GetYaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_pt_PP_21006->GetYaxis()->SetTitleSize(0.06);
   Graph_gCrossSection_pt_PP_21006->GetYaxis()->SetTitleOffset(1.25);
   Graph_gCrossSection_pt_PP_21006->GetYaxis()->SetTitleFont(42);
   Graph_gCrossSection_pt_PP_21006->GetZaxis()->SetLabelFont(42);
   Graph_gCrossSection_pt_PP_21006->GetZaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_pt_PP_21006->GetZaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_pt_PP_21006->GetZaxis()->SetTitleSize(0.06);
   Graph_gCrossSection_pt_PP_21006->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_gCrossSection_pt_PP_21006);
   
   auto gre3 = (TGraphErrors*)gre;
   gre->Draw("p");
   
   TLegend *leg = new TLegend(0.68,0.575,0.85,0.72,NULL,"brNDC");
   leg->SetMargin(0.2);
   leg->SetBorderSize(0);
   leg->SetTextSize(0.04);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetFillStyle(0);
   TLegendEntry *entry=leg->AddEntry(gre1," #varUpsilon(1S)","lp");
   //TLegendEntry *entry=leg->AddEntry("gCrossSection_pt_PP_0"," #varUpsilon(1S)","lp");

   ci = TColor::GetColor("#990033");
   entry->SetLineColor(ci);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);

   ci = TColor::GetColor("#990033");
   entry->SetMarkerColor(ci);
   entry->SetMarkerStyle(20);
   entry->SetMarkerSize(1.5);
   entry->SetTextFont(42);
   entry=leg->AddEntry(gre2," #varUpsilon(2S)","lp");
   //entry=leg->AddEntry("gCrossSection_pt_PP_1"," #varUpsilon(2S)","lp");

   ci = TColor::GetColor("#3333cc");
   entry->SetLineColor(ci);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);

   ci = TColor::GetColor("#3333cc");
   entry->SetMarkerColor(ci);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1.5);
   entry->SetTextFont(42);
   entry=leg->AddEntry(gre3," #varUpsilon(3S)","lp");
   //entry=leg->AddEntry("gCrossSection_pt_PP_2"," #varUpsilon(3S)","lp");

   ci = TColor::GetColor("#009900");
   entry->SetLineColor(ci);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);

   ci = TColor::GetColor("#009900");
   entry->SetMarkerColor(ci);
   entry->SetMarkerStyle(33);
   entry->SetMarkerSize(2.6);
   entry->SetTextFont(42);
   leg->Draw();
   TLatex *   tex = new TLatex(0.27,0.8225,"|y| < 2.4");
tex->SetNDC();
   tex->SetTextAlign(12);
   tex->SetTextFont(42);
   tex->SetTextSize(0.04);
   tex->SetLineWidth(2);
   tex->Draw();
      tex = new TLatex(0.94,0.92,"pp 28.0 pb^{-1} (5.02 TeV)");
tex->SetNDC();
   tex->SetTextAlign(31);
   tex->SetTextFont(42);
   tex->SetTextSize(0.054);
   tex->SetLineWidth(2);
   tex->Draw();
      tex = new TLatex(0.89805,0.8541,"CMS");
tex->SetNDC();
   tex->SetTextAlign(33);
   tex->SetTextFont(61);
   tex->SetTextSize(0.0675);
   tex->SetLineWidth(2);
   tex->Draw();
   c1->Modified();
   c1->cd();
   c1->SetSelected(c1);
   c1->SaveAs("Figure2_a.pdf");
}
