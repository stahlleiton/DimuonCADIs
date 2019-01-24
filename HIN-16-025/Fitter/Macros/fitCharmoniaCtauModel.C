#ifndef fitCharmoniaCtauModel_C
#define fitCharmoniaCtauModel_C

#include "Utilities/initClasses.h"
#include "buildCharmoniaCtauModel.C"
#include "fitCharmoniaMassModel.C"
#include "fitCharmoniaCtauResModel.C"
#include "fitCharmoniaCtauErrModel.C"
#include "drawCtauPlot.C"

bool setCtauModel    ( struct OniaModel& model, map<string, string>&  parIni, const bool& isPbPb, const bool& incJpsi,
                       const bool& incPsi2S, const bool& incBkg, const bool& incPrompt, const bool& incNonPrompt );
void setCtauFileName ( string& fileName, const string& outputDir, string TAG, const string& plotLabel, const struct KinCuts& cut, const bool& isPbPb, const bool& fitSideBand, const bool& usectauBkgTemplate );
void setCtauGlobalParameterRange ( RooWorkspace& myws, map<string, string>& parIni, struct KinCuts& cut, const string& label, const double& binWidth, const bool fitCtauRes=false, const bool is2DFit=false );
void setCtauCutParameters     ( struct KinCuts& cut, const bool& incNonPrompt );
bool isCtauBkgPdfAlreadyFound ( const RooWorkspace& myws, const string& fileName, const string& pdfName, const bool loadCtauBkgPdf=false );
bool createCtauDSUsingSPLOT   ( RooWorkspace& ws, const string& dsName, const map<string, string>&  parIni, const struct KinCuts& cut, const bool& incJpsi, const bool& incPsi2S, const bool& incBkg );


