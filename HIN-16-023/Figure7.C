#include "tdrstyle.C"
void Figure7()
{
//=========Macro generated from canvas: c1/c1
//=========  (Mon Sep 17 16:00:39 2018) by ROOT version 6.14/04
   setTDRStyle();
   TCanvas *c1 = new TCanvas("c1", "c1",0,0,600,600);
  gPad->SetBottomMargin(0.1);
  gPad->SetTopMargin(0.067);
   gStyle->SetOptFit(1);
   gStyle->SetOptStat(0);
   gStyle->SetOptTitle(0);
   c1->Range(-0.5643564,-0.120048,2.962871,1.080432);
   c1->SetFillColor(0);
   c1->SetBorderMode(0);
   c1->SetBorderSize(2);
   c1->SetTickx(1);
   c1->SetTicky(1);
   c1->SetLeftMargin(0.16);
   c1->SetRightMargin(0.032);
   c1->SetTopMargin(0.067);
   c1->SetFrameFillStyle(0);
   c1->SetFrameBorderMode(0);
   c1->SetFrameFillStyle(0);
   c1->SetFrameBorderMode(0);
   
   Double_t _fx3001[1] = {
   0.435};
   Double_t _fy3001[1] = {
   0.453};
   Double_t _felx3001[1] = {
   0.05};
   Double_t _fely3001[1] = {
   0.046};
   Double_t _fehx3001[1] = {
   0.05};
   Double_t _fehy3001[1] = {
   0.046};
   TGraphAsymmErrors *grae = new TGraphAsymmErrors(1,_fx3001,_fy3001,_felx3001,_fehx3001,_fely3001,_fehy3001);
   grae->SetName("");
   grae->SetTitle("");

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = 1179;
   color = new TColor(ci, 0.8, 0.8, 0.8, " ", 0.5);
   grae->SetFillColor(ci);
   grae->SetFillStyle(1000);
   grae->SetMarkerStyle(20);
   
   TH1F *Graph_Graph3001 = new TH1F("Graph_Graph3001","",100,0,2.85);
   Graph_Graph3001->SetMinimum(0);
   Graph_Graph3001->SetMaximum(1);
   Graph_Graph3001->SetDirectory(0);
   Graph_Graph3001->SetStats(0);
   Graph_Graph3001->SetLineStyle(0);
   Graph_Graph3001->SetMarkerStyle(20);
   Graph_Graph3001->GetXaxis()->SetBinLabel(10,"");
   Graph_Graph3001->GetXaxis()->CenterTitle(true);
   Graph_Graph3001->GetXaxis()->SetLabelFont(42);
   Graph_Graph3001->GetXaxis()->SetLabelOffset(0.007);
   Graph_Graph3001->GetXaxis()->SetLabelSize(0.05);
   Graph_Graph3001->GetXaxis()->SetTitleSize(0.06);
   Graph_Graph3001->GetXaxis()->SetTitleOffset(0.9);
   Graph_Graph3001->GetXaxis()->SetTitleFont(42);
   Graph_Graph3001->GetYaxis()->SetTitle("R_{AA}");
   Graph_Graph3001->GetYaxis()->CenterTitle(true);
   Graph_Graph3001->GetYaxis()->SetLabelFont(42);
   Graph_Graph3001->GetYaxis()->SetLabelOffset(0.007);
   Graph_Graph3001->GetYaxis()->SetLabelSize(0.05);
   Graph_Graph3001->GetYaxis()->SetTitleSize(0.06);
   Graph_Graph3001->GetYaxis()->SetTitleOffset(1.25);
   Graph_Graph3001->GetYaxis()->SetTitleFont(42);
   Graph_Graph3001->GetZaxis()->SetLabelFont(42);
   Graph_Graph3001->GetZaxis()->SetLabelOffset(0.007);
   Graph_Graph3001->GetZaxis()->SetLabelSize(0.05);
   Graph_Graph3001->GetZaxis()->SetTitleSize(0.06);
   Graph_Graph3001->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_Graph3001);
   
   grae->Draw("a5");
   
   Double_t _fx1001[1] = {
   0.435};
   Double_t _fy1001[1] = {
   0.453};
   Double_t _fex1001[1] = {
   0};
   Double_t _fey1001[1] = {
   0.014};
   TGraphErrors *gre = new TGraphErrors(1,_fx1001,_fy1001,_fex1001,_fey1001);
   gre->SetName("");
   gre->SetTitle("");
   gre->SetFillStyle(1000);
   gre->SetMarkerStyle(24);
   gre->SetMarkerSize(1.5);
   
   TH1F *Graph_Graph1001 = new TH1F("Graph_Graph1001","",100,0.335,1.535);
   Graph_Graph1001->SetMinimum(0.4362);
   Graph_Graph1001->SetMaximum(0.4698);
   Graph_Graph1001->SetDirectory(0);
   Graph_Graph1001->SetStats(0);
   Graph_Graph1001->SetLineStyle(0);
   Graph_Graph1001->SetMarkerStyle(20);
   Graph_Graph1001->GetXaxis()->SetBinLabel(10,"");
   Graph_Graph1001->GetXaxis()->SetLabelFont(42);
   Graph_Graph1001->GetXaxis()->SetLabelOffset(0.007);
   Graph_Graph1001->GetXaxis()->SetLabelSize(0.05);
   Graph_Graph1001->GetXaxis()->SetTitleSize(0.06);
   Graph_Graph1001->GetXaxis()->SetTitleOffset(0.9);
   Graph_Graph1001->GetXaxis()->SetTitleFont(42);
   Graph_Graph1001->GetYaxis()->SetLabelFont(42);
   Graph_Graph1001->GetYaxis()->SetLabelOffset(0.007);
   Graph_Graph1001->GetYaxis()->SetLabelSize(0.05);
   Graph_Graph1001->GetYaxis()->SetTitleSize(0.06);
   Graph_Graph1001->GetYaxis()->SetTitleOffset(1.25);
   Graph_Graph1001->GetYaxis()->SetTitleFont(42);
   Graph_Graph1001->GetZaxis()->SetLabelFont(42);
   Graph_Graph1001->GetZaxis()->SetLabelOffset(0.007);
   Graph_Graph1001->GetZaxis()->SetLabelSize(0.05);
   Graph_Graph1001->GetZaxis()->SetTitleSize(0.06);
   Graph_Graph1001->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph1001);
   
   gre->Draw("p");
   auto gre1_1 = (TGraphErrors*) gre;
   
   Double_t _fx3002[1] = {
   0.585};
   Double_t _fy3002[1] = {
   0.375961};
   Double_t _felx3002[1] = {
   0.05};
   Double_t _fely3002[1] = {
   0.03472246};
   Double_t _fehx3002[1] = {
   0.05};
   Double_t _fehy3002[1] = {
   0.03395686};
   grae = new TGraphAsymmErrors(1,_fx3002,_fy3002,_felx3002,_fehx3002,_fely3002,_fehy3002);
   grae->SetName("");
   grae->SetTitle("");

   ci = 1182;
   color = new TColor(ci, 1, 0.8, 0.8, " ", 0.5);
   grae->SetFillColor(ci);
   grae->SetFillStyle(1000);

   ci = TColor::GetColor("#990033");
   grae->SetLineColor(ci);
   grae->SetMarkerStyle(20);
   
   TH1F *Graph_Graph3002 = new TH1F("Graph_Graph3002","",100,0.525,0.645);
   Graph_Graph3002->SetMinimum(0.3343706);
   Graph_Graph3002->SetMaximum(0.4167858);
   Graph_Graph3002->SetDirectory(0);
   Graph_Graph3002->SetStats(0);
   Graph_Graph3002->SetLineStyle(0);
   Graph_Graph3002->SetMarkerStyle(20);
   Graph_Graph3002->GetXaxis()->SetBinLabel(10,"");
   Graph_Graph3002->GetXaxis()->SetLabelFont(42);
   Graph_Graph3002->GetXaxis()->SetLabelOffset(0.007);
   Graph_Graph3002->GetXaxis()->SetLabelSize(0.05);
   Graph_Graph3002->GetXaxis()->SetTitleSize(0.06);
   Graph_Graph3002->GetXaxis()->SetTitleOffset(0.9);
   Graph_Graph3002->GetXaxis()->SetTitleFont(42);
   Graph_Graph3002->GetYaxis()->SetLabelFont(42);
   Graph_Graph3002->GetYaxis()->SetLabelOffset(0.007);
   Graph_Graph3002->GetYaxis()->SetLabelSize(0.05);
   Graph_Graph3002->GetYaxis()->SetTitleSize(0.06);
   Graph_Graph3002->GetYaxis()->SetTitleOffset(1.25);
   Graph_Graph3002->GetYaxis()->SetTitleFont(42);
   Graph_Graph3002->GetZaxis()->SetLabelFont(42);
   Graph_Graph3002->GetZaxis()->SetLabelOffset(0.007);
   Graph_Graph3002->GetZaxis()->SetLabelSize(0.05);
   Graph_Graph3002->GetZaxis()->SetTitleSize(0.06);
   Graph_Graph3002->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_Graph3002);
   
   grae->Draw("5");
   
   Double_t gRAA_int_fx1002[1] = {
   0.585};
   Double_t gRAA_int_fy1002[1] = {
   0.375961};
   Double_t gRAA_int_fex1002[1] = {
   0};
   Double_t gRAA_int_fey1002[1] = {
   0.01335999};
   gre = new TGraphErrors(1,gRAA_int_fx1002,gRAA_int_fy1002,gRAA_int_fex1002,gRAA_int_fey1002);
   gre->SetName("gRAA_int");
   gre->SetTitle("raa_integrated");

   ci = TColor::GetColor("#6699ff");
   gre->SetFillColor(ci);

   ci = TColor::GetColor("#990033");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#990033");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetMarkerSize(1.5);
   
   TH1F *Graph_gRAA_int1002 = new TH1F("Graph_gRAA_int1002","raa_integrated",100,0.485,1.685);
   Graph_gRAA_int1002->SetMinimum(0.359929);
   Graph_gRAA_int1002->SetMaximum(0.391993);
   Graph_gRAA_int1002->SetDirectory(0);
   Graph_gRAA_int1002->SetStats(0);
   Graph_gRAA_int1002->SetLineStyle(0);
   Graph_gRAA_int1002->SetMarkerStyle(20);
   Graph_gRAA_int1002->GetXaxis()->SetBinLabel(10,"");
   Graph_gRAA_int1002->GetXaxis()->CenterTitle(true);
   Graph_gRAA_int1002->GetXaxis()->SetLabelFont(42);
   Graph_gRAA_int1002->GetXaxis()->SetLabelOffset(999);
   Graph_gRAA_int1002->GetXaxis()->SetLabelSize(0);
   Graph_gRAA_int1002->GetXaxis()->SetTitleSize(0.035);
   Graph_gRAA_int1002->GetXaxis()->SetTitleFont(42);
   Graph_gRAA_int1002->GetYaxis()->CenterTitle(true);
   Graph_gRAA_int1002->GetYaxis()->SetLabelFont(42);
   Graph_gRAA_int1002->GetYaxis()->SetLabelSize(0.035);
   Graph_gRAA_int1002->GetYaxis()->SetTitleSize(0.035);
   Graph_gRAA_int1002->GetYaxis()->SetTitleOffset(1.25);
   Graph_gRAA_int1002->GetYaxis()->SetTitleFont(42);
   Graph_gRAA_int1002->GetZaxis()->SetLabelFont(42);
   Graph_gRAA_int1002->GetZaxis()->SetLabelOffset(0.007);
   Graph_gRAA_int1002->GetZaxis()->SetLabelSize(0.05);
   Graph_gRAA_int1002->GetZaxis()->SetTitleSize(0.06);
   Graph_gRAA_int1002->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_gRAA_int1002);
   
   gre->Draw("p");
   auto gre2_1 = (TGraphErrors*) gre;
   
   Double_t _fx3003[1] = {
   1.35};
   Double_t _fy3003[1] = {
   0.119};
   Double_t _felx3003[1] = {
   0.05};
   Double_t _fely3003[1] = {
   0.015};
   Double_t _fehx3003[1] = {
   0.05};
   Double_t _fehy3003[1] = {
   0.015};
   grae = new TGraphAsymmErrors(1,_fx3003,_fy3003,_felx3003,_fehx3003,_fely3003,_fehy3003);
   grae->SetName("");
   grae->SetTitle("");

   ci = 1180;
   color = new TColor(ci, 0.8, 0.8, 0.8, " ", 0.5);
   grae->SetFillColor(ci);
   grae->SetFillStyle(1000);
   grae->SetMarkerStyle(20);
   
   TH1F *Graph_Graph3003 = new TH1F("Graph_Graph3003","",100,1.29,1.41);
   Graph_Graph3003->SetMinimum(0.101);
   Graph_Graph3003->SetMaximum(0.137);
   Graph_Graph3003->SetDirectory(0);
   Graph_Graph3003->SetStats(0);
   Graph_Graph3003->SetLineStyle(0);
   Graph_Graph3003->SetMarkerStyle(20);
   Graph_Graph3003->GetXaxis()->SetBinLabel(10,"");
   Graph_Graph3003->GetXaxis()->SetLabelFont(42);
   Graph_Graph3003->GetXaxis()->SetLabelOffset(0.007);
   Graph_Graph3003->GetXaxis()->SetLabelSize(0.05);
   Graph_Graph3003->GetXaxis()->SetTitleSize(0.06);
   Graph_Graph3003->GetXaxis()->SetTitleOffset(0.9);
   Graph_Graph3003->GetXaxis()->SetTitleFont(42);
   Graph_Graph3003->GetYaxis()->SetLabelFont(42);
   Graph_Graph3003->GetYaxis()->SetLabelOffset(0.007);
   Graph_Graph3003->GetYaxis()->SetLabelSize(0.05);
   Graph_Graph3003->GetYaxis()->SetTitleSize(0.06);
   Graph_Graph3003->GetYaxis()->SetTitleOffset(1.25);
   Graph_Graph3003->GetYaxis()->SetTitleFont(42);
   Graph_Graph3003->GetZaxis()->SetLabelFont(42);
   Graph_Graph3003->GetZaxis()->SetLabelOffset(0.007);
   Graph_Graph3003->GetZaxis()->SetLabelSize(0.05);
   Graph_Graph3003->GetZaxis()->SetTitleSize(0.06);
   Graph_Graph3003->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_Graph3003);
   
   grae->Draw("5");
   
   Double_t _fx1003[1] = {
   1.35};
   Double_t _fy1003[1] = {
   0.119};
   Double_t _fex1003[1] = {
   0};
   Double_t _fey1003[1] = {
   0.028};
   gre = new TGraphErrors(1,_fx1003,_fy1003,_fex1003,_fey1003);
   gre->SetName("");
   gre->SetTitle("");
   gre->SetFillStyle(1000);
   gre->SetMarkerStyle(25);
   gre->SetMarkerSize(1.5);
   
   TH1F *Graph_Graph1003 = new TH1F("Graph_Graph1003","",100,1.25,2.45);
   Graph_Graph1003->SetMinimum(0.0854);
   Graph_Graph1003->SetMaximum(0.1526);
   Graph_Graph1003->SetDirectory(0);
   Graph_Graph1003->SetStats(0);
   Graph_Graph1003->SetLineStyle(0);
   Graph_Graph1003->SetMarkerStyle(20);
   Graph_Graph1003->GetXaxis()->SetBinLabel(10,"");
   Graph_Graph1003->GetXaxis()->SetLabelFont(42);
   Graph_Graph1003->GetXaxis()->SetLabelOffset(0.007);
   Graph_Graph1003->GetXaxis()->SetLabelSize(0.05);
   Graph_Graph1003->GetXaxis()->SetTitleSize(0.06);
   Graph_Graph1003->GetXaxis()->SetTitleOffset(0.9);
   Graph_Graph1003->GetXaxis()->SetTitleFont(42);
   Graph_Graph1003->GetYaxis()->SetLabelFont(42);
   Graph_Graph1003->GetYaxis()->SetLabelOffset(0.007);
   Graph_Graph1003->GetYaxis()->SetLabelSize(0.05);
   Graph_Graph1003->GetYaxis()->SetTitleSize(0.06);
   Graph_Graph1003->GetYaxis()->SetTitleOffset(1.25);
   Graph_Graph1003->GetYaxis()->SetTitleFont(42);
   Graph_Graph1003->GetZaxis()->SetLabelFont(42);
   Graph_Graph1003->GetZaxis()->SetLabelOffset(0.007);
   Graph_Graph1003->GetZaxis()->SetLabelSize(0.05);
   Graph_Graph1003->GetZaxis()->SetTitleSize(0.06);
   Graph_Graph1003->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph1003);
   
   gre->Draw("p");
   auto gre1_2 = (TGraphErrors*) gre;
   
   Double_t _fx3004[1] = {
   1.5};
   Double_t _fy3004[1] = {
   0.1165341};
   Double_t _felx3004[1] = {
   0.05};
   Double_t _fely3004[1] = {
   0.01937991};
   Double_t _fehx3004[1] = {
   0.05};
   Double_t _fehy3004[1] = {
   0.01924914};
   grae = new TGraphAsymmErrors(1,_fx3004,_fy3004,_felx3004,_fehx3004,_fely3004,_fehy3004);
   grae->SetName("");
   grae->SetTitle("");

   ci = 1183;
   color = new TColor(ci, 0.8, 0.8, 1, " ", 0.5);
   grae->SetFillColor(ci);
   grae->SetFillStyle(1000);

   ci = TColor::GetColor("#3333cc");
   grae->SetLineColor(ci);
   grae->SetMarkerStyle(20);
   
   TH1F *Graph_Graph3004 = new TH1F("Graph_Graph3004","",100,1.44,1.56);
   Graph_Graph3004->SetMinimum(0.09329133);
   Graph_Graph3004->SetMaximum(0.1396462);
   Graph_Graph3004->SetDirectory(0);
   Graph_Graph3004->SetStats(0);
   Graph_Graph3004->SetLineStyle(0);
   Graph_Graph3004->SetMarkerStyle(20);
   Graph_Graph3004->GetXaxis()->SetBinLabel(10,"");
   Graph_Graph3004->GetXaxis()->SetLabelFont(42);
   Graph_Graph3004->GetXaxis()->SetLabelOffset(0.007);
   Graph_Graph3004->GetXaxis()->SetLabelSize(0.05);
   Graph_Graph3004->GetXaxis()->SetTitleSize(0.06);
   Graph_Graph3004->GetXaxis()->SetTitleOffset(0.9);
   Graph_Graph3004->GetXaxis()->SetTitleFont(42);
   Graph_Graph3004->GetYaxis()->SetLabelFont(42);
   Graph_Graph3004->GetYaxis()->SetLabelOffset(0.007);
   Graph_Graph3004->GetYaxis()->SetLabelSize(0.05);
   Graph_Graph3004->GetYaxis()->SetTitleSize(0.06);
   Graph_Graph3004->GetYaxis()->SetTitleOffset(1.25);
   Graph_Graph3004->GetYaxis()->SetTitleFont(42);
   Graph_Graph3004->GetZaxis()->SetLabelFont(42);
   Graph_Graph3004->GetZaxis()->SetLabelOffset(0.007);
   Graph_Graph3004->GetZaxis()->SetLabelSize(0.05);
   Graph_Graph3004->GetZaxis()->SetTitleSize(0.06);
   Graph_Graph3004->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_Graph3004);
   
   grae->Draw("5");
   
   Double_t gRAA_int_fx1004[1] = {
   1.5};
   Double_t gRAA_int_fy1004[1] = {
   0.1165341};
   Double_t gRAA_int_fex1004[1] = {
   0};
   Double_t gRAA_int_fey1004[1] = {
   0.02168668};
   gre = new TGraphErrors(1,gRAA_int_fx1004,gRAA_int_fy1004,gRAA_int_fex1004,gRAA_int_fey1004);
   gre->SetName("gRAA_int");
   gre->SetTitle("raa_integrated");

   ci = TColor::GetColor("#6699ff");
   gre->SetFillColor(ci);

   ci = TColor::GetColor("#3333cc");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#3333cc");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(21);
   gre->SetMarkerSize(1.5);
   
   TH1F *Graph_gRAA_int1004 = new TH1F("Graph_gRAA_int1004","raa_integrated",100,1.4,2.6);
   Graph_gRAA_int1004->SetMinimum(0.09051013);
   Graph_gRAA_int1004->SetMaximum(0.1425582);
   Graph_gRAA_int1004->SetDirectory(0);
   Graph_gRAA_int1004->SetStats(0);
   Graph_gRAA_int1004->SetLineStyle(0);
   Graph_gRAA_int1004->SetMarkerStyle(20);
   Graph_gRAA_int1004->GetXaxis()->SetBinLabel(10,"");
   Graph_gRAA_int1004->GetXaxis()->CenterTitle(true);
   Graph_gRAA_int1004->GetXaxis()->SetLabelFont(42);
   Graph_gRAA_int1004->GetXaxis()->SetLabelOffset(999);
   Graph_gRAA_int1004->GetXaxis()->SetLabelSize(0);
   Graph_gRAA_int1004->GetXaxis()->SetTitleSize(0.035);
   Graph_gRAA_int1004->GetXaxis()->SetTitleFont(42);
   Graph_gRAA_int1004->GetYaxis()->CenterTitle(true);
   Graph_gRAA_int1004->GetYaxis()->SetLabelFont(42);
   Graph_gRAA_int1004->GetYaxis()->SetLabelSize(0.035);
   Graph_gRAA_int1004->GetYaxis()->SetTitleSize(0.035);
   Graph_gRAA_int1004->GetYaxis()->SetTitleOffset(1.25);
   Graph_gRAA_int1004->GetYaxis()->SetTitleFont(42);
   Graph_gRAA_int1004->GetZaxis()->SetLabelFont(42);
   Graph_gRAA_int1004->GetZaxis()->SetLabelOffset(0.007);
   Graph_gRAA_int1004->GetZaxis()->SetLabelSize(0.05);
   Graph_gRAA_int1004->GetZaxis()->SetTitleSize(0.06);
   Graph_gRAA_int1004->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_gRAA_int1004);
   
   gre->Draw("p");
   auto gre2_2 = (TGraphErrors*) gre;
   
   Double_t _fx3005[1] = {
   -100};
   Double_t _fy3005[1] = {
   -100};
   Double_t _felx3005[1] = {
   0.05};
   Double_t _fely3005[1] = {
   0.058};
   Double_t _fehx3005[1] = {
   0.05};
   Double_t _fehy3005[1] = {
   0.058};
   grae = new TGraphAsymmErrors(1,_fx3005,_fy3005,_felx3005,_fehx3005,_fely3005,_fehy3005);
   grae->SetName("");
   grae->SetTitle("");

   ci = 1181;
   color = new TColor(ci, 0.8, 0.8, 0.8, " ", 0.5);
   grae->SetFillColor(ci);
   grae->SetFillStyle(1000);
   grae->SetMarkerStyle(20);
   
   TH1F *Graph_Graph3005 = new TH1F("Graph_Graph3005","",100,-100.06,-99.94);
   Graph_Graph3005->SetMinimum(-100.0696);
   Graph_Graph3005->SetMaximum(-99.9304);
   Graph_Graph3005->SetDirectory(0);
   Graph_Graph3005->SetStats(0);
   Graph_Graph3005->SetLineStyle(0);
   Graph_Graph3005->SetMarkerStyle(20);
   Graph_Graph3005->GetXaxis()->SetBinLabel(10,"");
   Graph_Graph3005->GetXaxis()->SetLabelFont(42);
   Graph_Graph3005->GetXaxis()->SetLabelOffset(0.007);
   Graph_Graph3005->GetXaxis()->SetLabelSize(0.05);
   Graph_Graph3005->GetXaxis()->SetTitleSize(0.06);
   Graph_Graph3005->GetXaxis()->SetTitleOffset(0.9);
   Graph_Graph3005->GetXaxis()->SetTitleFont(42);
   Graph_Graph3005->GetYaxis()->SetLabelFont(42);
   Graph_Graph3005->GetYaxis()->SetLabelOffset(0.007);
   Graph_Graph3005->GetYaxis()->SetLabelSize(0.05);
   Graph_Graph3005->GetYaxis()->SetTitleSize(0.06);
   Graph_Graph3005->GetYaxis()->SetTitleOffset(1.25);
   Graph_Graph3005->GetYaxis()->SetTitleFont(42);
   Graph_Graph3005->GetZaxis()->SetLabelFont(42);
   Graph_Graph3005->GetZaxis()->SetLabelOffset(0.007);
   Graph_Graph3005->GetZaxis()->SetLabelSize(0.05);
   Graph_Graph3005->GetZaxis()->SetTitleSize(0.06);
   Graph_Graph3005->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_Graph3005);
   
   grae->Draw("5");
   
   Double_t _fx1005[1] = {
   -100};
   Double_t _fy1005[1] = {
   -100};
   Double_t _fex1005[1] = {
   0};
   Double_t _fey1005[1] = {
   0.031};
   gre = new TGraphErrors(1,_fx1005,_fy1005,_fex1005,_fey1005);
   gre->SetName("");
   gre->SetTitle("");
   gre->SetFillStyle(1000);
   gre->SetMarkerStyle(27);
   gre->SetMarkerSize(1.5);
   
   TH1F *Graph_Graph1005 = new TH1F("Graph_Graph1005","",100,-100.1,-98.9);
   Graph_Graph1005->SetMinimum(-100.0372);
   Graph_Graph1005->SetMaximum(-99.9628);
   Graph_Graph1005->SetDirectory(0);
   Graph_Graph1005->SetStats(0);
   Graph_Graph1005->SetLineStyle(0);
   Graph_Graph1005->SetMarkerStyle(20);
   Graph_Graph1005->GetXaxis()->SetBinLabel(10,"");
   Graph_Graph1005->GetXaxis()->SetLabelFont(42);
   Graph_Graph1005->GetXaxis()->SetLabelOffset(0.007);
   Graph_Graph1005->GetXaxis()->SetLabelSize(0.05);
   Graph_Graph1005->GetXaxis()->SetTitleSize(0.06);
   Graph_Graph1005->GetXaxis()->SetTitleOffset(0.9);
   Graph_Graph1005->GetXaxis()->SetTitleFont(42);
   Graph_Graph1005->GetYaxis()->SetLabelFont(42);
   Graph_Graph1005->GetYaxis()->SetLabelOffset(0.007);
   Graph_Graph1005->GetYaxis()->SetLabelSize(0.05);
   Graph_Graph1005->GetYaxis()->SetTitleSize(0.06);
   Graph_Graph1005->GetYaxis()->SetTitleOffset(1.25);
   Graph_Graph1005->GetYaxis()->SetTitleFont(42);
   Graph_Graph1005->GetZaxis()->SetLabelFont(42);
   Graph_Graph1005->GetZaxis()->SetLabelOffset(0.007);
   Graph_Graph1005->GetZaxis()->SetLabelSize(0.05);
   Graph_Graph1005->GetZaxis()->SetTitleSize(0.06);
   Graph_Graph1005->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph1005);
   
   gre->Draw("p");
   auto gre1_3 = (TGraphErrors*) gre;
   
   Double_t _fx3006[1] = {
   -100};
   Double_t _fy3006[1] = {
   -100};
   Double_t _felx3006[1] = {
   0.05};
   Double_t _fely3006[1] = {
   0.01596587};
   Double_t _fehx3006[1] = {
   0.05};
   Double_t _fehy3006[1] = {
   0.0159604};
   grae = new TGraphAsymmErrors(1,_fx3006,_fy3006,_felx3006,_fehx3006,_fely3006,_fehy3006);
   grae->SetName("");
   grae->SetTitle("");

   ci = 1184;
   color = new TColor(ci, 0.8, 1, 0.8, " ", 0.5);
   grae->SetFillColor(ci);
   grae->SetFillStyle(1000);

   ci = TColor::GetColor("#009900");
   grae->SetLineColor(ci);
   grae->SetMarkerStyle(20);
   
   TH1F *Graph_Graph3006 = new TH1F("Graph_Graph3006","",100,-100.06,-99.94);
   Graph_Graph3006->SetMinimum(-100.0192);
   Graph_Graph3006->SetMaximum(-99.98085);
   Graph_Graph3006->SetDirectory(0);
   Graph_Graph3006->SetStats(0);
   Graph_Graph3006->SetLineStyle(0);
   Graph_Graph3006->SetMarkerStyle(20);
   Graph_Graph3006->GetXaxis()->SetBinLabel(10,"");
   Graph_Graph3006->GetXaxis()->SetLabelFont(42);
   Graph_Graph3006->GetXaxis()->SetLabelOffset(0.007);
   Graph_Graph3006->GetXaxis()->SetLabelSize(0.05);
   Graph_Graph3006->GetXaxis()->SetTitleSize(0.06);
   Graph_Graph3006->GetXaxis()->SetTitleOffset(0.9);
   Graph_Graph3006->GetXaxis()->SetTitleFont(42);
   Graph_Graph3006->GetYaxis()->SetLabelFont(42);
   Graph_Graph3006->GetYaxis()->SetLabelOffset(0.007);
   Graph_Graph3006->GetYaxis()->SetLabelSize(0.05);
   Graph_Graph3006->GetYaxis()->SetTitleSize(0.06);
   Graph_Graph3006->GetYaxis()->SetTitleOffset(1.25);
   Graph_Graph3006->GetYaxis()->SetTitleFont(42);
   Graph_Graph3006->GetZaxis()->SetLabelFont(42);
   Graph_Graph3006->GetZaxis()->SetLabelOffset(0.007);
   Graph_Graph3006->GetZaxis()->SetLabelSize(0.05);
   Graph_Graph3006->GetZaxis()->SetTitleSize(0.06);
   Graph_Graph3006->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_Graph3006);
   
   grae->Draw("5");
   
   Double_t gRAA_int_fx1006[1] = {
   -100};
   Double_t gRAA_int_fy1006[1] = {
   -100};
   Double_t gRAA_int_fex1006[1] = {
   0};
   Double_t gRAA_int_fey1006[1] = {
   0.03838972};
   gre = new TGraphErrors(1,gRAA_int_fx1006,gRAA_int_fy1006,gRAA_int_fex1006,gRAA_int_fey1006);
   gre->SetName("gRAA_int");
   gre->SetTitle("raa_integrated");

   ci = TColor::GetColor("#6699ff");
   gre->SetFillColor(ci);

   ci = TColor::GetColor("#009900");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#009900");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(33);
   gre->SetMarkerSize(2.6);
   
   TH1F *Graph_gRAA_int1006 = new TH1F("Graph_gRAA_int1006","raa_integrated",100,-100.1,-98.9);
   Graph_gRAA_int1006->SetMinimum(-100.0461);
   Graph_gRAA_int1006->SetMaximum(-99.95393);
   Graph_gRAA_int1006->SetDirectory(0);
   Graph_gRAA_int1006->SetStats(0);
   Graph_gRAA_int1006->SetLineStyle(0);
   Graph_gRAA_int1006->SetMarkerStyle(20);
   Graph_gRAA_int1006->GetXaxis()->SetBinLabel(10,"");
   Graph_gRAA_int1006->GetXaxis()->CenterTitle(true);
   Graph_gRAA_int1006->GetXaxis()->SetLabelFont(42);
   Graph_gRAA_int1006->GetXaxis()->SetLabelOffset(999);
   Graph_gRAA_int1006->GetXaxis()->SetLabelSize(0);
   Graph_gRAA_int1006->GetXaxis()->SetTitleSize(0.035);
   Graph_gRAA_int1006->GetXaxis()->SetTitleFont(42);
   Graph_gRAA_int1006->GetYaxis()->CenterTitle(true);
   Graph_gRAA_int1006->GetYaxis()->SetLabelFont(42);
   Graph_gRAA_int1006->GetYaxis()->SetLabelSize(0.035);
   Graph_gRAA_int1006->GetYaxis()->SetTitleSize(0.035);
   Graph_gRAA_int1006->GetYaxis()->SetTitleOffset(1.25);
   Graph_gRAA_int1006->GetYaxis()->SetTitleFont(42);
   Graph_gRAA_int1006->GetZaxis()->SetLabelFont(42);
   Graph_gRAA_int1006->GetZaxis()->SetLabelOffset(0.007);
   Graph_gRAA_int1006->GetZaxis()->SetLabelSize(0.05);
   Graph_gRAA_int1006->GetZaxis()->SetTitleSize(0.06);
   Graph_gRAA_int1006->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_gRAA_int1006);
   
   gre->Draw("p");
   auto gre2_3 = (TGraphErrors*) gre;
   TArrow *arrow = new TArrow(2.3205,0,2.3205,0.145,0.027,"<-|");
   arrow->SetFillColor(1);
   arrow->SetFillStyle(1001);
   arrow->SetLineWidth(2);
   arrow->Draw();
   arrow = new TArrow(2.4705,0,2.4705,0.09626922,0.027,"<-|");
   arrow->SetFillColor(1);
   arrow->SetFillStyle(1001);

   ci = TColor::GetColor("#009900");
   arrow->SetLineColor(ci);
   arrow->SetLineWidth(2);
   arrow->Draw();
   TLine *line = new TLine(0,1,2.85,1);
   line->SetLineStyle(7);
   line->Draw();
   
   TLegend *leg = new TLegend(0.474,0.6,0.904,0.87,NULL,"brNDC");
  leg->SetNColumns(2);
