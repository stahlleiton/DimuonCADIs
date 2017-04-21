#if !defined(__CINT__) || defined(__MAKECINT__)
#include <Riostream.h>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "TROOT.h"
#include "TStyle.h"

#include "TFile.h"
#include "TArrow.h"
#include "TBox.h"
#include "TCanvas.h"
#include "TColor.h"
#include "TF1.h"
#include "TH1.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TLegendEntry.h"
#include "TLine.h"

#include "../CMS_lumi.C"
#include "../tdrstyle.C"
#include "../textPosition.h"

#endif
// all minbias numbers
void bindingPlot_ups()
{
  bool bSavePlots      = true;
  setTDRStyle();
    
  // Jpsi, psi(2S), Y(1S), Y(2S), Y(3S)(upper limit)
  double xBindingEn[] = {0.64, 0.05, 1.10, 0.54, 0.20};
  double xRadius[]    = {0.25, 0.45, 0.28, 0.56, 0.78};
  double x_err[]      = {0.0, 0.0, 0.0, 0.0, 0.0};
  double x_sys[]      = {0.02, 0.02, 0.02, 0.02, 0.02};

  // 1s and 2s
  double ups276_cent0100_pt020_y024[]      = { 0.453, 0.119};
  double ups276Stat_cent0100_pt020_y024[]  = { 0.014, 0.028};
  double ups276Syst_cent0100_pt020_y024[]  = { 0.048, 0.015};
  double ups3s_snn276_UL[1]                = { 0.145};

  //Y(1S), Y(2S),
  double ups502_cent0100_pt030_y024[]      = { 0.364, 0.104};
  double ups502Stat_cent0100_pt030_y024[]  = { 0.014, 0.021};
  double ups502Syst_cent0100_pt030_y024[]  = { 0.048, 0.014};
  double ups3s_snn502_UL[1]                = { 0.071};

  double arrowEnd = 0.02;
  double xAx[5] = {0};
  double xBindingEnShift[5] = {0};
  for(int i=0; i<5; i++)
    {
      xAx[i]             = xBindingEn[i];
      xBindingEnShift[i] = xBindingEn[i]-0.03;
    }
  

  // run 1: 2.76tev results
  TLine *gUps3sRun1_limit       = new TLine(xAx[4]-arrowEnd,ups3s_snn276_UL[0] ,xAx[4]+arrowEnd,ups3s_snn276_UL[0]);
  TArrow *gUps3sRun1_limitArrow = new TArrow(xAx[4],        ups3s_snn276_UL[0], xAx[4],       0.03,0.015,"|>");
  
  TGraphErrors *gUps1Run1 = new TGraphErrors(1, &xAx[2], &ups276_cent0100_pt020_y024[0],     &x_err[2], &ups276Stat_cent0100_pt020_y024[0]);
  TGraphErrors *gUps2Run1 = new TGraphErrors(1, &xAx[3], &ups276_cent0100_pt020_y024[1],     &x_err[3], &ups276Stat_cent0100_pt020_y024[1]);
  TGraphErrors *gUps3Run1 = new TGraphErrors(1, &xAx[4], 0,     &x_err[3], 0);

  TGraphErrors *gUps1Run1_sys = new TGraphErrors(1, &xAx[2], &ups276_cent0100_pt020_y024[0], &x_sys[2], &ups276Syst_cent0100_pt020_y024[0]);
  TGraphErrors *gUps2Run1_sys = new TGraphErrors(1, &xAx[3], &ups276_cent0100_pt020_y024[1], &x_sys[3], &ups276Syst_cent0100_pt020_y024[1]);
 
  TGraphErrors *gUps1Run1_c = new TGraphErrors(1, &xAx[2], &ups276_cent0100_pt020_y024[0],   &x_err[2], &ups276Stat_cent0100_pt020_y024[0]);
  TGraphErrors *gUps2Run1_c = new TGraphErrors(1, &xAx[3], &ups276_cent0100_pt020_y024[1],   &x_err[3], &ups276Stat_cent0100_pt020_y024[1]);

  gUps3sRun1_limit->SetLineWidth(3);
  gUps3sRun1_limit->SetLineColor(kBlue+2);
  gUps3sRun1_limitArrow->SetLineWidth(3);
  gUps3sRun1_limitArrow->SetLineColor(kBlue+2);

 
  gUps1Run1_sys->SetFillColor(kBlue-10);
  gUps2Run1_sys->SetFillColor(kBlue-10);
  
  gUps1Run1->SetMarkerStyle(29);
  gUps2Run1->SetMarkerStyle(34);
  gUps3Run1->SetMarkerStyle(23);
  
  gUps1Run1->SetMarkerColor(kBlue+2);
  gUps2Run1->SetMarkerColor(kBlue+2);
  gUps3Run1->SetMarkerColor(kBlue+2);
  gUps1Run1->SetMarkerSize(1.7);
  gUps2Run1->SetMarkerSize(1.7);
  gUps3Run1->SetMarkerSize(1.7);

  gUps1Run1_c->SetMarkerStyle(30);
  gUps2Run1_c->SetMarkerStyle(28);
  gUps1Run1_c->SetMarkerSize(1.7);
  gUps2Run1_c->SetMarkerSize(1.7);
  //--------- 
  // run 2: 5.02tev results
  TGraphErrors *gUps1 = new TGraphErrors(1, &xBindingEnShift[2], &ups502_cent0100_pt030_y024[0],     &x_err[2], &ups502Stat_cent0100_pt030_y024[0]);
  TGraphErrors *gUps2 = new TGraphErrors(1, &xBindingEnShift[3], &ups502_cent0100_pt030_y024[1],     &x_err[3], &ups502Stat_cent0100_pt030_y024[1]);
  TGraphErrors *gUps3 = new TGraphErrors(1, &xBindingEnShift[3], 0,     &x_err[3], 0);
  
  TGraphErrors *gUps1_sys = new TGraphErrors(1, &xBindingEnShift[2], &ups502_cent0100_pt030_y024[0], &x_sys[2], &ups502Syst_cent0100_pt030_y024[0]);
  TGraphErrors *gUps2_sys = new TGraphErrors(1, &xBindingEnShift[3], &ups502_cent0100_pt030_y024[1], &x_sys[3], &ups502Syst_cent0100_pt030_y024[1]);

  TGraphErrors *gUps1_c = new TGraphErrors(1, &xBindingEnShift[2], &ups502_cent0100_pt030_y024[0],   &x_err[2], &ups502Stat_cent0100_pt030_y024[0]);
  TGraphErrors *gUps2_c = new TGraphErrors(1, &xBindingEnShift[3], &ups502_cent0100_pt030_y024[1],   &x_err[3], &ups502Stat_cent0100_pt030_y024[1]);

  TLine *gUps3s_limit       = new TLine(xBindingEnShift[4]-arrowEnd,ups3s_snn502_UL[0], xBindingEnShift[4]+arrowEnd,ups3s_snn502_UL[0]);
  TArrow *gUps3s_limitArrow = new TArrow(xBindingEnShift[4],        ups3s_snn502_UL[0], xBindingEnShift[4],0.03,0.015,"|>");
       
  gUps3s_limit->SetLineWidth(3);
  gUps3s_limit->SetLineColor(kRed-2);
  gUps3s_limitArrow->SetLineWidth(3);
  gUps3s_limitArrow->SetLineColor(kRed-2);

 
  gUps1_sys->SetFillColor(kRed-10);
  gUps2_sys->SetFillColor(kRed-10);
  gUps1->SetMarkerStyle(29);
  gUps1_c->SetMarkerStyle(30);
  gUps2->SetMarkerStyle(34);
  gUps2_c->SetMarkerStyle(28);
  gUps3->SetMarkerStyle(23);
  gUps1->SetMarkerColor(kRed+2);
  gUps2->SetMarkerColor(kRed+2);
  gUps3->SetMarkerColor(kRed-2);
  gUps1->SetMarkerSize(1.7);
  gUps2->SetMarkerSize(1.7);
  gUps3->SetMarkerSize(1.7);
  gUps1_c->SetMarkerSize(1.7);
  gUps2_c->SetMarkerSize(1.7);


  //----------------------------- legends
  TLegend *legBeauty = new TLegend(0.2,0.38,0.7,0.58,NULL,"brNDC");
  legBeauty->AddEntry(gUps1,"#varUpsilon(1S)","P");
  legBeauty->AddEntry(gUps2,"#varUpsilon(2S)","P");
  legBeauty->AddEntry(gUps3,"#varUpsilon(3S) 95% C.L.","P");

  
  TLegend *legRun1 = new TLegend(0.16,0.6,0.4,0.75,NULL,"brNDC");
  legRun1->SetFillStyle(0);
  legRun1->SetFillColor(0);
  legRun1->SetBorderSize(0);
  legRun1->SetMargin(0.15);
  legRun1->SetTextColor(kBlue+2);
  TLegendEntry *entry_r1;
  entry_r1=legRun1->AddEntry(gUps3,"2.76 TeV","");
  entry_r1->SetTextFont(62);
  entry_r1->SetTextSize(0.04);
  entry_r1=legRun1->AddEntry(gUps3,"p_{T}#in [0,20] GeV/c, |y| < 2.4","");
  entry_r1->SetTextFont(42);
  entry_r1->SetTextSize(0.035);
  entry_r1=legRun1->AddEntry(gUps3,"arXiv:1611.01510","");
  entry_r1->SetTextFont(42);
  entry_r1->SetTextSize(0.035);

  TLegend *legRun2 = new TLegend(0.54,0.6,0.78,0.75,NULL,"brNDC");
  legRun2->SetFillStyle(0);
  legRun2->SetFillColor(0);
  legRun2->SetBorderSize(0);
  legRun2->SetMargin(0.15);
  legRun2->SetTextColor(kRed+2);
  TLegendEntry *entry_r2;
  entry_r2=legRun2->AddEntry(gUps3,"5.02 TeV","");
  entry_r2->SetTextFont(62);
  entry_r2->SetTextSize(0.04);
  entry_r2=legRun2->AddEntry(gUps3,"p_{T}#in [0,30] GeV/c, |y| < 2.4","");
  entry_r2->SetTextFont(42);
  entry_r2->SetTextSize(0.035);
  entry_r2=legRun2->AddEntry(gUps3,"HIN-16-023","");
  entry_r2->SetTextFont(42);
  entry_r2->SetTextSize(0.035);
  
   //------------------------------------- drawing
  TH1F *phAxis = new TH1F("phAxis",";Binding energy [GeV];R_{AA}",1,0,1.2);
  phAxis->GetYaxis()->SetRangeUser(0.0,1.2);
  phAxis->GetXaxis()->CenterTitle(kTRUE);
  phAxis->GetYaxis()->CenterTitle(kTRUE);
  
  TLine *line = new TLine(0.,1,1.2,1);
  line->SetLineStyle(1);
  line->SetLineWidth(1);

  // vs binding energy
  TCanvas *c1 = new TCanvas("c1","c1");
  phAxis->Draw();
  line->Draw();
  
  gUps1_sys->Draw("2");
  gUps2_sys->Draw("2");
  gUps1->Draw("P");
  gUps2->Draw("P");
  gUps1_c->Draw("P");
  gUps2_c->Draw("P");
  gUps3s_limit->Draw("D");
  gUps3s_limitArrow->Draw();

  gUps1Run1_sys->Draw("2");
  gUps2Run1_sys->Draw("2");
  gUps1Run1->Draw("P");
  gUps2Run1->Draw("P");
  gUps1Run1_c->Draw("P");
  gUps2Run1_c->Draw("P");
  gUps3sRun1_limit->Draw("D");
  gUps3sRun1_limitArrow->Draw();
  

  // legBeauty->Draw();
  legRun1->Draw();
  legRun2->Draw();

  TLatex *what = new TLatex(0.1,1.1,"CMS");
  what->SetTextFont(42);
  what->SetTextSize(0.05);
  what->DrawLatex(0.97,0.56,"#varUpsilon(1S)");
  what->DrawLatex(0.5, 0.35,"#varUpsilon(2S)");
  what->DrawLatex(0.175, 0.2,"#varUpsilon(3S)");


  //---------------
  TLatex *pre = new TLatex();
  pre->SetTextFont(62);
  pre->DrawLatex(0.99,1.1,"CMS");
  pre->DrawLatex(0.08,1.1,"PbPb");
  pre->SetTextFont(52);
  pre->DrawLatex(0.81,1.02,"Preliminary");

  pre->SetTextFont(42);
  pre->SetTextSize(0.035);
  pre->DrawLatex(0.08,1.02,"Cent. 0-100%");
  
  gPad->RedrawAxis();

  if(bSavePlots )
    {
      c1->SaveAs(Form("figsQM17/raaVSdE_ups_run12.pdf"));
      c1->SaveAs(Form("figsQM17/raaVSdE_ups_run12.png"));
    }

}