bool fitCharmoniaCtauModel( RooWorkspace& myws,             // Local Workspace
                            const RooWorkspace& inputWorkspace,  // Workspace with all the input RooDatasets
                                  struct KinCuts& cut,           // Variable containing all kinematic cuts
                                  map<string, string>&  parIni,  // Variable containing all initial parameters
                            const struct InputOpt& opt,          // Variable with run information (kept for legacy purpose)
                            const string& outputDir,             // Path to output directory
                            // Select the type of datasets to fit
                                  string DSTAG,                  // Specifies the type of datasets: i.e, DATA, MCJPSINP, ...
                            const bool& isPbPb,                  // isPbPb = false for pp, true for PbPb
                            const bool& importDS,                // Select if the dataset is imported in the local workspace
                            // Select the type of object to fit
                            const bool incJpsi,                  // Includes Jpsi model
                            const bool incPsi2S,                 // Includes Psi(2S) model
                            const bool incBkg,                   // Includes Background model
                            const bool& incPrompt,               // Includes Prompt ctau model
                            const bool& incNonPrompt,            // Includes NonPrompt ctau model
                            const bool& useTotctauErrPdf,        // If yes use the total ctauErr PDF instead of Jpsi and bkg ones
                            const bool& usectauBkgTemplate,      // If yes use a template for Bkg ctau instead of the fitted Pdf
                            // Select the fitting options
                            const bool& useSPlot,                // If yes, then use SPlot technique, if no, use mass range
                                  bool  doFit,                   // Flag to indicate if we want to perform the fit
                            const bool& doJpsi,                  // Flag to indicate if we want to perform fits for Jpsi analysis
                            const bool& doPsi2S,                 // Flag to indicate if we want to perform fits for Psi2S analysis
                                  bool  wantPureSMC,             // Flag to indicate if we want to fit pure signal MC
                            const bool& loadFitResult,           // Load previous fit results
                            const map<string, string>& inputFitDir,// Location of the fit results
                            const int&  numCores,                // Number of cores used for fitting
                            // Select the drawing options
                            const bool& setLogScale,             // Draw plot with log scale
                            const bool& incSS,                   // Include Same Sign data
                            const map<string, double>& binWidth  // Bin width used for plotting
                            )
{
  
  const bool usePerEventError = true;
  
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
  if (useSPlot && ((incBkg&&incJpsi)||(incBkg&&incPsi2S)||(incPsi2S&&incJpsi))) { cout << "[ERROR] Only one object can be fitted using sPlot!" << endl; return false; }
  const bool fitSideBand = (incBkg && (!incPsi2S && !incJpsi));

  const string COLL = (isPbPb ? "PbPb" : "PP" );
  string fitType = "CTAU";
  if (!isMC && fitSideBand) { fitType = "CTAUSB";  }
  string label = ((DSTAG.find(COLL.c_str())!=std::string::npos) ? DSTAG.c_str() : Form("%s_%s", DSTAG.c_str(), COLL.c_str()));

  if (importDS) {
    setMassCutParameters(cut, doJpsi, doPsi2S, isMC, true);
    setCtauCutParameters(cut, incNonPrompt);
    if (usePerEventError) {
      // check if we have already done the ctauErr fits. If yes, load their parameters
      string fileName = "";
      const string pdfName = Form("pdfCTAUERR_Tot_%s", COLL.c_str());
      string plotLabel = "";
      const bool fitSideBand = false;
      if (doJpsi)  { plotLabel = plotLabel + "_Jpsi";     }
      if (doPsi2S) { plotLabel = plotLabel + "_Psi2S";    }
      plotLabel = plotLabel + "_Bkg";
      setCtauErrFileName(fileName, (inputFitDir.at("CTAUERR")=="" ? outputDir : inputFitDir.at("CTAUERR")), "DATA", plotLabel, cut, isPbPb, fitSideBand);
      bool foundFit = false;
      if ( loadCtauErrRange(fileName, cut) ) { foundFit = true; }
      if (foundFit) { cout << "[INFO] The ctauErr fit was found and I'll load the ctau Error range used." << endl; }
      else { cout << "[ERROR] The ctauErr fit was not found!" << endl; return false; }
      // Importing SPLOT DS from ctauErr results
      const string sPlotDSName = Form("dOS_%s_SPLOT", label.c_str());
      if (useSPlot) { loadSPlotDS(myws, fileName, sPlotDSName); }
    }
    setCtauErrCutParameters(cut);
  }
  // Import the local datasets
  double numEntries = 1000000;
  if (wantPureSMC) label = Form("%s_NoBkg", label.c_str());
  const string dsName = Form("dOS_%s", label.c_str());
  if (importDS) {
    if ( !(myws.data(dsName.c_str())) ) {
      const int importID = importDataset(myws, inputWorkspace, cut, label, (fitSideBand && !useSPlot));
      if (importID<0) { return false; }
      else if (importID==0) { doFit = false; }
    }
    numEntries = myws.data(dsName.c_str())->sumEntries(); if (numEntries<=0) { doFit = false; }
  }
  else if (doFit && !(myws.data(dsName.c_str()))) { cout << "[ERROR] No local dataset was found to perform the fit!" << endl; return false; }

  if (importDS) { 
    // Set global parameters
    setCtauErrGlobalParameterRange(myws, parIni, cut, "", binWidth.at("CTAUERR"), true);
    setCtauGlobalParameterRange(myws, parIni, cut, label, binWidth.at(fitType.c_str()), (!incBkg&&useSPlot&&!isPbPb));
  }

  string dsName2Fit = dsName;
  if (useSPlot && incBkg) dsName2Fit += "_BKG";
  else if (useSPlot && incJpsi) dsName2Fit += "_JPSI";
  else if (useSPlot && incPsi2S) dsName2Fit += "_PSI2S";
  const string dsName2FitCut = dsName2Fit+"_CTAUCUT";

  string plotLabel = "";
  const map<string, bool> plotLabels = {{"JpsiNoPR", (incJpsi&&incNonPrompt)}, 
                                        {"Psi2SNoPR", (incPsi2S&&incNonPrompt)}, 
                                        {"BkgNoPR", (incBkg&&incNonPrompt)},
                                        {"CtauRes", (true)}};
  for (auto const& lbl : plotLabels) {
    const string& obj = lbl.first;
    const bool& cond = lbl.second;
    if (cond && parIni.count(Form("Model_%s_%s", obj.c_str(), COLL.c_str()))>0) { 
      plotLabel = plotLabel + Form("_%s_%s", obj.c_str(), parIni[Form("Model_%s_%s", obj.c_str(), COLL.c_str())].c_str()); 
    }
  }
  if (wantPureSMC) { plotLabel = plotLabel + "_NoBkg"; }
  const string pdfName = Form("pdfCTAU_Tot_%s", COLL.c_str());

  string fileName = "";
  setCtauFileName(fileName, (inputFitDir.at(fitType)=="" ? outputDir : inputFitDir.at(fitType)), DSTAG, plotLabel, cut, isPbPb, fitSideBand, usectauBkgTemplate);
  if (gSystem->AccessPathName(fileName.c_str()) && inputFitDir.at(fitType)!="") {
    cout << "[WARNING] User Input File : " << fileName << " was not found!" << endl;
    if (loadFitResult) return false;
    setCtauFileName(fileName, outputDir, DSTAG, plotLabel, cut, isPbPb, fitSideBand, usectauBkgTemplate);
  }

  // check if we have already done this fit. If yes, do nothing and return true.
  bool found =  true; bool skipFit = !doFit;
  if (usectauBkgTemplate)
  {
    vector<string> pdfNames; pdfNames.push_back(Form("pdfCTAUCOND_Bkg_%s", COLL.c_str()));
    found = found && isPdfAlreadyFound(myws, fileName, pdfNames, loadFitResult);
    if (found) {
      if (loadFitResult) {
        cout << "[INFO] This ctau Bkg Pdf was already made, so I'll load the pdf." << endl;
      } else {
        cout << "[INFO] This ctau Bkg Pdf was already made, so I'll just go to the next one." << endl;
      }
      return true;
    }
  }

  if (useSPlot) {
    // Check if we have already made the Background DataSet
    const vector<string> dsNames = { dsName2Fit };
    const bool createSPLOTDS = ( !isSPlotDSAlreadyFound(myws, fileName, dsNames, true) );
    const bool compDS = loadYields(myws, fileName, dsName, pdfName);
    if (!compDS && (incJpsi || incPsi2S || incBkg)) {
      // Setting extra input information needed by each fitter
      const string iMassFitDir = inputFitDir.at("MASS");
      const double ibWidth = binWidth.at("MASS");
      const bool loadMassFitResult = true;
      const bool doMassFit = false;
      const bool impDS = false;
      const bool getMeanPT = false;
      const bool zoomPsi = false;
      const string applyCorr = "";
      const bool doSimulFit = false;
      const bool cutCtau = false;
      const bool doConstrFit = false;
      
      if ( !fitCharmoniaMassModel( myws, inputWorkspace, cut, parIni, opt, outputDir,
                                   DSTAG, isPbPb, impDS,
                                   doJpsi, doPsi2S, true,
                                   doMassFit, cutCtau, doConstrFit, doSimulFit, false, applyCorr, loadMassFitResult, iMassFitDir, numCores,
                                   setLogScale, incSS, zoomPsi, ibWidth, getMeanPT
                                   )
           ) { return false; }
      // Let's set all mass parameters to constant except the yields
      if (myws.pdf(Form("pdfMASS_Tot_%s", (isPbPb?"PbPb":"PP")))) {
        cout << "[INFO] Setting mass parameters to constant!" << endl;
        auto par = std::unique_ptr<RooArgSet>(myws.pdf(Form("pdfMASS_Tot_%s", (isPbPb?"PbPb":"PP")))->getParameters(RooArgSet(*myws.var("invMass"))));
        par->setAttribAll("Constant", kTRUE);
      } else { cout << "[ERROR] Mass PDF was not found!" << endl; return false; }
      std::vector< std::string > objs = {"Bkg", "Jpsi", "Psi2S"};
      for (auto obj : objs) { if (myws.var(Form("N_%s_%s", obj.c_str(), (isPbPb?"PbPb":"PP")))) setConstant( myws, Form("N_%s_%s", obj.c_str(), (isPbPb?"PbPb":"PP")), false); }
      // Let's set the minimum value of the yields to zero
      for (auto obj : objs) { if (myws.var(Form("N_%s_%s", obj.c_str(), (isPbPb?"PbPb":"PP")))) myws.var(Form("N_%s_%s", obj.c_str(), (isPbPb?"PbPb":"PP")))->setMin(0.0); }
    }
    if (createSPLOTDS) {
      if (importDS && !myws.data(dsName2Fit.c_str())) {
        if (!createCtauDSUsingSPLOT(myws, dsName, parIni, cut, incJpsi, incPsi2S, incBkg)){ cout << "[ERROR] Creating the Ctau Templates using sPLOT failed" << endl; return false; }
      }
    }
  }
  if (importDS && !myws.data(dsName2FitCut.c_str())) {
    // Cut the RooDataSet
    auto ds = std::unique_ptr<RooDataSet>((RooDataSet*)(myws.data(dsName2Fit.c_str())->reduce(parIni.at("CtauRange_Cut").c_str())));
    auto dataToFit = std::unique_ptr<RooDataSet>((RooDataSet*)ds->Clone(dsName2FitCut.c_str()));
    myws.import(*dataToFit, Rename(dsName2FitCut.c_str()));
    const double lostEvents = (myws.data(dsName2Fit.c_str())->numEntries()-myws.data(dsName2FitCut.c_str())->numEntries());
    const double lostPerc = lostEvents*100./(myws.data(dsName2Fit.c_str())->numEntries());
    cout << "[INFO] After applying the cut: " << parIni.at("CtauRange_Cut") << " , we lost " << lostEvents << " events ( " << lostPerc << " % ) " << endl;
  }
  
  if (!loadFitResult) {
    // Set models based on initial parameters
    struct OniaModel model;
    if (!usectauBkgTemplate)
    {
      if (!setCtauModel(model, parIni, isPbPb, incJpsi, incPsi2S, incBkg, incPrompt, incNonPrompt)) { return false; }
    }

    //// LOAD CTAU ERROR PDF
    if (usePerEventError) {
      // Setting extra input information needed by each fitter
      const bool loadCtauErrFitResult = true;
      const bool doCtauErrFit = true;
      const bool importDS = isMC;
      const string DSTAG = Form("DATA_%s", (isPbPb?"PbPb":"PP"));
        
      if ( !fitCharmoniaCtauErrModel( myws, inputWorkspace, cut, parIni, opt, outputDir,
                                      DSTAG, isPbPb, importDS,
                                      doJpsi, doPsi2S, incBkg,
                                      doCtauErrFit, wantPureSMC, loadCtauErrFitResult, inputFitDir, numCores,
                                      setLogScale, incSS, binWidth
                                      )
           ) { return false; }
    }

    // Build the Fit Model
    if (!buildCharmoniaCtauModel(myws, (isPbPb ? model.PbPb : model.PP), parIni, dsName2FitCut, cut, isPbPb, incBkg, incJpsi, incPsi2S, incPrompt, incNonPrompt, useTotctauErrPdf, usectauBkgTemplate, binWidth.at("CTAUSB"), numEntries)) { return false; }

    //// LOAD CTAU RESOLUTION PDF
    if ((fitSideBand || useSPlot) && !usectauBkgTemplate) {
      // check if we have already done the resolution fits. If yes, load their results
      string fileName = "";
      string plotLabel = Form("_CtauRes_%s", parIni[Form("Model_CtauRes_%s", COLL.c_str())].c_str());
      string DSTAG = Form("DATA_%s", (isPbPb?"PbPb":"PP"));
      if (doJpsi && inputFitDir.at("CTAURES").find("MCFits")!=std::string::npos) {
        DSTAG = Form("MCJPSIPR_%s", (isPbPb?"PbPb":"PP"));
        if (inputFitDir.at("CTAURES").find("nonPrompt")!=std::string::npos) DSTAG = Form("MCJPSINOPR_%s", (isPbPb?"PbPb":"PP"));
      }
      else if (doPsi2S && inputFitDir.at("CTAURES").find("MCFits")!=std::string::npos) {
        DSTAG = Form("MCPSI2SPR_%s", (isPbPb?"PbPb":"PP"));
        if (inputFitDir.at("CTAURES").find("nonPrompt")!=std::string::npos) DSTAG = Form("MCPSI2SNOPR_%s", (isPbPb?"PbPb":"PP"));
      }
      if (inputFitDir.at("CTAURES").find("DataFits")!=std::string::npos && inputFitDir.at("CTAURES").find("MCFits")==std::string::npos) DSTAG = Form("DATA_%s", (isPbPb?"PbPb":"PP"));
      setCtauResFileName(fileName, (inputFitDir.at("CTAURES")=="" ? outputDir : inputFitDir.at("CTAURES")), DSTAG, plotLabel, cut, isPbPb);
      if (wantPureSMC) { plotLabel = plotLabel + "_NoBkg"; }
      bool found = false;
      if (!found && gSystem->AccessPathName(fileName.c_str()) && inputFitDir.at("CTAURES")!="") {
        plotLabel = string(Form("_CtauRes_%s_NoBkg", parIni[Form("Model_CtauRes_%s", COLL.c_str())].c_str()));
        setCtauResFileName(fileName, (inputFitDir.at("CTAURES")=="" ? outputDir : inputFitDir.at("CTAURES")), DSTAG, plotLabel, cut, isPbPb);
      } else if (inputFitDir.at("CTAURES")!="") { found = true; }
      if (!found && gSystem->AccessPathName(fileName.c_str()) && inputFitDir.at("CTAURES")!="") {
        plotLabel = Form("_CtauRes_%s", parIni[Form("Model_CtauRes_%s", COLL.c_str())].c_str());
        setCtauResFileName(fileName, outputDir, DSTAG, plotLabel, cut, isPbPb);
      } else if (inputFitDir.at("CTAURES")!="") { found = true; }
      if (!found && gSystem->AccessPathName(fileName.c_str())) {
        plotLabel = string(Form("_CtauRes_%s_NoBkg", parIni[Form("Model_CtauRes_%s", COLL.c_str())].c_str()));
        setCtauResFileName(fileName, outputDir, DSTAG, plotLabel, cut, isPbPb);
      } else { found = true; }
      if (!found && gSystem->AccessPathName(fileName.c_str())) {
        cout << "[ERROR] User Input File : " << fileName << " was not found!" << endl;
        return false;
      }
      if ( !loadPreviousFitResult(myws, fileName, DSTAG, isPbPb, false, false) ) {
        cout << "[ERROR] The ctau resolution fit results were not loaded!" << endl;
        return false;
      } else {
        cout << "[INFO] The ctau resolution fits were found, so I'll load the fit results." << endl; 
      }
      std::vector< std::string > objs = {"Bkg", "Jpsi", "Psi2S"}; bool hasPromptPdf = false;
      for (auto obj : objs) {
        if (myws.pdf(Form("pdfCTAU_%sPR_%s", obj.c_str(), (isPbPb?"PbPb":"PP")))) {
          cout << "[INFO] Setting Prompt " << obj << " parameters to constant!" << endl; hasPromptPdf = true;
          auto par = std::unique_ptr<RooArgSet>(myws.pdf(Form("pdfCTAU_%sPR_%s", obj.c_str(), (isPbPb?"PbPb":"PP")))->getParameters(RooArgSet(*myws.var("ctau"), *myws.var("ctauErr"))));
          par->setAttribAll("Constant", kTRUE); 
        }
      }
      if (!hasPromptPdf) { cout << "[ERROR] Prompt Ctau PDF was not found!" << endl; return false; }
      if (!setConstant(myws, Form("s1_CtauRes_%s", COLL.c_str()), true)) { return false; }
    }

    // save the initial values of the model we've just created
    auto params = std::unique_ptr<RooArgSet>((RooArgSet*) myws.pdf(pdfName.c_str())->getParameters(RooArgSet(*myws.var("ctau"), *myws.var("invMass"), *myws.var("ctauErr"))));
    myws.saveSnapshot((pdfName+"_parIni").c_str(),*params,kTRUE);
  }

  std::unique_ptr<RooArgSet> newpars = std::unique_ptr<RooArgSet>(myws.pdf(pdfName.c_str())->getParameters(*(myws.data(dsName.c_str()))));
  
  if (!usectauBkgTemplate)
  {
    found = found && isFitAlreadyFound(*newpars, fileName, pdfName.c_str());
    if (loadFitResult) {
      if ( loadPreviousFitResult(myws, fileName, DSTAG, isPbPb, false, false) ) { skipFit = true; } else  { skipFit = false; }
      if (skipFit) { cout << "[INFO] This ctau fit was already done, so I'll load the fit results." << endl; }
      myws.saveSnapshot(Form("%s_parLoad", pdfName.c_str()),*newpars,kTRUE);
    } else if (found) {
      cout << "[INFO] This ctau fit was already done, so I'll just go to the next one." << endl;
      return true;
    }
  }

  // Fit the Datasets
  if (skipFit==false) {
    if (!usectauBkgTemplate)
    {
      const bool isWeighted = myws.data(dsName2FitCut.c_str())->isWeighted();
      auto fitResult = std::unique_ptr<RooFitResult>(myws.pdf(pdfName.c_str())->fitTo(*myws.data(dsName2FitCut.c_str()), Extended(kTRUE), NumCPU(numCores), Optimize(kFALSE), SumW2Error(isWeighted), Save()));
      fitResult->Print("v");
      myws.import(*fitResult, Form("fitResult_%s", pdfName.c_str()));
    }
    
    // Draw the mass plot
    drawCtauPlot(myws, outputDir, opt, cut, parIni, plotLabel, DSTAG, isPbPb, incJpsi, incPsi2S, incBkg, incPrompt, incNonPrompt, useSPlot, wantPureSMC, setLogScale, incSS, binWidth.at(fitType.c_str()));
    // Save the results
    string fileName = ""; setCtauFileName(fileName, outputDir, DSTAG, plotLabel, cut, isPbPb, fitSideBand, usectauBkgTemplate);
    myws.saveSnapshot(Form("%s_parFit", pdfName.c_str()),*newpars,kTRUE);
    saveWorkSpace(myws, Form("%sctau%s/%s/result", outputDir.c_str(), (fitSideBand?Form("SB%s",usectauBkgTemplate?"Temp":""):""), DSTAG.c_str()), fileName);
  }

  return true;
};


