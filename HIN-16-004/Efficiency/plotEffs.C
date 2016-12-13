#include "TFile.h"
#include "TH1F.h"
#include "TGraphAsymmErrors.h"
#include "TString.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TSystem.h"

#include "../Fitter/Macros/CMS/CMS_lumi.C"
#include "../Fitter/Macros/CMS/tdrstyle.C"
#include "../Fitter/Macros/Utilities/texUtils.h"
#include "JpsiDrawing.h"

#include <iostream>
#include <fstream>
#include <string>

using namespace std;


TH1F* integrateHist(TH1F *hist) {
   TString name = hist->GetName(); name = name + "_int";
   TString title = hist->GetTitle(); title = title + " integrated";
   double integral, integralerror;
   int nbins = hist->GetNbinsX();
   integral = hist->IntegralAndError(1,nbins,integralerror);
   TH1F *ans = new TH1F(name, title, 1, hist->GetXaxis()->GetBinLowEdge(1), hist->GetXaxis()->GetBinUpEdge(nbins));
   ans->SetBinContent(1,integral);
   ans->SetBinError(1,integralerror);
   return ans;
}




class drawingEff {
public:

  // Bin boundaries for efficiency histograms
  vector<float> bins_4rap;
  vector<float> bins_3cent;
  int nbins_4rap;
  int nbins_3cent;
  bool ispbpb;

  // Input file
  TFile *finput;

  // Eff vs centrality in 4+1 |y| regions (6.5-50 GeV/c), forward & low pT region
  vector<TH1F*> hnum_cent_rap;
  vector<TH1F*> hden_cent_rap;
  vector<TGraphAsymmErrors*> heff_cent_rap;
  // Eff vs pT in 4+1 |y| regions
  vector<TH1F*> hnum_pt_rap;
  vector<TH1F*> hden_pt_rap;
  vector<TGraphAsymmErrors*> heff_pt_rap;
  // Eff vs pT in 3 centrality regions
  vector<TH1F*> hnum_pt_cent;
  vector<TH1F*> hden_pt_cent;
  vector<TGraphAsymmErrors*> heff_pt_cent;
  // Eff vs rap integrated
  TH1F *hnum_rap;
  TH1F *hden_rap;
  TGraphAsymmErrors *heff_rap;

  drawingEff(string fname, bool ispbpb);
  ~drawingEff();
  void loadHisto();
  void getEfficiency();
  void drawHisto(vector<string> outname);
  void fixCentPp(TH1F*);
  void checkUnderFlow(TH1 *hnum, TH1 *hden);
};

void drawingEff::fixCentPp(TH1F *hist) {
   int nbins = hist->GetNbinsX();
   float y = hist->GetBinContent(1);
   float dy = hist->GetBinError(1);
   for (int i=2; i<nbins+1; i++) {
      hist->SetBinContent(i,y);
      hist->SetBinError(i,dy);
   }
}

drawingEff::~drawingEff(){
  // Eff vs centrality in 4+1 |y| regions (6.5-50 GeV/c), forward & low pT region
  for (size_t i=0; i<hnum_cent_rap.size(); i++) {
    delete hnum_cent_rap[i];
    delete hden_cent_rap[i];
    delete heff_cent_rap[i];
  }
  // Eff vs pT in 4+1 |y| regions
  for (size_t i=0; i<hnum_pt_rap.size(); i++) {
    delete hnum_pt_rap[i];
    delete hden_pt_rap[i];
    delete heff_pt_rap[i];
  }
  // Eff vs pT in 3 centrality regions
  for (size_t i=0; i<hnum_pt_cent.size(); i++) {
    delete hnum_pt_cent[i];
    delete hden_pt_cent[i];
    delete heff_pt_cent[i];
  }
  // Eff vs rap integrated
  delete hnum_rap;
  delete hden_rap;
  delete heff_rap;

  finput->Close();
}

