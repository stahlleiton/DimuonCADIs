#ifndef fitCharmoniaMassModel_C
#define fitCharmoniaMassModel_C

#include "Utilities/initClasses.h"
#include "buildCharmoniaMassModel.C"
#include "drawMassPlot.C"

void setCtauCuts     ( struct KinCuts& cut, const bool& isPbPb );
bool setMassModel    ( struct OniaModel& model, const map<string, string>&  parIni, const bool& isPbPb, const bool& incJpsi, const bool& incPsi2S, const bool& incBkg );
void setMassFileName ( string& fileName, const string& outputDir, string TAG, const string& plotLabel,
                       const struct KinCuts& cut, const bool& isPbPb, const bool cutSideBand=false, const bool doSimulFit=false );
void setMassGlobalParameterRange ( RooWorkspace& myws, map<string, string>& parIni, const struct KinCuts& cut, const bool& incJpsi, const bool& incPsi2S, const bool& incBkg, const bool wantPureSMC=false );
void setMassCutParameters ( struct KinCuts& cut, const bool& incJpsi, const bool& incPsi2S, const bool isMC=false, const bool useForCtauFits=false );


bool fitCharmoniaMassModel( RooWorkspace& myws,                  // Local Workspace
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
                            // Select the fitting options
                                  bool  doFit,                   // Flag to indicate if we want to perform the fit
                            const bool& cutCtau,                 // Apply prompt ctau cuts
                            const bool& doConstrFit,             // Do constrained fit
                            const bool& doSimulFit,              // Do simultaneous fit
                                  bool  wantPureSMC,             // Flag to indicate if we want to fit pure signal MC
                            const string& applyCorr,             // Flag to indicate if we want corrected dataset and which correction
                            const uint& loadFitResult,           // Load previous fit results
                            const string& inputFitDir,           // Location of the fit results
                            const int&  numCores,                // Number of cores used for fitting
                            // Select the drawing options
                            const bool& setLogScale,             // Draw plot with log scale
                            const bool& incSS,                   // Include Same Sign data
                            const bool& zoomPsi,                 // Zoom Psi(2S) peak on extra pad
                            const double& binWidth,              // Bin width used for plotting
                            const bool& getMeanPT                // Compute the mean PT (NEED TO FIX)
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
  wantPureSMC = (isMC && wantPureSMC);
  const bool cutSideBand = (incBkg && (!incPsi2S && !incJpsi));
  const bool applyWeight_Corr = ( applyCorr!="" );
  
  // Define the mass range
  setMassCutParameters(cut, incJpsi, incPsi2S, isMC);
  parIni["invMassNorm"] = Form("RooFormulaVar::%s('( -1.0 + 2.0*( @0 - @1 )/( @2 - @1) )', {%s, mMin[%.6f], mMax[%.6f]})", "invMassNorm", "invMass", cut.dMuon.M.Min, cut.dMuon.M.Max );
  // Apply the ctau cuts to reject non-prompt charmonia
  if (cutCtau) { setCtauCuts(cut, isPbPb); }
  
  string COLL = (isPbPb ? "PbPb" : "PP" );
  string plotLabelPbPb,  plotLabelPP;

  if (doSimulFit || !isPbPb) {
    // Set models based on initial parameters
    struct OniaModel model;
    if (!setMassModel(model, parIni, false, incJpsi, incPsi2S, incBkg)) { return false; }

    // Import the local datasets
    double numEntries = 1000000;
    string label = ((DSTAG.find("PP")!=std::string::npos) ? DSTAG.c_str() : Form("%s_%s", DSTAG.c_str(), "PP"));
    if (wantPureSMC) label += "_NoBkg";
    if (applyWeight_Corr) label += Form("_%s",applyCorr.c_str());
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
    if (myws.data(dsName.c_str())) numEntries = myws.data(dsName.c_str())->sumEntries();

    // Set global parameters
    setMassGlobalParameterRange(myws, parIni, cut, incJpsi, incPsi2S, incBkg, wantPureSMC);

    // Build the Fit Model
    if (!buildCharmoniaMassModel(myws, model.PP, parIni, false, doConstrFit, doSimulFit, incBkg, incJpsi, incPsi2S, numEntries))  { return false; }

    // Define plot names
    if (incJpsi)  { plotLabelPP += Form("_Jpsi_%s", parIni["Model_Jpsi_PP"].c_str());   } 
    if (incPsi2S) { plotLabelPP += Form("_Psi2S_%s", parIni["Model_Psi2S_PP"].c_str()); }
    if (incBkg)   { plotLabelPP += Form("_Bkg_%s", parIni["Model_Bkg_PP"].c_str());     }
    if (wantPureSMC) plotLabelPP +="_NoBkg";
    if (applyWeight_Corr) plotLabelPP +=Form("_%s",applyCorr.c_str());
  }

  if (doSimulFit || isPbPb) {
    // Set models based on initial parameters
    struct OniaModel model;
    if (!setMassModel(model, parIni, true, incJpsi, incPsi2S, incBkg)) { return false; }

    // Import the local datasets
    double numEntries = 1000000;
    string label = ((DSTAG.find("PbPb")!=std::string::npos) ? DSTAG.c_str() : Form("%s_%s", DSTAG.c_str(), "PbPb"));
    if (wantPureSMC) label += "_NoBkg";
    if (applyWeight_Corr) label += Form("_%s",applyCorr.c_str());
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
    if (myws.data(dsName.c_str())) numEntries = myws.data(dsName.c_str())->sumEntries();
      
    // Set global parameters
    setMassGlobalParameterRange(myws, parIni, cut, incJpsi, incPsi2S, incBkg, wantPureSMC);

    // Build the Fit Model
    if (!buildCharmoniaMassModel(myws, model.PbPb, parIni, true, doConstrFit, doSimulFit, incBkg, incJpsi, incPsi2S, numEntries))  { return false; }

    // Define plot names
    if (incJpsi)  { plotLabelPbPb += Form("_Jpsi_%s", parIni["Model_Jpsi_PbPb"].c_str());   } 
    if (incPsi2S) { plotLabelPbPb += Form("_Psi2S_%s", parIni["Model_Psi2S_PbPb"].c_str()); }
    if (incBkg)   { plotLabelPbPb += Form("_Bkg_%s", parIni["Model_Bkg_PbPb"].c_str());     }
    if (wantPureSMC) plotLabelPbPb += "_NoBkg";
    if (applyWeight_Corr) plotLabelPbPb += Form("_%s",applyCorr.c_str());
  }

  if (doSimulFit) {
    // Create the combided datasets
    auto sample = std::unique_ptr<RooCategory>(new RooCategory("sample","sample")); sample->defineType("PbPb"); sample->defineType("PP");
    auto combData = std::unique_ptr<RooDataSet>(new RooDataSet("combData","combined data", *myws.var("invMass"), Index(*sample),
                                                               Import("PbPb", *((RooDataSet*)myws.data("dOS_DATA_PbPb"))),
                                                               Import("PP",   *((RooDataSet*)myws.data("dOS_DATA_PP")))
                                                               ));
    myws.import(*sample);

    // Create the combided models
    auto simPdf = std::unique_ptr<RooSimultaneous>(new RooSimultaneous("simPdf", "simultaneous pdf", *sample));
    simPdf->addPdf(*myws.pdf("pdfMASS_Tot_PbPb"), "PbPb"); simPdf->addPdf(*myws.pdf("pdfMASS_Tot_PP"), "PP");
    myws.import(*simPdf);

    // check if we have already done this fit. If yes, do nothing and return true.
    string fileName = "";
    setMassFileName(fileName, (inputFitDir=="" ? outputDir : inputFitDir), DSTAG, (plotLabelPP + plotLabelPbPb), cut, isPbPb, cutSideBand, doSimulFit);
    if (gSystem->AccessPathName(fileName.c_str()) && inputFitDir!="") {
      cout << "[WARNING] User Input File : " << fileName << " was not found!" << endl;
      if (loadFitResult) return false;
      setMassFileName(fileName, outputDir, DSTAG, (plotLabelPP + plotLabelPbPb), cut, isPbPb, cutSideBand, doSimulFit);
    }
    bool found =  true; bool skipFit = !doFit;
    std::unique_ptr<RooArgSet> newpars = std::unique_ptr<RooArgSet>(myws.pdf("simPdf")->getParameters(*myws.var("invMass")));
    myws.saveSnapshot("simPdf_parIni", *newpars, kTRUE);
    found = found && isFitAlreadyFound(*newpars, fileName, "simPdf");
    if (loadFitResult) {
      if ( loadPreviousFitResult(myws, fileName, DSTAG, false, (!isMC && !cutSideBand && loadFitResult==1), loadFitResult==1) ) { skipFit = true; } else { skipFit = false; }
      if ( loadPreviousFitResult(myws, fileName, DSTAG, true, (!isMC && !cutSideBand && loadFitResult==1), loadFitResult==1)  ) { skipFit = true; } else { skipFit = false; }
      if (skipFit) { cout << "[INFO] This simultaneous mass fit was already done, so I'll load the fit results." << endl; }
      myws.saveSnapshot("simPdf_parLoad", *newpars, kTRUE);
    } else if (found) {
      cout << "[INFO] This simultaneous mass fit was already done, so I'll just go to the next one." << endl;
      return true;
    }

    // Do the simultaneous fit
    if (skipFit==false) {
      auto fitResult = std::unique_ptr<RooFitResult>(simPdf->fitTo(*combData, Offset(kTRUE), Extended(kTRUE), NumCPU(numCores), Range("MassWindow"), Save())); //, Minimizer("Minuit2","Migrad")
      fitResult->Print("v");
      myws.import(*fitResult, "fitResult_simPdf"); 
      // Create the output files
      const int nBins = min(int( round((cut.dMuon.M.Max - cut.dMuon.M.Min)/binWidth) ), 1000);
      drawMassPlot(myws, outputDir, opt, cut, parIni, plotLabelPP, DSTAG, false, incJpsi, incPsi2S, incBkg, cutCtau, doSimulFit, false, setLogScale, incSS, zoomPsi, nBins, getMeanPT);
      drawMassPlot(myws, outputDir, opt, cut, parIni, plotLabelPbPb, DSTAG, true, incJpsi, incPsi2S, incBkg, cutCtau, doSimulFit, false, setLogScale, incSS, zoomPsi, nBins, getMeanPT);
      // Save the results
      string fileName = ""; setMassFileName(fileName, outputDir, DSTAG, (plotLabelPP + plotLabelPbPb), cut, isPbPb, cutSideBand, doSimulFit);
      myws.saveSnapshot("simPdf_parFit", *newpars, kTRUE);
      saveWorkSpace(myws, Form("%smass%s/%s/result", outputDir.c_str(), (cutSideBand?"SB":""), DSTAG.c_str()), fileName);
    }
  }
  else {
    // Define pdf and plot names
    const string pdfName = Form("pdfMASS_Tot_%s", COLL.c_str());
    const string plotLabel = (isPbPb ? plotLabelPbPb : plotLabelPP);

    // Import the local datasets
    string label = ((DSTAG.find(COLL.c_str())!=std::string::npos) ? DSTAG.c_str() : Form("%s_%s", DSTAG.c_str(), COLL.c_str()));
    if (wantPureSMC) label += "_NoBkg";
    if (applyWeight_Corr) label += Form("_%s",applyCorr.c_str());
    const string dsName = Form("dOS_%s", label.c_str());
      
    // check if we have already done this fit. If yes, do nothing and return true.
    string fileName = "";
    setMassFileName(fileName, (inputFitDir=="" ? outputDir : inputFitDir), DSTAG, plotLabel, cut, isPbPb, cutSideBand);
    if (gSystem->AccessPathName(fileName.c_str()) && inputFitDir!="") {
      cout << "[WARNING] User Input File : " << fileName << " was not found!" << endl;
      if (loadFitResult) return false;
      setMassFileName(fileName, outputDir, DSTAG, plotLabel, cut, isPbPb, cutSideBand);
    }
    bool found =  true; bool skipFit = !doFit;
    std::unique_ptr<RooArgSet> newpars = std::unique_ptr<RooArgSet>(myws.pdf(pdfName.c_str())->getParameters(*(myws.data(dsName.c_str()))));
    found = found && isFitAlreadyFound(*newpars, fileName, pdfName.c_str());
    if (loadFitResult) {
      if ( loadPreviousFitResult(myws, fileName, DSTAG, isPbPb, (!isMC && !cutSideBand && loadFitResult==1), loadFitResult==1) ) { skipFit = true; } else { skipFit = false; } 
      if (skipFit) { cout << "[INFO] This mass fit was already done, so I'll load the fit results." << endl; }
      myws.saveSnapshot(Form("%s_parLoad", pdfName.c_str()), *newpars, kTRUE);
    } else if (found) {
      cout << "[INFO] This mass fit was already done, so I'll just go to the next one." << endl;
      return true;
    }

    // Fit the Datasets
    if (skipFit==false) {
      const bool isWeighted = myws.data(dsName.c_str())->isWeighted();
      std::unique_ptr<RooFitResult> fitResult;
      if (doConstrFit)
      {
        cout << "[INFO] Performing constrained fit" << endl;
        
        if (isPbPb) {
          cout << "[INFO] Constrained variables: alpha, n, ratio of sigmas" << endl;
          fitResult.reset(myws.pdf(pdfName.c_str())->fitTo(*myws.data(dsName.c_str()), Extended(kTRUE), SumW2Error(isWeighted), Range(cutSideBand ? parIni["BkgMassRange_FULL_Label"].c_str() : "MassWindow"), ExternalConstraints(RooArgSet(*(myws.pdf("sigmaAlphaConstr")),*(myws.pdf("sigmaNConstr")),*(myws.pdf("sigmaRSigmaConstr")))), NumCPU(numCores), Save()));
        }
        else {
          cout << "[INFO] Constrained variables: alpha, n, ratio of sigmas" << endl;
          fitResult.reset(myws.pdf(pdfName.c_str())->fitTo(*myws.data(dsName.c_str()), Extended(kTRUE), SumW2Error(isWeighted), Range(cutSideBand ? parIni["BkgMassRange_FULL_Label"].c_str() : "MassWindow"), ExternalConstraints(RooArgSet(*(myws.pdf("sigmaAlphaConstr")),*(myws.pdf("sigmaNConstr")))), NumCPU(numCores), Save()));
        }
      }
      else
      {
        fitResult.reset(myws.pdf(pdfName.c_str())->fitTo(*myws.data(dsName.c_str()), Extended(kTRUE), SumW2Error(isWeighted), Range(cutSideBand ? parIni["BkgMassRange_FULL_Label"].c_str() : "MassWindow"), NumCPU(numCores), Save()));
      }
      fitResult->Print("v"); 
      myws.import(*fitResult, Form("fitResult_%s", pdfName.c_str())); 
      // Create the output files
      const int nBins = min(int( round((cut.dMuon.M.Max - cut.dMuon.M.Min)/binWidth) ), 1000);
      drawMassPlot(myws, outputDir, opt, cut, parIni, plotLabel, DSTAG, isPbPb, incJpsi, incPsi2S, incBkg, cutCtau, doSimulFit, wantPureSMC, setLogScale, incSS, zoomPsi, nBins, getMeanPT);
      // Save the results
      string fileName = ""; setMassFileName(fileName, outputDir, DSTAG, plotLabel, cut, isPbPb, cutSideBand);
      myws.saveSnapshot(Form("%s_parFit", pdfName.c_str()), *newpars, kTRUE);
      saveWorkSpace(myws, Form("%smass%s/%s/result", outputDir.c_str(), (cutSideBand?"SB":""), DSTAG.c_str()), fileName);
    }
  }
  return true;
};


