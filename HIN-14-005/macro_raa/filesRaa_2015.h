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

// ######################################### the nominal corrections #####################
  // ##### Lxyz with TnP corrections applied to data (not to the MC 4D efficiencies)
  const char* yieldHistFile_yesWeight_1[2] = {
    "histsRaaYields_20151217_PbPb_weightedEff_TnPAtRD3_4DEff2_RapPtEffMap3_Ratio.root",
    "histsRaaYields_20151217_pp_weightedEff_TnPAtRD3_4DEff2_RapPtEffMap3_Ratio.root"
  };
  
  const char* yieldHistFile_noWeight_1[2] = {
    "histsRaaYields_20151217_PbPb_noWeight_TnPAtRD3_4DEff2_RapPtEffMap3_Ratio.root",
    "histsRaaYields_20151217_pp_noWeight_TnPAtRD3_4DEff2_RapPtEffMap3_Ratio.root"
  };

// #######################################################################################

  // ##### Lxyz, no TnP corrections
  // Lxyz, 4D eff is applied
  const char* yieldHistFile_yesWeight_0[2] = {
   "histsRaaYields_20151209_PbPb_weightedEff_noTnP_4DEff2_RapPtEffMap4_Ratio.root",
   "histsRaaYields_20151209_pp_weightedEff_noTnP_4DEff2_RapPtEffMap4_Ratio.root"
  };
  
  // Lxyz, 4D eff is applied
  const char* yieldHistFile_noWeight_0[2] = {
    "histsRaaYields_20151209_PbPb_noWeight_TnPAtRD3_4DEff2_RapPtEffMap4_Ratio.root",
    "histsRaaYields_20151209_pp_noWeight_TnPAtRD3_4DEff2_RapPtEffMap4_Ratio.root"
  };

  // ##### 3D efficiencies
//  const char* effHistFile[2]       = {"histEff_pbpb_tradEff_20150825.root",       "histEff_pp_tradEff_20150825.root"};
  const char* effHistFile[2]       = {"histEff_20151201_PbPb_newTnP.root", "histEff_20151201_pp_newTnP.root"};
  const char* effHistFile_noTnP[2] = {"histEff_20151201_PbPb_noTnP.root", "histEff_20151201_pp_noTnP.root"};

#endif

