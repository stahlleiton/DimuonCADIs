#define oniaEff_data_cxx
#include "oniaEff_data.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TMath.h>
#include <TF1.h>
#include <TRandom3.h>

#include <iostream>

// define bins
const int nbins_centmid = 6;
const double bins_centmid[nbins_centmid+1] = {0, 10, 20, 30, 40, 50, 100};
const int nbins_centfwd = 3;
const double bins_centfwd[nbins_centfwd+1] = {0, 20, 40, 100};
const int nbins_ptmid = 5;
const double bins_ptmid[nbins_ptmid+1] = {6.5, 9, 12, 15, 20, 30};
const int nbins_ptfwd = 3;
const double bins_ptfwd[nbins_ptfwd+1] = {3, 6.5, 12, 30};

// pt-dependent ctau cuts
const double ctaucut_a_mid_pp = 0.012;//0.010;
const double ctaucut_a_fwd_pp = 0.014;//0.013;
const double ctaucut_a_mid_pbpb = 0.012;//0.013;
const double ctaucut_a_fwd_pbpb = 0.014;//0.015;
const double ctaucut_b_mid_pp = 0.23;//0.25;
const double ctaucut_b_fwd_pp = 0.28;//0.29;
const double ctaucut_b_mid_pbpb = 0.23;//0.22;
const double ctaucut_b_fwd_pbpb = 0.28;//0.28;

// other settings
const double massjpsi = 3.096;
const double masspsip = 3.686;
const double massdown = 0.1;
const double massup = 0.05;
const double massbkglow = 3.25;
const double massbkgup = 3.5;

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