//  leg -> SetHeader("");
   leg->SetMargin(0.2);
   leg->SetBorderSize(0);
   leg->SetTextSize(0.04);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(4000);
   TLegendEntry *entry=leg->AddEntry("NULL","","h");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry->SetTextFont(62);
   entry->SetTextSize(0.046);
   entry=leg->AddEntry(gre2_1," ","lp");

   ci = TColor::GetColor("#990033");
   entry->SetLineColor(ci);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);

   ci = TColor::GetColor("#990033");
   entry->SetMarkerColor(ci);
   entry->SetMarkerStyle(20);
   entry->SetMarkerSize(1.5);
   entry->SetTextFont(42);
   entry=leg->AddEntry(gre2_2,"      #sqrt{s_{NN}} = 5.02 TeV","lp");

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
   arrow = new TArrow(1.55,0.655,1.55,0.697,0.02,"<-|");
   arrow->SetFillColor(1);
   arrow->SetFillStyle(1001);

   ci = TColor::GetColor("#009900");
   arrow->SetLineColor(ci);
   arrow->SetLineWidth(2);
   arrow->Draw();
   
   arrow = new TArrow(1.55,0.655,1.55,0.697,0.02,"<-|");
   arrow->SetFillColor(1);
   arrow->SetFillStyle(1001);

   ci = TColor::GetColor("#009900");
   arrow->SetLineColor(ci);
   arrow->SetLineWidth(2);
   arrow->Draw();
   
   leg = new TLegend(0.474,0.545,0.904,0.735,NULL,"brNDC");
  leg->SetNColumns(2);
