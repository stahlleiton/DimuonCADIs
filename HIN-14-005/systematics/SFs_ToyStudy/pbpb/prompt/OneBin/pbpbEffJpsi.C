//===================== Macro to calculate PrJpsi Eff or Acc x Eff ============================================
//read tree from DiMuonOnia2DPlots.cc
#ifndef __CINT__
#endif
#include "TLatex.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "TAxis.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TTree.h"
#include "TFile.h"
#include "TH1D.h"
#include "TF1.h"
#include "TH1I.h"
#include "TCanvas.h"
#include "TLine.h"
#include "TMath.h"
#include "TVector3.h"
#include "TString.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TGraphErrors.h"
#include <fstream>
#include <map>
#include <string>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include "TObjArray.h"
// Prompt =1 prompt PrJpsi 
bool IsAccept(Double_t pt, Double_t eta); //you can define acceptance here 
double getEffEta(Double_t pt, Double_t eta);
double FindCenWeight(int Bin);//gives you weight according to cent
TF1 *fit = new TF1("fitf","[0]*TMath::Exp((x-[1])/[2])+[3]",1.5,19);

TF1 *fun1 = new TF1("fun1","_fnc1_",0.0,30.0);
TF1 *fun2 = new TF1("fun2","_fnc2_",0.0,30.0);
TF1 *fun3 = new TF1("fun3","_fnc3_",0.0,30.0);
TF1 *fun4 = new TF1("fun4","_fnc4_",0.0,30.0);

