#ifndef fitCharmoniaCtauModel_C
#define fitCharmoniaCtauModel_C

#include "Utilities/initClasses.h"
#include "buildCharmoniaCtauModel.C"
#include "fitCharmoniaMassModel.C"
#include "fitCharmoniaCtauErrModel.C"
#include "drawCtauPlot.C"

bool setCtauModel( struct OniaModel& model, map<string, string>&  parIni, bool isPbPb, bool incJpsi, bool incPsi2S, bool incBkg, bool incPrompt, bool incNonPrompt );
void setCtauFileName(string& FileName, string outputDir, string TAG, string plotLabel, struct KinCuts cut, bool isPbPb, bool cutSideBand);
void setCtauGlobalParameterRange(RooWorkspace& myws, map<string, string>& parIni, struct KinCuts& cut, string label);
void setCtauCutParameters(struct KinCuts& cut, bool incNonPrompt);


bool fitCharmoniaCtauModel( RooWorkspace& myws,             // Local Workspace
                            RooWorkspace& inputWorkspace,   // Workspace with all the input RooDatasets
                            struct KinCuts& cut,            // Variable containing all kinematic cuts
                            map<string, string>&  parIni,   // Variable containing all initial parameters
                            struct InputOpt& opt,           // Variable with run information (kept for legacy purpose)
                            string outputDir,               // Path to output directory
                            // Select the type of datasets to fit
                            string DSTAG,                   // Specifies the type of datasets: i.e, DATA, MCJPSINP, ...
                            bool isPbPb        = false,     // isPbPb = false for pp, true for PbPb
                            bool importDS      = true,      // Select if the dataset is imported in the local workspace
                            // Select the type of object to fit
                            bool incJpsi       = true,      // Includes Jpsi model
                            bool incPsi2S      = false,     // Includes Psi(2S) model
                            bool incBkg        = true,      // Includes Background model
                            bool incPrompt     = true,      // Includes Prompt ctau model
                            bool incNonPrompt  = false,     // Includes NonPrompt ctau model
                            // Select the fitting options
                            bool doFit         = true,      // Flag to indicate if we want to perform the fit
                            bool fitMass       = false,     // Indicates if the mass is fitted (2D fit)
                            bool wantPureSMC   = false,     // Flag to indicate if we want to fit pure signal MC
                            bool loadFitResult = false,     // Load previous fit results
                            map<string, string> inputFitDir={},// User-defined Location of the fit results
                            int  numCores      = 2,         // Number of cores used for fitting
                            // Select the drawing options
                            bool setLogScale   = true,      // Draw plot with log scale
                            bool incSS         = false,     // Include Same Sign data
                            map<string, double> binWidth={} // User-defined Location of the fit results
                            )  
{

  bool usePerEventError = true;
  
  if (DSTAG.find("_")!=std::string::npos) DSTAG.erase(DSTAG.find("_"));

  // Check if input dataset is MC
  bool isMC = false;
  if (DSTAG.find("MC")!=std::string::npos) {
    if (incJpsi && incPsi2S) { 
      cout << "[ERROR] We can only fit one type of signal using MC" << endl; return false; 
    }
    isMC = true;
  }
  if (!isMC) { wantPureSMC=false; }
  if (isMC) { fitMass = false; }
  bool cutSideBand = (incBkg && (!incPsi2S && !incJpsi));

  setCtauErrCutParameters(cut);
  setCtauCutParameters(cut, incNonPrompt);
  setMassCutParameters(cut, incJpsi, incPsi2S, isMC);

  string COLL = (isPbPb ? "PbPb" : "PP" );

  // Set models based on initial parameters
  struct OniaModel model;
  if (!setCtauModel(model, parIni, isPbPb, incJpsi, incPsi2S, incBkg, incPrompt, incNonPrompt)) { return false; }

  // Import the local datasets
  double numEntries = 1000000;
  string label = ((DSTAG.find(COLL.c_str())!=std::string::npos) ? DSTAG.c_str() : Form("%s_%s", DSTAG.c_str(), COLL.c_str()));
  if (wantPureSMC) label = Form("%s_NoBkg", label.c_str());
  string dsName = Form("dOS_%s", label.c_str());
  if (importDS) {
    if ( !(myws.data(dsName.c_str())) ) {
      int importID = importDataset(myws, inputWorkspace, cut, label, cutSideBand);
      if (importID<0) { return false; }
      else if (importID==0) { doFit = false; }
    }
    numEntries = myws.data(dsName.c_str())->sumEntries(); if (numEntries<=0) { doFit = false; }
  }
  else if (doFit && !(myws.data(dsName.c_str()))) { cout << "[ERROR] No local dataset was found to perform the fit!" << endl; return false; }
 
  // Set global parameters
  setCtauGlobalParameterRange(myws, parIni, cut, label);

  if (usePerEventError) { 
    // Setting extra input information needed by each fitter
    bool loadCtauErrFitResult = true;
    bool doCtauErrFit = true;
    bool importDS = isMC;
    string DSTAG = "DATA_PP";
  
    if ( !fitCharmoniaCtauErrModel( myws, inputWorkspace, cut, parIni, opt, outputDir, 
                                    DSTAG, isPbPb, importDS, 
                                    incJpsi, incPsi2S, incBkg, 
                                    doCtauErrFit, wantPureSMC, loadCtauErrFitResult, inputFitDir, numCores, 
                                    setLogScale, incSS, binWidth
                                    ) 
         ) { return false; }
  }

  if (fitMass) {
    // Setting extra input information needed by each fitter
    string iMassFitDir = inputFitDir["MASS"];
    double ibWidth = binWidth["MASS"];
    bool loadMassFitResult = true;
    bool doMassFit = true;
    bool importDS = false;
    bool getMeanPT = false;
    bool zoomPsi = false;
    const char* applyCorr = "";
    bool doSimulFit = false;
    bool cutCtau = false;

    if ( !fitCharmoniaMassModel( myws, inputWorkspace, cut, parIni, opt, outputDir, 
                                 DSTAG, isPbPb, importDS,
                                 incJpsi, incPsi2S, incBkg, 
                                 doMassFit, cutCtau, doSimulFit, wantPureSMC, applyCorr, loadMassFitResult, iMassFitDir, numCores, 
                                 setLogScale, incSS, zoomPsi, ibWidth, getMeanPT
                                 ) 
         ) { return false; }
  }

  // Build the Fit Model
  if (!buildCharmoniaCtauModel(myws, (isPbPb ? model.PbPb : model.PP), parIni, dsName, isPbPb, incBkg, incJpsi, incPsi2S, incPrompt, incNonPrompt,  numEntries))  { return false; }

  // Define pdf and plot names
  string pdfName = Form("pdfCTAU_Tot_%s", COLL.c_str());
  string plotLabel = "";
  if (incJpsi || incPsi2S) { plotLabel = plotLabel + Form("_Ctau_%s", parIni[Form("Model_Ctau_%s", COLL.c_str())].c_str());        }
  //if (incResol)            { plotLabel = plotLabel + Form("_CtauRes_%s", parIni[Form("Model_CtauRes_%s", COLL.c_str())].c_str()) ; }
  if (wantPureSMC)         { plotLabel = plotLabel + "_NoBkg"; }

  // check if we have already done this fit. If yes, do nothing and return true.
  string FileName = "";
  setCtauFileName(FileName, (inputFitDir["CTAU"]=="" ? outputDir : inputFitDir["CTAU"]), DSTAG, plotLabel, cut, isPbPb, cutSideBand);
  if (gSystem->AccessPathName(FileName.c_str()) && inputFitDir["CTAU"]!="") { 
    setCtauFileName(FileName, outputDir, DSTAG, plotLabel, cut, isPbPb, cutSideBand);
  }
  bool found =  true; bool skipFit = !doFit;
  RooArgSet *newpars = myws.pdf(pdfName.c_str())->getParameters(RooArgSet(*myws.var("ctau")));
  found = found && isFitAlreadyFound(newpars, FileName, pdfName.c_str());
  if (loadFitResult) {
    if ( loadPreviousFitResult(myws, FileName, DSTAG, isPbPb) ) { skipFit = true; } else  { skipFit = false; }
    if (skipFit) { cout << "[INFO] This ctau fit was already done, so I'll load the fit results." << endl; }
    myws.saveSnapshot(Form("%s_parLoad", pdfName.c_str()),*newpars,kTRUE);
  } else if (found) {
    cout << "[INFO] This ctau fit was already done, so I'll just go to the next one." << endl;
    return true;
  }

  // Fit the Datasets
  if (skipFit==false) {
    bool isWeighted = myws.data(dsName.c_str())->isWeighted();
    RooFitResult* fitResult = myws.pdf(pdfName.c_str())->fitTo(*myws.data(dsName.c_str()), Extended(kTRUE), SumW2Error(isWeighted), Range("CtauWindow"), NumCPU(numCores), Save());
    fitResult->Print("v");
    myws.import(*fitResult, Form("fitResult_%s", pdfName.c_str()));
    // Draw the mass plot
    int nBins = min(int( round((cut.dMuon.ctau.Max - cut.dMuon.ctau.Min)/binWidth["CTAU"]) ), 1000);
    drawCtauPlot(myws, outputDir, opt, cut, parIni, plotLabel, DSTAG, isPbPb, fitMass, incJpsi, incPsi2S, incBkg, incPrompt, incNonPrompt, wantPureSMC, setLogScale, incSS, nBins);
    // Save the results
    string FileName = ""; setCtauFileName(FileName, outputDir, DSTAG, plotLabel, cut, isPbPb, cutSideBand);
    myws.saveSnapshot(Form("%s_parFit", pdfName.c_str()),*newpars,kTRUE);
    saveWorkSpace(myws, Form("%sctau%s/%s/result", outputDir.c_str(), (cutSideBand?"SB":""), DSTAG.c_str()), FileName);
  }
  
  return true;
};


