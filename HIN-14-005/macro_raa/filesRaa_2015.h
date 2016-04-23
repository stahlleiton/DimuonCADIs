#ifndef FILESRAA_2015
#define FILESRAA_2015

// ---------------------------------------------- files with fit results!!!! they are in the readFitTable directory
// needed by makeRaa_x.C and compare_x.C macros

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
  const char* yieldHistFile_yesWeight_0[2] = {
   "histsRaaYields_20160304_PbPb_weighted_noTnPSF.root",
   "histsRaaYields_20160304_pp_weighted_noTnPSF.root"
  };

  // ##### 3D efficiencies
  const char* effHistFile[2]       = {"histEff_20160411_PbPb_newTnP.root", "histEff_20160411_pp_newTnP.root"};

#endif

