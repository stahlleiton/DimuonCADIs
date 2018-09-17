#include "tdrstyle.C"
void Figure2_b()
{
//=========Macro generated from canvas: c1/c1
//=========  (Mon Sep 17 15:45:08 2018) by ROOT version 6.14/04
   TCanvas *c1 = new TCanvas("c1", "c1",0,0,600,600);
   gStyle->SetOptFit(1);
   gStyle->SetOptStat(0);
   gStyle->SetOptTitle(0);
   gStyle->SetEndErrorSize(0);
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
   
   Double_t gCrossSection_pt_AA_0_fx1001[6] = {
   1,
   3,
   5,
   7.5,
   10.5,
   21};
   Double_t gCrossSection_pt_AA_0_fy1001[6] = {
   0.03071593,
   0.06989949,
   0.06835964,
   0.03739938,
   0.01625198,
   0.001948034};
   Double_t gCrossSection_pt_AA_0_fex1001[6] = {
   1,
   1,
   1,
   1.5,
   1.5,
   9};
   Double_t gCrossSection_pt_AA_0_fey1001[6] = {
   0.01162118,
   0.005253709,
   0.004684009,
   0.002107146,
   0.001669093,
   0.0001432995};
   TGraphErrors *gre = new TGraphErrors(6,gCrossSection_pt_AA_0_fx1001,gCrossSection_pt_AA_0_fy1001,gCrossSection_pt_AA_0_fex1001,gCrossSection_pt_AA_0_fey1001);
   gre->SetName("gCrossSection_pt_AA_0");
   gre->SetTitle("cs_vs_pt_AA");

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = 1181;
   color = new TColor(ci, 1, 0.8, 0.8, " ", 0.5);
   gre->SetFillColor(ci);

   ci = TColor::GetColor("#990033");
   gre->SetLineColor(ci);
   gre->SetMarkerColor(2);
   gre->SetMarkerStyle(20);
   
   TH1F *Graph_gCrossSection_pt_AA_01001 = new TH1F("Graph_gCrossSection_pt_AA_01001","cs_vs_pt_AA",100,0,30);
   Graph_gCrossSection_pt_AA_01001->SetMinimum(1e-05);
   Graph_gCrossSection_pt_AA_01001->SetMaximum(10);
   Graph_gCrossSection_pt_AA_01001->SetDirectory(0);
   Graph_gCrossSection_pt_AA_01001->SetStats(0);
   Graph_gCrossSection_pt_AA_01001->SetLineStyle(0);
   Graph_gCrossSection_pt_AA_01001->SetMarkerStyle(20);
   Graph_gCrossSection_pt_AA_01001->GetXaxis()->SetTitle("p_{T} (GeV)");
   Graph_gCrossSection_pt_AA_01001->GetXaxis()->CenterTitle(true);
   Graph_gCrossSection_pt_AA_01001->GetXaxis()->SetLabelFont(42);
   Graph_gCrossSection_pt_AA_01001->GetXaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_pt_AA_01001->GetXaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_pt_AA_01001->GetXaxis()->SetTitleSize(0.06);
   Graph_gCrossSection_pt_AA_01001->GetXaxis()->SetTitleFont(42);
   Graph_gCrossSection_pt_AA_01001->GetYaxis()->SetTitle("B #frac{1}{T_{AA}} #frac{d^{2}N}{dy dp_{T}} (nb/ GeV)");
   Graph_gCrossSection_pt_AA_01001->GetYaxis()->CenterTitle(true);
   Graph_gCrossSection_pt_AA_01001->GetYaxis()->SetLabelFont(42);
   Graph_gCrossSection_pt_AA_01001->GetYaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_pt_AA_01001->GetYaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_pt_AA_01001->GetYaxis()->SetTitleSize(0.045);
   Graph_gCrossSection_pt_AA_01001->GetYaxis()->SetTitleOffset(2);
   Graph_gCrossSection_pt_AA_01001->GetYaxis()->SetTitleFont(42);
   Graph_gCrossSection_pt_AA_01001->GetZaxis()->SetLabelFont(42);
   Graph_gCrossSection_pt_AA_01001->GetZaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_pt_AA_01001->GetZaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_pt_AA_01001->GetZaxis()->SetTitleSize(0.06);
   Graph_gCrossSection_pt_AA_01001->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_gCrossSection_pt_AA_01001);
   
   gre->Draw("a5");
   
   Double_t gCrossSection_pt_AA_1_fx1002[3] = {
   2,
   6.5,
   19.5};
   Double_t gCrossSection_pt_AA_1_fy1002[3] = {
   0.002659123,
   0.004143789,
   0.0004004361};
   Double_t gCrossSection_pt_AA_1_fex1002[3] = {
   2,
   2.5,
   10.5};
   Double_t gCrossSection_pt_AA_1_fey1002[3] = {
   0.002916014,
   0.0009593218,
   4.946594e-05};
   gre = new TGraphErrors(3,gCrossSection_pt_AA_1_fx1002,gCrossSection_pt_AA_1_fy1002,gCrossSection_pt_AA_1_fex1002,gCrossSection_pt_AA_1_fey1002);
   gre->SetName("gCrossSection_pt_AA_1");
   gre->SetTitle("cs_vs_pt_AA");

   ci = 1182;
   color = new TColor(ci, 0.8, 0.8, 1, " ", 0.5);
   gre->SetFillColor(ci);

   ci = TColor::GetColor("#3333cc");
   gre->SetLineColor(ci);
   gre->SetMarkerColor(2);
   gre->SetMarkerStyle(20);
   
   TH1F *Graph_gCrossSection_pt_AA_11002 = new TH1F("Graph_gCrossSection_pt_AA_11002","cs_vs_pt_AA",100,0,33);
   Graph_gCrossSection_pt_AA_11002->SetMinimum(0.0003158731);
   Graph_gCrossSection_pt_AA_11002->SetMaximum(0.006097553);
   Graph_gCrossSection_pt_AA_11002->SetDirectory(0);
   Graph_gCrossSection_pt_AA_11002->SetStats(0);
   Graph_gCrossSection_pt_AA_11002->SetLineStyle(0);
   Graph_gCrossSection_pt_AA_11002->SetMarkerStyle(20);
   Graph_gCrossSection_pt_AA_11002->GetXaxis()->SetLabelFont(42);
   Graph_gCrossSection_pt_AA_11002->GetXaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_pt_AA_11002->GetXaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_pt_AA_11002->GetXaxis()->SetTitleSize(0.06);
   Graph_gCrossSection_pt_AA_11002->GetXaxis()->SetTitleOffset(0.9);
   Graph_gCrossSection_pt_AA_11002->GetXaxis()->SetTitleFont(42);
   Graph_gCrossSection_pt_AA_11002->GetYaxis()->SetLabelFont(42);
   Graph_gCrossSection_pt_AA_11002->GetYaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_pt_AA_11002->GetYaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_pt_AA_11002->GetYaxis()->SetTitleSize(0.06);
   Graph_gCrossSection_pt_AA_11002->GetYaxis()->SetTitleOffset(1.25);
   Graph_gCrossSection_pt_AA_11002->GetYaxis()->SetTitleFont(42);
   Graph_gCrossSection_pt_AA_11002->GetZaxis()->SetLabelFont(42);
   Graph_gCrossSection_pt_AA_11002->GetZaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_pt_AA_11002->GetZaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_pt_AA_11002->GetZaxis()->SetTitleSize(0.06);
   Graph_gCrossSection_pt_AA_11002->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_gCrossSection_pt_AA_11002);
   
   gre->Draw("5");
   TBox *box = new TBox(2.4,0,3.6,0.001319571);

   ci = 1179;
   color = new TColor(ci, 0.4, 0.8, 0.4, " ", 0.5);
   box->SetFillColor(ci);

   ci = TColor::GetColor("#006600");
   box->SetLineColor(ci);
   box->Draw("l");
   box = new TBox(17.4,0,18.6,0.0002006808);

   ci = 1180;
   color = new TColor(ci, 0.4, 0.8, 0.4, " ", 0.5);
   box->SetFillColor(ci);

   ci = TColor::GetColor("#006600");
   box->SetLineColor(ci);
   box->Draw("l");
   
   Double_t gCrossSection_pt_AA_2_fx1003[3] = {
   -10,
   -11,
   -12};
   Double_t gCrossSection_pt_AA_2_fy1003[3] = {
   -10,
   -11,
   -12};
   Double_t gCrossSection_pt_AA_2_fex1003[3] = {
   0,
   0,
   0};
   Double_t gCrossSection_pt_AA_2_fey1003[3] = {
   0,
   0,
   0};
   gre = new TGraphErrors(3,gCrossSection_pt_AA_2_fx1003,gCrossSection_pt_AA_2_fy1003,gCrossSection_pt_AA_2_fex1003,gCrossSection_pt_AA_2_fey1003);
   gre->SetName("gCrossSection_pt_AA_2");
   gre->SetTitle("cs_vs_pt_AA");

   ci = 1183;
   color = new TColor(ci, 0.8, 1, 0.8, " ", 0.5);
   gre->SetFillColor(ci);

   ci = TColor::GetColor("#009900");
   gre->SetLineColor(ci);
   gre->SetMarkerColor(2);
   gre->SetMarkerStyle(20);
   
   TH1F *Graph_gCrossSection_pt_AA_21003 = new TH1F("Graph_gCrossSection_pt_AA_21003","cs_vs_pt_AA",100,0,30);
   Graph_gCrossSection_pt_AA_21003->SetMinimum(0);
   Graph_gCrossSection_pt_AA_21003->SetMaximum(1.3);
   Graph_gCrossSection_pt_AA_21003->SetDirectory(0);
   Graph_gCrossSection_pt_AA_21003->SetStats(0);
   Graph_gCrossSection_pt_AA_21003->SetLineStyle(0);
   Graph_gCrossSection_pt_AA_21003->SetMarkerStyle(20);
   Graph_gCrossSection_pt_AA_21003->GetXaxis()->SetRange(1,100);
   Graph_gCrossSection_pt_AA_21003->GetXaxis()->SetLabelFont(42);
   Graph_gCrossSection_pt_AA_21003->GetXaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_pt_AA_21003->GetXaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_pt_AA_21003->GetXaxis()->SetTitleSize(0.06);
   Graph_gCrossSection_pt_AA_21003->GetXaxis()->SetTitleOffset(0.9);
   Graph_gCrossSection_pt_AA_21003->GetXaxis()->SetTitleFont(42);
   Graph_gCrossSection_pt_AA_21003->GetYaxis()->SetLabelFont(42);
   Graph_gCrossSection_pt_AA_21003->GetYaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_pt_AA_21003->GetYaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_pt_AA_21003->GetYaxis()->SetTitleSize(0.06);
   Graph_gCrossSection_pt_AA_21003->GetYaxis()->SetTitleOffset(1.25);
   Graph_gCrossSection_pt_AA_21003->GetYaxis()->SetTitleFont(42);
   Graph_gCrossSection_pt_AA_21003->GetZaxis()->SetLabelFont(42);
   Graph_gCrossSection_pt_AA_21003->GetZaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_pt_AA_21003->GetZaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_pt_AA_21003->GetZaxis()->SetTitleSize(0.06);
   Graph_gCrossSection_pt_AA_21003->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_gCrossSection_pt_AA_21003);
   
   gre->Draw("5");
   
   Double_t gCrossSection_pt_AA_0_fx1004[6] = {
   1,
   3,
   5,
   7.5,
   10.5,
   21};
   Double_t gCrossSection_pt_AA_0_fy1004[6] = {
   0.03071593,
   0.06989949,
   0.06835964,
   0.03739938,
   0.01625198,
   0.001948034};
   Double_t gCrossSection_pt_AA_0_fex1004[6] = {
   0,
   0,
   0,
   0,
   0,
   0};
   Double_t gCrossSection_pt_AA_0_fey1004[6] = {
   0.002624668,
   0.005195844,
   0.005551522,
   0.00249163,
   0.001280485,
   0.000110687};
   gre = new TGraphErrors(6,gCrossSection_pt_AA_0_fx1004,gCrossSection_pt_AA_0_fy1004,gCrossSection_pt_AA_0_fex1004,gCrossSection_pt_AA_0_fey1004);
   gre->SetName("gCrossSection_pt_AA_0");
   gre->SetTitle("cs_vs_pt_AA");

   ci = TColor::GetColor("#990033");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#990033");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetMarkerSize(1.5);
   
   TH1F *Graph_gCrossSection_pt_AA_01004 = new TH1F("Graph_gCrossSection_pt_AA_01004","cs_vs_pt_AA",100,0,23);
   Graph_gCrossSection_pt_AA_01004->SetMinimum(0.001653612);
   Graph_gCrossSection_pt_AA_01004->SetMaximum(0.08242114);
   Graph_gCrossSection_pt_AA_01004->SetDirectory(0);
   Graph_gCrossSection_pt_AA_01004->SetStats(0);
   Graph_gCrossSection_pt_AA_01004->SetLineStyle(0);
   Graph_gCrossSection_pt_AA_01004->SetMarkerStyle(20);
   Graph_gCrossSection_pt_AA_01004->GetXaxis()->SetLabelFont(42);
   Graph_gCrossSection_pt_AA_01004->GetXaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_pt_AA_01004->GetXaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_pt_AA_01004->GetXaxis()->SetTitleSize(0.06);
   Graph_gCrossSection_pt_AA_01004->GetXaxis()->SetTitleOffset(0.9);
   Graph_gCrossSection_pt_AA_01004->GetXaxis()->SetTitleFont(42);
   Graph_gCrossSection_pt_AA_01004->GetYaxis()->SetLabelFont(42);
   Graph_gCrossSection_pt_AA_01004->GetYaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_pt_AA_01004->GetYaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_pt_AA_01004->GetYaxis()->SetTitleSize(0.06);
   Graph_gCrossSection_pt_AA_01004->GetYaxis()->SetTitleOffset(1.25);
   Graph_gCrossSection_pt_AA_01004->GetYaxis()->SetTitleFont(42);
   Graph_gCrossSection_pt_AA_01004->GetZaxis()->SetLabelFont(42);
   Graph_gCrossSection_pt_AA_01004->GetZaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_pt_AA_01004->GetZaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_pt_AA_01004->GetZaxis()->SetTitleSize(0.06);
   Graph_gCrossSection_pt_AA_01004->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_gCrossSection_pt_AA_01004);
   
   gre->Draw("p");
   auto gre1 = (TGraphErrors*) gre;

   Double_t gCrossSection_pt_AA_1_fx1005[3] = {
   2,
   6.5,
   19.5};
   Double_t gCrossSection_pt_AA_1_fy1005[3] = {
   0.002659123,
   0.004143789,
   0.0004004361};
   Double_t gCrossSection_pt_AA_1_fex1005[3] = {
   0,
   0,
   0};
   Double_t gCrossSection_pt_AA_1_fey1005[3] = {
   0.001223577,
   0.001209258,
   0.00010247};
   gre = new TGraphErrors(3,gCrossSection_pt_AA_1_fx1005,gCrossSection_pt_AA_1_fy1005,gCrossSection_pt_AA_1_fex1005,gCrossSection_pt_AA_1_fey1005);
   gre->SetName("gCrossSection_pt_AA_1");
   gre->SetTitle("cs_vs_pt_AA");

   ci = TColor::GetColor("#3333cc");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#3333cc");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(21);
   gre->SetMarkerSize(1.5);
   
   TH1F *Graph_gCrossSection_pt_AA_11005 = new TH1F("Graph_gCrossSection_pt_AA_11005","cs_vs_pt_AA",100,0.25,21.25);
   Graph_gCrossSection_pt_AA_11005->SetMinimum(0.0002681695);
   Graph_gCrossSection_pt_AA_11005->SetMaximum(0.005858556);
   Graph_gCrossSection_pt_AA_11005->SetDirectory(0);
   Graph_gCrossSection_pt_AA_11005->SetStats(0);
   Graph_gCrossSection_pt_AA_11005->SetLineStyle(0);
   Graph_gCrossSection_pt_AA_11005->SetMarkerStyle(20);
   Graph_gCrossSection_pt_AA_11005->GetXaxis()->SetLabelFont(42);
   Graph_gCrossSection_pt_AA_11005->GetXaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_pt_AA_11005->GetXaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_pt_AA_11005->GetXaxis()->SetTitleSize(0.06);
   Graph_gCrossSection_pt_AA_11005->GetXaxis()->SetTitleOffset(0.9);
   Graph_gCrossSection_pt_AA_11005->GetXaxis()->SetTitleFont(42);
   Graph_gCrossSection_pt_AA_11005->GetYaxis()->SetLabelFont(42);
   Graph_gCrossSection_pt_AA_11005->GetYaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_pt_AA_11005->GetYaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_pt_AA_11005->GetYaxis()->SetTitleSize(0.06);
   Graph_gCrossSection_pt_AA_11005->GetYaxis()->SetTitleOffset(1.25);
   Graph_gCrossSection_pt_AA_11005->GetYaxis()->SetTitleFont(42);
   Graph_gCrossSection_pt_AA_11005->GetZaxis()->SetLabelFont(42);
   Graph_gCrossSection_pt_AA_11005->GetZaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_pt_AA_11005->GetZaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_pt_AA_11005->GetZaxis()->SetTitleSize(0.06);
   Graph_gCrossSection_pt_AA_11005->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_gCrossSection_pt_AA_11005);
   
   gre->Draw("p");
   auto gre2 = (TGraphErrors*) gre;
   TArrow *arrow = new TArrow(3,0,3,0.002238415,0.027,"<-|");
   arrow->SetFillColor(1);
   arrow->SetFillStyle(1001);

   ci = TColor::GetColor("#009900");
   arrow->SetLineColor(ci);
   arrow->SetLineWidth(2);
   arrow->Draw();
   arrow = new TArrow(18,0,18,0.0003722393,0.027,"<-|");
   arrow->SetFillColor(1);
   arrow->SetFillStyle(1001);

   ci = TColor::GetColor("#009900");
   arrow->SetLineColor(ci);
   arrow->SetLineWidth(2);
   arrow->Draw();
   
   Double_t gCrossSection_pt_AA_2_fx1006[3] = {
   -10,
   -11,
   -12};
   Double_t gCrossSection_pt_AA_2_fy1006[3] = {
   -10,
   -11,
   -12};
   Double_t gCrossSection_pt_AA_2_fex1006[3] = {
   0,
   0,
   0};
   Double_t gCrossSection_pt_AA_2_fey1006[3] = {
   0,
   0,
   0};
   gre = new TGraphErrors(3,gCrossSection_pt_AA_2_fx1006,gCrossSection_pt_AA_2_fy1006,gCrossSection_pt_AA_2_fex1006,gCrossSection_pt_AA_2_fey1006);
   gre->SetName("gCrossSection_pt_AA_2");
   gre->SetTitle("cs_vs_pt_AA");

   ci = TColor::GetColor("#009900");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#009900");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(33);
   gre->SetMarkerSize(2.6);
   
   TH1F *Graph_gCrossSection_pt_AA_21006 = new TH1F("Graph_gCrossSection_pt_AA_21006","cs_vs_pt_AA",100,0,30);
   Graph_gCrossSection_pt_AA_21006->SetMinimum(0);
   Graph_gCrossSection_pt_AA_21006->SetMaximum(1.3);
   Graph_gCrossSection_pt_AA_21006->SetDirectory(0);
   Graph_gCrossSection_pt_AA_21006->SetStats(0);
   Graph_gCrossSection_pt_AA_21006->SetLineStyle(0);
   Graph_gCrossSection_pt_AA_21006->SetMarkerStyle(20);
   Graph_gCrossSection_pt_AA_21006->GetXaxis()->SetRange(1,100);
   Graph_gCrossSection_pt_AA_21006->GetXaxis()->SetLabelFont(42);
   Graph_gCrossSection_pt_AA_21006->GetXaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_pt_AA_21006->GetXaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_pt_AA_21006->GetXaxis()->SetTitleSize(0.06);
   Graph_gCrossSection_pt_AA_21006->GetXaxis()->SetTitleOffset(0.9);
   Graph_gCrossSection_pt_AA_21006->GetXaxis()->SetTitleFont(42);
   Graph_gCrossSection_pt_AA_21006->GetYaxis()->SetLabelFont(42);
   Graph_gCrossSection_pt_AA_21006->GetYaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_pt_AA_21006->GetYaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_pt_AA_21006->GetYaxis()->SetTitleSize(0.06);
   Graph_gCrossSection_pt_AA_21006->GetYaxis()->SetTitleOffset(1.25);
   Graph_gCrossSection_pt_AA_21006->GetYaxis()->SetTitleFont(42);
   Graph_gCrossSection_pt_AA_21006->GetZaxis()->SetLabelFont(42);
   Graph_gCrossSection_pt_AA_21006->GetZaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_pt_AA_21006->GetZaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_pt_AA_21006->GetZaxis()->SetTitleSize(0.06);
   Graph_gCrossSection_pt_AA_21006->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_gCrossSection_pt_AA_21006);
   
   gre->Draw("p");
   
   TLegend *leg = new TLegend(0.62,0.61,0.83,0.71,NULL,"brNDC");
   leg->SetBorderSize(0);
   leg->SetMargin(0.2);
   leg->SetTextSize(0.04);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(4000);
   TLegendEntry *entry=leg->AddEntry(gre1," #varUpsilon(1S)","lp");

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

   ci = TColor::GetColor("#3333cc");
   entry->SetLineColor(ci);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);

   ci = TColor::GetColor("#3333cc");
   entry->SetMarkerColor(ci);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1.5);
   entry->SetTextFont(42);
   leg->Draw();
   
   leg = new TLegend(0.62,0.51,0.83,0.61,NULL,"brNDC");
   leg->SetBorderSize(0);
   leg->SetMargin(0.2);
   leg->SetTextSize(0.04);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(4000);
   entry=leg->AddEntry("NULL"," #varUpsilon(3S) 68% CL","f");

   ci = 1184;
   color = new TColor(ci, 0.4, 0.8, 0.4, " ", 0.5);
   entry->SetFillColor(ci);
   entry->SetFillStyle(1001);

   ci = TColor::GetColor("#006600");
   entry->SetLineColor(ci);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);
   entry=leg->AddEntry("NULL"," #varUpsilon(3S) 95% CL","");
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);
   leg->Draw();
   arrow = new TArrow(17.4,0.0093,17.4,0.0145,0.021,"<-|");
   arrow->SetFillColor(1);
   arrow->SetFillStyle(1001);

   ci = TColor::GetColor("#009900");
   arrow->SetLineColor(ci);
   arrow->SetLineWidth(2);
   arrow->Draw();
   TLatex *   tex = new TLatex(0.27,0.8225,"|y| < 2.4");
tex->SetNDC();
   tex->SetTextAlign(12);
   tex->SetTextFont(42);
   tex->SetTextSize(0.04);
   tex->SetLineWidth(2);
   tex->Draw();
      tex = new TLatex(0.27,0.77,"Cent. 0-100%");
tex->SetNDC();
   tex->SetTextAlign(12);
   tex->SetTextFont(42);
   tex->SetTextSize(0.04);
   tex->SetLineWidth(2);
   tex->Draw();
      tex = new TLatex(0.94,0.92,"PbPb 368 #mub^{-1} (5.02 TeV)");
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
   c1->SaveAs("Figure2_b.pdf");
}