bool setCtauModel( struct OniaModel& model, map<string, string>&  parIni, bool isPbPb, bool incJpsi, bool incPsi2S, bool incBkg, bool incPrompt, bool incNonPrompt )
{
  if (isPbPb) {
    if (parIni.count("Model_CtauRes_PbPb")>0) {
      model.PbPb.CtauRes = CtauModelDictionary[parIni["Model_CtauRes_PbPb"]];
      if (model.PbPb.CtauRes==CtauModel(0)) {
        cout << "[ERROR] The ctau resolution model: " << parIni["Model_CtauRes_PbPb"] << " is invalid" << endl; return false;
      }
    } else { 
      cout << "[ERROR] Ctau Resolution model for PbPb was not found in the initial parameters!" << endl; return false;
    }
  } else {
    if (parIni.count("Model_CtauRes_PP")>0) {
      model.PP.CtauRes = CtauModelDictionary[parIni["Model_CtauRes_PP"]];
      if (model.PP.CtauRes==CtauModel(0)) {
        cout << "[ERROR] The ctau resolution model: " << parIni["Model_CtauRes_PP"] << " is invalid" << endl; return false;
      }
    } else { 
      cout << "[ERROR] Ctau Resolution model for PP was not found in the initial parameters!" << endl; return false;
    }
  }
  if (isPbPb && incBkg && incNonPrompt) {
    if (parIni.count("Model_BkgNoPR_PbPb")>0) {
      model.PbPb.Bkg.Ctau.NonPrompt = CtauModelDictionary[parIni["Model_BkgNoPR_PbPb"]];
      if (model.PbPb.Bkg.Ctau.NonPrompt==CtauModel(0)) {
        cout << "[ERROR] The background non-prompt ctau model: " << parIni["Model_BkgNoPR_PbPb"] << " is invalid" << endl; return false;
      }
    } else { 
      cout << "[ERROR] Background non-prompt ctau model for PbPb was not found in the initial parameters!" << endl; return false;
    }
  }
  if (isPbPb && incBkg && incPrompt) {
    if (parIni.count("Model_BkgPR_PbPb")>0) {
      model.PbPb.Bkg.Ctau.Prompt = CtauModelDictionary[parIni["Model_BkgPR_PbPb"]];
      if (model.PbPb.Bkg.Ctau.Prompt==CtauModel(0)) {
        cout << "[ERROR] The background prompt ctau model: " << parIni["Model_BkgPR_PbPb"] << " is invalid" << endl; return false;
      }
    } else {
      parIni["Model_BkgPR_PbPb"] = "Delta";
      model.PbPb.Bkg.Ctau.Prompt=CtauModel::Delta;
    }
  }
  if (isPbPb && incJpsi && incNonPrompt) {
    if (parIni.count("Model_JpsiNoPR_PbPb")>0) {
      model.PbPb.Jpsi.Ctau.NonPrompt = CtauModelDictionary[parIni["Model_JpsiNoPR_PbPb"]];
      if (model.PbPb.Jpsi.Ctau.NonPrompt==CtauModel(0)) {
        cout << "[ERROR] The Jpsi non-prompt ctau model: " << parIni["Model_JpsiNoPR_PbPb"] << " is invalid" << endl; return false;
      }
    } else { 
      cout << "[ERROR] Jpsi non-prompt ctau model for PbPb was not found in the initial parameters!" << endl; return false;
    }
  }
  if (isPbPb && incJpsi && incPrompt) {
    if (parIni.count("Model_JpsiPR_PbPb")>0) {
      model.PbPb.Jpsi.Ctau.Prompt = CtauModelDictionary[parIni["Model_JpsiPR_PbPb"]];
      if (model.PbPb.Jpsi.Ctau.Prompt==CtauModel(0)) {
        cout << "[ERROR] The Jpsi prompt ctau model: " << parIni["Model_JpsiPR_PbPb"] << " is invalid" << endl; return false;
      }
    } else { 
      parIni["Model_JpsiPR_PbPb"] = "Delta";
      model.PbPb.Jpsi.Ctau.Prompt=CtauModel::Delta;
    }
  }
  if (isPbPb && incPsi2S && incNonPrompt) {
    if (parIni.count("Model_Psi2SNoPR_PbPb")>0) {
      model.PbPb.Psi2S.Ctau.NonPrompt = CtauModelDictionary[parIni["Model_Psi2SNoPR_PbPb"]];
      if (model.PbPb.Psi2S.Ctau.NonPrompt==CtauModel(0)) {
        cout << "[ERROR] The psi(2S) non-prompt ctau model: " << parIni["Model_Psi2SNoPR_PbPb"] << " is invalid" << endl; return false;
      }
    } else { 
      cout << "[ERROR] psi(2S) non-prompt ctau model for PbPb was not found in the initial parameters!" << endl; return false;
    }
  }
  if (isPbPb && incPsi2S && incPrompt) {
    if (parIni.count("Model_Psi2SPR_PbPb")>0) {
      model.PbPb.Psi2S.Ctau.Prompt = CtauModelDictionary[parIni["Model_Psi2SPR_PbPb"]];
      if (model.PbPb.Psi2S.Ctau.Prompt==CtauModel(0)) {
        cout << "[ERROR] The psi(2S) prompt ctau model: " << parIni["Model_Psi2SPR_PbPb"] << " is invalid" << endl; return false;
      }
    } else { 
      parIni["Model_Psi2SPR_PbPb"] = "Delta";
      model.PbPb.Psi2S.Ctau.Prompt=CtauModel::Delta;
    }
  }

  if (!isPbPb && incBkg && incNonPrompt) {
    if (parIni.count("Model_BkgNoPR_PP")>0) {
      model.PP.Bkg.Ctau.NonPrompt = CtauModelDictionary[parIni["Model_BkgNoPR_PP"]];
      if (model.PP.Bkg.Ctau.NonPrompt==CtauModel(0)) {
        cout << "[ERROR] The background non-prompt ctau model: " << parIni["Model_BkgNoPR_PP"] << " is invalid" << endl; return false;
      }
    } else { 
      cout << "[ERROR] Background non-prompt ctau model for PP was not found in the initial parameters!" << endl; return false;
    }
  }
  if (!isPbPb && incBkg && incPrompt) {
    if (parIni.count("Model_BkgPR_PP")>0) {
      model.PP.Bkg.Ctau.Prompt = CtauModelDictionary[parIni["Model_BkgPR_PP"]];
      if (model.PP.Bkg.Ctau.Prompt==CtauModel(0)) {
        cout << "[ERROR] The background prompt ctau model: " << parIni["Model_BkgPR_PP"] << " is invalid" << endl; return false;
      }
    } else { 
      parIni["Model_BkgPR_PP"] = "Delta";
      model.PP.Bkg.Ctau.Prompt=CtauModel::Delta;
    }
  }
  if (!isPbPb && incJpsi && incNonPrompt) {
    if (parIni.count("Model_JpsiNoPR_PP")>0) {
      model.PP.Jpsi.Ctau.NonPrompt = CtauModelDictionary[parIni["Model_JpsiNoPR_PP"]];
      if (model.PP.Jpsi.Ctau.NonPrompt==CtauModel(0)) {
        cout << "[ERROR] The Jpsi non-prompt ctau model: " << parIni["Model_JpsiNoPR_PP"] << " is invalid" << endl; return false;
      }
    } else { 
      cout << "[ERROR] Jpsi non-prompt ctau model for PP was not found in the initial parameters!" << endl; return false;
    }
  }
  if (!isPbPb && incJpsi && incPrompt) {
    if (parIni.count("Model_JpsiPR_PP")>0) {
      model.PP.Jpsi.Ctau.Prompt = CtauModelDictionary[parIni["Model_JpsiPR_PP"]];
      if (model.PP.Jpsi.Ctau.Prompt==CtauModel(0)) {
        cout << "[ERROR] The Jpsi prompt ctau model: " << parIni["Model_JpsiPR_PP"] << " is invalid" << endl; return false;
      }
    } else { 
      parIni["Model_JpsiPR_PP"] = "Delta";
      model.PP.Jpsi.Ctau.Prompt=CtauModel::Delta;
    }
  }
  if (!isPbPb && incPsi2S && incNonPrompt) {
    if (parIni.count("Model_Psi2SNoPR_PP")>0) {
      model.PP.Psi2S.Ctau.NonPrompt = CtauModelDictionary[parIni["Model_Psi2SNoPR_PP"]];
      if (model.PP.Psi2S.Ctau.NonPrompt==CtauModel(0)) {
        cout << "[ERROR] The psi(2S) non-prompt ctau model: " << parIni["Model_Psi2SNoPR_PP"] << " is invalid" << endl; return false;
      }
    } else { 
      cout << "[ERROR] psi(2S) non-prompt ctau model for PP was not found in the initial parameters!" << endl; return false;
    }
  }
  if (!isPbPb && incPsi2S && incPrompt) {
    if (parIni.count("Model_Psi2SPR_PP")>0) {
      model.PP.Psi2S.Ctau.Prompt = CtauModelDictionary[parIni["Model_Psi2SPR_PP"]];
      if (model.PP.Psi2S.Ctau.Prompt==CtauModel(0)) {
        cout << "[ERROR] The psi(2S) prompt ctau model: " << parIni["Model_Psi2SPR_PP"] << " is invalid" << endl; return false;
      }
    } else { 
      parIni["Model_Psi2SPR_PP"] = "Delta";
      model.PP.Psi2S.Ctau.Prompt=CtauModel::Delta;
    }
  }

  return true;
};


