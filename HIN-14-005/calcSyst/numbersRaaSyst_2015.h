#ifndef NUMBERSRAASYST_2015
#define NUMBERSRAASYST_2015


// ###########################################################################################
// systematics fits
const int nFitVariations   = 5;
const int nEff4DVariations = 1;
const int nEffTnPVariation = 3; 
//pp
const char* yieldHistFile_pp_systSgnBkg[nFitVariations] = {
  "histsRaaYields_20160304_pp_signalCB3WN.root",
  "histsRaaYields_20160304_pp_polFunct.root",
  "histsRaaYields_20160304_pp_resOpt2.root",
  "histsRaaYields_20160304_pp_MLAR.root", 
  "histsRaaYields_20160304_pp_const.root" 
};
const char* yieldHistFile_pp_syst4DCorr[nEff4DVariations] = {
//  "histsRaaYields_20160304_pp_weighted_prof.root"
  "histsRaaYields_20160304_pp_ctau1mm.root"
};

const char* yieldHistFile_pp_systTnP[nEffTnPVariation] = {
  "histEff_effSyst_201602_pp_3dEff.root",
  "histEff_effSyst_201602_pp_tnp.root",
  "histEff_effSyst_201602_pp_sta.root"
};

//-----------------
// pbpb
const char* yieldHistFile_aa_systSgnBkg[nFitVariations]   = {
  "histsRaaYields_20160304_PbPb_signalCB3WN.root",
  "histsRaaYields_20160304_PbPb_polFunct.root",
  "histsRaaYields_20160304_PbPb_resOpt2.root", 
  "histsRaaYields_20160304_PbPb_MLAR.root", 
  "histsRaaYields_20160304_PbPb_const.root"
};
const char* yieldHistFile_aa_syst4DCorr[nEff4DVariations] = {
//  "histsRaaYields_20160304_PbPb_weighted_prof.root"
  "histsRaaYields_20160304_PbPb_ctau1mm.root"
};

const char* yieldHistFile_aa_systTnP[nEffTnPVariation] = {
  "histEff_effSyst_201602_pbpb_3dEff.root",
  "histEff_effSyst_201602_pbpb_tnp.root",
  "histEff_effSyst_201602_pbpb_sta.root"
};

//------------------
// legend or systm variation
const char* legendSyst[] = {
  "signal_CB3WN",
  "bkg_polFunct",
  "signal_resOpt2", 
  "bFrac_MLAR", 
  "constrained", 
//  "4DEff_profile", 
  "4DEff", 
  "3DEff_toy",
  "TnP_trgMuID", 
  "TnP_sta" 
};

// ######################################
// global uncertainties

  double systEventSelection[2] = {0.04, 0.01}; // HF gating and good event selection ... this should be different in pp and pbpb ...
  double systLumis[2]          = {0.04, 0.04}; // first for pp lumi, 2nd for counting the pbpb MB events

#endif

