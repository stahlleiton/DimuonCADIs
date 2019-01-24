#ifndef fitCharmoniaCtauMassModel_C
#define fitCharmoniaCtauMassModel_C

#include "Utilities/initClasses.h"
#include "fitCharmoniaMassModel.C"
#include "fitCharmoniaCtauModel.C"
#include "fitCharmoniaCtauErrModel.C"
#include "fitCharmoniaCtauTrueModel.C"
#include "fitCharmoniaCtauRecoModel.C"
#include "drawMassFrom2DPlot.C"
#include "drawCtauFrom2DPlot.C"
#include "drawCtauMass2DPlot.C"

void setCtauMassFileName ( string& fileName, const string& outputDir, string TAG, const string& plotLabel, const struct KinCuts& cut, const bool& isPbPb );


bool fitCharmoniaCtauMassModel( RooWorkspace& myws,             // Local Workspace
                                const RooWorkspace& inputWorkspace,  // Workspace with all the input RooDatasets
                                      struct KinCuts& cut,           // Variable containing all kinematic cuts
                                      map<string, string>&  parIni,  // Variable containing all initial parameters
                                const struct InputOpt& opt,          // Variable with run information (kept for legacy purpose)
                                const string& outputDir,             // Path to output directory
                                // Select the type of datasets to fit
                                      string DSTAG,                  // Specifies the type of datasets: i.e, DATA, MCJPSINP, ...
                                const bool& isPbPb,                  // isPbPb = false for pp, true for PbPb
                                // Select the type of object to fit
                                const bool incJpsi,                  // Includes Jpsi model
                                const bool incPsi2S,                 // Includes Psi(2S) model
                                // Select the fitting options
                                const bool& useTotctauErrPdf,        // If yes use the total ctauErr PDF instead of Jpsi and bkg ones
                                const bool& usectauBkgTemplate,      // If yes use a template for Bkg ctau instead of the fitted Pdf
                               const  bool& useCtauRecoPdf,          // If yes use the ctauReco PDF (template) instead of ctauTrue one
                                const map<string, string>& inputFitDir,// Location of the fit results
                                const int&  numCores,                // Number of cores used for fitting
                                // Select the drawing options
                                const bool& setLogScale,             // Draw plot with log scale
                                const bool& incSS,                   // Include Same Sign data
                                const map<string, double>& binWidth  // Bin width used for plotting
                                )
{
  
  if (DSTAG.find("_")!=std::string::npos) DSTAG.erase(DSTAG.find("_"));

  // Check if input dataset is MC
  if (DSTAG.find("MC")!=std::string::npos) {
    cout << "[ERROR] We can not make 2D fits on MC, change your input settings!" << endl; return false;
  }
  // Check if user has selected an object
  if (!incJpsi && !incPsi2S) {
    cout << "[ERROR] We need to include signal to perform 2D fits, change your input settings!" << endl; return false;
  }

  const string pdfType = "pdfCTAUMASS";
  const string COLL = (isPbPb ? "PbPb" : "PP" );
  const string pdfName = Form("%s_Tot_%s", pdfType.c_str(), COLL.c_str());

  const bool isMC          = false;
  const bool incBkg        = true;      // Includes Background model
  const bool incPrompt     = true;      // Includes Prompt ctau model
  const bool incNonPrompt  = true;     // Includes NonPrompt ctau model
  const bool fitSideBand   = false;
  const bool usePerEventError = true;
  const bool wantPureSMC = false;
  const bool fitMass = true;
  const bool fitCtau = true;

  // Set all the cuts on data
  setMassCutParameters(cut, incJpsi, incPsi2S, isMC, true);
  if (usePerEventError) {
    // check if we have already done the ctauErr fits. If yes, load their parameters
    string fileName = "";
    const string pdfName = Form("pdfCTAUERR_Tot_%s", COLL.c_str());
    string plotLabel = "";
    const bool fitSideBand = false;
    if (incJpsi)  { plotLabel = plotLabel + "_Jpsi";     }
    if (incPsi2S) { plotLabel = plotLabel + "_Psi2S";    }
    plotLabel = plotLabel + "_Bkg";
    setCtauErrFileName(fileName, (inputFitDir.at("CTAUERR")=="" ? outputDir : inputFitDir.at("CTAUERR")), "DATA", plotLabel, cut, isPbPb, fitSideBand);
    bool foundFit = false;
    if ( loadCtauErrRange(fileName, cut) ) { foundFit = true; }
    if (foundFit) { cout << "[INFO] The ctauErr fit was found and I'll load the ctau Error range used." << endl; }
    else { cout << "[ERROR] The ctauErr fit was not found!" << endl; return false; }
  }
  setCtauErrCutParameters(cut);
  setCtauCutParameters(cut, incNonPrompt);

  // Import the local datasets
  const string label = ((DSTAG.find(COLL.c_str())!=std::string::npos) ? DSTAG.c_str() : Form("%s_%s", DSTAG.c_str(), COLL.c_str()));
  const string dsName = Form("dOS_%s", label.c_str());
  if ( !(myws.data(dsName.c_str())) ) {
    const int importID = importDataset(myws, inputWorkspace, cut, label, fitSideBand);
    if (importID<0) { return false; }
    else if (importID==0) { cout << "[ERROR] Local dataset failed to import!" << endl; return false; }
  }
  const double numEntries = myws.data(dsName.c_str())->sumEntries(); 
  if ((numEntries<=0) || !(myws.data(dsName.c_str()))) { cout << "[ERROR] No local dataset was found to perform the fit!" << endl; return false; }
 
  // Set global parameters
  setMassGlobalParameterRange(myws, parIni, cut, incJpsi, incPsi2S, incBkg, false);
  setCtauErrGlobalParameterRange(myws, parIni, cut, "", binWidth.at("CTAUERR"), true);
  setCtauGlobalParameterRange(myws, parIni, cut, label, binWidth.at("CTAU"), (usectauBkgTemplate&&!isPbPb),true);

  // Cut the RooDataSet
  const string dsNameCut = dsName+"_CTAUCUT";
  auto ds = std::unique_ptr<RooDataSet>((RooDataSet*)(myws.data(dsName.c_str())->reduce(parIni.at("CtauRange_Cut").c_str())));
  auto dataToFit = std::unique_ptr<RooDataSet>((RooDataSet*)ds->Clone(dsNameCut.c_str()));
  myws.import(*dataToFit, Rename(dsNameCut.c_str()));
  const double lostEvents = (myws.data(dsName.c_str())->numEntries()-myws.data(dsNameCut.c_str())->numEntries());
  const double lostPerc = lostEvents*100./(myws.data(dsName.c_str())->numEntries());
  cout << "[INFO] After applying the cut: " << parIni.at("CtauRange_Cut") << " , we lost " << lostEvents << " events ( " << lostPerc << " % ) " << endl;
  
  // Set models based on initial parameters
  struct OniaModel model;
  if (!setCtauModel(model, parIni, isPbPb, incJpsi, incPsi2S, incBkg, incPrompt, incNonPrompt)) { return false; }

  string plotLabel = "";
  const map<string, bool> plotLabels = {{"Jpsi",      (incJpsi)},
                                        {"Psi2S",     (incPsi2S)},
                                        {"Bkg",       (incBkg)},
                                        {"JpsiNoPR",  (incJpsi&&incNonPrompt)}, 
                                        {"Psi2SNoPR", (incPsi2S&&incNonPrompt)}, 
                                        {"BkgNoPR",   (incBkg&&incNonPrompt)}, 
                                        {"CtauRes",   (true)}};
  for (auto const& lbl : plotLabels) {
    const string& obj = lbl.first;
    const bool& cond = lbl.second;
    if (cond && parIni.count(Form("Model_%s_%s", obj.c_str(), COLL.c_str()))>0) { 
      plotLabel = plotLabel + Form("_%s_%s", obj.c_str(), parIni.at(Form("Model_%s_%s", obj.c_str(), COLL.c_str())).c_str()); 
    }
  }

  string fileName = "";
  setCtauMassFileName(fileName, outputDir, DSTAG, plotLabel, cut, isPbPb);

  //// LOAD MASS PDF
  if (fitMass) {
    // Setting extra input information needed by each fitter
    const string& iMassFitDir = inputFitDir.at("MASS");
    const double& ibWidth = binWidth.at("MASS");
    const uint loadMassFitResult = (loadYields(myws, fileName, dsName, pdfName) ? 2 : 1);
    const bool doMassFit = true;
    const bool importDS = false;
    const bool getMeanPT = false;
    const bool zoomPsi = false;
    const string applyCorr = "";
    const bool doSimulFit = false;
    const bool cutCtau = false;
    const bool doConstrFit = false;

    if ( !fitCharmoniaMassModel( myws, inputWorkspace, cut, parIni, opt, outputDir, 
                                 DSTAG, isPbPb, importDS,
                                 incJpsi, incPsi2S, true, 
                                 doMassFit, cutCtau, doConstrFit, doSimulFit, wantPureSMC, applyCorr, loadMassFitResult, iMassFitDir, numCores, 
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
    for (auto obj : objs) { if (myws.var(Form("N_%s_%s", obj.c_str(), (isPbPb?"PbPb":"PP"))))  setConstant( myws, Form("N_%s_%s", obj.c_str(), (isPbPb?"PbPb":"PP")), false); }
    // Let's set the minimum value of the yields to zero
    for (auto obj : objs) { if (myws.var(Form("N_%s_%s", obj.c_str(), (isPbPb?"PbPb":"PP")))) myws.var(Form("N_%s_%s", obj.c_str(), (isPbPb?"PbPb":"PP")))->setMin(0.0); }
    // Let's constrain the Number of Signal events to the updated mass fit results
    RooArgSet pdfList = RooArgSet("ConstrainPdfList");
    for (auto obj : objs) {
      if (myws.var(Form("N_%s_%s", obj.c_str(), (isPbPb?"PbPb":"PP"))))  {
        myws.factory(Form("Gaussian::%s_Gauss(%s,%s_Mean[%f],%s_Sigma[%f])", Form("N_%s_%s", obj.c_str(), (isPbPb?"PbPb":"PP")), Form("N_%s_%s", obj.c_str(), (isPbPb?"PbPb":"PP")),
                          Form("N_%s_%s", obj.c_str(), (isPbPb?"PbPb":"PP")), myws.var(Form("N_%s_%s", obj.c_str(), isPbPb?"PbPb":"PP"))->getValV(),
                          Form("N_%s_%s", obj.c_str(), (isPbPb?"PbPb":"PP")) , myws.var(Form("N_%s_%s", obj.c_str(), isPbPb?"PbPb":"PP"))->getError()));
        pdfList.add(*myws.pdf(Form("N_%s_%s_Gauss", obj.c_str(), (isPbPb?"PbPb":"PP"))), kFALSE);
        std::cout << "[INFO] Constraining N_" << obj << " with Mean : " << myws.var(Form("N_%s_%s_Mean", obj.c_str(), (isPbPb?"PbPb":"PP")))->getVal()
                  << " and Sigma: " << myws.var(Form("N_%s_%s_Sigma", obj.c_str(), (isPbPb?"PbPb":"PP")))->getVal() << std::endl;
      }
    }
    myws.defineSet("ConstrainPdfList", pdfList);
    if (!myws.set("ConstrainPdfList")) { std::cout << "[ERROR] No constrain pdf list was found" << std::endl; return false; }
  }
  
  //// LOAD CTAU ERROR PDF
  if (usePerEventError) {
    // Setting extra input information needed by each fitter
    const bool loadCtauErrFitResult = true;
    const bool doCtauErrFit = false;
    const bool importDS = false;
    const bool wantPureSMC = false;
    
    if ( !fitCharmoniaCtauErrModel( myws, inputWorkspace, cut, parIni, opt, outputDir, 
                                    DSTAG, isPbPb, importDS, 
                                    incJpsi, incPsi2S, incBkg, 
                                    doCtauErrFit, wantPureSMC, loadCtauErrFitResult, inputFitDir, numCores, 
                                    setLogScale, incSS, binWidth
                                    ) 
         ) { return false; }
  }

  //// LOAD CTAU SIDEBAND PDF
  if (fitCtau) {
    // check if we have already done the sideband fits. If yes, load their results
    string fileName = "";
    const bool fitSB = true;
    const string plotLabel = Form("_BkgNoPR_%s_CtauRes_%s", parIni[Form("Model_BkgNoPR_%s", COLL.c_str())].c_str(), parIni[Form("Model_CtauRes_%s", COLL.c_str())].c_str());
    const string DSTAG = Form("DATA_%s", (isPbPb?"PbPb":"PP"));
    setCtauFileName(fileName, (inputFitDir.at("CTAUSB")=="" ? outputDir : inputFitDir.at("CTAUSB")), DSTAG, plotLabel, cut, isPbPb, fitSB, usectauBkgTemplate);
    bool found = false;
    if (!found && gSystem->AccessPathName(fileName.c_str()) && inputFitDir.at("CTAUSB")!="") {
      setCtauFileName(fileName, outputDir, DSTAG, plotLabel, cut, isPbPb, fitSB, usectauBkgTemplate);
    } else if (inputFitDir.at("CTAUSB")!="") { found = true; }
    if (!found && gSystem->AccessPathName(fileName.c_str())) {
      cout << "[ERROR] User Input File : " << fileName << " was not found!" << endl;
      return false;
    }
    if (usectauBkgTemplate) {
      vector<string> pdfNames; pdfNames.push_back(Form("pdfCTAUCOND_Bkg_%s", COLL.c_str()));
      if ( !isPdfAlreadyFound(myws, fileName, pdfNames, true) ) {
        cout << "[ERROR] The ctau sideband fit results were not loaded!" << endl;
        return false;
      } else {
        cout << "[INFO] The ctau sideband fits were found, so I'll load the fit results." << endl; 
      }
    }
  }

  //// LOAD CTAU RECO PDF RESULTS
  if (fitCtau) {
    if (useCtauRecoPdf) {
      // check if we have already done the ctau Reco fits. If yes, load their results
      string fileName = "";
      string plotLabel = "_CtauReco";
      const string DSTAG = Form("MCJPSINOPR_%s", (isPbPb?"PbPb":"PP"));
      const string pdfName = Form("pdfCTAU_%s_%s", incJpsi?"JpsiNoPR":"Psi2SNoPR", (isPbPb?"PbPb":"PP"));
      const string newPdfName = Form("pdfCTAUCOND_%s_%s", incJpsi?"JpsiNoPR":"Psi2SNoPR", (isPbPb?"PbPb":"PP"));
      setCtauRecoFileName(fileName, (inputFitDir.at("CTAURECO")=="" ? outputDir : inputFitDir.at("CTAURECO")), DSTAG, plotLabel, cut, isPbPb);
      bool found = false;
      if (!found && gSystem->AccessPathName(fileName.c_str()) && inputFitDir.at("CTAURECO")!="") {
        plotLabel = "_CtauReco_NoBkg";
        setCtauRecoFileName(fileName, (inputFitDir.at("CTAURECO")=="" ? outputDir : inputFitDir.at("CTAURECO")), DSTAG, plotLabel, cut, isPbPb);
      } else if (inputFitDir.at("CTAURECO")!="") { found = true; }
      if (!found && gSystem->AccessPathName(fileName.c_str()) && inputFitDir.at("CTAURECO")!="") {
        plotLabel = "_CtauReco";
        setCtauRecoFileName(fileName, outputDir, DSTAG, plotLabel, cut, isPbPb);
      } else if (inputFitDir.at("CTAURECO")!="") { found = true; }
      if (!found && gSystem->AccessPathName(fileName.c_str())) {
        plotLabel = "_CtauReco_NoBkg";
        setCtauRecoFileName(fileName, outputDir, DSTAG, plotLabel, cut, isPbPb);
      } else { found = true; }
      if (!found && gSystem->AccessPathName(fileName.c_str())) {
        cout << "[ERROR] User Input File : " << fileName << " was not found!" << endl;
        return false;
      }
      if ( !isCtauRecoPdfAlreadyFound(myws, fileName, pdfName, true, newPdfName ) ) {
        cout << "[ERROR] The ctau reco template results were not loaded!" << endl;
        return false;
      } else {
        cout << "[INFO] The ctau reco templates were found, so I'll load them." << endl; 
      }
    }
  }

  // Build the Fit Model
  if (!buildCharmoniaCtauModel(myws, (isPbPb ? model.PbPb : model.PP), parIni, dsName, cut, isPbPb, incBkg, incJpsi, incPsi2S, incPrompt, incNonPrompt, useTotctauErrPdf, usectauBkgTemplate, binWidth.at("CTAUSB"), numEntries))  { return false; }

  //// LOAD CTAU RESOLUTION PDF
  if (fitCtau && usectauBkgTemplate) {
    // check if we have already done the resolution fits. If yes, load their results
    string fileName = "";
    string plotLabel = Form("_CtauRes_%s", parIni[Form("Model_CtauRes_%s", COLL.c_str())].c_str());
    string DSTAG = Form("MCJPSIPR_%s", (isPbPb?"PbPb":"PP"));
    if (inputFitDir.at("CTAURES").find("nonPrompt")!=std::string::npos) DSTAG = Form("MCJPSINOPR_%s", (isPbPb?"PbPb":"PP"));
    if (inputFitDir.at("CTAURES").find("DataFits")!=std::string::npos) DSTAG = Form("DATA_%s", (isPbPb?"PbPb":"PP"));
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
    auto par = std::unique_ptr<RooArgSet>(myws.pdf(Form("pdfCTAU_JpsiPR_%s", (isPbPb?"PbPb":"PP")))->getParameters(RooArgSet(*myws.var("ctau"), *myws.var("ctauErr"))));
    par->setAttribAll("Constant", kTRUE);
  }

  //// LOAD CTAU SIDEBAND PDF
  if (fitCtau) {
    // check if we have already done the sideband fits. If yes, load their results
    string fileName = "";
    const bool fitSB = true;
    const string plotLabel = Form("_BkgNoPR_%s_CtauRes_%s", parIni[Form("Model_BkgNoPR_%s", COLL.c_str())].c_str(), parIni[Form("Model_CtauRes_%s", COLL.c_str())].c_str());
    const string DSTAG = Form("DATA_%s", (isPbPb?"PbPb":"PP"));
    setCtauFileName(fileName, (inputFitDir.at("CTAUSB")=="" ? outputDir : inputFitDir.at("CTAUSB")), DSTAG, plotLabel, cut, isPbPb, fitSB, usectauBkgTemplate);
    bool found = false;
    if (!found && gSystem->AccessPathName(fileName.c_str()) && inputFitDir.at("CTAUSB")!="") {
      setCtauFileName(fileName, outputDir, DSTAG, plotLabel, cut, isPbPb, fitSB, usectauBkgTemplate);
    } else if (inputFitDir.at("CTAUSB")!="") { found = true; }
    if (!found && gSystem->AccessPathName(fileName.c_str())) {
      cout << "[ERROR] User Input File : " << fileName << " was not found!" << endl;
      return false;
    }
    if (!usectauBkgTemplate) {
      if ( !loadPreviousFitResult(myws, fileName, DSTAG, isPbPb, false, false) ) {
        cout << "[ERROR] The ctau sideband fit results were not loaded!" << endl;
        return false;
      } else {
        cout << "[INFO] The ctau sideband fits were found, so I'll load the fit results." << endl;
      }
      if (myws.pdf(Form("pdfCTAU_BkgPR_%s", (isPbPb?"PbPb":"PP")))) {
        cout << "[INFO] Setting Prompt Background parameters to constant!" << endl;
        auto par = std::unique_ptr<RooArgSet>(myws.pdf(Form("pdfCTAU_BkgPR_%s", (isPbPb?"PbPb":"PP")))->getParameters(RooArgSet(*myws.var("ctau"), *myws.var("ctauErr"))));
        par->setAttribAll("Constant", kTRUE); 
      } else { cout << "[ERROR] Prompt Background PDF was not found!" << endl; return false; }
      if (myws.pdf(Form("pdfCTAU_BkgNoPR_%s", (isPbPb?"PbPb":"PP")))) {
        cout << "[INFO] Setting NonPrompt Background parameters to constant!" << endl;
        auto par = std::unique_ptr<RooArgSet>(myws.pdf(Form("pdfCTAU_BkgNoPR_%s", (isPbPb?"PbPb":"PP")))->getParameters(RooArgSet(*myws.var("ctau"), *myws.var("ctauErr"))));
        par->setAttribAll("Constant", kTRUE); 
      } else { cout << "[ERROR] NonPrompt Background PDF was not found!" << endl; return false; }
      if (!setConstant(myws, Form("b_Bkg_%s", COLL.c_str()), true)) { return false; }
      if (!setConstant(myws, Form("s1_CtauRes_%s", COLL.c_str()), true)) { return false; }
    }
  }

  //// LOAD CTAU TRUE PDF RESULTS
  if (fitCtau) {
    if (!useCtauRecoPdf) {
      // check if we have already done the ctau true fits. If yes, load their results
      string fileName = "";
      string modelName = Form("Model_JpsiNoPR_%s", COLL.c_str());;
      if (incPsi2S && !incJpsi) { modelName = Form("Model_Psi2SNoPR_%s", COLL.c_str()); }
      string plotLabel = Form("_CtauTrue_%s", parIni[modelName.c_str()].c_str());
      string DSTAG = Form("MCJPSINOPR_%s", (isPbPb?"PbPb":"PP"));
      if (incPsi2S && !incJpsi) { DSTAG = Form("MCPSI2SNOPR_%s", (isPbPb?"PbPb":"PP")); }
      setCtauTrueFileName(fileName, (inputFitDir.at("CTAUTRUE")=="" ? outputDir : inputFitDir.at("CTAUTRUE")), DSTAG, plotLabel, cut, isPbPb);
      bool found = false;
      if (!found && gSystem->AccessPathName(fileName.c_str()) && inputFitDir.at("CTAUTRUE")!="") {
        plotLabel = string(Form("_CtauTrue_%s_NoBkg", parIni[modelName.c_str()].c_str()));
        setCtauTrueFileName(fileName, (inputFitDir.at("CTAUTRUE")=="" ? outputDir : inputFitDir.at("CTAUTRUE")), DSTAG, plotLabel, cut, isPbPb);
      } else if (inputFitDir.at("CTAUTRUE")!="") { found = true; }
      if (!found && gSystem->AccessPathName(fileName.c_str()) && inputFitDir.at("CTAUTRUE")!="") {
        plotLabel = string(Form("_CtauTrue_%s", parIni[modelName.c_str()].c_str()));
        setCtauTrueFileName(fileName, outputDir, DSTAG, plotLabel, cut, isPbPb);
      } else if (inputFitDir.at("CTAUTRUE")!="") { found = true; }
      if (!found && gSystem->AccessPathName(fileName.c_str())) {
        plotLabel = string(Form("_CtauTrue_%s_NoBkg", parIni[modelName.c_str()].c_str()));
        setCtauTrueFileName(fileName, outputDir, DSTAG, plotLabel, cut, isPbPb);
      } else { found = true; }
      if (!found && gSystem->AccessPathName(fileName.c_str())) {
        cout << "[ERROR] User Input File : " << fileName << " was not found!" << endl;
        //return false;
      } else {
        if ( !loadPreviousFitResult(myws, fileName, DSTAG, isPbPb, false, false) ) {
          cout << "[ERROR] The ctau true fit results were not loaded!" << endl;
          return false;
        } else {
          cout << "[INFO] The ctau true fits were found, so I'll load the fit results." << endl; 
        }
      }
    }
  }

  // save the initial values of the model we've just created
  std::unique_ptr<RooArgSet> params = std::unique_ptr<RooArgSet>(myws.pdf(pdfName.c_str())->getParameters(*(myws.data(dsName.c_str()))));
  myws.saveSnapshot((pdfName+"_parIni").c_str(),*params,kTRUE);

  // check if we have already done this fit. If yes, do nothing and return true.
  bool found =  true;
  std::unique_ptr<RooArgSet> newpars = std::unique_ptr<RooArgSet>(myws.pdf(pdfName.c_str())->getParameters(*(myws.data(dsName.c_str()))));
  found = found && isFitAlreadyFound(*newpars, fileName, pdfName);
  if (found) {
    cout << "[INFO] This ctau fit was already done, so I'll just go to the next one." << endl;
    return true;
  }

  // Fit the Datasets
  const bool isWeighted = myws.data(dsName.c_str())->isWeighted();
  auto fitResult = std::unique_ptr<RooFitResult>(myws.pdf(pdfName.c_str())->fitTo(*myws.data(dsNameCut.c_str()), Extended(kTRUE), ExternalConstraints(*myws.set("ConstrainPdfList")), NumCPU(numCores), SumW2Error(isWeighted), Save()));
  fitResult->Print("v");
  myws.import(*fitResult, Form("fitResult_%s", pdfName.c_str()));
  // Draw the mass plot
  drawCtauFrom2DPlot(myws, outputDir, opt, cut, parIni, plotLabel, DSTAG, isPbPb, incJpsi, incPsi2S, incBkg, setLogScale, incSS, binWidth.at("CTAU"));
  drawMassFrom2DPlot(myws, outputDir, opt, cut, parIni, plotLabel, DSTAG, isPbPb, incJpsi, incPsi2S, incBkg, setLogScale, incSS, binWidth.at("MASS"));
  drawCtauMass2DPlot(myws, outputDir, cut, plotLabel, DSTAG, isPbPb, binWidth);
  // Save the results
  myws.saveSnapshot(Form("%s_parFit", pdfName.c_str()),*newpars,kTRUE);
  saveWorkSpace(myws, Form("%sctauMass/%s/result", outputDir.c_str(), DSTAG.c_str()), fileName);

  return true;
};


void setCtauMassFileName(string& fileName, const string& outputDir, string TAG, const string& plotLabel, const struct KinCuts& cut, const bool& isPbPb)
{
  if (TAG.find("_")!=std::string::npos) TAG.erase(TAG.find("_"));
  fileName = Form("%sctauMass/%s/result/FIT_%s_%s_%s%s_pt%.0f%.0f_rap%.0f%.0f_cent%d%d.root", outputDir.c_str(), TAG.c_str(), "CTAUMASS", TAG.c_str(), (isPbPb?"PbPb":"PP"), plotLabel.c_str(), (cut.dMuon.Pt.Min*10.0), (cut.dMuon.Pt.Max*10.0), (cut.dMuon.AbsRap.Min*10.0), (cut.dMuon.AbsRap.Max*10.0), cut.Centrality.Start, cut.Centrality.End);
  return;
};


#endif // #ifndef fitCharmoniaCtauMassModel_C