//  leg -> SetHeader("");
   leg->SetMargin(0.2);
   leg->SetBorderSize(0);
   leg->SetTextSize(0.04);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(4000);
   entry=leg->AddEntry("NULL","","h");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry->SetTextFont(62);
   entry->SetTextSize(0.046);
   entry=leg->AddEntry(gre1_1," ","lp");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(24);
   entry->SetMarkerSize(1.5);
   entry->SetTextFont(42);
   entry=leg->AddEntry(gre1_2,"      #sqrt{s_{NN}} = 2.76 TeV","lp");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(25);
   entry->SetMarkerSize(1.5);
   entry->SetTextFont(42);
   leg->Draw();
   arrow = new TArrow(1.55,0.57,1.55,0.61,0.02,"<-|");
   arrow->SetFillColor(1);
   arrow->SetFillStyle(1001);
   arrow->SetLineWidth(2);
   arrow->Draw();
   TLatex *   tex = new TLatex(0.79,0.25475,"95% CL");
tex->SetNDC();
   tex->SetTextAlign(12);
   tex->SetTextFont(42);
   tex->SetTextSize(0.04);
   tex->SetLineWidth(2);
   tex->Draw();
      tex = new TLatex(0.22,0.87,"p_{T} < 30 GeV");
tex->SetNDC();
   tex->SetTextAlign(12);
   tex->SetTextFont(42);
   tex->SetTextSize(0.04);
   tex->SetLineWidth(2);
   tex->Draw();
      tex = new TLatex(0.22,0.8095,"|y| < 2.4");
