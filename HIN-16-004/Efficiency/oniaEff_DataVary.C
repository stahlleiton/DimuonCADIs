#define oniaEff_DataVary_cxx
#include "oniaEff_DataVary.h"
#include "tnp_weight.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TMath.h>
#include <TF1.h>
#include <TKey.h>
#include <TClass.h>
#include <TObjArray.h>
#include <TSystem.h>

#include <iostream>

TObjArray* readFileWeight(const char* file);

// define bins
const int nbins_centmid = 6;
const float bins_centmid[nbins_centmid+1] = {0, 10, 20, 30, 40, 50, 100};
const int nbins_centfwd = 3;
const float bins_centfwd[nbins_centfwd+1] = {0, 20, 40, 100};
const int nbins_ptmid = 5;
const float bins_ptmid[nbins_ptmid+1] = {6.5, 9, 12, 15, 20, 30};
const int nbins_ptfwd = 3;
const float bins_ptfwd[nbins_ptfwd+1] = {3, 6.5, 12, 30};

// NP rejection cuts
const double ctaucutmid_pp = 0.03;
const double ctaucutfwd_pp = 0.05;
const double ctaucutmid_pbpb = 0.03;
const double ctaucutfwd_pbpb = 0.05;

// pt-dependent ctau cuts
const double ctaucut_a_mid_pp = 0.010;
const double ctaucut_a_fwd_pp = 0.013;
const double ctaucut_a_mid_pbpb = 0.013;
const double ctaucut_a_fwd_pbpb = 0.015;
const double ctaucut_b_mid_pp = 0.25;
const double ctaucut_b_fwd_pp = 0.29;
const double ctaucut_b_mid_pbpb = 0.22;
const double ctaucut_b_fwd_pbpb = 0.28;

// other settings
const double maxdr = 0.03;
const double massjpsi = 3.096;
const double masspsip = 3.686;
const double massdown = 0.25;
const double massup = 0.15;

// function for ctau cuts
bool ctaucut(double ctau, double y, double pt, bool ispbpb) {
   double a=1,b=1;
   if (ispbpb) {
      if (fabs(y)<1.6){a=ctaucut_a_mid_pbpb; b=ctaucut_b_mid_pbpb;} 
      else {a=ctaucut_a_fwd_pbpb; b=ctaucut_b_fwd_pbpb;}
   } else {
      if (fabs(y)<1.6){a=ctaucut_a_mid_pp; b=ctaucut_b_mid_pp;} 
      else {a=ctaucut_a_fwd_pp; b=ctaucut_b_fwd_pp;}
   }

   return ctau < a + b / pt;
};

using namespace HI;
using namespace std;

