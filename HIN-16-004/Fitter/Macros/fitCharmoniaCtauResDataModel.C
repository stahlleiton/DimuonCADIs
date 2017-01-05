#ifndef fitCharmoniaCtauResDataModel_C
#define fitCharmoniaCtauResDataModel_C

#include "Utilities/initClasses.h"
#include "buildCharmoniaCtauResModel.C"
#include "fitCharmoniaCtauErrModel.C"
#include "drawCtauResDataPlot.C"
#include "RooStats/SPlot.h"


void setCtauResDataCutParameters(struct KinCuts& cut);
void setCtauResDataFileName(string& FileName, string outputDir, string TAG, string plotLabel, struct KinCuts cut, bool isPbPb);
void setCtauResDataGlobalParameterRange(RooWorkspace& myws, map<string, string>& parIni, struct KinCuts& cut, string label, double binWidth);
bool setCtauResDataModel( struct OniaModel& model, map<string, string>&  parIni, bool isPbPb);
bool createSignalCtauDSUsingSPLOT(RooWorkspace& ws, string dsName, struct KinCuts cut, bool incJpsi, bool incPsi2S, double binWidth);


bool fitCharmoniaCtauResDataModel( RooWorkspace& myws,             // Local Workspace
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
                               bool incPsi2S      = true,      // Includes Psi(2S) model
                               // Select the fitting options
                               bool doFit         = true,      // Flag to indicate if we want to perform the fit
                               bool loadFitResult = false,     // Load previous fit results
                               map<string, string> inputFitDir={},// User-defined Location of the fit results
                               int  numCores      = 2,         // Number of cores used for fitting
                               // Select the drawing options
                               bool setLogScale   = true,      // Draw plot with log scale
                               bool incSS         = false,     // Include Same Sign data
                               map<string, double> binWidth={} // User-defined Location of the fit results
                               )  
{

  if (DSTAG.find("_")!=std::string::npos) DSTAG.erase(DSTAG.find("_"));

  // Check if input dataset is MC
  if (DSTAG.find("MC")!=std::string::npos) {
    cout << "[ERROR] We can only fit resolution from Data using Data" << endl; return false;
  }

  if (incJpsi && incPsi2S) {
    cout << "[ERROR] We can only fit one type of signal when computing resolution from Data" << endl; return false;
  }
  
  string COLL = (isPbPb ? "PbPb" : "PP" );
  bool usePerEventError = true;
  bool incBkg = false;

  if (importDS) {
    setCtauResDataCutParameters(cut);
    if (usePerEventError) {
      // check if we have already done the ctauErr fits. If yes, load their parameters
      string FileName = "";
      string pdfName = Form("pdfCTAUERR_Tot_%s", COLL.c_str());
      string plotLabel = "";
      bool incJpsi = true;
      bool fitSideBand = false;
      if (incJpsi)  { plotLabel = plotLabel + "_Jpsi";     }
      if (incPsi2S) { plotLabel = plotLabel + "_Psi2S";    }
      plotLabel = plotLabel + "_Bkg";
      setCtauErrFileName(FileName, (inputFitDir["CTAUERR"]=="" ? outputDir : inputFitDir["CTAUERR"]), "DATA", plotLabel, cut, isPbPb, fitSideBand);
      bool foundFit = false;
      if ( loadCtauErrRange(myws, FileName, cut) ) { foundFit = true; }
      if (foundFit) { cout << "[INFO] The ctauErr fit was found and I'll load the ctau Error range used." << endl; }
    }
    setCtauErrCutParameters(cut);
  }

  // Import the local datasets
  double numEntries = 1000000;
  string label = ((DSTAG.find(COLL.c_str())!=std::string::npos) ? DSTAG.c_str() : Form("%s_%s", DSTAG.c_str(), COLL.c_str()));
  string dsName = Form("dOS_%s", label.c_str());
  if (importDS) {
    if ( !(myws.data(dsName.c_str())) ) {
      int importID = importDataset(myws, inputWorkspace, cut, label);
      if (importID<0) { return false; }
      else if (importID==0) { doFit = false; }
    }
    numEntries = myws.data(dsName.c_str())->sumEntries(); if (numEntries<=0) { doFit = false; }
  }
  else if (doFit && !(myws.data(dsName.c_str()))) { cout << "[ERROR] No local dataset was found to perform the fit!" << endl; return false; }

  if (incJpsi || incPsi2S) {
    // Setting extra input information needed by each fitter
    string iMassFitDir = inputFitDir["MASS"];
    double ibWidth = binWidth["MASS"];
    bool loadMassFitResult = true;
    bool doMassFit = false;
    bool importDS = false;
    bool getMeanPT = false;
    bool zoomPsi = false;
    const char* applyCorr = "";
    bool doSimulFit = false;
    bool cutCtau = false;
    
    if ( !fitCharmoniaMassModel( myws, inputWorkspace, cut, parIni, opt, outputDir,
                                DSTAG, isPbPb, importDS,
                                incJpsi, incPsi2S, true,
                                doMassFit, cutCtau, doSimulFit, false, applyCorr, loadMassFitResult, iMassFitDir, numCores,
                                setLogScale, incSS, zoomPsi, ibWidth, getMeanPT
                                )
        ) { return false; }
    if (myws.pdf(Form("pdfMASS_Tot_%s", (isPbPb?"PbPb":"PP")))) {
      cout << "[INFO] Setting mass parameters to constant!" << endl;
      myws.pdf(Form("pdfMASS_Tot_%s", (isPbPb?"PbPb":"PP")))->getParameters(RooArgSet(*myws.var("invMass")))->setAttribAll("Constant", kTRUE);
    } else { cout << "[ERROR] Mass PDF was not found!" << endl; return false; }
    if (myws.pdf(Form("pdfMASS_Bkg_%s", (isPbPb?"PbPb":"PP"))))   { reNormMassVar(myws, "Bkg", isPbPb);   }
    if (myws.pdf(Form("pdfMASS_Jpsi_%s", (isPbPb?"PbPb":"PP"))))  { reNormMassVar(myws, "Jpsi", isPbPb);  }
    if (myws.pdf(Form("pdfMASS_Psi2S_%s", (isPbPb?"PbPb":"PP")))) { reNormMassVar(myws, "Psi2S", isPbPb); }
  }
  
  string dsName2Fit = dsName;
  if (incJpsi) dsName2Fit += "_JPSI";
  else if (incPsi2S) dsName2Fit += "_PSI2S";
  
  if (importDS) {
    // Set global parameters
    if (!createSignalCtauDSUsingSPLOT(myws, dsName, cut, incJpsi, incPsi2S, binWidth["CTAURES"])){ cout << "[ERROR] Creating the Ctau Error Templates using sPLOT failed" << endl; return false; }
    setCtauResDataGlobalParameterRange(myws, parIni, cut, dsName2Fit, binWidth["CTAURES"]);
    RooDataSet* dataToFit = (RooDataSet*)(myws.data(dsName2Fit.c_str())->reduce(parIni["CtauNResRange_Cut"].c_str()))->Clone((dsName2Fit+"_CTAUNRESCUT").c_str());
    myws.import(*dataToFit);
  }
  string dsNameCut = dsName2Fit+"_CTAUNRESCUT";
  string pdfName = Form("pdfCTAURES_Tot_%s", (isPbPb?"PbPb":"PP"));

  if (!loadFitResult) {
    // Set models based on initial parameters
    struct OniaModel model;
    if (!setCtauResDataModel(model, parIni, isPbPb)) { return false; }
    //// LOAD CTAU ERROR PDF
    if (usePerEventError) {
      // Setting extra input information needed by each fitter
      bool loadCtauErrFitResult = true;
      bool doCtauErrFit = true;
      bool importDS = true;
      bool incJpsi = true;
      string DSTAG = Form("DATA_%s", (isPbPb?"PbPb":"PP"));
  
      if ( !fitCharmoniaCtauErrModel( myws, inputWorkspace, cut, parIni, opt, outputDir, 
                                      DSTAG, isPbPb, importDS, 
                                      incJpsi, incPsi2S, incBkg, 
                                      doCtauErrFit, false, loadCtauErrFitResult, inputFitDir, numCores,
                                      setLogScale, incSS, binWidth
                                      ) 
           ) { return false; }
    }

    // Build the Fit Model
    if (!buildCharmoniaCtauResModel(myws, (isPbPb ? model.PbPb : model.PP), parIni, dsNameCut, "ctau", "pdfCTAURES", isPbPb, usePerEventError,  numEntries))  { return false; }
    if (!buildCharmoniaCtauResModel(myws, (isPbPb ? model.PbPb : model.PP), parIni, dsNameCut, "ctauN", "pdfCTAUNRES", isPbPb, false,  numEntries))  { return false; }

    // save the initial values of the model we've just created
    RooArgSet* params = (RooArgSet*) myws.pdf(pdfName.c_str())->getParameters(RooArgSet(*myws.var("ctauN"), *myws.var("ctauErr"), *myws.var("ctau")));
    myws.saveSnapshot(Form("%s_parIni", pdfName.c_str()),*params,kTRUE);
    delete params;
  }

  // Define pdf and plot names
  string plotLabel = "";
  if (incJpsi || incPsi2S) { plotLabel = plotLabel + Form("_CtauRes_%s", parIni[Form("Model_CtauRes_%s", COLL.c_str())].c_str()); }

  // check if we have already done this fit. If yes, do nothing and return true.
  string FileName = "";
  setCtauResDataFileName(FileName, (inputFitDir["CTAURES"]=="" ? outputDir : inputFitDir["CTAURES"]), DSTAG, plotLabel, cut, isPbPb);
  if (gSystem->AccessPathName(FileName.c_str()) && inputFitDir["CTAURES"]!="") {
    cout << "[WARNING] User Input File : " << FileName << " was not found!" << endl;
    if (loadFitResult) return false;
    setCtauResDataFileName(FileName, outputDir, DSTAG, plotLabel, cut, isPbPb);
  }
  bool found =  true; bool skipFit = !doFit;
  RooArgSet *newpars = myws.pdf(pdfName.c_str())->getParameters(RooArgSet(*myws.var("ctau"), *myws.var("ctauErr"), *myws.var("ctauN")));
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
    bool isWeighted = myws.data(dsNameCut.c_str())->isWeighted();
    RooFitResult* fitResult = myws.pdf(pdfName.c_str())->fitTo(*myws.data(dsNameCut.c_str()), Extended(kTRUE), NumCPU(numCores), ConditionalObservables(*myws.var("ctauErr")), SumW2Error(isWeighted), Save());
    fitResult->Print("v");
    myws.import(*fitResult, Form("fitResult_%s", pdfName.c_str()));

    // Draw the mass plot
    drawCtauResDataPlot(myws, outputDir, opt, cut, parIni, plotLabel, DSTAG, dsName2Fit.c_str(), isPbPb, setLogScale, incSS, binWidth["CTAURES"]);
    // Save the results
    string FileName = ""; setCtauResDataFileName(FileName, outputDir, DSTAG, plotLabel, cut, isPbPb);
    myws.saveSnapshot(Form("%s_parFit", pdfName.c_str()),*newpars,kTRUE);
    saveWorkSpace(myws, Form("%sctauRes/%s/result", outputDir.c_str(), DSTAG.c_str()), FileName);
  }
  
  return true;
};