void oniaEff_data::Loop(const char* fname_jpsi, const char* fname_psip, const char* fname_bkg, bool ispbpb)
{
//   In a ROOT session, you can do:
//      root> .L oniaEff_data.C
//      root> oniaEff_data t
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
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();
   TFile *fjpsi = new TFile(fname_jpsi, "RECREATE");
   TFile *fpsip = new TFile(fname_psip, "RECREATE");
   TFile *fbkg = new TFile(fname_bkg, "RECREATE");

   // define the histos
   // we need 2 numerators (with and without ctau3d cut), 1 denominator. Let's make them in arrays
   fjpsi->cd();
   TH1F *hnumjpsi_centmid = new TH1F("hnum_centmid","hnum_centmid",nbins_centmid,bins_centmid);
   TH1F *hnumjpsi_centfwd = new TH1F("hnum_centfwd","hnum_centfwd",nbins_centfwd,bins_centfwd);
   TH1F *hnumjpsi_ptmid = new TH1F("hnum_ptmid","hnum_ptmid",nbins_ptmid,bins_ptmid);
   TH1F *hnumjpsi_ptfwd = new TH1F("hnum_ptfwd","hnum_ptfwd",nbins_ptfwd,bins_ptfwd);
   TH2F *hctaujpsi_ptmid = new TH2F("hctau_ptmid","hctau_ptmid",nbins_ptmid,bins_ptmid,100,-0.5,0.5);
   TH2F *hctaujpsi_ptfwd = new TH2F("hctau_ptfwd","hctau_ptfwd",nbins_ptfwd,bins_ptfwd,100,-0.5,0.5);
   TH1F *hnumjpsiptdepcut_centmid = new TH1F("hnumptdepcut_centmid","hnumptdepcut_centmid",nbins_centmid,bins_centmid);
   TH1F *hnumjpsiptdepcut_centfwd = new TH1F("hnumptdepcut_centfwd","hnumptdepcut_centfwd",nbins_centfwd,bins_centfwd);
   TH1F *hnumjpsiptdepcut_ptmid = new TH1F("hnumptdepcut_ptmid","hnumptdepcut_ptmid",nbins_ptmid,bins_ptmid);
   TH1F *hnumjpsiptdepcut_ptfwd = new TH1F("hnumptdepcut_ptfwd","hnumptdepcut_ptfwd",nbins_ptfwd,bins_ptfwd);
   fpsip->cd();
   TH1F *hnumpsip_centmid = new TH1F("hnum_centmid","hnum_centmid",nbins_centmid,bins_centmid);
   TH1F *hnumpsip_centfwd = new TH1F("hnum_centfwd","hnum_centfwd",nbins_centfwd,bins_centfwd);
   TH1F *hnumpsip_ptmid = new TH1F("hnum_ptmid","hnum_ptmid",nbins_ptmid,bins_ptmid);
   TH1F *hnumpsip_ptfwd = new TH1F("hnum_ptfwd","hnum_ptfwd",nbins_ptfwd,bins_ptfwd);
   TH2F *hctaupsip_ptmid = new TH2F("hctau_ptmid","hctau_ptmid",nbins_ptmid,bins_ptmid,100,-0.5,0.5);
   TH2F *hctaupsip_ptfwd = new TH2F("hctau_ptfwd","hctau_ptfwd",nbins_ptfwd,bins_ptfwd,100,-0.5,0.5);
   TH1F *hnumpsipptdepcut_centmid = new TH1F("hnumptdepcut_centmid","hnumptdepcut_centmid",nbins_centmid,bins_centmid);
   TH1F *hnumpsipptdepcut_centfwd = new TH1F("hnumptdepcut_centfwd","hnumptdepcut_centfwd",nbins_centfwd,bins_centfwd);
   TH1F *hnumpsipptdepcut_ptmid = new TH1F("hnumptdepcut_ptmid","hnumptdepcut_ptmid",nbins_ptmid,bins_ptmid);
   TH1F *hnumpsipptdepcut_ptfwd = new TH1F("hnumptdepcut_ptfwd","hnumptdepcut_ptfwd",nbins_ptfwd,bins_ptfwd);
   fbkg->cd();
   TH1F *hnumbkg_centmid = new TH1F("hnum_centmid","hnum_centmid",nbins_centmid,bins_centmid);
   TH1F *hnumbkg_centfwd = new TH1F("hnum_centfwd","hnum_centfwd",nbins_centfwd,bins_centfwd);
   TH1F *hnumbkg_ptmid = new TH1F("hnum_ptmid","hnum_ptmid",nbins_ptmid,bins_ptmid);
   TH1F *hnumbkg_ptfwd = new TH1F("hnum_ptfwd","hnum_ptfwd",nbins_ptfwd,bins_ptfwd);
   TH2F *hctaubkg_ptmid = new TH2F("hctau_ptmid","hctau_ptmid",nbins_ptmid,bins_ptmid,100,-0.5,0.5);
   TH2F *hctaubkg_ptfwd = new TH2F("hctau_ptfwd","hctau_ptfwd",nbins_ptfwd,bins_ptfwd,100,-0.5,0.5);
   TH1F *hnumbkgptdepcut_centmid = new TH1F("hnumptdepcut_centmid","hnumptdepcut_centmid",nbins_centmid,bins_centmid);
   TH1F *hnumbkgptdepcut_centfwd = new TH1F("hnumptdepcut_centfwd","hnumptdepcut_centfwd",nbins_centfwd,bins_centfwd);
   TH1F *hnumbkgptdepcut_ptmid = new TH1F("hnumptdepcut_ptmid","hnumptdepcut_ptmid",nbins_ptmid,bins_ptmid);
   TH1F *hnumbkgptdepcut_ptfwd = new TH1F("hnumptdepcut_ptfwd","hnumptdepcut_ptfwd",nbins_ptfwd,bins_ptfwd);

   // also store more finely binned histos
   TH1F *hcentfine = new TH1F("hcentfine","hcentfine",200,0,200);
   TH2F *hnum2d = new TH2F("hnum2d","hnum2d",48,0,2.4,150,0,30);

   //nentries = 200;
   Long64_t nbytes = 0, nb = 0;
   gRandom = new TRandom3();
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;

      // is there at least one reco dimuon?
      b_Reco_QQ_size->GetEntry(ientry);
      if (Reco_QQ_size==0) continue;

      // ok, now read all other branches
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;

      b_Centrality->GetEntry(ientry); //read only this branch
      hcentfine->Fill(Centrality);

      for (int i=0; i<Reco_QQ_size; i++) {
         // acceptance
         if (!areMuonsInAcceptance2015(i)) continue;
         // sign
         if (Reco_QQ_sign[i]!=0) continue;
         // quality cuts
         if (!passQualityCuts2015(i)) continue;
         // trigger matching
         if (!isTriggerMatch(i,0)) continue; // HLT_HIL1DoubleMu0_v1
         // ljpsi
         if (Reco_QQ_ctau3D[i]>0) continue; // keep only negative ljpsi
         unsigned int sgn = -1+(2*(gRandom->Integer(2)));
         double ctau3D = ((int) sgn)*Reco_QQ_ctau3D[i]; // make the distribution symmetric
         // mass cut
         double mass = ((TLorentzVector*) Reco_QQ_4mom->At(i))->M();
         bool isjpsi=false, ispsip=false, isbkg=false;
         if (mass>massjpsi-massdown && mass<massjpsi+massup) isjpsi=true;
         if (mass>masspsip-massdown && mass<masspsip+massup) ispsip=true;
         if (mass>massbkglow && mass<massbkgup) isbkg=true;
         if (!isjpsi && !ispsip && !isbkg) continue;

         // check that the dimuon is inside the analysis bins
         bool rec_inbin = false;
         TLorentzVector *tlvrecqq = (TLorentzVector*) Reco_QQ_4mom->At(i);
         if (fabs(tlvrecqq->Rapidity())<1.6 && tlvrecqq->Pt()>6.5 && tlvrecqq->Pt()<30.) rec_inbin = true;
         if (fabs(tlvrecqq->Rapidity())>1.6 && fabs(tlvrecqq->Rapidity())<2.4 && tlvrecqq->Pt()>3. && tlvrecqq->Pt()<30.) rec_inbin = true;
         if (!rec_inbin) continue;

         double recopt = tlvrecqq->Pt();

         // fill the numerators
         if (fabs(tlvrecqq->Rapidity()) < 1.6) {
            TH1F *hnum_centmid = isjpsi ? hnumjpsi_centmid : (ispsip ? hnumpsip_centmid : hnumbkg_centmid);
            TH1F *hnum_ptmid = isjpsi ? hnumjpsi_ptmid : (ispsip ? hnumpsip_ptmid : hnumbkg_ptmid);
            TH2F *hctau_ptmid = isjpsi ? hctaujpsi_ptmid : (ispsip ? hctaupsip_ptmid : hctaubkg_ptmid);
            hnum_centmid->Fill(Centrality/2);
            hnum_ptmid->Fill(recopt);
            hctau_ptmid->Fill(recopt,ctau3D);
         } else {
            TH1F *hnum_centfwd = isjpsi ? hnumjpsi_centfwd : (ispsip ? hnumpsip_centfwd : hnumbkg_centfwd);
            TH1F *hnum_ptfwd = isjpsi ? hnumjpsi_ptfwd : (ispsip ? hnumpsip_ptfwd : hnumbkg_ptfwd);
            TH2F *hctau_ptfwd = isjpsi ? hctaujpsi_ptfwd : (ispsip ? hctaupsip_ptfwd : hctaubkg_ptfwd);
            hnum_centfwd->Fill(Centrality/2);
            hnum_ptfwd->Fill(recopt);
            hctau_ptfwd->Fill(recopt,ctau3D);
         }

         bool ctauptdepcutok = ctaucut(ctau3D, tlvrecqq->Rapidity(), tlvrecqq->Pt(), ispbpb);

         if (ctauptdepcutok) {
            if (fabs(tlvrecqq->Rapidity()) < 1.6) {
               TH1F *hnumptdepcut_centmid = isjpsi ? hnumjpsiptdepcut_centmid : (ispsip ? hnumpsipptdepcut_centmid : hnumbkgptdepcut_centmid);
               TH1F *hnumptdepcut_ptmid = isjpsi ? hnumjpsiptdepcut_ptmid : (ispsip ? hnumpsipptdepcut_ptmid : hnumbkgptdepcut_ptmid);
               hnumptdepcut_centmid->Fill(Centrality/2);
               hnumptdepcut_ptmid->Fill(recopt);
            } else {
               TH1F *hnumptdepcut_centfwd = isjpsi ? hnumjpsiptdepcut_centfwd : (ispsip ? hnumpsipptdepcut_centfwd : hnumbkgptdepcut_centfwd);
               TH1F *hnumptdepcut_ptfwd = isjpsi ? hnumjpsiptdepcut_ptfwd : (ispsip ? hnumpsipptdepcut_ptfwd : hnumbkgptdepcut_ptfwd);
               hnumptdepcut_centfwd->Fill(Centrality/2);
               hnumptdepcut_ptfwd->Fill(recopt);
            }
         }

         hnum2d->Fill(fabs(tlvrecqq->Rapidity()),tlvrecqq->Pt());

      } // Reco_QQ loop


   } // event loop

   fjpsi->Write(); fjpsi->Close();
   fpsip->Write(); fpsip->Close();
   fbkg->Write(); fbkg->Close();

}
