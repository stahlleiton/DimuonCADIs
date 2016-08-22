#if !defined(__CINT__) || defined(__MAKECINT__)
#include <Riostream.h>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "TROOT.h"
#include "TStyle.h"

#include "TAttMarker.h"
#include "TFile.h"
#include "TBox.h"
#include "TCanvas.h"
#include "TColor.h"
#include "TF1.h"
#include "TH1.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TLegendEntry.h"
#include "TLine.h"
#include "TSystem.h"


#include "dats/data_cms2016.h"
#include "dats/data_others2016.h"

#include "../CMS_lumi.C"
#include "../tdrstyle.C"
#include "../textPosition.h"

#endif
void raaTheoryOpen_pt(const char* inputDir = "../macro_raa/outRoot", // the place where the input root files, with the histograms are
                     bool bSavePlots      = true,
                     bool bDoBRaa  = true
                     )
{
   // set the style
  gSystem->mkdir(Form("./figs/png"), kTRUE);
  gSystem->mkdir(Form("./figs/pdf"), kTRUE);
  setTDRStyle();
  
  // read CMS graphs
  TFile *pgRaaCms_pt   = new TFile(Form("%s/makeRaa_pt.root",inputDir));
  // ##################### HIGH PT ############################
  TGraphErrors *pgCms     = (TGraphErrors *)pgRaaCms_pt->Get("gNonPrJpsi");
  TGraphErrors *pgCmsP    = (TGraphErrors *)pgRaaCms_pt->Get("gNonPrJpsiP");
  TGraphErrors *pgCmsSyst = (TGraphErrors *)pgRaaCms_pt->Get("gNonPrJpsiSyst");
  pgCmsSyst->SetFillColorAlpha(kOrange-9,0.5);
  pgCms->SetName("gNonPrJpsi");

  TBox *lumi = (TBox*)pgRaaCms_pt->Get("lumi");
  lumi->SetFillColor(kGray+1);
  lumi->SetFillStyle(1001);
  lumi->SetX1(28.5); lumi->SetX2(30);

  // ##################### LOW PT ############################
  TGraphErrors *pgCms_lowpt     = (TGraphErrors *)pgRaaCms_pt->Get("gNonPrJpsi_pt365y1624");
  TGraphErrors *pgCmsP_lowpt    = (TGraphErrors *)pgRaaCms_pt->Get("gNonPrJpsiP_pt365y1624");
  TGraphErrors *pgCmsSyst_lowpt = (TGraphErrors *)pgRaaCms_pt->Get("gNonPrJpsiSyst_pt365y1624");
  pgCmsSyst_lowpt->SetFillColorAlpha(kViolet-9,0.5);
  pgCms_lowpt->SetName("gNonPrJpsi_pt365y1624");

   
  //-------------------------------------------------------------------- 
  // *********** Theory curves with filled area
  TGraph *pgRaaB_rapp       = new TGraph(2*raaB_rapp_vspt_numpoints);
  pgRaaB_rapp->SetName("pgRaaB_rapp");
  for (int i=0; i<raaB_rapp_vspt_numpoints; i++) {
    pgRaaB_rapp->SetPoint(i,raaB_rapp_pt[i],raaB_rapp_vspt_yhigh[i]);
    pgRaaB_rapp->SetPoint(raaB_rapp_vspt_numpoints+i,
        raaB_rapp_pt[raaB_rapp_vspt_numpoints-i-1],raaB_rapp_vspt_ylow[raaB_rapp_vspt_numpoints-i-1]);
  }

  TGraph *pgRaaB_whdg       = new TGraph(2*raaB_whdg_vspt_numpoints);
  pgRaaB_whdg->SetName("pgRaaB_whdg");
  for (int i=0; i<raaB_whdg_vspt_numpoints; i++) {
    pgRaaB_whdg->SetPoint(i,raaB_whdg_pt[i],raaB_whdg_vspt_yhigh[i]);
    pgRaaB_whdg->SetPoint(raaB_whdg_vspt_numpoints+i,
        raaB_whdg_pt[raaB_whdg_vspt_numpoints-i-1],raaB_whdg_vspt_ylow[raaB_whdg_vspt_numpoints-i-1]);
  }

  TGraph *pgRaaNPJpsi_vitev       = new TGraph(2*raaNPJpsi_vitev_vspt_numpoints);
  pgRaaNPJpsi_vitev->SetName("pgRaaNPJpsi_vitev");
  for (int i=0; i<raaNPJpsi_vitev_vspt_numpoints; i++) {
    pgRaaNPJpsi_vitev->SetPoint(i,raaNPJpsi_vitev_pt[i],raaNPJpsi_vitev_vspt_yhigh[i]);
    pgRaaNPJpsi_vitev->SetPoint(raaNPJpsi_vitev_vspt_numpoints+i,
        raaNPJpsi_vitev_pt[raaNPJpsi_vitev_vspt_numpoints-i-1],raaNPJpsi_vitev_vspt_ylow[raaNPJpsi_vitev_vspt_numpoints-i-1]);
  }

  TGraph *pgRaaNPJpsi_vitev_noDissoc = new TGraph(2*raaNPJpsi_vitev_vspt_noDissoc_numpoints);
  pgRaaNPJpsi_vitev_noDissoc->SetName("pgRaaNPJpsi_vitev_noDissoc");
  for (int i=0; i<raaNPJpsi_vitev_vspt_noDissoc_numpoints; i++) {
    pgRaaNPJpsi_vitev_noDissoc->SetPoint(i,raaNPJpsi_vitev_noDissoc_pt[i],raaNPJpsi_vitev_vspt_noDissoc_yhigh[i]);
    pgRaaNPJpsi_vitev_noDissoc->SetPoint(raaNPJpsi_vitev_vspt_noDissoc_numpoints+i,
        raaNPJpsi_vitev_noDissoc_pt[raaNPJpsi_vitev_vspt_noDissoc_numpoints-i-1],raaNPJpsi_vitev_vspt_noDissoc_ylow[raaNPJpsi_vitev_vspt_noDissoc_numpoints-i-1]);
  }

  // *********** Theory curves with line
  TGraph *pgRaaNPJpsi_mcatshq     = new TGraph(raaNPJpsi_mcatshq_vspt_numpoints,raaNPJpsi_mcatshq_pt,raaNPJpsi_mcatshq_vspt);
  pgRaaNPJpsi_mcatshq->SetName("pgRaaNPJpsi_mcatshq");
  TGraph *pgRaaNPJpsi_mcatshq_rad = new TGraph(raaNPJpsi_mcatshq_rad_vspt_numpoints,raaNPJpsi_mcatshq_rad_pt,raaNPJpsi_mcatshq_rad_vspt);
  pgRaaNPJpsi_mcatshq_rad->SetName("pgRaaNPJpsi_mcatshq_rad");
  TGraph *pgRaaNPJpsi_bamps       = new TGraph(raaNPJpsi_bamps_vspt_numpoints,raaNPJpsi_bamps_pt,raaNPJpsi_bamps_vspt_yval);
  pgRaaNPJpsi_bamps->SetName("pgRaaNPJpsi_bamps");

  // Style for filled graphs
  pgRaaB_rapp->SetFillColor(kRed-7);
  pgRaaB_rapp->SetFillStyle(1001);
  pgRaaB_whdg->SetFillColor(kOrange);
  pgRaaB_whdg->SetFillStyle(1001);

  pgRaaNPJpsi_vitev->SetFillColor(kAzure+1);
  pgRaaNPJpsi_vitev->SetFillStyle(1001);
  pgRaaNPJpsi_vitev_noDissoc->SetFillColor(kAzure-9);
  pgRaaNPJpsi_vitev_noDissoc->SetFillStyle(1001);

  // Style for line graphs
  pgRaaNPJpsi_mcatshq->SetLineColor(kTeal+4);
  pgRaaNPJpsi_mcatshq_rad->SetLineColor(kTeal+3);
  pgRaaNPJpsi_bamps->SetLineColor(kRed+1);
  
//  pgRaaNPJpsi_mcatshq->SetLineStyle(3);
//  pgRaaNPJpsi_mcatshq_rad->SetLineStyle(3);
//  pgRaaNPJpsi_bamps->SetLineStyle(3);
  
  pgRaaNPJpsi_mcatshq->SetLineWidth(4);
  pgRaaNPJpsi_mcatshq_rad->SetLineWidth(4);
  pgRaaNPJpsi_bamps->SetLineWidth(3);


  //---------------------------------------------------------
  TLine *line = new TLine(0.,1,30,1);
  line->SetLineStyle(1);
  line->SetLineWidth(1);

  TCanvas *pc = new TCanvas("pc","pc");

  TF1 *f4 = new TF1("f4","1",0,30);
  f4->SetLineWidth(1);
  f4->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  f4->GetYaxis()->SetTitle("R_{AA}");
  f4->GetYaxis()->SetRangeUser(0.0,1.7);
  f4->GetXaxis()->CenterTitle(kTRUE);
 
  f4->Draw();
  lumi->Draw();
  pc->Update();
  
  pgCmsSyst->Draw("2"); // for drawing x-axis
  if(bDoBRaa)
  {
    pgRaaB_whdg->Draw("f");
  }
  pgRaaNPJpsi_vitev->Draw("f");
  pgRaaNPJpsi_vitev_noDissoc->Draw("f");
  if(bDoBRaa)
  {
    pgRaaB_rapp->Draw("f");
  }
  pgRaaNPJpsi_mcatshq->Draw("l");
  pgRaaNPJpsi_mcatshq_rad->Draw("l");
  pgRaaNPJpsi_bamps->Draw("l");

  pgCmsSyst->Draw("2");
  pgCmsP->Draw("P");
  pgCms->Draw("P");
  pgCmsSyst_lowpt->Draw("2");
  pgCmsP_lowpt->Draw("P");
  pgCms_lowpt->Draw("P");

  // additional info
  CMS_lumi(pc,14005000,33);

  TLegend *leg_cent = new TLegend(0.29,0.81,0.83,0.89,NULL,"brNDC");
  leg_cent->SetMargin(0.17);
  leg_cent->SetBorderSize(0);
  leg_cent->SetTextFont(62);
  leg_cent->SetTextSize(0.03);
  leg_cent->SetLineColor(1);
  leg_cent->SetLineStyle(1);
  leg_cent->SetLineWidth(1);
  leg_cent->SetFillColor(19);
  leg_cent->SetFillStyle(0);

  TLegendEntry *entry_cent;
  entry_cent=leg_cent->AddEntry("raab","Non-prompt J/#psi (CMS)","");
  entry_cent->SetFillStyle(1001);
  entry_cent->SetLineColor(1);
  entry_cent->SetLineStyle(1);
  entry_cent->SetLineWidth(1);
  entry_cent=leg_cent->AddEntry("gNonPrJpsi_pt365y1624", "1.6 < |y| < 2.4","p");
  entry_cent->SetTextFont(42);
  entry_cent=leg_cent->AddEntry("gNonPrJpsi", "|y| < 2.4","p");
  entry_cent->SetTextFont(42);

  TLegend *leg_theory_cent = new TLegend(0.29,0.61,0.83,0.81,NULL,"brNDC");
  leg_theory_cent->SetMargin(0.17);
  leg_theory_cent->SetBorderSize(0);
  leg_theory_cent->SetTextFont(62);
  leg_theory_cent->SetTextSize(0.028);
  leg_theory_cent->SetLineColor(1);
  leg_theory_cent->SetLineStyle(1);
  leg_theory_cent->SetLineWidth(1);
  leg_theory_cent->SetFillColor(10);
  leg_theory_cent->SetFillStyle(1001);
  
  TLegendEntry *entry_theory_cent;
  entry_theory_cent=leg_theory_cent->AddEntry("pgRaaNPJpsi_mcatshq","MC@sHQ+EPOS: standard (|y| < 1)","l");
  entry_theory_cent->SetTextFont(42);
  entry_theory_cent=leg_theory_cent->AddEntry("pgRaaNPJpsi_mcatshq_rad","MC@sHQ+EPOS+rad+LPM: standard (|y| < 1)","l");
  entry_theory_cent->SetTextFont(42);
  entry_theory_cent=leg_theory_cent->AddEntry("pgRaaNPJpsi_bamps","BAMPS: b=5 fm (|y| < 2.4)","l");
  entry_theory_cent->SetTextFont(42);
  entry_theory_cent=leg_theory_cent->AddEntry("pgRaaNPJpsi_vitev","Vitev: Rad E loss+CNM+Dissoc(0-10%, y~0)","f");
  entry_theory_cent->SetTextFont(42);
  entry_theory_cent=leg_theory_cent->AddEntry("pgRaaNPJpsi_vitev_noDissoc","Vitev: Rad E loss+CNM(0-10%, y~0)","f");
  entry_theory_cent->SetTextFont(42);
  if(bDoBRaa)
  {
    entry_theory_cent=leg_theory_cent->AddEntry("pgRaaB_rapp","He,Fries,Rapp: HF transport (0-90%, y~0)","f");
    entry_theory_cent->SetTextFont(42);
    entry_theory_cent=leg_theory_cent->AddEntry("pgRaaB_whdg","WHDG: Rad+Coll E loss (0-80%)","f");
    entry_theory_cent->SetTextFont(42);
  }

  line->Draw();
  leg_cent->Draw();
  leg_theory_cent->Draw();
  gPad->RedrawAxis();
  if(bSavePlots)
  {
    pc->SaveAs(Form("figs/pdf/raaTheoryOpen_pt_withB%d.pdf",bDoBRaa));
    pc->SaveAs(Form("figs/png/raaTheoryOpen_pt_withB%d.png",bDoBRaa));
  }
}


