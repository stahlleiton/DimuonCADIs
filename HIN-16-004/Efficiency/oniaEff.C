#define oniaEff_cxx
#include "oniaEff.h"
#include "tnp_weight.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TMath.h>
#include <TF1.h>

#include <algorithm>
#include <iostream>

// define bins (Jpsi Raa binning)
const double bins_cent_rap0024[] = {0, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 60, 70, 100};
const int nbins_cent_rap0024 = sizeof(bins_cent_rap0024)/sizeof(double) -1;
const double bins_cent_rap1824_pt3065[] = {0, 10, 20, 30, 40, 50, 100}; //Low pT forward
const int nbins_cent_rap1824_pt3065 = sizeof(bins_cent_rap1824_pt3065)/sizeof(double) -1; //Low pT forward
const double bins_cent_rap0006[] = {0, 10, 20, 30, 40, 50, 100};
const int nbins_cent_rap0006 = sizeof(bins_cent_rap0006)/sizeof(double) -1;
const double bins_cent_rap0612[] = {0, 10, 20, 30, 40, 50, 100};
const int nbins_cent_rap0612 = sizeof(bins_cent_rap0612)/sizeof(double) -1;
const double bins_cent_rap1218[] = {0, 10, 20, 30, 40, 50, 100};
const int nbins_cent_rap1218 = sizeof(bins_cent_rap1218)/sizeof(double) -1;
const double bins_cent_rap1824[] = {0, 10, 20, 30, 40, 50, 100};
const int nbins_cent_rap1824 = sizeof(bins_cent_rap1824)/sizeof(double) -1;
const double bins_pt_rap0024[] = {6.5, 7.5, 8.5, 9.5, 11, 13, 15, 20, 30, 50};
const int nbins_pt_rap0024 = sizeof(bins_pt_rap0024)/sizeof(double) -1;
const double bins_pt_rap0006[] = {6.5, 8.5, 9.5, 11, 15, 50};
const int nbins_pt_rap0006 = sizeof(bins_pt_rap0006)/sizeof(double) -1;
const double bins_pt_rap0612[] = {6.5, 8.5, 9.5, 11, 15, 50};
const int nbins_pt_rap0612 = sizeof(bins_pt_rap0612)/sizeof(double) -1;
const double bins_pt_rap1218[] = {6.5, 7.5, 8.5, 9.5, 11, 15, 50};
const int nbins_pt_rap1218 = sizeof(bins_pt_rap1218)/sizeof(double) -1;
const double bins_pt_rap1824[] = {2, 3, 4.5, 5.5, 6.5, 7.5, 8.5, 9.5, 11, 15, 50};
const int nbins_pt_rap1824 = sizeof(bins_pt_rap1824)/sizeof(double) -1;
const double bins_pt_cent010[] = {6.5, 7.5, 8.5, 9.5, 11, 13, 15, 20, 50};
const int nbins_pt_cent010 = sizeof(bins_pt_cent010)/sizeof(double) -1;
const double bins_pt_cent1030[] = {6.5, 7.5, 8.5, 9.5, 11, 13, 15, 20, 50};
const int nbins_pt_cent1030 = sizeof(bins_pt_cent1030)/sizeof(double) -1;
const double bins_pt_cent30100[] = {6.5, 7.5, 8.5, 9.5, 11, 13, 15, 20, 50};
const int nbins_pt_cent30100 = sizeof(bins_pt_cent30100)/sizeof(double) -1;
const double bins_rap[] = {0, 0.4, 0.8, 1.2, 1.6, 2, 2.4};
const int nbins_rap = sizeof(bins_rap)/sizeof(double) -1;

// Bin boundaries for efficiency histograms
const double bins_4rap[] = {0, 0.6, 1.2, 1.8, 2.4};
const int nbins_4rap = sizeof(bins_4rap)/sizeof(double) -1;
const double bins_3cent[] = {0, 10, 30, 100};
const int nbins_3cent = sizeof(bins_3cent)/sizeof(double) -1;

// other settings
const double maxdr = 0.03;
const double massjpsi = 3.096;
const double masspsip = 3.686;
const double massdown = 0.25;
const double massup = 0.15;

using namespace HI;
using namespace std;