void oniaEff_DataVary::Loop(const char* fname, bool ispbpb, bool isPsip)
{
//   In a ROOT session, you can do:
//      root> .L oniaEff.C
//      root> oniaEff t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
   fChain->SetBranchStatus("*",0);  // disable all branches
   fChain->SetBranchStatus("Centrality",1);  
   fChain->SetBranchStatus("HLTriggers",1);  
   fChain->SetBranchStatus("Reco_QQ_*",1);  
   fChain->SetBranchStatus("Gen_QQ_*",1);  
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();
   nentries = 5000;
   
   // define the histos
   // we need 2 numerators (with and without ctau3d cut), 1 denominator. Let's make them in arrays
   TH1F *hnum_centmid = new TH1F("hnum_centmid","hnum_centmid",nbins_centmid,bins_centmid);
   TH1F *hnum_centfwd = new TH1F("hnum_centfwd","hnum_centfwd",nbins_centfwd,bins_centfwd);
   TH1F *hnum_ptmid = new TH1F("hnum_ptmid","hnum_ptmid",nbins_ptmid,bins_ptmid);
   TH1F *hnum_ptfwd = new TH1F("hnum_ptfwd","hnum_ptfwd",nbins_ptfwd,bins_ptfwd);
   TH1F *hnumcut_centmid = new TH1F("hnumcut_centmid","hnumcut_centmid",nbins_centmid,bins_centmid);
   TH1F *hnumcut_centfwd = new TH1F("hnumcut_centfwd","hnumcut_centfwd",nbins_centfwd,bins_centfwd);
   TH1F *hnumcut_ptmid = new TH1F("hnumcut_ptmid","hnumcut_ptmid",nbins_ptmid,bins_ptmid);
   TH1F *hnumcut_ptfwd = new TH1F("hnumcut_ptfwd","hnumcut_ptfwd",nbins_ptfwd,bins_ptfwd);
   TH1F *hnumptdepcut_centmid = new TH1F("hnumptdepcut_centmid","hnumptdepcut_centmid",nbins_centmid,bins_centmid);
   TH1F *hnumptdepcut_centfwd = new TH1F("hnumptdepcut_centfwd","hnumptdepcut_centfwd",nbins_centfwd,bins_centfwd);
   TH1F *hnumptdepcut_ptmid = new TH1F("hnumptdepcut_ptmid","hnumptdepcut_ptmid",nbins_ptmid,bins_ptmid);
   TH1F *hnumptdepcut_ptfwd = new TH1F("hnumptdepcut_ptfwd","hnumptdepcut_ptfwd",nbins_ptfwd,bins_ptfwd);
   TH1F *hden_centmid = new TH1F("hden_centmid","hden_centmid",nbins_centmid,bins_centmid);
   TH1F *hden_centfwd = new TH1F("hden_centfwd","hden_centfwd",nbins_centfwd,bins_centfwd);
   TH1F *hden_ptmid = new TH1F("hden_ptmid","hden_ptmid",nbins_ptmid,bins_ptmid);
   TH1F *hden_ptfwd = new TH1F("hden_ptfwd","hden_ptfwd",nbins_ptfwd,bins_ptfwd);

   // also store more finely binned histos
   TH1F *hcentfine = new TH1F("hcentfine","hcentfine",200,0,200);
   TH2F *hnum2d = new TH2F("hnum2d","hnum2d",48,0,2.4,150,0,30);
   TH2F *hden2d = new TH2F("hden2d","hden2d",48,0,2.4,150,0,30);
   
   //Tobjext array for weighting function
   TObjArray*  wtHisto_num_centmid =  new TObjArray();
   TObjArray*  wtHisto_num_ptmid =  new TObjArray();
   TObjArray*  wtHisto_den_centmid =  new TObjArray();
   TObjArray*  wtHisto_den_ptmid =  new TObjArray();
   TObjArray*  wtHisto_num_centfwd =  new TObjArray();
   TObjArray*  wtHisto_num_ptfwd =  new TObjArray();
   TObjArray*  wtHisto_den_centfwd =  new TObjArray();
   TObjArray*  wtHisto_den_ptfwd =  new TObjArray();
   TObjArray* wtFnMid = NULL;
   TObjArray* wtFnFwd = NULL;
   TString colltag, objtag, raptag;


   if(!ispbpb && !isPsip){
     wtFnMid = readFileWeight(Form("%s/wFunctions/%s",gSystem->ExpandPathName(gSystem->pwd()), "weights_JPsi_PP_Mid.root"));
     wtFnFwd  = readFileWeight(Form("%s/wFunctions/%s",gSystem->ExpandPathName(gSystem->pwd()), "weights_JPsi_PP_Fwd.root"));
   }
   if(ispbpb && !isPsip){
     wtFnMid = readFileWeight(Form("%s/wFunctions/%s",gSystem->ExpandPathName(gSystem->pwd()), "weights_JPsi_PbPb_Mid.root"));
     wtFnFwd = readFileWeight(Form("%s/wFunctions/%s",gSystem->ExpandPathName(gSystem->pwd()), "weights_JPsi_PbPb_Fwd.root"));
   } 
   if(!ispbpb && isPsip) {
     wtFnMid = readFileWeight(Form("%s/wFunctions/%s",gSystem->ExpandPathName(gSystem->pwd()), "weights_Psi2S_PP_Mid.root"));
     wtFnFwd = readFileWeight(Form("%s/wFunctions/%s",gSystem->ExpandPathName(gSystem->pwd()), "weights_Psi2S_PP_Fwd.root"));
   }
   if(ispbpb && isPsip){
     wtFnMid = readFileWeight(Form("%s/wFunctions/%s",gSystem->ExpandPathName(gSystem->pwd()), "weights_Psi2S_PbPb_Mid.root"));
     wtFnFwd = readFileWeight(Form("%s/wFunctions/%s",gSystem->ExpandPathName(gSystem->pwd()), "weights_Psi2S_PbPb_Fwd.root"));
   }
   
   TF1* function(0x0);
   TString fnName;
   wtHisto_num_centmid->SetOwner(kTRUE);
   wtHisto_num_ptmid->SetOwner(kTRUE);
   wtHisto_den_centmid->SetOwner(kTRUE);
   wtHisto_den_ptmid->SetOwner(kTRUE);
   wtHisto_num_centfwd->SetOwner(kTRUE);
   wtHisto_num_ptfwd->SetOwner(kTRUE);
   wtHisto_den_centfwd->SetOwner(kTRUE);
   wtHisto_den_ptfwd->SetOwner(kTRUE);
   //wtFnMid->SetOwner(kTRUE);
   //wtFnFwd->SetOwner(kTRUE);
   
   if (!wtFnMid && !wtFnFwd)
     {
       cout << "[ERROR] both weight function do not exist" << endl;
       return;
     }

   //wtFnMid = static_cast<TObjArray*>(fileWt->FindObjectAny("wFunctions"));
   TIter nextFuncMid(wtFnMid);
   while ( (function = static_cast<TF1*>(nextFuncMid())) )
     {
       fnName = function->GetName();
       TH1F *h1 = new TH1F(Form("hnumptdepcut_centmid_%s",function->GetName()),"", nbins_centmid,bins_centmid);
       TH1F *h2 = new TH1F(Form("hnumptdepcut_ptmid_%s",function->GetName()),"", nbins_ptmid,bins_ptmid);
       TH1F *h3 = new TH1F(Form("hden_centmid_%s",function->GetName()),"", nbins_centmid,bins_centmid);
       TH1F *h4 = new TH1F(Form("hden_ptmid_%s",function->GetName()),"", nbins_ptmid,bins_ptmid);
       
       wtHisto_num_centmid->Add(h1);
       wtHisto_num_ptmid->Add(h2);
       wtHisto_den_centmid->Add(h3);
       wtHisto_den_ptmid->Add(h4);
     }
   
   //wtFnFwd = static_cast<TObjArray*>(fileWt->FindObjectAny("wFunctions"));
   TIter nextFuncFwd(wtFnFwd);
   while ( (function = static_cast<TF1*>(nextFuncFwd())) )
     {
       fnName = function->GetName();
       TH1F *h5 = new TH1F(Form("hnumptdepcut_centfwd_%s",function->GetName()),"", nbins_centfwd,bins_centfwd);
       TH1F *h6 = new TH1F(Form("hnumptdepcut_ptfwd_%s",function->GetName()),"", nbins_ptfwd,bins_ptfwd);
       TH1F *h7 = new TH1F(Form("hden_centfwd_%s",function->GetName()),"", nbins_centfwd,bins_centfwd);
       TH1F *h8 = new TH1F(Form("hden_ptfwd_%s",function->GetName()),"", nbins_ptfwd,bins_ptfwd);
       
       wtHisto_num_centfwd->Add(h5);
       wtHisto_num_ptfwd->Add(h6);
       wtHisto_den_centfwd->Add(h7);
       wtHisto_den_ptfwd->Add(h8);
     }
   
   TH1* hWt1; TH1* hWt2; TH1* hWt3; TH1* hWt4;
   TH1* hWt5; TH1* hWt6; TH1* hWt7; TH1* hWt8;

   //nentries = 200;
   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      // if (ientry>1000) break;

      // skip the event if Gen_QQ_size != 1 for now
      b_Gen_QQ_size->GetEntry(ientry); //read only this branch
      if (Gen_QQ_size !=1) continue;

      // apply gen acceptance cuts
      b_Gen_QQ_mupl_4mom->GetEntry(ientry); //read only this branch
      b_Gen_QQ_mumi_4mom->GetEntry(ientry); //read only this branch
      TLorentzVector *tlvgenpl = (TLorentzVector*) Gen_QQ_mupl_4mom->At(0);
      TLorentzVector *tlvgenmi = (TLorentzVector*) Gen_QQ_mumi_4mom->At(0);
      //if (!isGlobalMuonInAccept2015(tlvgenpl) || !isGlobalMuonInAccept2015(tlvgenmi)) continue;
      bool isgenok = true;
      if (!isGlobalMuonInAccept2015(tlvgenpl) || !isGlobalMuonInAccept2015(tlvgenmi)) isgenok=false;
      
      // fill denominators
      b_Gen_QQ_4mom->GetEntry(ientry); //read only this branch
      TLorentzVector *tlvgenqq = (TLorentzVector*) Gen_QQ_4mom->At(0);
      double genpt = tlvgenqq->Pt();
      b_Centrality->GetEntry(ientry); //read only this branch

      // check that the dimuon is inside the analysis bins
      bool gen_inbin = false;
      if (fabs(tlvgenqq->Rapidity())<1.6 && tlvgenqq->Pt()>6.5 && tlvgenqq->Pt()<30.) gen_inbin = true;
      if (fabs(tlvgenqq->Rapidity())>1.6 && fabs(tlvgenqq->Rapidity())<2.4 && tlvgenqq->Pt()>3. && tlvgenqq->Pt()<30.) gen_inbin = true;
      //if (!gen_inbin) continue;
      if (!gen_inbin) isgenok=false;
      
      double weight = ispbpb ? fChain->GetWeight()*findNcoll(Centrality) : 1.;
      double weight_DataMC = 0.0;
      
      if (!wtFnMid || !wtFnFwd)
	{
	  cout << "[ERROR] No weight array exist" << endl;
	  return;
	}
      
      TIter nextFunc2Mid(wtFnMid);
      TIter nextFunc2Fwd(wtFnFwd);
      TString fnName;
      
      if(fabs(tlvgenqq->Rapidity())<1.6){
	while ( (function = static_cast<TF1*>(nextFunc2Mid())) )
	  {
	    weight = ispbpb ? fChain->GetWeight()*findNcoll(Centrality) : 1.;
	    fnName = function->GetName();
	    hWt3 = static_cast<TH1*>(wtHisto_den_centmid->FindObject(Form("hden_centmid_%s",function->GetName())));
	    hWt4 = static_cast<TH1*>(wtHisto_den_ptmid->FindObject(Form("hden_ptmid_%s",function->GetName())));
	    
	    weight_DataMC = function->Eval(genpt);
	    if(weight_DataMC < 0) weight_DataMC = 0.0;
	    weight = weight*weight_DataMC;
	    hWt3->Fill(Centrality/2,weight);
	    hWt4->Fill(genpt,weight);
	  }
      }
      
      if(fabs(tlvgenqq->Rapidity())>1.6){
	while ( (function = static_cast<TF1*>(nextFunc2Fwd())) )
	  {
	    weight = ispbpb ? fChain->GetWeight()*findNcoll(Centrality) : 1.;
	    fnName = function->GetName();
	    hWt7 = static_cast<TH1*>(wtHisto_den_centfwd->FindObject(Form("hden_centfwd_%s",function->GetName())));
	    hWt8 = static_cast<TH1*>(wtHisto_den_ptfwd->FindObject(Form("hden_ptfwd_%s",function->GetName())));
	    
	    weight_DataMC = function->Eval(genpt);
	    if(weight_DataMC < 0) weight_DataMC = 0.0;
	    weight = weight*weight_DataMC;
	    hWt7->Fill(Centrality/2,weight);
	    hWt8->Fill(genpt,weight);
	  }
	  
	}
   
      //weight = weight*weight_DataMC;
      TF1 *wfunt = new TF1("wfunt","[0] + [1]*x",0.0,30.0);
      wfunt->SetParameters(1.3, -0.02); 
      //weight = weight*wfunt->Eval(genpt);
      
      hcentfine->Fill(Centrality,weight);
      hden2d->Fill(fabs(tlvgenqq->Rapidity()),genpt,weight);
      
      if(isgenok==true){
	if (fabs(tlvgenqq->Rapidity()) < 1.6) {
	  hden_centmid->Fill(Centrality/2,weight);
	  hden_ptmid->Fill(genpt,weight);
	} else if (fabs(tlvgenqq->Rapidity()) < 2.4) {
	  hden_centfwd->Fill(Centrality/2,weight);
	  hden_ptfwd->Fill(genpt,weight);
	} //else continue;
      }
   
      // is there at least one reco dimuon?
      b_Reco_QQ_size->GetEntry(ientry);
      if (Reco_QQ_size==0) continue;

      // ok, now read all other branches
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;

      // loop on the reconstructed dimuons to find the one matched to the gen one
      double mindr=999.;
      int ibestqq=-1;
      double tnp_weight = 1.0;
      for (int i=0; i<Reco_QQ_size; i++) {
	// acceptance
	if (!areMuonsInAcceptance2015(i)) continue;
         // sign
         if (Reco_QQ_sign[i]!=0) continue;
         // quality cuts
         if (!passQualityCuts2015(i)) continue;
         // trigger matching
         if (!isTriggerMatch(i,0)) continue; // HLT_HIL1DoubleMu0_v1
         // mass cut
         double mass = ((TLorentzVector*) Reco_QQ_4mom->At(i))->M();
         double mass0 = isPsip ? masspsip : massjpsi;
	 double massdown = isPsip ? 0.5 : 0.6;
	 double massup = isPsip ? 0.5 : 0.4;
         if (mass<(mass0-massdown) || mass>(mass0+massup)) continue;
         
	 // check that the dimuon is inside the analysis bins
         bool rec_inbin = false;
         TLorentzVector *tlvrecqq = (TLorentzVector*) Reco_QQ_4mom->At(i);
	 if (fabs(tlvrecqq->Rapidity())<1.6 && tlvrecqq->Pt()>6.5 && tlvrecqq->Pt()<30.) rec_inbin = true;
         if (fabs(tlvrecqq->Rapidity())>1.6 && fabs(tlvrecqq->Rapidity())<2.4 && tlvrecqq->Pt()>3. && tlvrecqq->Pt()<30.) rec_inbin = true;
         if (!rec_inbin) continue;
	 
	 // gen-reco matching
         TLorentzVector *tlvrecpl = (TLorentzVector*) Reco_QQ_mupl_4mom->At(i);
         TLorentzVector *tlvrecmi = (TLorentzVector*) Reco_QQ_mumi_4mom->At(i);
	 double dr = max(tlvrecpl->DeltaR(*tlvgenpl),tlvrecmi->DeltaR(*tlvgenmi));
         if (dr<mindr) {
	   mindr = dr;
	   ibestqq = i;
	 }

      } // Reco_QQ loop
      
      if (ibestqq<0) continue;
      
      weight = ispbpb ? fChain->GetWeight()*findNcoll(Centrality) : 1.;

      TLorentzVector *tlvrecqq = (TLorentzVector*) Reco_QQ_4mom->At(ibestqq);
      double recopt = tlvrecqq->Pt();
      
      TLorentzVector *tlvrecpl = (TLorentzVector*) Reco_QQ_mupl_4mom->At(ibestqq);
      TLorentzVector *tlvrecmi = (TLorentzVector*) Reco_QQ_mumi_4mom->At(ibestqq);
      double recMuPlpt = tlvrecpl->Pt();
      double recMuPlEta = tlvrecpl->Eta();
      double recMuMipt = tlvrecmi->Pt();
      double recMuMiEta = tlvrecmi->Eta();
      
      tnp_weight = tnp_weight_sta_pbpb(recMuPlpt, recMuPlEta, 0)*tnp_weight_sta_pbpb(recMuMipt, recMuMiEta, 0)*tnp_weight_muidtrg_pbpb(recMuPlpt, recMuPlEta, 0) * tnp_weight_muidtrg_pbpb(recMuMipt, recMuMiEta, 0);
      //weight = weight*tnp_weight;
      
      // fill the numerators
      if (fabs(tlvrecqq->Rapidity()) < 1.6) {
	hnum_centmid->Fill(Centrality/2,weight);
	hnum_ptmid->Fill(recopt,weight);
      } else {
	hnum_centfwd->Fill(Centrality/2,weight);
	hnum_ptfwd->Fill(recopt,weight);
      }
      
      // apply ctau3D cut
      bool ctaucutok = false;
      if (ispbpb) {
	if (fabs(tlvrecqq->Rapidity()) < 1.6) {
	  ctaucutok = (Reco_QQ_ctau3D[ibestqq] < ctaucutmid_pbpb);
	} else {
	  ctaucutok = (Reco_QQ_ctau3D[ibestqq] < ctaucutfwd_pbpb);
	}
      } else {
	if (fabs(tlvrecqq->Rapidity()) < 1.6) {
	  ctaucutok = (Reco_QQ_ctau3D[ibestqq] < ctaucutmid_pp);
	} else {
	  ctaucutok = (Reco_QQ_ctau3D[ibestqq] < ctaucutfwd_pp);
	}
      }
      
      if (ctaucutok) {
	if (fabs(tlvrecqq->Rapidity()) < 1.6) {
	  hnumcut_centmid->Fill(Centrality/2,weight);
	  hnumcut_ptmid->Fill(recopt,weight);
	} else {
	  hnumcut_centfwd->Fill(Centrality/2,weight);
	  hnumcut_ptfwd->Fill(recopt,weight);
	}
      }
      
      bool ctauptdepcutok = ctaucut(Reco_QQ_ctau3D[ibestqq], tlvrecqq->Rapidity(), tlvrecqq->Pt(), ispbpb);
      
      if (ctauptdepcutok) {
	if (fabs(tlvrecqq->Rapidity()) < 1.6) {
	  hnumptdepcut_centmid->Fill(Centrality/2,weight);
	  hnumptdepcut_ptmid->Fill(recopt,weight);
	} else {
	  hnumptdepcut_centfwd->Fill(Centrality/2,weight);
	  hnumptdepcut_ptfwd->Fill(recopt,weight);
	}
      }
      
      TIter nextFunc3Mid(wtFnMid);
      TIter nextFunc3Fwd(wtFnFwd);

      if (ctauptdepcutok) {
	if(fabs(tlvrecqq->Rapidity())<1.6){
	  while ( (function = static_cast<TF1*>(nextFunc3Mid())) )
	    {
	      fnName = function->GetName();
	      weight = ispbpb ? fChain->GetWeight()*findNcoll(Centrality) : 1.;
	      
	      hWt1 = static_cast<TH1*>(wtHisto_num_centmid->FindObject(Form("hnumptdepcut_centmid_%s",function->GetName())));
	      hWt2 = static_cast<TH1*>(wtHisto_num_ptmid->FindObject(Form("hnumptdepcut_ptmid_%s",function->GetName())));
	      
	      weight_DataMC = function->Eval(genpt);
	      if(weight_DataMC < 0.0) weight_DataMC = 0.0;
	      weight = weight*weight_DataMC;
	      hWt1->Fill(Centrality/2,weight);
	      hWt2->Fill(recopt,weight);
	    }
	}
	
	if(fabs(tlvrecqq->Rapidity())>1.6){
	  while ( (function = static_cast<TF1*>(nextFunc3Fwd())) )
	    {
	      fnName = function->GetName();
	      weight = ispbpb ? fChain->GetWeight()*findNcoll(Centrality) : 1.;
	      
	      hWt5 = static_cast<TH1*>(wtHisto_num_centfwd->FindObject(Form("hnumptdepcut_centfwd_%s",function->GetName())));
	      hWt6 = static_cast<TH1*>(wtHisto_num_ptfwd->FindObject(Form("hnumptdepcut_ptfwd_%s",function->GetName())));
	      
	      weight_DataMC = function->Eval(genpt);
	      if(weight_DataMC < 0.0) weight_DataMC = 0.0;
	      weight = weight*weight_DataMC;
	      hWt5->Fill(Centrality/2,weight);
	      hWt6->Fill(recopt,weight);
	    }
	}
      }
      
      hnum2d->Fill(fabs(tlvrecqq->Rapidity()),tlvrecqq->Pt(),weight);
      
   } // event loop
   
   if (!(wtHisto_num_ptfwd->GetSize()>0))
     {
       cout << "[ERROR] No array found." << endl;
       return;
     }
   
   TFile *f = new TFile(fname, "RECREATE");
   //f->cd();
   
   wtHisto_num_centmid->Write("wtHisto_num_centmid",TObject::kSingleKey);
   wtHisto_num_ptmid->Write("wtHisto_num_ptmid",TObject::kSingleKey);
   wtHisto_den_centmid->Write("wtHisto_den_centmid",TObject::kSingleKey);
   wtHisto_den_ptmid->Write("wtHisto_den_ptmid",TObject::kSingleKey);
   wtHisto_num_centfwd->Write("wtHisto_num_centfwd",TObject::kSingleKey);
   wtHisto_num_ptfwd->Write("wtHisto_num_ptfwd",TObject::kSingleKey);
   wtHisto_den_centfwd->Write("wtHisto_den_centfwd",TObject::kSingleKey);
   wtHisto_den_ptfwd->Write("wtHisto_den_ptfwd",TObject::kSingleKey);
   
   f->Write();
   f->Close();

   delete wtHisto_num_centmid;
   delete wtHisto_num_ptmid;
   delete wtHisto_den_centmid;
   delete wtHisto_den_ptmid;

   delete wtHisto_num_centfwd;
   delete wtHisto_num_ptfwd;
   delete wtHisto_den_centfwd;
   delete wtHisto_den_ptfwd;

}


TObjArray* readFileWeight(const char* file)
{
  TFile *fileWt = new TFile(file,"READ");
  if (!fileWt)
  {
    cout << "[ERROR] File "<< file << " for weights is not found" << endl;
    return NULL;
  }
  
  TString sName(fileWt->GetName());
  TObjArray* wtFn = NULL;
  
  if (sName.Contains("Mid") ) {
    wtFn = static_cast<TObjArray*>(fileWt->FindObjectAny("wFunctions"));
  }
  
  if (sName.Contains("Fwd") ) {
    wtFn = static_cast<TObjArray*>(fileWt->FindObjectAny("wFunctions"));
  }
  return wtFn;
}







