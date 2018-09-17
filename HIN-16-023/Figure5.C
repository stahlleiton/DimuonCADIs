#include "tdrstyle.C"
void Figure5()
{
   setTDRStyle();
//=========Macro generated from canvas: c1/c1
//=========  (Mon Sep 17 14:10:02 2018) by ROOT version 6.14/04
   TCanvas *c1 = new TCanvas("c1", "c1",0,0,720,600);
   gStyle->SetOptFit(1);
   gStyle->SetOptStat(0);
   gStyle->SetOptTitle(0);
   c1->Range(0,0,1,1);
   c1->SetFillColor(0);
   c1->SetBorderMode(0);
   c1->SetBorderSize(2);
   c1->SetTickx(1);
   c1->SetTicky(1);
   c1->SetLeftMargin(0.16);
   c1->SetRightMargin(0.032);
   c1->SetTopMargin(0.05);
   c1->SetBottomMargin(0.13);
   c1->SetFrameFillStyle(0);
   c1->SetFrameBorderMode(0);
  
// ------------>Primitives in pad: pad_diff
   TPad *pad_diff = new TPad("pad_diff", "",0,0,0.8333333,1);
   pad_diff->Draw();
   pad_diff->cd();
   pad_diff->Range(-80,-0.2295082,420,1.409836);
   pad_diff->SetFillColor(0);
   pad_diff->SetBorderMode(0);
   pad_diff->SetBorderSize(2);
   pad_diff->SetTickx(1);
   pad_diff->SetTicky(1);
   pad_diff->SetLeftMargin(0.16);
   pad_diff->SetRightMargin(0);
   pad_diff->SetTopMargin(0.067);
   pad_diff->SetBottomMargin(0.14);
   pad_diff->SetFrameFillStyle(0);
   pad_diff->SetFrameBorderMode(0);
   pad_diff->SetFrameFillStyle(0);
   pad_diff->SetFrameBorderMode(0);
   
   Double_t _fx3001[9] = {
   8.3,
   30.6,
   53.9,
   87,
   131.4,
   189.2,
   264.2,
   333.3,
   384.3};
   Double_t _fy3001[9] = {
   0.7919321,
   0.922196,
   0.6086184,
   0.5241877,
   0.4844938,
   0.4017651,
   0.3235429,
   0.3211745,
   0.3194109};
   Double_t _felx3001[9] = {
   6.5,
   6.5,
   6.5,
   6.5,
   6.5,
   6.5,
   6.5,
   6.5,
   6.5};
   Double_t _fely3001[9] = {
   0.09682648,
   0.1169516,
   0.06297354,
   0.0449575,
   0.03592627,
   0.02670031,
   0.02105022,
   0.02009717,
   0.01991985};
   Double_t _fehx3001[9] = {
   6.5,
   6.5,
   6.5,
   6.5,
   6.5,
   6.5,
   6.5,
   6.5,
   6.5};
   Double_t _fehy3001[9] = {
   0.1404223,
   0.1268311,
   0.06505664,
   0.04576916,
   0.03529345,
   0.02560477,
   0.01978118,
   0.01866271,
   0.01854871};
   TGraphAsymmErrors *grae = new TGraphAsymmErrors(9,_fx3001,_fy3001,_felx3001,_fehx3001,_fely3001,_fehy3001);
   grae->SetName("");
   grae->SetTitle("");

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = 1179;
   color = new TColor(ci, 1, 0.8, 0.8, " ", 0.5);
   grae->SetFillColor(ci);
   grae->SetFillStyle(1000);

   ci = TColor::GetColor("#990033");
   grae->SetLineColor(ci);
   grae->SetMarkerStyle(20);
   
   TH1F *Graph_Graph3001 = new TH1F("Graph_Graph3001","",100,0,420);
   Graph_Graph3001->SetMinimum(0);
   Graph_Graph3001->SetMaximum(1.3);
   Graph_Graph3001->SetDirectory(0);
   Graph_Graph3001->SetStats(0);
   Graph_Graph3001->SetLineStyle(0);
   Graph_Graph3001->SetMarkerStyle(20);
   Graph_Graph3001->GetXaxis()->SetTitle("<N_{part}>");
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
   
   Double_t _fx3002[9] = {
   8.3,
   30.6,
   53.9,
   87,
   131.4,
   189.2,
   264.2,
   333.3,
   -10};
   Double_t _fy3002[9] = {
   0.5629225,
   0.5301776,
   0.2104486,
   0.2289698,
   0.1939089,
   0.15358,
   0.1008641,
   0.1185284,
   -10};
   Double_t _felx3002[9] = {
   6.5,
   6.5,
   6.5,
   6.5,
   6.5,
   6.5,
   6.5,
   6.5,
   0};
   Double_t _fely3002[9] = {
   0.1356602,
   0.128076,
   0.03731097,
   0.02308734,
   0.01905314,
   0.03175675,
   0.03138051,
   0.009354116,
   0};
   Double_t _fehx3002[9] = {
   6.5,
   6.5,
   6.5,
   6.5,
   6.5,
   6.5,
   6.5,
   6.5,
   0};
   Double_t _fehy3002[9] = {
   0.1537196,
   0.1311469,
   0.03773587,
   0.02338964,
   0.01886273,
   0.03162464,
   0.03130017,
   0.008940195,
   0};
   grae = new TGraphAsymmErrors(9,_fx3002,_fy3002,_felx3002,_fehx3002,_fely3002,_fehy3002);
   grae->SetName("");
   grae->SetTitle("");

   ci = 1181;
   color = new TColor(ci, 0.8, 0.8, 1, " ", 0.5);
   grae->SetFillColor(ci);
   grae->SetFillStyle(1000);

   ci = TColor::GetColor("#3333cc");
   grae->SetLineColor(ci);
   grae->SetMarkerStyle(20);
   
   TH1F *Graph_Graph3002 = new TH1F("Graph_Graph3002","",100,-44.98,374.78);
   Graph_Graph3002->SetMinimum(-11.07166);
   Graph_Graph3002->SetMaximum(1.788306);
   Graph_Graph3002->SetDirectory(0);
   Graph_Graph3002->SetStats(0);
   Graph_Graph3002->SetLineStyle(0);
   Graph_Graph3002->SetMarkerStyle(20);
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
   TBox *box = new TBox(40.31,0.04925276,53.31,0.1879432);

   ci = TColor::GetColor("#009900");
   box->SetFillColor(ci);
   box->SetFillStyle(3005);
   box->Draw("l");
   box = new TBox(264.2,0,277.2,0.03967124);

   ci = TColor::GetColor("#009900");
   box->SetFillColor(ci);
   box->SetFillStyle(3005);
   box->Draw("l");
   
   Double_t gRAA_cent_fx1001[9] = {
   8.3,
   30.6,
   53.9,
   87,
   131.4,
   189.2,
   264.2,
   333.3,
   384.3};
   Double_t gRAA_cent_fy1001[9] = {
   0.7919321,
   0.922196,
   0.6086184,
   0.5241877,
   0.4844938,
   0.4017651,
   0.3235429,
   0.3211745,
   0.3194109};
   Double_t gRAA_cent_fex1001[9] = {
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0};
   Double_t gRAA_cent_fey1001[9] = {
   0.1304656,
   0.08759765,
   0.05271126,
   0.03564997,
   0.02704457,
   0.03906605,
   0.01713875,
   0.02111884,
   0.01860167};
   TGraphErrors *gre = new TGraphErrors(9,gRAA_cent_fx1001,gRAA_cent_fy1001,gRAA_cent_fex1001,gRAA_cent_fey1001);
   gre->SetName("gRAA_cent");
   gre->SetTitle("raa_vs_npart");

   ci = TColor::GetColor("#6699ff");
   gre->SetFillColor(ci);

   ci = TColor::GetColor("#990033");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#990033");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetMarkerSize(1.5);
   
   TH1F *Graph_gRAA_cent1001 = new TH1F("Graph_gRAA_cent1001","raa_vs_npart",100,0,421.9);
   Graph_gRAA_cent1001->SetMinimum(0.2290818);
   Graph_gRAA_cent1001->SetMaximum(1.080767);
   Graph_gRAA_cent1001->SetDirectory(0);
   Graph_gRAA_cent1001->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_gRAA_cent1001->SetLineColor(ci);
   Graph_gRAA_cent1001->GetXaxis()->CenterTitle(true);
   Graph_gRAA_cent1001->GetXaxis()->SetLabelFont(42);
   Graph_gRAA_cent1001->GetXaxis()->SetLabelSize(0.035);
   Graph_gRAA_cent1001->GetXaxis()->SetTitleSize(0.035);
   Graph_gRAA_cent1001->GetXaxis()->SetTitleFont(42);
   Graph_gRAA_cent1001->GetYaxis()->CenterTitle(true);
   Graph_gRAA_cent1001->GetYaxis()->SetLabelFont(42);
   Graph_gRAA_cent1001->GetYaxis()->SetLabelSize(0.035);
   Graph_gRAA_cent1001->GetYaxis()->SetTitleSize(0.035);
   Graph_gRAA_cent1001->GetYaxis()->SetTitleOffset(1.25);
   Graph_gRAA_cent1001->GetYaxis()->SetTitleFont(42);
   Graph_gRAA_cent1001->GetZaxis()->SetLabelFont(42);
   Graph_gRAA_cent1001->GetZaxis()->SetLabelSize(0.035);
   Graph_gRAA_cent1001->GetZaxis()->SetTitleSize(0.035);
   Graph_gRAA_cent1001->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_gRAA_cent1001);
   
   gre->Draw("p");
   
   Double_t gRAA_cent_fx1002[9] = {
   8.3,
   30.6,
   53.9,
   87,
   131.4,
   189.2,
   264.2,
   333.3,
   -10};
   Double_t gRAA_cent_fy1002[9] = {
   0.5629225,
   0.5301776,
   0.2104486,
   0.2289698,
   0.1939089,
   0.15358,
   0.1008641,
   0.1185284,
   -10};
   Double_t gRAA_cent_fex1002[9] = {
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0};
   Double_t gRAA_cent_fey1002[9] = {
   0.3192715,
   0.1694656,
   0.1060893,
   0.07738388,
   0.05997379,
   0.09225658,
   0.04217409,
   0.05129333,
   0};
   gre = new TGraphErrors(9,gRAA_cent_fx1002,gRAA_cent_fy1002,gRAA_cent_fex1002,gRAA_cent_fey1002);
   gre->SetName("gRAA_cent_1");
   gre->SetTitle("raa_vs_npart");

   ci = TColor::GetColor("#6699ff");
   gre->SetFillColor(ci);

   ci = TColor::GetColor("#3333cc");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#3333cc");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(21);
   gre->SetMarkerSize(1.5);
   
   TH1F *Graph_gRAA_cent1002 = new TH1F("Graph_gRAA_cent1002","raa_vs_npart",100,-44.33,367.63);
   Graph_gRAA_cent1002->SetMinimum(-11.08822);
   Graph_gRAA_cent1002->SetMaximum(1.970413);
   Graph_gRAA_cent1002->SetDirectory(0);
   Graph_gRAA_cent1002->SetStats(0);
   Graph_gRAA_cent1002->SetLineStyle(0);
   Graph_gRAA_cent1002->SetMarkerStyle(20);
   Graph_gRAA_cent1002->GetXaxis()->CenterTitle(true);
   Graph_gRAA_cent1002->GetXaxis()->SetLabelFont(42);
   Graph_gRAA_cent1002->GetXaxis()->SetLabelSize(0.035);
   Graph_gRAA_cent1002->GetXaxis()->SetTitleSize(0.035);
   Graph_gRAA_cent1002->GetXaxis()->SetTitleFont(42);
   Graph_gRAA_cent1002->GetYaxis()->CenterTitle(true);
   Graph_gRAA_cent1002->GetYaxis()->SetLabelFont(42);
   Graph_gRAA_cent1002->GetYaxis()->SetLabelSize(0.035);
   Graph_gRAA_cent1002->GetYaxis()->SetTitleSize(0.035);
   Graph_gRAA_cent1002->GetYaxis()->SetTitleOffset(1.25);
   Graph_gRAA_cent1002->GetYaxis()->SetTitleFont(42);
   Graph_gRAA_cent1002->GetZaxis()->SetLabelFont(42);
   Graph_gRAA_cent1002->GetZaxis()->SetLabelOffset(0.007);
   Graph_gRAA_cent1002->GetZaxis()->SetLabelSize(0.05);
   Graph_gRAA_cent1002->GetZaxis()->SetTitleSize(0.06);
   Graph_gRAA_cent1002->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_gRAA_cent1002);
   
   gre->Draw("p");
   TArrow *arrow = new TArrow(46.81,0,46.81,0.2583376,0.027,"<-|");
   arrow->SetFillColor(1);
   arrow->SetFillStyle(1001);

   ci = TColor::GetColor("#009900");
   arrow->SetLineColor(ci);
   arrow->SetLineWidth(2);
   arrow->Draw();
   arrow = new TArrow(270.7,0,270.7,0.07741529,0.027,"<-|");
   arrow->SetFillColor(1);
   arrow->SetFillStyle(1001);

   ci = TColor::GetColor("#009900");
   arrow->SetLineColor(ci);
   arrow->SetLineWidth(2);
   arrow->Draw();
   box = new TBox(377.8,0,390.8,0.0762);

   ci = TColor::GetColor("#3333cc");
   box->SetFillColor(ci);
   box->SetFillStyle(3005);

   ci = TColor::GetColor("#3333cc");
   box->SetLineColor(ci);
   box->Draw("l");
   arrow = new TArrow(384.3,0,384.3,0.117,0.027,"<-|");
   arrow->SetFillColor(1);
   arrow->SetFillStyle(1001);

   ci = TColor::GetColor("#3333cc");
   arrow->SetLineColor(ci);
   arrow->SetLineWidth(2);
   arrow->Draw();
   TLine *line = new TLine(0,1,420,1);
   line->SetLineStyle(7);
   line->Draw();
   
   TLegend *leg = new TLegend(0.39,0.58,0.59,0.7,NULL,"brNDC");
   leg->SetBorderSize(0);
   leg->SetTextSize(0.0385);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(4000);
   TLegendEntry *entry=leg->AddEntry("gRAA_cent"," #varUpsilon(1S)","lp");

   ci = TColor::GetColor("#990033");
   entry->SetLineColor(ci);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);

   ci = TColor::GetColor("#990033");
   entry->SetMarkerColor(ci);
   entry->SetMarkerStyle(20);
   entry->SetMarkerSize(1.5);
   entry->SetTextFont(42);
   entry=leg->AddEntry("gRAA_cent_1"," #varUpsilon(2S)","lp");

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
   
   leg = new TLegend(0.57,0.58,0.74,0.76,NULL,"brNDC");
   leg->SetBorderSize(0);
   leg->SetTextSize(0.0386);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(4000);
   entry=leg->AddEntry("NULL","68% CL","h");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);
   entry=leg->AddEntry("NULL"," #varUpsilon(2S)","f");

   ci = TColor::GetColor("#3333cc");
   entry->SetFillColor(ci);
   entry->SetFillStyle(3005);

   ci = TColor::GetColor("#3333cc");
   entry->SetLineColor(ci);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);
   entry=leg->AddEntry("NULL"," #varUpsilon(3S)","f");

   ci = TColor::GetColor("#009900");
   entry->SetFillColor(ci);
   entry->SetFillStyle(3005);

   ci = TColor::GetColor("#009900");
   entry->SetLineColor(ci);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);
   leg->Draw();
   
   leg = new TLegend(0.74,0.58,0.91,0.76,NULL,"brNDC");
   leg->SetBorderSize(0);
   leg->SetTextSize(0.0386);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(4000);
   entry=leg->AddEntry("NULL","95% CL","h");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);
   entry=leg->AddEntry("NULL"," #varUpsilon(2S)","");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);
   entry=leg->AddEntry("NULL"," #varUpsilon(3S)","");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);
   leg->Draw();
   arrow = new TArrow(300,0.739,300,0.789,0.022,"<-|");
   arrow->SetFillColor(1);
   arrow->SetFillStyle(1001);

   ci = TColor::GetColor("#009900");
   arrow->SetLineColor(ci);
   arrow->SetLineWidth(2);
   arrow->Draw();
   arrow = new TArrow(300,0.838,300,0.888,0.022,"<-|");
   arrow->SetFillColor(1);
   arrow->SetFillStyle(1001);

   ci = TColor::GetColor("#3333cc");
   arrow->SetLineColor(ci);
   arrow->SetLineWidth(2);
   arrow->Draw();
   TLatex *   tex = new TLatex(0.26,0.874,"p_{T} < 30 GeV");
