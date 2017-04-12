#ifndef fitCharmoniaCtauResDataModel_C
#define fitCharmoniaCtauResDataModel_C

#include "Utilities/initClasses.h"
#include "buildCharmoniaCtauResModel.C"
#include "fitCharmoniaMassModel.C"
#include "fitCharmoniaCtauErrModel.C"
#include "drawCtauResDataPlot.C"
#include "RooStats/SPlot.h"


void setCtauResDataCutParameters(struct KinCuts& cut);
void setCtauResDataFileName(string& FileName, string outputDir, string TAG, string plotLabel, struct KinCuts cut, bool isPbPb);
void setCtauResDataGlobalParameterRange(RooWorkspace& myws, map<string, string>& parIni, struct KinCuts& cut, string label, double binWidth);
bool setCtauResDataModel( struct OniaModel& model, map<string, string>&  parIni, bool isPbPb);
bool createSignalCtauDSUsingSPLOT(RooWorkspace& ws, string dsName, map<string, string>  parIni, struct KinCuts cut, bool incJpsi, bool incPsi2S, bool incBkg, bool useSPlot);


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
                               bool incBkg        = true,      // Includes Bkg model
                               // Select the fitting options
                               bool useSPlot      = true,      // If yes, then use SPlot technique, if no, use mass range
                               bool useTotctauErrPdf = false,  // If yes use the total ctauErr PDF instead of Jpsi and bkg ones
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

  if ( (incJpsi && incPsi2S) || (incJpsi && incBkg) || (incBkg && incPsi2S) ) {
    cout << "[ERROR] We can only fit one type of signal/background when computing resolution from Data" << endl; return false;
  }
  
  string COLL = (isPbPb ? "PbPb" : "PP" );
  bool usePerEventError = true;

  if (importDS) {
    setMassCutParameters(cut, incJpsi, incPsi2S, false, false);
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

  string dsName2Fit = dsName;
  if (incJpsi) dsName2Fit += "_JPSI";
  else if (incPsi2S) dsName2Fit += "_PSI2S";
  else if (incBkg) dsName2Fit += "_BKG";
  string dsNameCut = dsName2Fit+"_CTAUNCUT";

  // Define pdf and plot names
  string plotLabel = "";
  plotLabel = plotLabel + Form("_CtauRes_%s", parIni[Form("Model_CtauRes_%s", COLL.c_str())].c_str());
  string pdfName = Form("pdfCTAURES_Tot_%s", (isPbPb?"PbPb":"PP"));

  string FileName = "";
  setCtauResDataFileName(FileName, (inputFitDir["CTAURES"]=="" ? outputDir : inputFitDir["CTAURES"]), DSTAG, plotLabel, cut, isPbPb);


  // Check if we have already made the Signal DataSet
  vector<string> dsNames = { dsName2Fit };
  bool createSignalDS = ( !isSPlotDSAlreadyFound(myws, FileName, dsNames, true) );

  if (incJpsi || incPsi2S || incBkg) {
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
    bool doConstrFit = false;
    
    if ( !fitCharmoniaMassModel( myws, inputWorkspace, cut, parIni, opt, outputDir,
                                 DSTAG, isPbPb, importDS,
                                 true, incPsi2S, true,
                                 doMassFit, cutCtau, doConstrFit, doSimulFit, false, applyCorr, loadMassFitResult, iMassFitDir, numCores,
                                 setLogScale, incSS, zoomPsi, ibWidth, getMeanPT
                                 )
         ) { return false; }
    if (myws.pdf(Form("pdfMASS_Tot_%s", (isPbPb?"PbPb":"PP")))) {
      cout << "[INFO] Setting mass parameters to constant!" << endl;
      myws.pdf(Form("pdfMASS_Tot_%s", (isPbPb?"PbPb":"PP")))->getParameters(RooArgSet(*myws.var("invMass")))->setAttribAll("Constant", kTRUE);
    } else { cout << "[ERROR] Mass PDF was not found!" << endl; return false; }
    if (myws.pdf(Form("pdfMASS_Bkg_%s", (isPbPb?"PbPb":"PP"))))   { setConstant( myws, Form("N_Bkg_%s", (isPbPb?"PbPb":"PP")), false);   }
    if (myws.pdf(Form("pdfMASS_Jpsi_%s", (isPbPb?"PbPb":"PP"))))  { setConstant( myws, Form("N_Jpsi_%s", (isPbPb?"PbPb":"PP")), false);  }
    if (myws.pdf(Form("pdfMASS_Psi2S_%s", (isPbPb?"PbPb":"PP")))) { setConstant( myws, Form("N_Psi2S_%s", (isPbPb?"PbPb":"PP")), false); }
  }

  if (createSignalDS) {  
    if (importDS) {
      if (!createSignalCtauDSUsingSPLOT(myws, dsName, parIni, cut, incJpsi, incPsi2S, incBkg, useSPlot)){ cout << "[ERROR] Creating the Ctau Error Templates using sPLOT failed" << endl; return false; }
    }
  }

  if (importDS) {
    // Set global parameters
    setCtauResDataGlobalParameterRange(myws, parIni, cut, dsName2Fit, binWidth["CTAURES"]);
    RooDataSet* dataToFit = (RooDataSet*)(myws.data(dsName2Fit.c_str())->reduce(parIni["CtauNRange_Cut"].c_str()))->Clone((dsName2Fit+"_CTAUNCUT").c_str());
    myws.import(*dataToFit, Rename((dsName2Fit+"_CTAUNCUT").c_str()));
  }

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
    if (!buildCharmoniaCtauResModel(myws, (isPbPb ? model.PbPb : model.PP), parIni, dsNameCut, "ctau", "pdfCTAURES", isPbPb, usePerEventError, useTotctauErrPdf, numEntries))  { return false; }
    if (!buildCharmoniaCtauResModel(myws, (isPbPb ? model.PbPb : model.PP), parIni, dsNameCut, "ctauN", "pdfCTAUNRES", isPbPb, false, useTotctauErrPdf, numEntries))  { return false; }

    // save the initial values of the model we've just created
    RooArgSet* params = (RooArgSet*) myws.pdf(pdfName.c_str())->getParameters(RooArgSet(*myws.var("ctauN"), *myws.var("ctauErr"), *myws.var("ctau")));
    myws.saveSnapshot(Form("%s_parIni", pdfName.c_str()),*params,kTRUE);
    delete params;
  }

  // check if we have already done this fit. If yes, do nothing and return true.
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
    RooFitResult* fitResult = myws.pdf(pdfName.c_str())->fitTo(*myws.data(dsNameCut.c_str()), Extended(kTRUE), NumCPU(numCores), SumW2Error(isWeighted), Save());
    fitResult->Print("v");
    myws.import(*fitResult, Form("fitResult_%s", pdfName.c_str()));

    // Draw the mass plot
    if(!drawCtauResDataPlot(myws, outputDir, opt, cut, parIni, plotLabel, DSTAG, dsName2Fit.c_str(), isPbPb, setLogScale, incSS, binWidth["CTAURES"])) { return false; }
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
  ctauNMin -= 0.00000001; ctauNMax += 0.00000001;
  cout << "[INFO] Range from data: ctauNMin: " << ctauNMin << "  ctauNMax: " << ctauNMax << endl;
  int nBins = min(int( round((ctauNMax - ctauNMin)/binWidth) ), 1000);
  TH1D* hTot = (TH1D*)myws.data(dsName.c_str())->createHistogram("TMP", *myws.var("ctauN"), Binning(nBins, ctauNMin, ctauNMax));
  vector<double> rangeCtauN;
  getCtauErrRange(hTot, (int)(ceil(3)), rangeCtauN);
  hTot->Delete();
  ctauNMin = rangeCtauN[0];
  if (ctauNMin<-10.0){ ctauNMin = -10.0; }
  if (ctauNMax>  0.0){ ctauNMax =   0.0; }
  if (parIni.count("ctauNCut")>0 && parIni["ctauNCut"]!="") {
    parIni["ctauNCut"].erase(parIni["ctauNCut"].find("["), string("[").length());
    parIni["ctauNCut"].erase(parIni["ctauNCut"].find("]"), string("]").length());
    double ctauNCut = 0.0;
    try {
      ctauNCut = std::stod(parIni["ctauNCut"].c_str());
    } catch (const std::invalid_argument&) {
      cout << "[WARNING] ctauNCut is not a number, will ignore it!" << endl;
    }
    ctauNCut = abs(ctauNCut); if(ctauNCut>0.0) { ctauNMin = -1.0*abs(ctauNCut); }
  }
  cout << "[INFO] Selected range: ctauNMin: " << ctauNMin << "  ctauNMax: " << ctauNMax << endl;
  myws.var("ctauN")->setRange("CtauNWindow", ctauNMin, ctauNMax);
  parIni["CtauNRange_Cut"]   = Form("(%.12f <= ctauN && ctauN <= %.12f)", ctauNMin, ctauNMax);
  cut.dMuon.ctauN.Max = ctauNMax;
  cut.dMuon.ctauN.Min = ctauNMin;
  myws.var("ctauN")->setRange("FullWindow", cut.dMuon.ctauN.Min, cut.dMuon.ctauN.Max);
  
  Double_t ctauMax; Double_t ctauMin;
  myws.data(dsName.c_str())->getRange(*myws.var("ctau"), ctauMin, ctauMax);
  ctauMin -= 0.00000001; ctauMax += 0.00000001;
  if (ctauMax >  0.0){ ctauMax =   0.0; }
  cout << "[INFO] Range from data: ctauMin: " << ctauMin << "  ctauMax: " << ctauMax << endl;
  myws.var("ctau")->setRange("CtauWindow", ctauMin, ctauMax);
  parIni["CtauRange_Cut"]   = Form("(%.12f <= ctau && ctau <= %.12f)", ctauMin, ctauMax);
  cut.dMuon.ctau.Max = ctauMax;
  cut.dMuon.ctau.Min = ctauMin;
  myws.var("ctau")->setRange("FullWindow", cut.dMuon.ctau.Min, cut.dMuon.ctau.Max);

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
  
  return;
};