drawingEff::drawingEff(string fname, bool ispbpb_){
  finput = new TFile(fname.c_str());
  ispbpb = ispbpb_;

  float bins_4rap_[] = {0, 0.6, 1.2, 1.8, 2.4};
  float bins_3cent_[] = {0, 10, 30, 100};
  
  nbins_4rap = sizeof(bins_4rap_)/sizeof(float) -1;
  for (int i=0; i<=nbins_4rap; i++)
    bins_4rap.push_back(bins_4rap_[i]);

  nbins_3cent = sizeof(bins_3cent_)/sizeof(float) -1;
  for (int i=0; i<=nbins_3cent; i++) 
    bins_3cent.push_back(bins_3cent_[i]);
}

void drawingEff::checkUnderFlow(TH1 *hnum, TH1 *hden){

  cout << "\ncheckUnderFlow: " << hnum->GetName() << " " << hden->GetName() << endl;
  for (int j=0; j<=hnum->GetNbinsX()+1; j++) {
    double num0 = hnum->GetBinContent(j);
    double den0 = hden->GetBinContent(j);
    
    // prints bincontent for cross-check!
    if (num0>den0) {
      cout << "Bin " << j << ": "
           << hnum->GetBinContent(j) << " " << hden->GetBinContent(j) << " " 
           << hnum->GetBinContent(j) / hden->GetBinContent(j)
           << endl;
    }

    // If underflow bin has more entries in numerator than denominator, set it to 0
    if ((j==0 && num0>den0) || (j==hnum->GetNbinsX()+1 && num0>den0)) {
      hnum->SetBinContent(j,0);
      hden->SetBinContent(j,0);
      hnum->SetBinError(j,0);
      hden->SetBinError(j,0);
    }
    
    if (num0>den0) {
      cout << "Bin " << j << ": "
           << hnum->GetBinContent(j) << " " << hden->GetBinContent(j) << " " 
           << hnum->GetBinContent(j) / hden->GetBinContent(j)
           << endl;
    }

  }

}

void drawingEff::loadHisto(){
  if (!finput) cout << "Cannot open input file!\n" << endl;
  
  // Load denominator, numerators
  // Eff vs centrality in 4+1 |y| regions (6.5-50 GeV/c), forward & low pT region
  TH1F *hnum = (TH1F*)finput->Get("hnum_cent_rap0024");
  TH1F *hden = (TH1F*)finput->Get("hden_cent_rap0024");
  if (!ispbpb) { // set all bin contents to same as the 1st bin
    fixCentPp(hnum);
    fixCentPp(hden);
  }
  checkUnderFlow(hnum,hden);
  hnum_cent_rap.push_back(hnum);
  hden_cent_rap.push_back(hden);
  for (int i=0; i<nbins_4rap; i++) {
    TH1F *hnum1 = (TH1F*)finput->Get( Form("hnum_cent_rap%02.0f%02.0f",bins_4rap[i]*10,bins_4rap[i+1]*10) );
    TH1F *hden1 = (TH1F*)finput->Get( Form("hden_cent_rap%02.0f%02.0f",bins_4rap[i]*10,bins_4rap[i+1]*10) );
    if (!ispbpb) { // set all bin contents to same as the 1st bin
      fixCentPp(hnum1);
      fixCentPp(hden1);
    }
    checkUnderFlow(hnum1,hden1);
    hnum_cent_rap.push_back(hnum1);
    hden_cent_rap.push_back(hden1);
  }
  hnum = (TH1F*)finput->Get("hnum_cent_rap1824_pt3065");
  hden = (TH1F*)finput->Get("hden_cent_rap1824_pt3065");
  if (!ispbpb) { // set all bin contents to same as the 1st bin
    fixCentPp(hnum);
    fixCentPp(hden);
  }
  checkUnderFlow(hnum,hden);
  hnum_cent_rap.push_back(hnum);
  hden_cent_rap.push_back(hden);

  // Eff vs pT in 4+1 |y| regions
  hnum = (TH1F*)finput->Get("hnum_pt_rap0024");
  hden = (TH1F*)finput->Get("hden_pt_rap0024");
  checkUnderFlow(hnum,hden);
  hnum_pt_rap.push_back(hnum);
  hden_pt_rap.push_back(hden);
  for (int i=0; i<nbins_4rap; i++) {
    TH1F *hnum1 = (TH1F*)finput->Get( Form("hnum_pt_rap%02.0f%02.0f",bins_4rap[i]*10,bins_4rap[i+1]*10) );
    TH1F *hden1 = (TH1F*)finput->Get( Form("hden_pt_rap%02.0f%02.0f",bins_4rap[i]*10,bins_4rap[i+1]*10) );
    checkUnderFlow(hnum1,hden1);
    hnum_pt_rap.push_back(hnum1);
    hden_pt_rap.push_back(hden1);
  }

  // Eff vs pT in 3 centrality regions
  for (int i=0; i<(ispbpb?nbins_3cent-1:1); i++) { // for pp, only the 1st bin is necessary
    TH1F *hnum1 = (TH1F*)finput->Get( Form("hnum_pt_cent%02.0f%02.0f",bins_3cent[i],bins_3cent[i+1]) );
    TH1F *hden1 = (TH1F*)finput->Get( Form("hden_pt_cent%02.0f%02.0f",bins_3cent[i],bins_3cent[i+1]) );
    checkUnderFlow(hnum1,hden1);
    hnum_pt_cent.push_back(hnum1);
    hden_pt_cent.push_back(hden1);
  }
  if (ispbpb) {
    hnum = (TH1F*)finput->Get( Form("hnum_pt_cent%02.0f%03.0f",bins_3cent[nbins_3cent-1],bins_3cent[nbins_3cent]) );
    hden = (TH1F*)finput->Get( Form("hden_pt_cent%02.0f%03.0f",bins_3cent[nbins_3cent-1],bins_3cent[nbins_3cent]) );
    checkUnderFlow(hnum,hden);
    hnum_pt_cent.push_back(hnum);
    hden_pt_cent.push_back(hden);
  }

  // Eff vs rap integrated
  hnum_rap = (TH1F*)finput->Get("hnum_rap");
  hden_rap = (TH1F*)finput->Get("hden_rap");
  checkUnderFlow(hnum_rap,hden_rap);
}