void oniaEff::Loop(const char* fname, bool ispbpb, bool isPsip)
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

   TFile *f = new TFile(fname, "RECREATE");
   f->cd();

   TH1::SetDefaultSumw2();
   // define the histos
   // Eff vs centrality in 4+1 |y| regions (6.5-50 GeV/c), forward & low pT region
   TH1F *hnum_cent_rap[nbins_4rap+2];
   TH1F *hden_cent_rap[nbins_4rap+2];
   hnum_cent_rap[0] = new TH1F("hnum_cent_rap0024",";Centrality;Efficiency",nbins_cent_rap0024,bins_cent_rap0024);
   hden_cent_rap[0] = new TH1F("hden_cent_rap0024",";Centrality;Efficiency",nbins_cent_rap0024,bins_cent_rap0024);
   hnum_cent_rap[1] = new TH1F("hnum_cent_rap0006",";Centrality;Efficiency",nbins_cent_rap0006,bins_cent_rap0006);
   hden_cent_rap[1] = new TH1F("hden_cent_rap0006",";Centrality;Efficiency",nbins_cent_rap0006,bins_cent_rap0006);
   hnum_cent_rap[2] = new TH1F("hnum_cent_rap0612",";Centrality;Efficiency",nbins_cent_rap0612,bins_cent_rap0612);
   hden_cent_rap[2] = new TH1F("hden_cent_rap0612",";Centrality;Efficiency",nbins_cent_rap0612,bins_cent_rap0612);
   hnum_cent_rap[3] = new TH1F("hnum_cent_rap1218",";Centrality;Efficiency",nbins_cent_rap1218,bins_cent_rap1218);
   hden_cent_rap[3] = new TH1F("hden_cent_rap1218",";Centrality;Efficiency",nbins_cent_rap1218,bins_cent_rap1218);
   hnum_cent_rap[4] = new TH1F("hnum_cent_rap1824",";Centrality;Efficiency",nbins_cent_rap1824,bins_cent_rap1824);
   hden_cent_rap[4] = new TH1F("hden_cent_rap1824",";Centrality;Efficiency",nbins_cent_rap1824,bins_cent_rap1824);
   hnum_cent_rap[5] = new TH1F("hnum_cent_rap1824_pt3065",";Centrality;Efficiency",nbins_cent_rap1824_pt3065,bins_cent_rap1824_pt3065);
   hden_cent_rap[5] = new TH1F("hden_cent_rap1824_pt3065",";Centrality;Efficiency",nbins_cent_rap1824_pt3065,bins_cent_rap1824_pt3065);
   // Eff vs pT in 4+1 |y| regions
   TH1F *hnum_pt_rap[nbins_4rap+1];
   TH1F *hden_pt_rap[nbins_4rap+1];
   hnum_pt_rap[0] = new TH1F("hnum_pt_rap0024",";p_{T} [GeV/c];Efficiency",nbins_pt_rap0024,bins_pt_rap0024);
   hden_pt_rap[0] = new TH1F("hden_pt_rap0024",";p_{T} [GeV/c];Efficiency",nbins_pt_rap0024,bins_pt_rap0024);
   hnum_pt_rap[1] = new TH1F("hnum_pt_rap0006",";p_{T} [GeV/c];Efficiency",nbins_pt_rap0006,bins_pt_rap0006);
   hden_pt_rap[1] = new TH1F("hden_pt_rap0006",";p_{T} [GeV/c];Efficiency",nbins_pt_rap0006,bins_pt_rap0006);
   hnum_pt_rap[2] = new TH1F("hnum_pt_rap0612",";p_{T} [GeV/c];Efficiency",nbins_pt_rap0612,bins_pt_rap0612);
   hden_pt_rap[2] = new TH1F("hden_pt_rap0612",";p_{T} [GeV/c];Efficiency",nbins_pt_rap0612,bins_pt_rap0612);
   hnum_pt_rap[3] = new TH1F("hnum_pt_rap1218",";p_{T} [GeV/c];Efficiency",nbins_pt_rap1218,bins_pt_rap1218);
   hden_pt_rap[3] = new TH1F("hden_pt_rap1218",";p_{T} [GeV/c];Efficiency",nbins_pt_rap1218,bins_pt_rap1218);
   hnum_pt_rap[4] = new TH1F("hnum_pt_rap1824",";p_{T} [GeV/c];Efficiency",nbins_pt_rap1824,bins_pt_rap1824);
   hden_pt_rap[4] = new TH1F("hden_pt_rap1824",";p_{T} [GeV/c];Efficiency",nbins_pt_rap1824,bins_pt_rap1824);
   // Eff vs pT in 3 centrality regions
   TH1F *hnum_pt_cent[nbins_3cent];
   TH1F *hden_pt_cent[nbins_3cent];
   hnum_pt_cent[0] = new TH1F("hnum_pt_cent010",";p_{T} [GeV/c];Efficiency",nbins_pt_cent010,bins_pt_cent010);
   hden_pt_cent[0] = new TH1F("hden_pt_cent010",";p_{T} [GeV/c];Efficiency",nbins_pt_cent010,bins_pt_cent010);
   hnum_pt_cent[1] = new TH1F("hnum_pt_cent1030",";p_{T} [GeV/c];Efficiency",nbins_pt_cent1030,bins_pt_cent1030);
   hden_pt_cent[1] = new TH1F("hden_pt_cent1030",";p_{T} [GeV/c];Efficiency",nbins_pt_cent1030,bins_pt_cent1030);
   hnum_pt_cent[2] = new TH1F("hnum_pt_cent30100",";p_{T} [GeV/c];Efficiency",nbins_pt_cent30100,bins_pt_cent30100);
   hden_pt_cent[2] = new TH1F("hden_pt_cent30100",";p_{T} [GeV/c];Efficiency",nbins_pt_cent30100,bins_pt_cent30100);
   // Eff vs rap integrated
   TH1F *hnum_rap = new TH1F("hnum_rap",";|y|;Efficiency",nbins_rap,bins_rap);
   TH1F *hden_rap = new TH1F("hden_rap",";|y|;Efficiency",nbins_rap,bins_rap);
   
   // also store more finely binned histos
   TH1F *hcentfine = new TH1F("hcentfine","hcentfine",200,0,200);
   TH2F *hnum2d = new TH2F("hnum2d","hnum2d",48,0,2.4,150,0,30);
   TH2F *hden2d = new TH2F("hden2d","hden2d",48,0,2.4,150,0,30);


   ULong64_t nentries = fChain->GetEntries();
   cout << "nentries: " << nentries << endl;

   ULong64_t nbytes = 0, nb = 0;
   for (ULong64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      if (ispbpb && ientry>=1000000) {
        continue;
      } else if (!ispbpb && ientry>=10000000) {
        break;
      }
      if (jentry%100000==0)
        cout << "jentry/ientry: " << jentry << " " << ientry << endl;

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
      double genrap = fabs(tlvgenqq->Rapidity());
      b_Centrality->GetEntry(ientry); //read only this branch

      // check that the dimuon is inside the analysis bins
      bool gen_inbin = false;
      if (genrap<1.8 && genpt>=6.5 && genpt<50.) gen_inbin = true;
      if (genrap>=1.8 && genrap<2.4 && genpt>=2. && genpt<50.) gen_inbin = true;
      //if (!gen_inbin) continue;
      if (!gen_inbin) isgenok=false;
      
      double weight = ispbpb ? fChain->GetWeight()*findNcoll(Centrality) : 1.;
//      double weight_DataMC = 0.0;
//      if(fabs(tlvgenqq->Rapidity())<1.6){
//      weight_DataMC = 1.659 - 0.0844*genpt + 0.0019*genpt*genpt;
//      if(ispbpb && !isPsip) weight_DataMC = 1.88 - 0.120*genpt + 0.0033*genpt*genpt; 
//      if(!ispbpb && isPsip) weight_DataMC = 1.059 + 0.0001*genpt - 0.0005*genpt*genpt;
//      if(ispbpb && isPsip) weight_DataMC = 0.254 + 0.109*genpt - 0.0033*genpt*genpt;
//      
//      }
//      if(fabs(tlvgenqq->Rapidity())>1.6){
//      weight_DataMC = 1.155 - 0.0197*genpt;
//      if(ispbpb && !isPsip) weight_DataMC = 1.168 - 0.0223*genpt;
//      if(!ispbpb && isPsip) weight_DataMC = 0.814 + 0.0453*genpt - 0.0021*genpt*genpt;
//      if(ispbpb && isPsip) weight_DataMC = 0.721 + 0.0507*genpt; 
//      
//      }
      
      //weight = weight*weight_DataMC;
//      TF1 *wfunt = new TF1("wfunt","[0] + [1]*x",0.0,30.0);
//      wfunt->SetParameters(1.3, -0.02); 
      //weight = weight*wfunt->Eval(genpt);
      
      hcentfine->Fill(Centrality,weight);
      hden2d->Fill(genrap,genpt,weight);
      
      if(isgenok) {
        // Eff vs rap integrated
        if (genrap>=0 && genrap<2.4)
          hden_rap->Fill(genrap,weight);

        // Eff vs cent in 4 |y| regions (6.5-50 GeV/c)
        if (genrap>=bins_4rap[0] && genrap<bins_4rap[nbins_4rap])
          hden_cent_rap[0]->Fill(Centrality/2.0,weight); //[0] is for 0-100
        
        for (int i=0; i<nbins_4rap; i++) {
          if (genrap>=bins_4rap[i] && genrap<bins_4rap[i+1])
            hden_cent_rap[i+1]->Fill(Centrality/2.0,weight); //[0] is for 0-100
        }
        
        // Eff vs cent at 1.8-2.4, 3-6.5
        if (genrap>=1.8 && genrap<2.4 && genpt>=3 && genpt<6.5) {
          hden_cent_rap[nbins_4rap+1]->Fill(Centrality/2.0,weight); //last histogram is for low pT
        }
        
        // Eff vs pt in 4 |y| regions
        if (genrap>=bins_4rap[0] && genrap<bins_4rap[nbins_4rap])
          hden_pt_rap[0]->Fill(genpt,weight); //[0] is for 0024
        
        for (int i=0; i<nbins_4rap; i++) {
          if (genrap>=bins_4rap[i] && genrap<bins_4rap[i+1])
            hden_pt_rap[i+1]->Fill(genpt,weight); //[0] is for 0024
        }
        
        // Eff vs pt in 3 cent regions
        for (int i=0; i<nbins_3cent; i++) {
          if (Centrality/2.0>=bins_3cent[i] && Centrality/2.0<bins_3cent[i+1])
            hden_pt_cent[i]->Fill(genpt,weight);
        }
      
        ///// start reco loop
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
        
        TLorentzVector *tlvrecqq = (TLorentzVector*) Reco_QQ_4mom->At(ibestqq);
        double recopt = tlvrecqq->Pt();
        double recorap = fabs(tlvrecqq->Rapidity());

        // check that the dimuon is inside the analysis bins
        bool rec_inbin = false;
        if (recorap<1.8 && recopt>=6.5 && recopt<50.) rec_inbin = true;
        if (recorap>=1.8 && recorap<2.4 && recopt>=2. && recopt<50.) rec_inbin = true;
        if (!rec_inbin) continue;
         
        TLorentzVector *tlvrecpl = (TLorentzVector*) Reco_QQ_mupl_4mom->At(ibestqq);
        TLorentzVector *tlvrecmi = (TLorentzVector*) Reco_QQ_mumi_4mom->At(ibestqq);
        double recMuPlpt = tlvrecpl->Pt();
        double recMuPlEta = tlvrecpl->Eta();
        double recMuMipt = tlvrecmi->Pt();
        double recMuMiEta = tlvrecmi->Eta();
        
        tnp_weight = tnp_weight_sta_pbpb(recMuPlpt, recMuPlEta, 0)*tnp_weight_sta_pbpb(recMuMipt, recMuMiEta, 0)*tnp_weight_muidtrg_pbpb(recMuPlpt, recMuPlEta, 0) * tnp_weight_muidtrg_pbpb(recMuMipt, recMuMiEta, 0);
        //weight = weight*tnp_weight;
        
        // fill the numerators
        // Eff vs rap integrated
        if (recorap>=0 && recorap<2.4)
          hnum_rap->Fill(genrap,weight);

        // Eff vs cent in 4 |y| regions (6.5-50 GeV/c)
        if (recorap>=bins_4rap[0] && recorap<bins_4rap[nbins_4rap])
          hnum_cent_rap[0]->Fill(Centrality/2.0,weight); //[0] is for 0-100
        
        for (int i=0; i<nbins_4rap; i++) {
          if (recorap>=bins_4rap[i] && recorap<bins_4rap[i+1])
            hnum_cent_rap[i+1]->Fill(Centrality/2.0,weight); //[0] is for 0-100
        
        }
        // Eff vs cent at 1.8-2.4, 3-6.5
        if (recorap>=1.8 && recorap<2.4 && recopt>=3 && recopt<6.5)
            hnum_cent_rap[nbins_4rap+1]->Fill(Centrality/2.0,weight); //last histogram is for low pT
        
        // Eff vs pt in 4 |y| regions
        if (recorap>=bins_4rap[0] && recorap<bins_4rap[nbins_4rap])
          hnum_pt_rap[0]->Fill(genpt,weight); //[0] is for 0024
        
        for (int i=0; i<nbins_4rap; i++) {
          if (recorap>=bins_4rap[i] && recorap<bins_4rap[i+1])
            hnum_pt_rap[i+1]->Fill(genpt,weight); //[0] is for 0024
        }
        
        // Eff vs pt in 3 cent regions
        for (int i=0; i<nbins_3cent; i++) {
          if (Centrality/2.0>=bins_3cent[i] && Centrality/2.0<bins_3cent[i+1])
            hnum_pt_cent[i]->Fill(genpt,weight);
        }
        
        hnum2d->Fill(recorap,recopt,weight);
      } // end of if(isgenok)
      
   } // event loop
  

   f->Write();
   f->Close();

}