tex->SetNDC();
   tex->SetTextAlign(12);
   tex->SetTextFont(42);
   tex->SetTextSize(0.0387);
   tex->SetLineWidth(2);
   tex->Draw();
      tex = new TLatex(0.26,0.8182,"|y| < 2.4");
tex->SetNDC();
   tex->SetTextAlign(12);
   tex->SetTextFont(42);
   tex->SetTextSize(0.0387);
   tex->SetLineWidth(2);
   tex->Draw();
   box = new TBox(375,0.967922,390,1.032078);

   ci = 1185;
   color = new TColor(ci, 0.4, 0.4, 0.4, " ", 0);
   box->SetFillColor(ci);
   box->SetLineWidth(2);
   box->Draw("l");
   box = new TBox(390,0.9638371,406,1.036163);

   ci = TColor::GetColor("#990033");
   box->SetFillColor(ci);
   box->Draw();
   box = new TBox(405,0.9524021,420,1.047598);

   ci = TColor::GetColor("#3333cc");
   box->SetFillColor(ci);
   box->Draw();
      tex = new TLatex(1,0.945264,"PbPb 368/464 #mub^{-1}, pp 28.0 pb^{-1} (5.02 TeV)");
tex->SetNDC();
   tex->SetTextAlign(31);
   tex->SetTextFont(42);
   tex->SetTextSize(0.045828);
   tex->SetLineWidth(2);
   tex->Draw();
      tex = new TLatex(0.9522,0.9007622,"CMS");
