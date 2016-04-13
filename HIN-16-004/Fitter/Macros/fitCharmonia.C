#include "Utilities/initClasses.h"
#include "buildCharmoniaMassModel.C"
#include "buildCharmoniaCtauModel.C"
#include "drawMassPlot.C"
#include "drawCtauPlot.C"

#include <algorithm>

void setCtauCuts(struct KinCuts& cut, bool isPbPb);
int  importDataset(RooWorkspace& myws, RooWorkspace& inputWS, struct KinCuts cut, string label);
bool setMassModel( struct OniaModel& model, map<string, string>  parIni, bool isPbPb, bool incJpsi, bool incPsi2S, bool incBkg);
bool setCtauModel( struct OniaModel& model, map<string, string>  parIni, bool isPbPb, bool incJpsi, bool incPsi2S, bool incBkg, bool incPrompt, bool incNonPrompt);

void setOptions(struct InputOpt* opt);
void setFileName(string& FileName, string outputDir, string TAG, string plotLabel, struct KinCuts cut, bool fitMass, bool fitCtau, bool doSimulFit, bool isPbPb=true);
bool saveWorkSpace(RooWorkspace& myws, string outputDir, string plotLabel, string DSTAG, struct KinCuts cut, bool fitMass, bool fitCtau, bool doSimulFit, bool isPbPb=true);

