#include "Utilities/initClasses.h"

void setCtauTrueDefaultParameters(map<string, string> &parIni, bool isPbPb, double numEntries);
bool defineCtauTrueResolModel(RooWorkspace& ws, string object, CtauModel model, map<string,string> parIni, bool isPbPb); 
bool addSignalCtauTrueModel(RooWorkspace& ws, string object, CtauModel model, map<string,string> parIni, bool isPbPb); 


bool buildCharmoniaCtauTrueModel(RooWorkspace& ws, struct CharmModel model, map<string, string>  parIni, 
                                 bool isPbPb,                 // Determine if we are working with PbPb (True) or PP (False)
                                 bool incJpsi,                // Include Jpsi model
                                 bool incPsi2S,                // Include Psi(2S) model
                                 double  numEntries = 300000. // Number of entries in the dataset
                                 )
{

  // If the initial parameters are empty, set defaul parameter values
  setCtauTrueDefaultParameters(parIni, isPbPb, numEntries);

  // C r e a t e   m o d e l 
     
  string pdfName     = "pdfCTAUTRUE";

  if (incJpsi) {
    if(!defineCtauTrueResolModel(ws, "JpsiNoPR", model.CtauTrueRes, parIni, isPbPb)) { cout << "[ERROR] Defining the Ctau Truth Resolution Model failed" << endl; return false; }
    if(!addSignalCtauTrueModel(ws, "JpsiNoPR", model.CtauTrue, parIni, isPbPb)) { cout << "[ERROR] Adding Non-Prompt Jpsi Ctau Truth Model failed" << endl; return false; }
  }
  if (incPsi2S) {
    if(!defineCtauTrueResolModel(ws, "Psi2SNoPR", model.CtauTrueRes, parIni, isPbPb)) { cout << "[ERROR] Defining the Ctau Truth Resolution Model failed" << endl; return false; }
    if(!addSignalCtauTrueModel(ws, "Psi2SNoPR", model.CtauTrue, parIni, isPbPb)) { cout << "[ERROR] Adding Non-Prompt Psi2S Ctau Truth Model failed" << endl; return false; }
  }

  // Total PDF
  RooAbsPdf *themodel = NULL;

  if (incJpsi && !incPsi2S) {
    themodel = new RooAddPdf(Form("%s_Tot_%s", pdfName.c_str(), (isPbPb?"PbPb":"PP")), Form("%s_Tot_%s", pdfName.c_str(), (isPbPb?"PbPb":"PP")), 
                             RooArgList( 
                                        *ws.pdf(Form("%sTot_JpsiNoPR_%s", pdfName.c_str(), (isPbPb?"PbPb":"PP")))
                                         )
                             );
  }
  if (!incJpsi && incPsi2S) {
    themodel = new RooAddPdf(Form("%s_Tot_%s", pdfName.c_str(), (isPbPb?"PbPb":"PP")), Form("%s_Tot_%s", pdfName.c_str(), (isPbPb?"PbPb":"PP")), 
                             RooArgList( 
                                        *ws.pdf(Form("%sTot_Psi2SNoPR_%s", pdfName.c_str(), (isPbPb?"PbPb":"PP")))
                                         )
                             );
  }
  if (incJpsi && incPsi2S) {
    cout << "[ERROR] User included Jpsi and Psi2S together, but can only fit them separately, please fix your input settings!" << endl; return false;
  }
  if (!incJpsi && !incPsi2S) {
    cout << "[ERROR] User did not include any model, please fix your input settings!" << endl; return false;
  }
  ws.import(*themodel);
  //ws.pdf(Form("%s_Tot_%s", pdfName.c_str(), (isPbPb?"PbPb":"PP")))->setNormRange("CtauWindow");
  
  // save the initial values of the model we've just created
  RooArgSet* params = (RooArgSet*) themodel->getParameters(RooArgSet(*ws.var("ctauTrue"))) ;
  pdfName+=Form("_Tot_%s_parIni", (isPbPb?"PbPb":"PP"));
  ws.saveSnapshot(pdfName.c_str(),*params,kTRUE) ;
  
  return true;
};