bool setCtauModel(struct OniaModel& model, map<string, string>& parIni, const bool& isPbPb, const bool& incJpsi,
                  const bool& incPsi2S, const bool& incBkg, const bool& incPrompt, const bool& incNonPrompt)
{
  if (isPbPb) {
    if (parIni.count("Model_CtauRes_PbPb")>0) {
      model.PbPb.CtauRes = CtauModelDictionary[parIni.at("Model_CtauRes_PbPb")];
      if (model.PbPb.CtauRes==CtauModel(0)) {
        cout << "[ERROR] The ctau resolution model: " << parIni.at("Model_CtauRes_PbPb") << " is invalid" << endl; return false;
      }
    } else { 
      cout << "[ERROR] Ctau Resolution model for PbPb was not found in the initial parameters!" << endl; return false;
    }
  } else {
    if (parIni.count("Model_CtauRes_PP")>0) {
      model.PP.CtauRes = CtauModelDictionary[parIni.at("Model_CtauRes_PP")];
      if (model.PP.CtauRes==CtauModel(0)) {
        cout << "[ERROR] The ctau resolution model: " << parIni.at("Model_CtauRes_PP") << " is invalid" << endl; return false;
      }
    } else { 
      cout << "[ERROR] Ctau Resolution model for PP was not found in the initial parameters!" << endl; return false;
    }
  }
  if (isPbPb && incBkg && incNonPrompt) {
    if (parIni.count("Model_BkgNoPR_PbPb")>0) {
      model.PbPb.Bkg.Ctau.NonPrompt = CtauModelDictionary[parIni.at("Model_BkgNoPR_PbPb")];
      if (model.PbPb.Bkg.Ctau.NonPrompt==CtauModel(0)) {
        cout << "[ERROR] The background non-prompt ctau model: " << parIni.at("Model_BkgNoPR_PbPb") << " is invalid" << endl; return false;
      }
    } else { 
      cout << "[ERROR] Background non-prompt ctau model for PbPb was not found in the initial parameters!" << endl; return false;
    }
  }
  if (isPbPb && incBkg && incPrompt) {
    if (parIni.count("Model_BkgPR_PbPb")>0) {
      model.PbPb.Bkg.Ctau.Prompt = CtauModelDictionary[parIni.at("Model_BkgPR_PbPb")];
      if (model.PbPb.Bkg.Ctau.Prompt==CtauModel(0)) {
        cout << "[ERROR] The background prompt ctau model: " << parIni.at("Model_BkgPR_PbPb") << " is invalid" << endl; return false;
      }
    } else {
      parIni["Model_BkgPR_PbPb"] = "Delta";
      model.PbPb.Bkg.Ctau.Prompt=CtauModel::Delta;
    }
  }
  if (isPbPb && incJpsi && incNonPrompt) {
    if (parIni.count("Model_JpsiNoPR_PbPb")>0) {
      model.PbPb.Jpsi.Ctau.NonPrompt = CtauModelDictionary[parIni.at("Model_JpsiNoPR_PbPb")];
      if (model.PbPb.Jpsi.Ctau.NonPrompt==CtauModel(0)) {
        cout << "[ERROR] The Jpsi non-prompt ctau model: " << parIni.at("Model_JpsiNoPR_PbPb") << " is invalid" << endl; return false;
      }
    } else { 
      cout << "[ERROR] Jpsi non-prompt ctau model for PbPb was not found in the initial parameters!" << endl; return false;
    }
  }
  if (isPbPb && incJpsi && incPrompt) {
    if (parIni.count("Model_JpsiPR_PbPb")>0) {
      model.PbPb.Jpsi.Ctau.Prompt = CtauModelDictionary[parIni.at("Model_JpsiPR_PbPb")];
      if (model.PbPb.Jpsi.Ctau.Prompt==CtauModel(0)) {
        cout << "[ERROR] The Jpsi prompt ctau model: " << parIni.at("Model_JpsiPR_PbPb") << " is invalid" << endl; return false;
      }
    } else { 
      parIni["Model_JpsiPR_PbPb"] = "Delta";
      model.PbPb.Jpsi.Ctau.Prompt=CtauModel::Delta;
    }
  }
  if (isPbPb && incPsi2S && incNonPrompt) {
    if (parIni.count("Model_Psi2SNoPR_PbPb")>0) {
      model.PbPb.Psi2S.Ctau.NonPrompt = CtauModelDictionary[parIni.at("Model_Psi2SNoPR_PbPb")];
      if (model.PbPb.Psi2S.Ctau.NonPrompt==CtauModel(0)) {
        cout << "[ERROR] The psi(2S) non-prompt ctau model: " << parIni.at("Model_Psi2SNoPR_PbPb") << " is invalid" << endl; return false;
      }
    } else { 
      cout << "[ERROR] psi(2S) non-prompt ctau model for PbPb was not found in the initial parameters!" << endl; return false;
    }
  }
  if (isPbPb && incPsi2S && incPrompt) {
    if (parIni.count("Model_Psi2SPR_PbPb")>0) {
      model.PbPb.Psi2S.Ctau.Prompt = CtauModelDictionary[parIni.at("Model_Psi2SPR_PbPb")];
      if (model.PbPb.Psi2S.Ctau.Prompt==CtauModel(0)) {
        cout << "[ERROR] The psi(2S) prompt ctau model: " << parIni.at("Model_Psi2SPR_PbPb") << " is invalid" << endl; return false;
      }
    } else { 
      parIni["Model_Psi2SPR_PbPb"] = "Delta";
      model.PbPb.Psi2S.Ctau.Prompt=CtauModel::Delta;
    }
  }
  if (!isPbPb && incBkg && incNonPrompt) {
    if (parIni.count("Model_BkgNoPR_PP")>0) {
      model.PP.Bkg.Ctau.NonPrompt = CtauModelDictionary[parIni.at("Model_BkgNoPR_PP")];
      if (model.PP.Bkg.Ctau.NonPrompt==CtauModel(0)) {
        cout << "[ERROR] The background non-prompt ctau model: " << parIni.at("Model_BkgNoPR_PP") << " is invalid" << endl; return false;
      }
    } else { 
      cout << "[ERROR] Background non-prompt ctau model for PP was not found in the initial parameters!" << endl; return false;
    }
  }
  if (!isPbPb && incBkg && incPrompt) {
    if (parIni.count("Model_BkgPR_PP")>0) {
      model.PP.Bkg.Ctau.Prompt = CtauModelDictionary[parIni.at("Model_BkgPR_PP")];
      if (model.PP.Bkg.Ctau.Prompt==CtauModel(0)) {
        cout << "[ERROR] The background prompt ctau model: " << parIni.at("Model_BkgPR_PP") << " is invalid" << endl; return false;
      }
    } else { 
      parIni["Model_BkgPR_PP"] = "Delta";
      model.PP.Bkg.Ctau.Prompt=CtauModel::Delta;
    }
  }
  if (!isPbPb && incJpsi && incNonPrompt) {
    if (parIni.count("Model_JpsiNoPR_PP")>0) {
      model.PP.Jpsi.Ctau.NonPrompt = CtauModelDictionary[parIni.at("Model_JpsiNoPR_PP")];
      if (model.PP.Jpsi.Ctau.NonPrompt==CtauModel(0)) {
        cout << "[ERROR] The Jpsi non-prompt ctau model: " << parIni.at("Model_JpsiNoPR_PP") << " is invalid" << endl; return false;
      }
    } else {
      cout << "[ERROR] Jpsi non-prompt ctau model for PP was not found in the initial parameters!" << endl; return false;
    }
  }
  if (!isPbPb && incJpsi && incPrompt) {
    if (parIni.count("Model_JpsiPR_PP")>0) {
      model.PP.Jpsi.Ctau.Prompt = CtauModelDictionary[parIni.at("Model_JpsiPR_PP")];
      if (model.PP.Jpsi.Ctau.Prompt==CtauModel(0)) {
        cout << "[ERROR] The Jpsi prompt ctau model: " << parIni.at("Model_JpsiPR_PP") << " is invalid" << endl; return false;
      }
    } else { 
      parIni["Model_JpsiPR_PP"] = "Delta";
      model.PP.Jpsi.Ctau.Prompt=CtauModel::Delta;
    }
  }
  if (!isPbPb && incPsi2S && incNonPrompt) {
    if (parIni.count("Model_Psi2SNoPR_PP")>0) {
      model.PP.Psi2S.Ctau.NonPrompt = CtauModelDictionary[parIni.at("Model_Psi2SNoPR_PP")];
      if (model.PP.Psi2S.Ctau.NonPrompt==CtauModel(0)) {
        cout << "[ERROR] The psi(2S) non-prompt ctau model: " << parIni.at("Model_Psi2SNoPR_PP") << " is invalid" << endl; return false;
      }
    } else { 
      cout << "[ERROR] psi(2S) non-prompt ctau model for PP was not found in the initial parameters!" << endl; return false;
    }
  }
  if (!isPbPb && incPsi2S && incPrompt) {
    if (parIni.count("Model_Psi2SPR_PP")>0) {
      model.PP.Psi2S.Ctau.Prompt = CtauModelDictionary[parIni.at("Model_Psi2SPR_PP")];
      if (model.PP.Psi2S.Ctau.Prompt==CtauModel(0)) {
        cout << "[ERROR] The psi(2S) prompt ctau model: " << parIni.at("Model_Psi2SPR_PP") << " is invalid" << endl; return false;
      }
    } else { 
      parIni["Model_Psi2SPR_PP"] = "Delta";
      model.PP.Psi2S.Ctau.Prompt=CtauModel::Delta;
    }
  }

  return true;
};


