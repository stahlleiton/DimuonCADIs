#include "Utilities/initClasses.h"
#include "buildCharmoniaMassModel.C"
#include "buildCharmoniaCtauModel.C"
#include "buildCharmoniaCtauTrueModel.C"
#include "drawMassPlot.C"
#include "drawCtauPlot.C"
#include "drawCtauTruePlot.C"

#include <algorithm>

void setCtauCuts(struct KinCuts& cut, bool isPbPb);
int  importDataset(RooWorkspace& myws, RooWorkspace& inputWS, struct KinCuts& cut, string label, map<string, struct RangeStruc>& rangeMap, bool cutSideBand=false, bool fitCtau=false);
bool setMassModel( struct OniaModel& model, map<string, string>&  parIni, bool isPbPb, bool incJpsi, bool incPsi2S, bool incBkg);
bool setCtauModel( struct OniaModel& model, map<string, string>&  parIni, bool isPbPb, bool incJpsi, bool incPsi2S, bool incBkg, bool incPrompt, bool incNonPrompt);
bool setCtauTrueModel( struct OniaModel& model, map<string, string>&  parIni, bool isPbPb );

void setOptions(struct InputOpt* opt);
void setFileName(string& FileName, string outputDir, string TAG, string plotLabel, struct KinCuts cut, bool fitMass, bool fitCtau, bool fitCtauTrue, bool doSimulFit, bool isPbPb=true, bool cutSideBand=false);
bool saveWorkSpace(RooWorkspace& myws, string outputDir, string plotLabel, string DSTAG, struct KinCuts cut, bool fitMass, bool fitCtau, bool fitCtauTrue, bool doSimulFit, bool isPbPb=true);

bool isFitAlreadyFound(RooArgSet *newpars, string outputDir, string plotLabel, string DSTAG, struct KinCuts cut, bool fitMass, bool fitCtau, bool fitCtauTrue, bool doSimulFit, bool isPbPb=true);
bool loadPreviousFitResult(RooWorkspace& myws, string outputDir, string plotLabel, string DSTAG, struct KinCuts cut, bool fitMass, bool fitCtau, bool fitCtauTrue, bool doSimulFit, bool isPbPb=true);
bool compareSnapshots(RooArgSet *pars1, const RooArgSet *pars2);

