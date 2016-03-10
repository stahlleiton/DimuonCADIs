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
  const int nBin = 1; // Cent(12), pT(4), y(3)
  double eff[100][100] = {0.0};
  for(int idir = 1; idir < 101; idir++){
    if(iCat_ == 0) in.open(Form("%s../Eff_00%d/eff_HighPt_Pts_default_Bit1_y_0.0_2.4_pT_10.0_30.0_pr.tex",dir.Data(),idir));
    if(iCat_ == 1) in.open(Form("%s../Eff_00%d/eff_HighPt_Pts_default_Bit1_y_0.0_2.4_pT_6.5_30.0_pr.tex",dir.Data(),idir));
    if(iCat_ == 2) in.open(Form("%s../Eff_00%d/eff_HighPt_Pts_default_Bit1_y_1.6_2.4_pT_3.0_30.0_pr.tex",dir.Data(),idir));
    if(iCat_ == 3) in.open(Form("%s../Eff_00%d/eff_HighPt_Pts_default_Bit1_y_1.6_2.4_pT_3.0_6.5_pr.tex",dir.Data(),idir));
    if(iCat_ == 4) in.open(Form("%s../Eff_00%d/eff_HighPt_Pts_default_Bit1_y_1.6_2.4_pT_6.5_30.0_pr.tex",dir.Data(),idir));
    if(iCat_ == 5) in.open(Form("%s../Eff_00%d/eff_HighPt_Pts_default_Bit1_y_1.6_2.4_pT_3.0_5.5_pr.tex",dir.Data(),idir));
    if(iCat_ == 6) in.open(Form("%s../Eff_00%d/eff_HighPt_Pts_default_Bit1_y_0.0_2.4_pT_6.5_8.5_pr.tex",dir.Data(),idir));
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
    if(iCat_ == 0) hDis[in] = new TH1F(Form("hDis_%d",in),"",180,0.25,0.85); // pT
    if(!((iCat_ == 1) || (iCat_ == 4) || (iCat_ == 6) || (iCat_ == 0))) hDis[in] = new TH1F(Form("hDis_%d",in),"",120,0.05,0.45); // cent
    if((iCat_ == 1) || (iCat_ == 4) || (iCat_ == 6) ) hDis[in] = new TH1F(Form("hDis_%d",in),"",120,0.2,0.6); // pT
    //hDis[in] = new TH1F(Form("hDis_%d",in),"",15,0.10,0.40); // pT
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

  char OutTextFile[100];
  if(iCat_ == 0) sprintf(OutTextFile,"pbpb_non-prompt_y_0.0_2.4_pT_10_30.tex");
  if(iCat_ == 1) sprintf(OutTextFile,"pbpb_non-prompt_y_0.0_2.4_pT_6.5_30.tex");
  if(iCat_ == 2) sprintf(OutTextFile,"pbpb_non-prompt_y_1.6_2.4_pT_3.0_30.tex");
  if(iCat_ == 3) sprintf(OutTextFile,"pbpb_non-prompt_y_1.6_2.4_pT_3.0_6.5.tex");
  if(iCat_ == 4) sprintf(OutTextFile,"pbpb_non-prompt_y_1.6_2.4_pT_6.5_30.0.tex");
  if(iCat_ == 5) sprintf(OutTextFile,"pbpb_non-prompt_y_1.6_2.4_pT_3.0_5.5.tex");
  if(iCat_ == 6) sprintf(OutTextFile,"pbpb_non-prompt_y_0.0_2.4_pT_6.5_8.5.tex");
  ofstream dataFile(OutTextFile);

  TLatex *lt = new TLatex();
  lt->SetNDC();

  TCanvas *c1 = new TCanvas("c1","",660,600);
  c1->Divide(1);
  for(int ic = 1; ic < nBin+1; ic++){
    c1->cd(ic);
    hDis[ic-1]->Draw("E");
    //cout<<"Mean : "<<hDis[ic-1]->GetMean()<<endl;
    //cout<<"RMS : "<<hDis[ic-1]->GetRMS()<<endl;
    if(iCat_ == 0) dataFile<<"rap : 0.0-2.4, pT : 10-30"<<endl;
    if(iCat_ == 1) dataFile<<"rap : 0.0-2.4, pT : 6.5-30"<<endl;
    if(iCat_ == 2) dataFile<<"rap : 1.6-2.4, pT : 3-30"<<endl;
    if(iCat_ == 3) dataFile<<"rap : 1.6-2.4, pT : 3-6.5"<<endl;
    if(iCat_ == 4) dataFile<<"rap : 1.6-2.4, pT : 6.5-30"<<endl;
    if(iCat_ == 5) dataFile<<"rap : 1.6-2.4, pT : 3-5.5"<<endl;
    if(iCat_ == 6) dataFile<<"rap : 0.0-2.4, pT : 6.5-8.5"<<endl;
    if(iCat_==0) lt->DrawLatex(0.15,0.95,"PbPb, p_{T} [10,30], y [0.0,2.4]");
    if(iCat_==1) lt->DrawLatex(0.15,0.95,"PbPb, p_{T} [6.5,30], y [0.0,2.4]");
    if(iCat_==2) lt->DrawLatex(0.15,0.95,"PbPb, p_{T} [3,30], y [1.6,2.4]");
    if(iCat_==3) lt->DrawLatex(0.15,0.95,"PbPb, p_{T} [3,6.5], y [1.6,2.4]");
    if(iCat_==4) lt->DrawLatex(0.15,0.95,"PbPb, p_{T} [6.5,30], y [1.6,2.4]");
    if(iCat_==5) lt->DrawLatex(0.15,0.95,"PbPb, p_{T} [3,5.5], y [1.6,2.4]");
    if(iCat_==6) lt->DrawLatex(0.15,0.95,"PbPb, p_{T} [6.5,8.5], y [0.0,2.4]");
    lt->DrawLatex(0.15,0.85,"Cent [0-100]");
    dataFile<<hDis[ic-1]->GetMean()<<" "<<hDis[ic-1]->GetRMS()<<endl;
    cout<<hDis[ic-1]->GetMean()<<" "<<hDis[ic-1]->GetRMS()<<endl;
  }

  if(iCat_ == 0) c1->SaveAs("plot_pbpb_npr_eff_dis_y_0.0_2.4_pT_10_30.png");c1->SaveAs("plot_pbpb_npr_eff_dis_y_0.0_2.4_pT_10_30.pdf");
  if(iCat_ == 1) c1->SaveAs("plot_pbpb_npr_eff_dis_y_0.0_2.4_pT_6.5_30.png");c1->SaveAs("plot_pbpb_npr_eff_dis_y_0.0_2.4_pT_6.5_30.pdf");
  if(iCat_ == 2) c1->SaveAs("plot_pbpb_npr_eff_dis_y_1.6_2.4_pT_3.0_30.png");c1->SaveAs("plot_pbpb_npr_eff_dis_y_1.6_2.4_pT_3.0_30.pdf");
  if(iCat_ == 3) c1->SaveAs("plot_pbpb_npr_eff_dis_y_1.6_2.4_pT_3.0_6.5.png");c1->SaveAs("plot_pbpb_npr_eff_dis_y_1.6_2.4_pT_3.0_6.5.pdf");
  if(iCat_ == 4) c1->SaveAs("plot_pbpb_npr_eff_dis_y_1.6_2.4_pT_6.5_30.0.png");c1->SaveAs("plot_pbpb_npr_eff_dis_y_1.6_2.4_pT_6.5_30.0.pdf");
  if(iCat_ == 5) c1->SaveAs("plot_pbpb_npr_eff_dis_y_1.6_2.4_pT_3.0_5.5.png");c1->SaveAs("plot_pbpb_npr_eff_dis_y_1.6_2.4_pT_3.0_5.5.pdf");
  if(iCat_ == 6) c1->SaveAs("plot_pbpb_npr_eff_dis_y_0.0_2.4_pT_6.5_8.5.png");c1->SaveAs("plot_pbpb_npr_eff_dis_y_0.0_2.4_pT_6.5_8.5.pdf");

  return;
}