bool defineCtauTrueResolModel(RooWorkspace& ws, string object, CtauModel model, map<string,string> parIni, bool isPbPb) 
{ 

  if (ws.pdf(Form("pdfCTAUTRUERES_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")))) { 
    cout << Form("[ERROR] The %s Ctau Truth Resolution Model has already been implemented!", object.c_str()) << endl;
    return false; 
  }

  cout << Form("[INFO] Implementing %s Ctau Truth Resolution Model", object.c_str()) << endl;
  
  switch(model) 
    {  
    case (CtauModel::SingleGaussianResolution):
      if (!( 
            parIni.count(Form("ctauMC_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))) && 
            parIni.count(Form("sigmaMC_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))) 
             )) { 
 	cout << Form("[ERROR] Initial parameters where not found for Single Gaussian Ctau Truth Resolution Model in %s", (isPbPb?"PbPb":"PP")) << endl; return false; 
      }

      // create the variables for this model  
      if (!ws.var("One")) { ws.factory("One[1.0]"); }
      if (!ws.var(Form("ctauMC_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))))  { ws.factory( parIni[Form("ctauMC_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str() );  }
      if (!ws.var(Form("sigmaMC_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")))) { ws.factory( parIni[Form("sigmaMC_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str() ); }

      // create the two PDFs
      ws.factory(Form("GaussModel::%s(%s, %s, %s, One, One)", Form("pdfCTAUTRUERES_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")), "ctauTrue", 
 		      Form("ctauMC_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")), 
 		      Form("sigmaMC_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))
 		      ));
      
      cout << Form("[INFO] %s Single Gaussian Ctau Truth Resolution PDF in %s included", object.c_str(), (isPbPb?"PbPb":"PP")) << endl; break;
       
    default :

      cout<< "[ERROR] Selected Ctau Truth Resolution Model has not been implemented"<< endl; return false;

    }
 
  return true;

};


bool addSignalCtauTrueModel(RooWorkspace& ws, string object, CtauModel model, map<string,string> parIni, bool isPbPb) 
{
  if (ws.pdf(Form("pdfCTAUTRUE_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")))) { 
    cout << Form("[ERROR] The %s Signal Ctau Truth Model has already been implemented!", object.c_str()) << endl;
    return false; 
  }

  cout << Form("[INFO] Implementing %s Signal Ctau Truth Model", object.c_str()) << endl;
   
  string objectInc = object; 
  if (objectInc.find("NoPR")!=std::string::npos) { objectInc.erase(objectInc.find("NoPR"), objectInc.length()); }
  if (objectInc.find("PR")!=std::string::npos)   { objectInc.erase(objectInc.find("PR"), objectInc.length());   } 

  switch(model) 
    {  
    case (CtauModel::SingleSidedDecay): 

      if (!( 
            parIni.count(Form("lambdaDSS_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))) 
             )) { 
 	cout << Form("[ERROR] Initial parameters where not found for %s Signal Single Sided Decay Ctau Truth Model in %s", object.c_str(), (isPbPb?"PbPb":"PP")) << endl; return false; 
      }
      
      // create the variables for this model 
      if (!ws.var(Form("N_%s_%s", objectInc.c_str(), (isPbPb?"PbPb":"PP")))){ ws.factory( parIni[Form("N_%s_%s", objectInc.c_str(), (isPbPb?"PbPb":"PP"))].c_str() ); } 
      ws.factory( parIni[Form("lambdaDSS_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str() ); 
            
      // create the PDF
      ws.factory(Form("Decay::%s(%s, %s, %s, RooDecay::SingleSided)", Form("pdfCTAUTRUE_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")), "ctauTrue", 
 		      Form("lambdaDSS_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")),
 		      Form("pdfCTAUTRUERES_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))
 		      ));
      
      ws.factory(Form("RooExtendPdf::%s(%s,%s)", Form("pdfCTAUTRUETot_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")),
                      Form("pdfCTAUTRUE_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")),
                      Form("N_%s_%s", objectInc.c_str(), (isPbPb?"PbPb":"PP"))
                      ));
 
      cout << Form("[INFO] %s Signal Single Sided Decay Ctau Truth PDF in %s included", object.c_str(), (isPbPb?"PbPb":"PP")) << endl; break; 
 
    case (CtauModel::Delta):

      if (!ws.var(Form("N_%s_%s", objectInc.c_str(), (isPbPb?"PbPb":"PP")))){ ws.factory( parIni[Form("N_%s_%s", objectInc.c_str(), (isPbPb?"PbPb":"PP"))].c_str() ); } 

      // create the three PDFs 
      ws.factory(Form("SUM::%s(%s)", Form("pdfCTAUTRUE_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")),
 		      Form("pdfCTAUTRUERES_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))
 		      ));
     
      ws.factory(Form("RooExtendPdf::%s(%s,%s)", Form("pdfCTAUTRUETot_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")),
                      Form("pdfCTAUTRUE_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")),
                      Form("N_%s_%s", objectInc.c_str(), (isPbPb?"PbPb":"PP"))
                      ));
 		      
      cout << Form("[INFO] %s Signal Delta Ctau Truth PDF in %s included", object.c_str(), (isPbPb?"PbPb":"PP")) << endl; break; 
 
    default :

      cout<< "[ERROR] Selected Signal Ctau Truth Model has not been implemented"<< endl; return false;

    }
   
  return true;
};


void setCtauTrueDefaultParameters(map<string, string> &parIni, bool isPbPb, double numEntries)
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

  // CTAU TRUE FIT PARAMETERS
  

 // Resolution Ctau Truth Model
  if (parIni.count(Form("ctauMC_JpsiNoPR_%s", (isPbPb?"PbPb":"PP")))==0 || parIni[Form("ctauMC_JpsiNoPR_%s", (isPbPb?"PbPb":"PP"))]=="") {
    //parIni[Form("ctauMC_JpsiNoPR_%s", (isPbPb?"PbPb":"PP"))] = Form("%s[%.4f,%.4f,%.4f]", Form("ctauMC_JpsiNoPR_%s", (isPbPb?"PbPb":"PP")), 0.0, -100.0, 100.0);
    parIni[Form("ctauMC_JpsiNoPR_%s", (isPbPb?"PbPb":"PP"))] = Form("%s[%.4f,%.4f,%.4f]", Form("ctauMC_JpsiNoPR_%s", (isPbPb?"PbPb":"PP")), 0.0, 0.0, 0.0);
  }
  if (parIni.count(Form("sigmaMC_JpsiNoPR_%s", (isPbPb?"PbPb":"PP")))==0 || parIni[Form("sigmaMC_JpsiNoPR_%s", (isPbPb?"PbPb":"PP"))]=="") { 
    parIni[Form("sigmaMC_JpsiNoPR_%s", (isPbPb?"PbPb":"PP"))] = Form("%s[%.4f,%.4f,%.4f]", Form("sigmaMC_JpsiNoPR_%s", (isPbPb?"PbPb":"PP")), 0.001, 0.000000001, 1.0);
  }
  if (parIni.count(Form("ctauMC_Psi2SNoPR_%s", (isPbPb?"PbPb":"PP")))==0 || parIni[Form("ctauMC_Psi2SNoPR_%s", (isPbPb?"PbPb":"PP"))]=="") { 
    //parIni[Form("ctauMC_Psi2SNoPR_%s", (isPbPb?"PbPb":"PP"))] = Form("%s[%.4f,%.4f,%.4f]", Form("ctauMC_Psi2SNoPR_%s", (isPbPb?"PbPb":"PP")), 0.0, -100.0, 100.0);
    parIni[Form("ctauMC_Psi2SNoPR_%s", (isPbPb?"PbPb":"PP"))] = Form("%s[%.4f,%.4f,%.4f]", Form("ctauMC_Psi2SNoPR_%s", (isPbPb?"PbPb":"PP")), 0.0, 0.0, 0.0);
  }
  if (parIni.count(Form("sigmaMC_Psi2SNoPR_%s", (isPbPb?"PbPb":"PP")))==0 || parIni[Form("sigmaMC_Psi2SNoPR_%s", (isPbPb?"PbPb":"PP"))]=="") { 
    parIni[Form("sigmaMC_Psi2SNoPR_%s", (isPbPb?"PbPb":"PP"))] = Form("%s[%.4f,%.4f,%.4f]", Form("sigmaMC_Psi2SNoPR_%s", (isPbPb?"PbPb":"PP")), 0.001, 0.000000001, 1.0);
  }

  // Signal Ctau Truth Model
  if (parIni.count(Form("lambdaDSS_JpsiNoPR_%s", (isPbPb?"PbPb":"PP")))==0 || parIni[Form("lambdaDSS_JpsiNoPR_%s", (isPbPb?"PbPb":"PP"))]=="") { 
    parIni[Form("lambdaDSS_JpsiNoPR_%s", (isPbPb?"PbPb":"PP"))] = Form("%s[%.4f,%.4f,%.4f]", Form("lambdaDSS_JpsiNoPR_%s", (isPbPb?"PbPb":"PP")), 0.8, 0.001, 2.0);
  }
  if (parIni.count(Form("lambdaDSS_JpsiNoPR_%s", (isPbPb?"PbPb":"PP")))==0 || parIni[Form("lambdaDSS_JpsiNoPR_%s", (isPbPb?"PbPb":"PP"))]=="") { 
    parIni[Form("lambdaDSS_Psi2SNoPR_%s", (isPbPb?"PbPb":"PP"))] = Form("%s[%.4f,%.4f,%.4f]", Form("lambdaDSS_Psi2SNoPR_%s", (isPbPb?"PbPb":"PP")), 0.8, 0.001, 2.0);
  }

  
};