void pbpbEffJpsi__idx_(double minPt = 6.5, double maxPt = 30.0)   
{
  int Prompt =1; int PutWeight = 1;

  int JpsiCat_ = 1; // 1 : prompt, 2 : non-prompt

  for(int iCat_ = 6; iCat_ < 7; iCat_++){

    // One bin
    // 1.6-2.4 : 3.0-6.5, 3.0-30.0
    // 0.0-2.4 : 10.0-30.0, 6.5-30.0
    double minRap = 1.6; double maxRap = 2.4; minPt = 3.0; maxPt = 30.0;
    //double minRap = 1.6; double maxRap = 2.4; minPt = 3.0; maxPt = 6.5;
    //double minRap = 1.6; double maxRap = 2.4; minPt = 6.5; maxPt = 30.0;
    //double minRap = 0.0; double maxRap = 2.4; minPt = 10.0; maxPt = 30.0;
    //double minRap = 0.0; double maxRap = 2.4; minPt = 6.5; maxPt = 30.0;
    if(iCat_ == 0) {minRap = 1.6; maxRap = 2.4; minPt = 3.0; maxPt = 30.0;} 
    if(iCat_ == 1) {minRap = 1.6; maxRap = 2.4; minPt = 3.0; maxPt = 6.5;} 
    if(iCat_ == 2) {minRap = 1.6; maxRap = 2.4; minPt = 6.5; maxPt = 30.0;} 
    if(iCat_ == 3) {minRap = 0.0; maxRap = 2.4; minPt = 10.0; maxPt = 30.0;} 
    if(iCat_ == 4) {minRap = 0.0; maxRap = 2.4; minPt = 6.5; maxPt = 30.0;} 
    if(iCat_ == 5) {minRap = 1.6; maxRap = 2.4; minPt = 3.0; maxPt = 5.5;} 
    if(iCat_ == 6) {minRap = 0.0; maxRap = 2.4; minPt = 6.5; maxPt = 8.5;} 

    cout<<"Min y = "<<minRap<<", Max y = "<<maxRap<<", Min pT = "<<minPt<<", Max pT = "<<maxPt<<endl;

    bool bDefault = true; // true : default, false : sailor or cowboy

    char cCd[512];
    if(bDefault) sprintf(cCd, "default");

    gROOT->SetStyle("Plain");
    gStyle->SetPalette(1);
    gStyle->SetFrameBorderMode(0);
    gStyle->SetFrameFillColor(0);
    gStyle->SetCanvasColor(0);
    gStyle->SetTitleFillColor(0);
    gStyle->SetStatColor(0);
    gStyle->SetPadBorderSize(0);
    gStyle->SetCanvasBorderSize(0);
    gStyle->SetOptTitle(0); // at least most of the time
    gStyle->SetOptStat(0); // most of the time, sometimes "nemriou" might be useful to display name, 
    //number of entries, mean, rms, integral, overflow and underflow
    gStyle->SetOptFit(0); // set to 1 only if you want to display fit results
    //==================================== Define Histograms====================================================

    //==============================================Define Acc Eff Stuff here===========================================
    // Pt bin sizes
    // 0-1.5, 1.5-3, 3-4.5, 4.5-6, 6-7.5...

    const char *cTrg[5] = {"Bit1", "L1DM0HighQ", "L2Mu3NHitQ", "L3Mu3", "L3DMOpen"};
    //int iTrg = 0;
    TFile *outfile;
    char tmp_output[512];
    if(JpsiCat_ == 1) sprintf(tmp_output,"pbpbPrJpsi_y_%0.1f_%0.1f_pT_%0.1f_%0.1f_HighQ_tnpWgt_pr2.root", minRap, maxRap, minPt, maxPt);
    if(JpsiCat_ == 2) sprintf(tmp_output,"pbpbPrJpsi_y_%0.1f_%0.1f_pT_%0.1f_%0.1f_HighQ_tnpWgt_npr2.root", minRap, maxRap, minPt, maxPt);
    outfile =new TFile(tmp_output, "Recreate");
    TH1F *hGenCent = new TH1F("hGenCent",";Centrality (%);Weighted Counts",40,0,40);
    TH1F *hRecCent = new TH1F("hRecCent",";Centrality (%);Weighted Counts",40,0,40);
    hGenCent->Sumw2();
    hRecCent->Sumw2();

    for(int iSpec = 1; iSpec < 2; iSpec++){

      ////const int nCentBins = 11; // Non-prompt 
      ////const int nCentBins = 14; // Prompt
      const int nCentBins = 10; // Non-prompt (60-100)
      //const int nCentBins = 13; // Prompt (60-100)
      const int nPtBins = 1;
      const int nRapBins = 6;
      const int ndPhiBins = 8;
      //const int ndPhiBins = 4;
      const int ndPhi2Bins = 8;
      const int nPhiBins = 8;
      const int nFiles = 6;
      //if(JpsiCat_ == 2) nCentBins = 11;
      // more bins 0-10,10-20,20-30,30-40,40-50,50-70,70-100
      // cent bins : 0-5, 5-10, 10-15, 15-20, 20-25, 25-30, 30-35, 35-40, 40-45, 45-50, 50-55, 55-60, 60-70, 70-100
      ////double ct_bound[nCentBins+1] = {0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 28, 40};
      //double ct_bound[nCentBins+1] = {0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 40}; // Prompt (60-100)
      double ct_bound[nCentBins+1] = {0, 2, 4, 6, 8, 10, 12, 16, 20, 24, 40}; // NonPrompt (60-100)
      double ct_bound2[nCentBins+1] = {0.0};
      for(int ict = 0; ict < nCentBins+1; ict++){
        ct_bound2[ict] = ct_bound[ict]*2.5;
      }
      double xct_bound[nCentBins] = {0.0};
      //double pt_bound[nPtBins+1] = {0.0, 3.0, 6.5};
      // pT bins : 6.5-7.5, 7.5-8.5, 8.5-9.5, 9.5-10.5, 10.5-11.5, 11.5-12.5, 12.5-14.5, 14.5-16.5, 16.5-20.0, 20.0-30.0
      double pt_bound[nPtBins+1];
      pt_bound[0] = minPt;
      pt_bound[1] = maxPt;
      double xpt_bound[nPtBins] = {0.0};
      // rap bins : 0.0-0.3, 0.3-0.6, 0.6-0.9, 0.9-1.2, 1.2-1.5, 1.5-1.8, 1.8-2.1, 2.1-2.4
      double rap_bound[nRapBins+1] = {0.0, 0.4, 0.8, 1.2, 1.6, 2.0, 2.4};
      double xrap_bound[nRapBins] = {0.0};
      double dphi_bound[ndPhiBins+1] = {0.0, TMath::Pi()/16, 2*TMath::Pi()/16, 3*TMath::Pi()/16, 4*TMath::Pi()/16, 5*TMath::Pi()/16, 6*TMath::Pi()/16, 7*TMath::Pi()/16, 8*TMath::Pi()/16};
      double xdphi_bound[ndPhiBins] = {0.0};
      double phi_bound[nPhiBins+1] = {-4*TMath::Pi()/4, -3*TMath::Pi()/4, -2*TMath::Pi()/4, -1*TMath::Pi()/4,0.0, TMath::Pi()/4, 2*TMath::Pi()/4, 3*TMath::Pi()/4, TMath::Pi()};
      double xphi_bound[nPhiBins] = {0.0};
      double dphi2_bound[ndPhi2Bins+1] = {0.0, TMath::Pi()/16, 2*TMath::Pi()/16, 3*TMath::Pi()/16, 4*TMath::Pi()/16, 5*TMath::Pi()/16, 6*TMath::Pi()/16, 7*TMath::Pi()/16, 8*TMath::Pi()/16};
      double xdphi2_bound[ndPhi2Bins] = {0.0};

      const char *cSp[6] = {"Cents","Pts","Raps","Phi","dPhi","gdPhi"};
      char OutTextFile[100];
      if(JpsiCat_ == 1) sprintf(OutTextFile,"eff_HighPt_%s_%s_%s_y_%0.1f_%0.1f_pT_%0.1f_%0.1f_pr.tex", cSp[iSpec], cCd, cTrg[0], minRap, maxRap, minPt, maxPt);
      if(JpsiCat_ == 2) sprintf(OutTextFile,"eff_HighPt_%s_%s_%s_y_%0.1f_%0.1f_pT_%0.1f_%0.1f_npr.tex", cSp[iSpec], cCd, cTrg[0], minRap, maxRap, minPt, maxPt);
      //sprintf(OutTextFile,"eff_HighPt_%s_%s_%s.tex", cSp[iSpec], cCd, cTrg[iCat]);
      ofstream dataFile(Form(OutTextFile));

      char tmp_start[512];
      sprintf(tmp_start,"%%%% Getting Efficiency starts, Category : %s !!!!! %%%%%", cCd);  
      cout<< tmp_start << endl;
      //dataFile<< tmp_start << endl;

      // x, y, z - axis 
      //dataFile<<""<<endl;
      //dataFile<<"xaxis of Cent"<<endl;
      for(int i = 0; i < nCentBins; i++){
        if(i == (nCentBins-1)){
          xct_bound[i] = ct_bound[i-4] + (ct_bound[i-1]-ct_bound[i-4])/2;
          //cout<<"xct_bound["<<i<<"] : "<<xct_bound[i]<<endl;
          //dataFile<<"xct_bound["<<i<<"] : "<<xct_bound[i]<<endl;
        }else{
          xct_bound[i] = ct_bound[i] + (ct_bound[i+1]-ct_bound[i])/2;
          //cout<<"xct_bound["<<i<<"] : "<<xct_bound[i]<<endl;
          //dataFile<<"xct_bound["<<i<<"] : "<<xct_bound[i]<<endl;
        }
      }
      //dataFile<<""<<endl;
      //dataFile<<"xaxis of pT"<<endl;
      for(int i = 0; i < nPtBins; i++){
        xpt_bound[i] = pt_bound[i] + (pt_bound[i+1]-pt_bound[i])/2;
        //cout<<"xpt_bound["<<i<<"] : "<<xpt_bound[i]<<endl;
        //dataFile<<"xpt_bound["<<i<<"] : "<<xpt_bound[i]<<endl;
      }
      //dataFile<<""<<endl;
      //dataFile<<"xaxis of rap"<<endl;
      for(int i = 0; i < nRapBins; i++){
        xrap_bound[i] = rap_bound[i] + (rap_bound[i+1]-rap_bound[i])/2;
        //cout<<"xrap_bound["<<i<<"] : "<<xrap_bound[i]<<endl;
        //dataFile<<"xrap_bound["<<i<<"] : "<<xrap_bound[i]<<endl;
      }
      //dataFile<<""<<endl;
      //dataFile<<"xaxis of dphi"<<endl;
      for(int i = 0; i < ndPhiBins; i++){
        xdphi_bound[i] = dphi_bound[i] + (dphi_bound[i+1]-dphi_bound[i])/2;
        //cout<<"xdphi_bound["<<i<<"] : "<<xdphi_bound[i]<<endl;
        //dataFile<<"xdphi_bound["<<i<<"] : "<<xdphi_bound[i]<<endl;
      }
      //dataFile<<""<<endl;
      //dataFile<<"xaxis of phi"<<endl;
      for(int i = 0; i < nPhiBins; i++){
        xphi_bound[i] = phi_bound[i] + (phi_bound[i+1]-phi_bound[i])/2;
        //cout<<"xphi_bound["<<i<<"] : "<<xphi_bound[i]<<endl;
        //dataFile<<"xphi_bound["<<i<<"] : "<<xphi_bound[i]<<endl;
      }
      //dataFile<<""<<endl;
      //dataFile<<"xaxis of dphi2"<<endl;
      for(int i = 0; i < ndPhi2Bins; i++){
        xdphi2_bound[i] = dphi2_bound[i] + (dphi2_bound[i+1]-dphi2_bound[i])/2;
        //cout<<"xdphi2_bound["<<i<<"] : "<<xdphi2_bound[i]<<endl;
        //dataFile<<"xdphi2_bound["<<i<<"] : "<<xdphi2_bound[i]<<endl;
      }

      int nBins_tmp = 0;
      if(iSpec == 0) { nBins_tmp = nCentBins; }
      if(iSpec == 1) { nBins_tmp = nPtBins; }
      if(iSpec == 2) { nBins_tmp = nRapBins; }
      if(iSpec == 3) { nBins_tmp = nPhiBins; }
      if(iSpec == 4) { nBins_tmp = ndPhiBins; }
      if(iSpec == 5) { nBins_tmp = ndPhi2Bins; }
      const int nBins = nBins_tmp;

      TH1F *hTempMass = new TH1F("hTempMass","",100, 2.0, 4.0);
      TH1F *hGenDiMuonf[nFiles][nBins];
      TH1F *hRecoDiMuonf[nFiles][nBins];
      TH1F *hGenDiMuon[nBins];
      TH1F *hRecoDiMuon[nBins];
      double genNo[nBins];
      double genErr[nBins];
      double recoNo[nBins];
      double recoErr[nBins];
      double eff[nBins];
      double effErr[nBins];
      for(int fl = 0; fl < nFiles; fl++){
        for(int i = 0; i < nBins; i++){
          hGenDiMuonf[fl][i] = (TH1F*)hTempMass->Clone();
          hRecoDiMuonf[fl][i] = (TH1F*)hTempMass->Clone();
          hGenDiMuon[i] = (TH1F*)hTempMass->Clone();
          hRecoDiMuon[i] = (TH1F*)hTempMass->Clone();
          hGenDiMuonf[fl][i]->Sumw2();
          hRecoDiMuonf[fl][i]->Sumw2();
          hGenDiMuon[i]->Sumw2();
          hRecoDiMuon[i]->Sumw2();
        }
      }

      char fileName[10][512];
      //scales for different pT bins
      double scale[6]={1.758e-07, 1.46074e-07, 2.58269e-08, 5.4948e-09, 1.63265e-09, 7.70795e-10};
      if(PutWeight==0){scale[0]=(1);scale[1]=(1);scale[2]=(1);scale[3]=(1);scale[4]=(1);scale[5]=(1);}

      // loop for pT
      cout<<"================================="<<endl;
      sprintf(fileName[0],"/Users/dmoon/Dropbox/Analysis/HiMC/EffStudy/gRpAngRootFiles_Regit_Pro2/HiDiMuonAna_Prompt_RegIt_MC_20131006_Pt_0003.root");
      sprintf(fileName[1],"/Users/dmoon/Dropbox/Analysis/HiMC/EffStudy/gRpAngRootFiles_Regit_Pro2/HiDiMuonAna_Prompt_RegIt_MC_20131006_Pt_0306.root");
      sprintf(fileName[2],"/Users/dmoon/Dropbox/Analysis/HiMC/EffStudy/gRpAngRootFiles_Regit_Pro2/HiDiMuonAna_Prompt_RegIt_MC_20131006_Pt_0609.root");
      sprintf(fileName[3],"/Users/dmoon/Dropbox/Analysis/HiMC/EffStudy/gRpAngRootFiles_Regit_Pro2/HiDiMuonAna_Prompt_RegIt_MC_20131006_Pt_0912.root");
      sprintf(fileName[4],"/Users/dmoon/Dropbox/Analysis/HiMC/EffStudy/gRpAngRootFiles_Regit_Pro2/HiDiMuonAna_Prompt_RegIt_MC_20131006_Pt_1215.root");
      sprintf(fileName[5],"/Users/dmoon/Dropbox/Analysis/HiMC/EffStudy/gRpAngRootFiles_Regit_Pro2/HiDiMuonAna_Prompt_RegIt_MC_20131006_Pt_1530.root");

      TFile *infile;
      TTree *tree;
      TTree *gentree;

      for(int ifile = 0; ifile < nFiles; ifile++){
        infile=new TFile(fileName[ifile],"R");
        tree=(TTree*)infile->Get("SingleMuonTree");
        gentree=(TTree*)infile->Get("SingleGenMuonTree");
        //Event variables
        int eventNb,runNb,lumiBlock, gbin, rbin;
        int hbit1;
        double zVtx;
        //Jpsi Variables
        Double_t JpsiMass,JpsiPt,JpsiRap, JpsiCharge;
        Double_t JpsiVprob;
        Double_t JpsiPhi;
        Double_t JpsiEta;
        Double_t JpsiPsi[38];
        Double_t JpsiGenPsi;
        //2.) muon variables RECO                                                                       
        double muPosPx, muPosPy, muPosPz,  muPosEta, muPosPt, muPosP, muPosPhi;
        double muNegPx, muNegPy, muNegPz,  muNegEta, muNegPt, muNegP, muNegPhi;
        //(1).Positive Muon                                     
        double muPos_nchi2In, muPos_dxy, muPos_dz, muPos_nchi2Gl;
        int muPos_found, muPos_pixeLayers, muPos_nValidMuHits,muPos_arbitrated,muPos_TMOneST,muPos_trkLayMeas;
        bool muPos_matches,muPos_tracker;
        int muPos_Trigger2, muPos_Trigger10;
        int muPos_Trigger4, muPos_Trigger12, muPos_Trigger13;
        int muPos_Trigger21, muPos_Trigger22;
        //(2).Negative Muon                                     
        double muNeg_nchi2In, muNeg_dxy, muNeg_dz, muNeg_nchi2Gl;
        int muNeg_found, muNeg_pixeLayers, muNeg_nValidMuHits,muNeg_arbitrated,muNeg_TMOneST,muNeg_trkLayMeas;
        bool muNeg_matches,muNeg_tracker;
        int muNeg_Trigger2, muNeg_Trigger10;
        int muNeg_Trigger4, muNeg_Trigger12, muNeg_Trigger13;
        int muNeg_Trigger21, muNeg_Trigger22;

        //Gen Level variables
        //Gen PrJpsi Variables
        double GenJpsiMass, GenJpsiPt, GenJpsiRap;
        double GenJpsiPx, GenJpsiPy, GenJpsiPz;
        double GenJpsiPhi;
        double GenJpsiEta;
        double GenJpsiPsi;
        //2.) Gen muon variables 
        double GenmuPosPx, GenmuPosPy, GenmuPosPz,  GenmuPosEta, GenmuPosPt, GenmuPosPhi;
        double GenmuNegPx, GenmuNegPy, GenmuNegPz,  GenmuNegEta, GenmuNegPt, GenmuNegPhi;

        //Event variables
        tree->SetBranchAddress("eventNb",&eventNb);
        tree->SetBranchAddress("runNb",&runNb);
        tree->SetBranchAddress("lumiBlock",&lumiBlock);
        tree->SetBranchAddress("hbit1",&hbit1);
        tree->SetBranchAddress("zVtx",&zVtx);

        //Jpsi Variables
        tree->SetBranchAddress("JpsiCharge",&JpsiCharge);
        tree->SetBranchAddress("JpsiMass",&JpsiMass);
        tree->SetBranchAddress("JpsiPt",&JpsiPt);
        tree->SetBranchAddress("JpsiPhi",&JpsiPhi);
        tree->SetBranchAddress("JpsiEta",&JpsiEta);
        tree->SetBranchAddress("JpsiPsi",&JpsiPsi);
        tree->SetBranchAddress("JpsiGenPsi",&JpsiGenPsi);
        tree->SetBranchAddress("JpsiRap",&JpsiRap);
        tree->SetBranchAddress("JpsiVprob",&JpsiVprob);
        tree->SetBranchAddress("rbin",&rbin);

        //muon variable
        tree->SetBranchAddress("muPosPx",&muPosPx);
        tree->SetBranchAddress("muPosPy",&muPosPy);
        tree->SetBranchAddress("muPosPz",&muPosPz);
        tree->SetBranchAddress("muPosEta",&muPosEta);
        tree->SetBranchAddress("muPosPhi",&muPosPhi);
        tree->SetBranchAddress("muNegPx", &muNegPx);
        tree->SetBranchAddress("muNegPy", &muNegPy);
        tree->SetBranchAddress("muNegPz", &muNegPz);
        tree->SetBranchAddress("muNegEta", &muNegEta);
        tree->SetBranchAddress("muNegPhi", &muNegPhi);


        //1). Positive Muon
        tree->SetBranchAddress("muPos_nchi2In", &muPos_nchi2In);
        tree->SetBranchAddress("muPos_dxy", &muPos_dxy);
        tree->SetBranchAddress("muPos_dz", &muPos_dz);
        tree->SetBranchAddress("muPos_nchi2Gl", &muPos_nchi2Gl);
        tree->SetBranchAddress("muPos_found", &muPos_found);
        tree->SetBranchAddress("muPos_pixeLayers", &muPos_pixeLayers);
        tree->SetBranchAddress("muPos_nValidMuHits", &muPos_nValidMuHits);
        tree->SetBranchAddress("muPos_matches", &muPos_matches);
        tree->SetBranchAddress("muPos_tracker", &muPos_tracker);
        tree->SetBranchAddress("muPos_arbitrated", &muPos_arbitrated);
        tree->SetBranchAddress("muPos_TMOneST", &muPos_TMOneST);
        tree->SetBranchAddress("muPos_trkLayMeas", &muPos_trkLayMeas);
        tree->SetBranchAddress("muPos_Trigger2", &muPos_Trigger2);
        tree->SetBranchAddress("muPos_Trigger21", &muPos_Trigger21);
        tree->SetBranchAddress("muPos_Trigger22", &muPos_Trigger22);
        tree->SetBranchAddress("muPos_Trigger4", &muPos_Trigger4);
        tree->SetBranchAddress("muPos_Trigger10", &muPos_Trigger10);
        tree->SetBranchAddress("muPos_Trigger12", &muPos_Trigger12);
        tree->SetBranchAddress("muPos_Trigger13", &muPos_Trigger13);

        //2). Negative Muon                                                                            
        tree->SetBranchAddress("muNeg_nchi2In", &muNeg_nchi2In);
        tree->SetBranchAddress("muNeg_dxy", &muNeg_dxy);
        tree->SetBranchAddress("muNeg_dz", &muNeg_dz);
        tree->SetBranchAddress("muNeg_nchi2Gl", &muNeg_nchi2Gl);
        tree->SetBranchAddress("muNeg_found", &muNeg_found);
        tree->SetBranchAddress("muNeg_pixeLayers", &muNeg_pixeLayers);
        tree->SetBranchAddress("muNeg_nValidMuHits", &muNeg_nValidMuHits);
        tree->SetBranchAddress("muNeg_matches", &muNeg_matches);
        tree->SetBranchAddress("muNeg_tracker", &muNeg_tracker);
        tree->SetBranchAddress("muNeg_arbitrated", &muNeg_arbitrated);
        tree->SetBranchAddress("muNeg_TMOneST", &muNeg_TMOneST);
        tree->SetBranchAddress("muNeg_trkLayMeas", &muNeg_trkLayMeas);
        tree->SetBranchAddress("muNeg_Trigger2", &muNeg_Trigger2);
        tree->SetBranchAddress("muNeg_Trigger21", &muNeg_Trigger21);
        tree->SetBranchAddress("muNeg_Trigger22", &muNeg_Trigger22);
        tree->SetBranchAddress("muNeg_Trigger4", &muNeg_Trigger4);
        tree->SetBranchAddress("muNeg_Trigger10", &muNeg_Trigger10);
        tree->SetBranchAddress("muNeg_Trigger12", &muNeg_Trigger12);
        tree->SetBranchAddress("muNeg_Trigger13", &muNeg_Trigger13);
        //====================================Gen Variables=========================================================
        //Gen Jpsi Variables
        gentree->SetBranchAddress("GenJpsiMass",   &GenJpsiMass);
        gentree->SetBranchAddress("GenJpsiPt",     &GenJpsiPt);
        gentree->SetBranchAddress("GenJpsiPhi",    &GenJpsiPhi);
        gentree->SetBranchAddress("GenJpsiPsi",    &GenJpsiPsi);
        gentree->SetBranchAddress("GenJpsiRap",    &GenJpsiRap);
        gentree->SetBranchAddress("GenJpsiEta",    &GenJpsiEta);
        gentree->SetBranchAddress("GenJpsiPx",     &GenJpsiPx);
        gentree->SetBranchAddress("GenJpsiPy",     &GenJpsiPy);
        gentree->SetBranchAddress("GenJpsiPz",     &GenJpsiPz);
        gentree->SetBranchAddress("gbin",          &gbin);
        //muon variable
        gentree->SetBranchAddress("GenmuPosPx",    &GenmuPosPx);
        gentree->SetBranchAddress("GenmuPosPy",    &GenmuPosPy);
        gentree->SetBranchAddress("GenmuPosPz",    &GenmuPosPz);
        gentree->SetBranchAddress("GenmuPosEta",   &GenmuPosEta);
        gentree->SetBranchAddress("GenmuPosPhi",   &GenmuPosPhi);
        gentree->SetBranchAddress("GenmuNegPx",    &GenmuNegPx);
        gentree->SetBranchAddress("GenmuNegPy",    &GenmuNegPy);
        gentree->SetBranchAddress("GenmuNegPz",    &GenmuNegPz);
        gentree->SetBranchAddress("GenmuNegEta",   &GenmuNegEta);
        gentree->SetBranchAddress("GenmuNegPhi",   &GenmuNegPhi);

        //====================================================== Gen tree loop ================================================
        int NAccep=0;
        int nGenEntries=gentree->GetEntries();
        cout<<" Total Entries in GenLevel Tree for pT range: "<<fileName[ifile]<<"  "<<   nGenEntries<< " ==============="<<endl;

        for(int iGen=0; iGen< nGenEntries; iGen++)  {        
          //cout<<"i : "<<i<<endl;
          if(!(gentree->GetEntry(iGen))) continue;
          //cout<<" gentree ("<<i<<")"<<endl;
          //Only printing 
          if(iGen%10000==0){
            //cout<<" processing record "<<iGen<<"/"<<nGenEntries<<endl;
            //cout<<" Mass "<< GenJpsiMass<< " pT "<< GenJpsiPt << " Y " <<GenJpsiRap<<endl;
          }

          bool GenPosIn=0, GenNegIn=0;
          GenmuPosPt= TMath::Sqrt(GenmuPosPx*GenmuPosPx + GenmuPosPy*GenmuPosPy); 
          GenmuNegPt= TMath::Sqrt(GenmuNegPx*GenmuNegPx + GenmuNegPy*GenmuNegPy); 

          if(IsAccept(GenmuPosPt, GenmuPosEta)) {GenPosIn=1;}
          if(IsAccept(GenmuNegPt, GenmuNegEta)) {GenNegIn=1;}

          double GenCenWeight =0, GenWeight =0;
          GenCenWeight=FindCenWeight(gbin);   
          GenWeight=GenCenWeight*scale[ifile];

          hGenCent->Fill(gbin,GenWeight);

          int AccJpsi = 0;
          //if(GenJpsiPt < 6.5) continue;
          if((GenJpsiPt >= minPt && GenJpsiPt <= maxPt && fabs(GenJpsiRap) >= minRap && fabs(GenJpsiRap) <= maxRap && 
                GenPosIn == 1 && GenNegIn == 1 && GenJpsiMass > 2.0 && GenJpsiMass < 4.0)) {AccJpsi = 1;}

          if((GenPosIn ==1 && GenNegIn==1)) NAccep++;

          if(PutWeight==0) GenWeight=1; 

          //adding pT of all pt bins to see diss is cont

          //cout<<"1. GenJpsiPt : "<<GenJpsiPt<<", GenJpsiEta : "<<GenJpsiEta<<", GenJpsiRap : "<<GenJpsiRap<<", |GenJpsiPsi| : "<<TMath::Abs(GenJpsiPsi)<<endl;
          //cout<<"1. GenPosIn : "<<GenPosIn<<", GenNegIn : "<<GenNegIn<<endl;
          double vars = 0.0, bin1 = 0.0, bin2 = 0.0;
          if(iSpec == 0) vars = gbin;
          if(iSpec == 1) vars = GenJpsiPt;
          if(iSpec == 2) vars = fabs(GenJpsiRap);
          if(iSpec == 3) vars = GenJpsiPhi;
          if(iSpec == 4) vars = fabs(GenJpsiPsi);
          if(iSpec == 5) vars = fabs(GenJpsiPsi);
          for(int i = 0; i < nBins; i++){
            if(iSpec == 0){
              bin1 = ct_bound[i]; bin2 = ct_bound[i+1];
              if( (AccJpsi==1) && (vars >= bin1 && vars < bin2) && GenJpsiPt >= minPt && GenJpsiPt <= maxPt && fabs(GenJpsiRap) >= minRap && fabs(GenJpsiRap) < maxRap ) {
                hGenDiMuonf[ifile][i]->Fill(GenJpsiMass,GenWeight);
              }
            }
            if(iSpec == 1){
              bin1 = pt_bound[i]; bin2 = pt_bound[i+1];
              if( (AccJpsi==1) && (vars >= bin1 && vars < bin2) && gbin >= 0 && gbin <= 40 && fabs(GenJpsiRap) >= minRap && fabs(GenJpsiRap) < maxRap ) {
                hGenDiMuonf[ifile][i]->Fill(GenJpsiMass,GenWeight);
              }
            }
            if(iSpec == 2){
              bin1 = rap_bound[i]; bin2 = rap_bound[i+1];
              if( (AccJpsi==1) && (vars >= bin1 && vars < bin2) && gbin >= 0 && gbin <= 40 && GenJpsiPt >= minPt && GenJpsiPt < maxPt) {
                hGenDiMuonf[ifile][i]->Fill(GenJpsiMass,GenWeight);
              }
            }
            if(iSpec == 3){
              bin1 = phi_bound[i]; bin2 = phi_bound[i+1];
              if( (AccJpsi==1) && (vars >= bin1 && vars < bin2) && gbin >= 0 && gbin <= 40 && GenJpsiPt >= minPt && GenJpsiPt < maxPt && fabs(GenJpsiRap) < maxRap) {
                hGenDiMuonf[ifile][i]->Fill(GenJpsiMass,GenWeight);
              }
            }
            if(iSpec == 4){
              bin1 = dphi_bound[i]; bin2 = dphi_bound[i+1];
              if( (AccJpsi==1) && (vars >= bin1 && vars < bin2) && gbin >= 0 && gbin <= 40 && GenJpsiPt >= minPt && GenJpsiPt < maxPt && fabs(GenJpsiRap) < maxRap) {
                hGenDiMuonf[ifile][i]->Fill(GenJpsiMass,GenWeight);
              }
            }
            if(iSpec == 5){
              bin1 = dphi2_bound[i]; bin2 = dphi2_bound[i+1];
              if( (AccJpsi==1) && (vars >= bin1 && vars < bin2) && gbin >= 0 && gbin <= 40 && GenJpsiPt >= minPt && GenJpsiPt < maxPt && fabs(GenJpsiRap) < maxRap) {
                hGenDiMuonf[ifile][i]->Fill(GenJpsiMass,GenWeight);
              }
            }
          }
        }//gen loop end

        //cout<<" accepted no "<< NAccep<<endl;

        //=============== Rec Tree Loop ==============================================================================

        int nRecEntries=tree->GetEntries();
        cout<<"Total Entries in reconstructed Tree for pT range "<<fileName[ifile]<<"  "<<nRecEntries<< "====="<<endl;
        for(int iRec=0; iRec<nRecEntries; iRec++)  {     
          tree->GetEntry(iRec);
          //Only printing 
          if(iRec%10000==0){
            //cout<<" processing record "<<iRec<<"/"<<nRecEntries<<endl;
            //cout<<" processing Run  " <<runNb <<" event "<<eventNb<<" lum block "<<lumiBlock<<endl;    
            //cout<<" Mass "<< JpsiMass<< " pT "<< JpsiPt << " Y " <<JpsiRap<<"  "<<JpsiVprob<<" charge "<<JpsiCharge<<" rbin "<<rbin<<endl; 
          }
          bool PosPass=0, NegPass=0, AllCut=0 ,PosIn=0, NegIn=0;
          muPosPt= TMath::Sqrt(muPosPx*muPosPx + muPosPy*muPosPy); 
          muPosP = TMath::Sqrt(muPosPx*muPosPx + muPosPy*muPosPy+ muPosPz*muPosPz); 
          muNegPt= TMath::Sqrt(muNegPx*muNegPx + muNegPy*muNegPy); 
          muNegP = TMath::Sqrt(muNegPx*muNegPx + muNegPy*muNegPy +muNegPz*muNegPz); 

          double tnpWgt1 = 0.0, tnpWgt2 = 0.0;

          double x = 0.0;
          if(fabs(muPosEta) < 0.9){
            x = muPosPt;
            tnpWgt1 = fun1->Eval(x);
          }else if(fabs(muPosEta) < 1.6){
            x = muPosPt;
            tnpWgt1 = fun2->Eval(x);
          }else if(fabs(muPosEta) < 2.1){
            x = muPosPt;
            tnpWgt1 = fun3->Eval(x);
          }else{
            x = muPosPt;
            tnpWgt1 = fun4->Eval(x);
          }

          if(fabs(muNegEta) < 0.9){
            x = muNegPt;
            tnpWgt2 = fun1->Eval(x);
          }else if(fabs(muNegEta) < 1.6){
            x = muNegPt;
            tnpWgt2 = fun2->Eval(x);
          }else if(fabs(muNegEta) < 2.1){
            x = muNegPt;
            tnpWgt2 = fun3->Eval(x);
          }else{
            x = muNegPt;
            tnpWgt2 = fun4->Eval(x);
          }


          double RecCenWeight=0,RecWeight=0;
          RecCenWeight=FindCenWeight(rbin);   
          RecWeight=RecCenWeight*scale[ifile]*tnpWgt1*tnpWgt2;

          hRecCent->Fill(gbin,RecWeight);

          if(IsAccept(muPosPt, muPosEta)){PosIn=1;}
          if(IsAccept(muNegPt, muNegEta)){NegIn=1;}

          int AccJpsi = 0;
          if((JpsiPt >= minPt && JpsiPt <= maxPt && fabs(JpsiRap) >= minRap && fabs(JpsiRap) <= maxRap && PosIn == 1 && NegIn == 1)) {AccJpsi = 1;}

          /*
             cout<<"muPos_pixeLayers : "<<muPos_pixeLayers<<" muPos_nchi2In : "<<muPos_nchi2In
             <<" TMath::Abs(muPos_dxy) : "<<TMath::Abs(muPos_dxy)<<" TMath::Abs(muPos_dz) : "<<TMath::Abs(muPos_dz)
             <<" muPos_trkLayMeas : "<<muPos_trkLayMeas
             <<" muPos_arbitrated : "<<muPos_arbitrated<<" muPos_TMOneST : "<<muPos_TMOneST<<" muPos_tracker : "<<muPos_tracker<<endl;
             cout<<"muNeg_pixeLayers : "<<muNeg_pixeLayers<<" muNeg_nchi2In : "<<muNeg_nchi2In
             <<" TMath::Abs(muNeg_dxy) : "<<TMath::Abs(muNeg_dxy)<<" TMath::Abs(muNeg_dz) : "<<TMath::Abs(muNeg_dz)
             <<" muNeg_trkLayMeas : "<<muNeg_trkLayMeas
             <<" muNeg_arbitrated : "<<muNeg_arbitrated<<" muNeg_TMOneST : "<<muNeg_TMOneST<<" muNeg_tracker : "<<muNeg_tracker<<endl;
             */
          bool mu_Global = ((muPos_nchi2Gl >=0) && (muNeg_nchi2Gl >=0));
          bool mu_Tracker = ((muPos_tracker==1) && (muNeg_tracker==1));

          if(muPos_found > 10 &&
              muPos_pixeLayers > 0 &&
              muPos_nchi2In < 4.0 &&
              TMath::Abs(muPos_dxy) < 3 &&
              TMath::Abs(muPos_dz) < 15 && muPos_nchi2Gl < 20 &&
              muPos_arbitrated==1 &&
              muPos_tracker==1){
            PosPass=1;
          }

          if(muNeg_found > 10 &&
              muNeg_pixeLayers > 0 &&
              muNeg_nchi2In < 4.0 &&
              TMath::Abs(muNeg_dxy) < 3 &&
              TMath::Abs(muNeg_dz) < 15 &&
              muNeg_nchi2Gl < 20 &&
              muNeg_arbitrated==1 &&
              muNeg_tracker==1){
            NegPass=1;
          }

          if(hbit1 == 1 && (muPos_Trigger22==1 && muNeg_Trigger22==1) && (PosIn==1 && NegIn==1) && (PosPass==1 && NegPass==1) && mu_Global && mu_Tracker ){AllCut=1;}

          if(PutWeight==0)RecWeight=1;

          double JpsidPhi = JpsiGenPsi;

          //Eff loop for reco
          double vars = 0.0, bin1 = 0.0, bin2 = 0.0;
          if(iSpec == 0) vars = rbin;
          if(iSpec == 1) vars = JpsiPt;
          if(iSpec == 2) vars = fabs(JpsiRap);
          if(iSpec == 3) vars = JpsiPhi;
          if(iSpec == 4) vars = fabs(JpsidPhi);
          if(iSpec == 5) vars = fabs(JpsiGenPsi);
          if((JpsiCharge == 0) && (JpsiVprob > 0.01) && (JpsiMass >= 2.95 && JpsiMass < 3.25)) {     
            for(int i = 0; i < nBins; i++){
              if(iSpec == 0){
                bin1 = ct_bound[i]; bin2 = ct_bound[i+1];
                if( AccJpsi == 1 && (AllCut == 1) && (vars >= bin1 && vars < bin2) && JpsiPt >= minPt && JpsiPt <= maxPt && fabs(JpsiRap) >= minRap && fabs(JpsiRap) <= maxRap ) {
                  hRecoDiMuonf[ifile][i]->Fill(JpsiMass,RecWeight);
                }
              }
              if(iSpec == 1){
                bin1 = pt_bound[i]; bin2 = pt_bound[i+1];
                if( AccJpsi == 1 && (AllCut == 1) && (vars >= bin1 && vars < bin2) && rbin >= 0 && rbin <= 40 && fabs(JpsiRap) >= minRap && fabs(JpsiRap) <= maxRap ) {
                  hRecoDiMuonf[ifile][i]->Fill(JpsiMass,RecWeight);
                }
              }
              if(iSpec == 2){ 
                bin1 = rap_bound[i]; bin2 = rap_bound[i+1];
                if( AccJpsi == 1 && (AllCut == 1) && (vars >= bin1 && vars < bin2) && rbin >= 0 && rbin <= 40 && JpsiPt >= minPt && JpsiPt <= maxPt) {
                  hRecoDiMuonf[ifile][i]->Fill(JpsiMass,RecWeight);
                }
              }
              if(iSpec == 3){ 
                bin1 = phi_bound[i]; bin2 = phi_bound[i+1];
                if( AccJpsi == 1 && (AllCut == 1) && (vars >= bin1 && vars < bin2) && rbin >= 0 && rbin <= 40 && JpsiPt >= minPt && JpsiPt <= maxPt && fabs(JpsiRap) <=maxRap) {
                  hRecoDiMuonf[ifile][i]->Fill(JpsiMass,RecWeight);
                }
              }
              if(iSpec == 4){ 
                bin1 = dphi_bound[i]; bin2 = dphi_bound[i+1];
                if( AccJpsi == 1 && (AllCut == 1) && (vars >= bin1 && vars < bin2) && rbin >= 0 && rbin <= 40 && JpsiPt >= minPt && JpsiPt <= maxPt && fabs(JpsiRap) <=maxRap) {
                  hRecoDiMuonf[ifile][i]->Fill(JpsiMass,RecWeight);
                }
              }
              if(iSpec == 5){ 
                bin1 = dphi2_bound[i]; bin2 = dphi2_bound[i+1];
                if( AccJpsi == 1 && (AllCut == 1) && (vars >= bin1 && vars < bin2) && rbin >= 0 && rbin <= 40 && JpsiPt >= minPt && JpsiPt <= maxPt && fabs(JpsiRap) <=maxRap) {
                  hRecoDiMuonf[ifile][i]->Fill(JpsiMass,RecWeight);
                }
              }
            }
          }
        } //rec tree loop ends
      }  // file loop ends

      //======================  File loop Starts ============================

      ///////////////////////////////////////////////////////////////////
      cout<< " adding "<<endl;
      TCanvas *c1 = new TCanvas();
      char gtmp[512], gtmp1[512];
      char rtmp[512], rtmp1[512];

      for(int i = 0; i < nBins; i++){
        for(int ifile = 0; ifile < nFiles; ifile++){
          hGenDiMuon[i]->Add(hGenDiMuonf[ifile][i],1);
          hRecoDiMuon[i]->Add(hRecoDiMuonf[ifile][i],1);
        }
        if(iSpec == 0) sprintf(gtmp,"hGenDiMuon_Cent_%1.f_%1.f",ct_bound[i],ct_bound[i+1]);
        if(iSpec == 1) sprintf(gtmp,"hGenDiMuon_Pt_%0.1f_%0.1f",pt_bound[i],pt_bound[i+1]);
        if(iSpec == 2) sprintf(gtmp,"hGenDiMuon_Rap_%0.1f_%0.1f",rap_bound[i],rap_bound[i+1]);
        if(iSpec == 0) sprintf(gtmp1,"plots/Gen/hGenDiMuon_Cent_%1.f_%1.f_%s.png",ct_bound[i],ct_bound[i+1],cCd);
        if(iSpec == 1) sprintf(gtmp1,"plots/Gen/hGenDiMuon_Pt_%0.1f_%0.1f_%s.png",pt_bound[i],pt_bound[i+1],cCd);
        if(iSpec == 2) sprintf(gtmp1,"plots/Gen/hGenDiMuon_Rap_%0.1f_%0.1f_%s.png",rap_bound[i],rap_bound[i+1],cCd);
        if(iSpec == 0) sprintf(rtmp,"hRecDiMuon_Cent_%1.f_%1.f",ct_bound[i],ct_bound[i+1]);
        if(iSpec == 1) sprintf(rtmp,"hRecDiMuon_Pt_%0.1f_%0.1f",pt_bound[i],pt_bound[i+1]);
        if(iSpec == 2) sprintf(rtmp,"hRecDiMuon_Rap_%0.1f_%0.1f",rap_bound[i],rap_bound[i+1]);
        if(iSpec == 0) sprintf(rtmp1,"plots/Rec/hRecDiMuon_Cent_%1.f_%1.f_%s.png",ct_bound[i],ct_bound[i+1],cCd);
        if(iSpec == 1) sprintf(rtmp1,"plots/Rec/hRecDiMuon_Pt_%0.1f_%0.1f_%s.png",pt_bound[i],pt_bound[i+1],cCd);
        if(iSpec == 2) sprintf(rtmp1,"plots/Rec/hRecDiMuon_Rap_%0.1f_%0.1f_%s.png",rap_bound[i],rap_bound[i+1],cCd);
        hGenDiMuon[i]->SetName(gtmp);
        //hGenDiMuon[i]->Write();
        //hGenDiMuon[ipt][irap][idphi]->Draw();
        //c1->SaveAs(gtmp1);
        hRecoDiMuon[i]->SetName(rtmp);
        //hRecoDiMuon[i]->Write();
        //hRecoDiMuon[ipt][irap][idphi]->Draw();
        //c1->SaveAs(rtmp1);
      }

      TH1F *hReco = new TH1F();
      TH1F *hGen = new TH1F();
      hReco->Sumw2();
      hGen->Sumw2();
      int nRecoBins;
      int nGenBins;
      if(iSpec == 0) {hReco = new TH1F("hReco","hReco;Centrality (%);Weighted Yields",nCentBins,ct_bound2); nRecoBins = nCentBins;}
      if(iSpec == 1) {hReco = new TH1F("hReco","hReco;p_{T} GeV/c;Weighted Yields",nPtBins,pt_bound); nRecoBins = nPtBins;}
      if(iSpec == 2) {hReco = new TH1F("hReco","hReco;y;Weighted Yields",nRapBins,rap_bound); nRecoBins = nRapBins;}
      if(iSpec == 3) {hReco = new TH1F("hReco","hReco;#phi;Weighted Yields",nPhiBins,phi_bound); nRecoBins = nPhiBins;}
      if(iSpec == 4) {hReco = new TH1F("hReco","hReco;d#phi;Weighted Yields",ndPhiBins,dphi_bound); nRecoBins = ndPhiBins;}
      if(iSpec == 5) {hReco = new TH1F("hReco","hReco;d#phi;Weighted Yields",ndPhi2Bins,dphi2_bound); nRecoBins = ndPhi2Bins;}

      if(iSpec == 0) {hGen = new TH1F("hGen","hGen;Centrality (%);Weighted Yields",nCentBins,ct_bound2); nGenBins = nCentBins;}
      if(iSpec == 1) {hGen = new TH1F("hGen","hGen;p_{T} GeV/c;Weighted Yields",nPtBins,pt_bound); nGenBins = nPtBins;}
      if(iSpec == 2) {hGen = new TH1F("hGen","hGen;y;Weighted Yields",nRapBins,rap_bound); nGenBins = nRapBins;}
      if(iSpec == 3) {hGen = new TH1F("hGen","hGen;#phi;Weighted Yields",nPhiBins,phi_bound); nGenBins = nPhiBins;}
      if(iSpec == 4) {hGen = new TH1F("hGen","hGen;d#phi;Weighted Yields",ndPhiBins,dphi_bound); nGenBins = ndPhiBins;}
      if(iSpec == 5) {hGen = new TH1F("hGen","hGen;d#phi;Weighted Yields",ndPhi2Bins,dphi2_bound); nGenBins = ndPhi2Bins;}

      cout<<"Starts to calculate efficiency"<<endl;
      //dataFile<<""<<endl;
      //=====================Loop for eff========================================================================================//
      //define stuff here for error on weighted samples
      for(int i = 0; i < nBins; i++){
        int Gbinlow =hGenDiMuon[i]->GetXaxis()->FindBin(2.0);//2.95
        int Gbinhi=hGenDiMuon[i]->GetXaxis()->FindBin(4.0);//2.95
        //int Gbinlow =hGenDiMuon[i]->GetXaxis()->FindBin(2.95);//2.95
        //int Gbinhi=hGenDiMuon[i]->GetXaxis()->FindBin(3.25);//2.95

        int Rbinlow =hRecoDiMuon[i]->GetXaxis()->FindBin(2.95);//2.95
        int Rbinhi=hRecoDiMuon[i]->GetXaxis()->FindBin(3.25);//2.95

        cout<<"Gbinlow : "<<Gbinlow<<", Gbinhi : "<<Gbinhi<<endl;
        cout<<"Rbinlow : "<<Rbinlow<<", Rbinhi : "<<Rbinhi<<endl;

        genNo[i] = hGenDiMuon[i]->IntegralAndError(Gbinlow, Gbinhi, genErr[i]);
        recoNo[i] = hRecoDiMuon[i]->IntegralAndError(Rbinlow, Rbinhi, recoErr[i]);

        //calculate Eff         
        if(genNo[i] == 0 || recoNo[i] == 0) {
          cout<<"No Gen or Reco, # of Gen : "<<genNo[i] <<", # of Reco : "<<recoNo[i]<<endl;
          eff[i] = 0;
          effErr[i] = 0;
        }else{
          eff[i] = recoNo[i]/genNo[i]; 

          double tmpGenNo = genNo[i];
          double tmpGenErr = genErr[i];
          double tmpRecNo = recoNo[i];
          double tmpRecErr = recoErr[i];
          double tmpEff = eff[i];
          double tmpEffErr = 0.0;

          hReco->SetBinContent(i+1,tmpRecNo);
          hReco->SetBinError(i+1,tmpRecErr);
          hGen->SetBinContent(i+1,tmpGenNo);
          hGen->SetBinError(i+1,tmpGenErr);

          //error    
          double tmp_err_s1_1 = (tmpEff * tmpEff)/(tmpGenNo * tmpGenNo);
          double tmp_err_s1_2 = (tmpRecErr * tmpRecErr);
          double tmp_err_cat_s1 = tmp_err_s1_1 * tmp_err_s1_2;


          double tmp_err_s2_1 = ( (1 - tmpEff)*(1 - tmpEff) ) / (tmpGenNo * tmpGenNo);
          double tmp_err_s2_2 = TMath::Abs(( tmpGenErr*tmpGenErr ) - ( tmpRecErr * tmpRecErr));
          double tmp_err_cat_s2 = tmp_err_s2_1 * tmp_err_s2_2;
          tmpEffErr = sqrt( tmp_err_cat_s1 + tmp_err_cat_s2 );

          effErr[i] = tmpEffErr;

          //error without weight
          //dataFile<<" Bin ["<<i<<"] - "<< " Reco Jpsi : "<< tmpRecNo  <<", Gen Jpsi : "<< tmpGenNo <<endl;
          //dataFile<<" Eff ["<<i<<"] - "<< tmpEff <<" Error "<< tmpEffErr <<endl;
          cout<<" Bin ["<<i<<"] - "<< " Reco Jpsi : "<< tmpRecNo  <<", Gen Jpsi : "<< tmpGenNo <<endl;
          cout<<" Eff ["<<i<<"] - "<< tmpEff <<" Error "<< tmpEffErr <<endl;
        }
      }
      TH1F *hEff = new TH1F();
      hEff->Sumw2();
      int nEffBins = 0;
      if(iSpec == 0) {hEff = new TH1F("hEff","hEff;Centrality (%);Efficiency",nCentBins,ct_bound2); nEffBins = nCentBins;}
      if(iSpec == 1) {hEff = new TH1F("hEff","hEff;p_{T} GeV/c;Efficiency",nPtBins,pt_bound); nEffBins = nPtBins;}
      if(iSpec == 2) {hEff = new TH1F("hEff","hEff;y;Efficiency",nRapBins,rap_bound); nEffBins = nRapBins;}
      if(iSpec == 3) {hEff = new TH1F("hEff","hEff;#phi;Efficiency",nPhiBins,phi_bound); nEffBins = nPhiBins;}
      if(iSpec == 4) {hEff = new TH1F("hEff","hEff;d#phi;Efficiency",ndPhiBins,dphi_bound); nEffBins = ndPhiBins;}
      if(iSpec == 5) {hEff = new TH1F("hEff","hEff;d#phi;Efficiency",ndPhi2Bins,dphi2_bound); nEffBins = ndPhi2Bins;}

      for(int i = 0; i < nEffBins; i++){
        hEff->SetBinContent(i+1,eff[i]);
        hEff->SetBinError(i+1,effErr[i]);
        cout<<"Trying to measure Reconstruction eff vs "<<cSp[iSpec]<<endl;
        cout<<"Eff : "<<eff[i]<<", err : "<<effErr[i]<<endl;
        dataFile<<eff[i]<<endl;//", err : "<<effErr[i]<<endl;
        //dataFile<<"Eff : "<<eff[i]<<", err : "<<effErr[i]<<endl;
      }

      outfile->cd();
      hEff->Draw();
      char tmp_histo[512];
      sprintf(tmp_histo,"hEff_%s",cSp[iSpec]);
      char tmp_histo_reco[512];
      sprintf(tmp_histo_reco,"hReco_%s",cSp[iSpec]);
      char tmp_histo_gen[512];
      sprintf(tmp_histo_gen,"hGen_%s",cSp[iSpec]);
      hEff->SetName(tmp_histo);
      hEff->Write();
      hReco->SetName(tmp_histo_reco);
      hReco->Write();
      hGen->SetName(tmp_histo_gen);
      hGen->Write();
      //c1->SaveAs("eff_3D_default_etHFm.png");
      //outfile->Close();
      dataFile<<""<<endl;
      dataFile.close();
    }
    hGenCent->Write();
    hRecCent->Write();
    outfile->Write();
  }
}