void setCtauCuts(struct KinCuts& cut, const bool& isPbPb) 
{
  if (cut.dMuon.AbsRap.Max<=1.6) {
    cut.dMuon.ctauCut = "( ctau < (0.012 + (0.23/pt)) )";
  }
  if (cut.dMuon.AbsRap.Min>=1.6) {
    cut.dMuon.ctauCut = "( ctau < (0.014 + (0.28/pt)) )";
  }
  return;
};


bool setMassModel(struct OniaModel& model, const map<string, string>&  parIni, const bool& isPbPb, const bool& incJpsi, const bool& incPsi2S, const bool& incBkg)
{
  if (isPbPb && incBkg) {
    if (parIni.count("Model_Bkg_PbPb")>0) {
      model.PbPb.Bkg.Mass = MassModelDictionary[parIni.at("Model_Bkg_PbPb")];
      if (model.PbPb.Bkg.Mass==MassModel(0)) {
        cout << "[ERROR] The background model: " << parIni.at("Model_Bkg_PbPb") << " is invalid" << endl; return false;
      }
    } else { 
      cout << "[ERROR] Background mass model for PbPb was not found in the initial parameters!" << endl; return false;
    }
  }
  if (isPbPb && incJpsi) {
    if (parIni.count("Model_Jpsi_PbPb")>0) {
      model.PbPb.Jpsi.Mass = MassModelDictionary[parIni.at("Model_Jpsi_PbPb")];
      if (model.PbPb.Jpsi.Mass==MassModel(0)) {
        cout << "[ERROR] The Jpsi model: " << parIni.at("Model_Jpsi_PbPb") << " is invalid" << endl; return false;
      }
    } else { 
      cout << "[ERROR] Jpsi mass model for PbPb was not found in the initial parameters!" << endl; return false;
    }
  }
  if (isPbPb && incPsi2S) {
    if (parIni.count("Model_Psi2S_PbPb")>0) {
      model.PbPb.Psi2S.Mass = MassModelDictionary[parIni.at("Model_Psi2S_PbPb")];
      if (model.PbPb.Psi2S.Mass==MassModel(0)) {
        cout << "[ERROR] The psi2S model: " << parIni.at("Model_Psi2S_PbPb") << " is invalid" << endl; return false;
      }
    } else { 
      cout << "[ERROR] psi(2S) mass model for PbPb was not found in the initial parameters!" << endl; return false;
    }
  }
  if (!isPbPb && incBkg) {
    if (parIni.count("Model_Bkg_PP")>0) {
      model.PP.Bkg.Mass = MassModelDictionary[parIni.at("Model_Bkg_PP")];
      if (model.PP.Bkg.Mass==MassModel(0)) {
        cout << "[ERROR] The background model: " << parIni.at("Model_Bkg_PP") << " is invalid" << endl; return false;
      }
    } else { 
      cout << "[ERROR] Background mass model for PP was not found in the initial parameters!" << endl; return false;
    }
  }
  if (!isPbPb && incJpsi) {
    if (parIni.count("Model_Jpsi_PP")>0) {
      model.PP.Jpsi.Mass = MassModelDictionary[parIni.at("Model_Jpsi_PP")];
      if (model.PP.Jpsi.Mass==MassModel(0)) {
        cout << "[ERROR] The Jpsi model: " << parIni.at("Model_Jpsi_PbPb") << " is invalid" << endl; return false;
      }
    } else { 
      cout << "[ERROR] Jpsi mass model for PP was not found in the initial parameters!" << endl; return false;
    }
  }
  if (!isPbPb && incPsi2S) {
    if (parIni.count("Model_Psi2S_PP")>0) {
      model.PP.Psi2S.Mass = MassModelDictionary[parIni.at("Model_Psi2S_PP")];
      if (model.PP.Psi2S.Mass==MassModel(0)) {
        cout << "[ERROR] The psi2S model: " << parIni.at("Model_Psi2S_PbPb") << " is invalid" << endl; return false;
      }
    } else { 
      cout << "[ERROR] psi(2S) mass model for PP was not found in the initial parameters!" << endl; return false;
    }
  }
  return true;
};


