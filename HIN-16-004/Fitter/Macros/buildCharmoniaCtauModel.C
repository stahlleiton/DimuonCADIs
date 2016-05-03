#include "Utilities/initClasses.h"

void fixCtauParPsi2StoJpsi(map<string, string>& parIni, bool isPbPb);
void setCtauDefaultParameters(map<string, string> &parIni, bool isPbPb, double numEntries);
bool defineCtauResolModel(RooWorkspace& ws, string object, CtauModel model, map<string,string> parIni, bool isPbPb, bool usePromptCtauRes); 
bool addSignalCtauModel(RooWorkspace& ws, string object, CtauModel model, map<string,string> parIni, bool isPbPb); 
bool addBackgroundCtauModel(RooWorkspace& ws, string object, CtauModel model, map<string,string> parIni, bool isPbPb);


bool buildCharmoniaCtauModel(RooWorkspace& ws, struct CharmModel model, map<string, string>  parIni, 
                             bool isPbPb,                 // Determine if we are working with PbPb (True) or PP (False)
                             bool incBkg,                 // Include background model
                             bool incJpsi,                // Include Jpsi model
                             bool incPsi2S,               // Include Psi(2S) model
                             bool incPrompt,              // Include Prompt models
                             bool incNonPrompt,           // Include NonPrompt models
                             double  numEntries = 300000. // Number of entries in the dataset
                             )
{

  // If the initial parameters are empty, set defaul parameter values
  setCtauDefaultParameters(parIni, isPbPb, numEntries);
  
  // Fix all psi2S parameters to jpsi
  if (incJpsi && incPsi2S) {
    fixCtauParPsi2StoJpsi(parIni, isPbPb);
  }

  // C r e a t e   m o d e l 
     
  bool fitMass = false;
  if ( ws.pdf(Form("pdfMASS_Tot_%s", (isPbPb?"PbPb":"PP"))) ) { fitMass = true; } 

  string pdfName     = "pdfCTAU";
  if (fitMass) { pdfName = "pdfCTAUMASS"; }

  if (incJpsi) {
    if (incPrompt) {
      if(!defineCtauResolModel(ws, "JpsiPR", model.CtauRes, parIni, isPbPb, true)) { cout << "[ERROR] Defining the Prompt Ctau Resolution Model failed" << endl; return false; }
      if(!addSignalCtauModel(ws, "JpsiPR", model.Jpsi.Ctau.Prompt, parIni, isPbPb)) { cout << "[ERROR] Adding Prompt Jpsi Ctau Model failed" << endl; return false; }
    }
    if (incNonPrompt) {
      if(!defineCtauResolModel(ws, "JpsiNoPR", model.CtauRes, parIni, isPbPb, false)) { cout << "[ERROR] Defining the Non-Prompt Ctau Resolution Model failed" << endl; return false; }
      if(!addSignalCtauModel(ws, "JpsiNoPR", model.Jpsi.Ctau.NonPrompt, parIni, isPbPb)) { cout << "[ERROR] Adding NonPrompt Jpsi Ctau Model failed" << endl; return false; }
    }
    if (incPrompt && incNonPrompt) {
      ws.factory( parIni[Form("b_Jpsi_%s", (isPbPb?"PbPb":"PP"))].c_str() );
      ws.factory(Form("SUM::%s(%s*%s, %s)", Form("%sTot_Jpsi_%s", pdfName.c_str(), (isPbPb?"PbPb":"PP")),
                      Form("b_Jpsi_%s", (isPbPb?"PbPb":"PP")),
                      Form("%sTot_JpsiNoPR_%s", pdfName.c_str(), (isPbPb?"PbPb":"PP")),
                      Form("%sTot_JpsiPR_%s", pdfName.c_str(), (isPbPb?"PbPb":"PP"))
                      ));
    }
  }
  if (incPsi2S) {
    if (incPrompt) {
      if(!defineCtauResolModel(ws, "Psi2SPR", model.CtauRes, parIni, isPbPb, true)) { cout << "[ERROR] Defining the Prompt Ctau Resolution Model failed" << endl; return false; }
      if(!addSignalCtauModel(ws, "Psi2SPR", model.Psi2S.Ctau.Prompt, parIni, isPbPb)) { cout << "[ERROR] Adding Prompt Psi2S Ctau Model failed" << endl; return false; }
    }
    if (incNonPrompt) {
      if(!defineCtauResolModel(ws, "Psi2SNoPR", model.CtauRes, parIni, isPbPb, false)) { cout << "[ERROR] Defining the Non-Prompt Ctau Resolution Model failed" << endl; return false; }
      if(!addSignalCtauModel(ws, "Psi2SNoPR", model.Psi2S.Ctau.NonPrompt, parIni, isPbPb)) { cout << "[ERROR] Adding NonPrompt Psi2S Ctau Model failed" << endl; return false; }
    }
    if (incPrompt && incNonPrompt) {
      ws.factory( parIni[Form("b_Psi2S_%s", (isPbPb?"PbPb":"PP"))].c_str() );
      ws.factory(Form("SUM::%s(%s*%s, %s)", Form("%sTot_Psi2S_%s", pdfName.c_str(), (isPbPb?"PbPb":"PP")),
                      Form("b_Psi2S_%s", (isPbPb?"PbPb":"PP")),
                      Form("%sTot_Psi2SNoPR_%s", pdfName.c_str(), (isPbPb?"PbPb":"PP")),
                      Form("%sTot_Psi2SPR_%s", pdfName.c_str(), (isPbPb?"PbPb":"PP"))
                      ));
    }
  }
  if (incBkg) {
    if (incPrompt) {
      if(!defineCtauResolModel(ws, "BkgPR", model.CtauRes, parIni, isPbPb, true)) { cout << "[ERROR] Defining the Prompt Ctau Resolution Model failed" << endl; return false; }
      if(!addBackgroundCtauModel(ws, "BkgPR", model.Bkg.Ctau.Prompt, parIni, isPbPb)) { cout << "[ERROR] Adding Prompt Background Ctau Model failed" << endl; return false; }
    }
    if (incNonPrompt) {
      if(!defineCtauResolModel(ws, "BkgNoPR", model.CtauRes, parIni, isPbPb, true)) { cout << "[ERROR] Defining the Prompt Ctau Resolution Model failed" << endl; return false; }
      if(!addBackgroundCtauModel(ws, "BkgNoPR", model.Bkg.Ctau.NonPrompt, parIni, isPbPb)) { cout << "[ERROR] Adding NonPrompt Background Ctau Model failed" << endl; return false; }
    }
    if (incPrompt && incNonPrompt) {
      ws.factory( parIni[Form("b_Bkg_%s", (isPbPb?"PbPb":"PP"))].c_str() );
      ws.factory(Form("SUM::%s(%s*%s, %s)", Form("%sTot_Bkg_%s", pdfName.c_str(), (isPbPb?"PbPb":"PP")),
                      Form("b_Bkg_%s", (isPbPb?"PbPb":"PP")),
                      Form("%sTot_BkgNoPR_%s", pdfName.c_str(), (isPbPb?"PbPb":"PP")),
                      Form("%sTot_BkgSPR_%s", pdfName.c_str(), (isPbPb?"PbPb":"PP"))
                      ));
    }
  }

  // Total PDF
  RooAbsPdf *themodel = NULL;
  string tag = "";
  if (incPrompt  && !incNonPrompt) { tag = "PR"; }
  if (!incPrompt && incNonPrompt)  { tag = "NoPR";   }

  if (incJpsi && incPsi2S && incBkg) {
    themodel = new RooAddPdf(Form("%s_Tot_%s", pdfName.c_str(), (isPbPb?"PbPb":"PP")), Form("%s_Tot_%s", pdfName.c_str(), (isPbPb?"PbPb":"PP")), 
                             RooArgList( 
                                        *ws.pdf(Form("%sTot_Jpsi%s_%s", pdfName.c_str(), tag.c_str(), (isPbPb?"PbPb":"PP"))), 
                                        *ws.pdf(Form("%sTot_Psi2S%s_%s", pdfName.c_str(), tag.c_str(), (isPbPb?"PbPb":"PP"))), 
                                        *ws.pdf(Form("%sTot_Bkg%s_%s", pdfName.c_str(), tag.c_str(), (isPbPb?"PbPb":"PP"))) 
                                         )
                             );
  }
  if (incJpsi && incPsi2S && !incBkg) {
    themodel = new RooAddPdf(Form("%s_Tot_%s", pdfName.c_str(), (isPbPb?"PbPb":"PP")), Form("%s_Tot_%s", pdfName.c_str(), (isPbPb?"PbPb":"PP")), 
                             RooArgList( 
                                        *ws.pdf(Form("%sTot_Jpsi%s_%s", pdfName.c_str(), tag.c_str(), (isPbPb?"PbPb":"PP"))), 
                                        *ws.pdf(Form("%sTot_Psi2S%s_%s", pdfName.c_str(), tag.c_str(), (isPbPb?"PbPb":"PP")))
                                         )
                             );
  }
  if (incJpsi && !incPsi2S && incBkg) {
    themodel = new RooAddPdf(Form("%s_Tot_%s", pdfName.c_str(), (isPbPb?"PbPb":"PP")), Form("%s_Tot_%s", pdfName.c_str(), (isPbPb?"PbPb":"PP")), 
                             RooArgList( 
                                        *ws.pdf(Form("%sTot_Jpsi%s_%s", pdfName.c_str(), tag.c_str(), (isPbPb?"PbPb":"PP"))), 
                                        *ws.pdf(Form("%sTot_Bkg%s_%s", pdfName.c_str(), tag.c_str(), (isPbPb?"PbPb":"PP")))
                                         )
                             );
  }
  if (!incJpsi && incPsi2S && incBkg) {
    themodel = new RooAddPdf(Form("%s_Tot_%s", pdfName.c_str(), (isPbPb?"PbPb":"PP")), Form("%s_Tot_%s", pdfName.c_str(), (isPbPb?"PbPb":"PP")), 
                             RooArgList( 
                                        *ws.pdf(Form("%sTot_Psi2S%s_%s", pdfName.c_str(), tag.c_str(), (isPbPb?"PbPb":"PP"))), 
                                        *ws.pdf(Form("%sTot_Bkg%s_%s", pdfName.c_str(), tag.c_str(), (isPbPb?"PbPb":"PP")))
                                         )
                             );
  }
  if (incJpsi && !incPsi2S && !incBkg) {
    themodel = new RooAddPdf(Form("%s_Tot_%s", pdfName.c_str(), (isPbPb?"PbPb":"PP")), Form("%s_Tot_%s", pdfName.c_str(), (isPbPb?"PbPb":"PP")), 
                             RooArgList( 
                                        *ws.pdf(Form("%sTot_Jpsi%s_%s", pdfName.c_str(), tag.c_str(), (isPbPb?"PbPb":"PP")))
                                         )
                             );
  }
  if (!incJpsi && incPsi2S && !incBkg) {
    themodel = new RooAddPdf(Form("%s_Tot_%s", pdfName.c_str(), (isPbPb?"PbPb":"PP")), Form("%s_Tot_%s", pdfName.c_str(), (isPbPb?"PbPb":"PP")), 
                             RooArgList( 
                                        *ws.pdf(Form("%sTot_Psi2S%s_%s", pdfName.c_str(), tag.c_str(), (isPbPb?"PbPb":"PP")))
                                         )
                             );
  }
  if (!incJpsi && !incPsi2S && incBkg) {
    themodel = new RooAddPdf(Form("%s_Tot_%s", pdfName.c_str(), (isPbPb?"PbPb":"PP")), Form("%s_Tot_%s", pdfName.c_str(), (isPbPb?"PbPb":"PP")), 
                             RooArgList( 
                                        *ws.pdf(Form("%sTot_Bkg%s_%s", pdfName.c_str(), tag.c_str(), (isPbPb?"PbPb":"PP")))
                                         )
                             );
  }
  if (!incJpsi && !incPsi2S && !incBkg) {
    cout << "[ERROR] User did not include any model, please fix your input settings!" << endl; return false;
  }
  ws.import(*themodel);
  //ws.pdf(Form("%s_Tot_%s", pdfName.c_str(), (isPbPb?"PbPb":"PP")))->setNormRange("CtauWindow");

  // save the initial values of the model we've just created
  RooArgSet* params = (RooArgSet*) themodel->getParameters(RooArgSet(*ws.var("ctau"), *ws.var("invMass"), *ws.var("ctauErr"))) ;
  pdfName+=Form("_Tot_%s_parIni", (isPbPb?"PbPb":"PP"));
  ws.saveSnapshot(pdfName.c_str(),*params,kTRUE) ;
  
  //ws.Print();
  return true;
};

