#include "tdrstyle.C"
void Figure4_b()
{
//=========Macro generated from canvas: c1/c1
//=========  (Fri Sep 14 14:22:20 2018) by ROOT version 6.14/04
   setTDRStyle();
   TCanvas *c1 = new TCanvas("c1", "c1",0,23,600,600);
   gStyle->SetOptFit(1);
   gStyle->SetOptStat(0);
   gStyle->SetOptTitle(0);
   c1->Range(-0.4752475,-0.201261,2.49505,1.236318);
   c1->SetFillColor(0);
   c1->SetBorderMode(0);
   c1->SetBorderSize(2);
   c1->SetTickx(1);
   c1->SetTicky(1);
   c1->SetLeftMargin(0.16);
   c1->SetRightMargin(0.032);
   c1->SetTopMargin(0.067);
   c1->SetBottomMargin(0.14);
   c1->SetFrameFillStyle(0);
   c1->SetFrameBorderMode(0);
   c1->SetFrameFillStyle(0);
   c1->SetFrameBorderMode(0);
   
   Double_t gRAA_rap_fx1001[6] = {
   0.2,
   0.6,
   1,
   1.4,
   1.8,
   2.2};
   Double_t gRAA_rap_fy1001[6] = {
   0.3831062,
   0.381457,
   0.3936485,
   0.361707,
   0.3782128,
   0.4149587};
   Double_t gRAA_rap_fex1001[6] = {
   0.2,
   0.2,
   0.2,
   0.2,
   0.2,
   0.2};
   Double_t gRAA_rap_fey1001[6] = {
   0.02976265,
   0.02413394,
   0.03599151,
   0.03102483,
   0.02623905,
   0.06596358};
   TGraphErrors *gre = new TGraphErrors(6,gRAA_rap_fx1001,gRAA_rap_fy1001,gRAA_rap_fex1001,gRAA_rap_fey1001);
   gre->SetName("gRAA_rap");
   gre->SetTitle("raa_vs_rap");

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = 1181;
   color = new TColor(ci, 1, 0.8, 0.8, " ", 0.5);
   gre->SetFillColor(ci);

   ci = TColor::GetColor("#990033");
   gre->SetLineColor(ci);
   gre->SetMarkerColor(2);
   gre->SetMarkerStyle(20);
   
   TH1F *Graph_gRAA_rap1001 = new TH1F("Graph_gRAA_rap1001","raa_vs_rap",100,0,2.4);
   Graph_gRAA_rap1001->SetMinimum(0);
   Graph_gRAA_rap1001->SetMaximum(1.14);
   Graph_gRAA_rap1001->SetDirectory(0);
   Graph_gRAA_rap1001->SetStats(0);
   Graph_gRAA_rap1001->SetLineStyle(0);
   Graph_gRAA_rap1001->SetMarkerStyle(20);
   Graph_gRAA_rap1001->GetXaxis()->SetTitle("|y|");
   Graph_gRAA_rap1001->GetXaxis()->CenterTitle(true);
   Graph_gRAA_rap1001->GetXaxis()->SetNdivisions(505);
   Graph_gRAA_rap1001->GetXaxis()->SetLabelFont(42);
   Graph_gRAA_rap1001->GetXaxis()->SetLabelOffset(0.007);
   Graph_gRAA_rap1001->GetXaxis()->SetLabelSize(0.05);
   Graph_gRAA_rap1001->GetXaxis()->SetTitleSize(0.06);
   Graph_gRAA_rap1001->GetXaxis()->SetTitleOffset(0.9);
   Graph_gRAA_rap1001->GetXaxis()->SetTitleFont(42);
   Graph_gRAA_rap1001->GetYaxis()->SetTitle("R_{AA}");
   Graph_gRAA_rap1001->GetYaxis()->CenterTitle(true);
   Graph_gRAA_rap1001->GetYaxis()->SetLabelFont(42);
   Graph_gRAA_rap1001->GetYaxis()->SetLabelOffset(0.007);
   Graph_gRAA_rap1001->GetYaxis()->SetLabelSize(0.05);
   Graph_gRAA_rap1001->GetYaxis()->SetTitleSize(0.06);
   Graph_gRAA_rap1001->GetYaxis()->SetTitleOffset(1.25);
   Graph_gRAA_rap1001->GetYaxis()->SetTitleFont(42);
   Graph_gRAA_rap1001->GetZaxis()->SetLabelFont(42);
   Graph_gRAA_rap1001->GetZaxis()->SetLabelOffset(0.007);
   Graph_gRAA_rap1001->GetZaxis()->SetLabelSize(0.05);
   Graph_gRAA_rap1001->GetZaxis()->SetTitleSize(0.06);
   Graph_gRAA_rap1001->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_gRAA_rap1001);
   
   gre->Draw("a5");
   
   Double_t gRAA_rap_fx1002[3] = {
   0.4,
   1.2,
   2};
   Double_t gRAA_rap_fy1002[3] = {
   0.1114396,
   0.1303647,
   0.07022626};
   Double_t gRAA_rap_fex1002[3] = {
   0.4,
   0.4,
   0.4};
   Double_t gRAA_rap_fey1002[3] = {
   0.009479943,
   0.02650891,
   0.01815395};
   gre = new TGraphErrors(3,gRAA_rap_fx1002,gRAA_rap_fy1002,gRAA_rap_fex1002,gRAA_rap_fey1002);
   gre->SetName("gRAA_rap");
   gre->SetTitle("raa_vs_rap");

   ci = 1182;
   color = new TColor(ci, 0.8, 0.8, 1, " ", 0.5);
   gre->SetFillColor(ci);

   ci = TColor::GetColor("#3333cc");
   gre->SetLineColor(ci);
   gre->SetMarkerColor(2);
   gre->SetMarkerStyle(20);
   
   TH1F *Graph_gRAA_rap1002 = new TH1F("Graph_gRAA_rap1002","raa_vs_rap",100,0,2.64);
   Graph_gRAA_rap1002->SetMinimum(0.04159219);
   Graph_gRAA_rap1002->SetMaximum(0.1673537);
   Graph_gRAA_rap1002->SetDirectory(0);
   Graph_gRAA_rap1002->SetStats(0);
   Graph_gRAA_rap1002->SetLineStyle(0);
   Graph_gRAA_rap1002->SetMarkerStyle(20);
   Graph_gRAA_rap1002->GetXaxis()->SetLabelFont(42);
   Graph_gRAA_rap1002->GetXaxis()->SetLabelOffset(0.007);
   Graph_gRAA_rap1002->GetXaxis()->SetLabelSize(0.05);
   Graph_gRAA_rap1002->GetXaxis()->SetTitleSize(0.06);
   Graph_gRAA_rap1002->GetXaxis()->SetTitleOffset(0.9);
   Graph_gRAA_rap1002->GetXaxis()->SetTitleFont(42);
   Graph_gRAA_rap1002->GetYaxis()->SetLabelFont(42);
   Graph_gRAA_rap1002->GetYaxis()->SetLabelOffset(0.007);
   Graph_gRAA_rap1002->GetYaxis()->SetLabelSize(0.05);
   Graph_gRAA_rap1002->GetYaxis()->SetTitleSize(0.06);
   Graph_gRAA_rap1002->GetYaxis()->SetTitleOffset(1.25);
   Graph_gRAA_rap1002->GetYaxis()->SetTitleFont(42);
   Graph_gRAA_rap1002->GetZaxis()->SetLabelFont(42);
   Graph_gRAA_rap1002->GetZaxis()->SetLabelOffset(0.007);
   Graph_gRAA_rap1002->GetZaxis()->SetLabelSize(0.05);
   Graph_gRAA_rap1002->GetZaxis()->SetTitleSize(0.06);
   Graph_gRAA_rap1002->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_gRAA_rap1002);
   
   gre->Draw("5");
   TBox *box = new TBox(0.55,0,0.65,0.05892339);

   ci = 1179;
   color = new TColor(ci, 0.4, 0.8, 0.4, " ", 0.5);
   box->SetFillColor(ci);

   ci = TColor::GetColor("#006600");
   box->SetLineColor(ci);
   box->Draw("l");
   box = new TBox(1.75,0,1.85,0.06993767);

   ci = 1180;
   color = new TColor(ci, 0.4, 0.8, 0.4, " ", 0.5);
   box->SetFillColor(ci);

   ci = TColor::GetColor("#006600");
   box->SetLineColor(ci);
   box->Draw("l");
   
   Double_t gRAA_rap_fx1003[3] = {
   -10,
   -11,
   -12};
   Double_t gRAA_rap_fy1003[3] = {
   -10,
   -11,
   -12};
   Double_t gRAA_rap_fex1003[3] = {
   0,
   0,
   0};
   Double_t gRAA_rap_fey1003[3] = {
   0,
   0,
   0};
   gre = new TGraphErrors(3,gRAA_rap_fx1003,gRAA_rap_fy1003,gRAA_rap_fex1003,gRAA_rap_fey1003);
   gre->SetName("gRAA_rap");
   gre->SetTitle("raa_vs_rap");

   ci = 1183;
   color = new TColor(ci, 0.8, 1, 0.8, " ", 0.5);
   gre->SetFillColor(ci);

   ci = TColor::GetColor("#009900");
   gre->SetLineColor(ci);
   gre->SetMarkerColor(2);
   gre->SetMarkerStyle(20);
   
   TH1F *Graph_gRAA_rap1003 = new TH1F("Graph_gRAA_rap1003","raa_vs_rap",100,0,30);
   Graph_gRAA_rap1003->SetMinimum(0);
   Graph_gRAA_rap1003->SetMaximum(1.3);
   Graph_gRAA_rap1003->SetDirectory(0);
   Graph_gRAA_rap1003->SetStats(0);
   Graph_gRAA_rap1003->SetLineStyle(0);
   Graph_gRAA_rap1003->SetMarkerStyle(20);
   Graph_gRAA_rap1003->GetXaxis()->SetRange(1,100);
   Graph_gRAA_rap1003->GetXaxis()->SetLabelFont(42);
   Graph_gRAA_rap1003->GetXaxis()->SetLabelOffset(0.007);
   Graph_gRAA_rap1003->GetXaxis()->SetLabelSize(0.05);
   Graph_gRAA_rap1003->GetXaxis()->SetTitleSize(0.06);
   Graph_gRAA_rap1003->GetXaxis()->SetTitleOffset(0.9);
   Graph_gRAA_rap1003->GetXaxis()->SetTitleFont(42);
   Graph_gRAA_rap1003->GetYaxis()->SetLabelFont(42);
   Graph_gRAA_rap1003->GetYaxis()->SetLabelOffset(0.007);
   Graph_gRAA_rap1003->GetYaxis()->SetLabelSize(0.05);
   Graph_gRAA_rap1003->GetYaxis()->SetTitleSize(0.06);
   Graph_gRAA_rap1003->GetYaxis()->SetTitleOffset(1.25);
   Graph_gRAA_rap1003->GetYaxis()->SetTitleFont(42);
   Graph_gRAA_rap1003->GetZaxis()->SetLabelFont(42);
   Graph_gRAA_rap1003->GetZaxis()->SetLabelOffset(0.007);
   Graph_gRAA_rap1003->GetZaxis()->SetLabelSize(0.05);
   Graph_gRAA_rap1003->GetZaxis()->SetTitleSize(0.06);
   Graph_gRAA_rap1003->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_gRAA_rap1003);
   
   gre->Draw("5");
   
   Double_t gRAA_rap_0s_fx1004[6] = {
   0.2,
   0.6,
   1,
   1.4,
   1.8,
   2.2};
   Double_t gRAA_rap_0s_fy1004[6] = {
   0.3831062,
   0.381457,
   0.3936485,
   0.361707,
   0.3782128,
   0.4149587};
   Double_t gRAA_rap_0s_fex1004[6] = {
   0,
   0,
   0,
   0,
   0,
   0};
   Double_t gRAA_rap_0s_fey1004[6] = {
   0.02238725,
   0.0254519,
   0.03098862,
   0.02895736,
   0.03571978,
   0.06924711};
   gre = new TGraphErrors(6,gRAA_rap_0s_fx1004,gRAA_rap_0s_fy1004,gRAA_rap_0s_fex1004,gRAA_rap_0s_fey1004);
   gre->SetName("gRAA_rap_0s");
   gre->SetTitle("raa_vs_rap");

   ci = TColor::GetColor("#990033");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#990033");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetMarkerSize(1.5);
   
   TH1F *Graph_gRAA_rap_0s1004 = new TH1F("Graph_gRAA_rap_0s1004","raa_vs_rap",100,0,2.4);
   Graph_gRAA_rap_0s1004->SetMinimum(0.317604);
   Graph_gRAA_rap_0s1004->SetMaximum(0.4993514);
   Graph_gRAA_rap_0s1004->SetDirectory(0);
   Graph_gRAA_rap_0s1004->SetStats(0);
   Graph_gRAA_rap_0s1004->SetLineStyle(0);
   Graph_gRAA_rap_0s1004->SetMarkerStyle(20);
   Graph_gRAA_rap_0s1004->GetXaxis()->SetLabelFont(42);
   Graph_gRAA_rap_0s1004->GetXaxis()->SetLabelOffset(0.007);
   Graph_gRAA_rap_0s1004->GetXaxis()->SetLabelSize(0.05);
   Graph_gRAA_rap_0s1004->GetXaxis()->SetTitleSize(0.06);
   Graph_gRAA_rap_0s1004->GetXaxis()->SetTitleOffset(0.9);
   Graph_gRAA_rap_0s1004->GetXaxis()->SetTitleFont(42);
   Graph_gRAA_rap_0s1004->GetYaxis()->SetLabelFont(42);
   Graph_gRAA_rap_0s1004->GetYaxis()->SetLabelOffset(0.007);
   Graph_gRAA_rap_0s1004->GetYaxis()->SetLabelSize(0.05);
   Graph_gRAA_rap_0s1004->GetYaxis()->SetTitleSize(0.06);
   Graph_gRAA_rap_0s1004->GetYaxis()->SetTitleOffset(1.25);
   Graph_gRAA_rap_0s1004->GetYaxis()->SetTitleFont(42);
   Graph_gRAA_rap_0s1004->GetZaxis()->SetLabelFont(42);
   Graph_gRAA_rap_0s1004->GetZaxis()->SetLabelOffset(0.007);
   Graph_gRAA_rap_0s1004->GetZaxis()->SetLabelSize(0.05);
   Graph_gRAA_rap_0s1004->GetZaxis()->SetTitleSize(0.06);
   Graph_gRAA_rap_0s1004->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_gRAA_rap_0s1004);
   
   gre->Draw("p");
   
   Double_t gRAA_rap_1s_fx1005[3] = {
   0.4,
   1.2,
   2};
   Double_t gRAA_rap_1s_fy1005[3] = {
   0.1114396,
   0.1303647,
   0.07022626};
   Double_t gRAA_rap_1s_fex1005[3] = {
   0,
   0,
   0};
   Double_t gRAA_rap_1s_fey1005[3] = {
   0.03040212,
   0.03856313,
   0.04965036};
   gre = new TGraphErrors(3,gRAA_rap_1s_fx1005,gRAA_rap_1s_fy1005,gRAA_rap_1s_fex1005,gRAA_rap_1s_fey1005);
   gre->SetName("gRAA_rap_1s");
   gre->SetTitle("raa_vs_rap");

   ci = TColor::GetColor("#3333cc");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#3333cc");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(21);
   gre->SetMarkerSize(1.5);
   
   TH1F *Graph_gRAA_rap_1s1005 = new TH1F("Graph_gRAA_rap_1s1005","raa_vs_rap",100,0.24,2.16);
   Graph_gRAA_rap_1s1005->SetMinimum(0.005740717);
   Graph_gRAA_rap_1s1005->SetMaximum(0.183763);
   Graph_gRAA_rap_1s1005->SetDirectory(0);
   Graph_gRAA_rap_1s1005->SetStats(0);
   Graph_gRAA_rap_1s1005->SetLineStyle(0);
   Graph_gRAA_rap_1s1005->SetMarkerStyle(20);
   Graph_gRAA_rap_1s1005->GetXaxis()->SetLabelFont(42);
   Graph_gRAA_rap_1s1005->GetXaxis()->SetLabelOffset(0.007);
   Graph_gRAA_rap_1s1005->GetXaxis()->SetLabelSize(0.05);
   Graph_gRAA_rap_1s1005->GetXaxis()->SetTitleSize(0.06);
   Graph_gRAA_rap_1s1005->GetXaxis()->SetTitleOffset(0.9);
   Graph_gRAA_rap_1s1005->GetXaxis()->SetTitleFont(42);
   Graph_gRAA_rap_1s1005->GetYaxis()->SetLabelFont(42);
   Graph_gRAA_rap_1s1005->GetYaxis()->SetLabelOffset(0.007);
   Graph_gRAA_rap_1s1005->GetYaxis()->SetLabelSize(0.05);
   Graph_gRAA_rap_1s1005->GetYaxis()->SetTitleSize(0.06);
   Graph_gRAA_rap_1s1005->GetYaxis()->SetTitleOffset(1.25);
   Graph_gRAA_rap_1s1005->GetYaxis()->SetTitleFont(42);
   Graph_gRAA_rap_1s1005->GetZaxis()->SetLabelFont(42);
   Graph_gRAA_rap_1s1005->GetZaxis()->SetLabelOffset(0.007);
   Graph_gRAA_rap_1s1005->GetZaxis()->SetLabelSize(0.05);
   Graph_gRAA_rap_1s1005->GetZaxis()->SetTitleSize(0.06);
   Graph_gRAA_rap_1s1005->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_gRAA_rap_1s1005);
   
   gre->Draw("p");
   TArrow *arrow = new TArrow(0.6,0,0.6,0.1016435,0.027,"<-|");
   arrow->SetFillColor(1);
   arrow->SetFillStyle(1001);

   ci = TColor::GetColor("#009900");
   arrow->SetLineColor(ci);
   arrow->SetLineWidth(2);
   arrow->Draw();
   arrow = new TArrow(1.8,0,1.8,0.1282432,0.027,"<-|");
   arrow->SetFillColor(1);
   arrow->SetFillStyle(1001);

   ci = TColor::GetColor("#009900");
   arrow->SetLineColor(ci);
   arrow->SetLineWidth(2);
   arrow->Draw();
   
   Double_t gRAA_rap_2s_fx1006[3] = {
   -10,
   -11,
   -12};
   Double_t gRAA_rap_2s_fy1006[3] = {
   -10,
   -11,
   -12};
   Double_t gRAA_rap_2s_fex1006[3] = {
   0,
   0,
   0};
   Double_t gRAA_rap_2s_fey1006[3] = {
   0,
   0,
   0};
   gre = new TGraphErrors(3,gRAA_rap_2s_fx1006,gRAA_rap_2s_fy1006,gRAA_rap_2s_fex1006,gRAA_rap_2s_fey1006);
   gre->SetName("gRAA_rap_2s");
   gre->SetTitle("raa_vs_rap");

   ci = TColor::GetColor("#009900");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#009900");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(33);
   gre->SetMarkerSize(2.6);
   
   TH1F *Graph_gRAA_rap_2s1006 = new TH1F("Graph_gRAA_rap_2s1006","raa_vs_rap",100,0,30);
   Graph_gRAA_rap_2s1006->SetMinimum(0);
   Graph_gRAA_rap_2s1006->SetMaximum(1.3);
   Graph_gRAA_rap_2s1006->SetDirectory(0);
   Graph_gRAA_rap_2s1006->SetStats(0);
   Graph_gRAA_rap_2s1006->SetLineStyle(0);
   Graph_gRAA_rap_2s1006->SetMarkerStyle(20);
   Graph_gRAA_rap_2s1006->GetXaxis()->SetRange(1,100);
   Graph_gRAA_rap_2s1006->GetXaxis()->SetLabelFont(42);
   Graph_gRAA_rap_2s1006->GetXaxis()->SetLabelOffset(0.007);
   Graph_gRAA_rap_2s1006->GetXaxis()->SetLabelSize(0.05);
   Graph_gRAA_rap_2s1006->GetXaxis()->SetTitleSize(0.06);
   Graph_gRAA_rap_2s1006->GetXaxis()->SetTitleOffset(0.9);
   Graph_gRAA_rap_2s1006->GetXaxis()->SetTitleFont(42);
   Graph_gRAA_rap_2s1006->GetYaxis()->SetLabelFont(42);
   Graph_gRAA_rap_2s1006->GetYaxis()->SetLabelOffset(0.007);
   Graph_gRAA_rap_2s1006->GetYaxis()->SetLabelSize(0.05);
   Graph_gRAA_rap_2s1006->GetYaxis()->SetTitleSize(0.06);
   Graph_gRAA_rap_2s1006->GetYaxis()->SetTitleOffset(1.25);
   Graph_gRAA_rap_2s1006->GetYaxis()->SetTitleFont(42);
   Graph_gRAA_rap_2s1006->GetZaxis()->SetLabelFont(42);
   Graph_gRAA_rap_2s1006->GetZaxis()->SetLabelOffset(0.007);
   Graph_gRAA_rap_2s1006->GetZaxis()->SetLabelSize(0.05);
   Graph_gRAA_rap_2s1006->GetZaxis()->SetTitleSize(0.06);
   Graph_gRAA_rap_2s1006->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_gRAA_rap_2s1006);
   
   gre->Draw("p");
   TLine *line = new TLine(0,1,2.4,1);
   line->SetLineStyle(7);
   line->Draw();
   
   TLegend *leg = new TLegend(0.57,0.5,0.78,0.62,NULL,"brNDC");
   leg->SetMargin(0.2);
   leg->SetBorderSize(0);
   leg->SetTextSize(0.04);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(4000);
   TLegendEntry *entry=leg->AddEntry("NULL"," #varUpsilon(3S) 68% CL","f");

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
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);
   leg->Draw();
   
   leg = new TLegend(0.57,0.62,0.785,0.74,NULL,"brNDC");
   leg->SetMargin(0.2);
   leg->SetBorderSize(0);
   leg->SetTextSize(0.04);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(4000);
   entry=leg->AddEntry("gRAA_rap_0s"," #varUpsilon(1S)","lp");

   ci = TColor::GetColor("#990033");
   entry->SetLineColor(ci);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);

   ci = TColor::GetColor("#990033");
   entry->SetMarkerColor(ci);
   entry->SetMarkerStyle(20);
   entry->SetMarkerSize(1.5);
   entry->SetTextFont(42);
   entry=leg->AddEntry("gRAA_rap_1s"," #varUpsilon(2S)","lp");

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
   
   leg = new TLegend(0.57,0.5,0.78,0.62,NULL,"brNDC");
   leg->SetMargin(0.2);
   leg->SetBorderSize(0);
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
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);
   leg->Draw();
   arrow = new TArrow(1.285,0.532,1.285,0.582,0.02,"<-|");
   arrow->SetFillColor(1);
   arrow->SetFillStyle(1001);

   ci = TColor::GetColor("#009900");
   arrow->SetLineColor(ci);
   arrow->SetLineWidth(2);
   arrow->Draw();
   TLatex *   tex = new TLatex(0.22,0.8715,"p_{T} < 30 GeV");
