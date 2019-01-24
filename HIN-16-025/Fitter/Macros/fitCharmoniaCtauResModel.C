#ifndef fitCharmoniaCtauResModel_C
#define fitCharmoniaCtauResModel_C

#include "Utilities/initClasses.h"
#include "buildCharmoniaCtauResModel.C"
#include "fitCharmoniaCtauErrModel.C"
#include "drawCtauResPlot.C"


void setCtauResCutParameters ( struct KinCuts& cut, const bool& incNonPrompt );
void setCtauResFileName      ( string& fileName, const string& outputDir, string TAG, const string& plotLabel, const struct KinCuts& cut, const bool& isPbPb );
void setCtauResGlobalParameterRange ( RooWorkspace& myws, map<string, string>& parIni, struct KinCuts& cut, const string& label, const double& binWidth );
bool setCtauResModel         ( struct OniaModel& model, const map<string, string>& parIni, const bool& isPbPb );


bool fitCharmoniaCtauResModel( RooWorkspace& myws,             // Local Workspace
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
                               // Select the fitting options
                               const bool  useTotctauErrPdf,        // If yes use the total ctauErr PDF instead of Jpsi and bkg ones
                                     bool  doFit,                   // Flag to indicate if we want to perform the fit
                                     bool  wantPureSMC,             // Flag to indicate if we want to fit pure signal MC
                               const uint& loadFitResult,           // Load previous fit results
                               const map<string, string>& inputFitDir,// User-defined Location of the fit results
                               const int&  numCores,                // Number of cores used for fitting
                               // Select the drawing options
                               const bool& setLogScale,             // Draw plot with log scale
                               const bool& incSS,                   // Include Same Sign data
                               const map<string, double>& binWidth  // Bin width used for plotting
                               )
{

  if (DSTAG.find("_")!=std::string::npos) DSTAG.erase(DSTAG.find("_"));

  // Check if input dataset is MC
  bool isMC = false;
  if (DSTAG.find("MC")!=std::string::npos) {
    if (incJpsi && incPsi2S) { 
      cout << "[ERROR] We can only fit one type of signal using MC" << endl; return false; 
    }
    isMC = true;
  }
  if (!isMC)  { cout << "[ERROR] The ctau resolution fit can only be performed in MC!" << endl; return false; }
  const bool incNonPrompt = (DSTAG.find("NOPR")!=std::string::npos);

  const string COLL = (isPbPb ? "PbPb" : "PP" );
  const bool usePerEventError = true;
  const bool incBkg = false;

  if (importDS) {
    setCtauResCutParameters(cut, incNonPrompt);
    if (usePerEventError) {
      // check if we have already done the ctauErr fits. If yes, load their parameters
      string fileName = "";
      const string pdfName = Form("pdfCTAUERR_Tot_%s", COLL.c_str());
      string plotLabel = "";
      const bool fitSideBand = false;
      if (incJpsi)  { plotLabel = plotLabel + "_Jpsi";  }
      if (incPsi2S) { plotLabel = plotLabel + "_Psi2S"; }
      plotLabel = plotLabel + "_Bkg";
      setCtauErrFileName(fileName, (inputFitDir.at("CTAUERR")=="" ? outputDir : inputFitDir.at("CTAUERR")), "DATA", plotLabel, cut, isPbPb, fitSideBand);
      bool foundFit = false;
      if ( loadCtauErrRange(fileName, cut) ) { foundFit = true; }
      if (foundFit) { cout << "[INFO] The ctauErr fit was found and I'll load the ctau Error range used." << endl; }
      else { cout << "[ERROR] The ctauErr fit was not found!" << endl; return false; }
    }
    setCtauErrCutParameters(cut);
  }

  // Import the local datasets
  double numEntries = 1000000;
  string label = ((DSTAG.find(COLL.c_str())!=std::string::npos) ? DSTAG.c_str() : Form("%s_%s", DSTAG.c_str(), COLL.c_str()));
  if (wantPureSMC) label = Form("%s_NoBkg", label.c_str());
  const string dsName = Form("dOS_%s", label.c_str());
  if (importDS) {
    if ( !(myws.data(dsName.c_str())) ) {
      const int importID = importDataset(myws, inputWorkspace, cut, label);
      if (importID<0) { return false; }
      else if (importID==0) { doFit = false; }
    }
    numEntries = myws.data(dsName.c_str())->sumEntries(); if (numEntries<=0) { doFit = false; }
  }
  else if (doFit && !(myws.data(dsName.c_str()))) { cout << "[ERROR] No local dataset was found to perform the fit!" << endl; return false; }

  if (importDS) { 
    // Set global parameters
    setCtauResGlobalParameterRange(myws, parIni, cut, label, binWidth.at("CTAURES"));
    auto ds = std::unique_ptr<RooDataSet>((RooDataSet*)(myws.data(dsName.c_str())->reduce(parIni.at("CtauNResRange_Cut").c_str())));
    auto dataToFit = std::unique_ptr<RooDataSet>((RooDataSet*)ds->Clone((dsName+"_CTAUNRESCUT").c_str()));
    myws.import(*dataToFit);
  }
  const string dsNameCut = dsName+"_CTAUNRESCUT";
  const string pdfName = Form("pdfCTAURES_Tot_%s", (isPbPb?"PbPb":"PP"));

  if (!loadFitResult) {
    // Set models based on initial parameters
    struct OniaModel model;
    if (!setCtauResModel(model, parIni, isPbPb)) { return false; }
    //// LOAD CTAU ERROR PDF
    if (usePerEventError) {
      // Setting extra input information needed by each fitter
      const bool loadCtauErrFitResult = true;
      const bool doCtauErrFit = true;
      const bool importDS = isMC;
      const string DSTAG = Form("DATA_%s", (isPbPb?"PbPb":"PP"));
  
      if ( !fitCharmoniaCtauErrModel( myws, inputWorkspace, cut, parIni, opt, outputDir, 
                                      DSTAG, isPbPb, importDS, 
                                      incJpsi, incPsi2S, incBkg, 
                                      doCtauErrFit, wantPureSMC, loadCtauErrFitResult, inputFitDir, numCores, 
                                      setLogScale, incSS, binWidth
                                      ) 
           ) { return false; }
    }

    // Build the Fit Model
    if (!buildCharmoniaCtauResModel(myws, (isPbPb ? model.PbPb : model.PP), parIni, dsName, "ctauRes", "pdfCTAURES", isPbPb, usePerEventError, useTotctauErrPdf, numEntries))  { return false; }
    if (!buildCharmoniaCtauResModel(myws, (isPbPb ? model.PbPb : model.PP), parIni, dsName, "ctauNRes", "pdfCTAUNRES", isPbPb, false, useTotctauErrPdf, numEntries))  { return false; }

    // save the initial values of the model we've just created
    auto params = std::unique_ptr<RooArgSet>((RooArgSet*) myws.pdf(pdfName.c_str())->getParameters(RooArgSet(*myws.var("ctauRes"), *myws.var("ctauNRes"), *myws.var("ctauErr"), *myws.var("ctau"))));
    myws.saveSnapshot(Form("%s_parIni", pdfName.c_str()),*params,kTRUE);
  }

  // Define pdf and plot names
  string plotLabel = "";
  if (incJpsi || incPsi2S) { plotLabel = plotLabel + Form("_CtauRes_%s", parIni[Form("Model_CtauRes_%s", COLL.c_str())].c_str()); }
  if (wantPureSMC)         { plotLabel = plotLabel + "_NoBkg"; }

  // check if we have already done this fit. If yes, do nothing and return true.
  string fileName = "";
  setCtauResFileName(fileName, (inputFitDir.at("CTAURES")=="" ? outputDir : inputFitDir.at("CTAURES")), DSTAG, plotLabel, cut, isPbPb);
  if (gSystem->AccessPathName(fileName.c_str()) && inputFitDir.at("CTAURES")!="") {
    cout << "[WARNING] User Input File : " << fileName << " was not found!" << endl;
    if (loadFitResult) return false;
    setCtauResFileName(fileName, outputDir, DSTAG, plotLabel, cut, isPbPb);
  }
  bool found =  true; bool skipFit = !doFit;
  std::unique_ptr<RooArgSet> newpars = std::unique_ptr<RooArgSet>(myws.pdf(pdfName.c_str())->getParameters(*(myws.data(dsName.c_str()))));
  found = found && isFitAlreadyFound(*newpars, fileName, pdfName.c_str());
  if (loadFitResult) {
    if ( loadPreviousFitResult(myws, fileName, DSTAG, isPbPb, false, false) ) { skipFit = true; } else  { skipFit = false; }
    if (skipFit) { cout << "[INFO] This ctau fit was already done, so I'll load the fit results." << endl; }
    myws.saveSnapshot(Form("%s_parLoad", pdfName.c_str()),*newpars,kTRUE);
  } else if (found) {
    cout << "[INFO] This ctau fit was already done, so I'll just go to the next one." << endl;
    return true;
  }

  // Fit the Datasets
  if (skipFit==false) {
    const bool isWeighted = myws.data(dsName.c_str())->isWeighted();
    auto fitResult = std::unique_ptr<RooFitResult>(myws.pdf(pdfName.c_str())->fitTo(*myws.data(dsNameCut.c_str()), Extended(kTRUE), NumCPU(numCores), SumW2Error(isWeighted), Save()));
    fitResult->Print("v");
    myws.import(*fitResult, Form("fitResult_%s", pdfName.c_str()));
    // Draw the mass plot
    drawCtauResPlot(myws, outputDir, opt, cut, parIni, plotLabel, DSTAG, isPbPb, wantPureSMC, setLogScale, incSS, binWidth.at("CTAURES"));
    // Save the results
    string fileName = ""; setCtauResFileName(fileName, outputDir, DSTAG, plotLabel, cut, isPbPb);
    myws.saveSnapshot(Form("%s_parFit", pdfName.c_str()),*newpars,kTRUE);
    saveWorkSpace(myws, Form("%sctauRes/%s/result", outputDir.c_str(), DSTAG.c_str()), fileName);
  }
  
  return true;
};


