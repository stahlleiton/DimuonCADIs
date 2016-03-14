#include <Riostream.h>
#include <TSystem.h>
#include <TProfile.h>
#include <TBrowser.h>
#include <TROOT.h>
#include <TGraph.h>
#include <TNtuple.h>
#include <TString.h>
#include <TH1D.h>
#include <TFile.h>
#include <TH1D.h>
#include <TF1.h>
#include <TMath.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TStyle.h>
#include <TLatex.h>
#include <TInterpreter.h>
#include <TGraphAsymmErrors.h>
#include <TGraphErrors.h>
#include "Riostream.h"
void mergeEff(int iCat_ = 0) {
  TString dir = gSystem->UnixPathName(gInterpreter->GetCurrentMacroName());
  dir.ReplaceAll("mergeEff.C","");
  dir.ReplaceAll("/./","/");
  ifstream in;
  const int nBin = 6; // 2D (6)
  double eff[100][100] = {0.0};
  for(int idir = 1; idir < 101; idir++){
    if(iCat_==0) in.open(Form("%s../Eff_00%d/eff_Cents_default_Bit1_y_0.0_1.2_pT_6.5_30.0_2D_y1.tex",dir.Data(),idir));
    if(iCat_==1) in.open(Form("%s../Eff_00%d/eff_Cents_default_Bit1_y_1.2_1.6_pT_6.5_30.0_2D_y2.tex",dir.Data(),idir));
    if(iCat_==2) in.open(Form("%s../Eff_00%d/eff_Cents_default_Bit1_y_1.6_2.4_pT_6.5_30.0_2D_y3.tex",dir.Data(),idir));
    if(iCat_==3) in.open(Form("%s../Eff_00%d/eff_Cents_default_Bit1_y_0.0_2.4_pT_6.5_30.0_2D_y4.tex",dir.Data(),idir));
    if(iCat_==4) in.open(Form("%s../Eff_00%d/eff_Cents_default_Bit1_y_1.6_2.4_pT_3.0_6.5_2D_lowpt.tex",dir.Data(),idir));
    Float_t x[300];
    Int_t nlines = 0;
    int nl = 0;
    while (1) {
      in >> x[0] ;
      eff[nl][idir-1] = x[0]; 
      //cout<<"eff : "<<eff[nl][idir-1]<<endl;
      if (!in.good()) break;
      nlines++;
      nl++;
    }
    in.close();
  }

  TH1F *hDis[100]; 

  string eff_tot[100];
  for(int in = 0; in < nBin; in++){
    if(iCat_==0) hDis[in] = new TH1F(Form("hDis_%d",in),"",200,0.30,0.70); // high pt 4
    if(iCat_==1) hDis[in] = new TH1F(Form("hDis_%d",in),"",200,0.30,0.70); // high pt 3
    if(iCat_==2) hDis[in] = new TH1F(Form("hDis_%d",in),"",200,0.35,0.65); // high pt 2
    if(iCat_==3) hDis[in] = new TH1F(Form("hDis_%d",in),"",140,0.45,0.65); // high pt
    hDis[in] = new TH1F(Form("hDis_%d",in),"",200,0.00,0.80); // low pt
    hDis[in]->Sumw2();
    hDis[in]->SetMarkerStyle(20);
    hDis[in]->SetMarkerSize(1.2);
    puts("");
    for(int id = 0; id < 100; id++){
      printf("%0.4f ", eff[in][id]);
      hDis[in]->Fill(eff[in][id]);
    }
  }
  puts("");

  TLatex *lt = new TLatex();
  lt->SetNDC();

  const int nCentBins = 6;
  double ct_bound[nCentBins+1] = {0, 4, 8, 12, 16, 20, 40};

  char OutTextFile[100];
  if(iCat_ == 0) sprintf(OutTextFile,"pbpb_prompt_y_0.0_1.2_pT_6.5_30.tex");
  if(iCat_ == 1) sprintf(OutTextFile,"pbpb_prompt_y_1.2_1.6_pT_6.5_30.tex");
  if(iCat_ == 2) sprintf(OutTextFile,"pbpb_prompt_y_1.6_2.4_pT_6.5_30.tex");
  if(iCat_ == 3) sprintf(OutTextFile,"pbpb_prompt_y_1.6_2.4_pT_3.0_6.5.tex");
  if(iCat_ == 4) sprintf(OutTextFile,"pbpb_prompt_y_0.0_2.4_pT_6.5_30.tex");
  ofstream dataFile(OutTextFile);


  TCanvas *c1 = new TCanvas("c1","",1400,600);
  c1->Divide(5,2);
  for(int ic = 1; ic < nBin+1; ic++){
    c1->cd(ic);
    hDis[ic-1]->Draw("E");
    //cout<<"Mean : "<<hDis[ic-1]->GetMean()<<endl;
    //cout<<"RMS : "<<hDis[ic-1]->GetRMS()<<endl;
    cout<<hDis[ic-1]->GetMean()<<" "<<hDis[ic-1]->GetRMS()<<endl;
    dataFile<<"Cent : "<<ct_bound[ic-1]*2.5<<"-"<<ct_bound[ic]*2.5<<endl;
    dataFile<<hDis[ic-1]->GetMean()<<" "<<hDis[ic-1]->GetRMS()<<endl;
    if(iCat_==0) lt->DrawLatex(0.15,0.95,"pbpb, p_{T} [6.5,30], y [0.0,1.2]");
    if(iCat_==1) lt->DrawLatex(0.15,0.95,"pbpb, p_{T} [6.5,30], y [1.2,1.6]");
    if(iCat_==2) lt->DrawLatex(0.15,0.95,"pbpb, p_{T} [6.5,30], y [1.6,2.4]");
    if(iCat_==3) lt->DrawLatex(0.15,0.95,"pbpb, p_{T} [3,6.5], y [1.6,2.4]");
    if(iCat_==4) lt->DrawLatex(0.15,0.95,"pbpb, p_{T} [6.5,30], y [0.0,2.4]");
    lt->DrawLatex(0.15,0.85,Form("Cent [%0.0f,%0.0f]",ct_bound[ic-1]*2.5,ct_bound[ic]*2.5));
  }

  if(iCat_==0) {c1->SaveAs("plot_pbpb_eff_STA_pr_dis_y1.png");c1->SaveAs("plot_pbpb_eff_STA_pr_dis_y1.pdf");}
  if(iCat_==1) {c1->SaveAs("plot_pbpb_eff_STA_pr_dis_y2.png");c1->SaveAs("plot_pbpb_eff_STA_pr_dis_y2.pdf");}
  if(iCat_==2) {c1->SaveAs("plot_pbpb_eff_STA_pr_dis_y3.png");c1->SaveAs("plot_pbpb_eff_STA_pr_dis_y3.pdf");}
  if(iCat_==3) {c1->SaveAs("plot_pbpb_eff_STA_pr_dis_y4.png");c1->SaveAs("plot_pbpb_eff_STA_pr_dis_y4.pdf");}
  if(iCat_==4) {c1->SaveAs("plot_pbpb_eff_STA_pr_dis_lowpt.png");c1->SaveAs("plot_pbpb_eff_STA_pr_dis_lowpt.pdf");}

  return;
}