void setMassGlobalParameterRange(RooWorkspace& myws, map<string, string>& parIni, const struct KinCuts& cut, const bool& incJpsi, const bool& incPsi2S, const bool& incBkg,  const bool wantPureSMC)
{
  if (wantPureSMC)
    {
      if (incPsi2S)
        {
          if (cut.dMuon.AbsRap.Min >= 1.6) {
            myws.var("invMass")->setRange("MassWindow", cut.dMuon.M.Min, 3.95);
            parIni["MassRange_Cut"] = Form("(invMass>%.6f && invMass<%.6f)", cut.dMuon.M.Min, 3.95);
          }
          else { 
            myws.var("invMass")->setRange("MassWindow", cut.dMuon.M.Min, 3.85);
            parIni["MassRange_Cut"] = Form("(invMass>%.6f && invMass<%.6f)", cut.dMuon.M.Min, 3.85);
          }
        }
      if (incJpsi)
        {
          if (cut.dMuon.AbsRap.Min >= 1.6) {
            myws.var("invMass")->setRange("MassWindow", cut.dMuon.M.Min, 3.32);
            parIni["MassRange_Cut"] = Form("(invMass>%.6f && invMass<%.6f)", cut.dMuon.M.Min, 3.32);
          }
          else {
            myws.var("invMass")->setRange("MassWindow", cut.dMuon.M.Min, 3.26);
            parIni["MassRange_Cut"] = Form("(invMass>%.6f && invMass<%.6f)", cut.dMuon.M.Min, 3.26);
          }
        }
    }
  else {
    myws.var("invMass")->setRange("InclusiveMassRegion", 2.6, cut.dMuon.M.Max);
    myws.var("invMass")->setRange("FullWindow", cut.dMuon.M.Min, cut.dMuon.M.Max);
    myws.var("invMass")->setRange("MassWindow", cut.dMuon.M.Min, cut.dMuon.M.Max);
    parIni["MassRange_Cut"] = Form("(invMass>%.6f && invMass<%.6f)", cut.dMuon.M.Min, cut.dMuon.M.Max);
    if (incJpsi) {
      myws.var("invMass")->setRange("JpsiWindow", 2.9, 3.3);
      parIni["JpsiMassRange_Cut"] = Form("(invMass>%.6f && invMass<%.6f)", 2.9, 3.3);
    }
    if (incPsi2S) {
      myws.var("invMass")->setRange("Psi2SWindow", 3.5, 3.9);
      parIni["Psi2SMassRange_Cut"] = Form("(invMass>%.6f && invMass<%.6f)", 3.5, 3.9);
    }
    if (incBkg) {
      myws.var("invMass")->setRange("SideBandMID_FULL",  ((cut.dMuon.M.Min<3.3)?3.3:cut.dMuon.M.Min), ((cut.dMuon.M.Max>3.5)?3.5:cut.dMuon.M.Max));
      myws.var("invMass")->setRange("SideBandMID_JPSI",  ((cut.dMuon.M.Min<3.3)?3.3:cut.dMuon.M.Min), ((cut.dMuon.M.Max>3.5)?3.5:cut.dMuon.M.Max));
      myws.var("invMass")->setRange("SideBandMID_PSI2S", ((cut.dMuon.M.Min<3.4)?3.4:cut.dMuon.M.Min), ((cut.dMuon.M.Max>3.5)?3.5:cut.dMuon.M.Max));
      parIni["BkgMassRange_FULL_Label"]  = "SideBandMID_FULL";
      parIni["BkgMassRange_JPSI_Label"]  = "SideBandMID_JPSI";
      parIni["BkgMassRange_PSI2S_Label"] = "SideBandMID_PSI2S";
      if (cut.dMuon.M.Min < 2.8) {
        myws.var("invMass")->setRange("SideBandBOT_FULL", cut.dMuon.M.Min, 2.8);
        myws.var("invMass")->setRange("SideBandBOT_JPSI", ((cut.dMuon.M.Min<2.0)?2.0:cut.dMuon.M.Min), 2.8);
        parIni["BkgMassRange_FULL_Label"] = parIni["BkgMassRange_FULL_Label"] + "," + "SideBandBOT_FULL";
        parIni["BkgMassRange_JPSI_Label"] = parIni["BkgMassRange_JPSI_Label"] + "," + "SideBandBOT_JPSI";
      }
      if (cut.dMuon.M.Max > 3.9) {
        myws.var("invMass")->setRange("SideBandTOP_FULL", 3.9, cut.dMuon.M.Max);
        myws.var("invMass")->setRange("SideBandTOP_PSI2S", 3.9, ((cut.dMuon.M.Max>4.2)?4.2:cut.dMuon.M.Max));
        parIni["BkgMassRange_FULL_Label"] = parIni["BkgMassRange_FULL_Label"] + "," + "SideBandTOP_FULL";
        parIni["BkgMassRange_PSI2S_Label"] = parIni["BkgMassRange_PSI2S_Label"] + "," + "SideBandTOP_PSI2S";
      }
      parIni["BkgMassRange_FULL_Cut"]  = Form("(%.6f < invMass && invMass < %.6f)",       cut.dMuon.M.Min,       cut.dMuon.M.Max);
      parIni["BkgMassRange_FULL_Cut"]  = parIni["BkgMassRange_FULL_Cut"]  + "&&" + "((2.0 < invMass && invMass < 2.8) || (3.3 < invMass && invMass < 3.5) || (3.9 < invMass && invMass < 5.0))";
      parIni["BkgMassRange_JPSI_Cut"]  = parIni["BkgMassRange_FULL_Cut"]  + "&&" + "(2.0 < invMass && invMass < 3.5)";
      parIni["BkgMassRange_PSI2S_Cut"] = parIni["BkgMassRange_FULL_Cut"] + "&&" + "(3.4 < invMass && invMass < 5.0)";
      parIni["BkgMassRange_FULL_Cut"]  = "("+parIni["BkgMassRange_FULL_Cut"]+")";
      parIni["BkgMassRange_JPSI_Cut"]  = "("+parIni["BkgMassRange_JPSI_Cut"]+")";
      parIni["BkgMassRange_PSI2S_Cut"] = "("+parIni["BkgMassRange_PSI2S_Cut"]+")";
    }
  }
  return;
};