bool setCtauResModel(struct OniaModel& model, const map<string, string>& parIni, const bool& isPbPb)
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
  return true;
};


void setCtauResGlobalParameterRange(RooWorkspace& myws, map<string, string>& parIni, struct KinCuts& cut, const string& label, const double& binWidth)
{
  double ctauNResMax , ctauNResMin;
  myws.data(Form("dOS_%s", label.c_str()))->getRange(*myws.var("ctauNRes"), ctauNResMin, ctauNResMax);
  ctauNResMin -= 0.00001;  ctauNResMax += 0.00001;
  cout << "[INFO] Range from data: ctauNResMin: " << ctauNResMin << "  ctauNResMax: " << ctauNResMax << endl;
  const int nBins = min(int( round((ctauNResMax - ctauNResMin)/binWidth) ), 1000);
  auto hTot = std::unique_ptr<TH1D>((TH1D*)myws.data(Form("dOS_%s", label.c_str()))->createHistogram("TMP", *myws.var("ctauNRes"), Binning(nBins, ctauNResMin, ctauNResMax)));
  vector<double> rangeCtauNRes;
  getRange(rangeCtauNRes, *hTot, (int)(ceil(5)));
  ctauNResMin = rangeCtauNRes[0];
  ctauNResMax = rangeCtauNRes[1];
  if (abs(ctauNResMax)>abs(ctauNResMin)) { ctauNResMax = abs(ctauNResMin); } else { ctauNResMin = -1.0*abs(ctauNResMax); }
  if (ctauNResMin<cut.dMuon.ctauNRes.Min) { ctauNResMin = cut.dMuon.ctauNRes.Min; }
  if (ctauNResMax>cut.dMuon.ctauNRes.Max) { ctauNResMax = cut.dMuon.ctauNRes.Max; }
  if (ctauNResMin<-10.0){ ctauNResMin = -10.0; }
  if (ctauNResMax>10.0) { ctauNResMax = 10.0;  }
  if (parIni.count("ctauResCut")>0 && parIni.at("ctauResCut")!="") {
    parIni.at("ctauResCut").erase(parIni.at("ctauResCut").find("["), string("[").length());
    parIni.at("ctauResCut").erase(parIni.at("ctauResCut").find("]"), string("]").length());
    double ctauResCut = 0.0;
    try {
      ctauResCut = std::stod(parIni.at("ctauResCut").c_str());
    } catch (const std::invalid_argument&) {
      cout << "[WARNING] ctauResCut is not a number, will ignore it!" << endl;
    }
    ctauResCut = abs(ctauResCut); if(ctauResCut>0.0) { ctauNResMax = abs(ctauResCut); ctauNResMin = -1.0*abs(ctauResCut); }
  }
  cout << "[INFO] Selected range: ctauNResMin: " << ctauNResMin << "  ctauNResMax: " << ctauNResMax << endl;
  myws.var("ctauNRes")->setRange("CtauNResWindow", ctauNResMin, ctauNResMax);
  parIni.at("CtauNResRange_Cut") = Form("(%.12f <= ctauNRes && ctauNRes <= %.12f)", ctauNResMin, ctauNResMax);
  cut.dMuon.ctauNRes.Max = ctauNResMax;
  cut.dMuon.ctauNRes.Min = ctauNResMin;
  myws.var("ctauNRes")->setRange("FullWindow", cut.dMuon.ctauNRes.Min, cut.dMuon.ctauNRes.Max);
  double ctauResMax , ctauResMin;
  myws.data(Form("dOS_%s", label.c_str()))->reduce(parIni.at("CtauNResRange_Cut").c_str())->getRange(*myws.var("ctauRes"), ctauResMin, ctauResMax);
  ctauResMin -= 0.00001;  ctauResMax += 0.00001;
  cout << "[INFO] Range from data: ctauResMin: " << ctauResMin << "  ctauResMax: " << ctauResMax << endl;
  myws.var("ctauRes")->setRange("CtauResWindow", ctauResMin, ctauResMax);
  parIni.at("CtauResRange_Cut")  = Form("(%.12f <= ctauRes && ctauRes <= %.12f)", ctauResMin, ctauResMax);
  cut.dMuon.ctauRes.Max = ctauResMax;
  cut.dMuon.ctauRes.Min = ctauResMin;
  myws.var("ctauRes")->setRange("FullWindow", cut.dMuon.ctauRes.Min, cut.dMuon.ctauRes.Max);
  return;
};


