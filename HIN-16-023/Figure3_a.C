void Figure3_a()
{
//=========Macro generated from canvas: c1/c1
//=========  (Mon Sep 17 15:52:50 2018) by ROOT version 6.14/04
   TCanvas *c1 = new TCanvas("c1", "c1",0,0,600,600);
   gStyle->SetOptFit(1);
   gStyle->SetOptStat(0);
   gStyle->SetOptTitle(0);
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
   
   Double_t gCrossSection_rap_PP_fx1001[6] = {
   0.2,
   0.6,
   1,
   1.4,
   1.8,
   2.2};
   Double_t gCrossSection_rap_PP_fy1001[6] = {
   1.492248,
   1.52289,
   1.558653,
   1.356358,
   1.327006,
   1.125615};
   Double_t gCrossSection_rap_PP_fex1001[6] = {
   0.2,
   0.2,
   0.2,
   0.2,
   0.2,
   0.2};
   Double_t gCrossSection_rap_PP_fey1001[6] = {
   0.07981223,
   0.05286293,
   0.1129125,
   0.06556598,
   0.05576018,
   0.04983239};
   TGraphErrors *gre = new TGraphErrors(6,gCrossSection_rap_PP_fx1001,gCrossSection_rap_PP_fy1001,gCrossSection_rap_PP_fex1001,gCrossSection_rap_PP_fey1001);
   gre->SetName("gCrossSection_rap_PP");
   gre->SetTitle("cs_vs_rap_PP");

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = 1181;
   color = new TColor(ci, 1, 0.8, 0.8, " ", 0.5);
   gre->SetFillColor(ci);

   ci = TColor::GetColor("#990033");
   gre->SetLineColor(ci);
   gre->SetMarkerStyle(20);
   
   TH1F *Graph_gCrossSection_rap_PP1001 = new TH1F("Graph_gCrossSection_rap_PP1001","cs_vs_rap_PP",100,0,2.4);
   Graph_gCrossSection_rap_PP1001->SetMinimum(0.004);
   Graph_gCrossSection_rap_PP1001->SetMaximum(100);
   Graph_gCrossSection_rap_PP1001->SetDirectory(0);
   Graph_gCrossSection_rap_PP1001->SetStats(0);
   Graph_gCrossSection_rap_PP1001->SetLineStyle(0);
   Graph_gCrossSection_rap_PP1001->SetMarkerStyle(20);
   Graph_gCrossSection_rap_PP1001->GetXaxis()->SetTitle("|y|");
   Graph_gCrossSection_rap_PP1001->GetXaxis()->CenterTitle(true);
   Graph_gCrossSection_rap_PP1001->GetXaxis()->SetNdivisions(505);
   Graph_gCrossSection_rap_PP1001->GetXaxis()->SetLabelFont(42);
   Graph_gCrossSection_rap_PP1001->GetXaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_rap_PP1001->GetXaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_rap_PP1001->GetXaxis()->SetTitleSize(0.06);
   Graph_gCrossSection_rap_PP1001->GetXaxis()->SetTitleFont(42);
   Graph_gCrossSection_rap_PP1001->GetYaxis()->SetTitle("B #frac{d#sigma}{dy} (nb)");
   Graph_gCrossSection_rap_PP1001->GetYaxis()->CenterTitle(true);
   Graph_gCrossSection_rap_PP1001->GetYaxis()->SetLabelFont(42);
   Graph_gCrossSection_rap_PP1001->GetYaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_rap_PP1001->GetYaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_rap_PP1001->GetYaxis()->SetTitleSize(0.045);
   Graph_gCrossSection_rap_PP1001->GetYaxis()->SetTitleOffset(2);
   Graph_gCrossSection_rap_PP1001->GetYaxis()->SetTitleFont(42);
   Graph_gCrossSection_rap_PP1001->GetZaxis()->SetLabelFont(42);
   Graph_gCrossSection_rap_PP1001->GetZaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_rap_PP1001->GetZaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_rap_PP1001->GetZaxis()->SetTitleSize(0.06);
   Graph_gCrossSection_rap_PP1001->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_gCrossSection_rap_PP1001);
   
   gre->Draw("a5");
   
   Double_t gCrossSection_rap_PP_fx1002[3] = {
   0.4,
   1.2,
   2};
   Double_t gCrossSection_rap_PP_fy1002[3] = {
   0.4001517,
   0.3651425,
   0.3259705};
   Double_t gCrossSection_rap_PP_fex1002[3] = {
   0.4,
   0.4,
   0.4};
   Double_t gCrossSection_rap_PP_fey1002[3] = {
   0.01396992,
   0.02639363,
   0.01654593};
   gre = new TGraphErrors(3,gCrossSection_rap_PP_fx1002,gCrossSection_rap_PP_fy1002,gCrossSection_rap_PP_fex1002,gCrossSection_rap_PP_fey1002);
   gre->SetName("gCrossSection_rap_PP");
   gre->SetTitle("cs_vs_rap_PP");

   ci = 1182;
   color = new TColor(ci, 0.8, 0.8, 1, " ", 0.5);
   gre->SetFillColor(ci);

   ci = TColor::GetColor("#3333cc");
   gre->SetLineColor(ci);
   gre->SetMarkerStyle(20);
   
   TH1F *Graph_gCrossSection_rap_PP1002 = new TH1F("Graph_gCrossSection_rap_PP1002","cs_vs_rap_PP",100,0,2.64);
   Graph_gCrossSection_rap_PP1002->SetMinimum(0.2989549);
   Graph_gCrossSection_rap_PP1002->SetMaximum(0.4245913);
   Graph_gCrossSection_rap_PP1002->SetDirectory(0);
   Graph_gCrossSection_rap_PP1002->SetStats(0);
   Graph_gCrossSection_rap_PP1002->SetLineStyle(0);
   Graph_gCrossSection_rap_PP1002->SetMarkerStyle(20);
   Graph_gCrossSection_rap_PP1002->GetXaxis()->SetLabelFont(42);
   Graph_gCrossSection_rap_PP1002->GetXaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_rap_PP1002->GetXaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_rap_PP1002->GetXaxis()->SetTitleSize(0.06);
   Graph_gCrossSection_rap_PP1002->GetXaxis()->SetTitleOffset(0.9);
   Graph_gCrossSection_rap_PP1002->GetXaxis()->SetTitleFont(42);
   Graph_gCrossSection_rap_PP1002->GetYaxis()->SetLabelFont(42);
   Graph_gCrossSection_rap_PP1002->GetYaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_rap_PP1002->GetYaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_rap_PP1002->GetYaxis()->SetTitleSize(0.06);
   Graph_gCrossSection_rap_PP1002->GetYaxis()->SetTitleOffset(1.25);
   Graph_gCrossSection_rap_PP1002->GetYaxis()->SetTitleFont(42);
   Graph_gCrossSection_rap_PP1002->GetZaxis()->SetLabelFont(42);
   Graph_gCrossSection_rap_PP1002->GetZaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_rap_PP1002->GetZaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_rap_PP1002->GetZaxis()->SetTitleSize(0.06);
   Graph_gCrossSection_rap_PP1002->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_gCrossSection_rap_PP1002);
   
   gre->Draw("5");
   
   Double_t gCrossSection_rap_PP_fx1003[2] = {
   0.6,
   1.8};
   Double_t gCrossSection_rap_PP_fy1003[2] = {
   0.185769,
   0.1508917};
   Double_t gCrossSection_rap_PP_fex1003[2] = {
   0.6,
   0.6};
   Double_t gCrossSection_rap_PP_fey1003[2] = {
   0.007729513,
   0.009637914};
   gre = new TGraphErrors(2,gCrossSection_rap_PP_fx1003,gCrossSection_rap_PP_fy1003,gCrossSection_rap_PP_fex1003,gCrossSection_rap_PP_fey1003);
   gre->SetName("gCrossSection_rap_PP");
   gre->SetTitle("cs_vs_rap_PP");

   ci = 1183;
   color = new TColor(ci, 0.8, 1, 0.8, " ", 0.5);
   gre->SetFillColor(ci);

   ci = TColor::GetColor("#009900");
   gre->SetLineColor(ci);
   gre->SetMarkerStyle(20);
   
   TH1F *Graph_gCrossSection_rap_PP1003 = new TH1F("Graph_gCrossSection_rap_PP1003","cs_vs_rap_PP",100,0,2.64);
   Graph_gCrossSection_rap_PP1003->SetMinimum(0.1360293);
   Graph_gCrossSection_rap_PP1003->SetMaximum(0.198723);
   Graph_gCrossSection_rap_PP1003->SetDirectory(0);
   Graph_gCrossSection_rap_PP1003->SetStats(0);
   Graph_gCrossSection_rap_PP1003->SetLineStyle(0);
   Graph_gCrossSection_rap_PP1003->SetMarkerStyle(20);
   Graph_gCrossSection_rap_PP1003->GetXaxis()->SetLabelFont(42);
   Graph_gCrossSection_rap_PP1003->GetXaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_rap_PP1003->GetXaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_rap_PP1003->GetXaxis()->SetTitleSize(0.06);
   Graph_gCrossSection_rap_PP1003->GetXaxis()->SetTitleOffset(0.9);
   Graph_gCrossSection_rap_PP1003->GetXaxis()->SetTitleFont(42);
   Graph_gCrossSection_rap_PP1003->GetYaxis()->SetLabelFont(42);
   Graph_gCrossSection_rap_PP1003->GetYaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_rap_PP1003->GetYaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_rap_PP1003->GetYaxis()->SetTitleSize(0.06);
   Graph_gCrossSection_rap_PP1003->GetYaxis()->SetTitleOffset(1.25);
   Graph_gCrossSection_rap_PP1003->GetYaxis()->SetTitleFont(42);
   Graph_gCrossSection_rap_PP1003->GetZaxis()->SetLabelFont(42);
   Graph_gCrossSection_rap_PP1003->GetZaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_rap_PP1003->GetZaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_rap_PP1003->GetZaxis()->SetTitleSize(0.06);
   Graph_gCrossSection_rap_PP1003->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_gCrossSection_rap_PP1003);
   
   gre->Draw("5");
   
   Double_t gCrossSection_rap_PP_fx1004[6] = {
   0.2,
   0.6,
   1,
   1.4,
   1.8,
   2.2};
   Double_t gCrossSection_rap_PP_fy1004[6] = {
   1.492248,
   1.52289,
   1.558653,
   1.356358,
   1.327006,
   1.125615};
   Double_t gCrossSection_rap_PP_fex1004[6] = {
   0,
   0,
   0,
   0,
   0,
   0};
   Double_t gCrossSection_rap_PP_fey1004[6] = {
   0.02766757,
   0.03256425,
   0.03526084,
   0.02944414,
   0.04068704,
   0.03641631};
   gre = new TGraphErrors(6,gCrossSection_rap_PP_fx1004,gCrossSection_rap_PP_fy1004,gCrossSection_rap_PP_fex1004,gCrossSection_rap_PP_fey1004);
   gre->SetName("gCrossSection_rap_PP");
   gre->SetTitle("cs_vs_rap_PP");

   ci = TColor::GetColor("#990033");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#990033");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetMarkerSize(1.5);
   
   TH1F *Graph_gCrossSection_rap_PP1004 = new TH1F("Graph_gCrossSection_rap_PP1004","cs_vs_rap_PP",100,0,2.4);
   Graph_gCrossSection_rap_PP1004->SetMinimum(1.038727);
   Graph_gCrossSection_rap_PP1004->SetMaximum(1.644385);
   Graph_gCrossSection_rap_PP1004->SetDirectory(0);
   Graph_gCrossSection_rap_PP1004->SetStats(0);
   Graph_gCrossSection_rap_PP1004->SetLineStyle(0);
   Graph_gCrossSection_rap_PP1004->SetMarkerStyle(20);
   Graph_gCrossSection_rap_PP1004->GetXaxis()->SetLabelFont(42);
   Graph_gCrossSection_rap_PP1004->GetXaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_rap_PP1004->GetXaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_rap_PP1004->GetXaxis()->SetTitleSize(0.06);
   Graph_gCrossSection_rap_PP1004->GetXaxis()->SetTitleOffset(0.9);
   Graph_gCrossSection_rap_PP1004->GetXaxis()->SetTitleFont(42);
   Graph_gCrossSection_rap_PP1004->GetYaxis()->SetLabelFont(42);
   Graph_gCrossSection_rap_PP1004->GetYaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_rap_PP1004->GetYaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_rap_PP1004->GetYaxis()->SetTitleSize(0.06);
   Graph_gCrossSection_rap_PP1004->GetYaxis()->SetTitleOffset(1.25);
   Graph_gCrossSection_rap_PP1004->GetYaxis()->SetTitleFont(42);
   Graph_gCrossSection_rap_PP1004->GetZaxis()->SetLabelFont(42);
   Graph_gCrossSection_rap_PP1004->GetZaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_rap_PP1004->GetZaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_rap_PP1004->GetZaxis()->SetTitleSize(0.06);
   Graph_gCrossSection_rap_PP1004->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_gCrossSection_rap_PP1004);
   
   gre->Draw("p");
   auto gre1 = (TGraphErrors*) gre;
   
   Double_t gCrossSection_rap_PP_fx1005[3] = {
   0.4,
   1.2,
   2};
   Double_t gCrossSection_rap_PP_fy1005[3] = {
   0.4001517,
   0.3651425,
   0.3259705};
   Double_t gCrossSection_rap_PP_fex1005[3] = {
   0,
   0,
   0};
   Double_t gCrossSection_rap_PP_fey1005[3] = {
   0.01046484,
   0.014479,
   0.01293119};
   gre = new TGraphErrors(3,gCrossSection_rap_PP_fx1005,gCrossSection_rap_PP_fy1005,gCrossSection_rap_PP_fex1005,gCrossSection_rap_PP_fey1005);
   gre->SetName("gCrossSection_rap_PP");
   gre->SetTitle("cs_vs_rap_PP");

   ci = TColor::GetColor("#3333cc");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#3333cc");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(21);
   gre->SetMarkerSize(1.5);
   
   TH1F *Graph_gCrossSection_rap_PP1005 = new TH1F("Graph_gCrossSection_rap_PP1005","cs_vs_rap_PP",100,0.24,2.16);
   Graph_gCrossSection_rap_PP1005->SetMinimum(0.3032816);
   Graph_gCrossSection_rap_PP1005->SetMaximum(0.4203742);
   Graph_gCrossSection_rap_PP1005->SetDirectory(0);
   Graph_gCrossSection_rap_PP1005->SetStats(0);
   Graph_gCrossSection_rap_PP1005->SetLineStyle(0);
   Graph_gCrossSection_rap_PP1005->SetMarkerStyle(20);
   Graph_gCrossSection_rap_PP1005->GetXaxis()->SetLabelFont(42);
   Graph_gCrossSection_rap_PP1005->GetXaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_rap_PP1005->GetXaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_rap_PP1005->GetXaxis()->SetTitleSize(0.06);
   Graph_gCrossSection_rap_PP1005->GetXaxis()->SetTitleOffset(0.9);
   Graph_gCrossSection_rap_PP1005->GetXaxis()->SetTitleFont(42);
   Graph_gCrossSection_rap_PP1005->GetYaxis()->SetLabelFont(42);
   Graph_gCrossSection_rap_PP1005->GetYaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_rap_PP1005->GetYaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_rap_PP1005->GetYaxis()->SetTitleSize(0.06);
   Graph_gCrossSection_rap_PP1005->GetYaxis()->SetTitleOffset(1.25);
   Graph_gCrossSection_rap_PP1005->GetYaxis()->SetTitleFont(42);
   Graph_gCrossSection_rap_PP1005->GetZaxis()->SetLabelFont(42);
   Graph_gCrossSection_rap_PP1005->GetZaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_rap_PP1005->GetZaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_rap_PP1005->GetZaxis()->SetTitleSize(0.06);
   Graph_gCrossSection_rap_PP1005->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_gCrossSection_rap_PP1005);
   
   gre->Draw("p");
   auto gre2 = (TGraphErrors*) gre;
   
   Double_t gCrossSection_rap_PP_fx1006[2] = {
   0.6,
   1.8};
   Double_t gCrossSection_rap_PP_fy1006[2] = {
   0.185769,
   0.1508917};
   Double_t gCrossSection_rap_PP_fex1006[2] = {
   0,
   0};
   Double_t gCrossSection_rap_PP_fey1006[2] = {
   0.0063669,
   0.008091514};
   gre = new TGraphErrors(2,gCrossSection_rap_PP_fx1006,gCrossSection_rap_PP_fy1006,gCrossSection_rap_PP_fex1006,gCrossSection_rap_PP_fey1006);
   gre->SetName("gCrossSection_rap_PP");
   gre->SetTitle("cs_vs_rap_PP");

   ci = TColor::GetColor("#009900");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#009900");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(33);
   gre->SetMarkerSize(2.6);
   
   TH1F *Graph_gCrossSection_rap_PP1006 = new TH1F("Graph_gCrossSection_rap_PP1006","cs_vs_rap_PP",100,0.48,1.92);
   Graph_gCrossSection_rap_PP1006->SetMinimum(0.1378666);
   Graph_gCrossSection_rap_PP1006->SetMaximum(0.1970695);
   Graph_gCrossSection_rap_PP1006->SetDirectory(0);
   Graph_gCrossSection_rap_PP1006->SetStats(0);
   Graph_gCrossSection_rap_PP1006->SetLineStyle(0);
   Graph_gCrossSection_rap_PP1006->SetMarkerStyle(20);
   Graph_gCrossSection_rap_PP1006->GetXaxis()->SetLabelFont(42);
   Graph_gCrossSection_rap_PP1006->GetXaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_rap_PP1006->GetXaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_rap_PP1006->GetXaxis()->SetTitleSize(0.06);
   Graph_gCrossSection_rap_PP1006->GetXaxis()->SetTitleOffset(0.9);
   Graph_gCrossSection_rap_PP1006->GetXaxis()->SetTitleFont(42);
   Graph_gCrossSection_rap_PP1006->GetYaxis()->SetLabelFont(42);
   Graph_gCrossSection_rap_PP1006->GetYaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_rap_PP1006->GetYaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_rap_PP1006->GetYaxis()->SetTitleSize(0.06);
   Graph_gCrossSection_rap_PP1006->GetYaxis()->SetTitleOffset(1.25);
   Graph_gCrossSection_rap_PP1006->GetYaxis()->SetTitleFont(42);
   Graph_gCrossSection_rap_PP1006->GetZaxis()->SetLabelFont(42);
   Graph_gCrossSection_rap_PP1006->GetZaxis()->SetLabelOffset(0.007);
   Graph_gCrossSection_rap_PP1006->GetZaxis()->SetLabelSize(0.05);
   Graph_gCrossSection_rap_PP1006->GetZaxis()->SetTitleSize(0.06);
   Graph_gCrossSection_rap_PP1006->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_gCrossSection_rap_PP1006);
   
   gre->Draw("p");
   auto gre3 = (TGraphErrors*) gre;
   
   TLegend *leg = new TLegend(0.68,0.61,0.85,0.77,NULL,"brNDC");
   leg->SetBorderSize(0);
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
   entry=leg->AddEntry(gre3," #varUpsilon(3S)","lp");

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
   TLatex *   tex = new TLatex(0.27,0.8225,"p_{T} < 30 GeV");
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
   c1->SaveAs("Figure3_a.pdf");
}
