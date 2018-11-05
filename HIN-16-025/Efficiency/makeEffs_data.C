#include "oniaEff_data.C"
#include <TChain.h>
#include <TFile.h>
#include <TSystem.h>
#include <TTree.h>
#include <TDirectory.h>

#include <iostream>

using namespace std;

void makeEffs_data() {
   // PP
   TChain *tch_jpsi_pp = new TChain("hionia/myTree");
   tch_jpsi_pp->Add("root://xrootd.unl.edu//store/group/phys_heavyions/dileptons/Data2015/pp502TeV/TTrees/PromptAOD/OniaTree_DoubleMu_Run2015E-PromptReco-v1_Run_262157_262328.root");

   // PbPb
   TChain *tch_jpsi_pbpb = new TChain("hionia/myTree");
   tch_jpsi_pbpb->Add("root://xrootd.unl.edu//store/group/phys_heavyions/dileptons/Data2015/PbPb502TeV/TTrees/PromptAOD/OniaTree_HIOniaL1DoubleMu0_HIRun2015-PromptReco-v1_Run_262620_263757.root");
   tch_jpsi_pbpb->Add("root://xrootd.unl.edu//store/group/phys_heavyions/dileptons/Data2015/PbPb502TeV/TTrees/PromptAOD/OniaTree_HIOniaL1DoubleMu0B_HIRun2015-PromptReco-v1_Run_263322_263757.root");
   tch_jpsi_pbpb->Add("root://xrootd.unl.edu//store/group/phys_heavyions/dileptons/Data2015/PbPb502TeV/TTrees/PromptAOD/OniaTree_HIOniaL1DoubleMu0C_HIRun2015-PromptReco-v1_Run_263322_263757.root");
   tch_jpsi_pbpb->Add("root://xrootd.unl.edu//store/group/phys_heavyions/dileptons/Data2015/PbPb502TeV/TTrees/PromptAOD/OniaTree_HIOniaL1DoubleMu0D_HIRun2015-PromptReco-v1_Run_263322_263757.root");

   // make the efficiency histos
   cout << "Efficiencies for pp" << endl;
   oniaEff_data obj_jpsi_pp(tch_jpsi_pp);
   obj_jpsi_pp.Loop("files_data/histos_pp_jpsi.root","files_data/histos_pp_psip.root","files_data/histos_pp_bkg.root",false);
   cout << "Efficiencies for pbpb" << endl;
   oniaEff_data obj_jpsi_pbpb(tch_jpsi_pbpb);
   obj_jpsi_pbpb.Loop("files_data/histos_pbpb_jpsi.root","files_data/histos_pbpb_psip.root","files_data/histos_pbpb_bkg.root",true);
}