tex->SetNDC();
   tex->SetTextAlign(33);
   tex->SetTextFont(61);
   tex->SetTextSize(0.057285);
   tex->SetLineWidth(2);
   tex->Draw();
   pad_diff->Modified();
   c1->cd();
  
// ------------>Primitives in pad: pad_int
   TPad *pad_int = new TPad("pad_int", "",0.8333333,0,1,1);
   pad_int->Draw();
   pad_int->cd();
   pad_int->Range(0.6,-0.2295082,2.028571,1.409836);
   pad_int->SetFillColor(0);
   pad_int->SetBorderMode(0);
   pad_int->SetBorderSize(2);
   pad_int->SetTickx(1);
   pad_int->SetTicky(1);
   pad_int->SetLeftMargin(0);
   pad_int->SetRightMargin(0.16);
   pad_int->SetTopMargin(0.067);
   pad_int->SetBottomMargin(0.14);
   pad_int->SetFrameFillStyle(0);
   pad_int->SetFrameBorderMode(0);
   pad_int->SetFrameFillStyle(0);
   pad_int->SetFrameBorderMode(0);
   
   Double_t _fx3003[1] = {
   1};
   Double_t _fy3003[1] = {
   0.375961};
   Double_t _felx3003[1] = {
   0.09};
   Double_t _fely3003[1] = {
   0.03472246};
   Double_t _fehx3003[1] = {
   0.09};
   Double_t _fehy3003[1] = {
   0.03395686};
   grae = new TGraphAsymmErrors(1,_fx3003,_fy3003,_felx3003,_fehx3003,_fely3003,_fehy3003);
   grae->SetName("");
   grae->SetTitle("");

   ci = 1180;
   color = new TColor(ci, 1, 0.8, 0.8, " ", 0.5);
   grae->SetFillColor(ci);
   grae->SetFillStyle(1000);

   ci = TColor::GetColor("#990033");
   grae->SetLineColor(ci);
   grae->SetMarkerStyle(20);
   
   TH1F *Graph_Graph3003 = new TH1F("Graph_Graph3003","",100,0.6,1.8);
   Graph_Graph3003->SetMinimum(0);
   Graph_Graph3003->SetMaximum(1.3);
   Graph_Graph3003->SetDirectory(0);
   Graph_Graph3003->SetStats(0);
   Graph_Graph3003->SetLineStyle(0);
   Graph_Graph3003->SetMarkerStyle(20);
   Graph_Graph3003->GetXaxis()->SetNdivisions(101);
   Graph_Graph3003->GetXaxis()->SetLabelFont(42);
   Graph_Graph3003->GetXaxis()->SetLabelOffset(0.007);
   Graph_Graph3003->GetXaxis()->SetLabelSize(0);
   Graph_Graph3003->GetXaxis()->SetTitleSize(0.06);
   Graph_Graph3003->GetXaxis()->SetTitleOffset(0.9);
   Graph_Graph3003->GetXaxis()->SetTitleFont(42);
   Graph_Graph3003->GetYaxis()->SetLabelFont(42);
   Graph_Graph3003->GetYaxis()->SetLabelOffset(0.007);
   Graph_Graph3003->GetYaxis()->SetLabelSize(0);
   Graph_Graph3003->GetYaxis()->SetTitleSize(0.06);
   Graph_Graph3003->GetYaxis()->SetTickLength(0.15);
   Graph_Graph3003->GetYaxis()->SetTitleOffset(1.25);
   Graph_Graph3003->GetYaxis()->SetTitleFont(42);
   Graph_Graph3003->GetZaxis()->SetLabelFont(42);
   Graph_Graph3003->GetZaxis()->SetLabelOffset(0.007);
   Graph_Graph3003->GetZaxis()->SetLabelSize(0.05);
   Graph_Graph3003->GetZaxis()->SetTitleSize(0.06);
   Graph_Graph3003->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_Graph3003);
   
   grae->Draw("a5");
   
   Double_t _fx3004[1] = {
   1.2};
   Double_t _fy3004[1] = {
   0.1165341};
   Double_t _felx3004[1] = {
   0.09};
   Double_t _fely3004[1] = {
   0.01937991};
   Double_t _fehx3004[1] = {
   0.09};
   Double_t _fehy3004[1] = {
   0.01924914};
   grae = new TGraphAsymmErrors(1,_fx3004,_fy3004,_felx3004,_fehx3004,_fely3004,_fehy3004);
   grae->SetName("");
   grae->SetTitle("");

   ci = 1182;
   color = new TColor(ci, 0.8, 0.8, 1, " ", 0.5);
   grae->SetFillColor(ci);
   grae->SetFillStyle(1000);

   ci = TColor::GetColor("#3333cc");
   grae->SetLineColor(ci);
   grae->SetMarkerStyle(20);
   
   TH1F *Graph_Graph3004 = new TH1F("Graph_Graph3004","",100,1.092,1.308);
   Graph_Graph3004->SetMinimum(0.09329133);
   Graph_Graph3004->SetMaximum(0.1396462);
   Graph_Graph3004->SetDirectory(0);
   Graph_Graph3004->SetStats(0);
   Graph_Graph3004->SetLineStyle(0);
   Graph_Graph3004->SetMarkerStyle(20);
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
   box = new TBox(1.31,0,1.49,0.05927921);

   ci = TColor::GetColor("#009900");
   box->SetFillColor(ci);
   box->SetFillStyle(3005);

   ci = TColor::GetColor("#009900");
   box->SetLineColor(ci);
   box->Draw("l");
   
   Double_t gRAA_int_fx1003[1] = {
   1};
   Double_t gRAA_int_fy1003[1] = {
   0.375961};
   Double_t gRAA_int_fex1003[1] = {
   0};
   Double_t gRAA_int_fey1003[1] = {
   0.01335999};
   gre = new TGraphErrors(1,gRAA_int_fx1003,gRAA_int_fy1003,gRAA_int_fex1003,gRAA_int_fey1003);
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
   
   TH1F *Graph_gRAA_int1003 = new TH1F("Graph_gRAA_int1003","raa_integrated",100,0.9,2.1);
   Graph_gRAA_int1003->SetMinimum(0.359929);
   Graph_gRAA_int1003->SetMaximum(0.391993);
   Graph_gRAA_int1003->SetDirectory(0);
   Graph_gRAA_int1003->SetStats(0);
   Graph_gRAA_int1003->SetLineStyle(0);
   Graph_gRAA_int1003->SetMarkerStyle(20);
   Graph_gRAA_int1003->GetXaxis()->CenterTitle(true);
   Graph_gRAA_int1003->GetXaxis()->SetLabelFont(42);
   Graph_gRAA_int1003->GetXaxis()->SetLabelOffset(999);
   Graph_gRAA_int1003->GetXaxis()->SetLabelSize(0);
   Graph_gRAA_int1003->GetXaxis()->SetTitleSize(0.035);
   Graph_gRAA_int1003->GetXaxis()->SetTitleFont(42);
   Graph_gRAA_int1003->GetYaxis()->CenterTitle(true);
   Graph_gRAA_int1003->GetYaxis()->SetLabelFont(42);
   Graph_gRAA_int1003->GetYaxis()->SetLabelSize(0.035);
   Graph_gRAA_int1003->GetYaxis()->SetTitleSize(0.035);
   Graph_gRAA_int1003->GetYaxis()->SetTitleOffset(1.25);
   Graph_gRAA_int1003->GetYaxis()->SetTitleFont(42);
   Graph_gRAA_int1003->GetZaxis()->SetLabelFont(42);
   Graph_gRAA_int1003->GetZaxis()->SetLabelOffset(0.007);
   Graph_gRAA_int1003->GetZaxis()->SetLabelSize(0.05);
   Graph_gRAA_int1003->GetZaxis()->SetTitleSize(0.06);
   Graph_gRAA_int1003->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_gRAA_int1003);
   
   gre->Draw("p");
   
   Double_t gRAA_int_fx1004[1] = {
   1.2};
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
   
   TH1F *Graph_gRAA_int1004 = new TH1F("Graph_gRAA_int1004","raa_integrated",100,1.1,2.3);
   Graph_gRAA_int1004->SetMinimum(0.09051013);
   Graph_gRAA_int1004->SetMaximum(0.1425582);
   Graph_gRAA_int1004->SetDirectory(0);
   Graph_gRAA_int1004->SetStats(0);
   Graph_gRAA_int1004->SetLineStyle(0);
   Graph_gRAA_int1004->SetMarkerStyle(20);
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
   arrow = new TArrow(1.4,0,1.4,0.09626922,0.02,"<-|");
   arrow->SetFillColor(1);
   arrow->SetFillStyle(1001);

   ci = TColor::GetColor("#009900");
   arrow->SetLineColor(ci);
   arrow->SetLineWidth(2);
   arrow->Draw();
   line = new TLine(0,1,1.8,1);
   line->SetLineStyle(7);
   line->Draw();
      tex = new TLatex(0.42,0.7842,"Cent.");
tex->SetNDC();
   tex->SetTextAlign(22);
   tex->SetTextFont(42);
   tex->SetTextSize(0.19);
   tex->SetLineWidth(2);
   tex->Draw();
      tex = new TLatex(0.42,0.7284,"0-100%");
tex->SetNDC();
   tex->SetTextAlign(22);
   tex->SetTextFont(42);
   tex->SetTextSize(0.19);
   tex->SetLineWidth(2);
   tex->Draw();
   pad_int->Modified();
   c1->cd();
   c1->Modified();
   c1->cd();
   c1->SetSelected(c1);
   c1->SaveAs("Figure5.pdf");
}
