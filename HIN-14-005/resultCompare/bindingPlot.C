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
void bindingPlot()
{
  
  bool bSavePlots      = true;
  bool bDo502          = false;
  bool bJustBeauty     = false;
  bool bJustCharm      = false;

  setTDRStyle();
    
  // Jpsi, psi(2S), Y(1S), Y(2S), Y(3S)(upper limit)
  double xBindingEn[] = {0.64, 0.05, 1.10, 0.54, 0.20};
  double xRadius[]    = {0.25, 0.45, 0.28, 0.56, 0.78};
  double x_err[]      = {0.0, 0.0, 0.0, 0.0, 0.0};
  double x_sys[]      = {0.02, 0.02, 0.02, 0.02, 0.02};

  double jpsiPr276_cent05_pt6530_y024[]     = {0.282};
  double jpsiPr276Stat_cent05_pt6530_y024[] = {0.010};
  double jpsiPr276Syst_cent05_pt6530_y024[] = {0.023};

  double psiPPr276_cent0100_pt6530_y016[]     = {0.13};
  double psiPPr276Stat_cent0100_pt6530_y016[] = {0.04};
  double psiPPr276Syst_cent0100_pt6530_y016[] = {0.02};

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
  
  double xAx[5];
  for(int i=0; i<5; i++)xAx[i] = xBindingEn[i];
  double arrowEnd = 0.02;

  
  TLine *gUps3s_limit = new TLine(xAx[4]-arrowEnd,ups3s_snn276_UL[0],xAx[4]+arrowEnd,ups3s_snn276_UL[0]);
  TArrow *gUps3s_limitArrow = new TArrow( xAx[4],ups3s_snn276_UL[0], xAx[4],0.03,0.015,"|>");
  
  TGraphErrors *gJpsi = new TGraphErrors(1, &xAx[0], &jpsiPr276_cent05_pt6530_y024[0],   &x_err[0], &jpsiPr276Stat_cent05_pt6530_y024[0]);
  TGraphErrors *gPsiP = new TGraphErrors(1, &xAx[1], &psiPPr276_cent0100_pt6530_y016[0], &x_err[1], &psiPPr276Stat_cent0100_pt6530_y016[0]);
  TGraphErrors *gUps1 = new TGraphErrors(1, &xAx[2], &ups276_cent0100_pt020_y024[0],     &x_err[2], &ups276Stat_cent0100_pt020_y024[0]);
  TGraphErrors *gUps2 = new TGraphErrors(1, &xAx[3], &ups276_cent0100_pt020_y024[1],     &x_err[3], &ups276Stat_cent0100_pt020_y024[1]);
  TGraphErrors *gUps3 = new TGraphErrors(1, &xAx[3], 0,     &x_err[3], 0);


  TGraphErrors *gJpsi_sys = new TGraphErrors(1, &xAx[0], &jpsiPr276_cent05_pt6530_y024[0],   &x_sys[0], &jpsiPr276Syst_cent05_pt6530_y024[0]);
  TGraphErrors *gPsiP_sys = new TGraphErrors(1, &xAx[1], &psiPPr276_cent0100_pt6530_y016[0], &x_sys[1], &psiPPr276Syst_cent0100_pt6530_y016[0]);
  TGraphErrors *gUps1_sys = new TGraphErrors(1, &xAx[2], &ups276_cent0100_pt020_y024[0],     &x_sys[2], &ups276Syst_cent0100_pt020_y024[0]);
  TGraphErrors *gUps2_sys = new TGraphErrors(1, &xAx[3], &ups276_cent0100_pt020_y024[1],     &x_sys[3], &ups276Syst_cent0100_pt020_y024[1]);


  TGraphErrors *gJpsi_c = new TGraphErrors(1, &xAx[0], &jpsiPr276_cent05_pt6530_y024[0],   &x_err[0], &jpsiPr276Stat_cent05_pt6530_y024[0]);
  TGraphErrors *gPsiP_c = new TGraphErrors(1, &xAx[1], &psiPPr276_cent0100_pt6530_y016[0], &x_err[1], &psiPPr276Stat_cent0100_pt6530_y016[0]);
  TGraphErrors *gUps1_c = new TGraphErrors(1, &xAx[2], &ups276_cent0100_pt020_y024[0],     &x_err[2], &ups276Stat_cent0100_pt020_y024[0]);
  TGraphErrors *gUps2_c = new TGraphErrors(1, &xAx[3], &ups276_cent0100_pt020_y024[1],     &x_err[3], &ups276Stat_cent0100_pt020_y024[1]);
 
  if(bDo502)
    {
      gUps1 = new TGraphErrors(1, &xAx[2], &ups502_cent0100_pt030_y024[0],     &x_err[2], &ups502Stat_cent0100_pt030_y024[0]);
      gUps2 = new TGraphErrors(1, &xAx[3], &ups502_cent0100_pt030_y024[1],     &x_err[3], &ups502Stat_cent0100_pt030_y024[1]);

      gUps1_sys = new TGraphErrors(1, &xAx[2], &ups502_cent0100_pt030_y024[0], &x_sys[2], &ups502Syst_cent0100_pt030_y024[0]);
      gUps2_sys = new TGraphErrors(1, &xAx[3], &ups502_cent0100_pt030_y024[1], &x_sys[3], &ups502Syst_cent0100_pt030_y024[1]);

      gUps1_c = new TGraphErrors(1, &xAx[2], &ups502_cent0100_pt030_y024[0],   &x_err[2], &ups502Stat_cent0100_pt030_y024[0]);
      gUps2_c = new TGraphErrors(1, &xAx[3], &ups502_cent0100_pt030_y024[1],   &x_err[3], &ups502Stat_cent0100_pt030_y024[1]);

      gUps3s_limit = new TLine(xAx[4]-arrowEnd,ups3s_snn502_UL[0],xAx[4]+arrowEnd,ups3s_snn502_UL[0]);
      gUps3s_limitArrow = new TArrow( xAx[4],ups3s_snn502_UL[0], xAx[4],0.03,0.015,"|>");
    }
  gUps3s_limit->SetLineWidth(3);
  gUps3s_limit->SetLineColor(kViolet-2);
  gUps3s_limitArrow->SetLineWidth(3);
  gUps3s_limitArrow->SetLineColor(kViolet-2);

  gJpsi_sys->SetFillColor(kRed-9);
  gPsiP_sys->SetFillColor(kOrange-9);
  gUps1_sys->SetFillColor(kGreen-9);
  gUps2_sys->SetFillColor(kBlue-9);


  gJpsi->SetMarkerStyle(21);
  gJpsi_c->SetMarkerStyle(25);

  gPsiP->SetMarkerStyle(20);
  gPsiP_c->SetMarkerStyle(24);


  gUps1->SetMarkerStyle(29);
  gUps1_c->SetMarkerStyle(30);
  
  gUps2->SetMarkerStyle(34);
  gUps2_c->SetMarkerStyle(28);

  gUps3->SetMarkerStyle(23);
 
 
  gJpsi->SetMarkerColor(kRed+2);
  gPsiP->SetMarkerColor(kOrange+2);
  gUps1->SetMarkerColor(kGreen+2);
  gUps2->SetMarkerColor(kBlue+2);
  gUps3->SetMarkerColor(kViolet-2);

  gJpsi->SetMarkerSize(1.2);
  gPsiP->SetMarkerSize(1.2);
  gUps1->SetMarkerSize(1.7);
  gUps2->SetMarkerSize(1.7);
  gUps3->SetMarkerSize(1.7);
  
   
  gJpsi_c->SetMarkerSize(1.2);
  gPsiP_c->SetMarkerSize(1.2);
  gUps1_c->SetMarkerSize(1.7);
  gUps2_c->SetMarkerSize(1.7);
 
  //----------------------------- legends
  TLegend *legCharm = new TLegend(0.2,0.58,0.7,0.785,NULL,"brNDC");
  legCharm->SetHeader("Prompt charmonia ");
  legCharm->AddEntry(gJpsi,"J/#psi       (arXiv:1610.00613)","P");
  legCharm->AddEntry(gJpsi,"    p_{T}#in [6.5,30] GeV/c, |y| < 2.4, Cent. 0-5%","");
  // legCharm->AddEntry(gJpsi,"    arXiv:1610.00613","");
  legCharm->AddEntry(gPsiP,"#psi(2S)   (PRL 113, 2014)","P");
  legCharm->AddEntry(gPsiP,"    p_{T}#in [6.5,30] GeV/c, |y| < 1.6","");
  // legCharm->AddEntry(gPsiP,"    PRL 113 (2014)","");

  legCharm->SetFillStyle(0);
  legCharm->SetFillColor(0);
  legCharm->SetBorderSize(0);
  legCharm->SetMargin(0.15);
  legCharm->SetTextFont(42);
  legCharm->SetTextSize(0.035);

  TLegend *legBeauty = new TLegend(0.2,0.38,0.7,0.58,NULL,"brNDC");
  if(bDo502 || bJustBeauty) legBeauty = new TLegend(0.2,0.5,0.7,0.75,NULL,"brNDC");
  legBeauty->SetHeader("Bottomonia");
  if(!bDo502)
    {
      legBeauty->AddEntry(gUps3,"p_{T}#in [0,20] GeV/c, |y| < 2.4","");
      legBeauty->AddEntry(gUps3,"arXiv:1611.01510","");
    }
  else
    {
      legBeauty->AddEntry(gUps3,"p_{T}#in [0,30] GeV/c, |y| < 2.4","");
      legBeauty->AddEntry(gUps3,"HIN-16-023","");

    }
  legBeauty->AddEntry(gUps1,"#varUpsilon(1S)","P");
  legBeauty->AddEntry(gUps2,"#varUpsilon(2S)","P");
  legBeauty->AddEntry(gUps3,"#varUpsilon(3S) 95% C.L.","P");

  legBeauty->SetFillStyle(0);
  legBeauty->SetFillColor(0);
  legBeauty->SetBorderSize(0);
  legBeauty->SetMargin(0.15);
  legBeauty->SetTextFont(42);
  legBeauty->SetTextSize(0.035);
  
   //------------------------------------- drawing
  TH1F *phAxis = new TH1F("phAxis",";Binding energy [GeV];R_{AA}",1,0,1.2);
  phAxis->GetYaxis()->SetRangeUser(0.0,1.2);
  phAxis->GetXaxis()->CenterTitle(kTRUE);
  phAxis->GetYaxis()->CenterTitle(kTRUE);
  TLine *line = new TLine(0.,1,1.2,1);
  line->SetLineStyle(1);
  line->SetLineWidth(1);

  // vs binding energy
  TLatex *what = new TLatex(0.1,1.1,"CMS");
  what->SetTextFont(42);
  what->SetTextSize(0.04);
  
  TCanvas *c1 = new TCanvas("c1","c1");
  phAxis->Draw();
  line->Draw();
  if(!bJustBeauty || (bJustCharm && !bDo502) )
    {
      gJpsi_sys->Draw("2");
      gPsiP_sys->Draw("2");
      gJpsi->Draw("P");
      gPsiP->Draw("P");
      gJpsi_c->Draw("P");
      gPsiP_c->Draw("P");

      legCharm->Draw();

      if(bJustCharm)
	what->DrawLatex(0.08, 0.17,"#psi(2S)");
      else
	what->DrawLatex(0.05, 0.17,"#psi(2S)");

	what->DrawLatex(0.67, 0.32,"J/#psi");
	
    }
  if(bJustBeauty || !bJustCharm)
     {
       gUps1_sys->Draw("2");
       gUps2_sys->Draw("2");
       gUps1->Draw("P");
       gUps2->Draw("P");
       gUps1_c->Draw("P");
       gUps2_c->Draw("P");
       gUps3s_limit->Draw("D");
       gUps3s_limitArrow->Draw();

       legBeauty->Draw();
       if(bDo502)
	 {
	   what->DrawLatex(1.03,0.45,"#varUpsilon(1S)");
	   what->DrawLatex(0.52, 0.17,"#varUpsilon(2S)");
	   what->DrawLatex(0.22, 0.12,"#varUpsilon(3S)");

	 }
       else
	 {
	   what->DrawLatex(0.94,0.4,"#varUpsilon(1S)");
	   what->DrawLatex(0.55, 0.16,"#varUpsilon(2S)");
	   what->DrawLatex(0.22, 0.16,"#varUpsilon(3S)");
	 }
     }

  //---------------
  TLatex *pre = new TLatex();
  pre->SetTextFont(62);
  if(!bDo502)
    {
      pre->DrawLatex(0.99,1.1,"CMS");
      pre->DrawLatex(0.08,1.1,"PbPb #sqrt{s_{NN}} = 2.76 TeV");
    }
  else
    {
      pre->DrawLatex(0.99,1.1,"CMS");
      pre->DrawLatex(0.08,1.1,"PbPb #sqrt{s_{NN}} = 5.02 TeV");
      pre->SetTextFont(52);
      pre->DrawLatex(0.81,1.02,"Preliminary");
    }
  pre->SetTextFont(42);
  pre->SetTextSize(0.035);
  pre->DrawLatex(0.08,1.02,"Cent. 0-100%");
  
  gPad->RedrawAxis();

  if(bSavePlots )
    {
      if(!bJustBeauty && !bJustCharm)
	{
	  c1->SaveAs(Form("figsQM17/raaVSdE_bDo502%d.pdf",bDo502));
	  c1->SaveAs(Form("figsQM17/raaVSdE_bDo502%d.png",bDo502));
	}
      else
	{
	  c1->SaveAs(Form("figsQM17/raaVSdE_bDo502%d_charm%d_beauty%d.pdf",bDo502,bJustCharm,bJustBeauty));
	  c1->SaveAs(Form("figsQM17/raaVSdE_bDo502%d_charm%d_beauty%d.png",bDo502,bJustCharm,bJustBeauty));
	}
    }

}
