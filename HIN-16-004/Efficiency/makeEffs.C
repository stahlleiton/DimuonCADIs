#include "oniaEff.C"
#include "oniaEff_TnPToyStudy.C"
#include "oniaEff_pTShapeVary.C"
#include <TChain.h>
#include <TFile.h>
#include <TSystem.h>
#include <TTree.h>
#include <TDirectory.h>

#include <iostream>

using namespace std;

void makeEffs() {
   // PP
   TChain *tch_jpsi_pp = new TChain("hionia/myTree");
   // acceptance sample
   tch_jpsi_pp->Add("root://xrootd.unl.edu//store/group/phys_heavyions/dileptons/MC2015/pp502TeV/TTrees/OniaTree_JpsiMM_5p02TeV_TuneCUETP8M1_nofilter_pp502Fall15-MCRUN2_71_V1-v1_GENONLY.root");
   // efficiency sample
   tch_jpsi_pp->Add("root://xrootd.unl.edu//store/group/phys_heavyions/dileptons/MC2015/pp502TeV/TTrees/OniaTree_JpsiMM_5p02TeV_TuneCUETP8M1_HINppWinter16DR-75X_mcRun2_asymptotic_ppAt5TeV_v3-v1_Extended.root");
   
   TChain *tch_npjpsi_pp = new TChain("hionia/myTree");
   // acceptance sample
   tch_npjpsi_pp->Add("root://xrootd.unl.edu//store/group/phys_heavyions/dileptons/MC2015/pp502TeV/TTrees/OniaTree_BJpsiMM_5p02TeV_TuneCUETP8M1_nofilter_pp502Fall15-MCRUN2_71_V1-v1_GENONLY.root");
   // efficiency sample
   tch_npjpsi_pp->Add("root://xrootd.unl.edu//store/group/phys_heavyions/dileptons/MC2015/pp502TeV/TTrees/OniaTree_BJpsiMM_5p02TeV_TuneCUETP8M1_HINppWinter16DR-75X_mcRun2_asymptotic_ppAt5TeV_v3-v1_Extended.root");

   // PbPb
   TChain *tch_jpsi_pbpb = new TChain("hionia/myTree");
   // tch_jpsi_pbpb->Add("root://xrootd.unl.edu//store/group/phys_heavyions/dileptons/MC2015/PbPb502TeV/TTrees/OniaTree_Pythia8_JpsiMM_ptJpsi_00_03_Hydjet_MB_HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v13-v1.root");
   tch_jpsi_pbpb->Add("root://xrootd.unl.edu//store/group/phys_heavyions/dileptons/MC2015/PbPb502TeV/TTrees/OniaTree_Pythia8_JpsiMM_ptJpsi_03_06_Hydjet_MB_HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v13-v1.root");
   tch_jpsi_pbpb->Add("root://xrootd.unl.edu//store/group/phys_heavyions/dileptons/MC2015/PbPb502TeV/TTrees/OniaTree_Pythia8_JpsiMM_ptJpsi_06_09_Hydjet_MB_HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v13-v1.root");
   tch_jpsi_pbpb->Add("root://xrootd.unl.edu//store/group/phys_heavyions/dileptons/MC2015/PbPb502TeV/TTrees/OniaTree_Pythia8_JpsiMM_ptJpsi_09_12_Hydjet_MB_HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v13-v1.root");
   tch_jpsi_pbpb->Add("root://xrootd.unl.edu//store/group/phys_heavyions/dileptons/MC2015/PbPb502TeV/TTrees/OniaTree_Pythia8_JpsiMM_ptJpsi_12_15_Hydjet_MB_HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v13-v1.root");
   tch_jpsi_pbpb->Add("root://xrootd.unl.edu//store/group/phys_heavyions/dileptons/MC2015/PbPb502TeV/TTrees/OniaTree_Pythia8_JpsiMM_ptJpsi_15_30_Hydjet_MB_HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v13-v1.root");
   tch_jpsi_pbpb->Add("root://xrootd.unl.edu//store/group/phys_heavyions/dileptons/MC2015/PbPb502TeV/TTrees/OniaTree_Pythia8_JpsiMM_ptJpsi_30_Inf_Hydjet_MB_HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v13-v1.root");

   TChain *tch_npjpsi_pbpb = new TChain("hionia/myTree");
//   tch_npjpsi_pbpb->Add("root://xrootd.unl.edu//store/group/phys_heavyions/dileptons/MC2015/PbPb502TeV/TTrees/OniaTree_Pythia8_BJpsiMM_ptJpsi_00_03_Hydjet_MB_HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v13-v1.root");
   tch_npjpsi_pbpb->Add("root://xrootd.unl.edu//store/group/phys_heavyions/dileptons/MC2015/PbPb502TeV/TTrees/OniaTree_Pythia8_BJpsiMM_ptJpsi_03_06_Hydjet_MB_HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v13-v1.root");
   tch_npjpsi_pbpb->Add("root://xrootd.unl.edu//store/group/phys_heavyions/dileptons/MC2015/PbPb502TeV/TTrees/OniaTree_Pythia8_BJpsiMM_ptJpsi_06_09_Hydjet_MB_HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v13-v1.root");
   tch_npjpsi_pbpb->Add("root://xrootd.unl.edu//store/group/phys_heavyions/dileptons/MC2015/PbPb502TeV/TTrees/OniaTree_Pythia8_BJpsiMM_ptJpsi_09_12_Hydjet_MB_HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v13-v1.root");
   tch_npjpsi_pbpb->Add("root://xrootd.unl.edu//store/group/phys_heavyions/dileptons/MC2015/PbPb502TeV/TTrees/OniaTree_Pythia8_BJpsiMM_ptJpsi_12_15_Hydjet_MB_HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v13-v1.root");
   tch_npjpsi_pbpb->Add("root://xrootd.unl.edu//store/group/phys_heavyions/dileptons/MC2015/PbPb502TeV/TTrees/OniaTree_Pythia8_BJpsiMM_ptJpsi_15_30_Hydjet_MB_HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v13-v1.root");
   tch_npjpsi_pbpb->Add("root://xrootd.unl.edu//store/group/phys_heavyions/dileptons/MC2015/PbPb502TeV/TTrees/OniaTree_Pythia8_BJpsiMM_ptJpsi_30_Inf_Hydjet_MB_HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v13-v1.root");

   string dir = "files"; // output files will be stored under this directory
   gSystem->mkdir(Form("./%s",dir.c_str()), kTRUE);

   // make the acceptance histos
//   cout << "Acceptance for pp prompt Jpsi" << endl;
//   oniaEff obj_jpsi_pp(tch_jpsi_pp);
//   obj_jpsi_pp.Loop(Form("%s/histos_jpsi_pp.root",dir.c_str()),false,obj_jpsi_pp.tnpTypes::trg,true);
//
//   cout << "Acceptance for pp non-prompt Jpsi" << endl;
//   oniaEff obj_npjpsi_pp(tch_npjpsi_pp);
//   obj_npjpsi_pp.Loop(Form("%s/histos_npjpsi_pp.root",dir.c_str()),false,obj_npjpsi_pp.tnpTypes::trg,true);

   // make the efficiency histos
   cout << "Efficiencies for pp prompt Jpsi" << endl;
   oniaEff obj_jpsi_pp(tch_jpsi_pp);
   obj_jpsi_pp.Loop(Form("%s/histos_jpsi_pp.root",dir.c_str()),false,obj_jpsi_pp.tnpTypes::trg,false);

   cout << "Efficiencies for pp non-prompt Jpsi" << endl;
   oniaEff obj_npjpsi_pp(tch_npjpsi_pp);
   obj_npjpsi_pp.Loop(Form("%s/histos_npjpsi_pp.root",dir.c_str()),false,obj_npjpsi_pp.tnpTypes::trg,false);

   cout << "Efficiencies for pbpb prompt Jpsi" << endl;
   oniaEff obj_jpsi_pbpb(tch_jpsi_pbpb);
   obj_jpsi_pbpb.Loop(Form("%s/histos_jpsi_pbpb.root",dir.c_str()),true,obj_jpsi_pbpb.tnpTypes::trg,false);

   cout << "Efficiencies for pbpb non-prompt Jpsi" << endl;
   oniaEff obj_npjpsi_pbpb(tch_npjpsi_pbpb);
   obj_npjpsi_pbpb.Loop(Form("%s/histos_npjpsi_pbpb.root",dir.c_str()),true,obj_npjpsi_pbpb.tnpTypes::trg,false);

   // Or efficiency systematics can be tested like below
//   oniaEff_pTShapeVary obj_npjpsi_pbpb(tch_npjpsi_pbpb);
//   obj_npjpsi_pbpb.LoopVary(Form("%s/histos_npjpsi_pbpb.root",dir.c_str()),true,obj_npjpsi_pbpb.tnpTypes::trg_ptWeighting);

//   oniaEff_TnPToyStudy obj_npjpsi_pbpb(tch_npjpsi_pbpb);
//   obj_npjpsi_pbpb.LoopVary(Form("%s/histos_npjpsi_pbpb.root",dir.c_str()),true,obj_npjpsi_pbpb.tnpTypes::trg_toy);

}
