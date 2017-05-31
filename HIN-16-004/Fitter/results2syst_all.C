#ifndef results2syst_all_C
#define results2syst_all_C

#include <iostream>

#include "TString.h"

#include "results2syst.C"

using namespace std;

void results2syst_all(bool is16004 = false)
{
  TString app("");
  if (is16004) app = "16004_";
  
    // Background systematics
    //NJpsi
    results2syst(Form("DataFits_%s2D_2CB_polBkg_nominal,DataFits_%s2D_2CB_polBkg_minvRange,DataFits_%s2D_2CB_polBkg_LLR25,DataFits_%s2D_2CB_polBkg_LLR10,DataFits_%s2D_2CB_expBkg",app.Data(),app.Data(),app.Data(),app.Data(),app.Data()),"syst_NJpsi_PbPb_massBkg.csv","inv. mass bkg.",1,"PbPb",true,"N_Jpsi");
    results2syst(Form("DataFits_%s2D_2CB_polBkg_nominal,DataFits_%s2D_2CB_polBkg_minvRange,DataFits_%s2D_2CB_polBkg_LLR25,DataFits_%s2D_2CB_polBkg_LLR10,DataFits_%s2D_2CB_expBkg",app.Data(),app.Data(),app.Data(),app.Data(),app.Data()),"syst_NJpsi_PP_massBkg.csv","inv. mass bkg.",1,"PP",true,"N_Jpsi");
    
    //b fraction
    results2syst(Form("DataFits_%s2D_2CB_polBkg_nominal,DataFits_%s2D_2CB_polBkg_minvRange,DataFits_%s2D_2CB_polBkg_LLR25,DataFits_%s2D_2CB_polBkg_LLR10,DataFits_%s2D_2CB_expBkg",app.Data(),app.Data(),app.Data(),app.Data(),app.Data()),"syst_BJpsi_PbPb_massBkg.csv","inv. mass bkg.",1,"PbPb",true,"b_Jpsi");
    results2syst(Form("DataFits_%s2D_2CB_polBkg_nominal,DataFits_%s2D_2CB_polBkg_minvRange,DataFits_%s2D_2CB_polBkg_LLR25,DataFits_%s2D_2CB_polBkg_LLR10,DataFits_%s2D_2CB_expBkg",app.Data(),app.Data(),app.Data(),app.Data(),app.Data()),"syst_BJpsi_PP_massBkg.csv","inv. mass bkg.",1,"PP",true,"b_Jpsi");
    
    
    // Signal systematics
    //NJpsi
    results2syst(Form("DataFits_%s2D_2CB_polBkg_nominal,DataFits_%s2D_2CB_polBkg_constrained,DataFits_%s2D_CBG_polBkg",app.Data(),app.Data(),app.Data()),"syst_NJpsi_PbPb_massSig.csv","inv. mass sig.",1,"PbPb",true,"N_Jpsi");
    results2syst(Form("DataFits_%s2D_2CB_polBkg_nominal,DataFits_%s2D_2CB_polBkg_constrained,DataFits_%s2D_CBG_polBkg",app.Data(),app.Data(),app.Data()),"syst_NJpsi_PP_massSig.csv","inv. mass sig.",1,"PP",true,"N_Jpsi");
    
    //b fraction
    results2syst(Form("DataFits_%s2D_2CB_polBkg_nominal,DataFits_%s2D_2CB_polBkg_constrained,DataFits_%s2D_CBG_polBkg",app.Data(),app.Data(),app.Data()),"syst_BJpsi_PbPb_massSig.csv","inv. mass sig.",1,"PbPb",true,"b_Jpsi");
    results2syst(Form("DataFits_%s2D_2CB_polBkg_nominal,DataFits_%s2D_2CB_polBkg_constrained,DataFits_%s2D_CBG_polBkg",app.Data(),app.Data(),app.Data()),"syst_BJpsi_PP_massSig.csv","inv. mass sig.",1,"PP",true,"b_Jpsi");
    
    // CtauErr systematics
    //NJpsi
    results2syst(Form("DataFits_%s2D_2CB_polBkg_nominal,DataFits_%s2D_2CB_polBkg_ctauErrTot",app.Data(),app.Data()),"syst_NJpsi_PbPb_ctauErr.csv","ctau error",1,"PbPb",true,"N_Jpsi");
    results2syst(Form("DataFits_%s2D_2CB_polBkg_nominal,DataFits_%s2D_2CB_polBkg_ctauErrTot",app.Data(),app.Data()),"syst_NJpsi_PP_ctauErr.csv","ctau error",1,"PP",true,"N_Jpsi");
    
    //b fraction
    results2syst(Form("DataFits_%s2D_2CB_polBkg_nominal,DataFits_%s2D_2CB_polBkg_ctauErrTot",app.Data(),app.Data()),"syst_BJpsi_PbPb_ctauErr.csv","ctau error",1,"PbPb",true,"b_Jpsi");
    results2syst(Form("DataFits_%s2D_2CB_polBkg_nominal,DataFits_%s2D_2CB_polBkg_ctauErrTot",app.Data(),app.Data()),"syst_BJpsi_PP_ctauErr.csv","ctau error",1,"PP",true,"b_Jpsi");
    
    // CtauTrue systematics
    //NJpsi
    results2syst(Form("DataFits_%s2D_2CB_polBkg_nominal,DataFits_%s2D_2CB_polBkg_ctauRecoTemp",app.Data(),app.Data()),"syst_NJpsi_PbPb_ctauTrue.csv","ctau true",1,"PbPb",true,"N_Jpsi");
    results2syst(Form("DataFits_%s2D_2CB_polBkg_nominal,DataFits_%s2D_2CB_polBkg_ctauRecoTemp",app.Data(),app.Data()),"syst_NJpsi_PP_ctauTrue.csv","ctau true",1,"PP",true,"N_Jpsi");
    
    //b fraction
    results2syst(Form("DataFits_%s2D_2CB_polBkg_nominal,DataFits_%s2D_2CB_polBkg_ctauRecoTemp",app.Data(),app.Data()),"syst_BJpsi_PbPb_ctauTrue.csv","ctau true",1,"PbPb",true,"b_Jpsi");
    results2syst(Form("DataFits_%s2D_2CB_polBkg_nominal,DataFits_%s2D_2CB_polBkg_ctauRecoTemp",app.Data(),app.Data()),"syst_BJpsi_PP_ctauTrue.csv","ctau true",1,"PP",true,"b_Jpsi");
    
    // CtauRes systematics
    //NJpsi
    results2syst(Form("DataFits_%s2D_2CB_polBkg_nominal,DataFits_%s2D_2CB_polBkg_ctauResPromptMC,DataFits_%s2D_2CB_polBkg_ctauResNonPromptMC",app.Data(),app.Data(),app.Data()),"syst_NJpsi_PbPb_ctauRes.csv","ctau res.",1,"PbPb",true,"N_Jpsi");
    results2syst(Form("DataFits_%s2D_2CB_polBkg_nominal,DataFits_%s2D_2CB_polBkg_ctauResPromptMC,DataFits_%s2D_2CB_polBkg_ctauResNonPromptMC",app.Data(),app.Data(),app.Data()),"syst_NJpsi_PP_ctauRes.csv","ctau res.",1,"PP",true,"N_Jpsi");
    
    //b fraction
    results2syst(Form("DataFits_%s2D_2CB_polBkg_nominal,DataFits_%s2D_2CB_polBkg_ctauResPromptMC,DataFits_%s2D_2CB_polBkg_ctauResNonPromptMC",app.Data(),app.Data(),app.Data()),"syst_BJpsi_PbPb_ctauRes.csv","ctau res.",1,"PbPb",true,"b_Jpsi");
    results2syst(Form("DataFits_%s2D_2CB_polBkg_nominal,DataFits_%s2D_2CB_polBkg_ctauResPromptMC,DataFits_%s2D_2CB_polBkg_ctauResNonPromptMC",app.Data(),app.Data(),app.Data()),"syst_BJpsi_PP_ctauRes.csv","ctau res.",1,"PP",true,"b_Jpsi");
    
    // CtauBkg systematics
    //NJpsi
    results2syst(Form("DataFits_%s2D_2CB_polBkg_nominal,DataFits_%s2D_2CB_polBkg_ctauBkgTemp",app.Data(),app.Data()),"syst_NJpsi_PbPb_ctauBkg.csv","ctau bkg.",1,"PbPb",true,"N_Jpsi");
    results2syst(Form("DataFits_%s2D_2CB_polBkg_nominal,DataFits_%s2D_2CB_polBkg_ctauBkgTemp",app.Data(),app.Data()),"syst_NJpsi_PP_ctauBkg.csv","ctau bkg.",1,"PP",true,"N_Jpsi");
    
    //b fraction
    results2syst(Form("DataFits_%s2D_2CB_polBkg_nominal,DataFits_%s2D_2CB_polBkg_ctauBkgTemp",app.Data(),app.Data()),"syst_BJpsi_PbPb_ctauBkg.csv","ctau bkg.",1,"PbPb",true,"b_Jpsi");
    results2syst(Form("DataFits_%s2D_2CB_polBkg_nominal,DataFits_%s2D_2CB_polBkg_ctauBkgTemp",app.Data(),app.Data()),"syst_BJpsi_PP_ctauBkg.csv","ctau bkg.",1,"PP",true,"b_Jpsi");
    
    return;
  }
#endif