bool createSignalCtauDSUsingSPLOT(RooWorkspace& ws, string dsName, map<string, string>  parIni, struct KinCuts cut, bool incJpsi, bool incPsi2S, bool incBkg, bool useSPlot)
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

  if (useSPlot) {
    RooStats::SPlot sData = RooStats::SPlot("sData","An SPlot", *data, pdf, yieldList);
    ws.import(*data, Rename((dsName+"_SPLOT").c_str()));
    if (incJpsi) {
      cout <<  "[INFO] Jpsi yield -> Mass Fit: " << ws.var(Form("N_Jpsi_%s", (isPbPb?"PbPb":"PP")))->getVal() << " , sWeights: " << sData.GetYieldFromSWeight(Form("N_Jpsi_%s", (isPbPb?"PbPb":"PP"))) << std::endl;
    }
    if (incPsi2S) {
      cout <<  "[INFO] Psi2S yield -> Mass Fit: " << ws.var(Form("N_Psi2S_%s", (isPbPb?"PbPb":"PP")))->getVal() << " , sWeights: " << sData.GetYieldFromSWeight(Form("N_Psi2S_%s", (isPbPb?"PbPb":"PP"))) << std::endl;
    }
    cout <<  "[INFO] Bkg yield -> Mass Fit: " << ws.var(Form("N_Bkg_%s", (isPbPb?"PbPb":"PP")))->getVal() << " , sWeights: " << sData.GetYieldFromSWeight(Form("N_Bkg_%s", (isPbPb?"PbPb":"PP"))) << std::endl;

    if (incBkg) {
      RooDataSet* dataw_Bkg  = new RooDataSet("TMP_BKG_DATA","TMP_BKG_DATA", (RooDataSet*)ws.data((dsName+"_SPLOT").c_str()), RooArgSet(*ws.var("invMass"), *ws.var("ctau"), *ws.var("ctauN"), *ws.var("ctauErr"), *ws.var(Form("N_Bkg_%s_sw", (isPbPb?"PbPb":"PP")))), 0, Form("N_Bkg_%s_sw", (isPbPb?"PbPb":"PP")));
      RooDataSet* dataw_Bkg_reduced = (RooDataSet*)dataw_Bkg->reduce("(invMass<3.4)&&(ctau<=0)");
      ws.import(*dataw_Bkg_reduced, Rename((dsName+"_BKG").c_str()));
      delete dataw_Bkg;
    }
    if (incJpsi) {
      RooDataSet* dataw_Jpsi  = new RooDataSet("TMP_JPSI_DATA","TMP_JPSI_DATA", (RooDataSet*)ws.data((dsName+"_SPLOT").c_str()), RooArgSet(*ws.var("invMass"), *ws.var("ctau"), *ws.var("ctauN"), *ws.var("ctauErr"), *ws.var(Form("N_Jpsi_%s_sw", (isPbPb?"PbPb":"PP")))), 0, Form("N_Jpsi_%s_sw", (isPbPb?"PbPb":"PP")));
      RooDataSet* dataw_Jpsi_reduced = (RooDataSet*)dataw_Jpsi->reduce("(invMass<3.4)&&(ctau<=0)");
      ws.import(*dataw_Jpsi_reduced, Rename((dsName+"_JPSI").c_str()));
      delete dataw_Jpsi;
    }
    if (incPsi2S) {
      RooDataSet* dataw_Psi2S  = new RooDataSet("TMP_PSI2S_DATA","TMP_PSI2S_DATA", (RooDataSet*)ws.data((dsName+"_SPLOT").c_str()), RooArgSet(*ws.var("invMass"),*ws.var("pt"),*ws.var("rap"),*ws.var("cent"),*ws.var("ctau"), *ws.var("ctauN"), *ws.var("ctauErr"), *ws.var(Form("N_Psi2S_%s_sw", (isPbPb?"PbPb":"PP")))), 0, Form("N_Psi2S_%s_sw", (isPbPb?"PbPb":"PP")));
      RooDataSet* dataw_Psi2S_reduced = (RooDataSet*)dataw_Psi2S->reduce("(invMass<3.4)&&(ctau<=0)");
      ws.import(*dataw_Psi2S_reduced, Rename((dsName+"_PSI2S").c_str()));
      delete dataw_Psi2S;
    }
  }
  else {
    if (incJpsi) {
      cout <<  "[INFO] Using mass window: " << (parIni["JpsiMassRange_Cut"]+"&&(ctau<=0)") <<  std::endl;
      cout <<  "[INFO] Number of entries in Jpsi mass window: " << ((RooDataSet*)data->reduce((parIni["JpsiMassRange_Cut"]+"&&(ctau<=0)").c_str()))->numEntries() <<  std::endl;
      RooDataSet* dataw_Jpsi_reduced = (RooDataSet*)data->reduce((parIni["JpsiMassRange_Cut"]+"&&(ctau<=0)").c_str());
      ws.var(Form("N_Jpsi_%s", (isPbPb?"PbPb":"PP")))->setVal(((RooDataSet*)data->reduce((parIni["JpsiMassRange_Cut"]+"&&(ctau<=0)").c_str()))->numEntries());
      ws.import(*dataw_Jpsi_reduced, Rename((dsName+"_JPSI").c_str()));
    }
    if (incPsi2S) {
      cout <<  "[INFO] Using mass window: " << (parIni["Psi2SMassRange_Cut"]+"&&(ctau<=0)") <<  std::endl;
      cout <<  "[INFO] Number of entries in Psi2S mass window: " << ((RooDataSet*)data->reduce((parIni["Psi2SMassRange_Cut"]+"&&(ctau<=0)").c_str()))->numEntries() <<  std::endl;
      RooDataSet* dataw_Psi2S_reduced = (RooDataSet*)data->reduce((parIni["Psi2SMassRange_Cut"]+"&&(ctau<=0)").c_str());
      ws.var(Form("N_Psi2S_%s", (isPbPb?"PbPb":"PP")))->setVal(((RooDataSet*)data->reduce((parIni["Psi2SMassRange_Cut"]+"&&(ctau<=0)").c_str()))->numEntries());
      ws.import(*dataw_Psi2S_reduced, Rename((dsName+"_PSI2S").c_str()));
    }
    if (incBkg) {
      cout <<  "[INFO] Using mass window: " << (parIni["BkgMassRange_FULL_Cut"]+"&&(ctau<=0)") <<  std::endl;
      cout <<  "[INFO] Number of entries in sideband mass window: " << ((RooDataSet*)data->reduce((parIni["BkgMassRange_FULL_Cut"]+"&&(ctau<=0)").c_str()))->numEntries() <<  std::endl;
      RooDataSet* dataw_Bkg_reduced = (RooDataSet*)data->reduce((parIni["BkgMassRange_FULL_Cut"]+"&&(ctau<=0)").c_str());
      ws.var(Form("N_Bkg_%s", (isPbPb?"PbPb":"PP")))->setVal(((RooDataSet*)data->reduce((parIni["BkgMassRange_FULL_Cut"]+"&&(ctau<=0)").c_str()))->numEntries());
      ws.import(*dataw_Bkg_reduced, Rename((dsName+"_BKG").c_str()));
    }
  }
  delete data; delete pdf;
  
  return true;
};

#endif // #ifndef fitCharmoniaCtauResDataModel_C