tex->SetNDC();
   tex->SetTextAlign(12);
   tex->SetTextFont(42);
   tex->SetTextSize(0.04);
   tex->SetLineWidth(2);
   tex->Draw();
      tex = new TLatex(0.22,0.818,"Cent. 0-100%");
tex->SetNDC();
   tex->SetTextAlign(12);
   tex->SetTextFont(42);
   tex->SetTextSize(0.04);
   tex->SetLineWidth(2);
   tex->Draw();
   box = new TBox(2.34,0.953256,2.4,1.042579);

   ci = 1185;
   color = new TColor(ci, 0.4, 0.4, 0.4, " ", 0.6);
   box->SetFillColor(ci);
   box->Draw("l");
      tex = new TLatex(0.968,0.945264,"PbPb 368 #mub^{-1}, pp 28.0 pb^{-1} (5.02 TeV)");
tex->SetNDC();
   tex->SetTextAlign(31);
   tex->SetTextFont(42);
   tex->SetTextSize(0.045828);
   tex->SetLineWidth(2);
   tex->Draw();
      tex = new TLatex(0.92164,0.9007622,"CMS");
tex->SetNDC();
   tex->SetTextAlign(33);
   tex->SetTextFont(61);
   tex->SetTextSize(0.057285);
   tex->SetLineWidth(2);
   tex->Draw();
   c1->Modified();
   c1->cd();
   c1->SetSelected(c1);
   c1->SaveAs("Figure4_b.pdf");
}