void drawingEff::getEfficiency(){

  setTDRStyle();
  gStyle->SetEndErrorSize(3);

  for (int i=0; i<nbins_4rap+2; i++) {
    TGraphAsymmErrors *geff = new TGraphAsymmErrors(hnum_cent_rap[i],hden_cent_rap[i],"n");
    heff_cent_rap.push_back(geff);

    string gname = hnum_cent_rap[i]->GetName();
    gname.replace(gname.begin(),gname.begin()+4,"heff");
    heff_cent_rap[i]->SetName(gname.c_str());
    
    heff_cent_rap[i]->GetXaxis()->SetTitle(hnum_cent_rap[i]->GetXaxis()->GetTitle());
    heff_cent_rap[i]->GetYaxis()->SetTitle(hnum_cent_rap[i]->GetYaxis()->GetTitle());
    heff_cent_rap[i]->GetXaxis()->SetRangeUser(0,100);
    SetHistStyle(heff_cent_rap[i],i,i,0,1.3);
  }
  
  for (int i=0; i<nbins_4rap+1; i++) {
    TGraphAsymmErrors *geff = new TGraphAsymmErrors(hnum_pt_rap[i],hden_pt_rap[i],"n");
    heff_pt_rap.push_back(geff);
   
    string gname = hnum_pt_rap[i]->GetName();
    gname.replace(gname.begin(),gname.begin()+4,"heff");
    heff_pt_rap[i]->SetName(gname.c_str());

    heff_pt_rap[i]->GetXaxis()->SetTitle(hnum_pt_rap[i]->GetXaxis()->GetTitle());
    heff_pt_rap[i]->GetYaxis()->SetTitle(hnum_pt_rap[i]->GetYaxis()->GetTitle());
    heff_pt_rap[i]->GetXaxis()->SetRangeUser(0,50);
    SetHistStyle(heff_pt_rap[i],i,i,0,1.3);
  }
 
  for (int i=0; i<(ispbpb?nbins_3cent:1); i++) {
    TGraphAsymmErrors *geff = new TGraphAsymmErrors(hnum_pt_cent[i],hden_pt_cent[i],"n");
    heff_pt_cent.push_back(geff);
    
    string gname = hnum_pt_cent[i]->GetName();
    gname.replace(gname.begin(),gname.begin()+4,"heff");
    heff_pt_cent[i]->SetName(gname.c_str());

    heff_pt_cent[i]->GetXaxis()->SetTitle(hnum_pt_cent[i]->GetXaxis()->GetTitle());
    heff_pt_cent[i]->GetYaxis()->SetTitle(hnum_pt_cent[i]->GetYaxis()->GetTitle());
    heff_pt_cent[i]->GetXaxis()->SetRangeUser(0,50);
    SetHistStyle(heff_pt_cent[i],i,i,0,1.3);
  }

  heff_rap = new TGraphAsymmErrors(hnum_rap,hden_rap,"n");
  
  string gname = hnum_rap->GetName();
  gname.replace(gname.begin(),gname.begin()+4,"heff");
  heff_rap->SetName(gname.c_str());

  heff_rap->GetXaxis()->SetTitle(hnum_rap->GetXaxis()->GetTitle());
  heff_rap->GetYaxis()->SetTitle(hnum_rap->GetYaxis()->GetTitle());
  heff_rap->GetXaxis()->SetRangeUser(0,2.4);
  SetHistStyle(heff_rap,0,0,0,1.3);
}