void setMassFileName(string& fileName, const string& outputDir, string TAG, const string& plotLabel, const struct KinCuts& cut, const bool& isPbPb, const bool cutSideBand, const bool doSimulFit) 
{
  if (TAG.find("_")!=std::string::npos) TAG.erase(TAG.find("_"));
  fileName = Form("%smass%s/%s/result/FIT_%s_%s_%s%s_pt%.0f%.0f_rap%.0f%.0f_cent%d%d.root", outputDir.c_str(), (cutSideBand?"SB":""), TAG.c_str(), "MASS", TAG.c_str(), (doSimulFit ? "COMB" : (isPbPb?"PbPb":"PP")), plotLabel.c_str(), (cut.dMuon.Pt.Min*10.0), (cut.dMuon.Pt.Max*10.0), (cut.dMuon.AbsRap.Min*10.0), (cut.dMuon.AbsRap.Max*10.0), cut.Centrality.Start, cut.Centrality.End);
};


void setMassCutParameters(struct KinCuts& cut, const bool& incJpsi, const bool& incPsi2S, const bool isMC, const bool useForCtauFits)
{
  // Define the mass range
  if (cut.dMuon.M.Max==5 && cut.dMuon.M.Min==2) { 
    // Default mass values, means that the user did not specify a mass range
    if ( incJpsi && !incPsi2S) {
      if (isMC && !useForCtauFits){
        cut.dMuon.M.Min = 2.6;
        cut.dMuon.M.Max = 3.5;
      } else {
        cut.dMuon.M.Min = 2.6;
        cut.dMuon.M.Max = 3.5;
      }
    }
    else if ( !incJpsi && incPsi2S) {
      if(isMC && !useForCtauFits) {
        cut.dMuon.M.Min = 2.8;
        cut.dMuon.M.Max = 4.0;
      } else {
        cut.dMuon.M.Min = 3.4;
        cut.dMuon.M.Max = 4.0;
      }
    }
    else {
      cut.dMuon.M.Min = 2.6;
      cut.dMuon.M.Max = 4.0;
    }
  }
  cout << "[INFO] Setting mass range to min: " << cut.dMuon.M.Min << " and max " << cut.dMuon.M.Max << endl;
  return;
};


#endif // #ifndef fitCharmoniaMassModel_C