bool setCtauResDataModel( struct OniaModel& model, map<string, string>&  parIni, bool isPbPb)
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

  return true;
};


void setCtauResDataGlobalParameterRange(RooWorkspace& myws, map<string, string>& parIni, struct KinCuts& cut, string dsName, double binWidth)
{
  Double_t ctauNMax; Double_t ctauNMin;
  myws.data(dsName.c_str())->getRange(*myws.var("ctauN"), ctauNMin, ctauNMax);
  ctauNMin -= 0.00001;  ctauNMax += 0.00001;
  cout << "[INFO] Range from data: ctauNMin: " << ctauNMin << "  ctauNMax: " << ctauNMax << endl;
  int nBins = min(int( round((ctauNMax - ctauNMin)/binWidth) ), 1000);
  TH1D* hTot = (TH1D*)myws.data(dsName.c_str())->createHistogram("TMP", *myws.var("ctauN"), Binning(nBins, ctauNMin, ctauNMax));
  vector<double> rangeCtauN;
  getCtauErrRange(hTot, (int)(ceil(2)), rangeCtauN);
  hTot->Delete();
  ctauNMin = rangeCtauN[0];
//  ctauNMax = rangeCtauN[1];
//  cout << ctauNMin << " ; " << ctauNMax << endl;
//  if (abs(ctauNMax)>abs(ctauNMin)) { ctauNMax = abs(ctauNMin); } else { ctauNMin = -1.0*abs(ctauNMax); }
//  if (ctauNMin<cut.dMuon.ctauNRes.Min) { ctauNMin = cut.dMuon.ctauNRes.Min; }
//  if (ctauNMax>cut.dMuon.ctauNRes.Max) { ctauNMax = cut.dMuon.ctauNRes.Max; }
//  if (ctauNMin<-10.0){ ctauNMin = -10.0; }
//  if (ctauNMax>10.0) { ctauNMax = 10.0;  }
  if (parIni.count("ctauResCut")>0 && parIni["ctauResCut"]!="") {
    parIni["ctauResCut"].erase(parIni["ctauResCut"].find("["), string("[").length());
    parIni["ctauResCut"].erase(parIni["ctauResCut"].find("]"), string("]").length());
    double ctauResCut = 0.0;
    try {
      ctauResCut = std::stod(parIni["ctauResCut"].c_str());
    } catch (const std::invalid_argument&) {
      cout << "[WARNING] ctauResCut is not a number, will ignore it!" << endl;
    }
    ctauResCut = abs(ctauResCut); if(ctauResCut>0.0) { ctauNMax = abs(ctauResCut); ctauNMin = -1.0*abs(ctauResCut); }
  }
  cout << "[INFO] Selected range: ctauNMin: " << ctauNMin << "  ctauNMax: " << ctauNMax << endl;
  myws.var("ctauN")->setRange("CtauNResWindow", ctauNMin, ctauNMax);
  parIni["CtauNResRange_Cut"]   = Form("(%.12f <= ctauN && ctauN <= %.12f)", ctauNMin, ctauNMax);
  cut.dMuon.ctauNRes.Max = ctauNMax;
  cut.dMuon.ctauNRes.Min = ctauNMin;
  myws.var("ctauN")->setRange("FullWindow", cut.dMuon.ctauNRes.Min, cut.dMuon.ctauNRes.Max);
  
  Double_t ctauMax; Double_t ctauMin;
  myws.data(dsName.c_str())->getRange(*myws.var("ctau"), ctauMin, ctauMax);
  ctauMin -= 0.00001;  ctauMax += 0.00001;
  cout << "[INFO] Range from data: ctauMin: " << ctauMin << "  ctauMax: " << ctauMax << endl;
  myws.var("ctau")->setRange("CtauResWindow", ctauMin, ctauMax);
  parIni["CtauResRange_Cut"]   = Form("(%.12f <= ctau && ctau <= %.12f)", ctauMin, ctauMax);
  cut.dMuon.ctauRes.Max = ctauMax;
  cut.dMuon.ctauRes.Min = ctauMin;
  myws.var("ctau")->setRange("FullWindow", cut.dMuon.ctauRes.Min, cut.dMuon.ctauRes.Max);

  return;
};