void drawingEff::drawHisto(vector<string> outname){

  gStyle->SetEndErrorSize(3);

  TCanvas *can = new TCanvas("can","can",600,600);
  TLatex *lat = new TLatex(); lat->SetNDC(); lat->SetTextSize(0.035);
  TLegend *leg = new TLegend(0.55,0.60,0.94,0.88);
  SetLegendStyle(leg);

  // Eff vs centrality in 4+1 |y| regions (6.5-50 GeV/c), forward & low pT region
  heff_cent_rap[0]->Draw("ap");
  leg->AddEntry(heff_cent_rap[0],"|y|: 0-2.4, 6.5-50 GeV/c","p");
  for (int i=0; i<nbins_4rap; i++) {
    heff_cent_rap[i+1]->Draw("p");
    leg->AddEntry(heff_cent_rap[i+1],Form("|y|: %.1f-%.1f, 6.5-50 GeV/c",bins_4rap[i],bins_4rap[i+1]),"p");
  }
  heff_cent_rap[nbins_4rap+1]->Draw("p");
  leg->AddEntry(heff_cent_rap[nbins_4rap+1],Form("|y|: %.1f-%.1f, 3-6.5 GeV/c",bins_4rap[nbins_4rap-1],bins_4rap[nbins_4rap]),"p");
  leg->Draw();
  lat->DrawLatex(0.2,0.85,outname[0].c_str());
  can->SaveAs(Form("%s_cent_rap.png",outname[1].c_str()));
  can->SaveAs(Form("%s_cent_rap.pdf",outname[1].c_str()));
  can->SaveAs(Form("%s_cent_rap.root",outname[1].c_str()));

  delete can;
  delete leg;
  

  // Eff vs pT in 4+1 |y| regions
  can = new TCanvas("can","can",600,600);
  leg = new TLegend(0.6,0.68,0.9,0.88);
  SetLegendStyle(leg);

  heff_pt_rap[0]->Draw("ap");
  if (ispbpb) leg->AddEntry(heff_pt_rap[0],"|y|: 0-2.4, 0-100%","p");
  else leg->AddEntry(heff_pt_rap[0],"|y|: 0-2.4","p");
  for (int i=0; i<nbins_4rap; i++) {
    heff_pt_rap[i+1]->Draw("p");
    if (ispbpb) leg->AddEntry(heff_pt_rap[i+1],Form("|y|: %.1f-%.1f, 0-100%%",bins_4rap[i],bins_4rap[i+1]),"p");
    else leg->AddEntry(heff_pt_rap[i+1],Form("|y|: %.1f-%.1f",bins_4rap[i],bins_4rap[i+1]),"p");
  }
  leg->Draw();
  lat->DrawLatex(0.2,0.85,outname[0].c_str());
  can->SaveAs(Form("%s_pt_rap.png",outname[1].c_str()));
  can->SaveAs(Form("%s_pt_rap.pdf",outname[1].c_str()));
  can->SaveAs(Form("%s_pt_rap.root",outname[1].c_str()));

  delete can;
  delete leg;
  
 
  // Eff vs pT in 3 centrality regions
  can = new TCanvas("can","can",600,600);
  leg = new TLegend(0.67,0.68,0.9,0.88);
  SetLegendStyle(leg);

  heff_pt_cent[0]->Draw("ap");
  leg->AddEntry(heff_pt_cent[0],Form("%.0f-%.0f%%",bins_3cent[0],bins_3cent[1]),"p");
  if (ispbpb) {
    for (int i=1; i<nbins_3cent; i++) {
      heff_pt_cent[i]->Draw("p");
      leg->AddEntry(heff_pt_cent[i],Form("%.0f-%.0f%%",bins_3cent[i],bins_3cent[i+1]),"p");
    }
    leg->Draw();
  }
  lat->DrawLatex(0.2,0.85,outname[0].c_str());
  lat->DrawLatex(0.2,0.80,"|y| < 2.4");
  can->SaveAs(Form("%s_pt_cent.png",outname[1].c_str()));
  can->SaveAs(Form("%s_pt_cent.pdf",outname[1].c_str()));
  can->SaveAs(Form("%s_pt_cent.root",outname[1].c_str()));

  delete can;
  delete leg;
  

  // Eff vs rap integrated
  can = new TCanvas("can","can",600,600);

  heff_rap->Draw("ap");
  lat->DrawLatex(0.2,0.85,outname[0].c_str());
  lat->DrawLatex(0.2,0.80,"6.5-50 GeV/c");
  if (ispbpb) lat->DrawLatex(0.2,0.75,"0-100%");
  can->SaveAs(Form("%s_rap.png",outname[1].c_str()));
  can->SaveAs(Form("%s_rap.pdf",outname[1].c_str()));
  can->SaveAs(Form("%s_rap.root",outname[1].c_str()));

  delete can;
  
}










