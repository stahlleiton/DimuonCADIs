#ifndef fitCharmonia_C
#define fitCharmonia_C

#include "Utilities/initClasses.h"
#include "fitCharmoniaMassModel.C"
#include "fitCharmoniaCtauModel.C"
#include "fitCharmoniaCtauErrModel.C"
#include "fitCharmoniaCtauTrueModel.C"
#include "fitCharmoniaCtauRecoModel.C"
#include "fitCharmoniaCtauMassModel.C"
#include "fitCharmoniaCtauResModel.C"
#include "fitCharmoniaCtauResDataModel.C"

void setOptions ( struct InputOpt& opt );

bool fitCharmonia( const RooWorkspace& inputWorkspace,    // Workspace with all the input RooDatasets
		         struct KinCuts cut,              // Variable containing all kinematic cuts
		         map<string, string> parIni,      // Variable containing all initial parameters
		   const string& outputDir,               // Path to output directory
                   // Select the type of datasets to fit
		   const string& DSTAG,                   // Specifies the type of datasets: i.e, DATA, MCJPSINP, ...
		   const bool& isPbPb,                    // isPbPb = false for pp, true for PbPb
                   // Select the type of object to fit
                   const bool& fitMass,                   // Fit mass distribution
                   const bool& fitCtau,                   // Fit ctau distribution
                   const bool& fitCtauTrue,               // Fit ctau truth MC distribution
                   const bool& fitCtauReco,               // Fit ctau reco MC distribution
                   const bool& incJpsi,                   // Includes Jpsi model
                   const bool& incPsi2S,                  // Includes Psi(2S) model
                   const bool& incBkg,                    // Includes Background model
                   const bool& incPrompt,                 // Includes Prompt ctau model
                   const bool& incNonPrompt,              // Includes NonPrompt ctau model
                   const bool& doCtauErrPDF,              // If yes, it builds the Ctau Error PDFs from data
                   const bool& fitRes,                    // If yes fits the resolution from Data or MC
                   // Select the fitting options
                   const bool& useTotctauErrPdf,          // If yes use the total ctauErr PDF instead of Jpsi and bkg ones
                   const bool& usectauBkgTemplate,        // If yes use a template for Bkg ctau instead of the fitted Pdf
                   const bool& useCtauRecoPdf,            // If yes use the ctauReco PDF (template) instead of ctauTrue one
                         bool  cutCtau,                   // Apply prompt ctau cuts
                   const bool& doConstrFit,               // Do constrained fit
                   const bool& doSimulFit,                // Do simultaneous fit
                   const bool& doJpsi,                    // Flag to indicate if we want to perform fits for Jpsi analysis
                   const bool& doPsi2S,                   // Flag to indicate if we want to perform fits for Psi2S analysis
                         bool  wantPureSMC,               // Flag to indicate if we want to fit pure signal MC
                         map<string, string> inputFitDir, // User-defined Location of the fit results
                   const string& applyCorr,               // Flag to indicate if we want corrected dataset and which correction
                   const int&  numCores,                  // Number of cores used for fitting
                   // Select the drawing options
                   const bool& setLogScale,               // Draw plot with log scale
                   const bool& incSS,                     // Include Same Sign data
                   const bool& zoomPsi,                   // Zoom Psi(2S) peak on extra pad
                         map<string, double> binWidth,    // Bin width used for plotting
                         bool  getMeanPT                  // Compute the mean PT (NEED TO FIX)
		   )  
{

  RooMsgService::instance().getStream(0).removeTopic(RooFit::Caching);
  RooMsgService::instance().getStream(0).removeTopic(RooFit::Plotting);
  RooMsgService::instance().getStream(0).removeTopic(RooFit::Integration);
  RooMsgService::instance().getStream(0).removeTopic(RooFit::NumIntegration);
  RooMsgService::instance().getStream(0).removeTopic(RooFit::Minimization);
  RooMsgService::instance().getStream(1).removeTopic(RooFit::Caching);
  RooMsgService::instance().getStream(1).removeTopic(RooFit::Plotting);
  RooMsgService::instance().getStream(1).removeTopic(RooFit::Integration);
  RooMsgService::instance().getStream(1).removeTopic(RooFit::NumIntegration);
  RooMsgService::instance().getStream(1).removeTopic(RooFit::Minimization);
  RooMsgService::instance().setGlobalKillBelow(RooFit::WARNING);;

  RooWorkspace myws("workspace", "local workspace");

  // Preventing issues in the fitter
  cutCtau = (cutCtau && !fitCtau && !fitCtauTrue);
  getMeanPT = false;
  wantPureSMC = (DSTAG.find("MC")!=std::string::npos && wantPureSMC);
  const bool isMC = (DSTAG.find("MC")!=std::string::npos);

  // Setting default user-defined input fit directories ( "" means use current working directory )
  if (inputFitDir.count("MASS")==0)     { inputFitDir["MASS"]     = ""; }
  if (inputFitDir.count("CTAUTRUE")==0) { inputFitDir["CTAUTRUE"] = ""; }
  if (inputFitDir.count("CTAUERR")==0)  { inputFitDir["CTAUERR"]  = ""; }
  if (inputFitDir.count("CTAURES")==0)  { inputFitDir["CTAURES"]  = ""; }
  if (inputFitDir.count("CTAUSB")==0)   { inputFitDir["CTAUSB"]   = ""; }
  // Setting default user-defined bin width
  if (binWidth.count("MASS")==0)     { binWidth["MASS"]     = 0.05; }
  if (binWidth.count("CTAUTRUE")==0) { binWidth["CTAUTRUE"] = 0.05; }
  if (binWidth.count("CTAUERR")==0)  { binWidth["CTAUERR"]  = 0.05; }
  if (binWidth.count("CTAUSB")==0)   { binWidth["CTAUSB"]   = 0.05; }
  if (binWidth.count("CTAURES")==0)  { binWidth["CTAURES"]  = 0.05; }
  if (binWidth.count("CTAU")==0)     { binWidth["CTAU"]     = 0.05; }
  binWidth["CTAUERRFORCUT"]  = 0.0025;

  if (isPbPb==false) {
    cut.Centrality.Start = 0;
    cut.Centrality.End = 200;
  }

  // Setting run information
  struct InputOpt opt; setOptions(opt);

  // Starting the fits for each variable (where the magic starts)
  if (fitMass && !fitCtau && !fitCtauTrue && !doCtauErrPDF && !fitRes && !fitCtauReco) {
    
    // Setting extra input information needed by each fitter
    const double& ibWidth = binWidth.at("MASS");
    const string& iFitDir = inputFitDir.at("MASS");
    const bool loadFitResult = false;
    const bool doFit = true;
    const bool importDS = true;

    if ( !fitCharmoniaMassModel( myws, inputWorkspace, cut, parIni, opt, outputDir, 
                                 DSTAG, isPbPb, importDS,
                                 incJpsi, incPsi2S, incBkg, 
                                 doFit, cutCtau, doConstrFit, doSimulFit, wantPureSMC, applyCorr, loadFitResult, iFitDir, numCores,
                                 setLogScale, incSS, zoomPsi, ibWidth, getMeanPT 
                                 ) 
         ) { return false; }
  }

  if (fitCtauTrue && !fitCtau && !fitMass && !doCtauErrPDF && !fitRes && !fitCtauReco) {

    // Setting extra input information needed by each fitter
    const double& ibWidth = binWidth.at("CTAUTRUE");
    const string& iFitDir = inputFitDir.at("CTAUTRUE");
    const bool loadFitResult = false;
    const bool doFit = true;
    const bool importDS = true;
    const bool incResol = false;

    if ( !fitCharmoniaCtauTrueModel( myws, inputWorkspace, cut, parIni, opt, outputDir, 
                                     DSTAG, isPbPb, importDS, 
                                     incJpsi, incPsi2S, incResol, 
                                     doFit, wantPureSMC, loadFitResult, iFitDir, numCores, 
                                     setLogScale, incSS, ibWidth
                                     ) 
         ) { return false; }
  }

  if (fitCtauReco && !fitCtauTrue && !fitCtau && !fitMass && !doCtauErrPDF && !fitRes) {
    
    // Setting extra input information needed by each fitter
    const double ibWidth = binWidth.at("CTAURECO");
    const string iFitDir = inputFitDir.at("CTAURECO");
    const bool loadFitResult = false;
    const bool importDS = true;
    const bool doCtauRecoPdf = true;
    
    if ( !fitCharmoniaCtauRecoModel( myws, inputWorkspace, cut, parIni, opt, outputDir,
                                    DSTAG, isPbPb, importDS,
                                    incJpsi, incPsi2S,
                                    doCtauRecoPdf, wantPureSMC, loadFitResult, iFitDir, numCores,
                                    setLogScale, incSS, ibWidth
                                    )
        ) { return false; }
  }
  
  if (doCtauErrPDF && !fitCtau && !fitCtauTrue && !fitMass && !fitRes && !fitCtauReco) {

    // Setting extra input information needed by each fitter
    const bool loadFitResult = false;
    const bool doFit = true;
    const bool importDS = true;

    if ( !fitCharmoniaCtauErrModel( myws, inputWorkspace, cut, parIni, opt, outputDir, 
                                    DSTAG, isPbPb, importDS, 
                                    incJpsi, incPsi2S, incBkg, 
                                    doFit, wantPureSMC, loadFitResult, inputFitDir, numCores, 
                                    setLogScale, incSS, binWidth
                                    ) 
         ) { return false; }
  }

  if (fitCtau && !doCtauErrPDF && !fitCtauTrue && !fitMass && !fitRes && !fitCtauReco && ((incJpsi!=incBkg && !incPsi2S) || (incPsi2S!=incBkg && !incJpsi)) && !isMC) {

    // Setting extra input information needed by each fitter
    const bool loadFitResult = false;
    const bool doFit = true;
    const bool importDS = true;
    const bool useSPlot = true;

    if ( !fitCharmoniaCtauModel( myws, inputWorkspace, cut, parIni, opt, outputDir, 
                                 DSTAG, isPbPb, importDS, 
                                 incJpsi, incPsi2S, incBkg, incPrompt, incNonPrompt, useTotctauErrPdf, usectauBkgTemplate,
                                 useSPlot, doFit, doJpsi, doPsi2S, wantPureSMC, loadFitResult, inputFitDir, numCores, 
                                 setLogScale, incSS, binWidth
                                 ) 
         ) { return false; }
  }

  if (fitRes && !doCtauErrPDF && !fitCtauTrue && !fitMass && !fitCtau && !fitCtauReco && !incBkg && isMC) {

    // Setting extra input information needed by each fitter
    const bool loadFitResult = false;
    const bool doFit = true;
    const bool importDS = true;

    if ( !fitCharmoniaCtauResModel( myws, inputWorkspace, cut, parIni, opt, outputDir, 
                                    DSTAG, isPbPb, importDS, 
                                    incJpsi, incPsi2S, useTotctauErrPdf,
                                    doFit, wantPureSMC, loadFitResult, inputFitDir, numCores, 
                                    setLogScale, incSS, binWidth
                                    ) 
         ) { return false; }
  }
  
  if (fitRes && !doCtauErrPDF && !fitCtauTrue && !fitMass && !fitCtau && !fitCtauReco && !isMC) {
    
    // Setting extra input information needed by each fitter
    const bool loadFitResult = false;
    const bool doFit = true;
    const bool importDS = true;    
    const bool useSPlot = true;

    if ( !fitCharmoniaCtauResDataModel( myws, inputWorkspace, cut, parIni, opt, outputDir,
                                        DSTAG, isPbPb, importDS,
                                        incJpsi, incPsi2S, incBkg, useSPlot, useTotctauErrPdf,
                                        doFit, loadFitResult, inputFitDir, numCores,
                                        setLogScale, incSS, binWidth
                                        )
         ) { return false; }
  }

  if (fitCtau && fitMass && !doCtauErrPDF && !fitCtauTrue && !fitRes && !fitCtauReco ) {

    // Setting extra input information needed by each fitter
   
    if ( !fitCharmoniaCtauMassModel( myws, inputWorkspace, cut, parIni, opt, outputDir, 
                                     DSTAG, isPbPb,
                                     incJpsi, incPsi2S, useTotctauErrPdf, usectauBkgTemplate, useCtauRecoPdf,
                                     inputFitDir, numCores,
                                     setLogScale, incSS, binWidth
                                     ) 
         ) { return false; }
  }

  return true;
};


void setOptions(struct InputOpt& opt) 
{
  opt.pp.RunNb.Start   = 262157; opt.PbPb.RunNb.Start = 262620;
  opt.pp.RunNb.End     = 262328; opt.PbPb.RunNb.End   = 263757;
  opt.pp.TriggerBit    = (int) PP::HLT_HIL1DoubleMu0_v1; 
  opt.PbPb.TriggerBit  = (int) HI::HLT_HIL1DoubleMu0_v1; 
  return;
};


#endif // #ifndef fitCharmonia_C