void fixCtauParPsi2StoJpsi(map<string, string>& parIni, bool isPbPb)
{
  cout << "[INFO] Constraining Psi(2S) parameters to Jpsi" << endl;
  parIni[Form("sigmaMC_Psi2SNoPR_%s", (isPbPb?"PbPb":"PP"))] = Form("RooFormulaVar::%s('@0',{%s})", Form("sigmaMC_Psi2SNoPR_%s", (isPbPb?"PbPb":"PP")), Form("sigmaMC_JpsiNoPR_%s", (isPbPb?"PbPb":"PP") ));
  parIni[Form("lambdaDSS_Psi2SNoPR_%s", (isPbPb?"PbPb":"PP"))] = Form("RooFormulaVar::%s('@0',{%s})", Form("lambdaDSS_Psi2SNoPR_%s", (isPbPb?"PbPb":"PP")), Form("lambdaDSS_JpsiNoPR_%s", (isPbPb?"PbPb":"PP") ));
  parIni[Form("lambdaDSS_Psi2SPR_%s", (isPbPb?"PbPb":"PP"))] = Form("RooFormulaVar::%s('@0',{%s})", Form("lambdaDSS_Psi2SPR_%s", (isPbPb?"PbPb":"PP")), Form("lambdaDSS_JpsiPR_%s", (isPbPb?"PbPb":"PP") ));
};