bool IsAccept(Double_t pt, Double_t eta)
{

  //return(1);
  //return (fabs(eta) < 2.4); 

  return (fabs(eta) < 2.4 &&
      (    ( fabs(eta) < 1.0 && pt >= 3.4 ) ||
           (  1.0 <= fabs(eta) && fabs(eta) < 1.5 && pt >= 5.8-2.4*fabs(eta) ) ||
           (  1.5 <= fabs(eta) && pt >= 3.3667 - 7.0/9.0*fabs(eta)) ));
}


double FindCenWeight(int Bin)
{
  double NCollArray[40]={
    1747.8600, 1567.5300, 1388.3900, 1231.7700, 1098.2000, 980.4390, 861.6090, 766.0420, 676.5150, 593.4730,
    521.9120, 456.5420, 398.5460, 346.6470, 299.3050, 258.3440, 221.2160, 188.6770, 158.9860, 134.7000,
    112.5470, 93.4537, 77.9314, 63.5031, 52.0469, 42.3542, 33.9204, 27.3163, 21.8028, 17.2037,
    13.5881, 10.6538, 8.3555, 6.4089, 5.1334, 3.7322, 3.0663, 2.4193, 2.1190, 1.7695
  };
  return(NCollArray[Bin]);
}

// New correction (Trg X Muon Id)
double getEffEta(Double_t pt, Double_t eta)
{
  if(fabs(eta)<1.6) return ( 0.9988*TMath::Erf( (pt-1.322)/2.688)/TMath::Erf( (pt-1.796)/2.516) );
  if(1.6<fabs(eta)) return ( 1.04*TMath::Erf( (pt-1.41)/3.629)/TMath::Erf( (pt-1.571)/3.752) );
}
