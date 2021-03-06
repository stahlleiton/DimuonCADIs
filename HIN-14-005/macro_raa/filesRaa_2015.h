#ifndef FILESRAA_2015
#define FILESRAA_2015

// ---------------------------------------------- files with fit results!!!! they are in the readFitTable directory
// needed by makeRaa_x.C and compare_x.C macros
  // ##### Lxy fits and TnP corrections
  const char* yieldHistFile_yesWeight_3[2] = {"histsRaaYields_20150127_PbPb_raa_weightedEff_InEta.root",
               "histsRaaYields_20150127_pp_raa_weightedEff_InEta.root"};
  const char* yieldHistFile_noWeight_3[2]  = {"histsRaaYields_20150127_PbPb_raa_noWeight_InEta.root",
               "histsRaaYields_20150127_pp_raa_noWeight_InEta.root"};

  // ##### Lxyz with TnP corrections applied to the MC 4D efficiencies)  
  const char* yieldHistFile_yesWeight_2[2] = {
   "histsRaaYields_20150823_PbPb_Lxyz_weightedEff_Lxyz_pTtune_PRMC.root",
   "histsRaaYields_20150823_pp_Lxyz_weightedEff_Lxyz_finerpT_PRMC.root"
  };
  
  const char* yieldHistFile_noWeight_2[2] = {
   "histsRaaYields_20150823_PbPb_Lxyz_noWeight_Lxyz_pTtune_PRMC.root",
   "histsRaaYields_20150823_pp_Lxyz_noWeight_Lxyz_finerpT_PRMC.root"
  };

  // default setting
  // ##### Lxyz with TnP corrections applied to data (not to the MC 4D efficiencies)
  const char* yieldHistFile_yesWeight_1[2] = {
    "histsRaaYields_20160304_PbPb_weighted.root",
    "histsRaaYields_20160304_pp_weighted.root",
  };
  
  const char* yieldHistFile_noWeight_1[2] = {
    "histsRaaYields_20160304_PbPb_noWeight.root",
    "histsRaaYields_20160304_pp_noWeight.root",
  };

  // ##### Lxyz, no TnP corrections
  // nominal for comp
  const char* yieldHistFile_yesWeight_0[2] = {
    "histsRaaYields_20160304_PbPb_weighted.root",
    "histsRaaYields_20160304_pp_weighted.root",
  };
  
  // Lxyz, 4D eff is applied
  const char* yieldHistFile_noWeight_0[2] = {
   "histsRaaYields_20160304_PbPb_weighted_noTnPSF.root",
   "histsRaaYields_20160304_pp_weighted_noTnPSF.root"
  };

  // ##### 3D efficiencies
  const char* effHistFile[2]       = {"histEff_20160411_PbPb_newTnP.root", "histEff_20160411_pp_newTnP.root"};
  const char* effHistFile_noTnP[2] = {"histEff_20160411_PbPb_newTnP.root", "histEff_20160411_pp_newTnP.root"}; // currently bogus file names are filled 

#endif