bool defineCtauResolModel(RooWorkspace& ws, string object, CtauModel model, map<string,string> parIni, bool isPbPb, bool usePromptCtauRes) 
{ 

  if (ws.pdf(Form("pdfCTAURES_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")))) { 
    cout << Form("[ERROR] The %s Ctau Resolution Model has already been implemented!", object.c_str()) << endl;
    return false; 
  }

  cout << Form("[INFO] Implementing %s Ctau Resolution Model", object.c_str()) << endl;
  
  switch(model) 
    {  
    case (CtauModel::SingleGaussianResolution):  
      if (!( 
            parIni.count(Form("ctau1_CtauRes_%s", (isPbPb?"PbPb":"PP"))) && 
            parIni.count(Form("sigma1_CtauRes_%s", (isPbPb?"PbPb":"PP"))) 
             )) { 
 	cout << Form("[ERROR] Initial parameters where not found for Single Gaussian Ctau Resolution Model in %s", (isPbPb?"PbPb":"PP")) << endl; return false; 
      }

      // create the variables for this model  
      if (!ws.var("One")) { ws.factory("One[1.0]"); }
      if (!ws.var(Form("ctau1_CtauRes_%s", (isPbPb?"PbPb":"PP"))))  { ws.factory( parIni[Form("ctau1_CtauRes_%s", (isPbPb?"PbPb":"PP"))].c_str() );  }
      if (!ws.var(Form("sigma1_CtauRes_%s", (isPbPb?"PbPb":"PP")))) { ws.factory( parIni[Form("sigma1_CtauRes_%s", (isPbPb?"PbPb":"PP"))].c_str() ); }
      if (!usePromptCtauRes) {
        if (!ws.var(Form("sigmaMC_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")))) { ws.factory( parIni[Form("sigmaMC_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str() ); }
        if (!ws.var(Form("sigma1_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")))) { 
          ws.factory( Form("RooFormulaVar::%s('sqrt((@0*@1)**2+(@2)**2)',{%s,%s,%s})", Form("sigma1_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")), 
                           Form("sigma1_CtauRes_%s", (isPbPb?"PbPb":"PP")),
                           "ctauErr",
                           Form("sigmaMC_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))
                           )
                      );
        }
      }

      // create the two PDFs
      ws.factory(Form("GaussModel::%s(%s, %s, %s, One, %s)", Form("pdfCTAURES_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")), "ctau", 
 		      Form("ctau1_CtauRes_%s", (isPbPb?"PbPb":"PP")), 
 		      Form("sigma1_%s_%s", (usePromptCtauRes?"CtauRes":object.c_str()), (isPbPb?"PbPb":"PP")),
                      (usePromptCtauRes ? "ctauErr" : "One")
 		      ));

      cout << Form("[INFO] %s Single Gaussian Ctau Resolution PDF in %s included", object.c_str(), (isPbPb?"PbPb":"PP")) << endl; break;
       
    case (CtauModel::DoubleGaussianResolution):  

      if (!( 
            parIni.count(Form("ctau1_CtauRes_%s", (isPbPb?"PbPb":"PP"))) && 
            parIni.count(Form("sigma1_CtauRes_%s", (isPbPb?"PbPb":"PP"))) && 
            parIni.count(Form("ctau2_CtauRes_%s", (isPbPb?"PbPb":"PP"))) && 
            parIni.count(Form("sigma2_CtauRes_%s", (isPbPb?"PbPb":"PP"))) && 
            parIni.count(Form("f_CtauRes_%s", (isPbPb?"PbPb":"PP"))) 
             )) { 
 	cout << Form("[ERROR] Initial parameters where not found for Double Gaussian Ctau Resolution Model in %s", (isPbPb?"PbPb":"PP")) << endl; return false; 
      }
      
      // create the variables for this model  
      if (!ws.var("One")) { ws.factory("One[1.0]"); }
      if (!ws.var(Form("ctau1_CtauRes_%s", (isPbPb?"PbPb":"PP"))))   { ws.factory( parIni[Form("ctau1_CtauRes_%s", (isPbPb?"PbPb":"PP"))].c_str()  ); }
      if (!ws.var(Form("sigma1_CtauRes_%s", (isPbPb?"PbPb":"PP"))))  { ws.factory( parIni[Form("sigma1_CtauRes_%s", (isPbPb?"PbPb":"PP"))].c_str() ); }
      if (!ws.var(Form("ctau2_CtauRes_%s", (isPbPb?"PbPb":"PP"))))   { ws.factory( parIni[Form("ctau2_CtauRes_%s", (isPbPb?"PbPb":"PP"))].c_str()  ); }
      if (!ws.var(Form("sigma2_CtauRes_%s", (isPbPb?"PbPb":"PP"))))  { ws.factory( parIni[Form("sigma2_CtauRes_%s", (isPbPb?"PbPb":"PP"))].c_str() ); }
      
      if (!usePromptCtauRes) {
        if (!ws.var(Form("sigmaMC_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")))) { ws.factory( parIni[Form("sigmaMC_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str() ); }
        if (!ws.var(Form("sigma1_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")))) { 
          ws.factory( Form("RooFormulaVar::%s('sqrt((@0*@1)**2+(@2)**2)',{%s,%s,%s})", Form("sigma1_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")), 
                           Form("sigma1_CtauRes_%s", (isPbPb?"PbPb":"PP")),
                           "ctauErr",
                           Form("sigmaMC_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))
                           )
                      );
        }
        if (!ws.var(Form("sigma2_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")))) { 
          ws.factory( Form("RooFormulaVar::%s('sqrt((@0*@1)**2+(@2)**2)',{%s,%s,%s})", Form("sigma2_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")), 
                           Form("sigma2_CtauRes_%s", (isPbPb?"PbPb":"PP")),
                           "ctauErr",
                           Form("sigmaMC_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))
                           )
                      );
        }
      }

      // create the two PDFs
      ws.factory(Form("GaussModel::%s(%s, %s, %s, One, %s)", Form("pdfCTAURES1_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")), "ctau", 
 		      Form("ctau1_CtauRes_%s", (isPbPb?"PbPb":"PP")),
 		      Form("sigma1_%s_%s", (usePromptCtauRes?"CtauRes":object.c_str()), (isPbPb?"PbPb":"PP")),
                      (usePromptCtauRes ? "ctauErr" : "One")
 		      ));
      ws.factory(Form("GaussModel::%s(%s, %s, %s, One, %s)", Form("pdfCTAURES2_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")), "ctau", 
 		      Form("ctau2_CtauRes_%s", (isPbPb?"PbPb":"PP")), 
 		      Form("sigma2_%s_%s", (usePromptCtauRes?"CtauRes":object.c_str()), (isPbPb?"PbPb":"PP")),
                      (usePromptCtauRes ? "ctauErr" : "One")
 		      ));

      // combine the two PDFs
      ws.factory(Form("AddModel::%s({%s, %s}, {%s})", Form("pdfCTAURES_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")), 
 		      Form("pdfCTAURES1_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")), 
 		      Form("pdfCTAURES2_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")),  
 		      parIni[Form("f_CtauRes_%s", (isPbPb?"PbPb":"PP"))].c_str()
 		      ));
      cout << Form("[INFO] %s Double Gaussian Ctau Resolution PDF in %s included", object.c_str(), (isPbPb?"PbPb":"PP")) << endl; break;
 
    default :

      cout<< "[ERROR] Selected Ctau Resolution Model has not been implemented"<< endl; return false;

    }
 
  return true;

};