void setCtauGlobalParameterRange(RooWorkspace& myws, map<string, string>& parIni, struct KinCuts& cut, string label)
{
  Double_t ctauMax; Double_t ctauMin;
  myws.data(Form("dOS_%s", label.c_str()))->getRange(*myws.var("ctau"), ctauMin, ctauMax);
  if (ctauMin<cut.dMuon.ctau.Min) { ctauMin = cut.dMuon.ctau.Min; }
  if (ctauMax>cut.dMuon.ctau.Max) { ctauMax = cut.dMuon.ctau.Max; }
  cout << "Range from data: ctauMin: " << ctauMin << "  ctauMax: " << ctauMax << endl;
  myws.var("ctau")->setRange("CtauWindow", ctauMin, ctauMax);
  parIni["CtauRange_Cut"]   = Form("(%.12f <= ctau && ctau < %.12f)", ctauMin, ctauMax);
  cut.dMuon.ctau.Max = (double)(ceil(ctauMax*10.)/10.);
  cut.dMuon.ctau.Min = (double)(floor(ctauMin*10.)/10.);
  myws.var("ctau")->setMin(cut.dMuon.ctau.Min);
  myws.var("ctau")->setMax(cut.dMuon.ctau.Max);
  myws.var("ctau")->setRange("CtauFullWindow", cut.dMuon.ctau.Min, cut.dMuon.ctau.Max);
  myws.var("ctau")->setRange("FullWindow", cut.dMuon.ctau.Min, cut.dMuon.ctau.Max);
  myws.var("ctau")->setRange("SideBandTOP_FULL", cut.dMuon.ctau.Min, cut.dMuon.ctau.Max); 
  myws.var("ctau")->setRange("SideBandMID_FULL", cut.dMuon.ctau.Min, cut.dMuon.ctau.Max);
  myws.var("ctau")->setRange("SideBandBOT_FULL", cut.dMuon.ctau.Min, cut.dMuon.ctau.Max); 
  myws.var("ctau")->setRange("SideBandMID_JPSI", cut.dMuon.ctau.Min, cut.dMuon.ctau.Max);
  myws.var("ctau")->setRange("SideBandBOT_JPSI", cut.dMuon.ctau.Min, cut.dMuon.ctau.Max);
  myws.var("ctau")->setRange("SideBandTOP_PSI2S", cut.dMuon.ctau.Min, cut.dMuon.ctau.Max); 
  myws.var("ctau")->setRange("SideBandMID_PSI2S", cut.dMuon.ctau.Min, cut.dMuon.ctau.Max);

  return;
};


