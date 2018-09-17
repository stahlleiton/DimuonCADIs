void Figure3_b()
{
//=========Macro generated from canvas: c1/c1
//=========  (Mon Sep 17 15:55:45 2018) by ROOT version 6.14/04
   TCanvas *c1 = new TCanvas("c1", "c1",0,0,600,600);
   gStyle->SetOptFit(1);
   gStyle->SetOptStat(0);
   gStyle->SetOptTitle(0);
   gStyle->SetEndErrorSize(0);
   c1->Range(-0.7774648,-3.348846,2.602817,2.594316);
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
   
   Double_t gCrossSection_rap_AA_fx1001[6] = {
   0.2,
   0.6,
   1,
   1.4,
   1.8,
   2.2};
   Double_t gCrossSection_rap_AA_fy1001[6] = {
   0.5716893,
   0.5809172,
   0.6135613,
   0.4906042,
   0.5018906,
   0.4670837};
   Double_t gCrossSection_rap_AA_fex1001[6] = {
   0.2,
   0.2,
   0.2,
   0.2,
   0.2,
   0.2};
   Double_t gCrossSection_rap_AA_fey1001[6] = {
   0.03221195,
   0.03072757,
   0.03422581,
   0.03476146,
   0.02770621,
   0.07131203};
   TGraphErrors *gre = new TGraphErrors(6,gCrossSection_rap_AA_fx1001,gCrossSection_rap_AA_fy1001,gCrossSection_rap_AA_fex1001,gCrossSection_rap_AA_fey1001);
   gre->SetName("gCrossSection_rap_AA");
   gre->SetTitle("cs_vs_rap_AA");

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = 1181;
   color = new TColor(ci, 1, 0.8, 0.8, " ", 0.5);
   gre->SetFillColor(ci);

   ci = TColor::GetColor("#990033");
   gre->SetLineColor(ci);
   gre->SetMarkerColor(2);
   gre->SetMarkerStyle(20);
   
   TH1F *Graph_gCrossSection_rap_AA1001 = new TH1F("Graph_gCrossSection_rap_AA1001","cs_vs_rap_AA",100,0,2.4);
   Graph_gCrossSection_rap_AA1001->SetMinimum(0.004);
   Graph_gCrossSection_rap_AA1001->SetMaximum(100);
   Graph_gCrossSection_rap_AA1001->SetDirectory(0);
   Graph_gCrossSection_rap_AA1001->SetStats(0);
   Graph_gCrossSection_rap_AA1001->SetLineStyle(0);
   Graph_gCrossSection_rap_AA1001->SetMarkerStyle(20);
   Graph_gCrossSection_rap_AA1001->GetXaxis()->SetTitle("|y|");
   Graph_gCrossSection_rap_AA1001->GetXaxis()->CenterTitle(true);
   Graph_gCrossSection_rap_AA1001->GetXaxis()->SetNdivisions(505);
   Graph_gCrossSection_rap_AA1001->GetXaxis()->SetLabelFont(42);
   Graph_gCrossSection_rap_AA1001->GetXaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_rap_AA1001->GetXaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_rap_AA1001->GetXaxis()->SetTitleSize(0.06);
   Graph_gCrossSection_rap_AA1001->GetXaxis()->SetTitleFont(42);
   Graph_gCrossSection_rap_AA1001->GetYaxis()->SetTitle("B #frac{1}{T_{AA}} #frac{dN}{dy} (nb)");
   Graph_gCrossSection_rap_AA1001->GetYaxis()->CenterTitle(true);
   Graph_gCrossSection_rap_AA1001->GetYaxis()->SetLabelFont(42);
   Graph_gCrossSection_rap_AA1001->GetYaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_rap_AA1001->GetYaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_rap_AA1001->GetYaxis()->SetTitleSize(0.045);
   Graph_gCrossSection_rap_AA1001->GetYaxis()->SetTitleOffset(2);
   Graph_gCrossSection_rap_AA1001->GetYaxis()->SetTitleFont(42);
   Graph_gCrossSection_rap_AA1001->GetZaxis()->SetLabelFont(42);
   Graph_gCrossSection_rap_AA1001->GetZaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_rap_AA1001->GetZaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_rap_AA1001->GetZaxis()->SetTitleSize(0.06);
   Graph_gCrossSection_rap_AA1001->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_gCrossSection_rap_AA1001);
   
   gre->Draw("a5");
   
   Double_t gCrossSection_rap_AA_fx1002[3] = {
   0.4,
   1.2,
   2};
   Double_t gCrossSection_rap_AA_fy1002[3] = {
   0.04459272,
   0.04760168,
   0.02289169};
   Double_t gCrossSection_rap_AA_fex1002[3] = {
   0.4,
   0.4,
   0.4};
   Double_t gCrossSection_rap_AA_fey1002[3] = {
   0.003459244,
   0.009047334,
   0.005802453};
   gre = new TGraphErrors(3,gCrossSection_rap_AA_fx1002,gCrossSection_rap_AA_fy1002,gCrossSection_rap_AA_fex1002,gCrossSection_rap_AA_fey1002);
   gre->SetName("gCrossSection_rap_AA");
   gre->SetTitle("cs_vs_rap_AA");

   ci = 1182;
   color = new TColor(ci, 0.8, 0.8, 1, " ", 0.5);
   gre->SetFillColor(ci);

   ci = TColor::GetColor("#3333cc");
   gre->SetLineColor(ci);
   gre->SetMarkerColor(2);
   gre->SetMarkerStyle(20);
   
   TH1F *Graph_gCrossSection_rap_AA1002 = new TH1F("Graph_gCrossSection_rap_AA1002","cs_vs_rap_AA",100,0,2.64);
   Graph_gCrossSection_rap_AA1002->SetMinimum(0.01313326);
   Graph_gCrossSection_rap_AA1002->SetMaximum(0.06060499);
   Graph_gCrossSection_rap_AA1002->SetDirectory(0);
   Graph_gCrossSection_rap_AA1002->SetStats(0);
   Graph_gCrossSection_rap_AA1002->SetLineStyle(0);
   Graph_gCrossSection_rap_AA1002->SetMarkerStyle(20);
   Graph_gCrossSection_rap_AA1002->GetXaxis()->SetLabelFont(42);
   Graph_gCrossSection_rap_AA1002->GetXaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_rap_AA1002->GetXaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_rap_AA1002->GetXaxis()->SetTitleSize(0.06);
   Graph_gCrossSection_rap_AA1002->GetXaxis()->SetTitleOffset(0.9);
   Graph_gCrossSection_rap_AA1002->GetXaxis()->SetTitleFont(42);
   Graph_gCrossSection_rap_AA1002->GetYaxis()->SetLabelFont(42);
   Graph_gCrossSection_rap_AA1002->GetYaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_rap_AA1002->GetYaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_rap_AA1002->GetYaxis()->SetTitleSize(0.06);
   Graph_gCrossSection_rap_AA1002->GetYaxis()->SetTitleOffset(1.25);
   Graph_gCrossSection_rap_AA1002->GetYaxis()->SetTitleFont(42);
   Graph_gCrossSection_rap_AA1002->GetZaxis()->SetLabelFont(42);
   Graph_gCrossSection_rap_AA1002->GetZaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_rap_AA1002->GetZaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_rap_AA1002->GetZaxis()->SetTitleSize(0.06);
   Graph_gCrossSection_rap_AA1002->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_gCrossSection_rap_AA1002);
   
   gre->Draw("5");
   TBox *box = new TBox(0.55,0,0.65,0.01094549);

   ci = 1179;
   color = new TColor(ci, 0.4, 0.8, 0.4, " ", 0.5);
   box->SetFillColor(ci);

   ci = TColor::GetColor("#006600");
   box->SetLineColor(ci);
   box->Draw("l");
   box = new TBox(1.75,0,1.85,0.01055235);

   ci = 1180;
   color = new TColor(ci, 0.4, 0.8, 0.4, " ", 0.5);
   box->SetFillColor(ci);

   ci = TColor::GetColor("#006600");
   box->SetLineColor(ci);
   box->Draw("l");
   
   Double_t gCrossSection_rap_AA_fx1003[3] = {
   -10,
   -11,
   -12};
   Double_t gCrossSection_rap_AA_fy1003[3] = {
   -10,
   -11,
   -12};
   Double_t gCrossSection_rap_AA_fex1003[3] = {
   0,
   0,
   0};
   Double_t gCrossSection_rap_AA_fey1003[3] = {
   0,
   0,
   0};
   gre = new TGraphErrors(3,gCrossSection_rap_AA_fx1003,gCrossSection_rap_AA_fy1003,gCrossSection_rap_AA_fex1003,gCrossSection_rap_AA_fey1003);
   gre->SetName("gCrossSection_rap_AA");
   gre->SetTitle("cs_vs_rap_AA");

   ci = 1183;
   color = new TColor(ci, 0.8, 1, 0.8, " ", 0.5);
   gre->SetFillColor(ci);

   ci = TColor::GetColor("#009900");
   gre->SetLineColor(ci);
   gre->SetMarkerColor(2);
   gre->SetMarkerStyle(20);
   
   TH1F *Graph_gCrossSection_rap_AA1003 = new TH1F("Graph_gCrossSection_rap_AA1003","cs_vs_rap_AA",100,0,2.4);
   Graph_gCrossSection_rap_AA1003->SetMinimum(0);
   Graph_gCrossSection_rap_AA1003->SetMaximum(1.3);
   Graph_gCrossSection_rap_AA1003->SetDirectory(0);
   Graph_gCrossSection_rap_AA1003->SetStats(0);
   Graph_gCrossSection_rap_AA1003->SetLineStyle(0);
   Graph_gCrossSection_rap_AA1003->SetMarkerStyle(20);
   Graph_gCrossSection_rap_AA1003->GetXaxis()->SetRange(1,100);
   Graph_gCrossSection_rap_AA1003->GetXaxis()->SetLabelFont(42);
   Graph_gCrossSection_rap_AA1003->GetXaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_rap_AA1003->GetXaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_rap_AA1003->GetXaxis()->SetTitleSize(0.06);
   Graph_gCrossSection_rap_AA1003->GetXaxis()->SetTitleOffset(0.9);
   Graph_gCrossSection_rap_AA1003->GetXaxis()->SetTitleFont(42);
   Graph_gCrossSection_rap_AA1003->GetYaxis()->SetLabelFont(42);
   Graph_gCrossSection_rap_AA1003->GetYaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_rap_AA1003->GetYaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_rap_AA1003->GetYaxis()->SetTitleSize(0.06);
   Graph_gCrossSection_rap_AA1003->GetYaxis()->SetTitleOffset(1.25);
   Graph_gCrossSection_rap_AA1003->GetYaxis()->SetTitleFont(42);
   Graph_gCrossSection_rap_AA1003->GetZaxis()->SetLabelFont(42);
   Graph_gCrossSection_rap_AA1003->GetZaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_rap_AA1003->GetZaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_rap_AA1003->GetZaxis()->SetTitleSize(0.06);
   Graph_gCrossSection_rap_AA1003->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_gCrossSection_rap_AA1003);
   
   gre->Draw("5");
   
   Double_t gCrossSection_rap_AA_fx1004[6] = {
   0.2,
   0.6,
   1,
   1.4,
   1.8,
   2.2};
   Double_t gCrossSection_rap_AA_fy1004[6] = {
   0.5716893,
   0.5809172,
   0.6135613,
   0.4906042,
   0.5018906,
   0.4670837};
   Double_t gCrossSection_rap_AA_fex1004[6] = {
   0,
   0,
   0,
   0,
   0,
   0};
   Double_t gCrossSection_rap_AA_fey1004[6] = {
   0.03168117,
   0.03671608,
   0.04626309,
   0.03780505,
   0.04483295,
   0.07646675};
   gre = new TGraphErrors(6,gCrossSection_rap_AA_fx1004,gCrossSection_rap_AA_fy1004,gCrossSection_rap_AA_fex1004,gCrossSection_rap_AA_fey1004);
   gre->SetName("gCrossSection_rap_AA");
   gre->SetTitle("cs_vs_rap_AA");

   ci = TColor::GetColor("#990033");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#990033");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetMarkerSize(1.5);
   
   TH1F *Graph_gCrossSection_rap_AA1004 = new TH1F("Graph_gCrossSection_rap_AA1004","cs_vs_rap_AA",100,0,2.4);
   Graph_gCrossSection_rap_AA1004->SetMinimum(0.3636962);
   Graph_gCrossSection_rap_AA1004->SetMaximum(0.6867451);
   Graph_gCrossSection_rap_AA1004->SetDirectory(0);
   Graph_gCrossSection_rap_AA1004->SetStats(0);
   Graph_gCrossSection_rap_AA1004->SetLineStyle(0);
   Graph_gCrossSection_rap_AA1004->SetMarkerStyle(20);
   Graph_gCrossSection_rap_AA1004->GetXaxis()->SetLabelFont(42);
   Graph_gCrossSection_rap_AA1004->GetXaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_rap_AA1004->GetXaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_rap_AA1004->GetXaxis()->SetTitleSize(0.06);
   Graph_gCrossSection_rap_AA1004->GetXaxis()->SetTitleOffset(0.9);
   Graph_gCrossSection_rap_AA1004->GetXaxis()->SetTitleFont(42);
   Graph_gCrossSection_rap_AA1004->GetYaxis()->SetLabelFont(42);
   Graph_gCrossSection_rap_AA1004->GetYaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_rap_AA1004->GetYaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_rap_AA1004->GetYaxis()->SetTitleSize(0.06);
   Graph_gCrossSection_rap_AA1004->GetYaxis()->SetTitleOffset(1.25);
   Graph_gCrossSection_rap_AA1004->GetYaxis()->SetTitleFont(42);
   Graph_gCrossSection_rap_AA1004->GetZaxis()->SetLabelFont(42);
   Graph_gCrossSection_rap_AA1004->GetZaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_rap_AA1004->GetZaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_rap_AA1004->GetZaxis()->SetTitleSize(0.06);
   Graph_gCrossSection_rap_AA1004->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_gCrossSection_rap_AA1004);
   
   gre->Draw("p");
   auto gre1 = (TGraphErrors*) gre;
   
   Double_t gCrossSection_rap_AA_fx1005[3] = {
   0.4,
   1.2,
   2};
   Double_t gCrossSection_rap_AA_fy1005[3] = {
   0.04459272,
   0.04760168,
   0.02289169};
   Double_t gCrossSection_rap_AA_fex1005[3] = {
   0,
   0,
   0};
   Double_t gCrossSection_rap_AA_fey1005[3] = {
   0.01210943,
   0.01395396,
   0.01615906};
   gre = new TGraphErrors(3,gCrossSection_rap_AA_fx1005,gCrossSection_rap_AA_fy1005,gCrossSection_rap_AA_fex1005,gCrossSection_rap_AA_fey1005);
   gre->SetName("gCrossSection_rap_AA");
   gre->SetTitle("cs_vs_rap_AA");

   ci = TColor::GetColor("#3333cc");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#3333cc");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(21);
   gre->SetMarkerSize(1.5);
   
   TH1F *Graph_gCrossSection_rap_AA1005 = new TH1F("Graph_gCrossSection_rap_AA1005","cs_vs_rap_AA",100,0.24,2.16);
   Graph_gCrossSection_rap_AA1005->SetMinimum(0.001250336);
   Graph_gCrossSection_rap_AA1005->SetMaximum(0.06703794);
   Graph_gCrossSection_rap_AA1005->SetDirectory(0);
   Graph_gCrossSection_rap_AA1005->SetStats(0);
   Graph_gCrossSection_rap_AA1005->SetLineStyle(0);
   Graph_gCrossSection_rap_AA1005->SetMarkerStyle(20);
   Graph_gCrossSection_rap_AA1005->GetXaxis()->SetLabelFont(42);
   Graph_gCrossSection_rap_AA1005->GetXaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_rap_AA1005->GetXaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_rap_AA1005->GetXaxis()->SetTitleSize(0.06);
   Graph_gCrossSection_rap_AA1005->GetXaxis()->SetTitleOffset(0.9);
   Graph_gCrossSection_rap_AA1005->GetXaxis()->SetTitleFont(42);
   Graph_gCrossSection_rap_AA1005->GetYaxis()->SetLabelFont(42);
   Graph_gCrossSection_rap_AA1005->GetYaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_rap_AA1005->GetYaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_rap_AA1005->GetYaxis()->SetTitleSize(0.06);
   Graph_gCrossSection_rap_AA1005->GetYaxis()->SetTitleOffset(1.25);
   Graph_gCrossSection_rap_AA1005->GetYaxis()->SetTitleFont(42);
   Graph_gCrossSection_rap_AA1005->GetZaxis()->SetLabelFont(42);
   Graph_gCrossSection_rap_AA1005->GetZaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_rap_AA1005->GetZaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_rap_AA1005->GetZaxis()->SetTitleSize(0.06);
   Graph_gCrossSection_rap_AA1005->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_gCrossSection_rap_AA1005);
   
   gre->Draw("p");
   auto gre2 = (TGraphErrors*) gre;
   TArrow *arrow = new TArrow(0.6,0,0.6,0.01888086,0.027,"<-|");
   arrow->SetFillColor(1);
   arrow->SetFillStyle(1001);

   ci = TColor::GetColor("#009900");
   arrow->SetLineColor(ci);
   arrow->SetLineWidth(2);
   arrow->Draw();
   arrow = new TArrow(1.8,0,1.8,0.01934971,0.027,"<-|");
   arrow->SetFillColor(1);
   arrow->SetFillStyle(1001);

   ci = TColor::GetColor("#009900");
   arrow->SetLineColor(ci);
   arrow->SetLineWidth(2);
   arrow->Draw();
   
   Double_t gCrossSection_rap_AA_fx1006[3] = {
   -10,
   -11,
   -12};
   Double_t gCrossSection_rap_AA_fy1006[3] = {
   -10,
   -11,
   -12};
   Double_t gCrossSection_rap_AA_fex1006[3] = {
   0,
   0,
   0};
   Double_t gCrossSection_rap_AA_fey1006[3] = {
   0,
   0,
   0};
   gre = new TGraphErrors(3,gCrossSection_rap_AA_fx1006,gCrossSection_rap_AA_fy1006,gCrossSection_rap_AA_fex1006,gCrossSection_rap_AA_fey1006);
   gre->SetName("gCrossSection_rap_AA");
   gre->SetTitle("cs_vs_rap_AA");

   ci = TColor::GetColor("#009900");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#009900");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(33);
   gre->SetMarkerSize(2.6);
   
   TH1F *Graph_gCrossSection_rap_AA1006 = new TH1F("Graph_gCrossSection_rap_AA1006","cs_vs_rap_AA",100,0,2.4);
   Graph_gCrossSection_rap_AA1006->SetMinimum(0);
   Graph_gCrossSection_rap_AA1006->SetMaximum(1.3);
   Graph_gCrossSection_rap_AA1006->SetDirectory(0);
   Graph_gCrossSection_rap_AA1006->SetStats(0);
   Graph_gCrossSection_rap_AA1006->SetLineStyle(0);
   Graph_gCrossSection_rap_AA1006->SetMarkerStyle(20);
   Graph_gCrossSection_rap_AA1006->GetXaxis()->SetRange(1,100);
   Graph_gCrossSection_rap_AA1006->GetXaxis()->SetLabelFont(42);
   Graph_gCrossSection_rap_AA1006->GetXaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_rap_AA1006->GetXaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_rap_AA1006->GetXaxis()->SetTitleSize(0.06);
   Graph_gCrossSection_rap_AA1006->GetXaxis()->SetTitleOffset(0.9);
   Graph_gCrossSection_rap_AA1006->GetXaxis()->SetTitleFont(42);
   Graph_gCrossSection_rap_AA1006->GetYaxis()->SetLabelFont(42);
   Graph_gCrossSection_rap_AA1006->GetYaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_rap_AA1006->GetYaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_rap_AA1006->GetYaxis()->SetTitleSize(0.06);
   Graph_gCrossSection_rap_AA1006->GetYaxis()->SetTitleOffset(1.25);
   Graph_gCrossSection_rap_AA1006->GetYaxis()->SetTitleFont(42);
   Graph_gCrossSection_rap_AA1006->GetZaxis()->SetLabelFont(42);
   Graph_gCrossSection_rap_AA1006->GetZaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_rap_AA1006->GetZaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_rap_AA1006->GetZaxis()->SetTitleSize(0.06);
   Graph_gCrossSection_rap_AA1006->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_gCrossSection_rap_AA1006);
   
   gre->Draw("p");
   
   TLegend *leg = new TLegend(0.62,0.67,0.83,0.76,NULL,"brNDC");
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
   
   leg = new TLegend(0.62,0.58,0.83,0.67,NULL,"brNDC");
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
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);
   leg->Draw();
   arrow = new TArrow(1.39,1.4275,1.39,2.09,0.021,"<-|");
   arrow->SetFillColor(1);
   arrow->SetFillStyle(1001);

   ci = TColor::GetColor("#009900");
   arrow->SetLineColor(ci);
   arrow->SetLineWidth(2);
   arrow->Draw();
   TLatex *   tex = new TLatex(0.27,0.8225,"p_{T} < 30 GeV");
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
   c1->SaveAs("Figure3_b.pdf");
}
