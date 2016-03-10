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
  const int nBin = 4; // pT(4), y(3)
  double eff[100][100] = {0.0};
  for(int idir = 1; idir < 101; idir++){
    if(iCat_ == 0) in.open(Form("%s../Eff_00%d/ppPromp_eff_Pts_pT_6.5_30.0_y_0.0_2.4_HighQ.tex",dir.Data(),idir));
    if(iCat_ == 1) in.open(Form("%s../Eff_00%d/ppPromp_eff_Raps_pT_6.5_30.0_y_0.0_2.4_HighQ.tex",dir.Data(),idir));
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
    //hDis[in] = new TH1F(Form("hDis_%d",in),"",20,0.35,0.65); // cent
    hDis[in] = new TH1F(Form("hDis_%d",in),"",180,0.25,0.85); // pT
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
  const int nPtBins = 4;
  const int nRapBins = 3; // 6
  double pt_bound[nPtBins+1] = {6.5, 8.0, 10.0, 13.0, 30.0};
  double rap_bound[nRapBins+1] = {0.0, 1.2, 1.6, 2.4};
  char OutTextFile[100];
  if(iCat_==0) sprintf(OutTextFile,"pp_prompt_old_pt.tex");
  if(iCat_==1) sprintf(OutTextFile,"pp_prompt_old_rap.tex");
  ofstream dataFile(OutTextFile);
  TCanvas *c1 = new TCanvas("c1","",1200,300);
  c1->Divide(4,1);
  for(int ic = 1; ic < nBin+1; ic++){
    c1->cd(ic);
    hDis[ic-1]->GetXaxis()->SetTitle("Efficiency");
    hDis[ic-1]->Draw("E");
    //cout<<"Mean : "<<hDis[ic-1]->GetMean()<<endl;
    //cout<<"RMS : "<<hDis[ic-1]->GetRMS()<<endl;
    cout<<hDis[ic-1]->GetMean()<<" "<<hDis[ic-1]->GetRMS()<<endl;
    if(iCat_==0) dataFile<<"pT : "<<pt_bound[ic-1]<<"-"<<pt_bound[ic]<<endl;
    if(iCat_==1) dataFile<<"rap : "<<rap_bound[ic-1]<<"-"<<rap_bound[ic]<<endl;
    dataFile<<hDis[ic-1]->GetMean()<<" "<<hDis[ic-1]->GetRMS()<<endl;
    if(iCat_==0) lt->DrawLatex(0.2,0.95,Form("pp, p_{T} [%0.1f,%0.1f], y [0.0,2.4]",pt_bound[ic-1],pt_bound[ic]));
    if(iCat_==1) lt->DrawLatex(0.2,0.95,Form("pp, p_{T} [6.5,30.0], y [%0.1f,%0.1f]",pt_bound[ic-1],pt_bound[ic]));
  }

  if(iCat_ == 0) {c1->SaveAs("plot_pp_eff_dis_pt.png");c1->SaveAs("plot_pp_eff_dis_pt.pdf");}
  if(iCat_ == 1) {c1->SaveAs("plot_pp_eff_dis_rap.png");c1->SaveAs("plot_pp_eff_dis_rap.pdf");}

  return;
}

