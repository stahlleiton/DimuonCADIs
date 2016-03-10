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
  const int nBinC = 2; // Cent(2)
  double eff[100][100] = {0.0};
  for(int idir = 1; idir < 101; idir++){
    if(iCat_ == 0) in.open(Form("%s../Eff_00%d/eff_HighPt_Cents_default_Bit1_y_0.0_2.4_pT_6.5_30.0_pr.tex",dir.Data(),idir));
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

  TH1F *hDis[13]; 

  int nBins = 0;
  if(iCat_ == 0) nBins = nBinC;
  string eff_tot[100];
  for(int in = 0; in < nBins; in++){
    if(iCat_ == 2) hDis[in] = new TH1F(Form("hDis_%d",in),"",120,0.35,0.65); // cent
    if(iCat_ != 2) hDis[in] = new TH1F(Form("hDis_%d",in),"",120,0.25,0.85); // pT
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

  const int nCentBins = 2;
  double ct_bound[nCentBins+1] = {24, 28, 40};

  char OutTextFile[100];
  if(iCat_ == 0) sprintf(OutTextFile,"pbpb_prompt_cent70100.tex");
  ofstream dataFile(OutTextFile);

  TLatex *lt = new TLatex();
  lt->SetNDC();

  TCanvas *c1 = new TCanvas("c1","",800,400);
  c1->Divide(2,1);
  for(int ic = 1; ic < nBins+1; ic++){
    c1->cd(ic);
    hDis[ic-1]->Draw("E");
    cout<<"Mean : "<<hDis[ic-1]->GetMean()<<endl;
    cout<<"RMS : "<<hDis[ic-1]->GetRMS()<<endl;
    if(iCat_ == 0) dataFile<<"Cent : "<<ct_bound[ic-1]*2.5<<"-"<<ct_bound[ic]*2.5<<endl;
    dataFile<<hDis[ic-1]->GetMean()<<" "<<hDis[ic-1]->GetRMS()<<endl;
    cout<<hDis[ic-1]->GetMean()<<" "<<hDis[ic-1]->GetRMS()<<endl;
    if(iCat_==0) lt->DrawLatex(0.15,0.95,"PbPb, p_{T} [6.5,30], y [0.0,2.4]");
    if(ic==1) lt->DrawLatex(0.15,0.85,"Cent [60,70]");
    if(ic==2) lt->DrawLatex(0.15,0.85,"Cent [70,100]");
  }

  if(iCat_ == 0) c1->SaveAs("plot_pbpb_eff_STA_npr_dis_cent70100.png");c1->SaveAs("plot_pbpb_eff_STA_npr_dis_cent70100.pdf");

  return;
}