void setCtauResFileName(string& fileName, const string& outputDir, string TAG, const string& plotLabel, const struct KinCuts& cut, const bool& isPbPb)
{
  if (TAG.find("_")!=std::string::npos) TAG.erase(TAG.find("_"));
  fileName = Form("%sctauRes/%s/result/FIT_%s_%s_%s%s_pt%.0f%.0f_rap%.0f%.0f_cent%d%d.root", outputDir.c_str(), TAG.c_str(), "CTAURES", TAG.c_str(), (isPbPb?"PbPb":"PP"), plotLabel.c_str(), (cut.dMuon.Pt.Min*10.0), (cut.dMuon.Pt.Max*10.0), (cut.dMuon.AbsRap.Min*10.0), (cut.dMuon.AbsRap.Max*10.0), cut.Centrality.Start, cut.Centrality.End);
  return;
};


void setCtauResCutParameters(struct KinCuts& cut, const bool& incNonPrompt)
{
  if (cut.dMuon.ctauNRes.Min==-100000.0 && cut.dMuon.ctauNRes.Max==100000.0) {
    // Default ctau values, means that the user did not specify a ctau Normalized Resolution range
    cut.dMuon.ctauNRes.Min = -30.0;
    cut.dMuon.ctauNRes.Max = 30.0;
  }
  // Define the ctau true range
  if (cut.dMuon.ctauTrue.Min==-1000.0) { cut.dMuon.ctauTrue.Min = (incNonPrompt ? 0.1 : -5.0); } // Removes cases with ctauTrue = -99
  if (cut.dMuon.ctau.Min==-1000.0)     { cut.dMuon.ctau.Min = -5.0;     } // Removes cases with ctau = -99
  return;
};


#endif // #ifndef fitCharmoniaCtauResModel_C