void setCtauResDataFileName(string& FileName, string outputDir, string TAG, string plotLabel, struct KinCuts cut, bool isPbPb)
{
  if (TAG.find("_")!=std::string::npos) TAG.erase(TAG.find("_"));
  FileName = Form("%sctauRes/%s/result/FIT_%s_%s_%s%s_pt%.0f%.0f_rap%.0f%.0f_cent%d%d.root", outputDir.c_str(), TAG.c_str(), "CTAURES", TAG.c_str(), (isPbPb?"PbPb":"PP"), plotLabel.c_str(), (cut.dMuon.Pt.Min*10.0), (cut.dMuon.Pt.Max*10.0), (cut.dMuon.AbsRap.Min*10.0), (cut.dMuon.AbsRap.Max*10.0), cut.Centrality.Start, cut.Centrality.End);
  
  return;
};
 

void setCtauResDataCutParameters(struct KinCuts& cut)
{
  if (cut.dMuon.ctauNRes.Min==-100000.0 && cut.dMuon.ctauNRes.Max==100000.0) {
    // Default ctau values, means that the user did not specify a ctau Normalized Resolution range
    cut.dMuon.ctauNRes.Min = -30.0;
    cut.dMuon.ctauNRes.Max = 30.0;
  }
  // Define the ctau true range
  if (cut.dMuon.ctauTrue.Min==-1000.0) { cut.dMuon.ctauTrue.Min = -5.0; } // Removes cases with ctauTrue = -99
  if (cut.dMuon.ctau.Min==-1000.0)     { cut.dMuon.ctau.Min = -5.0;     } // Removes cases with ctau = -99
  
  return;
};