bool isFitAlreadyFound(RooArgSet *newpars, string outputDir, string plotLabel, string DSTAG, struct KinCuts cut, bool fitMass, bool fitCtau, bool doSimulFit, bool isPbPb=true);
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

  // Define the mass range
  if (fitMass) {
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
  }
  // Define the mass range
  if (fitCtau) {
    if (cut.dMuon.ctau.Min==-10. && cut.dMuon.ctau.Max==10.) { 
      // Default ctau values, means that the user did not specify a ctau range
      cut.dMuon.ctau.Min = -3.0;
      cut.dMuon.ctau.Max = 3.0;
    }
    parIni["Model_CtauRes_PbPb"] = "DoubleGaussianResolution";
    parIni["Model_CtauRes_PP"]   = "DoubleGaussianResolution";
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
  
  string plotMassLabelPbPb,  plotMassLabelPP, plotCtauLabelPbPb,  plotCtauLabelPP;

  struct OniaModel model;
  RooWorkspace     myws("workspace", "local workspace");

  bool doFit = true;
  if (doSimulFit || !isPbPb) {
    
    // Set models based on initial parameters
    if (fitMass) { if (!setMassModel(model, parIni, false, incJpsi, incPsi2S, incBkg)) { return false; } }
    if (fitCtau) { if (!setCtauModel(model, parIni, false, incJpsi, incPsi2S, incBkg, incPrompt, incNonPrompt)) { return false; } }
    
    // Import the local datasets
    string label = Form("%s_%s", DSTAG.c_str(), "PP");
    if (wantPureSMC) label = Form("%s_%s_NoBkg", DSTAG.c_str(), "PP");
    string dsName = Form("dOS_%s", label.c_str());
    int importID = importDataset(myws, inputWorkspace, cut, label);
    if (importID<0) { return false; }
    else if (importID==0) { doFit = false; }
    
    // Build the Fit Model    
    double numEntries = myws.data(dsName.c_str())->sumEntries();
    if (fitMass) { if (!buildCharmoniaMassModel(myws, model.PP, parIni, false, doSimulFit, incBkg, incJpsi, incPsi2S, numEntries))  { return false; } }
    if (fitCtau) { if (!buildCharmoniaCtauModel(myws, model.PP, parIni, false, incBkg, incJpsi, incPsi2S, incPrompt, incNonPrompt, numEntries))  { return false; } }

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
  }
  if (doSimulFit || isPbPb) {
    
    // Set models based on initial parameters
    if (fitMass) { if (!setMassModel(model, parIni, true, incJpsi, incPsi2S, incBkg)) { return false; } }
    if (fitCtau) { if (!setCtauModel(model, parIni, true, incJpsi, incPsi2S, incBkg, incPrompt, incNonPrompt)) { return false; } }
    
    // Import the local datasets
    string label = Form("%s_%s", DSTAG.c_str(), "PbPb");
    if (wantPureSMC) label = Form("%s_%s_NoBkg", DSTAG.c_str(), "PbPb");
    string dsName = Form("dOS_%s", label.c_str());
    int importID = importDataset(myws, inputWorkspace, cut, label);
    if (importID<0) { return false; }
    else if (importID==0) { doFit = false; }
    
    // Build the Fit Model
    double    numEntries = myws.data(dsName.c_str())->sumEntries();
    if (fitMass) { if (!buildCharmoniaMassModel(myws, model.PbPb, parIni, true, doSimulFit, incBkg, incJpsi, incPsi2S, numEntries)) { return false; } }
    if (fitCtau) { if (!buildCharmoniaCtauModel(myws, model.PbPb, parIni, true, incBkg, incJpsi, incPsi2S, incPrompt, incNonPrompt, numEntries)) { return false; } }

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
  }

  if (doFit)
  {
    if (doSimulFit) {

      string plotLabel = ""; string pdfName = "";
      if (fitMass && !fitCtau) { pdfName = "pdfMASS";     plotLabel = plotMassLabelPbPb+plotCtauLabelPbPb; }
      if (!fitMass && fitCtau) { pdfName = "pdfCTAU";     plotLabel = plotMassLabelPbPb;                   }
      if (fitMass &&  fitCtau) { pdfName = "pdfCTAUMASS"; plotLabel = plotCtauLabelPbPb;                   }
      
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
      RooArgSet *newpars = myws.pdf("simPdf")->getParameters(RooArgSet(*myws.var("invMass"),*myws.var("ctau")));
      found = found && isFitAlreadyFound(newpars, outputDir, plotLabel, DSTAG, cut, fitMass, fitCtau, true);
      if (found) {
        cout << "[INFO] This fit was already done, so I'll just go to the next one." << endl;
        return true;
      }
      myws.saveSnapshot("simPdf_parIni", *newpars, kTRUE) ;

      // Do the simultaneous fit
      RooFitResult* fitResult = simPdf->fitTo(*combData, Offset(kTRUE), Extended(kTRUE), NumCPU(numCores), Range("MassWindow,CtauWindow"), Save(), Minimizer("Minuit2","Migrad"));
      fitResult->Print();

      // Create the output files
      int nBins = 0;
      if (fitMass) {
        nBins = min(int( round((cut.dMuon.M.Max - cut.dMuon.M.Min)/binWidth) ), 100);
        drawMassPlot(myws, outputDir, opt, cut, plotMassLabelPbPb, DSTAG, true, incJpsi, incPsi2S, incBkg, cutCtau, false, setLogScale, incSS, zoomPsi, nBins, getMeanPT);
        drawMassPlot(myws, outputDir, opt, cut, plotMassLabelPP, DSTAG, false, incJpsi, incPsi2S, incBkg, cutCtau, false, setLogScale, incSS, zoomPsi, nBins, getMeanPT);
      }
      if (fitCtau) {
        nBins = min(int( round((cut.dMuon.ctau.Max - cut.dMuon.ctau.Min)/binWidth) ), 100);
        drawCtauPlot(myws, outputDir, opt, cut, plotCtauLabelPbPb, DSTAG, true, fitMass, incJpsi, incPsi2S, incBkg, incPrompt, incNonPrompt, cutCtau, false, setLogScale, incSS, nBins);
        drawCtauPlot(myws, outputDir, opt, cut, plotCtauLabelPP, DSTAG, false, fitMass, incJpsi, incPsi2S, incBkg, incPrompt, incNonPrompt, cutCtau, false, setLogScale, incSS, nBins);
      }
      saveWorkSpace(myws, outputDir, plotLabel, DSTAG, cut, fitMass, fitCtau, true);
 
      // Delete the objects used during the simultaneous fit
      delete sample; delete combData; delete simPdf;
      
    }
    else {
      if (isPbPb) {

        string plotLabel = ""; string pdfName = "";
        if (fitMass && !fitCtau) { pdfName = "pdfMASS_Tot_PbPb";     plotLabel = plotMassLabelPbPb+plotCtauLabelPbPb; }
        if (!fitMass && fitCtau) { pdfName = "pdfCTAU_Tot_PbPb";     plotLabel = plotMassLabelPbPb;                   }
        if (fitMass &&  fitCtau) { pdfName = "pdfCTAUMASS_Tot_PbPb"; plotLabel = plotCtauLabelPbPb;                   }
        
        string dsName = Form("dOS_%s_PbPb", DSTAG.c_str());
        if (wantPureSMC) dsName = Form("dOS_%s_PbPb_NoBkg", DSTAG.c_str());

        // check if we have already done this fit. If yes, do nothing and return true.
        RooArgSet *newpars = myws.pdf(pdfName.c_str())->getParameters(RooArgSet(*myws.var("invMass"), *myws.var("ctau"), *myws.var("ctauErr")));
        bool found =  true;
        found = found && isFitAlreadyFound(newpars, outputDir, (wantPureSMC ? (plotLabel+"_NoBkg") : plotLabel), DSTAG, cut, fitMass, fitCtau, false, true);
        if (found) {
          cout << "[INFO] This fit was already done, so I'll just go to the next one." << endl;
          return true;
        }

        bool isWeighted = myws.data(dsName.c_str())->isWeighted();
        myws.Print();
        // Fit the Datasets
        if (incJpsi || incPsi2S) {
          if (isWeighted) {
            RooFitResult* fitResult = myws.pdf(pdfName.c_str())->fitTo(*myws.data(dsName.c_str()), Extended(kTRUE), SumW2Error(kTRUE), Range("CtauWindow"), NumCPU(numCores), Save());
            fitResult->Print();
          } else {
            RooFitResult* fitResult = myws.pdf(pdfName.c_str())->fitTo(*myws.data(dsName.c_str()), Extended(kTRUE), Range("CtauWindow&&MassWindow"), ConditionalObservables(*myws.var("ctauErr")), NumCPU(numCores), Save());
            fitResult->Print();
          }  
        } else {
          RooFitResult* fitResult = myws.pdf(pdfName.c_str())->fitTo(*myws.data(dsName.c_str()), Extended(kTRUE), Range("(SideBand1||SideBand2)&&CtauWindow"), NumCPU(numCores), Save());
          fitResult->Print();
        }
        
        // Create the output files
        int nBins = 0;
        if (fitMass) { 
          nBins = min(int( round((cut.dMuon.M.Max - cut.dMuon.M.Min)/binWidth) ), 100);
          drawMassPlot(myws, outputDir, opt, cut, (wantPureSMC ? (plotMassLabelPbPb+"_NoBkg") : plotMassLabelPbPb), DSTAG, true, incJpsi, incPsi2S, incBkg, cutCtau, wantPureSMC, setLogScale, incSS, zoomPsi, nBins, getMeanPT); 
        }
        if (fitCtau) { 
          nBins = min(int( round((cut.dMuon.ctau.Max - cut.dMuon.ctau.Min)/binWidth) ), 100);
          drawCtauPlot(myws, outputDir, opt, cut, (wantPureSMC ? (plotCtauLabelPbPb+"_NoBkg") : plotCtauLabelPbPb), DSTAG, true, fitMass, incJpsi, incPsi2S, incBkg, incPrompt, incNonPrompt, cutCtau, wantPureSMC, setLogScale, incSS, nBins); 
        }
        saveWorkSpace(myws, outputDir, plotLabel, DSTAG, cut, fitMass, fitCtau, false, true);
        return false;
      }
      else {
        cut.Centrality.Start = 0;
        cut.Centrality.End = 200;

        string plotLabel = ""; string pdfName = "";
        if (fitMass && !fitCtau) { pdfName = "pdfMASS_Tot_PP";     plotLabel = plotMassLabelPP+plotCtauLabelPP; }
        if (!fitMass && fitCtau) { pdfName = "pdfCTAU_Tot_PP";     plotLabel = plotMassLabelPP;                 }
        if (fitMass &&  fitCtau) { pdfName = "pdfCTAUMASS_Tot_PP"; plotLabel = plotCtauLabelPP;                 }
        string dsName = Form("dOS_%s_PP", DSTAG.c_str());
        if (wantPureSMC) dsName = Form("dOS_%s_PP_NoBkg", DSTAG.c_str());
        
        // check if we have already done this fit. If yes, do nothing and return true.
        RooArgSet *newpars = myws.pdf(pdfName.c_str())->getParameters(RooArgSet(*myws.var("invMass"), *myws.var("ctau"), *myws.var("ctau")));
        bool found =  true;
        found = found && isFitAlreadyFound(newpars, outputDir, (wantPureSMC ? (plotLabel+"_NoBkg") : plotLabel), DSTAG, cut, fitMass, fitCtau, false, false);
        if (found) {
          cout << "[INFO] This fit was already done, so I'll just go to the next one." << endl;
          return true;
        }

        bool isWeighted = myws.data(dsName.c_str())->isWeighted();
        
        // Fit the Datasets
        if (incJpsi || incPsi2S) {
          RooFitResult* fitResult = myws.pdf(pdfName.c_str())->fitTo(*myws.data(dsName.c_str()), Extended(kTRUE), ConditionalObservables(*myws.var("ctauErr")), NumCPU(numCores), Save());
          fitResult->Print(); 
        } else {
          RooFitResult* fitResult = myws.pdf(pdfName.c_str())->fitTo(*myws.data(dsName.c_str()), Extended(kTRUE), Range("(SideBand1||SideBand2)&&CtauWindow"), ConditionalObservables(*myws.var("ctauErr")), NumCPU(numCores), Save());
          fitResult->Print();
        }
        
        // Draw the mass plot
        int nBins = 0;
        if (fitMass) { 
          nBins = min(int( round((cut.dMuon.M.Max - cut.dMuon.M.Min)/binWidth) ), 100);
          drawMassPlot(myws, outputDir, opt, cut, (wantPureSMC ? (plotMassLabelPP+"_NoBkg") : plotMassLabelPP), DSTAG, false, incJpsi, incPsi2S, incBkg, cutCtau, wantPureSMC, setLogScale, incSS, zoomPsi, nBins, getMeanPT); 
        }
        if (fitCtau) { 
          nBins = min(int( round((cut.dMuon.ctau.Max - cut.dMuon.ctau.Min)/binWidth) ), 100);
          drawCtauPlot(myws, outputDir, opt, cut, (wantPureSMC ? (plotCtauLabelPP+"_NoBkg") : plotCtauLabelPP), DSTAG, false, fitMass, incJpsi, incPsi2S, incBkg, incPrompt, incNonPrompt, cutCtau, wantPureSMC, setLogScale, incSS, nBins); 
        }
        saveWorkSpace(myws, outputDir, plotLabel, DSTAG, cut, fitMass, fitCtau, false, false);
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


bool setMassModel( struct OniaModel& model, map<string, string>  parIni, bool isPbPb, bool incJpsi, bool incPsi2S, bool incBkg )
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


bool setCtauModel( struct OniaModel& model, map<string, string>  parIni, bool isPbPb, bool incJpsi, bool incPsi2S, bool incBkg, bool incPrompt, bool incNonPrompt )
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
      cout << "[ERROR] Background prompt ctau model for PbPb was not found in the initial parameters!" << endl; return false;
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
      cout << "[ERROR] Jpsi prompt ctau model for PbPb was not found in the initial parameters!" << endl; return false;
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
      cout << "[ERROR] psi(2S) prompt ctau model for PbPb was not found in the initial parameters!" << endl; return false;
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
      cout << "[ERROR] Background prompt ctau model for PP was not found in the initial parameters!" << endl; return false;
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
      cout << "[ERROR] Jpsi prompt ctau model for PP was not found in the initial parameters!" << endl; return false;
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
      cout << "[ERROR] psi(2S) prompt ctau model for PP was not found in the initial parameters!" << endl; return false;
    }
  }

  return true;
};
   

int importDataset(RooWorkspace& myws, RooWorkspace& inputWS, struct KinCuts cut, string label)
{
  string indMuonMass    = Form("(%.6f < invMass && invMass < %.6f)",       cut.dMuon.M.Min,       cut.dMuon.M.Max);
  string indMuonRap     = Form("(%.6f <= abs(rap) && abs(rap) < %.6f)",    cut.dMuon.AbsRap.Min,  cut.dMuon.AbsRap.Max);
  string indMuonPt      = Form("(%.6f <= pt && pt < %.6f)",                cut.dMuon.Pt.Min,      cut.dMuon.Pt.Max);
  string indMuonCtau    = Form("(%.6f < ctau && ctau < %.6f)",             cut.dMuon.ctau.Min,    cut.dMuon.ctau.Max); 
  if(cut.dMuon.ctauCut!=""){ indMuonCtau = cut.dMuon.ctauCut; }
  string indMuonCtauErr = Form("(%.6f < ctauErr && ctauErr < %.6f)",       cut.dMuon.ctauErr.Min, cut.dMuon.ctauErr.Max);
  string inCentrality   = Form("(%d <= cent && cent < %d)",                cut.Centrality.Start,  cut.Centrality.End);

  string strCut         = indMuonMass +"&&"+ indMuonRap +"&&"+ indMuonPt +"&&"+ indMuonCtau +"&&"+ indMuonCtauErr;
  if (label.find("PbPb")!=std::string::npos){ strCut = strCut +"&&"+ inCentrality; } 

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
  
  // Set the range of each global parameter in the local workspace
  myws.var("invMass")->setMin(cut.dMuon.M.Min);        
  myws.var("invMass")->setMax(cut.dMuon.M.Max);
  myws.var("pt")->setMin(cut.dMuon.Pt.Min);            
  myws.var("pt")->setMax(cut.dMuon.Pt.Max);
  myws.var("rap")->setMin(cut.dMuon.AbsRap.Min);       
  myws.var("rap")->setMax(cut.dMuon.AbsRap.Max);
  myws.var("ctau")->setMin(cut.dMuon.ctau.Min);        
  myws.var("ctau")->setMax(cut.dMuon.ctau.Max);
  myws.var("ctauErr")->setMin(cut.dMuon.ctauErr.Min);  
  myws.var("ctauErr")->setMax(cut.dMuon.ctauErr.Max);
  if (label.find("PbPb")!=std::string::npos){
    myws.var("cent")->setMin(cut.Centrality.Start);      
    myws.var("cent")->setMax(cut.Centrality.End);
  }

  myws.var("ctau")->setRange("CtauWindow", cut.dMuon.ctau.Min, cut.dMuon.ctau.Max);
  myws.var("ctauErr")->setRange("CtauErrWindow", cut.dMuon.ctauErr.Min, cut.dMuon.ctauErr.Max);
  if (label.find("MC")!=std::string::npos)
  {
    if (label.find("PSI2S")!=std::string::npos)
    {
      if (cut.dMuon.AbsRap.Min >= 1.6) myws.var("invMass")->setRange("MassWindow", cut.dMuon.M.Min, 3.95);
      else myws.var("invMass")->setRange("MassWindow", cut.dMuon.M.Min, 3.85);
    }
    else
    {
      if (cut.dMuon.AbsRap.Min >= 1.6) myws.var("invMass")->setRange("MassWindow", cut.dMuon.M.Min, 3.32);
      else myws.var("invMass")->setRange("MassWindow", cut.dMuon.M.Min, 3.26);
    }
   
  }
  else myws.var("invMass")->setRange("MassWindow", cut.dMuon.M.Min, cut.dMuon.M.Max);
  
  if (cut.dMuon.M.Min<2.8) { myws.var("invMass")->setRange("SideBand1",  cut.dMuon.M.Min, 2.8); }
  if (cut.dMuon.M.Max>4.0) { myws.var("invMass")->setRange("SideBand2",  4.0, cut.dMuon.M.Max); }

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

void setFileName(string& FileName, string outputDir, string TAG, string plotLabel, struct KinCuts cut,bool fitMass, bool fitCtau, bool doSimulFit, bool isPbPb) 
{
  if (doSimulFit) {
    if (fitMass && fitCtau) {
      FileName = Form("%sresult/%s/FIT_%s_%s_%s_%s%s_pt%.0f%.0f_rap%.0f%.0f_cent%d%d.root", outputDir.c_str(), TAG.c_str(), "CTAUMASS", TAG.c_str(), "Psi2SJpsi", "COMB", plotLabel.c_str(), (cut.dMuon.Pt.Min*10.0), (cut.dMuon.Pt.Max*10.0), (cut.dMuon.AbsRap.Min*10.0), (cut.dMuon.AbsRap.Max*10.0), cut.Centrality.Start, cut.Centrality.End);
    } else if (fitMass) {
      FileName = Form("%sresult/%s/FIT_%s_%s_%s_%s%s_pt%.0f%.0f_rap%.0f%.0f_cent%d%d.root", outputDir.c_str(), TAG.c_str(), "MASS", TAG.c_str(), "Psi2SJpsi", "COMB", plotLabel.c_str(), (cut.dMuon.Pt.Min*10.0), (cut.dMuon.Pt.Max*10.0), (cut.dMuon.AbsRap.Min*10.0), (cut.dMuon.AbsRap.Max*10.0), cut.Centrality.Start, cut.Centrality.End);
    } else if (fitCtau) {
      FileName = Form("%sresult/%s/FIT_%s_%s_%s_%s%s_pt%.0f%.0f_rap%.0f%.0f_cent%d%d.root", outputDir.c_str(), TAG.c_str(), "CTAU", TAG.c_str(), "Psi2SJpsi", "COMB", plotLabel.c_str(), (cut.dMuon.Pt.Min*10.0), (cut.dMuon.Pt.Max*10.0), (cut.dMuon.AbsRap.Min*10.0), (cut.dMuon.AbsRap.Max*10.0), cut.Centrality.Start, cut.Centrality.End);
    }
  } else {
    if (fitMass && fitCtau) {
      FileName = Form("%sresult/%s/FIT_%s_%s_%s_%s%s_pt%.0f%.0f_rap%.0f%.0f_cent%d%d.root", outputDir.c_str(), TAG.c_str(), "CTAUMASS", TAG.c_str(), "Psi2SJpsi", (isPbPb?"PbPb":"PP"), plotLabel.c_str(), (cut.dMuon.Pt.Min*10.0), (cut.dMuon.Pt.Max*10.0), (cut.dMuon.AbsRap.Min*10.0), (cut.dMuon.AbsRap.Max*10.0), cut.Centrality.Start, cut.Centrality.End);
    } else if (fitMass) {
      FileName = Form("%sresult/%s/FIT_%s_%s_%s_%s%s_pt%.0f%.0f_rap%.0f%.0f_cent%d%d.root", outputDir.c_str(), TAG.c_str(), "MASS", TAG.c_str(), "Psi2SJpsi", (isPbPb?"PbPb":"PP"), plotLabel.c_str(), (cut.dMuon.Pt.Min*10.0), (cut.dMuon.Pt.Max*10.0), (cut.dMuon.AbsRap.Min*10.0), (cut.dMuon.AbsRap.Max*10.0), cut.Centrality.Start, cut.Centrality.End);
    } else if (fitCtau) {
      FileName = Form("%sresult/%s/FIT_%s_%s_%s_%s%s_pt%.0f%.0f_rap%.0f%.0f_cent%d%d.root", outputDir.c_str(), TAG.c_str(), "CTAU", TAG.c_str(), "Psi2SJpsi", (isPbPb?"PbPb":"PP"), plotLabel.c_str(), (cut.dMuon.Pt.Min*10.0), (cut.dMuon.Pt.Max*10.0), (cut.dMuon.AbsRap.Min*10.0), (cut.dMuon.AbsRap.Max*10.0), cut.Centrality.Start, cut.Centrality.End);
    } 
  }
};

bool saveWorkSpace(RooWorkspace& myws, string outputDir, string plotLabel, string DSTAG, struct KinCuts cut, bool fitMass, bool fitCtau, bool doSimulFit, bool isPbPb) 
{
  // Save the workspace
  gSystem->mkdir(Form("%sresult/%s/", outputDir.c_str(), DSTAG.c_str()), kTRUE); 
  string FileName = "";
  setFileName(FileName, outputDir, DSTAG, plotLabel, cut, fitMass, fitCtau, doSimulFit, isPbPb);
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

bool isFitAlreadyFound(RooArgSet *newpars, string outputDir, string plotLabel, string DSTAG, struct KinCuts cut, bool fitMass, bool fitCtau, bool doSimulFit, bool isPbPb) 
{
  string FileName = "";
  setFileName(FileName, outputDir, DSTAG, plotLabel, cut, fitMass, fitCtau, doSimulFit, isPbPb);
  if (gSystem->AccessPathName(FileName.c_str())) return false; // File was not found

  TFile *file = new TFile(FileName.c_str());

  if (!file) return false;

  RooWorkspace *ws = (RooWorkspace*) file->Get("workspace");
  if (!ws) {
    file->Close(); delete file;
    return false;
  }

  string snapShotName = "";
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