bool addBackgroundCtauModel(RooWorkspace& ws, string object, CtauModel model, map<string,string> parIni, bool isPbPb) 
{
  if (ws.pdf(Form("pdfCTAUTot_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")))) { 
    cout << Form("[ERROR] The %s Background Ctau Model has already been implemented!", object.c_str()) << endl;
    return false; 
  }

  cout << Form("[INFO] Implementing %s Background Ctau Model", object.c_str()) << endl;
   
  string objectInc = object; 
  if (objectInc.find("NoPR")!=std::string::npos) { objectInc.erase(objectInc.find("NoPR"), objectInc.length()); }
  if (objectInc.find("PR")!=std::string::npos)   { objectInc.erase(objectInc.find("PR"), objectInc.length());   }

  switch(model) 
    {  
    case (CtauModel::TripleDecay): 
      if (!( 
            parIni.count(Form("lambdaDSS_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))) && 
            parIni.count(Form("lambdaDF_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))) && 
            parIni.count(Form("lambdaDDS_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))) && 
            parIni.count(Form("fDFSS_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))) && 
            parIni.count(Form("fDLIV_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))) 
             )) { 
 	cout << Form("[ERROR] Initial parameters where not found for %s Background Triple Decay Ctau Model in %s", object.c_str(), (isPbPb?"PbPb":"PP")) << endl; return false; 
      }
      
      // create the variables for this model 
      if (!ws.var(Form("N_%s_%s", objectInc.c_str(), (isPbPb?"PbPb":"PP")))){ ws.factory( parIni[Form("N_%s_%s", objectInc.c_str(), (isPbPb?"PbPb":"PP"))].c_str() ); } 
      ws.factory( parIni[Form("lambdaDSS_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str() ); 
      ws.factory( parIni[Form("lambdaDF_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str()  );
      ws.factory( parIni[Form("lambdaDDS_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str() ); 
      ws.factory( parIni[Form("fDFSS_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str()     ); 
      ws.factory( parIni[Form("fDLIV_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str()     );

      // create the three PDFs
      ws.factory(Form("Decay::%s(%s, %s, %s, RooDecay::SingleSided)", Form("pdfCTAUDSS_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")), "ctau", 
 		      Form("lambdaDSS_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")),
 		      Form("pdfCTAURES_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))
 		      ));
      ws.factory(Form("Decay::%s(%s, %s, %s, RooDecay::Flipped)", Form("pdfCTAUDF_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")), "ctau", 
 		      Form("lambdaDF_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")),
 		      Form("pdfCTAURES_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))
 		      ));
      ws.factory(Form("Decay::%s(%s, %s, %s, RooDecay::DoubleSided)", Form("pdfCTAUDDS_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")), "ctau", 
 		      Form("lambdaDDS_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")),
 		      Form("pdfCTAURES_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))
 		      ));

      // combine the three PDFs
      ws.factory(Form("SUM::%s(%s*%s, %s)", Form("pdfCTAU1_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")), 
 		      Form("fDFSS_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")),
 		      Form("pdfCTAUDSS_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")),
 		      Form("pdfCTAUDF_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))
 		      ));

      ws.factory(Form("SUM::%s(%s*%s, %s)", Form("pdfCTAU_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")), 
 		      Form("fDLIV_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")),
 		      Form("pdfCTAU1_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")),
 		      Form("pdfCTAUDDS_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))
 		      ));
     
      if ( ws.pdf(Form("pdfMASSTot_%s_%s", objectInc.c_str(), (isPbPb?"PbPb":"PP"))) ){
        ws.factory(Form("PROD::%s(%s, %s)", Form("pdfCTAUMASSTot_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")),
                        Form("pdfCTAU_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")),
                        Form("pdfMASSTot_%s_%s", objectInc.c_str(),(isPbPb?"PbPb":"PP"))
                        ));
      } else {
        ws.factory(Form("RooExtendPdf::%s(%s,%s)", Form("pdfCTAUTot_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")),
                        Form("pdfCTAU_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")),
                        Form("N_%s_%s", objectInc.c_str(), (isPbPb?"PbPb":"PP"))
                        ));
      }
 
      cout << Form("[INFO] %s Background Triple Decay Ctau PDF in %s included", object.c_str(), (isPbPb?"PbPb":"PP")) << endl; break; 
 
    case (CtauModel::Delta): 
      
      if (!ws.var(Form("N_%s_%s", objectInc.c_str(), (isPbPb?"PbPb":"PP")))){ ws.factory( parIni[Form("N_%s_%s", objectInc.c_str(), (isPbPb?"PbPb":"PP"))].c_str() ); } 

      // create the PDF
      ws.factory(Form("SUM::%s(%s)", Form("pdfCTAU_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")), 
 		      Form("pdfCTAURES_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))
 		      ));
     
      if ( ws.pdf(Form("pdfMASSTot_%s_%s", objectInc.c_str(), (isPbPb?"PbPb":"PP"))) ){
        ws.factory(Form("PROD::%s(%s, %s)", Form("pdfCTAUMASSTot_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")),
                        Form("pdfCTAU_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")),
                        Form("pdfMASSTot_%s_%s", objectInc.c_str(),(isPbPb?"PbPb":"PP"))
                        ));
      } else {
        ws.factory(Form("RooExtendPdf::%s(%s,%s)", Form("pdfCTAUTot_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")),
                        Form("pdfCTAU_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")),
                        Form("N_%s_%s", objectInc.c_str(), (isPbPb?"PbPb":"PP"))
                        ));
      }

      cout << Form("[INFO] %s Background Delta Ctau PDF in %s included", object.c_str(), (isPbPb?"PbPb":"PP")) << endl; break; 
      
    default :

      cout<< "[ERROR] Selected Background Ctau Model has not been implemented"<< endl; return false;

    }
   
  return true;
};


bool addSignalCtauModel(RooWorkspace& ws, string object, CtauModel model, map<string,string> parIni, bool isPbPb) 
{
  if (ws.pdf(Form("pdfCTAUTot_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")))) { 
    cout << Form("[ERROR] The %s Signal Ctau Model has already been implemented!", object.c_str()) << endl;
    return false; 
  }

  cout << Form("[INFO] Implementing %s Signal Ctau Model", object.c_str()) << endl;
   
  string objectInc = object; 
  if (objectInc.find("NoPR")!=std::string::npos) { objectInc.erase(objectInc.find("NoPR"), objectInc.length()); }
  if (objectInc.find("PR")!=std::string::npos)   { objectInc.erase(objectInc.find("PR"), objectInc.length());   } 

  switch(model) 
    {  
    case (CtauModel::SingleSidedDecay): 

      if (!( 
            parIni.count(Form("lambdaDSS_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))) 
             )) { 
 	cout << Form("[ERROR] Initial parameters where not found for %s Signal Single Sided Decay Ctau Model in %s", object.c_str(), (isPbPb?"PbPb":"PP")) << endl; return false; 
      }
      
      // create the variables for this model 
      if (!ws.var(Form("N_%s_%s", objectInc.c_str(), (isPbPb?"PbPb":"PP")))){ ws.factory( parIni[Form("N_%s_%s", objectInc.c_str(), (isPbPb?"PbPb":"PP"))].c_str() ); } 
      ws.factory( parIni[Form("lambdaDSS_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str() ); 
            
      // create the PDF
      ws.factory(Form("Decay::%s(%s, %s, %s, RooDecay::SingleSided)", Form("pdfCTAU_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")), "ctau", 
 		      Form("lambdaDSS_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")),
 		      Form("pdfCTAURES_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))
 		      ));
      
      if ( ws.pdf(Form("pdfMASSTot_%s_%s", objectInc.c_str(), (isPbPb?"PbPb":"PP"))) ){
        ws.factory(Form("PROD::%s(%s, %s)", Form("pdfCTAUMASSTot_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")),
                        Form("pdfCTAU_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")),
                        Form("pdfMASSTot_%s_%s", objectInc.c_str(),(isPbPb?"PbPb":"PP"))
                        ));
      } else {
        ws.factory(Form("RooExtendPdf::%s(%s,%s)", Form("pdfCTAUTot_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")),
                        Form("pdfCTAU_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")),
                        Form("N_%s_%s", objectInc.c_str(), (isPbPb?"PbPb":"PP"))
                        ));
      }
 
      cout << Form("[INFO] %s Signal Single Sided Decay Ctau PDF in %s included", object.c_str(), (isPbPb?"PbPb":"PP")) << endl; break; 
 
    case (CtauModel::Delta):

      if (!ws.var(Form("N_%s_%s", objectInc.c_str(), (isPbPb?"PbPb":"PP")))){ ws.factory( parIni[Form("N_%s_%s", objectInc.c_str(), (isPbPb?"PbPb":"PP"))].c_str() ); } 

      // create the three PDFs 
      ws.factory(Form("SUM::%s(%s)", Form("pdfCTAU_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")),
 		      Form("pdfCTAURES_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))
 		      ));
     
      if ( ws.pdf(Form("pdfMASSTot_%s_%s", objectInc.c_str(), (isPbPb?"PbPb":"PP"))) ){
        ws.factory(Form("PROD::%s(%s, %s)", Form("pdfCTAUMASSTot_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")),
                        Form("pdfCTAU_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")),
                        Form("pdfMASSTot_%s_%s", objectInc.c_str(),(isPbPb?"PbPb":"PP"))
                        ));
      } else {
        ws.factory(Form("RooExtendPdf::%s(%s,%s)", Form("pdfCTAUTot_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")),
                        Form("pdfCTAU_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")),
                        Form("N_%s_%s", objectInc.c_str(), (isPbPb?"PbPb":"PP"))
                        ));
      }
 		      
      cout << Form("[INFO] %s Signal Delta Ctau PDF in %s included", object.c_str(), (isPbPb?"PbPb":"PP")) << endl; break; 
 
    default :

      cout<< "[ERROR] Selected Signal Ctau Model has not been implemented"<< endl; return false;

    }
   
  return true;
};

void setCtauDefaultParameters(map<string, string> &parIni, bool isPbPb, double numEntries)
{

  cout << "[INFO] Setting user undefined initial parameters to their default values" << endl;

  // DEFAULT RANGE OF NUMBER OF EVENTS
  if (parIni.count(Form("N_Jpsi_%s", (isPbPb?"PbPb":"PP")))==0 || parIni[Form("N_Jpsi_%s", (isPbPb?"PbPb":"PP"))]=="") { 
    parIni[Form("N_Jpsi_%s", (isPbPb?"PbPb":"PP"))]  = Form("%s[%.12f,%.12f,%.12f]", Form("N_Jpsi_%s", (isPbPb?"PbPb":"PP")), numEntries, 0.0, numEntries*2.0);
  }
  if (parIni.count(Form("N_Psi2S_%s", (isPbPb?"PbPb":"PP")))==0 || parIni[Form("N_Psi2S_%s", (isPbPb?"PbPb":"PP"))]=="") { 
    parIni[Form("N_Psi2S_%s", (isPbPb?"PbPb":"PP"))]  = Form("%s[%.12f,%.12f,%.12f]", Form("N_Psi2S_%s", (isPbPb?"PbPb":"PP")), numEntries, 0.0, numEntries*2.0);
  }
  if (parIni.count(Form("N_Bkg_%s", (isPbPb?"PbPb":"PP")))==0 || parIni[Form("N_Bkg_%s", (isPbPb?"PbPb":"PP"))]=="") { 
    parIni[Form("N_Bkg_%s", (isPbPb?"PbPb":"PP"))]  = Form("%s[%.12f,%.12f,%.12f]", Form("N_Bkg_%s", (isPbPb?"PbPb":"PP")), numEntries, 0.0, numEntries*2.0);
  }

 // CTAU FIT PARAMETERS

 // Resolution Ctau Model
  if (parIni.count(Form("ctau1_CtauRes_%s", (isPbPb?"PbPb":"PP")))==0 || parIni[Form("ctau1_CtauRes_%s", (isPbPb?"PbPb":"PP"))]=="") { 
    parIni[Form("ctau1_CtauRes_%s", (isPbPb?"PbPb":"PP"))] = Form("%s[%.4f,%.4f,%.4f]", Form("ctau1_CtauRes_%s", (isPbPb?"PbPb":"PP")), 0.0, -1.0, 1.0);
  }
  if (parIni.count(Form("ctau2_CtauRes_%s", (isPbPb?"PbPb":"PP")))==0) {
    parIni[Form("ctau2_CtauRes_%s", (isPbPb?"PbPb":"PP"))] = Form("RooFormulaVar::%s('@0',{%s})", Form("ctau2_CtauRes_%s", (isPbPb?"PbPb":"PP")), Form("ctau1_CtauRes_%s", (isPbPb?"PbPb":"PP") ));
  } else if ( parIni[Form("ctau2_CtauRes_%s", (isPbPb?"PbPb":"PP"))]=="" ) { 
    parIni[Form("ctau2_CtauRes_%s", (isPbPb?"PbPb":"PP"))] = Form("%s[%.4f,%.4f,%.4f]", Form("ctau2_CtauRes_%s", (isPbPb?"PbPb":"PP")), 0.0, -1.0, 1.0);
  }
  if (parIni.count(Form("sigma1_CtauRes_%s", (isPbPb?"PbPb":"PP")))==0 || parIni[Form("sigma1_CtauRes_%s", (isPbPb?"PbPb":"PP"))]=="") { 
    parIni[Form("sigma1_CtauRes_%s", (isPbPb?"PbPb":"PP"))] = Form("%s[%.4f,%.4f,%.4f]", Form("sigma1_CtauRes_%s", (isPbPb?"PbPb":"PP")), 0.8, 0.001, 2.0);
  }
  if (parIni.count(Form("sigma2_CtauRes_%s", (isPbPb?"PbPb":"PP")))==0 || parIni[Form("sigma2_CtauRes_%s", (isPbPb?"PbPb":"PP"))]=="") { 
    parIni[Form("sigma2_CtauRes_%s", (isPbPb?"PbPb":"PP"))] = Form("%s[%.4f,%.4f,%.4f]", Form("sigma2_CtauRes_%s", (isPbPb?"PbPb":"PP")), 1.2, 0.001, 60.0);
  }
  if (parIni.count(Form("sigmaMC_JpsiNoPR_%s", (isPbPb?"PbPb":"PP")))==0 || parIni[Form("sigmaMC_JpsiNoPR_%s", (isPbPb?"PbPb":"PP"))]=="") { 
    parIni[Form("sigmaMC_JpsiNoPR_%s", (isPbPb?"PbPb":"PP"))] = Form("%s[%.4f,%.4f,%.4f]", Form("sigmaMC_JpsiNoPR_%s", (isPbPb?"PbPb":"PP")), 0.00001, 0.0000001, 1.0);
  }
  if (parIni.count(Form("sigmaMC_Psi2SNoPR_%s", (isPbPb?"PbPb":"PP")))==0 || parIni[Form("sigmaMC_Psi2SNoPR_%s", (isPbPb?"PbPb":"PP"))]=="") {
    parIni[Form("sigmaMC_Psi2SNoPR_%s", (isPbPb?"PbPb":"PP"))] = Form("%s[%.4f,%.4f,%.4f]", Form("sigmaMC_Psi2SNoPR_%s", (isPbPb?"PbPb":"PP")), 0.00001, 0.0000001, 1.0);
  }
  if (parIni.count(Form("f_CtauRes_%s", (isPbPb?"PbPb":"PP")))==0 || parIni[Form("f_CtauRes_%s", (isPbPb?"PbPb":"PP"))]=="") { 
    parIni[Form("f_CtauRes_%s", (isPbPb?"PbPb":"PP"))] = Form("%s[%.4f,%.4f,%.4f]", Form("f_CtauRes_%s", (isPbPb?"PbPb":"PP")), 0.8, 0.0, 1.0);
  }

  // Signal Ctau Model
  if (parIni.count(Form("lambdaDSS_JpsiNoPR_%s", (isPbPb?"PbPb":"PP")))==0 || parIni[Form("lambdaDSS_JpsiNoPR_%s", (isPbPb?"PbPb":"PP"))]=="") { 
    parIni[Form("lambdaDSS_JpsiNoPR_%s", (isPbPb?"PbPb":"PP"))] = Form("%s[%.4f,%.4f,%.4f]", Form("lambdaDSS_JpsiNoPR_%s", (isPbPb?"PbPb":"PP")), 0.8, 0.01, 2.0);
  }
  if (parIni.count(Form("lambdaDSS_JpsiNoPR_%s", (isPbPb?"PbPb":"PP")))==0 || parIni[Form("lambdaDSS_JpsiNoPR_%s", (isPbPb?"PbPb":"PP"))]=="") { 
    parIni[Form("lambdaDSS_Psi2SNoPR_%s", (isPbPb?"PbPb":"PP"))] = Form("%s[%.4f,%.4f,%.4f]", Form("lambdaDSS_Psi2SNoPR_%s", (isPbPb?"PbPb":"PP")), 0.8, 0.01, 2.0);
  }
  if (parIni.count(Form("lambdaDSS_JpsiPR_%s", (isPbPb?"PbPb":"PP")))==0 || parIni[Form("lambdaDSS_JpsiPR_%s", (isPbPb?"PbPb":"PP"))]=="") { 
    parIni[Form("lambdaDSS_JpsiPR_%s", (isPbPb?"PbPb":"PP"))] = Form("%s[%.4f,%.4f,%.4f]", Form("lambdaDSS_JpsiPR_%s", (isPbPb?"PbPb":"PP")), 0.8, 0.01, 2.0);
  }
  if (parIni.count(Form("lambdaDSS_JpsiPR_%s", (isPbPb?"PbPb":"PP")))==0 || parIni[Form("lambdaDSS_JpsiPR_%s", (isPbPb?"PbPb":"PP"))]=="") { 
    parIni[Form("lambdaDSS_Psi2SPR_%s", (isPbPb?"PbPb":"PP"))] = Form("%s[%.4f,%.4f,%.4f]", Form("lambdaDSS_Psi2SPR_%s", (isPbPb?"PbPb":"PP")), 0.8, 0.01, 2.0);
  }

  // Background Ctau Model
  if (parIni.count(Form("f_BkgNoPR_%s", (isPbPb?"PbPb":"PP")))==0 || parIni[Form("f_BkgNoPR_%s", (isPbPb?"PbPb":"PP"))]=="") { 
    parIni[Form("f_BkgNoPR_%s", (isPbPb?"PbPb":"PP"))] = Form("%s[%.4f,%.4f,%.4f]", Form("f_BkgNoPR_%s", (isPbPb?"PbPb":"PP")), 0.3, 0., 1.);
  }
  if (parIni.count(Form("fDFSS_BkgNoPR_%s", (isPbPb?"PbPb":"PP")))==0 || parIni[Form("fDFSS_BkgNoPR_%s", (isPbPb?"PbPb":"PP"))]=="") { 
    parIni[Form("fDFSS_BkgNoPR_%s", (isPbPb?"PbPb":"PP"))] = Form("%s[%.4f,%.4f,%.4f]", Form("fDFSS_BkgNoPR_%s", (isPbPb?"PbPb":"PP")), 0.9, 0., 1.);
  }
  if (parIni.count(Form("fDLIV_BkgNoPR_%s", (isPbPb?"PbPb":"PP")))==0 || parIni[Form("fDLIV_BkgNoPR_%s", (isPbPb?"PbPb":"PP"))]=="") { 
    parIni[Form("fDLIV_BkgNoPR_%s", (isPbPb?"PbPb":"PP"))] = Form("%s[%.4f,%.4f,%.4f]", Form("fDLIV_BkgNoPR_%s", (isPbPb?"PbPb":"PP")), 0.9, 0., 1.);
  }
  if (parIni.count(Form("lambdaDSS_BkgNoPR_%s", (isPbPb?"PbPb":"PP")))==0 || parIni[Form("lambdaDSS_BkgNoPR_%s", (isPbPb?"PbPb":"PP"))]=="") { 
    parIni[Form("lambdaDSS_BkgNoPR_%s", (isPbPb?"PbPb":"PP"))] = Form("%s[%.4f,%.4f,%.4f]", Form("lambdaDSS_BkgNoPR_%s", (isPbPb?"PbPb":"PP")), 0.42, 0.05, 1.5);
  }
  if (parIni.count(Form("lambdaDF_BkgNoPR_%s", (isPbPb?"PbPb":"PP")))==0 || parIni[Form("lambdaDF_BkgNoPR_%s", (isPbPb?"PbPb":"PP"))]=="") { 
    parIni[Form("lambdaDF_BkgNoPR_%s", (isPbPb?"PbPb":"PP"))] = Form("%s[%.4f,%.4f,%.4f]", Form("lambdaDF_BkgNoPR_%s", (isPbPb?"PbPb":"PP")), 0.79, 0.001, 1.5);
  }
  if (parIni.count(Form("lambdaDDS_BkgNoPR_%s", (isPbPb?"PbPb":"PP")))==0 || parIni[Form("lambdaDDS_BkgNoPR_%s", (isPbPb?"PbPb":"PP"))]=="") { 
    parIni[Form("lambdaDDS_BkgNoPR_%s", (isPbPb?"PbPb":"PP"))] = Form("%s[%.4f,%.4f,%.4f]", Form("lambdaDDS_BkgNoPR_%s", (isPbPb?"PbPb":"PP")), 0.69, 0.02, 5.0);
  }
  if (parIni.count(Form("f_BkgPR_%s", (isPbPb?"PbPb":"PP")))==0 || parIni[Form("f_BkgPR_%s", (isPbPb?"PbPb":"PP"))]=="") { 
    parIni[Form("f_BkgPR_%s", (isPbPb?"PbPb":"PP"))] = Form("%s[%.4f,%.4f,%.4f]", Form("f_BkgPR_%s", (isPbPb?"PbPb":"PP")), 0.3, 0., 1.);
  }
  if (parIni.count(Form("fDFSS_BkgPR_%s", (isPbPb?"PbPb":"PP")))==0 || parIni[Form("fDFSS_BkgPR_%s", (isPbPb?"PbPb":"PP"))]=="") { 
    parIni[Form("fDFSS_BkgPR_%s", (isPbPb?"PbPb":"PP"))] = Form("%s[%.4f,%.4f,%.4f]", Form("fDFSS_BkgPR_%s", (isPbPb?"PbPb":"PP")), 0.9, 0., 1.);
  }
  if (parIni.count(Form("fDLIV_BkgPR_%s", (isPbPb?"PbPb":"PP")))==0 || parIni[Form("fDLIV_BkgPR_%s", (isPbPb?"PbPb":"PP"))]=="") { 
    parIni[Form("fDLIV_BkgPR_%s", (isPbPb?"PbPb":"PP"))] = Form("%s[%.4f,%.4f,%.4f]", Form("fDLIV_BkgPR_%s", (isPbPb?"PbPb":"PP")), 0.9, 0., 1.);
  }
  if (parIni.count(Form("lambdaDSS_BkgPR_%s", (isPbPb?"PbPb":"PP")))==0 || parIni[Form("lambdaDSS_BkgPR_%s", (isPbPb?"PbPb":"PP"))]=="") { 
    parIni[Form("lambdaDSS_BkgPR_%s", (isPbPb?"PbPb":"PP"))] = Form("%s[%.4f,%.4f,%.4f]", Form("lambdaDSS_BkgPR_%s", (isPbPb?"PbPb":"PP")), 0.42, 0.05, 1.5);
  }
  if (parIni.count(Form("lambdaDF_BkgPR_%s", (isPbPb?"PbPb":"PP")))==0 || parIni[Form("lambdaDF_BkgPR_%s", (isPbPb?"PbPb":"PP"))]=="") { 
    parIni[Form("lambdaDF_BkgPR_%s", (isPbPb?"PbPb":"PP"))] = Form("%s[%.4f,%.4f,%.4f]", Form("lambdaDF_BkgPR_%s", (isPbPb?"PbPb":"PP")), 0.79, 0.001, 1.5);
  }
  if (parIni.count(Form("lambdaDDS_BkgPR_%s", (isPbPb?"PbPb":"PP")))==0 || parIni[Form("lambdaDDS_BkgPR_%s", (isPbPb?"PbPb":"PP"))]=="") { 
    parIni[Form("lambdaDDS_BkgPR_%s", (isPbPb?"PbPb":"PP"))] = Form("%s[%.4f,%.4f,%.4f]", Form("lambdaDDS_BkgPR_%s", (isPbPb?"PbPb":"PP")), 0.69, 0.02, 5.0);
  }

};
