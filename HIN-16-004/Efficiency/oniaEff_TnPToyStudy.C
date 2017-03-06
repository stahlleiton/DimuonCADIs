#ifdef oniaEff_cxx
#include "tnp_weight.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TMath.h>
#include <TF1.h>
#include <TObjArray.h>

#include <algorithm>
#include <iostream>
#include <string>

using namespace HI;
using namespace std;


class oniaEff_TnPToyStudy : public oniaEff {
  public:

    oniaEff_TnPToyStudy(TTree *tree=0);
    virtual ~oniaEff_TnPToyStudy();
    virtual const char* GetHistName(TH1F *h, const char *token);
    virtual void LoopVary(const char *fname, bool ispbpb, const int tnptype);

};

oniaEff_TnPToyStudy::oniaEff_TnPToyStudy(TTree *tree) : oniaEff(tree)
{

}

oniaEff_TnPToyStudy::~oniaEff_TnPToyStudy()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}


const char * oniaEff_TnPToyStudy::GetHistName(TH1F* h, const char *_token) {
  string token = _token;
  string histname = h->GetName();
  std::size_t found = histname.rfind(token);
  string histnamebase = histname.erase(found, string::npos);
  return histnamebase.c_str();
}


void oniaEff_TnPToyStudy::LoopVary(const char* fname, bool ispbpb, const int tnptype)
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
   // define the histos : Tobjext array for weighting function
   // Eff vs centrality in 4+1 |y| regions (6.5-50 GeV/c), forward & low pT region
   TObjArray *onum_cent_rap[nbins_4rap+2];
   TObjArray *oden_cent_rap[nbins_4rap+2];
   for (int i=0; i<nbins_4rap+2; i++) {
     onum_cent_rap[i] = new TObjArray();
     oden_cent_rap[i] = new TObjArray();
     onum_cent_rap[i]->SetOwner(kTRUE);
     oden_cent_rap[i]->SetOwner(kTRUE);
   }

   // Eff vs pT in 4+1 |y| regions
   TObjArray *onum_pt_rap[nbins_4rap+1];
   TObjArray *oden_pt_rap[nbins_4rap+1];
   for (int i=0; i<nbins_4rap+1; i++) {
     onum_pt_rap[i] = new TObjArray();
     oden_pt_rap[i] = new TObjArray();
     onum_pt_rap[i]->SetOwner();
     oden_pt_rap[i]->SetOwner();
   }
   
   // Eff vs pT in 3 centrality regions
   TObjArray *onum_pt_cent[nbins_3cent];
   TObjArray *oden_pt_cent[nbins_3cent];
   for (int i=0; i<nbins_3cent; i++) {
     onum_pt_cent[i] = new TObjArray();
     oden_pt_cent[i] = new TObjArray();
     onum_pt_cent[i]->SetOwner();
     oden_pt_cent[i]->SetOwner();
   }

   // Eff vs rap integrated
   TObjArray *onum_rap = new TObjArray();
   TObjArray *oden_rap = new TObjArray();
   onum_rap->SetOwner();
   oden_rap->SetOwner();

   // Create 100 histograms with TObjArray for each TH1F
   TH1F *fnum_cent_rap[nbins_4rap+2];
   TH1F *fden_cent_rap[nbins_4rap+2];
   TH1F *fnum_pt_rap[nbins_4rap+1];
   TH1F *fden_pt_rap[nbins_4rap+1];
   TH1F *fnum_pt_cent[nbins_3cent];
   TH1F *fden_pt_cent[nbins_3cent];
   TH1F *fnum_rap;
   TH1F *fden_rap;

   for (int i=0; i<100; i++) {
     fnum_cent_rap[0] = new TH1F(Form("hnum_cent_rap0024_%d",i),";Centrality;Efficiency",nbins_cent_rap0024,bins_cent_rap0024);
     fden_cent_rap[0] = new TH1F(Form("hden_cent_rap0024_%d",i),";Centrality;Efficiency",nbins_cent_rap0024,bins_cent_rap0024);
     fnum_cent_rap[1] = new TH1F(Form("hnum_cent_rap0006_%d",i),";Centrality;Efficiency",nbins_cent_rap0006,bins_cent_rap0006);
     fden_cent_rap[1] = new TH1F(Form("hden_cent_rap0006_%d",i),";Centrality;Efficiency",nbins_cent_rap0006,bins_cent_rap0006);
     fnum_cent_rap[2] = new TH1F(Form("hnum_cent_rap0612_%d",i),";Centrality;Efficiency",nbins_cent_rap0612,bins_cent_rap0612);
     fden_cent_rap[2] = new TH1F(Form("hden_cent_rap0612_%d",i),";Centrality;Efficiency",nbins_cent_rap0612,bins_cent_rap0612);
     fnum_cent_rap[3] = new TH1F(Form("hnum_cent_rap1218_%d",i),";Centrality;Efficiency",nbins_cent_rap1218,bins_cent_rap1218);
     fden_cent_rap[3] = new TH1F(Form("hden_cent_rap1218_%d",i),";Centrality;Efficiency",nbins_cent_rap1218,bins_cent_rap1218);
     fnum_cent_rap[4] = new TH1F(Form("hnum_cent_rap1824_%d",i),";Centrality;Efficiency",nbins_cent_rap1824,bins_cent_rap1824);
     fden_cent_rap[4] = new TH1F(Form("hden_cent_rap1824_%d",i),";Centrality;Efficiency",nbins_cent_rap1824,bins_cent_rap1824);
     fnum_cent_rap[5] = new TH1F(Form("hnum_cent_rap1824_pt3065_%d",i),";Centrality;Efficiency",nbins_cent_rap1824_pt3065,bins_cent_rap1824_pt3065);
     fden_cent_rap[5] = new TH1F(Form("hden_cent_rap1824_pt3065_%d",i),";Centrality;Efficiency",nbins_cent_rap1824_pt3065,bins_cent_rap1824_pt3065);

     fnum_pt_rap[0] = new TH1F(Form("hnum_pt_rap0024_%d",i),";p_{T} [GeV/c];Efficiency",nbins_pt_rap0024,bins_pt_rap0024);
     fden_pt_rap[0] = new TH1F(Form("hden_pt_rap0024_%d",i),";p_{T} [GeV/c];Efficiency",nbins_pt_rap0024,bins_pt_rap0024);
     fnum_pt_rap[1] = new TH1F(Form("hnum_pt_rap0006_%d",i),";p_{T} [GeV/c];Efficiency",nbins_pt_rap0006,bins_pt_rap0006);
     fden_pt_rap[1] = new TH1F(Form("hden_pt_rap0006_%d",i),";p_{T} [GeV/c];Efficiency",nbins_pt_rap0006,bins_pt_rap0006);
     fnum_pt_rap[2] = new TH1F(Form("hnum_pt_rap0612_%d",i),";p_{T} [GeV/c];Efficiency",nbins_pt_rap0612,bins_pt_rap0612);
     fden_pt_rap[2] = new TH1F(Form("hden_pt_rap0612_%d",i),";p_{T} [GeV/c];Efficiency",nbins_pt_rap0612,bins_pt_rap0612);
     fnum_pt_rap[3] = new TH1F(Form("hnum_pt_rap1218_%d",i),";p_{T} [GeV/c];Efficiency",nbins_pt_rap1218,bins_pt_rap1218);
     fden_pt_rap[3] = new TH1F(Form("hden_pt_rap1218_%d",i),";p_{T} [GeV/c];Efficiency",nbins_pt_rap1218,bins_pt_rap1218);
     fnum_pt_rap[4] = new TH1F(Form("hnum_pt_rap1824_%d",i),";p_{T} [GeV/c];Efficiency",nbins_pt_rap1824,bins_pt_rap1824);
     fden_pt_rap[4] = new TH1F(Form("hden_pt_rap1824_%d",i),";p_{T} [GeV/c];Efficiency",nbins_pt_rap1824,bins_pt_rap1824);
     fnum_pt_cent[0] = new TH1F(Form("hnum_pt_cent0010_%d",i),";p_{T} [GeV/c];Efficiency",nbins_pt_cent010,bins_pt_cent010);
     fden_pt_cent[0] = new TH1F(Form("hden_pt_cent0010_%d",i),";p_{T} [GeV/c];Efficiency",nbins_pt_cent010,bins_pt_cent010);
     fnum_pt_cent[1] = new TH1F(Form("hnum_pt_cent1030_%d",i),";p_{T} [GeV/c];Efficiency",nbins_pt_cent1030,bins_pt_cent1030);
     fden_pt_cent[1] = new TH1F(Form("hden_pt_cent1030_%d",i),";p_{T} [GeV/c];Efficiency",nbins_pt_cent1030,bins_pt_cent1030);
     fnum_pt_cent[2] = new TH1F(Form("hnum_pt_cent30100_%d",i),";p_{T} [GeV/c];Efficiency",nbins_pt_cent30100,bins_pt_cent30100);
     fden_pt_cent[2] = new TH1F(Form("hden_pt_cent30100_%d",i),";p_{T} [GeV/c];Efficiency",nbins_pt_cent30100,bins_pt_cent30100);
     fnum_rap = new TH1F(Form("hnum_rap_%d",i),";|y|;Efficiency",nbins_rap,bins_rap);
     fden_rap = new TH1F(Form("hden_rap_%d",i),";|y|;Efficiency",nbins_rap,bins_rap);

     onum_rap->Add(fnum_rap);
     oden_rap->Add(fden_rap);
     
     for (int j=0; j<nbins_4rap+2; j++) {
       onum_cent_rap[j]->Add(fnum_cent_rap[j]);
       oden_cent_rap[j]->Add(fden_cent_rap[j]);
     }
     for (int j=0; j<nbins_4rap+1; j++) {
       onum_pt_rap[j]->Add(fnum_pt_rap[j]);
       oden_pt_rap[j]->Add(fden_pt_rap[j]);
     }
     for (int j=0; j<nbins_3cent; j++) {
       onum_pt_cent[j]->Add(fnum_pt_cent[j]);
       oden_pt_cent[j]->Add(fden_pt_cent[j]);
     }
   }


   // Loop over events
   Long64_t nentries = fChain->GetEntries();
   cout << "nentries: " << nentries << endl;

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      if (!ispbpb && ientry>=10000000) {
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
      if (genrap>=1.8 && genrap<2.4 && genpt>=3. && genpt<50.) gen_inbin = true;
      //if (!gen_inbin) continue;
      if (!gen_inbin) isgenok=false;
      
      
      if(isgenok) {
        for (int a=0; a<100; a++) {
          
          double weight = ispbpb ? fChain->GetWeight()*findNcoll(Centrality) : 1.;
          
          // temporary histograms to be added to TObjArray 
          TH1F *den_cent_rap[nbins_4rap+2];
          TH1F *den_pt_rap[nbins_4rap+1];
          TH1F *den_pt_cent[nbins_3cent];

          string hname = Form("%s_%d",GetHistName(fden_rap, "_"),a);
          TH1F *den_rap = (TH1F*)oden_rap->FindObject(hname.c_str());

          for (int j=0; j<nbins_4rap+2; j++) {
            hname = Form("%s_%d",GetHistName(fden_cent_rap[j], "_"),a);
//            cout << "GetHistName: " << hname << "  ";
            den_cent_rap[j] = (TH1F*)oden_cent_rap[j]->FindObject(hname.c_str());
//            cout << den_cent_rap[j] << endl;
          }
          for (int j=0; j<nbins_4rap+1; j++) {
            hname = Form("%s_%d",GetHistName(fden_pt_rap[j], "_"),a);
            den_pt_rap[j] = (TH1F*)oden_pt_rap[j]->FindObject(hname.c_str());
          }
          for (int j=0; j<nbins_3cent; j++) {
            hname = Form("%s_%d",GetHistName(fden_pt_cent[j], "_"),a);
            den_pt_cent[j] = (TH1F*)oden_pt_cent[j]->FindObject(hname.c_str());
          }

          // Eff vs rap integrated
          if (genrap>=0 && genrap<2.4 && genpt>=6.5 && genpt<50)
            den_rap->Fill(genrap,weight);

          // Eff vs cent in 4 |y| regions (6.5-50 GeV/c)
          if (genrap>=bins_4rap[0] && genrap<bins_4rap[nbins_4rap] && genpt>=6.5 && genpt<50)
            den_cent_rap[0]->Fill(Centrality/2.0,weight); //[0] is for 0-100
          
          for (int i=0; i<nbins_4rap; i++) {
            if (genrap>=bins_4rap[i] && genrap<bins_4rap[i+1] && genpt>=6.5 && genpt<50)
              den_cent_rap[i+1]->Fill(Centrality/2.0,weight); //[0] is for 0-100
          }
          
          // Eff vs cent at 1.8-2.4, 3-6.5
          if (genrap>=1.8 && genrap<2.4 && genpt>=3 && genpt<6.5) {
            den_cent_rap[nbins_4rap+1]->Fill(Centrality/2.0,weight); //last histogram is for low pT
          }
          
          // Eff vs pt in 4 |y| regions
          if (genrap>=bins_4rap[0] && genrap<bins_4rap[nbins_4rap] && genpt>=6.5 && genpt<50)
            den_pt_rap[0]->Fill(genpt,weight); //[0] is for 0024
          
          for (int i=0; i<nbins_4rap; i++) {
            if ( genrap>=bins_4rap[i] && genrap<bins_4rap[i+1] &&
               ( (genrap<1.8 && genpt>=6.5) || (genrap>=1.8 && genpt>=3) ) &&
               genpt<50 )
              den_pt_rap[i+1]->Fill(genpt,weight); //[0] is for 0024
          }
          
          // Eff vs pt in 3 cent regions
          for (int i=0; i<nbins_3cent; i++) {
            if (Centrality/2.0>=bins_3cent[i] && Centrality/2.0<bins_3cent[i+1] && genpt>=6.5 && genpt<50) {
              den_pt_cent[i]->Fill(genpt,weight);
            }
          }
        } // end of 100 histograms loop
      } // end of if(isgenok)

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
         double mass0 = massjpsi;
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
      if (recorap>=1.8 && recorap<2.4 && recopt>=3. && recopt<50.) rec_inbin = true;
      if (!rec_inbin) continue;
       
      TLorentzVector *tlvrecpl = (TLorentzVector*) Reco_QQ_mupl_4mom->At(ibestqq);
      TLorentzVector *tlvrecmi = (TLorentzVector*) Reco_QQ_mumi_4mom->At(ibestqq);
      double recMuPlpt = tlvrecpl->Pt();
      double recMuPlEta = tlvrecpl->Eta();
      double recMuMipt = tlvrecmi->Pt();
      double recMuMiEta = tlvrecmi->Eta();
     
      for (int a=0; a<100; a++) {
          
        double weight = ispbpb ? fChain->GetWeight()*findNcoll(Centrality) : 1.;
          
        // temporary histograms to be added to TObjArray 
        TH1F *num_cent_rap[nbins_4rap+2];
        TH1F *num_pt_rap[nbins_4rap+1];
        TH1F *num_pt_cent[nbins_3cent];

        string hname = Form("%s_%d",GetHistName(fnum_rap, "_"),a);
        TH1F *num_rap = (TH1F*)onum_rap->FindObject(hname.c_str());
        
        for (int j=0; j<nbins_4rap+2; j++) {
          hname = Form("%s_%d",GetHistName(fnum_cent_rap[j], "_"),a);
//          cout << "GetHistName: " << hname << "  ";
          num_cent_rap[j] = (TH1F*)onum_cent_rap[j]->FindObject(hname.c_str());
//          cout << num_cent_rap[j] << endl;
        }
        for (int j=0; j<nbins_4rap+1; j++) {
          hname = Form("%s_%d",GetHistName(fnum_pt_rap[j], "_"),a);
          num_pt_rap[j] = (TH1F*)onum_pt_rap[j]->FindObject(hname.c_str());
        }
        for (int j=0; j<nbins_3cent; j++) {
          hname = Form("%s_%d",GetHistName(fnum_pt_cent[j], "_"),a);
          num_pt_cent[j] = (TH1F*)onum_pt_cent[j]->FindObject(hname.c_str());
        }

        if (ispbpb) { 
          if (tnptype == tnpTypes::trg_toy) { // toy variations, stat. only
            tnp_weight = tnp_weight_trg_pbpb(recMuPlpt, recMuPlEta, a+1) * tnp_weight_trg_pbpb(recMuMipt, recMuMiEta, a+1);
          }
        } else { //pp
          if (tnptype == tnpTypes::trg_toy) { // toy variations, stat. only
            tnp_weight = tnp_weight_trg_pp(recMuPlpt, recMuPlEta, a+1) * tnp_weight_trg_pp(recMuMipt, recMuMiEta, a+1);
          }
        }
        
        if (tnptype != tnpTypes::noTnPSFs) {
          // tnp scale factors applied only when it's requested
          weight = weight*tnp_weight;
        }
        
        // fill the numerators
        // Eff vs rap integrated
        if (recorap>=0 && recorap<2.4 && recopt>=6.5 && recopt<50)
          num_rap->Fill(genrap,weight);

        // Eff vs cent in 4 |y| regions (6.5-50 GeV/c)
        if (recorap>=bins_4rap[0] && recorap<bins_4rap[nbins_4rap] && recopt>=6.5 && recopt<50)
          num_cent_rap[0]->Fill(Centrality/2.0,weight); //[0] is for 0-100
        
        for (int i=0; i<nbins_4rap; i++) {
          if (recorap>=bins_4rap[i] && recorap<bins_4rap[i+1] && recopt>=6.5 && recopt<50)
            num_cent_rap[i+1]->Fill(Centrality/2.0,weight); //[0] is for 0-100
        }
        // Eff vs cent at 1.8-2.4, 3-6.5
        if (recorap>=1.8 && recorap<2.4 && recopt>=3 && recopt<6.5)
            num_cent_rap[nbins_4rap+1]->Fill(Centrality/2.0,weight); //last histogram is for low pT
        
        // Eff vs pt in 4 |y| regions
        if (recorap>=bins_4rap[0] && recorap<bins_4rap[nbins_4rap] && recopt>=6.5 && recopt<50)
          num_pt_rap[0]->Fill(genpt,weight); //[0] is for 0024
        
        for (int i=0; i<nbins_4rap; i++) {
          if ( recorap>=bins_4rap[i] && recorap<bins_4rap[i+1] &&
             ( (recorap<1.8 && recopt>=6.5) || (recorap>=1.8 && recopt>=3) ) &&
             recopt<50 )
            num_pt_rap[i+1]->Fill(genpt,weight); //[0] is for 0024
        }
        
        // Eff vs pt in 3 cent regions
        for (int i=0; i<nbins_3cent; i++) {
          if (Centrality/2.0>=bins_3cent[i] && Centrality/2.0<bins_3cent[i+1] && recopt>=6.5 && recopt<50)
            num_pt_cent[i]->Fill(genpt,weight);
        }

     } // end of 100 histograms loop
   } // event loop
  

   // Write TObjArrays
   onum_cent_rap[0]->Write("hnum_cent_rap0024",TObject::kSingleKey);
   oden_cent_rap[0]->Write("hden_cent_rap0024",TObject::kSingleKey);
   onum_cent_rap[1]->Write("hnum_cent_rap0006",TObject::kSingleKey);
   oden_cent_rap[1]->Write("hden_cent_rap0006",TObject::kSingleKey);
   onum_cent_rap[2]->Write("hnum_cent_rap0612",TObject::kSingleKey);
   oden_cent_rap[2]->Write("hden_cent_rap0612",TObject::kSingleKey);
   onum_cent_rap[3]->Write("hnum_cent_rap1218",TObject::kSingleKey);
   oden_cent_rap[3]->Write("hden_cent_rap1218",TObject::kSingleKey);
   onum_cent_rap[4]->Write("hnum_cent_rap1824",TObject::kSingleKey);
   oden_cent_rap[4]->Write("hden_cent_rap1824",TObject::kSingleKey);
   onum_cent_rap[5]->Write("hnum_cent_rap1824_pt3065",TObject::kSingleKey);
   oden_cent_rap[5]->Write("hden_cent_rap1824_pt3065",TObject::kSingleKey);
   onum_pt_rap[0]->Write("hnum_pt_rap0024",TObject::kSingleKey);
   oden_pt_rap[0]->Write("hden_pt_rap0024",TObject::kSingleKey);
   onum_pt_rap[1]->Write("hnum_pt_rap0006",TObject::kSingleKey);
   oden_pt_rap[1]->Write("hden_pt_rap0006",TObject::kSingleKey);
   onum_pt_rap[2]->Write("hnum_pt_rap0612",TObject::kSingleKey);
   oden_pt_rap[2]->Write("hden_pt_rap0612",TObject::kSingleKey);
   onum_pt_rap[3]->Write("hnum_pt_rap1218",TObject::kSingleKey);
   oden_pt_rap[3]->Write("hden_pt_rap1218",TObject::kSingleKey);
   onum_pt_rap[4]->Write("hnum_pt_rap1824",TObject::kSingleKey);
   oden_pt_rap[4]->Write("hden_pt_rap1824",TObject::kSingleKey);
   onum_pt_cent[0]->Write("hnum_pt_cent0010",TObject::kSingleKey);
   oden_pt_cent[0]->Write("hden_pt_cent0010",TObject::kSingleKey);
   onum_pt_cent[1]->Write("hnum_pt_cent1030",TObject::kSingleKey);
   oden_pt_cent[1]->Write("hden_pt_cent1030",TObject::kSingleKey);
   onum_pt_cent[2]->Write("hnum_pt_cent30100",TObject::kSingleKey);
   oden_pt_cent[2]->Write("hden_pt_cent30100",TObject::kSingleKey);
   onum_rap->Write("hnum_rap",TObject::kSingleKey);
   oden_rap->Write("hden_rap",TObject::kSingleKey);

//   f->Write();
   f->Close();

}
#endif // #ifdef oniaEff_cxx