void setCtauGlobalParameterRange(RooWorkspace& myws, map<string, string>& parIni, struct KinCuts& cut, const string& label, const double& binWidth, const bool optimizeRange, const bool is2DFit)
{
  const bool isPbPb = (label.find("PbPb")!=std::string::npos);
  double ctauMax , ctauMin;
  myws.data(Form("dOS_%s", label.c_str()))->getRange(*myws.var("ctau"), ctauMin, ctauMax);
  ctauMin -= 0.00001;  ctauMax += 0.00001;
  const int nBins = min(int( round((ctauMax - ctauMin)/binWidth) ), 1000);
  if (optimizeRange) {
    auto ds = std::unique_ptr<RooDataSet>((RooDataSet*)myws.data(Form("dOS_%s", label.c_str()))->Clone("dTMP"));
    auto hTot = std::unique_ptr<TH1D>((TH1D*)(ds->createHistogram("hTMP", *myws.var("ctau"), Binning(nBins, ctauMin, ctauMax))));
    vector<double> rangeCtau; 
    getRange(rangeCtau, *hTot, 4);
    ctauMin = rangeCtau[0];
    if (ctauMin<cut.dMuon.ctau.Min) { ctauMin = cut.dMuon.ctau.Min; }
    if (ctauMax>cut.dMuon.ctau.Max) { ctauMax = cut.dMuon.ctau.Max; }
    if (ctauMin < -4.0) { ctauMin = -4.0; }
    ctauMax =  7.0;
  }
  else if (!is2DFit && parIni.count(Form("ctauCut_%s",isPbPb?"PbPb":"PP"))>0 && parIni.at(Form("ctauCut_%s",isPbPb?"PbPb":"PP"))!="") {
    parIni.at(Form("ctauCut_%s",isPbPb?"PbPb":"PP")).erase(parIni.at(Form("ctauCut_%s",isPbPb?"PbPb":"PP")).find("["), string("[").length());
    parIni.at(Form("ctauCut_%s",isPbPb?"PbPb":"PP")).erase(parIni.at(Form("ctauCut_%s",isPbPb?"PbPb":"PP")).find("]"), string("]").length());
    TString sctauCut(parIni.at(Form("ctauCut_%s",isPbPb?"PbPb":"PP")).c_str());
    auto actauCut = std::unique_ptr<TObjArray>(sctauCut.Tokenize(","));
    ctauMin = (static_cast<TObjString*>(actauCut->At(0)))->GetString().Atof();
    ctauMax = (static_cast<TObjString*>(actauCut->At(1)))->GetString().Atof();
  }
  cout << "[INFO] Range from data: ctauMin: " << ctauMin << "  ctauMax: " << ctauMax << endl;
  myws.var("ctau")->setRange("CtauWindow", ctauMin, ctauMax);
  parIni["CtauRange_Cut"] = Form("(%.12f <= ctau && ctau < %.12f)", ctauMin, ctauMax);
  cut.dMuon.ctau.Max = ctauMax;
  cut.dMuon.ctau.Min = ctauMin;
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


void setCtauFileName(string& fileName, const string& outputDir, string TAG, const string& plotLabel, const struct KinCuts& cut, const bool& isPbPb, const bool& fitSideBand, const bool& usectauBkgTemplate)
{
  if (TAG.find("_")!=std::string::npos) TAG.erase(TAG.find("_"));
  if (!usectauBkgTemplate) {fileName = Form("%sctau%s/%s/result/FIT_%s_%s_%s%s_pt%.0f%.0f_rap%.0f%.0f_cent%d%d.root", outputDir.c_str(), (fitSideBand?"SB":""), TAG.c_str(), "CTAU", TAG.c_str(), (isPbPb?"PbPb":"PP"), plotLabel.c_str(), (cut.dMuon.Pt.Min*10.0), (cut.dMuon.Pt.Max*10.0), (cut.dMuon.AbsRap.Min*10.0), (cut.dMuon.AbsRap.Max*10.0), cut.Centrality.Start, cut.Centrality.End);}
  else {fileName = Form("%sctau%sTemp/%s/result/FIT_%s_%s_%s_%s_pt%.0f%.0f_rap%.0f%.0f_cent%d%d.root", outputDir.c_str(), (fitSideBand?"SB":""), TAG.c_str(), "CTAU", TAG.c_str(), (isPbPb?"PbPb":"PP"),
                        "Bkg", (cut.dMuon.Pt.Min*10.0), (cut.dMuon.Pt.Max*10.0), (cut.dMuon.AbsRap.Min*10.0), (cut.dMuon.AbsRap.Max*10.0), cut.Centrality.Start, cut.Centrality.End);}
  return;
};
 

void setCtauCutParameters(struct KinCuts& cut, const bool& incNonPrompt)
{
  // Define the ctau range
  if (cut.dMuon.ctau.Min==-1000. && cut.dMuon.ctau.Max==1000.) { 
    // Default ctau values, means that the user did not specify a ctau range
    if (incNonPrompt) {
      cut.dMuon.ctau.Min = -30.0;
      cut.dMuon.ctau.Max = 100.0;
    } else {
      cut.dMuon.ctau.Min = -2.0;
      cut.dMuon.ctau.Max = 2.0;
    }
  }        
  cout << "[INFO] Setting ctau range to min: " << cut.dMuon.ctau.Min << " and max " << cut.dMuon.ctau.Max << endl;
  return;
};


bool createCtauDSUsingSPLOT(RooWorkspace& ws, const string& dsName, const map<string, string>&  parIni, const struct KinCuts& cut, const bool& incJpsi, const bool& incPsi2S, const bool& incBkg)
{
  bool isPbPb = false;
  if (dsName.find("PbPb")!=std::string::npos) { isPbPb = true; }
  if (dsName.find("MC")!=std::string::npos)   { return false;  }  // Only accept data
  //
  if (!ws.data((dsName+"_SPLOT").c_str())) {
    const string pdfMassName = Form("pdfMASS_Tot_%s", (isPbPb?"PbPb":"PP"));
    RooArgList yieldList;
    yieldList.add( *ws.var(Form("N_Jpsi_%s", (isPbPb?"PbPb":"PP"))) );
    yieldList.add( *ws.var(Form("N_Bkg_%s", (isPbPb?"PbPb":"PP"))) ); // Always add background
    auto data = std::unique_ptr<RooDataSet>((RooDataSet*)ws.data(dsName.c_str())->Clone("TMP_DATA"));
    auto pdf = std::unique_ptr<RooAbsPdf>(clone(*ws.pdf(pdfMassName.c_str())));
    RooStats::SPlot sData = RooStats::SPlot("sData","An SPlot", *data, pdf.get(), yieldList);
    ws.import(*data, Rename((dsName+"_SPLOT").c_str()));
    cout <<  "[INFO] Bkg yield -> Mass Fit: " << ws.var(Form("N_Bkg_%s", (isPbPb?"PbPb":"PP")))->getVal() << " , sWeights: " << sData.GetYieldFromSWeight(Form("N_Bkg_%s", (isPbPb?"PbPb":"PP"))) << std::endl;
    if (!isCompatibleDataset(*data, *(RooDataSet*)ws.data(dsName.c_str()))){ cout << "[ERROR] sPlot and original Datasets are inconsistent!" << endl; return false; }
  }
  if (incBkg) {
    auto dataw_Bkg = std::unique_ptr<RooDataSet>(new RooDataSet("TMP_BKG_DATA","TMP_BKG_DATA", (RooDataSet*)ws.data((dsName+"_SPLOT").c_str()), RooArgSet(*ws.var("invMass"), *ws.var("ctau"), *ws.var("ctauN"), *ws.var("ctauErr"), *ws.var(Form("N_Bkg_%s_sw", (isPbPb?"PbPb":"PP")))), 0, Form("N_Bkg_%s_sw", (isPbPb?"PbPb":"PP"))));
    ws.import(*dataw_Bkg, Rename((dsName+"_BKG").c_str()));
    cout <<  "[INFO] sPLOT_BKG_DS Events: " << ws.data((dsName+"_BKG").c_str())->sumEntries() << std::endl;
  }
  if (incJpsi) {
    auto dataw_Sig = std::unique_ptr<RooDataSet>(new RooDataSet("TMP_JPSI_DATA","TMP_JPSI_DATA", (RooDataSet*)ws.data((dsName+"_SPLOT").c_str()), RooArgSet(*ws.var("invMass"), *ws.var("ctau"), *ws.var("ctauN"), *ws.var("ctauErr"), *ws.var(Form("N_Jpsi_%s_sw", (isPbPb?"PbPb":"PP")))), 0, Form("N_Jpsi_%s_sw", (isPbPb?"PbPb":"PP"))));
    ws.import(*dataw_Sig, Rename((dsName+"_JPSI").c_str()));
    cout <<  "[INFO] sPLOT_JPSI_DS Events: " << ws.data((dsName+"_JPSI").c_str())->sumEntries() << std::endl;
  }
  if (incPsi2S) {
    auto dataw_Sig = std::unique_ptr<RooDataSet>(new RooDataSet("TMP_PSI2S_DATA","TMP_PSI2S_DATA", (RooDataSet*)ws.data((dsName+"_SPLOT").c_str()), RooArgSet(*ws.var("invMass"), *ws.var("ctau"), *ws.var("ctauN"), *ws.var("ctauErr"), *ws.var(Form("N_Psi2S_%s_sw", (isPbPb?"PbPb":"PP")))), 0, Form("N_Psi2S_%s_sw", (isPbPb?"PbPb":"PP"))));
    ws.import(*dataw_Sig, Rename((dsName+"_PSI2S").c_str()));
    cout <<  "[INFO] sPLOT_PSI2S_DS Events: " << ws.data((dsName+"_PSI2S").c_str())->sumEntries() << std::endl;
  }
  return true;
};


#endif // #ifndef fitCharmoniaCtauModel_C