bool fitCharmonia( RooWorkspace&  inputWorkspace, // Workspace with all the input RooDatasets
		   struct KinCuts cut,            // Variable containing all kinematic cuts
		   map<string, string>  parIni,   // Variable containing all initial parameters
		   string outputDir,              // Path to output directory
                   // Select the type of datasets to fit
		   string DSTAG,                  // Specifies the type of datasets: i.e, DATA, MCJPSINP, ...
		   bool isPbPb      = false,      // isPbPb = false for pp, true for PbPb
                   // Select the type of object to fit                   
                   bool fitMass      = true,      // Fit mass distribution
                   bool fitCtau      = false,     // Fit ctau distribution
                   bool fitCtauTrue  = false,     // Fit ctau truth MC distribution
                   bool incJpsi      = true,      // Includes Jpsi model
                   bool incPsi2S     = true,      // Includes Psi(2S) model
                   bool incBkg       = true,      // Includes Background model
                   bool incPrompt    = true,      // Includes Prompt ctau model
                   bool incNonPrompt = false,     // Includes NonPrompt ctau model
                   // Select the fitting options
                   bool cutCtau      = false,     // Apply prompt ctau cuts
                   bool doSimulFit   = false,     // Do simultaneous fit
                   bool wantPureSMC  = false,     // Flag to indicate if we want to fit pure signal MC
                   int  numCores     = 32,         // Number of cores used for fitting
                   // Select the drawing options
                   bool  setLogScale  = true,      // Draw plot with log scale
                   bool  incSS        = false,     // Include Same Sign data
                   bool  zoomPsi      = false,     // Zoom Psi(2S) peak on extra pad
                   float binWidth     = 0.05,      // Bin width used for plotting
                   bool  getMeanPT    = false      // Compute the mean PT (NEED TO FIX)
		   )  
{
 
  RooMsgService::instance().getStream(0).removeTopic(Caching);  
  RooMsgService::instance().getStream(1).removeTopic(Caching);
  RooMsgService::instance().getStream(0).removeTopic(Plotting);
  RooMsgService::instance().getStream(1).removeTopic(Plotting);
  RooMsgService::instance().getStream(0).removeTopic(Integration);
  RooMsgService::instance().getStream(1).removeTopic(Integration);
  RooMsgService::instance().setGlobalKillBelow(RooFit::WARNING) ;
  
  // Define the mass range
  if (cut.dMuon.M.Max==5 && cut.dMuon.M.Min==2) { 
    // Default mass values, means that the user did not specify a mass range
    if ( incJpsi && !incPsi2S) {
      cut.dMuon.M.Min = 2.6;
      cut.dMuon.M.Max = 3.5;
    }
    else if ( !incJpsi && incPsi2S) {
      cut.dMuon.M.Min = 3.0;
      cut.dMuon.M.Max = 4.1;
    }
    else {
      cut.dMuon.M.Min = 2.2;
      cut.dMuon.M.Max = 4.5;
    }
  }
  parIni["invMassNorm"] = Form("RooFormulaVar::%s('( -1.0 + 2.0*( @0 - @1 )/( @2 - @1) )', {%s, mMin[%.6f], mMax[%.6f]})", "invMassNorm", "invMass", cut.dMuon.M.Min, cut.dMuon.M.Max );
  bool cutSideBand = false;
  if (incBkg && (!incPsi2S && !incJpsi)) { cutSideBand = true; }
  
  // Define the mass range
  if (fitCtau) {
    if (cut.dMuon.ctau.Min==-50. && cut.dMuon.ctau.Max==50.) { 
      // Default ctau values, means that the user did not specify a ctau range
      if (incNonPrompt) {
        cut.dMuon.ctau.Min = -10.0;
        cut.dMuon.ctau.Max = 10.0;
      } else {
        cut.dMuon.ctau.Min = -2.0;
        cut.dMuon.ctau.Max = 2.0;
      }
    }
    if (cut.dMuon.ctauErr.Min==-50. && cut.dMuon.ctauErr.Max==50.) { 
      // Default ctau values, means that the user did not specify a ctau Err range
      if (isPbPb) {
        cut.dMuon.ctauErr.Min = 0.00000001; 
        cut.dMuon.ctauErr.Max = 1.0;
      } else {
        cut.dMuon.ctauErr.Min = 0.00000001; 
        cut.dMuon.ctauErr.Max = 1.0;
      }
    }
  }
  // Define the mass range
  if (fitCtauTrue) {
    if (cut.dMuon.ctauTrue.Min==-50.0 && cut.dMuon.ctauTrue.Max==50.0) { 
      // Default ctau values, means that the user did not specify a ctau True range
      cut.dMuon.ctauTrue.Min = -1.0;
      cut.dMuon.ctauTrue.Max = 10.0;
    }
  }
  // Apply the ctau cuts to reject non-prompt charmonia
  if (cutCtau) { setCtauCuts(cut, isPbPb); }  

  // Check if input dataset is MC
  bool isMC = false;
  if (DSTAG.find("MC")!=std::string::npos) {
    if (incJpsi && incPsi2S) { 
      cout << "[ERROR] We can not fit Jpsi and Psi(2S) simultaneously using MC" << endl; return false; 
    }
    if (incPrompt && incNonPrompt) { 
      cout << "[ERROR] We can not fit Prompt and Non-Prompt simultaneously using MC" << endl; return false; 
    }
    isMC = true;
  }
  if (isMC && wantPureSMC) wantPureSMC=true;
  else wantPureSMC=false;

  struct InputOpt opt; setOptions(&opt);
  
  string plotMassLabelPbPb,  plotMassLabelPP, plotCtauLabelPbPb,  plotCtauLabelPP, plotCtauTrueLabelPbPb,  plotCtauTrueLabelPP;

  struct OniaModel model;
  map<string, struct RangeStruc> rangeMap;
  RooWorkspace     myws("workspace", "local workspace");

  bool doFit = true;
  if (doSimulFit || !isPbPb) {
    
    // Set models based on initial parameters
    if (fitMass)     { if (!setMassModel(model, parIni, false, incJpsi, incPsi2S, incBkg)) { return false; } }
    if (fitCtau)     { if (!setCtauModel(model, parIni, false, incJpsi, incPsi2S, incBkg, incPrompt, incNonPrompt)) { return false; } }
    if (fitCtauTrue) { if (!setCtauTrueModel(model, parIni, false)) { return false; } }
    
    // Import the local datasets
    string label = Form("%s_%s", DSTAG.c_str(), "PP");
    if (wantPureSMC) label = Form("%s_%s_NoBkg", DSTAG.c_str(), "PP");
    string dsName = Form("dOS_%s", label.c_str());
    int importID = importDataset(myws, inputWorkspace, cut, label, rangeMap, cutSideBand, fitCtau);
    if (importID<0) { return false; }
    else if (importID==0) { doFit = false; }
    
    // Build the Fit Model    
    double numEntries = myws.data(dsName.c_str())->sumEntries();
    if (fitMass)     { if (!buildCharmoniaMassModel(myws, model.PP, parIni, false, doSimulFit, incBkg, incJpsi, incPsi2S, numEntries))  { return false; } }
    if (fitCtau)     { if (!buildCharmoniaCtauModel(myws, model.PP, parIni, false, incBkg, incJpsi, incPsi2S, incPrompt, incNonPrompt, numEntries))  { return false; } }
    if (fitCtauTrue) { if (!buildCharmoniaCtauTrueModel(myws, model.PP, parIni, false, incJpsi, incPsi2S, numEntries))  { return false; } }

    if (fitMass) {
      if (incJpsi)  { plotMassLabelPP = plotMassLabelPP + Form("_Jpsi_%s", parIni["Model_Jpsi_PP"].c_str());   } 
      if (incPsi2S) { plotMassLabelPP = plotMassLabelPP + Form("_Psi2S_%s", parIni["Model_Psi2S_PP"].c_str()); }
      if (incBkg)   { plotMassLabelPP = plotMassLabelPP + Form("_Bkg_%s", parIni["Model_Bkg_PP"].c_str());     }
    }
    if (fitCtau) {
      if (incJpsi  && incPrompt)    { plotCtauLabelPP = plotCtauLabelPP + Form("_JpsiPR_%s", parIni["Model_JpsiPR_PP"].c_str());      } 
      if (incJpsi  && incNonPrompt) { plotCtauLabelPP = plotCtauLabelPP + Form("_JpsiNoPR_%s", parIni["Model_JpsiNoPR_PP"].c_str());  }
      if (incPsi2S && incPrompt)    { plotCtauLabelPP = plotCtauLabelPP + Form("_Psi2SPR_%s", parIni["Model_Psi2SPR_PP"].c_str());    } 
      if (incPsi2S && incNonPrompt) { plotCtauLabelPP = plotCtauLabelPP + Form("_Psi2SNoPR_%s", parIni["Model_Psi2SNoPR_PP"].c_str());}
      if (incBkg && incPrompt)      { plotCtauLabelPP = plotCtauLabelPP + Form("_BkgPR_%s", parIni["Model_BkgPR_PP"].c_str());        } 
      if (incBkg && incNonPrompt)   { plotCtauLabelPP = plotCtauLabelPP + Form("_BkgNoPR_%s", parIni["Model_BkgNoPR_PP"].c_str());    }
    }
    if (fitCtauTrue) {
      plotCtauTrueLabelPP = string("") + Form("_CtauTrueRes_%s", parIni["Model_CtauTrueRes_PP"].c_str()) + Form("_CtauTrue_%s", parIni["Model_CtauTrue_PP"].c_str());
    }
  }
  if (doSimulFit || isPbPb) {
    
    // Set models based on initial parameters
    if (fitMass)     { if (!setMassModel(model, parIni, true, incJpsi, incPsi2S, incBkg)) { return false; } }
    if (fitCtau)     { if (!setCtauModel(model, parIni, true, incJpsi, incPsi2S, incBkg, incPrompt, incNonPrompt)) { return false; } }
    if (fitCtauTrue) { if (!setCtauTrueModel(model, parIni, true)) { return false; } }
    
    // Import the local datasets
    string label = Form("%s_%s", DSTAG.c_str(), "PbPb");
    if (wantPureSMC) label = Form("%s_%s_NoBkg", DSTAG.c_str(), "PbPb");
    string dsName = Form("dOS_%s", label.c_str());
    int importID = importDataset(myws, inputWorkspace, cut, label, rangeMap, cutSideBand, fitCtau);
    if (importID<0) { return false; }
    else if (importID==0) { doFit = false; }
    
    // Build the Fit Model
    double    numEntries = myws.data(dsName.c_str())->sumEntries();
    if (fitMass) { if (!buildCharmoniaMassModel(myws, model.PbPb, parIni, true, doSimulFit, incBkg, incJpsi, incPsi2S, numEntries)) { return false; } }
    if (fitCtau) { if (!buildCharmoniaCtauModel(myws, model.PbPb, parIni, true, incBkg, incJpsi, incPsi2S, incPrompt, incNonPrompt, numEntries)) { return false; } }
    if (fitCtauTrue) { if (!buildCharmoniaCtauTrueModel(myws, model.PbPb, parIni, true, incJpsi, incPsi2S, numEntries))  { return false; } }

    if (fitMass) {
      if (incJpsi)  { plotMassLabelPbPb = plotMassLabelPbPb + Form("_Jpsi_%s", parIni["Model_Jpsi_PbPb"].c_str());   } 
      if (incPsi2S) { plotMassLabelPbPb = plotMassLabelPbPb + Form("_Psi2S_%s", parIni["Model_Psi2S_PbPb"].c_str()); }
      if (incBkg)   { plotMassLabelPbPb = plotMassLabelPbPb + Form("_Bkg_%s", parIni["Model_Bkg_PbPb"].c_str());     }
    }
    if (fitCtau) {
      if (incJpsi  && incPrompt)    { plotCtauLabelPbPb = plotCtauLabelPbPb + Form("_JpsiPR_%s", parIni["Model_JpsiPR_PbPb"].c_str());      } 
      if (incJpsi  && incNonPrompt) { plotCtauLabelPbPb = plotCtauLabelPbPb + Form("_JpsiNoPR_%s", parIni["Model_JpsiNoPR_PbPb"].c_str());  }
      if (incPsi2S && incPrompt)    { plotCtauLabelPbPb = plotCtauLabelPbPb + Form("_Psi2SPR_%s", parIni["Model_Psi2SPR_PbPb"].c_str());    } 
      if (incPsi2S && incNonPrompt) { plotCtauLabelPbPb = plotCtauLabelPbPb + Form("_Psi2SNoPR_%s", parIni["Model_Psi2SNoPR_PbPb"].c_str());}
      if (incBkg && incPrompt)      { plotCtauLabelPbPb = plotCtauLabelPbPb + Form("_BkgPR_%s", parIni["Model_BkgPR_PbPb"].c_str());        } 
      if (incBkg && incNonPrompt)   { plotCtauLabelPbPb = plotCtauLabelPbPb + Form("_BkgNoPR_%s", parIni["Model_BkgNoPR_PbPb"].c_str());    }
    }
    if (fitCtauTrue) {
      plotCtauTrueLabelPbPb = string("") + Form("_CtauTrueRes_%s", parIni["Model_CtauTrueRes_PbPb"].c_str()) + Form("_CtauTrue_%s", parIni["Model_CtauTrue_PbPb"].c_str());
    }
  }

  if (doFit)
  {
    if (doSimulFit) {
      if (fitMass || fitCtau) {
        string plotLabel = ""; string pdfName = "";
        if (fitMass && !fitCtau) { pdfName = "pdfMASS";     plotLabel = plotMassLabelPbPb;                    }
        if (fitMass && !fitCtau) { pdfName = "pdfMASS";     plotLabel = plotMassLabelPbPb;                    }
        if (!fitMass && fitCtau) { pdfName = "pdfCTAU";     plotLabel = plotCtauLabelPbPb;                    }
        if (fitMass &&  fitCtau) { pdfName = "pdfCTAUMASS"; plotLabel = plotMassLabelPbPb+plotCtauLabelPbPb;  }
        
        // Create the combided datasets and models
        RooCategory* sample = new RooCategory("sample","sample"); sample->defineType("PbPb"); sample->defineType("PP");
        RooDataSet*  combData = new RooDataSet("combData","combined data", RooArgSet(*myws.var("invMass"), *myws.var("ctau")), Index(*sample),
                                               Import("PbPb", *((RooDataSet*)myws.data("dOS_DATA_PbPb"))),
                                               Import("PP",   *((RooDataSet*)myws.data("dOS_DATA_PP")))
                                               );
        RooSimultaneous* simPdf = new RooSimultaneous("simPdf", "simultaneous pdf", *sample);
        simPdf->addPdf(*myws.pdf(Form("%s_Tot_PbPb", pdfName.c_str())), "PbPb"); simPdf->addPdf(*myws.pdf(Form("%s_Tot_PP", pdfName.c_str())), "PP");
        myws.import(*simPdf);
        
        // check if we have already done this fit. If yes, do nothing and return true.
        
        bool found = true;
        RooArgSet *newpars = myws.pdf("simPdf")->getParameters(RooArgSet(*myws.var("invMass"),*myws.var("ctau"),*myws.var("ctauErr")));
        found = found && isFitAlreadyFound(newpars, outputDir, plotLabel, DSTAG, cut, fitMass, fitCtau, false,  true);
        if (found) {
          cout << "[INFO] This fit was already done, so I'll just go to the next one." << endl;
          return true;
        }
        myws.saveSnapshot("simPdf_parIni", *newpars, kTRUE) ;
        
        // Do the simultaneous fit
        RooFitResult* fitResult = simPdf->fitTo(*combData, Offset(kTRUE), Extended(kTRUE), NumCPU(numCores), ConditionalObservables(*myws.var("ctauErr")), Range("MassWindow,CtauWindow"), Save(), Minimizer("Minuit2","Migrad"));
        fitResult->Print();
        
        // Create the output files
        int nBins = 0;
        if (fitMass) {
          nBins = min(int( round((cut.dMuon.M.Max - cut.dMuon.M.Min)/binWidth) ), 1000);
          drawMassPlot(myws, outputDir, opt, cut, plotMassLabelPbPb, DSTAG, true, incJpsi, incPsi2S, incBkg, cutCtau, false, setLogScale, incSS, zoomPsi, nBins, getMeanPT);
          drawMassPlot(myws, outputDir, opt, cut, plotMassLabelPP, DSTAG, false, incJpsi, incPsi2S, incBkg, cutCtau, false, setLogScale, incSS, zoomPsi, nBins, getMeanPT);
        }
        if (fitCtau) {
          nBins = min(int( round((cut.dMuon.ctau.Max - cut.dMuon.ctau.Min)/binWidth) ), 1000);
          drawCtauPlot(myws, outputDir, opt, cut, plotCtauLabelPbPb, DSTAG, true, fitMass, incJpsi, incPsi2S, incBkg, incPrompt, incNonPrompt, cutCtau, false, setLogScale, incSS, nBins);
          drawCtauPlot(myws, outputDir, opt, cut, plotCtauLabelPP, DSTAG, false, fitMass, incJpsi, incPsi2S, incBkg, incPrompt, incNonPrompt, cutCtau, false, setLogScale, incSS, nBins);
        }
        saveWorkSpace(myws, outputDir, plotLabel, DSTAG, cut, fitMass, fitCtau, false, true);
        myws.saveSnapshot("simPdf_parFit", *newpars, kTRUE) ;
        
        // Delete the objects used during the simultaneous fit
        delete sample; delete combData; delete simPdf;
        
      } else {
        cout << "[ERROR] Neither mass or ctau were chosen for the simultaneous fit, please fix you input settings!" << endl; return false;
      }
    }
    else {
      if (isPbPb) {
        
        string plotLabel = ""; string pdfName = "";
        string dsName = Form("dOS_%s_PbPb", DSTAG.c_str());
        if (wantPureSMC) dsName = Form("dOS_%s_PbPb_NoBkg", DSTAG.c_str());

        if (fitCtauTrue) {
            
          pdfName = "pdfCTAUTRUE_Tot_PbPb"; plotLabel = plotCtauTrueLabelPbPb;
          // check if we have already done this fit. If yes, do nothing and return true.
          RooArgSet *newpars = myws.pdf(pdfName.c_str())->getParameters(RooArgSet(*myws.var("ctauTrue")));
          bool found =  true; bool skipFit = false;
          found = found && isFitAlreadyFound(newpars, outputDir, (wantPureSMC ? (plotLabel+"_NoBkg") : plotLabel), DSTAG, cut, false, false, fitCtauTrue, false, true);
          if (found) {
            if (fitCtau) {
              cout << "[INFO] This ctauTrue fit was already done, so I'll load the fit results." << endl;
              if ( loadPreviousFitResult(myws, outputDir, (wantPureSMC ? (plotLabel+"_NoBkg") : plotLabel), DSTAG, cut, false, false, fitCtauTrue, false, true) ) {
                myws.pdf("pdfCTAUTRUE_Tot_PbPb")->getParameters(RooArgSet(*myws.var("invMass")))->setAttribAll("Constant", kTRUE);
                skipFit = true;
              }
            } else {
              cout << "[INFO] This ctauTrue fit was already done, so I'll just go to the next one." << endl;
              return true;
            }
          }
          if (skipFit==false) {
            // Fit the Datasets
            RooFitResult* fitResult = myws.pdf(pdfName.c_str())->fitTo(*myws.data(dsName.c_str()), Extended(kTRUE), SumW2Error(kTRUE), Range(rangeMap["CtauTrue"].label.c_str()), NumCPU(numCores), Save());
            fitResult->Print();
            // Draw the mass plot
            int nBins = min(int( round((cut.dMuon.ctauTrue.Max - cut.dMuon.ctauTrue.Min)/binWidth) ), 1000);
            drawCtauTruePlot(myws, outputDir, opt, cut, (wantPureSMC ? (plotLabel+"_NoBkg") : plotLabel), DSTAG, true, incJpsi, incPsi2S, cutCtau, wantPureSMC, setLogScale, incSS, nBins); 
            saveWorkSpace(myws, outputDir, plotLabel, DSTAG, cut, false, false, fitCtauTrue, false, true);
            myws.saveSnapshot(Form("%s_parFit", pdfName.c_str()),*newpars,kTRUE) ;
          }
        }
        if (fitMass) {
          pdfName = "pdfMASS_Tot_PbPb"; plotLabel = plotMassLabelPbPb;
          
            // check if we have already done this fit. If yes, do nothing and return true.
          RooArgSet *newpars = myws.pdf(pdfName.c_str())->getParameters(RooArgSet(*myws.var("invMass")));
          bool found =  true; bool skipFit = false;
          found = found && isFitAlreadyFound(newpars, outputDir, (wantPureSMC ? (plotLabel+"_NoBkg") : plotLabel), DSTAG, cut, fitMass, false, false, false, true);
          if (found) {
            if (fitCtau) {
              cout << "[INFO] This mass fit was already done, so I'll load the fit results." << endl;
              string plotMassLabel = ""; 
              if (incJpsi)  { plotMassLabel = plotMassLabel + Form("_Jpsi_%s", parIni["Model_Jpsi_PbPb"].c_str());   } 
              if (incPsi2S) { plotMassLabel = plotMassLabel + Form("_Psi2S_%s", parIni["Model_Psi2S_PbPb"].c_str()); }
              if (incBkg)   { plotMassLabel = plotMassLabel + Form("_Bkg_%s", parIni["Model_Bkg_PbPb"].c_str());     }
              if ( loadPreviousFitResult(myws, outputDir, plotMassLabel, DSTAG, cut, fitMass, false, false, false, true) ) {
                myws.pdf("pdfMASS_Tot_PbPb")->getParameters(RooArgSet(*myws.var("invMass")))->setAttribAll("Constant", kTRUE);
                skipFit = true;
              }
            } else {
              cout << "[INFO] This mass fit was already done, so I'll just go to the next one." << endl;
              return true;
            }
          }
          if (skipFit==false) {
            RooFitResult* fitResult = myws.pdf("pdfMASS_Tot_PbPb")->fitTo(*myws.data(dsName.c_str()), Extended(kTRUE), NumCPU(numCores),
                                                                          Range(((incJpsi||incPsi2S)?rangeMap["Mass"]:rangeMap["SideBand"]).label.c_str()), Save());
            fitResult->Print();
              // Draw the mass plot
            int nBins = min(int( round((cut.dMuon.M.Max - cut.dMuon.M.Min)/binWidth) ), 1000);
            drawMassPlot(myws, outputDir, opt, cut, (wantPureSMC ? (plotLabel+"_NoBkg") : plotLabel), DSTAG, true, incJpsi, incPsi2S, incBkg, cutCtau, wantPureSMC, setLogScale, incSS, zoomPsi, nBins, getMeanPT); 
            saveWorkSpace(myws, outputDir, plotLabel, DSTAG, cut, fitMass, false, false, false, true);
            myws.saveSnapshot(Form("%s_parFit", pdfName.c_str()),*newpars,kTRUE) ;
          }
        }
        if (fitCtau) { 
          if (!fitMass && fitCtau) { pdfName = "pdfCTAU_Tot_PbPb";     plotLabel = plotCtauLabelPbPb;                 }
          if (fitMass &&  fitCtau) { pdfName = "pdfCTAUMASS_Tot_PbPb"; plotLabel = plotMassLabelPbPb+plotCtauLabelPbPb; }
          
          // check if we have already done this fit. If yes, do nothing and return true.
          RooArgSet *newpars = myws.pdf(pdfName.c_str())->getParameters(RooArgSet(*myws.var("invMass"), *myws.var("ctau"), *myws.var("ctau")));
          bool found =  true;
          found = found && isFitAlreadyFound(newpars, outputDir, (wantPureSMC ? (plotLabel+"_NoBkg") : plotLabel), DSTAG, cut, fitMass, fitCtau, false, false, true);
          if (found) {
            cout << "[INFO] This ctau fit was already done, so I'll just go to the next one." << endl;
            return true;
          }

          if (!isMC){
            if (myws.pdf("pdfMASS_Tot_PbPb")) {
              myws.pdf("pdfMASS_Tot_PbPb")->getParameters(RooArgSet(*myws.var("invMass")))->setAttribAll("Constant", kTRUE);
            }
            if (incPrompt) {
              string plotCtauLabel = Form("_JpsiPR_%s", (parIni["Model_JpsiPR_PbPb"]==""?"Delta":parIni["Model_JpsiPR_PbPb"].c_str()));
              loadPreviousFitResult(myws, outputDir, plotCtauLabel, "MCJPSIPR", cut, false, true, false, false, true);
              if (myws.pdf("pdfCTAU_JpsiPR_PbPb")) { 
                myws.pdf("pdfCTAU_JpsiPR_PbPb")->getParameters(RooArgSet(*myws.var("invMass"), *myws.var("ctau"), *myws.var("ctauErr")))->setAttribAll("Constant", kTRUE);
              }
              if (myws.pdf("pdfCTAU_Psi2SPR_PbPb")) { 
                myws.pdf("pdfCTAU_Psi2SPR_PbPb")->getParameters(RooArgSet(*myws.var("invMass"), *myws.var("ctau"), *myws.var("ctauErr")))->setAttribAll("Constant", kTRUE);
              }
              if (myws.pdf("pdfCTAU_BkgPR_PbPb")) { 
                myws.pdf("pdfCTAU_BkgPR_PbPb")->getParameters(RooArgSet(*myws.var("invMass"), *myws.var("ctau"), *myws.var("ctauErr")))->setAttribAll("Constant", kTRUE);
              }
            }
            if (incNonPrompt && (incJpsi || incPsi2S)) {
              string plotCtauTrueLabel = string("") + Form("_CtauTrueRes_%s", parIni["Model_CtauTrueRes_PbPb"].c_str()) + Form("_CtauTrue_%s", parIni["Model_CtauTrue_PbPb"].c_str());
              loadPreviousFitResult(myws, outputDir, plotCtauTrueLabel, "MCJPSINOPR", cut, false, false, true, false, true);
              if (myws.pdf("pdfCTAU_JpsiNoPR_PbPb")) { 
                myws.pdf("pdfCTAU_JpsiNoPR_PbPb")->getParameters(RooArgSet(*myws.var("invMass"), *myws.var("ctau"), *myws.var("ctauErr")))->setAttribAll("Constant", kTRUE);
              }
              if (myws.pdf("pdfCTAU_Psi2SNoPR_PbPb")) { 
                myws.pdf("pdfCTAU_Psi2SNoPR_PbPb")->getParameters(RooArgSet(*myws.var("invMass"), *myws.var("ctau"), *myws.var("ctauErr")))->setAttribAll("Constant", kTRUE);
              }
            }
          }
          if (fitMass){
            RooFitResult* fitResult = myws.pdf(pdfName.c_str())->fitTo(*myws.data(dsName.c_str()), Extended(kTRUE), NumCPU(numCores), ConditionalObservables(*myws.var("ctauErr")),
                                                                       Range((rangeMap["Ctau"].label+"&&"+((incJpsi||incPsi2S)?rangeMap["Mass"].label:rangeMap["SideBand"].label)).c_str()), Save());
            fitResult->Print();
          } else {
            RooFitResult* fitResult = myws.pdf(pdfName.c_str())->fitTo(*myws.data(dsName.c_str()), Extended(kTRUE), NumCPU(numCores), ConditionalObservables(*myws.var("ctauErr")),
                                                                       Range(rangeMap["Ctau"].label.c_str()), Save());
            fitResult->Print();
          }
          
          int nBins = min(int( round((cut.dMuon.ctau.Max - cut.dMuon.ctau.Min)/binWidth) ), 1000);
          drawCtauPlot(myws, outputDir, opt, cut, (wantPureSMC ? (plotLabel+"_NoBkg") : plotLabel), DSTAG, true, fitMass, incJpsi, incPsi2S, incBkg, incPrompt, incNonPrompt, cutCtau, wantPureSMC, setLogScale, incSS, nBins);
          saveWorkSpace(myws, outputDir, plotLabel, DSTAG, cut, fitMass, fitCtau, false, false, true);
          myws.saveSnapshot(Form("%s_parFit", pdfName.c_str()),*newpars,kTRUE) ;
        }
      }
      else {
        cut.Centrality.Start = 0;
        cut.Centrality.End = 200;
        
        string plotLabel = ""; string pdfName = "";
        string dsName = Form("dOS_%s_PP", DSTAG.c_str());
        if (wantPureSMC) dsName = Form("dOS_%s_PP_NoBkg", DSTAG.c_str());

        if (fitCtauTrue) {
            
          pdfName = "pdfCTAUTRUE_Tot_PP"; plotLabel = plotCtauTrueLabelPP;
          // check if we have already done this fit. If yes, do nothing and return true.
          RooArgSet *newpars = myws.pdf(pdfName.c_str())->getParameters(RooArgSet(*myws.var("ctauTrue")));
          bool found =  true; bool skipFit = false;
          found = found && isFitAlreadyFound(newpars, outputDir, (wantPureSMC ? (plotLabel+"_NoBkg") : plotLabel), DSTAG, cut, false, false, fitCtauTrue, false, false);
          if (found) {
            if (fitCtau) {
              cout << "[INFO] This ctauTrue fit was already done, so I'll load the fit results." << endl;
              if ( loadPreviousFitResult(myws, outputDir, (wantPureSMC ? (plotLabel+"_NoBkg") : plotLabel), DSTAG, cut, false, false, fitCtauTrue, false, false) ) {
                myws.pdf("pdfCTAUTRUE_Tot_PP")->getParameters(RooArgSet(*myws.var("invMass")))->setAttribAll("Constant", kTRUE);
                skipFit = true;
              }
            } else {
              cout << "[INFO] This ctauTrue fit was already done, so I'll just go to the next one." << endl;
              return true;
            }
          }
          if (skipFit==false) {
            // Fit the Datasets
            RooFitResult* fitResult = myws.pdf(pdfName.c_str())->fitTo(*myws.data(dsName.c_str()), Extended(kTRUE), Range(rangeMap["CtauTrue"].label.c_str()), NumCPU(numCores), Save());
            fitResult->Print();
            // Draw the mass plot
            int nBins = min(int( round((cut.dMuon.ctauTrue.Max - cut.dMuon.ctauTrue.Min)/binWidth) ), 1000);
            drawCtauTruePlot(myws, outputDir, opt, cut, (wantPureSMC ? (plotLabel+"_NoBkg") : plotLabel), DSTAG, false, incJpsi, incPsi2S, cutCtau, wantPureSMC, setLogScale, incSS, nBins); 
            saveWorkSpace(myws, outputDir, plotLabel, DSTAG, cut, false, false, fitCtauTrue, false, false);
            myws.saveSnapshot(Form("%s_parFit", pdfName.c_str()),*newpars,kTRUE) ;
          }
        }
        if (fitMass) {
          pdfName = "pdfMASS_Tot_PP"; plotLabel = plotMassLabelPP;
            
          // check if we have already done this fit. If yes, do nothing and return true.
          RooArgSet *newpars = myws.pdf(pdfName.c_str())->getParameters(RooArgSet(*myws.var("invMass")));
          bool found =  true; bool skipFit = false;
          found = found && isFitAlreadyFound(newpars, outputDir, (wantPureSMC ? (plotLabel+"_NoBkg") : plotLabel), DSTAG, cut, fitMass, false, false, false, false);
          if (found) {
            if (fitCtau) {
              cout << "[INFO] This mass fit was already done, so I'll load the fit results." << endl;
              string plotMassLabel = ""; 
              if (incJpsi)  { plotMassLabel = plotMassLabel + Form("_Jpsi_%s", parIni["Model_Jpsi_PP"].c_str());   } 
              if (incPsi2S) { plotMassLabel = plotMassLabel + Form("_Psi2S_%s", parIni["Model_Psi2S_PP"].c_str()); }
              if (incBkg)   { plotMassLabel = plotMassLabel + Form("_Bkg_%s", parIni["Model_Bkg_PP"].c_str());     }
              if ( loadPreviousFitResult(myws, outputDir, plotMassLabel, DSTAG, cut, true, false, false, false, false) ) {
                skipFit = true;
              }
            } else {
              cout << "[INFO] This mass fit was already done, so I'll just go to the next one." << endl;
              return true;
            }
          }
          if (skipFit==false) {
            RooFitResult* fitResult = myws.pdf("pdfMASS_Tot_PP")->fitTo(*myws.data(dsName.c_str()), Extended(kTRUE), NumCPU(numCores),
                                                                        Range(((incJpsi||incPsi2S)?rangeMap["Mass"]:rangeMap["SideBand"]).label.c_str()), Save());
            fitResult->Print();
            // Draw the mass plot
            int nBins = min(int( round((cut.dMuon.M.Max - cut.dMuon.M.Min)/binWidth) ), 1000);
            drawMassPlot(myws, outputDir, opt, cut, (wantPureSMC ? (plotLabel+"_NoBkg") : plotLabel), DSTAG, false, incJpsi, incPsi2S, incBkg, cutCtau, wantPureSMC, setLogScale, incSS, zoomPsi, nBins, getMeanPT); 
            saveWorkSpace(myws, outputDir, plotLabel, DSTAG, cut, fitMass, false, false, false, false);
            myws.saveSnapshot(Form("%s_parFit", pdfName.c_str()),*newpars,kTRUE) ;
          }
        }
        if (fitCtau) { 
          if (!fitMass && fitCtau) { pdfName = "pdfCTAU_Tot_PP";     plotLabel = plotCtauLabelPP;                 }
          if (fitMass &&  fitCtau) { pdfName = "pdfCTAUMASS_Tot_PP"; plotLabel = plotMassLabelPP+plotCtauLabelPP; }

          // check if we have already done this fit. If yes, do nothing and return true.
          RooArgSet *newpars = myws.pdf(pdfName.c_str())->getParameters(RooArgSet(*myws.var("invMass"), *myws.var("ctau"), *myws.var("ctau")));
          bool found =  true;
          found = found && isFitAlreadyFound(newpars, outputDir, (wantPureSMC ? (plotLabel+"_NoBkg") : plotLabel), DSTAG, cut, fitMass, fitCtau, false, false, false);
          if (found) {
            cout << "[INFO] This ctau fit was already done, so I'll just go to the next one." << endl;
            return true;
          }

          if (!isMC){
            if (myws.pdf("pdfMASS_Tot_PP")) {
              myws.pdf("pdfMASS_Tot_PP")->getParameters(RooArgSet(*myws.var("invMass")))->setAttribAll("Constant", kTRUE);
            }
            if (incPrompt) {
              string plotCtauLabel = Form("_JpsiPR_%s", (parIni["Model_JpsiPR_PP"]==""?"Delta":parIni["Model_JpsiPR_PP"].c_str()));
              loadPreviousFitResult(myws, outputDir, plotCtauLabel, "MCJPSIPR", cut, false, true, false, false, false);
              if (myws.pdf("pdfCTAU_JpsiPR_PP")) { 
                myws.pdf("pdfCTAU_JpsiPR_PP")->getParameters(RooArgSet(*myws.var("invMass"), *myws.var("ctau"), *myws.var("ctauErr")))->setAttribAll("Constant", kTRUE);
              }
              if (myws.pdf("pdfCTAU_Psi2SPR_PP")) { 
                myws.pdf("pdfCTAU_Psi2SPR_PP")->getParameters(RooArgSet(*myws.var("invMass"), *myws.var("ctau"), *myws.var("ctauErr")))->setAttribAll("Constant", kTRUE);
              }
              if (myws.pdf("pdfCTAU_BkgPR_PP")) { 
                myws.pdf("pdfCTAU_BkgPR_PP")->getParameters(RooArgSet(*myws.var("invMass"), *myws.var("ctau"), *myws.var("ctauErr")))->setAttribAll("Constant", kTRUE);
              }
            }
            if (incNonPrompt && (incJpsi || incPsi2S)) {
              string plotCtauTrueLabel = string("") + Form("_CtauTrueRes_%s", parIni["Model_CtauTrueRes_PP"].c_str()) + Form("_CtauTrue_%s", parIni["Model_CtauTrue_PP"].c_str());
              loadPreviousFitResult(myws, outputDir, plotCtauTrueLabel, "MCJPSINOPR", cut, false, false, true, false, false);
              if (myws.pdf("pdfCTAU_JpsiNoPR_PP")) { 
                myws.pdf("pdfCTAU_JpsiNoPR_PP")->getParameters(RooArgSet(*myws.var("invMass"), *myws.var("ctau"), *myws.var("ctauErr")))->setAttribAll("Constant", kTRUE);
              }
              if (myws.pdf("pdfCTAU_Psi2SNoPR_PP")) { 
                myws.pdf("pdfCTAU_Psi2SNoPR_PP")->getParameters(RooArgSet(*myws.var("invMass"), *myws.var("ctau"), *myws.var("ctauErr")))->setAttribAll("Constant", kTRUE);
              }
            }
          }
           
          if (fitMass){
            RooFitResult* fitResult = myws.pdf(pdfName.c_str())->fitTo(*myws.data(dsName.c_str()), Extended(kTRUE), NumCPU(numCores), ConditionalObservables(*myws.var("ctauErr")),
                                                                       Range((rangeMap["Ctau"].label+"&&"+((incJpsi||incPsi2S)?rangeMap["Mass"].label:rangeMap["SideBand"].label)).c_str()), Save());
            fitResult->Print();
          } else {
            RooFitResult* fitResult = myws.pdf(pdfName.c_str())->fitTo(*myws.data(dsName.c_str()), Extended(kTRUE), NumCPU(numCores), ConditionalObservables(*myws.var("ctauErr")),
                                                                       Range(rangeMap["Ctau"].label.c_str()), Save());
            fitResult->Print();
          }
            
          int nBins = min(int( round((cut.dMuon.ctau.Max - cut.dMuon.ctau.Min)/binWidth) ), 1000);
          drawCtauPlot(myws, outputDir, opt, cut, (wantPureSMC ? (plotLabel+"_NoBkg") : plotLabel), DSTAG, false, fitMass, incJpsi, incPsi2S, incBkg, incPrompt, incNonPrompt, cutCtau, wantPureSMC, setLogScale, incSS, nBins);
          saveWorkSpace(myws, outputDir, plotLabel, DSTAG, cut, fitMass, fitCtau, false, false, false);
          myws.saveSnapshot(Form("%s_parFit", pdfName.c_str()),*newpars,kTRUE) ;
        }
      }
    }
  }

  return true;
};