void setCtauFileName(string& FileName, string outputDir, string TAG, string plotLabel, struct KinCuts cut, bool isPbPb, bool cutSideBand)
{
  if (TAG.find("_")!=std::string::npos) TAG.erase(TAG.find("_"));
  FileName = Form("%sctau%s/%s/result/FIT_%s_%s_%s%s_pt%.0f%.0f_rap%.0f%.0f_cent%d%d.root", outputDir.c_str(), (cutSideBand?"SB":""), TAG.c_str(), "CTAU", TAG.c_str(), (isPbPb?"PbPb":"PP"), plotLabel.c_str(), (cut.dMuon.Pt.Min*10.0), (cut.dMuon.Pt.Max*10.0), (cut.dMuon.AbsRap.Min*10.0), (cut.dMuon.AbsRap.Max*10.0), cut.Centrality.Start, cut.Centrality.End);

  return;
};
 

void setCtauCutParameters(struct KinCuts& cut, bool incNonPrompt)
{
  // Define the ctau range
  if (cut.dMuon.ctau.Min==-50. && cut.dMuon.ctau.Max==50.) { 
    // Default ctau values, means that the user did not specify a ctau range
    if (incNonPrompt) {
      cut.dMuon.ctau.Min = -4.0;
      cut.dMuon.ctau.Max = 6.0;
    } else {
      cut.dMuon.ctau.Min = -2.0;
      cut.dMuon.ctau.Max = 2.0;
    }
  }
  cout << "[INFO] Setting ctau range to min: " << cut.dMuon.ctau.Min << " and max " << cut.dMuon.ctau.Max << endl;
  
  return;
};


#endif // #ifndef fitCharmoniaCtauModel_C
