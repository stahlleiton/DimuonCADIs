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
TF1 *f1 = new TF1("f1","TMath::Gaus(x,-0.3803,6.42,1)/TMath::Gaus(x,0.411,5.219,1)",-30,30);
TF1 *f2 = new TF1("f2","[0]*TMath::Erf((x-[1])/[2])/TMath::Erf((x-[3])/[4])",1.5,20);

TF1 *fun1 = new TF1("fun1","_fnc1_",0.0,30.0);
TF1 *fun2 = new TF1("fun2","_fnc2_",0.0,30.0);
TF1 *fun3 = new TF1("fun3","_fnc3_",0.0,30.0);
TF1 *fun4 = new TF1("fun4","_fnc4_",0.0,30.0);

void ppEffJpsi__idx_()   
{
  double minRap = 1.6;
  double maxRap = 2.4;
  double minPt = 3.0;
  double maxPt = 30.0;

  // if you want to measure low pT, you can set up minRap = 1.6 and minPt = 3.0, maxPt = 6.5;

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

  TFile *outfile;
  char tmp_output[512];
  sprintf(tmp_output,"ppPrJpsi_pT_%0.1f_%0.1f_y_%0.1f_%0.1f_HighQ_tnpWgt.root", minPt, maxPt, minRap, maxRap);
  outfile =new TFile(tmp_output, "Recreate");

  for(int iSpec = 0; iSpec < 1; iSpec++){

    const int nPtBins = 3;
    const int nRapBins = 6; // 6
    const int ndPhiBins = 4;
    double pt_bound[nPtBins+1] = {3.0, 4.5, 5.5, 6.5};
    double xpt_bound[nPtBins] = {0.0};
    //double rap_bound[nRapBins+1] = {0.0, 0.3, 0.6, 0.9, 1.2, 1.5, 1.8, 2.1, 2.4};
    double rap_bound[nRapBins+1] = {0.0, 0.4, 0.8, 1.2, 1.6, 2.0, 2.4};
    double xrap_bound[nRapBins] = {0.0};
    double dphi_bound[ndPhiBins+1] = {0.0, TMath::Pi()/8, TMath::Pi()/4, 3*TMath::Pi()/8, TMath::Pi()/2};
    double xdphi_bound[ndPhiBins] = {0.0};

    const char *cSp[2] = {"Pts","Raps"};
    char OutTextFile[100];
    sprintf(OutTextFile,"ppPromp_eff_%s_pT_%0.1f_%0.1f_y_%0.1f_%0.1f_HighQ.tex", cSp[iSpec], minPt, maxPt, minRap, maxRap);
    ofstream dataFile(OutTextFile);
    //ofstream dataFile(Form(OutTextFile));

    cout<< "%%%% Getting Efficiency starts : %s !!!!! %%%%%" << endl;
    //dataFile<< "%%%% Getting Efficiency starts : %s !!!!! %%%%%" << endl;

    // x, y, z - axis 
    //dataFile<<""<<endl;
    //dataFile<<"xaxis of pT"<<endl;
    for(int i = 0; i < nPtBins; i++){
      xpt_bound[i] = pt_bound[i] + (pt_bound[i+1]-pt_bound[i])/2;
      cout<<"xpt_bound["<<i<<"] : "<<xpt_bound[i]<<endl;
      //dataFile<<"xpt_bound["<<i<<"] : "<<xpt_bound[i]<<endl;
    }
    //dataFile<<""<<endl;
    //dataFile<<"xaxis of rap"<<endl;
    for(int i = 0; i < nRapBins; i++){
      xrap_bound[i] = rap_bound[i] + (rap_bound[i+1]-rap_bound[i])/2;
      cout<<"xrap_bound["<<i<<"] : "<<xrap_bound[i]<<endl;
      //dataFile<<"xrap_bound["<<i<<"] : "<<xrap_bound[i]<<endl;
    }
    //dataFile<<""<<endl;
    //dataFile<<"xaxis of dphi"<<endl;
    for(int i = 0; i < ndPhiBins; i++){
      xdphi_bound[i] = dphi_bound[i] + (dphi_bound[i+1]-dphi_bound[i])/2;
      cout<<"xdphi_bound["<<i<<"] : "<<xdphi_bound[i]<<endl;
      //dataFile<<"xdphi_bound["<<i<<"] : "<<xdphi_bound[i]<<endl;
    }

    int nBins_tmp = 0;
    if(iSpec == 0) { nBins_tmp = nPtBins; }
    if(iSpec == 1) { nBins_tmp = nRapBins; }
    const int nBins = nBins_tmp;

    TH1F *hTempMass = new TH1F("hTempMass","",100, 2.0, 4.0);
    TH1F *hGenDiMuon[nBins];
    TH1F *hRecoDiMuon[nBins];
    double genNo[nBins];
    double genErr[nBins];
    double recoNo[nBins];
    double recoErr[nBins];
    double eff[nBins];
    double effErr[nBins];
    for(int i = 0; i < nBins; i++){
      hGenDiMuon[i] = (TH1F*)hTempMass->Clone();
      hRecoDiMuon[i] = (TH1F*)hTempMass->Clone();
      hGenDiMuon[i]->Sumw2();
      hRecoDiMuon[i]->Sumw2();
    }

    char fileName[10][512];

    // loop for pT
    cout<<"==================Prompt PrJpsi================================================"<<endl;

    sprintf(fileName[0],"/Users/dmoon/Dropbox/Analysis/HiMC/EffStudy/gRpAngRootFiles_538_pp/HiDiMuonAna_538_pp_PromptJpsi_20140528_Trg21.root");


    TFile *infile;
    TTree *tree;
    TTree *gentree;
    TTree *evttree;

    infile=new TFile(fileName[0],"R");
    tree=(TTree*)infile->Get("SingleMuonTree");
    gentree=(TTree*)infile->Get("SingleGenMuonTree");
    evttree=(TTree*)infile->Get("EventTree");
    //Event variables
    int eventNb,runNb,lumiBlock;
    int hbit1;
    //double vertexZ;
    //double GenvertexZ;
    //Jpsi Variables
    Double_t JpsiMass,JpsiPt,JpsiRap, JpsiCharge;
    Double_t JpsiVprob;
    Double_t JpsiPhi;
    Double_t JpsiEta;
    //2.) muon variables RECO                                                                       
    double muPosPx, muPosPy, muPosPz,  muPosEta, muPosPt, muPosP, muPosPhi;
    double muNegPx, muNegPy, muNegPz,  muNegEta, muNegPt, muNegP, muNegPhi;
    //(1).Positive Muon                                     
    double muPos_nchi2In, muPos_dxy, muPos_dz, muPos_nchi2Gl;
    int muPos_found, muPos_pixeLayers, muPos_nValidMuHits,muPos_arbitrated;
    bool muPos_matches,muPos_tracker;
    int muPos_Trigger10, muPos_Trigger2, muPos_Trigger21, muPos_Trigger22;
    //(2).Negative Muon                                     
    double muNeg_nchi2In, muNeg_dxy, muNeg_dz, muNeg_nchi2Gl;
    int muNeg_found, muNeg_pixeLayers, muNeg_nValidMuHits,muNeg_arbitrated;
    bool muNeg_matches,muNeg_tracker;
    int muNeg_Trigger10, muNeg_Trigger2, muNeg_Trigger21, muNeg_Trigger22;

    //Gen Level variables
    //Gen PrJpsi Variables
    double GenJpsiMass, GenJpsiPt, GenJpsiRap;
    double GenJpsiPx, GenJpsiPy, GenJpsiPz;
    double GenJpsiPhi;
    double GenJpsiEta;
    //2.) Gen muon variables 
    double GenmuPosPx, GenmuPosPy, GenmuPosPz,  GenmuPosEta, GenmuPosPt, GenmuPosPhi;
    double GenmuNegPx, GenmuNegPy, GenmuNegPz,  GenmuNegEta, GenmuNegPt, GenmuNegPhi;
    double zVtx;

    //Event variables
    tree->SetBranchAddress("eventNb",&eventNb);
    tree->SetBranchAddress("runNb",&runNb);
    tree->SetBranchAddress("lumiBlock",&lumiBlock);
    tree->SetBranchAddress("hbit1",&hbit1);
    tree->SetBranchAddress("zVtx",&zVtx);
    //tree->SetBranchAddress("vertexZ",&vertexZ);

    //Jpsi Variables
    tree->SetBranchAddress("JpsiCharge",&JpsiCharge);
    tree->SetBranchAddress("JpsiMass",&JpsiMass);
    tree->SetBranchAddress("JpsiPt",&JpsiPt);
    tree->SetBranchAddress("JpsiPhi",&JpsiPhi);
    tree->SetBranchAddress("JpsiEta",&JpsiEta);
    tree->SetBranchAddress("JpsiRap",&JpsiRap);
    tree->SetBranchAddress("JpsiVprob",&JpsiVprob);

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
    tree->SetBranchAddress("muPos_Trigger10", &muPos_Trigger10);
    tree->SetBranchAddress("muPos_Trigger2", &muPos_Trigger2);
    tree->SetBranchAddress("muPos_Trigger21", &muPos_Trigger21);
    tree->SetBranchAddress("muPos_Trigger22", &muPos_Trigger22);

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
    tree->SetBranchAddress("muNeg_Trigger10", &muNeg_Trigger10);
    tree->SetBranchAddress("muNeg_Trigger2", &muNeg_Trigger2);
    tree->SetBranchAddress("muNeg_Trigger21", &muNeg_Trigger21);
    tree->SetBranchAddress("muNeg_Trigger22", &muNeg_Trigger22);
    //====================================Gen Variables=========================================================
    //Gen Jpsi Variables
    gentree->SetBranchAddress("GenJpsiMass",   &GenJpsiMass);
    gentree->SetBranchAddress("GenJpsiPt",     &GenJpsiPt);
    gentree->SetBranchAddress("GenJpsiPhi",    &GenJpsiPhi);
    gentree->SetBranchAddress("GenJpsiRap",    &GenJpsiRap);
    gentree->SetBranchAddress("GenJpsiEta",    &GenJpsiEta);
    gentree->SetBranchAddress("GenJpsiPx",     &GenJpsiPx);
    gentree->SetBranchAddress("GenJpsiPy",     &GenJpsiPy);
    gentree->SetBranchAddress("GenJpsiPz",     &GenJpsiPz);
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
    //gentree->SetBranchAddress("GenvertexZ",&GenvertexZ);

    //====================================================== Gen tree loop ================================================
    int NAccep=0;
    int nGenEntries=gentree->GetEntries();
    cout<<" Total Entries in GenLevel Tree for pT range: "<<fileName[0]<<"  "<<   nGenEntries<< " ==============="<<endl;

    for(int i=0; i< nGenEntries; i++)  {        
      //cout<<"i : "<<i<<endl;
      if(!(gentree->GetEntry(i))) continue;
      //cout<<" gentree ("<<i<<")"<<endl;
      //Only printing 
      if(i%1000000==0){
        cout<<" processing record "<<i<<"/"<<nGenEntries<<endl;
        //cout<<" Mass "<< GenJpsiMass<< " pT "<< GenJpsiPt << " Y " <<GenJpsiRap<<endl;
      }

      bool GenPosIn=0, GenNegIn=0;
      GenmuPosPt= TMath::Sqrt(GenmuPosPx*GenmuPosPx + GenmuPosPy*GenmuPosPy); 
      GenmuNegPt= TMath::Sqrt(GenmuNegPx*GenmuNegPx + GenmuNegPy*GenmuNegPy); 

      if(IsAccept(GenmuPosPt, GenmuPosEta)) {GenPosIn=1;}
      if(IsAccept(GenmuNegPt, GenmuNegEta)) {GenNegIn=1;}

      int AccJpsi = 0;
      //if(GenJpsiPt < 6.5) continue;
      if((GenJpsiPt >= minPt && GenJpsiPt <= maxPt && fabs(GenJpsiRap) >= minRap && TMath::Abs(GenJpsiRap) <= maxRap && GenPosIn == 1 && GenNegIn == 1)
          && GenJpsiMass > 2.0 && GenJpsiMass < 4.0
          ) {AccJpsi = 1;}

      if((GenPosIn ==1 && GenNegIn==1)) NAccep++;

      //cout<<"1. GenJpsiPt : "<<GenJpsiPt<<", GenJpsiEta : "<<GenJpsiEta<<", GenJpsiRap : "<<GenJpsiRap<<", |GenJpsiPsi| : "<<TMath::Abs(GenJpsiPsi)<<endl;
      //cout<<"1. GenPosIn : "<<GenPosIn<<", GenNegIn : "<<GenNegIn<<endl;
      double vars = 0.0, bin1 = 0.0, bin2 = 0.0;
      if(iSpec == 0) vars = GenJpsiPt;
      if(iSpec == 1) vars = fabs(GenJpsiRap);
      for(int j = 0; j < nBins; j++){
        if(iSpec == 0){
          bin1 = pt_bound[j]; bin2 = pt_bound[j+1];
          if( (AccJpsi==1) && (vars >= bin1 && vars < bin2) && fabs(GenJpsiRap) >= minRap && TMath::Abs(GenJpsiRap) <= maxRap && TMath::Abs(GenJpsiRap) >= minRap) {
            hGenDiMuon[j]->Fill(GenJpsiMass);
          }
        }
        if(iSpec == 1){
          bin1 = rap_bound[j]; bin2 = rap_bound[j+1];
          if( (AccJpsi==1) && (vars >= bin1 && vars < bin2) && GenJpsiPt >= minPt && GenJpsiPt <= maxPt) {
            hGenDiMuon[j]->Fill(GenJpsiMass);
          }
        }
      }
    }//gen loop end

    //cout<<" accepted no "<< NAccep<<endl;

    //=============== Rec Tree Loop ==============================================================================

    int nRecEntries=tree->GetEntries();
    cout<<"Total Entries in reconstructed Tree for pT range "<<fileName[0]<<"  "<<nRecEntries<< "====="<<endl;
    for(int i=0; i<nRecEntries; i++)  {     
      tree->GetEntry(i);
      //Only printing 
      if(i%1000000==0){
        cout<<" processing record "<<i<<"/"<<nRecEntries<<endl;
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


      double RecWeight = 1.0;
      RecWeight = tnpWgt1*tnpWgt2;
      

      if(IsAccept(muPosPt, muPosEta)){PosIn=1;}
      if(IsAccept(muNegPt, muNegEta)){NegIn=1;}

      int AccJpsi = 0;
      //if(JpsiPt < 6.5) continue;
      if((JpsiPt >= minPt && JpsiPt <= maxPt && fabs(JpsiRap) >= minRap && TMath::Abs(JpsiRap) <= maxRap && PosIn == 1 && NegIn == 1)
          && (JpsiMass >= 2.95 && JpsiMass < 3.25)
          ) {AccJpsi = 1;}

      bool mu_Global = ((muPos_nchi2Gl >=0) && (muNeg_nchi2Gl >=0));
      bool mu_Tracker = ((muPos_tracker==1) && (muNeg_tracker==1));

      if(muPos_found > 10 
          && muPos_pixeLayers > 0 
          && muPos_nchi2In < 4.0 
          && TMath::Abs(muPos_dxy) < 3 
          && TMath::Abs(muPos_dz) < 15 
          && muPos_nchi2Gl < 20 
          && muPos_arbitrated==1 
          && muPos_tracker==1){PosPass=1;}     

      if(muNeg_found > 10 
          && muNeg_pixeLayers > 0 
          && muNeg_nchi2In < 4.0 
          && TMath::Abs(muNeg_dxy) < 3 
          && TMath::Abs(muNeg_dz) < 15 
          && muNeg_nchi2Gl < 20 
          && muNeg_arbitrated==1 
          && muNeg_tracker==1){NegPass=1;}


      // Trigger10 : HLT_PAL2DoubleMu3_v1, Trigger 2 : HLT_PAL1DoubleMu0_HighQ_v1, Trigger 1 : HLT_PAL1DoubleMu0_v1
      // muPos_matches : HLT_PAL1DoubleMuOpen_v1
      //if((muPos_Trigger21==1 && muNeg_Trigger21==1) && (PosIn==1 && NegIn==1) && (PosPass==1 && NegPass==1)&& mu_Global && mu_Tracker){AllCut=1;}
      if(hbit1 == 1 && (muPos_Trigger22==1 && muNeg_Trigger22==1) && (PosIn==1 && NegIn==1) && (PosPass==1 && NegPass==1)&& mu_Global && mu_Tracker){AllCut=1;}
      //if(hbit1 == 1 && (muPos_Trigger2==1 && muNeg_Trigger2==1) && (PosIn==1 && NegIn==1) && (PosPass==1 && NegPass==1)&& mu_Global && mu_Tracker){AllCut=1;}
      //if((muPos_matches==1 && muNeg_matches==1) && (PosIn==1 && NegIn==1) && (PosPass==1 && NegPass==1)&& mu_Global && mu_Tracker){AllCut=1;}
      //if((muPos_Trigger10==1 && muNeg_Trigger10==1) && (PosIn==1 && NegIn==1) && (PosPass==1 && NegPass==1)&& mu_Global && mu_Tracker){AllCut=1;}

      // without trigger matched
      // if((PosIn==1 && NegIn==1) && (PosPass==1 && NegPass==1)&& mu_Global && mu_Tracker){AllCut=1;}

      //Eff loop for reco
      double vars = 0.0, bin1 = 0.0, bin2 = 0.0;
      if(iSpec == 0) vars = JpsiPt;
      if(iSpec == 1) vars = fabs(JpsiRap);
      if((JpsiCharge == 0) && (JpsiVprob > 0.01)) {     
        for(int j = 0; j < nBins; j++){
          if(iSpec == 0){
            bin1 = pt_bound[j]; bin2 = pt_bound[j+1];
            if( AccJpsi == 1 && (AllCut == 1) && (vars >= bin1 && vars < bin2) && TMath::Abs(JpsiRap) <= maxRap && TMath::Abs(JpsiRap) >= minRap) {
              hRecoDiMuon[j]->Fill(JpsiMass,RecWeight);
            }
          }
          if(iSpec == 1){ 
            bin1 = rap_bound[j]; bin2 = rap_bound[j+1];
            if( AccJpsi == 1 && (AllCut == 1) && (vars >= bin1 && vars < bin2) && JpsiPt >= minPt && JpsiPt <= maxPt) {
              hRecoDiMuon[j]->Fill(JpsiMass,RecWeight);
            }
          }
        }
      }
    } //rec tree loop ends

    //======================  File loop Starts ============================

    ///////////////////////////////////////////////////////////////////
    cout<< " adding "<<endl;
    char gtmp[512], gtmp1[512];
    char rtmp[512], rtmp1[512];

    for(int i = 0; i < nBins; i++){
      if(iSpec == 1) sprintf(gtmp,"hGenDiMuon_Pt_%0.1f_%0.1f",pt_bound[i],pt_bound[i+1]);
      if(iSpec == 2) sprintf(gtmp,"hGenDiMuon_Rap_%0.1f_%0.1f",rap_bound[i],rap_bound[i+1]);
      if(iSpec == 1) sprintf(gtmp1,"plots/Gen/hGenDiMuon_Pt_%0.1f_%0.1f.png",pt_bound[i],pt_bound[i+1]);
      if(iSpec == 2) sprintf(gtmp1,"plots/Gen/hGenDiMuon_Rap_%0.1f_%0.1f.png",rap_bound[i],rap_bound[i+1]);
      if(iSpec == 1) sprintf(rtmp,"hRecDiMuon_Pt_%0.1f_%0.1f",pt_bound[i],pt_bound[i+1]);
      if(iSpec == 2) sprintf(rtmp,"hRecDiMuon_Rap_%0.1f_%0.1f",rap_bound[i],rap_bound[i+1]);
      if(iSpec == 1) sprintf(rtmp1,"plots/Rec/hRecDiMuon_Pt_%0.1f_%0.1f.png",pt_bound[i],pt_bound[i+1]);
      if(iSpec == 2) sprintf(rtmp1,"plots/Rec/hRecDiMuon_Rap_%0.1f_%0.1f.png",rap_bound[i],rap_bound[i+1]);
      hGenDiMuon[i]->SetName(gtmp);
      hRecoDiMuon[i]->SetName(rtmp);
    }

    cout<<"Starts to calculate efficiency"<<endl;
    //dataFile<<""<<endl;
    //=====================Loop for eff========================================================================================//
    //define stuff here for error on weighted samples
    for(int i = 0; i < nBins; i++){
      int Gbinlow =hGenDiMuon[i]->GetXaxis()->FindBin(2.0);//2.95
      int Gbinhi=hGenDiMuon[i]->GetXaxis()->FindBin(4.0);//2.95

      int Rbinlow =hRecoDiMuon[i]->GetXaxis()->FindBin(2.95);//2.95
      int Rbinhi=hRecoDiMuon[i]->GetXaxis()->FindBin(3.25);//2.95

      genNo[i] = hGenDiMuon[i]->IntegralAndError(Gbinlow, Gbinhi, genErr[i]); 
      recoNo[i] = hRecoDiMuon[i]->IntegralAndError(Rbinlow, Rbinhi, recoErr[i]);
      //calculate Eff         
      if(genNo[i] == 0 || recoNo[i] == 0) {
        eff[i] = 0;
        effErr[i] = 0;
      }else{
        eff[i] = recoNo[i]/genNo[i]; 

        effErr[i] = recoNo[i]/genNo[i]*TMath::Sqrt(genErr[i]*genErr[i]/(genNo[i]*genNo[i]) + recoErr[i]*recoErr[i]/(recoNo[i]*recoNo[i]));

        //error without weight
        //dataFile<<" Bin ["<<i<<"] - "<< " Reco Jpsi : "<< recoNo[i]  <<", Gen Jpsi : "<< genNo[i] <<endl;
        //dataFile<<" Eff ["<<i<<"] - "<< eff[i] <<" Error "<< effErr[i] <<endl;
        cout<<" Bin ["<<i<<"] - "<< " Reco Jpsi : "<< recoNo[i]  <<", Gen Jpsi : "<< genNo[i] <<endl;
        cout<<" Eff ["<<i<<"] - "<< eff[i] <<" Error "<< effErr[i] <<endl;
      }
    }
    TH1F *hEff = new TH1F();
    int nEffBins = 0;
    if(iSpec == 0) {hEff = new TH1F("hEff","hEff;p_{T} GeV/c;Efficiency",nPtBins,pt_bound); nEffBins = nPtBins;}
    if(iSpec == 1) {hEff = new TH1F("hEff","hEff;y;Efficiency",nRapBins,rap_bound); nEffBins = nRapBins;}

    //dataFile<<"Efficiency"<<endl;
    for(int i = 0; i < nEffBins; i++){
      hEff->SetBinContent(i+1,eff[i]);
      hEff->SetBinError(i+1,effErr[i]);
      cout<<"Trying to measure eff vs "<<cSp[iSpec]<<endl;
      cout<<"Eff : "<<eff[i]<<", err : "<<effErr[i]<<endl;
      dataFile<<eff[i]<<endl;
    }

    //dataFile<<""<<endl;
    //dataFile<<"Errors"<<endl;
    for(int i = 0; i < nEffBins; i++){
      hEff->SetBinContent(i+1,eff[i]);
      hEff->SetBinError(i+1,effErr[i]);
      cout<<"Trying to measure eff vs "<<cSp[iSpec]<<endl;
      cout<<"Eff : "<<eff[i]<<", err : "<<effErr[i]<<endl;
      //dataFile<<effErr[i]<<endl;
    }


    outfile->cd();
    hEff->Draw();
    char tmp_histo[512];
    sprintf(tmp_histo,"hEff_%s",cSp[iSpec]);
    hEff->SetName(tmp_histo);
    hEff->Write();
    //dataFile<<""<<endl;
    dataFile.close();
  }
  outfile->Write();
}


bool IsAccept(Double_t pt, Double_t eta)
{

  //return(1);
  //return (TMath::Abs(eta) < 2.4); 

  return (TMath::Abs(eta) < 2.4 &&
      (    ( TMath::Abs(eta) < 1.0 && pt >= 3.4 ) ||
           (  1.0 <= TMath::Abs(eta) && TMath::Abs(eta) < 1.5 && pt >= 5.8-2.4*TMath::Abs(eta) ) ||
           (  1.5 <= TMath::Abs(eta) && pt >= 3.3667 - 7.0/9.0*TMath::Abs(eta)) ));
}

// New correction (Trg X Muon Id)
double getEffEta(Double_t pt, Double_t eta)
{
  // TF1 *f2 = new TF1("f2","[0]*TMath::Erf((x-[1])/[2])/TMath::Erf((x-[3])/[4])",1.5,20);
  if(fabs(eta)<1.6) return ( 0.998474*TMath::Erf( (pt-1.63555)/(0.797933) ) / TMath::Erf( (pt-0.222866)/(2.95593) ) );
  if(1.6<fabs(eta)) return ( 0.6977*TMath::Erf( (pt-1.61309)/(-1.03684) ) + 1.76794);
  //if(1.6<fabs(eta)) return ( 1.06963*TMath::Erf( (pt+317.027)/(-0.180123) ) / TMath::Erf( (pt-1.00156)/(-1.36329) ) );
}
