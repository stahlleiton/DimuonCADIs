#ifndef NUMBERSRAASYST_2015
#define NUMBERSRAASYST_2015


// ###########################################################################################
// systematics fits
const int nFitVariations   = 3;
const int nEff4DVariations = 2;
const int nEffTnPVariation = 1; 
//pp
const char* yieldHistFile_pp_systSgnBkg[nFitVariations] = {
  "histsRaaYieldsSyst_20151217_pp_polFunct.root","histsRaaYieldsSyst_20151217_pp_resOpt2.root", "histsRaaYieldsSyst_20151217_pp_signalCB3WN.root"//,"histsRaaYieldsSyst_20151217_pp_keysPdf.root"
};
const char* yieldHistFile_pp_syst4DCorr[nEff4DVariations] = {
  // "histsRaaYieldsSyst_20151217_pp_TnPAtRD3_4DEff2_RapPtEffMap3_Ratio_Prof.root","histsRaaYieldsSyst_20151217_pp_TnPAtRD3_4DEff2_RapPtEffMap5_Ratio.root"
  "histsRaaYieldsSyst_20151217_pp_polFunct.root","histsRaaYieldsSyst_20151217_pp_resOpt2.root"
};

const char* yieldHistFile_pp_systTnP[nEffTnPVariation] = {"histsRaaYieldsSyst_20151217_pp_polFunct.root"};

//-----------------
// pbpb
const char* yieldHistFile_aa_systSgnBkg[nFitVariations]   = {
  "histsRaaYieldsSyst_20151217_PbPb_polFunct.root","histsRaaYieldsSyst_20151217_PbPb_resOpt2.root","histsRaaYieldsSyst_20151217_PbPb_signalCB3WN.root"//,"histsRaaYieldsSyst_20151217_PbPb_keysPdf.root"
};
const char* yieldHistFile_aa_syst4DCorr[nEff4DVariations] = {
  // "histsRaaYieldsSyst_20151217_PbPb_TnPAtRD3_4DEff2_RapPtEffMap3_Ratio_Prof.root","histsRaaYieldsSyst_20151217_PbPb_TnPAtRD3_4DEff2_RapPtEffMap5_Ratio.root" 
  "histsRaaYieldsSyst_20151217_PbPb_polFunct.root","histsRaaYieldsSyst_20151217_PbPb_resOpt2.root"
};

const char* yieldHistFile_aa_systTnP[nEffTnPVariation] = {"histsRaaYieldsSyst_20151217_PbPb_polFunct.root"};


// ######################################
// global uncertainties

  double systEventSelection[2] = {0.04, 0.01}; // HF gating and good event selection ... this should be different in pp and pbpb ...
  double systLumis[2]          = {0.04, 0.04}; // first for pp lumi, 2nd for counting the pbpb MB events

#endif

