#ifndef NUMBERSRAASYST_2015
#define NUMBERSRAASYST_2015


// ###########################################################################################
// systematics fits
const int nFitVariations   = 3;
const int nEff4DVariations = 2;
const int nEffTnPVariation = 1; 
//pp
const char* yieldHistFile_pp_systSgnBkg[nFitVariations] = {
  "histsRaaYields_20160215_pp_polFunct_Eff3.root",
  "histsRaaYields_20160215_pp_resOpt2_Eff3.root",
  "histsRaaYields_20160215_pp_signalCB3WN_Eff3.root",
//  "histsRaaYields_20160215_pp_MLAR_Eff3.root", // this is fine, just pbpb doesn't work ..
//  "histsRaaYields_20160215_pp_const_Eff3.root" // this is fine, just pbpb doesn't work ..
};
const char* yieldHistFile_pp_syst4DCorr[nEff4DVariations] = {
  //resOpt2 to be changed for 3D eff uncert. table
  "histsRaaYields_20160215_pp_weightedEff_Prof_Eff3.root","histsRaaYields_20160215_pp_resOpt2_Eff3.root"
};

const char* yieldHistFile_pp_systTnP[nEffTnPVariation] = {
  "histsRaaYields_20160215_pp_polFunct_Eff3.root"
};

//-----------------
// pbpb
const char* yieldHistFile_aa_systSgnBkg[nFitVariations]   = {
  "histsRaaYields_20160215_PbPb_polFunct_Eff3_binv0.root",
  "histsRaaYields_20160215_PbPb_resOpt2_Eff3_binv0.root", 
  "histsRaaYields_20160215_PbPb_signalCB3WN_Eff3_binv0.root",
//  "histsRaaYields_20160215_PbPb_MLAR_Eff3_binv0.root", // need to fix fits on this
//  "histsRaaYields_20160215_PbPb_const_Eff3_binv0.root" // need to fix fits on this
};
const char* yieldHistFile_aa_syst4DCorr[nEff4DVariations] = {
  //resOpt2 to be changed for 3D eff uncert. table
  "histsRaaYields_20160215_PbPb_weightedEff_Prof_Eff3_binv0.root","histsRaaYields_20160215_PbPb_resOpt2_Eff3_binv0.root"
};

const char* yieldHistFile_aa_systTnP[nEffTnPVariation] = {"histsRaaYields_20160215_PbPb_polFunct_Eff3_binv0.root"};


// ######################################
// global uncertainties

  double systEventSelection[2] = {0.04, 0.01}; // HF gating and good event selection ... this should be different in pp and pbpb ...
  double systLumis[2]          = {0.04, 0.04}; // first for pp lumi, 2nd for counting the pbpb MB events

#endif

