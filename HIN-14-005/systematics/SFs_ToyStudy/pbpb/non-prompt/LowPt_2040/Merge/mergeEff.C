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
void mergeEff() {
  TString dir = gSystem->UnixPathName(gInterpreter->GetCurrentMacroName());
  dir.ReplaceAll("mergeEff.C","");
  dir.ReplaceAll("/./","/");
  ifstream in;
  const int nBin = 3; // Cent(12), pT(4), y(3)
  double eff[100][100] = {0.0};
  for(int idir = 1; idir < 101; idir++){
    in.open(Form("%s../Eff_00%d/eff_HighPt_Cents_default_Bit1_y_1.6_2.4_pT_3.0_6.5_pr.tex",dir.Data(),idir));
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

  string eff_tot[100];
  for(int in = 0; in < nBin; in++){
    hDis[in] = new TH1F(Form("hDis_%d",in),"",150,0.05,0.55); // pT
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

  const int nCentBins = 3;
  double ct_bound[nCentBins+1] = {0, 8, 16, 40};

  char OutTextFile[100];
  sprintf(OutTextFile,"pbpb_non-prompt_lowpt2040.tex");
  ofstream dataFile(OutTextFile);

  TLatex *lt = new TLatex();
  lt->SetNDC();

  TCanvas *c1 = new TCanvas("c1","",900,300);
  c1->Divide(3,1);
  for(int ic = 1; ic < nBin+1; ic++){
    c1->cd(ic);
    hDis[ic-1]->Draw("E");
    //cout<<"Mean : "<<hDis[ic-1]->GetMean()<<endl;
    //cout<<"RMS : "<<hDis[ic-1]->GetRMS()<<endl;
    dataFile<<"Cent : "<<ct_bound[ic-1]*2.5<<"-"<<ct_bound[ic]*2.5<<endl;
    dataFile<<hDis[ic-1]->GetMean()<<" "<<hDis[ic-1]->GetRMS()<<endl;
    lt->DrawLatex(0.15,0.95,"PbPb, p_{T} [3.0,6.5], y [1.6,2.4]");
    lt->DrawLatex(0.15,0.85,Form("Cent [%0.0f,%0.0f]",ct_bound[ic-1]*2.5,ct_bound[ic]*2.5));
    cout<<hDis[ic-1]->GetMean()<<" "<<hDis[ic-1]->GetRMS()<<endl;
  }

  c1->SaveAs("plot_pbpb_npr_eff_dis_cent2040.png");c1->SaveAs("plot_pbpb_npr_eff_dis_cent2040.pdf");

  return;
}