bool createSignalCtauDSUsingSPLOT(RooWorkspace& ws, string dsName, struct KinCuts cut, bool incJpsi, bool incPsi2S, double binWidth)
{
  
  bool isPbPb = false;
  if (dsName.find("PbPb")!=std::string::npos) { isPbPb = true; }
  if (dsName.find("MC")!=std::string::npos)   { return false;  }  // Only accept data
  
  string pdfMassName = Form("pdfMASS_Tot_%s", (isPbPb?"PbPb":"PP"));
  RooArgList yieldList;
  if (incJpsi)  { yieldList.add( *ws.var(Form("N_Jpsi_%s", (isPbPb?"PbPb":"PP"))) );  }
  if (incPsi2S) { yieldList.add( *ws.var(Form("N_Psi2S_%s", (isPbPb?"PbPb":"PP"))) ); }
  yieldList.add( *ws.var(Form("N_Bkg_%s", (isPbPb?"PbPb":"PP"))) ); // Always add background
  RooDataSet* data = (RooDataSet*)ws.data(dsName.c_str())->Clone("TMP_DATA");
  RooAbsPdf* pdf = (RooAbsPdf*)ws.pdf(pdfMassName.c_str())->Clone("TMP_PDF");
  
  RooStats::SPlot sData = RooStats::SPlot("sData","An SPlot", *data, pdf, yieldList);
  ws.import(*data, Rename((dsName+"_SPLOT").c_str()));
  if (incJpsi) {
    cout <<  "[INFO] Jpsi yield -> Mass Fit: " << ws.var(Form("N_Jpsi_%s", (isPbPb?"PbPb":"PP")))->getVal() << " , sWeights: " << sData.GetYieldFromSWeight(Form("N_Jpsi_%s", (isPbPb?"PbPb":"PP"))) << std::endl;
  }
  if (incPsi2S) {
    cout <<  "[INFO] Psi2S yield -> Mass Fit: " << ws.var(Form("N_Psi2S_%s", (isPbPb?"PbPb":"PP")))->getVal() << " , sWeights: " << sData.GetYieldFromSWeight(Form("N_Psi2S_%s", (isPbPb?"PbPb":"PP"))) << std::endl;
  }
  cout <<  "[INFO] Bkg yield -> Mass Fit: " << ws.var(Form("N_Bkg_%s", (isPbPb?"PbPb":"PP")))->getVal() << " , sWeights: " << sData.GetYieldFromSWeight(Form("N_Bkg_%s", (isPbPb?"PbPb":"PP"))) << std::endl;
  
  // create weighted data sets
  double ctauMax = cut.dMuon.ctau.Max, ctauMin = cut.dMuon.ctau.Min;
  vector<double> range; range.push_back(cut.dMuon.ctau.Min); range.push_back(cut.dMuon.ctau.Max);
  int nBins = min(int( round((ctauMax - ctauMin)/binWidth) ), 1000);
  
  if (incJpsi) {
    RooDataSet* dataw_Jpsi  = new RooDataSet("TMP_JPSI_DATA","TMP_JPSI_DATA", (RooDataSet*)ws.data((dsName+"_SPLOT").c_str()), RooArgSet(*ws.var("invMass"),*ws.var("pt"),*ws.var("rap"),*ws.var("cent"),*ws.var("ctau"), *ws.var("ctauN"), *ws.var("ctauErr"), *ws.var(Form("N_Jpsi_%s_sw", (isPbPb?"PbPb":"PP")))), 0, Form("N_Jpsi_%s_sw", (isPbPb?"PbPb":"PP")));
    RooDataSet* dataw_Jpsi_reduced = (RooDataSet*)dataw_Jpsi->reduce("ctau<=0");
    ws.import(*dataw_Jpsi_reduced, Rename((dsName+"_JPSI").c_str()));
  }
  if (incPsi2S) {
    RooDataSet* dataw_Psi2S  = new RooDataSet("TMP_PSI2S_DATA","TMP_PSI2S_DATA", (RooDataSet*)ws.data((dsName+"_SPLOT").c_str()), RooArgSet(*ws.var("invMass"),*ws.var("pt"),*ws.var("rap"),*ws.var("cent"),*ws.var("ctau"), *ws.var("ctauN"), *ws.var("ctauErr"), *ws.var(Form("N_Psi2S_%s_sw", (isPbPb?"PbPb":"PP")))), 0, Form("N_Psi2S_%s_sw", (isPbPb?"PbPb":"PP")));
    RooDataSet* dataw_Psi2S_reduced = (RooDataSet*)dataw_Psi2S->reduce("ctau<=0");
    ws.import(*dataw_Psi2S_reduced, Rename((dsName+"_PSI2S").c_str()));
  }
  
  return true;
};

#endif // #ifndef fitCharmoniaCtauResDataModel_C