void setCtauCuts(struct KinCuts& cut, bool isPbPb) 
{
  if (cut.dMuon.AbsRap.Max<=1.6 && isPbPb) {
    cut.dMuon.ctauCut = "( ctau < (0.013 + (0.22/pt)) )";
  }
  if (cut.dMuon.AbsRap.Min>=1.6 && isPbPb) {
    cut.dMuon.ctauCut = "( ctau < (0.015 + (0.28/pt)) )";
  }
  if (cut.dMuon.AbsRap.Max<=1.6 && !isPbPb) {
    cut.dMuon.ctauCut = "( ctau < (0.010 + (0.25/pt)) )";
  }
  if (cut.dMuon.AbsRap.Min>=1.6 && !isPbPb) {
    cut.dMuon.ctauCut = "( ctau < (0.013 + (0.29/pt)) )";
  }
};


bool setMassModel( struct OniaModel& model, map<string, string>& parIni, bool isPbPb, bool incJpsi, bool incPsi2S, bool incBkg )
{
  if (isPbPb && incBkg) {
    if (parIni.count("Model_Bkg_PbPb")>0) {
      model.PbPb.Bkg.Mass = MassModelDictionary[parIni["Model_Bkg_PbPb"]];
      if (model.PbPb.Bkg.Mass==MassModel(0)) {
        cout << "[ERROR] The background model: " << parIni["Model_Bkg_PbPb"] << " is invalid" << endl; return false;
      }
    } else { 
      cout << "[ERROR] Background mass model for PbPb was not found in the initial parameters!" << endl; return false;
    }
  }
  if (isPbPb && incJpsi) {
    if (parIni.count("Model_Jpsi_PbPb")>0) {
      model.PbPb.Jpsi.Mass = MassModelDictionary[parIni["Model_Jpsi_PbPb"]];
      if (model.PbPb.Jpsi.Mass==MassModel(0)) {
        cout << "[ERROR] The Jpsi model: " << parIni["Model_Jpsi_PbPb"] << " is invalid" << endl; return false;
      }
    } else { 
      cout << "[ERROR] Jpsi mass model for PbPb was not found in the initial parameters!" << endl; return false;
    }
  }
  if (isPbPb && incPsi2S) {
    if (parIni.count("Model_Psi2S_PbPb")>0) {
      model.PbPb.Psi2S.Mass = MassModelDictionary[parIni["Model_Psi2S_PbPb"]];
      if (model.PbPb.Psi2S.Mass==MassModel(0)) {
        cout << "[ERROR] The psi2S model: " << parIni["Model_Psi2S_PbPb"] << " is invalid" << endl; return false;
      }
    } else { 
      cout << "[ERROR] psi(2S) mass model for PbPb was not found in the initial parameters!" << endl; return false;
    }
  }
  if (!isPbPb && incBkg) {
    if (parIni.count("Model_Bkg_PP")>0) {
      model.PP.Bkg.Mass = MassModelDictionary[parIni["Model_Bkg_PP"]];
      if (model.PP.Bkg.Mass==MassModel(0)) {
        cout << "[ERROR] The background model: " << parIni["Model_Bkg_PP"] << " is invalid" << endl; return false;
      }
    } else { 
      cout << "[ERROR] Background mass model for PP was not found in the initial parameters!" << endl; return false;
    }
  }
  if (!isPbPb && incJpsi) {
    if (parIni.count("Model_Jpsi_PP")>0) {
      model.PP.Jpsi.Mass = MassModelDictionary[parIni["Model_Jpsi_PP"]];
      if (model.PP.Jpsi.Mass==MassModel(0)) {
        cout << "[ERROR] The Jpsi model: " << parIni["Model_Jpsi_PbPb"] << " is invalid" << endl; return false;
      }
    } else { 
      cout << "[ERROR] Jpsi mass model for PP was not found in the initial parameters!" << endl; return false;
    }
  }
  if (!isPbPb && incPsi2S) {
    if (parIni.count("Model_Psi2S_PP")>0) {
      model.PP.Psi2S.Mass = MassModelDictionary[parIni["Model_Psi2S_PP"]];
      if (model.PP.Psi2S.Mass==MassModel(0)) {
        cout << "[ERROR] The psi2S model: " << parIni["Model_Psi2S_PbPb"] << " is invalid" << endl; return false;
      }
    } else { 
      cout << "[ERROR] psi(2S) mass model for PP was not found in the initial parameters!" << endl; return false;
    }
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
      if (model.PbPb.Bkg.Ctau.NonPrompt==CtauModel(0)) {
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


bool setCtauTrueModel( struct OniaModel& model, map<string, string>&  parIni, bool isPbPb )
{
  if (isPbPb) {
    if (parIni.count("Model_CtauTrueRes_PbPb")>0) {
      model.PbPb.CtauTrueRes = CtauModelDictionary[parIni["Model_CtauTrueRes_PbPb"]];
      if (model.PbPb.CtauTrueRes==CtauModel(0)) {
        cout << "[ERROR] The ctau truth resolution model: " << parIni["Model_CtauTrueRes_PbPb"] << " is invalid" << endl; return false;
      }
    } else { 
      cout << "[ERROR] Ctau Truth resolution model for PbPb was not found in the initial parameters!" << endl; return false;
    }
    if (parIni.count("Model_CtauTrue_PbPb")>0) {
      model.PbPb.CtauTrue = CtauModelDictionary[parIni["Model_CtauTrue_PbPb"]];
      if (model.PbPb.CtauTrue==CtauModel(0)) {
        cout << "[ERROR] The ctau truth model: " << parIni["Model_CtauTrue_PbPb"] << " is invalid" << endl; return false;
      }
    } else { 
      cout << "[ERROR] Ctau Truth model for PbPb was not found in the initial parameters!" << endl; return false;
    }
  } else {
    if (parIni.count("Model_CtauTrueRes_PP")>0) {
      model.PP.CtauTrueRes = CtauModelDictionary[parIni["Model_CtauTrueRes_PP"]];
      if (model.PP.CtauTrueRes==CtauModel(0)) {
        cout << "[ERROR] The ctau truth resolution model: " << parIni["Model_CtauTrue_PP"] << " is invalid" << endl; return false;
      }
    } else { 
      cout << "[ERROR] Ctau truth resolution model for PP was not found in the initial parameters!" << endl; return false;
    }
    if (parIni.count("Model_CtauTrue_PP")>0) {
      model.PP.CtauTrue = CtauModelDictionary[parIni["Model_CtauTrue_PP"]];
      if (model.PP.CtauTrue==CtauModel(0)) {
        cout << "[ERROR] The ctau truth model: " << parIni["Model_CtauTrue_PP"] << " is invalid" << endl; return false;
      }
    } else { 
      cout << "[ERROR] Ctau truth model for PP was not found in the initial parameters!" << endl; return false;
    }
  }

  return true;
};
   
int importDataset(RooWorkspace& myws, RooWorkspace& inputWS, struct KinCuts& cut, string label, map<string, struct RangeStruc>& rangeMap, bool cutSideBand, bool fitCtau)
{
  string indMuonMass    = Form("(%.6f < invMass && invMass < %.6f)",       cut.dMuon.M.Min,       cut.dMuon.M.Max);
  if (cutSideBand) {
    indMuonMass =  indMuonMass + "&&" + "((2.0 < invMass && invMass < 2.9) || (3.3 < invMass && invMass < 3.5) || (3.7 < invMass && invMass < 5.0))";
  }
  string indMuonRap     = Form("(%.6f <= abs(rap) && abs(rap) < %.6f)",    cut.dMuon.AbsRap.Min,  cut.dMuon.AbsRap.Max);
  string indMuonPt      = Form("(%.6f <= pt && pt < %.6f)",                cut.dMuon.Pt.Min,      cut.dMuon.Pt.Max);
  string indMuonCtau    = Form("(%.12f <= ctau && ctau < %.12f)",          cut.dMuon.ctau.Min,    cut.dMuon.ctau.Max); 
  if(cut.dMuon.ctauCut!=""){ 
    indMuonCtau = cut.dMuon.ctauCut; 
  }
  string indMuonCtauErr = Form("(%.12f <= ctauErr && ctauErr < %.12f)",    cut.dMuon.ctauErr.Min, cut.dMuon.ctauErr.Max);
  string inCentrality   = Form("(%d <= cent && cent < %d)",                cut.Centrality.Start,  cut.Centrality.End);
  string indMuonCtauTrue = Form("(%.12f <= ctauTrue && ctauTrue < %.12f)", cut.dMuon.ctauTrue.Min,    cut.dMuon.ctauTrue.Max);

  string strCut         = indMuonMass +"&&"+ indMuonRap +"&&"+ indMuonPt +"&&"+ indMuonCtau +"&&"+ indMuonCtauErr;
  if (label.find("PbPb")!=std::string::npos){ strCut = strCut +"&&"+ inCentrality; } 
  if (label.find("MC")!=std::string::npos && label.find("NOPR")!=std::string::npos){ strCut = strCut +"&&"+ indMuonCtauTrue; } 

  // Reduce and import the datasets
  if (!(inputWS.data(Form("dOS_%s", label.c_str())))){ 
    cout << "[ERROR] The dataset " <<  Form("dOS_%s", label.c_str()) << " was not found!" << endl;
    return -1;
  }
  RooDataSet* dataOS = (RooDataSet*)inputWS.data(Form("dOS_%s", label.c_str()))->reduce(strCut.c_str());
  if (dataOS->sumEntries()==0){ 
    cout << "[WARNING] No events from dataset " <<  Form("dOS_%s", label.c_str()) << " passed the kinematic cuts!" << endl;
    return 0;
  }
  myws.import(*dataOS);
  delete dataOS;

  if (label.find("NoBkg")==std::string::npos) // Don't try to find SS dataset if label contais NoBkg
  {
    if (!(inputWS.data(Form("dSS_%s", label.c_str())))){
      cout << "[ERROR] The dataset " <<  Form("dSS_%s", label.c_str()) << " was not found!" << endl;
      return -1;
    }
    RooDataSet* dataSS = (RooDataSet*)inputWS.data(Form("dSS_%s", label.c_str()))->reduce(strCut.c_str());
    if (dataSS->sumEntries()==0){
      cout << "[WARNING] No events from dataset " <<  Form("dSS_%s", label.c_str()) << " passed the kinematic cuts!" << endl;
    }
    myws.import(*dataSS);
    delete dataSS;
  }
  
  if (fitCtau) {
    Double_t ctauErrMax; Double_t ctauErrMin;
    myws.data(Form("dOS_%s", label.c_str()))->getRange(*myws.var("ctauErr"), ctauErrMin, ctauErrMax);
    if (cut.dMuon.ctauErr.Min<ctauErrMin && cut.dMuon.ctauErr.Max>ctauErrMin) { cut.dMuon.ctauErr.Min = ctauErrMin; }
    if (cut.dMuon.ctauErr.Max>ctauErrMax && cut.dMuon.ctauErr.Min<ctauErrMax) { cut.dMuon.ctauErr.Max = ctauErrMax; }
    cout << "Range from data: ctauErrMin: " << ctauErrMin << "  ctauErrMax: " << ctauErrMax << endl;
  }
  
  // Set the range of each global parameter in the local workspace
  myws.var("invMass")->setMin(cut.dMuon.M.Min);        
  myws.var("invMass")->setMax(cut.dMuon.M.Max);
  myws.var("pt")->setMin(cut.dMuon.Pt.Min);            
  myws.var("pt")->setMax(cut.dMuon.Pt.Max);
  myws.var("rap")->setMin(-1.0*cut.dMuon.AbsRap.Max);       
  myws.var("rap")->setMax(cut.dMuon.AbsRap.Max);
  myws.var("ctau")->setMin(cut.dMuon.ctau.Min);        
  myws.var("ctau")->setMax(cut.dMuon.ctau.Max);
  myws.var("ctauErr")->setMin(cut.dMuon.ctauErr.Min);  
  myws.var("ctauErr")->setMax(cut.dMuon.ctauErr.Max);
  if (label.find("PbPb")!=std::string::npos){
    myws.var("cent")->setMin(cut.Centrality.Start);      
    myws.var("cent")->setMax(cut.Centrality.End);
  }
  if (label.find("MC")!=std::string::npos && label.find("NOPR")!=std::string::npos){ 
    myws.var("ctauTrue")->setMin(cut.dMuon.ctauTrue.Min);      
    myws.var("ctauTrue")->setMax(cut.dMuon.ctauTrue.Max);
  }

  struct RangeStruc range;
 
  if (fitCtau) {
    myws.var("ctauErr")->setRange("CtauErrWindow", cut.dMuon.ctauErr.Min, cut.dMuon.ctauErr.Max);
    range.label = "(CtauErrWindow)"; range.cut = indMuonCtauErr; rangeMap["CtauErr"] = range;
  }
  if (label.find("MC")!=std::string::npos && label.find("NOPR")!=std::string::npos){ 
    Double_t ctauTrueMax; Double_t ctauTrueMin;
    myws.data(Form("dOS_%s", label.c_str()))->getRange(*myws.var("ctauTrue"), ctauTrueMin, ctauTrueMax);
    if (ctauTrueMin<cut.dMuon.ctauTrue.Min) { ctauTrueMin = cut.dMuon.ctauTrue.Min; }
    if (ctauTrueMax>cut.dMuon.ctauTrue.Max) { ctauTrueMax = cut.dMuon.ctauTrue.Max; }
    cout << "Range from data: ctauTrueMin: " << ctauTrueMin << "  ctauTrueMax: " << ctauTrueMax << endl;
    myws.var("ctauTrue")->setRange("CtauTrueWindow", ctauTrueMin, ctauTrueMax);
    range.label = "(CtauTrueWindow)"; range.cut = Form("(%.12f <= ctauTrue && ctauTrue < %.12f)", ctauTrueMin, ctauTrueMax); 
    rangeMap["CtauTrue"] = range;
  }
  if (fitCtau) {
    Double_t ctauMax; Double_t ctauMin;
    myws.data(Form("dOS_%s", label.c_str()))->getRange(*myws.var("ctau"), ctauMin, ctauMax);
    if (ctauMin<cut.dMuon.ctau.Min) { ctauMin = cut.dMuon.ctau.Min; }
    if (ctauMax>cut.dMuon.ctau.Max) { ctauMax = cut.dMuon.ctau.Max; }
    cout << "Range from data: ctauMin: " << ctauMin << "  ctauMax: " << ctauMax << endl;
    myws.var("ctau")->setRange("CtauWindow", ctauMin, ctauMax);
    range.label = "(CtauWindow)"; range.cut = Form("(%.12f <= ctau && ctau < %.12f)", ctauMin, ctauMax); 
    rangeMap["Ctau"] = range;
  }
  if (label.find("MC")!=std::string::npos)
  {
    if (label.find("PSI2S")!=std::string::npos)
    {
      if (cut.dMuon.AbsRap.Min >= 1.6) {
        myws.var("invMass")->setRange("MassWindow", cut.dMuon.M.Min, 3.95);
        range.cut = Form("(invMass>%.6f && invMass<%.6f)", cut.dMuon.M.Min, 3.95);
      }
      else { 
        myws.var("invMass")->setRange("MassWindow", cut.dMuon.M.Min, 3.85);
        range.cut   = Form("(invMass>%.6f && invMass<%.6f)", cut.dMuon.M.Min, 3.85);
      }
    }
    else
    {
      if (cut.dMuon.AbsRap.Min >= 1.6) {
        myws.var("invMass")->setRange("MassWindow", cut.dMuon.M.Min, 3.32);
        range.cut   = Form("(invMass>%.6f && invMass<%.6f)", cut.dMuon.M.Min, 3.32);
      }
      else {
        myws.var("invMass")->setRange("MassWindow", cut.dMuon.M.Min, 3.26);
        range.cut   = Form("(invMass>%.6f && invMass<%.6f)", cut.dMuon.M.Min, 3.26);
      }
    }
  }
  else {
    myws.var("invMass")->setRange("MassWindow", cut.dMuon.M.Min, cut.dMuon.M.Max);
    range.cut = Form("(invMass>%.6f && invMass<%.6f)", cut.dMuon.M.Min, cut.dMuon.M.Max);
  }
  range.label = "(MassWindow)"; rangeMap["Mass"] = range;
  
  if ( cutSideBand ) {
    myws.var("invMass")->setRange("SideBandMID", ((cut.dMuon.M.Min<3.3)?3.3:cut.dMuon.M.Min), ((cut.dMuon.M.Max>3.5)?3.5:cut.dMuon.M.Max));
    range.label = "SideBandMID";
    if (cut.dMuon.M.Min < 2.9) {
      myws.var("invMass")->setRange("SideBandBOT", cut.dMuon.M.Min, 2.9);
      range.label = range.label + "," + "SideBandBOT";
    }
    if (cut.dMuon.M.Max > 3.7) {
      myws.var("invMass")->setRange("SideBandTOP", 3.7, cut.dMuon.M.Max);
      range.label = range.label + "," + "SideBandTOP";
    }
    range.cut = cutSideBand; range.cut = "("+range.cut+")";
    rangeMap["SideBand"] = range;
  }

  return 1;
};

void setOptions(struct InputOpt* opt) 
{
  opt->pp.RunNb.Start   = 262157; opt->PbPb.RunNb.Start = 262620;
  opt->pp.RunNb.End     = 262328; opt->PbPb.RunNb.End   = 263757;
  opt->pp.TriggerBit    = (int) PP::HLT_HIL1DoubleMu0_v1; 
  opt->PbPb.TriggerBit  = (int) HI::HLT_HIL1DoubleMu0_v1; 
  return;
};

void setFileName(string& FileName, string outputDir, string TAG, string plotLabel, struct KinCuts cut,bool fitMass, bool fitCtau, bool fitCtauTrue, bool doSimulFit, bool isPbPb, bool cutSideBand) 
{
  if (fitCtauTrue) {
    FileName = Form("%sresult/%s/ctauTrue%s/FIT_%s_%s_%s_%s%s_pt%.0f%.0f_rap%.0f%.0f_cent%d%d.root", outputDir.c_str(), TAG.c_str(), (cutSideBand?"SB":""), "CTAUTRUE", TAG.c_str(), "Psi2SJpsi", (isPbPb?"PbPb":"PP"), plotLabel.c_str(), (cut.dMuon.Pt.Min*10.0), (cut.dMuon.Pt.Max*10.0), (cut.dMuon.AbsRap.Min*10.0), (cut.dMuon.AbsRap.Max*10.0), cut.Centrality.Start, cut.Centrality.End);
  }  else {
    if (doSimulFit) {
      if (fitMass && fitCtau) { 
        FileName = Form("%sresult/%s/ctaumass%s/FIT_%s_%s_%s_%s%s_pt%.0f%.0f_rap%.0f%.0f_cent%d%d.root", outputDir.c_str(), TAG.c_str(), (cutSideBand?"SB":""), "CTAUMASS", TAG.c_str(), "Psi2SJpsi", "COMB", plotLabel.c_str(), (cut.dMuon.Pt.Min*10.0), (cut.dMuon.Pt.Max*10.0), (cut.dMuon.AbsRap.Min*10.0), (cut.dMuon.AbsRap.Max*10.0), cut.Centrality.Start, cut.Centrality.End);
      } else if (fitMass) {
        FileName = Form("%sresult/%s/mass%s/FIT_%s_%s_%s_%s%s_pt%.0f%.0f_rap%.0f%.0f_cent%d%d.root", outputDir.c_str(), TAG.c_str(), (cutSideBand?"SB":""), "MASS", TAG.c_str(), "Psi2SJpsi", "COMB", plotLabel.c_str(), (cut.dMuon.Pt.Min*10.0), (cut.dMuon.Pt.Max*10.0), (cut.dMuon.AbsRap.Min*10.0), (cut.dMuon.AbsRap.Max*10.0), cut.Centrality.Start, cut.Centrality.End);
      } else if (fitCtau) {
        FileName = Form("%sresult/%s/ctau%s/FIT_%s_%s_%s_%s%s_pt%.0f%.0f_rap%.0f%.0f_cent%d%d.root", outputDir.c_str(), TAG.c_str(), (cutSideBand?"SB":""), "CTAU", TAG.c_str(), "Psi2SJpsi", "COMB", plotLabel.c_str(), (cut.dMuon.Pt.Min*10.0), (cut.dMuon.Pt.Max*10.0), (cut.dMuon.AbsRap.Min*10.0), (cut.dMuon.AbsRap.Max*10.0), cut.Centrality.Start, cut.Centrality.End);
      }
    } else {    
      if (fitMass && fitCtau) {
        FileName = Form("%sresult/%s/ctaumass%s/FIT_%s_%s_%s_%s%s_pt%.0f%.0f_rap%.0f%.0f_cent%d%d.root", outputDir.c_str(), TAG.c_str(), (cutSideBand?"SB":""), "CTAUMASS", TAG.c_str(), "Psi2SJpsi", (isPbPb?"PbPb":"PP"), plotLabel.c_str(), (cut.dMuon.Pt.Min*10.0), (cut.dMuon.Pt.Max*10.0), (cut.dMuon.AbsRap.Min*10.0), (cut.dMuon.AbsRap.Max*10.0), cut.Centrality.Start, cut.Centrality.End);
      } else if (fitMass) {
        FileName = Form("%sresult/%s/mass%s/FIT_%s_%s_%s_%s%s_pt%.0f%.0f_rap%.0f%.0f_cent%d%d.root", outputDir.c_str(), TAG.c_str(), (cutSideBand?"SB":""), "MASS", TAG.c_str(), "Psi2SJpsi", (isPbPb?"PbPb":"PP"), plotLabel.c_str(), (cut.dMuon.Pt.Min*10.0), (cut.dMuon.Pt.Max*10.0), (cut.dMuon.AbsRap.Min*10.0), (cut.dMuon.AbsRap.Max*10.0), cut.Centrality.Start, cut.Centrality.End);
      } else if (fitCtau) {
        FileName = Form("%sresult/%s/ctau%s/FIT_%s_%s_%s_%s%s_pt%.0f%.0f_rap%.0f%.0f_cent%d%d.root", outputDir.c_str(), TAG.c_str(), (cutSideBand?"SB":""), "CTAU", TAG.c_str(), "Psi2SJpsi", (isPbPb?"PbPb":"PP"), plotLabel.c_str(), (cut.dMuon.Pt.Min*10.0), (cut.dMuon.Pt.Max*10.0), (cut.dMuon.AbsRap.Min*10.0), (cut.dMuon.AbsRap.Max*10.0), cut.Centrality.Start, cut.Centrality.End);
      } 
    }
  }
};

bool saveWorkSpace(RooWorkspace& myws, string outputDir, string plotLabel, string DSTAG, struct KinCuts cut, bool fitMass, bool fitCtau, bool fitCtauTrue, bool doSimulFit, bool isPbPb) 
{
  // Save the workspace
  if (fitCtauTrue) { 
    gSystem->mkdir(Form("%sresult/%s/ctauTrue", outputDir.c_str(), DSTAG.c_str()), kTRUE); 
  } else {
    if (fitMass && fitCtau) { gSystem->mkdir(Form("%sresult/%s/ctaumass", outputDir.c_str(), DSTAG.c_str()), kTRUE); } 
    else if (fitMass) { gSystem->mkdir(Form("%sresult/%s/mass", outputDir.c_str(), DSTAG.c_str()), kTRUE); } 
    else if (fitCtau) { gSystem->mkdir(Form("%sresult/%s/ctau", outputDir.c_str(), DSTAG.c_str()), kTRUE); }
  }
  string FileName = "";
  setFileName(FileName, outputDir, DSTAG, plotLabel, cut, fitMass, fitCtau, fitCtauTrue, doSimulFit, isPbPb);
  cout << FileName << endl;
  TFile *file =  new TFile(FileName.c_str(), "RECREATE");
  if (!file) { 
    cout << "[ERROR] Output root file with fit results could not be created!" << endl; return false; 
  } else {
    file->cd();    
    myws.Write("workspace"); 
    file->Write(); file->Close(); delete file;
  }
  return true;
};


bool loadPreviousFitResult(RooWorkspace& myws, string outputDir, string plotLabel, string DSTAG, struct KinCuts cut, bool fitMass, bool fitCtau, bool fitCtauTrue, bool doSimulFit, bool isPbPb)
{
  string FileName = "";
  setFileName(FileName, outputDir, DSTAG, plotLabel, cut, fitMass, fitCtau, fitCtauTrue, doSimulFit, isPbPb);
  if (gSystem->AccessPathName(FileName.c_str())) {
    cout << "[INFO] Results not found for: " << FileName << endl;
    return false; // File was not found
  }
 
  TFile *file = new TFile(FileName.c_str());
  if (!file) return false;

  RooWorkspace *ws = (RooWorkspace*) file->Get("workspace");
  if (!ws) {
    file->Close(); delete file;
    return false;
  }

  cout <<  "[INFO] Loading variables from: " << FileName << endl;
  RooArgSet listVar = ws->allVars();
  TIterator* parIt = listVar.createIterator();
  string print = "[INFO] Variables loaded: ";
  for (RooRealVar* it = (RooRealVar*)parIt->Next(); it!=NULL; it = (RooRealVar*)parIt->Next() ) {
    string name = it->GetName();
    if ( name=="invMass" || name=="ctau" || name=="ctauErr" || 
         name=="ctauTrue" || name=="pt" || name=="cent" || 
         name=="rap" || name=="One" ) continue;
    if ( (DSTAG.find("MC")!=std::string::npos) && (name.find("N_")!=std::string::npos) ) continue; 
    if (myws.var(name.c_str())) { 
      myws.var(name.c_str())->setVal  ( ws->var(name.c_str())->getValV()  );
      myws.var(name.c_str())->setError( ws->var(name.c_str())->getError() );
      print = print + "  " + name;
    }
  }
  cout << print << endl;
 
  return true;
};

bool isFitAlreadyFound(RooArgSet *newpars, string outputDir, string plotLabel, string DSTAG, struct KinCuts cut, bool fitMass, bool fitCtau, bool fitCtauTrue, bool doSimulFit, bool isPbPb) 
{
  string FileName = "";
  setFileName(FileName, outputDir, DSTAG, plotLabel, cut, fitMass, fitCtau, fitCtauTrue, doSimulFit, isPbPb);
  if (gSystem->AccessPathName(FileName.c_str())) return false; // File was not found

  TFile *file = new TFile(FileName.c_str());

  if (!file) return false;

  RooWorkspace *ws = (RooWorkspace*) file->Get("workspace");
  if (!ws) {
    file->Close(); delete file;
    return false;
  }

  string snapShotName = "";
  if (fitCtauTrue) {
    snapShotName = Form("pdfCTAUTRUE_Tot_%s_parIni", (isPbPb?"PbPb":"PP"));
  } else {
    if (doSimulFit) {
      string snapShotName = "simPdf_parIni";
    } else {
      if (fitMass && fitCtau) {
        snapShotName = Form("pdfCTAUMASS_Tot_%s_parIni", (isPbPb?"PbPb":"PP"));
      } else if (fitMass) {
        snapShotName = Form("pdfMASS_Tot_%s_parIni", (isPbPb?"PbPb":"PP"));
      } else if (fitCtau) {
        snapShotName = Form("pdfCTAU_Tot_%s_parIni", (isPbPb?"PbPb":"PP"));
      }    
    }
  }
  const RooArgSet *params = ws->getSnapshot(snapShotName.c_str());
  if (!params) {
    delete ws;
    file->Close(); delete file;
    return false;
  }

  bool result = compareSnapshots(newpars, params);

  delete ws;
  file->Close(); delete file; 

  return result;
}

bool compareSnapshots(RooArgSet *pars1, const RooArgSet *pars2) {
  TIterator* parIt = pars1->createIterator(); 

  for (RooRealVar* it = (RooRealVar*)parIt->Next(); it!=NULL; it = (RooRealVar*)parIt->Next() ) {
    double val = pars2->getRealValue(it->GetName(),-1e99);
    if (val==-1e99) return false;          // the parameter was not found!
    if (val != it->getVal()) return false; // the parameter was found, but with a different value!
    if ( ((RooRealVar&)(*pars2)[it->GetName()]).getMin() != it->getMin() ) return false; // the parameter has different lower limit
    if ( ((RooRealVar&)(*pars2)[it->GetName()]).getMax() != it->getMax() ) return false; // the parameter has different upper limit
  }

  return true;
}


/*
bool makeCtauErrPdf(RooWorkspace& ws, vector<string> objectColl, vector<string> rangeColl, bool isPbPb) 
{
  if (objectColl.size()!=rangeColl.size() || objectColl.size()<1) {
    cout << Form("[ERROR] Object and range collections are wrong, CtauErrPdf maker failed!", objectColl.at(i)) << endl; return false;
  }

  TMatrixD CoefficientMatrix = TMatrixD(objectColl.size(), rangeColl.size());
  TMatrixD TestMatrix = TMatrixD(objectColl.size(), rangeColl.size());
  for (int i=0; i<objectColl.size(); i++) {
    if (!ws.pdf(Form("pdfMASS_%s_%s", objectColl.at(i), (isPbPb?"PbPb":"PP")))) {
      cout << Form("[ERROR] Mass PDF for %s is missing!", objectColl.at(i)) << endl; return false;
    }
    for (int j=0; j<rangeColl.size(); j++) {
      CoefficientMatrix(i, j) =  ws.pdf(Form("pdfMASS_&s_%s", (objectColl.at(i), (isPbPb?"PbPb":"PP")))->createIntegral(*ws.var("invMass"), NormSet(*ws.var("invMass")), Range(rangeColl.at(j))))->getValV();
      Test(i, j) = CoefficientMatrix(i, j);
    }
  }

  CoefficientMatrix.InvertFast();
  TestMatrix = TestMatric * CoefficientMatrix;
  cout << "[INFO] Cross check of matrix inversion: " TestMatrix(0,0) << endl;
  
}
*/
  