////////////////////////////////////
//////////***** MAIN *****//////////
////////////////////////////////////
void plotEffs() {

  string dir = "figs";
  string subdir = "trg__sta";

  gSystem->mkdir(dir.c_str(),kTRUE);
  gSystem->mkdir(Form("%s/%s",dir.c_str(),subdir.c_str()),kTRUE);

  vector<string> latex;
  latex.push_back("prompt J/#psi (pp)");
  latex.push_back(Form("%s/%s/jpsi_pp",dir.c_str(),subdir.c_str()));
  drawingEff fjpsi_pp(Form("files/%s/histos_jpsi_pp.root",subdir.c_str()),false);
  fjpsi_pp.loadHisto();
  fjpsi_pp.getEfficiency();
  fjpsi_pp.drawHisto(latex);

  latex.clear();
  latex.push_back("nonprompt J/#psi (pp)");
  latex.push_back(Form("%s/%s/npjpsi_pp",dir.c_str(),subdir.c_str()));
  drawingEff fnpjpsi_pp(Form("files/%s/histos_npjpsi_pp.root",subdir.c_str()),false);
  fnpjpsi_pp.loadHisto();
  fnpjpsi_pp.getEfficiency();
  fnpjpsi_pp.drawHisto(latex);

  latex.clear();
  latex.push_back("prompt J/#psi (PbPb)");
  latex.push_back(Form("%s/%s/jpsi_pbpb",dir.c_str(),subdir.c_str()));
  drawingEff fjpsi_pbpb(Form("files/%s/histos_jpsi_pbpb.root",subdir.c_str()),true);
  fjpsi_pbpb.loadHisto();
  fjpsi_pbpb.getEfficiency();
  fjpsi_pbpb.drawHisto(latex);

  latex.clear();
  latex.push_back("nonprompt J/#psi (PbPb)");
  latex.push_back(Form("%s/%s/npjpsi_pbpb",dir.c_str(),subdir.c_str()));
  drawingEff fnpjpsi_pbpb(Form("files/%s/histos_npjpsi_pbpb.root",subdir.c_str()),true);
  fnpjpsi_pbpb.loadHisto();
  fnpjpsi_pbpb.getEfficiency();
  fnpjpsi_pbpb.drawHisto(latex);


}