tex->SetNDC();
   tex->SetTextAlign(12);
   tex->SetTextFont(42);
   tex->SetTextSize(0.04);
   tex->SetLineWidth(2);
   tex->Draw();
      tex = new TLatex(0.22,0.7523,"Cent. 0-100%");
tex->SetNDC();
   tex->SetTextAlign(12);
   tex->SetTextFont(42);
   tex->SetTextSize(0.04);
   tex->SetLineWidth(2);
   tex->Draw();
      tex = new TLatex(0.243,0.05466,"#varUpsilon(1S)");
tex->SetNDC();
   tex->SetTextAlign(12);
   tex->SetTextFont(42);
   tex->SetTextSize(0.047);
   tex->SetLineWidth(2);
   tex->Draw();
      tex = new TLatex(0.505,0.05466,"#varUpsilon(2S)");
tex->SetNDC();
   tex->SetTextAlign(12);
   tex->SetTextFont(42);
   tex->SetTextSize(0.047);
   tex->SetLineWidth(2);
   tex->Draw();
      tex = new TLatex(0.782,0.05466,"#varUpsilon(3S)");
tex->SetNDC();
   tex->SetTextAlign(12);
   tex->SetTextFont(42);
   tex->SetTextSize(0.047);
   tex->SetLineWidth(2);
   tex->Draw();
      tex = new TLatex(0.968,0.945264,"PbPb 368 #mub^{-1}, pp 28.0 pb^{-1} (5.02 TeV)");
tex->SetNDC();
   tex->SetTextAlign(31);
   tex->SetTextFont(42);
   tex->SetTextSize(0.045828);
   tex->SetLineWidth(2);
   tex->Draw();
      tex = new TLatex(0.92164,0.8993622,"CMS");
tex->SetNDC();
   tex->SetTextAlign(33);
   tex->SetTextFont(61);
   tex->SetTextSize(0.057285);
   tex->SetLineWidth(2);
   tex->Draw();
   c1->Modified();
   c1->cd();
   c1->SetSelected(c1);
   c1->SaveAs("Figure7.pdf");
}
