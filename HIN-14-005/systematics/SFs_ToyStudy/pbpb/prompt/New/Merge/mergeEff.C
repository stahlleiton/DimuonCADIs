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
  const int nBinC = 13; // Cent(13), pT(7), y(6)
  const int nBinP = 7; // Cent(13), pT(7), y(6)
  const int nBinY = 6; // Cent(13), pT(7), y(6)
  double eff[100][100] = {0.0};
  for(int idir = 1; idir < 101; idir++){
    if(iCat_ == 0) in.open(Form("%s../Eff_00%d/eff_HighPt_Cents_default_Bit1_y_0.0_2.4_pT_6.5_30.0_pr.tex",dir.Data(),idir));
    if(iCat_ == 1) in.open(Form("%s../Eff_00%d/eff_HighPt_Pts_default_Bit1_y_0.0_2.4_pT_6.5_30.0_pr.tex",dir.Data(),idir));
    if(iCat_ == 2) in.open(Form("%s../Eff_00%d/eff_HighPt_Raps_default_Bit1_y_0.0_2.4_pT_6.5_30.0_pr.tex",dir.Data(),idir));
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

  int nBins = 0;
  if(iCat_ == 0) nBins = nBinC;
  if(iCat_ == 1) nBins = nBinP;
  if(iCat_ == 2) nBins = nBinY;

  cout<<"nBins : "<<nBins<<endl;

  TH1F *hDis[13]; 

  string eff_tot[100];
  for(int in = 0; in < nBins; in++){
    if(iCat_ == 2) hDis[in] = new TH1F(Form("hDis_%d",in),"",90,0.35,0.65); // cent
    if(iCat_ != 2) hDis[in] = new TH1F(Form("hDis_%d",in),"",180,0.25,0.85); // pT
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

  const int nCentBins = 13;
  const int nRapBins = 6;
  const int nPtBins = 7;

  double ct_bound[nCentBins+1] = {0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 40};
  double pt_bound[nPtBins+1] = {6.5, 7.5, 8.5, 9.5, 11.0, 13.0, 16.0, 30.0};
  double rap_bound[nRapBins+1] = {0.0, 0.4, 0.8, 1.2, 1.6, 2.0, 2.4};


  char OutTextFile[100];
  if(iCat_ == 0) sprintf(OutTextFile,"pbpb_prompt_cent.tex");
  if(iCat_ == 1) sprintf(OutTextFile,"pbpb_prompt_pt.tex");
  if(iCat_ == 2) sprintf(OutTextFile,"pbpb_prompt_rap.tex");
  ofstream dataFile(OutTextFile);

  TLatex *lt = new TLatex();
  lt->SetNDC();

  TCanvas *c1 = new TCanvas("c1","",1400,900);
  c1->Divide(5,3);
  TCanvas *c2 = new TCanvas("c2","",1400,600);
  c2->Divide(5,2);
  for(int ic = 1; ic < nBins+1; ic++){
    if(iCat_==0) c1->cd(ic);
    if(iCat_!=0) c2->cd(ic);
    hDis[ic-1]->Draw("E");
    cout<<"Mean : "<<hDis[ic-1]->GetMean()<<endl;
    cout<<"RMS : "<<hDis[ic-1]->GetRMS()<<endl;
    if(iCat_ == 0) dataFile<<"Cent : "<<ct_bound[ic-1]*2.5<<"-"<<ct_bound[ic]*2.5<<endl;
    if(iCat_ == 1) dataFile<<"pT : "<<pt_bound[ic-1]<<"-"<<pt_bound[ic]<<endl;
    if(iCat_ == 2) dataFile<<"rap : "<<rap_bound[ic-1]<<"-"<<rap_bound[ic]<<endl;
    dataFile<<hDis[ic-1]->GetMean()<<" "<<hDis[ic-1]->GetRMS()<<endl;
    cout<<hDis[ic-1]->GetMean()<<" "<<hDis[ic-1]->GetRMS()<<endl;
    if(iCat_==0) lt->DrawLatex(0.15,0.95,"PbPb, p_{T} [6.5,30], y [0.0,2.4]");
    if(iCat_==0) lt->DrawLatex(0.15,0.85,Form("Cent [%0.0f,%0.0f]",ct_bound[ic-1]*2.5,ct_bound[ic]*2.5));
    if(iCat_==1) lt->DrawLatex(0.15,0.95,Form("PbPb, p_{T} [%0.1f,%0.1f], y [0.0,2.4]",pt_bound[ic-1],pt_bound[ic]));
    if(iCat_==1) lt->DrawLatex(0.15,0.85,"Cent 0-100");
    if(iCat_==2) lt->DrawLatex(0.15,0.95,Form("PbPb, p_{T} [6.5,30], y [%0.1f,%0.1f]",rap_bound[ic-1],rap_bound[ic]));
    if(iCat_==2) lt->DrawLatex(0.15,0.85,"Cent 0-100");
  }

  if(iCat_ == 0) c1->SaveAs("plot_pbpb_eff_dis_cent.png");c1->SaveAs("plot_pbpb_eff_dis_cent.pdf");
  if(iCat_ == 1) c2->SaveAs("plot_pbpb_eff_dis_pt.png");c2->SaveAs("plot_pbpb_eff_dis_pt.pdf");
  if(iCat_ == 2) c2->SaveAs("plot_pbpb_eff_dis_rap.png");c2->SaveAs("plot_pbpb_eff_dis_rap.pdf");

  return;
}

