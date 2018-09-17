#include "tdrstyle.C"
void Figure4_a()
{
//=========Macro generated from canvas: c1/c1
//=========  (Fri Sep 14 13:48:16 2018) by ROOT version 6.14/04
   setTDRStyle();
   TCanvas *c1 = new TCanvas("c1", "c1",0,23,600,600);
   gStyle->SetOptFit(1);
   gStyle->SetOptStat(0);
   gStyle->SetOptTitle(0);
   c1->Range(-5.940594,-0.201261,31.18812,1.236318);
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
   
   Double_t gRAA_pt_fx1001[6] = {
   1,
   3,
   5,
   7.5,
   10.5,
   21};
   Double_t gRAA_pt_fy1001[6] = {
   0.3013372,
   0.3618014,
   0.3876419,
   0.4016941,
   0.4222127,
   0.4245017};
   Double_t gRAA_pt_fex1001[6] = {
   1,
   1,
   1,
   1.5,
   1.5,
   9};
   Double_t gRAA_pt_fey1001[6] = {
   0.1230058,
   0.03496198,
   0.03526376,
   0.03157579,
   0.04486596,
   0.03583539};
   TGraphErrors *gre = new TGraphErrors(6,gRAA_pt_fx1001,gRAA_pt_fy1001,gRAA_pt_fex1001,gRAA_pt_fey1001);
   gre->SetName("gRAA_pt");
   gre->SetTitle("raa_vs_pt");

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = 1181;
   color = new TColor(ci, 1, 0.8, 0.8, " ", 0.5);
   gre->SetFillColor(ci);

   ci = TColor::GetColor("#990033");
   gre->SetLineColor(ci);
   gre->SetMarkerColor(2);
   gre->SetMarkerStyle(20);
   
   TH1F *Graph_gRAA_pt1001 = new TH1F("Graph_gRAA_pt1001","raa_vs_pt",100,0,30);
   Graph_gRAA_pt1001->SetMinimum(0);
   Graph_gRAA_pt1001->SetMaximum(1.14);
   Graph_gRAA_pt1001->SetDirectory(0);
   Graph_gRAA_pt1001->SetStats(0);
   Graph_gRAA_pt1001->SetLineStyle(0);
   Graph_gRAA_pt1001->SetMarkerStyle(20);
   Graph_gRAA_pt1001->GetXaxis()->SetTitle("p_{T} (GeV)");
   Graph_gRAA_pt1001->GetXaxis()->CenterTitle(true);
   Graph_gRAA_pt1001->GetXaxis()->SetLabelFont(42);
   Graph_gRAA_pt1001->GetXaxis()->SetLabelOffset(0.007);
   Graph_gRAA_pt1001->GetXaxis()->SetLabelSize(0.05);
   Graph_gRAA_pt1001->GetXaxis()->SetTitleSize(0.06);
   Graph_gRAA_pt1001->GetXaxis()->SetTitleOffset(0.9);
   Graph_gRAA_pt1001->GetXaxis()->SetTitleFont(42);
   Graph_gRAA_pt1001->GetYaxis()->SetTitle("R_{AA}");
   Graph_gRAA_pt1001->GetYaxis()->CenterTitle(true);
   Graph_gRAA_pt1001->GetYaxis()->SetLabelFont(42);
   Graph_gRAA_pt1001->GetYaxis()->SetLabelOffset(0.007);
   Graph_gRAA_pt1001->GetYaxis()->SetLabelSize(0.05);
   Graph_gRAA_pt1001->GetYaxis()->SetTitleSize(0.06);
   Graph_gRAA_pt1001->GetYaxis()->SetTitleOffset(1.25);
   Graph_gRAA_pt1001->GetYaxis()->SetTitleFont(42);
   Graph_gRAA_pt1001->GetZaxis()->SetLabelFont(42);
   Graph_gRAA_pt1001->GetZaxis()->SetLabelOffset(0.007);
   Graph_gRAA_pt1001->GetZaxis()->SetLabelSize(0.05);
   Graph_gRAA_pt1001->GetZaxis()->SetTitleSize(0.06);
   Graph_gRAA_pt1001->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_gRAA_pt1001);
   
   gre->Draw("a5");
   
   Double_t gRAA_pt_fx1002[3] = {
   2,
   6.5,
   19.5};
   Double_t gRAA_pt_fy1002[3] = {
   0.084637,
   0.1283587,
   0.1257775};
   Double_t gRAA_pt_fex1002[3] = {
   2,
   2.5,
   10.5};
   Double_t gRAA_pt_fey1002[3] = {
   0.09528671,
   0.0307109,
   0.01611695};
   gre = new TGraphErrors(3,gRAA_pt_fx1002,gRAA_pt_fy1002,gRAA_pt_fex1002,gRAA_pt_fey1002);
   gre->SetName("gRAA_pt");
   gre->SetTitle("raa_vs_pt");

   ci = 1182;
   color = new TColor(ci, 0.8, 0.8, 1, " ", 0.5);
   gre->SetFillColor(ci);

   ci = TColor::GetColor("#3333cc");
   gre->SetLineColor(ci);
   gre->SetMarkerColor(2);
   gre->SetMarkerStyle(20);
   
   TH1F *Graph_gRAA_pt1002 = new TH1F("Graph_gRAA_pt1002","raa_vs_pt",100,0,33);
   Graph_gRAA_pt1002->SetMinimum(-0.02970705);
   Graph_gRAA_pt1002->SetMaximum(0.198981);
   Graph_gRAA_pt1002->SetDirectory(0);
   Graph_gRAA_pt1002->SetStats(0);
   Graph_gRAA_pt1002->SetLineStyle(0);
   Graph_gRAA_pt1002->SetMarkerStyle(20);
   Graph_gRAA_pt1002->GetXaxis()->SetLabelFont(42);
   Graph_gRAA_pt1002->GetXaxis()->SetLabelOffset(0.007);
   Graph_gRAA_pt1002->GetXaxis()->SetLabelSize(0.05);
   Graph_gRAA_pt1002->GetXaxis()->SetTitleSize(0.06);
   Graph_gRAA_pt1002->GetXaxis()->SetTitleOffset(0.9);
   Graph_gRAA_pt1002->GetXaxis()->SetTitleFont(42);
   Graph_gRAA_pt1002->GetYaxis()->SetLabelFont(42);
   Graph_gRAA_pt1002->GetYaxis()->SetLabelOffset(0.007);
   Graph_gRAA_pt1002->GetYaxis()->SetLabelSize(0.05);
   Graph_gRAA_pt1002->GetYaxis()->SetTitleSize(0.06);
   Graph_gRAA_pt1002->GetYaxis()->SetTitleOffset(1.25);
   Graph_gRAA_pt1002->GetYaxis()->SetTitleFont(42);
   Graph_gRAA_pt1002->GetZaxis()->SetLabelFont(42);
   Graph_gRAA_pt1002->GetZaxis()->SetLabelOffset(0.007);
   Graph_gRAA_pt1002->GetZaxis()->SetLabelSize(0.05);
   Graph_gRAA_pt1002->GetZaxis()->SetTitleSize(0.06);
   Graph_gRAA_pt1002->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_gRAA_pt1002);
   
   gre->Draw("5");
   
   Double_t gRAA_pt_0s_fx1003[6] = {
   1,
   3,
   5,
   7.5,
   10.5,
   21};
   Double_t gRAA_pt_0s_fy1003[6] = {
   0.3013372,
   0.3618014,
   0.3876419,
   0.4016941,
   0.4222127,
   0.4245017};
   Double_t gRAA_pt_0s_fex1003[6] = {
   0,
   0,
   0,
   0,
   0,
   0};
   Double_t gRAA_pt_0s_fey1003[6] = {
   0.02646233,
   0.02889847,
   0.03269219,
   0.0279548,
   0.03446754,
   0.02547082};
   gre = new TGraphErrors(6,gRAA_pt_0s_fx1003,gRAA_pt_0s_fy1003,gRAA_pt_0s_fex1003,gRAA_pt_0s_fey1003);
   gre->SetName("gRAA_pt_0s");
   gre->SetTitle("raa_vs_pt");

   ci = TColor::GetColor("#990033");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#990033");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   gre->SetMarkerSize(1.5);
   
   TH1F *Graph_gRAA_pt_0s1003 = new TH1F("Graph_gRAA_pt_0s1003","raa_vs_pt",100,0,23);
   Graph_gRAA_pt_0s1003->SetMinimum(0.2566943);
   Graph_gRAA_pt_0s1003->SetMaximum(0.4748607);
   Graph_gRAA_pt_0s1003->SetDirectory(0);
   Graph_gRAA_pt_0s1003->SetStats(0);
   Graph_gRAA_pt_0s1003->SetLineStyle(0);
   Graph_gRAA_pt_0s1003->SetMarkerStyle(20);
   Graph_gRAA_pt_0s1003->GetXaxis()->SetLabelFont(42);
   Graph_gRAA_pt_0s1003->GetXaxis()->SetLabelOffset(0.007);
   Graph_gRAA_pt_0s1003->GetXaxis()->SetLabelSize(0.05);
   Graph_gRAA_pt_0s1003->GetXaxis()->SetTitleSize(0.06);
   Graph_gRAA_pt_0s1003->GetXaxis()->SetTitleOffset(0.9);
   Graph_gRAA_pt_0s1003->GetXaxis()->SetTitleFont(42);
   Graph_gRAA_pt_0s1003->GetYaxis()->SetLabelFont(42);
   Graph_gRAA_pt_0s1003->GetYaxis()->SetLabelOffset(0.007);
   Graph_gRAA_pt_0s1003->GetYaxis()->SetLabelSize(0.05);
   Graph_gRAA_pt_0s1003->GetYaxis()->SetTitleSize(0.06);
   Graph_gRAA_pt_0s1003->GetYaxis()->SetTitleOffset(1.25);
   Graph_gRAA_pt_0s1003->GetYaxis()->SetTitleFont(42);
   Graph_gRAA_pt_0s1003->GetZaxis()->SetLabelFont(42);
   Graph_gRAA_pt_0s1003->GetZaxis()->SetLabelOffset(0.007);
   Graph_gRAA_pt_0s1003->GetZaxis()->SetLabelSize(0.05);
   Graph_gRAA_pt_0s1003->GetZaxis()->SetTitleSize(0.06);
   Graph_gRAA_pt_0s1003->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_gRAA_pt_0s1003);
   
   gre->Draw("p");
   
   Double_t gRAA_pt_1s_fx1004[3] = {
   2,
   6.5,
   19.5};
   Double_t gRAA_pt_1s_fy1004[3] = {
   0.084637,
   0.1283587,
   0.1257775};
   Double_t gRAA_pt_1s_fex1004[3] = {
   0,
   0,
   0};
   Double_t gRAA_pt_1s_fey1004[3] = {
   0.03907188,
   0.0376192,
   0.03235042};
   gre = new TGraphErrors(3,gRAA_pt_1s_fx1004,gRAA_pt_1s_fy1004,gRAA_pt_1s_fex1004,gRAA_pt_1s_fey1004);
   gre->SetName("gRAA_pt_1s");
   gre->SetTitle("raa_vs_pt");

   ci = TColor::GetColor("#3333cc");
   gre->SetLineColor(ci);

   ci = TColor::GetColor("#3333cc");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(21);
   gre->SetMarkerSize(1.5);
   
   TH1F *Graph_gRAA_pt_1s1004 = new TH1F("Graph_gRAA_pt_1s1004","raa_vs_pt",100,0.25,21.25);
   Graph_gRAA_pt_1s1004->SetMinimum(0.03352384);
   Graph_gRAA_pt_1s1004->SetMaximum(0.1780191);
   Graph_gRAA_pt_1s1004->SetDirectory(0);
   Graph_gRAA_pt_1s1004->SetStats(0);
   Graph_gRAA_pt_1s1004->SetLineStyle(0);
   Graph_gRAA_pt_1s1004->SetMarkerStyle(20);
   Graph_gRAA_pt_1s1004->GetXaxis()->SetLabelFont(42);
   Graph_gRAA_pt_1s1004->GetXaxis()->SetLabelOffset(0.007);
   Graph_gRAA_pt_1s1004->GetXaxis()->SetLabelSize(0.05);
   Graph_gRAA_pt_1s1004->GetXaxis()->SetTitleSize(0.06);
   Graph_gRAA_pt_1s1004->GetXaxis()->SetTitleOffset(0.9);
   Graph_gRAA_pt_1s1004->GetXaxis()->SetTitleFont(42);
   Graph_gRAA_pt_1s1004->GetYaxis()->SetLabelFont(42);
   Graph_gRAA_pt_1s1004->GetYaxis()->SetLabelOffset(0.007);
   Graph_gRAA_pt_1s1004->GetYaxis()->SetLabelSize(0.05);
   Graph_gRAA_pt_1s1004->GetYaxis()->SetTitleSize(0.06);
   Graph_gRAA_pt_1s1004->GetYaxis()->SetTitleOffset(1.25);
   Graph_gRAA_pt_1s1004->GetYaxis()->SetTitleFont(42);
   Graph_gRAA_pt_1s1004->GetZaxis()->SetLabelFont(42);
   Graph_gRAA_pt_1s1004->GetZaxis()->SetLabelOffset(0.007);
   Graph_gRAA_pt_1s1004->GetZaxis()->SetLabelSize(0.05);
   Graph_gRAA_pt_1s1004->GetZaxis()->SetTitleSize(0.06);
   Graph_gRAA_pt_1s1004->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_gRAA_pt_1s1004);
   
   gre->Draw("p");
   TBox *box = new TBox(2.4,0,3.6,0.09157219);

   ci = 1179;
   color = new TColor(ci, 0.4, 0.8, 0.4, " ", 0.5);
   box->SetFillColor(ci);

   ci = TColor::GetColor("#006600");
   box->SetLineColor(ci);
   box->Draw("l");
   TArrow *arrow = new TArrow(3,0,3,0.1553818,0.027,"<-|");
   arrow->SetFillColor(1);
   arrow->SetFillStyle(1001);

   ci = TColor::GetColor("#009900");
   arrow->SetLineColor(ci);
   arrow->SetLineWidth(2);
   arrow->Draw();
   box = new TBox(17.4,0,18.6,0.05145168);

   ci = 1180;
   color = new TColor(ci, 0.4, 0.8, 0.4, " ", 0.5);
   box->SetFillColor(ci);

   ci = TColor::GetColor("#006600");
   box->SetLineColor(ci);
   box->Draw("l");
   arrow = new TArrow(18,0,18,0.09462273,0.027,"<-|");
   arrow->SetFillColor(1);
   arrow->SetFillStyle(1001);

   ci = TColor::GetColor("#009900");
   arrow->SetLineColor(ci);
   arrow->SetLineWidth(2);
   arrow->Draw();
   TLine *line = new TLine(0,1,30,1);
   line->SetLineStyle(7);
   line->Draw();
   
   TLegend *leg = new TLegend(0.57,0.5,0.78,0.615,NULL,"brNDC");
   leg->SetBorderSize(0);
   leg->SetTextSize(0.04);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(4000);
   leg->SetMargin(0.2);
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
   leg->SetBorderSize(0);
   leg->SetTextSize(0.04);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(4000);
   entry=leg->AddEntry("gRAA_pt_0s"," #varUpsilon(1S)","lp");
   leg->SetMargin(0.2);

   ci = TColor::GetColor("#990033");
   entry->SetLineColor(ci);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);

   ci = TColor::GetColor("#990033");
   entry->SetMarkerColor(ci);
   entry->SetMarkerStyle(20);
   entry->SetMarkerSize(1.5);
   entry->SetTextFont(42);
   entry=leg->AddEntry("gRAA_pt_1s"," #varUpsilon(2S)","lp");

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
   
   leg = new TLegend(0.57,0.5,0.78,0.615,NULL,"brNDC");
   leg->SetBorderSize(0);
   leg->SetTextSize(0.04);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(4000);
   entry=leg->AddEntry("NULL"," #varUpsilon(3S) 68% CL","f");
   leg->SetMargin(0.2);

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
   arrow = new TArrow(16,0.532,16,0.582,0.02,"<-|");
   arrow->SetFillColor(1);
   arrow->SetFillStyle(1001);

   ci = TColor::GetColor("#009900");
   arrow->SetLineColor(ci);
   arrow->SetLineWidth(2);
   arrow->Draw();
   TLatex *   tex = new TLatex(0.22,0.8715,"|y| < 2.4");
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
   box = new TBox(29.2,0.953256,30,1.042579);

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
   c1->SaveAs("Figure4_a.pdf");
}
