#include "Utilities/initClasses.h"

void setDefaultParameters(map<string, string> &parIni, bool isPbPb, double numEntries);
bool defineCtauResolModel(RooWorkspace& ws, CtauModel model, map<string,string> parIni, bool isPbPb); 
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
  setDefaultParameters(parIni, isPbPb, numEntries);

  if(!defineCtauResolModel(ws, model.CtauRes, parIni, isPbPb)) { cout << "[ERROR] Defining the Ctau Resolution Model failed" << endl; return false; }
     
  if (incJpsi && incPrompt) {
    if(!addSignalCtauModel(ws, "JpsiPR", model.Jpsi.Ctau.Prompt, parIni, isPbPb)) { cout << "[ERROR] Adding Prompt Jpsi Ctau Model failed" << endl; return false; }
  }
  if (incJpsi && incNonPrompt) {
    if(!addSignalCtauModel(ws, "JpsiNoPR", model.Jpsi.Ctau.NonPrompt, parIni, isPbPb)) { cout << "[ERROR] Adding NonPrompt Jpsi Ctau Model failed" << endl; return false; }
  }
  if (incPsi2S && incPrompt) {
    if(!addSignalCtauModel(ws, "Psi2SPR", model.Psi2S.Ctau.Prompt, parIni, isPbPb)) { cout << "[ERROR] Adding Prompt Psi(2S) Mass Model failed" << endl; return false; } 
  }
  if (incPsi2S && incNonPrompt) {
    if(!addSignalCtauModel(ws, "Psi2SNoPR", model.Psi2S.Ctau.NonPrompt, parIni, isPbPb)) { cout << "[ERROR] Adding NonPrompt Psi(2S) Mass Model failed" << endl; return false; }   
  }
  if (incBkg && incPrompt) {
    if(!addBackgroundCtauModel(ws, "BkgPR", model.Bkg.Ctau.Prompt, parIni, isPbPb)) { cout << "[ERROR] Adding Prompt Background Ctau Model failed" << endl; return false; }
  }
  if (incBkg && incNonPrompt) {
    if(!addBackgroundCtauModel(ws, "BkgNoPR", model.Bkg.Ctau.NonPrompt, parIni, isPbPb)) { cout << "[ERROR] Adding NonPrompt Background Ctau Model failed" << endl; return false; }
  }

  ws.factory(Form("SUM::%s(%s*%s, %s)", Form("pdfCTAU_Jpsi_%s", (isPbPb?"PbPb":"PP")),
                  parIni[Form("f_JpsiNP_%s", (isPbPb?"PbPb":"PP"))].c_str(),
                  Form("pdfCTAU_JpsiNP_%s", (isPbPb?"PbPb":"PP")),
                  Form("pdfCTAU_JpsiP_%s", (isPbPb?"PbPb":"PP"))
                  ));
  ws.factory(Form("PROD::%s(%s, %s)", Form("pdfCTAUMASS_Jpsi_%s", (isPbPb?"PbPb":"PP")),
                  Form("pdfCTAU_Jpsi_%s", (isPbPb?"PbPb":"PP")),
                  Form("pdfMASS_Jpsi_%s", (isPbPb?"PbPb":"PP"))
                  ));

  ws.factory(Form("SUM::%s(%s*%s, %s)", Form("pdfCTAU_Psi2S_%s", (isPbPb?"PbPb":"PP")),
                  parIni[Form("f_Psi2SNP_%s", (isPbPb?"PbPb":"PP"))].c_str(),
                  Form("pdfCTAU_Psi2SNP_%s", (isPbPb?"PbPb":"PP")),
                  Form("pdfCTAU_Psi2SP_%s", (isPbPb?"PbPb":"PP"))
                  ));
  ws.factory(Form("PROD::%s(%s, %s)", Form("pdfCTAUMASS_Psi2S_%s", (isPbPb?"PbPb":"PP")),
                  Form("pdfCTAU_Bkg_%s", (isPbPb?"PbPb":"PP")),
                  Form("pdfMASS_Bkg_%s", (isPbPb?"PbPb":"PP"))
                  ));

  ws.factory(Form("SUM::%s(%s*%s, %s)", Form("pdfCTAU_Bkg_%s", (isPbPb?"PbPb":"PP")),
                  parIni[Form("f_BkgNP_%s", (isPbPb?"PbPb":"PP"))].c_str(),
                  Form("pdfCTAU_BkgNP_%s", (isPbPb?"PbPb":"PP")),
                  Form("pdfCTAU_BkgP_%s", (isPbPb?"PbPb":"PP"))
                  // Total PDF = Signal + Background
                  if (opt.inExcStat) {
                    ws.factory(Form("SUM::%s(%s*%s, %s*%s, %s*%s)", Form("pdfMASS_Tot_%s", (isPbPb?"PbPb":"PP")),
                                    parIni[Form("N_Jpsi_%s", (isPbPb?"PbPb":"PP"))].c_str(),
                                    Form("pdfMASS_Jpsi_%s", (isPbPb?"PbPb":"PP")),
                                    parIni[Form("N_Psi2S_%s", (isPbPb?"PbPb":"PP"))].c_str(),
                                    Form("pdfMASS_Psi2S_%s", (isPbPb?"PbPb":"PP")),
                                    parIni[Form("N_Bkg_%s", (isPbPb?"PbPb":"PP"))].c_str(),
                                    Form("pdfMASS_Bkg_%s", (isPbPb?"PbPb":"PP"))
                                    ));
                    ws.factory(Form("PROD::%s(%s, %s)", Form("pdfCTAUMASS_Bkg_%s", (isPbPb?"PbPb":"PP")),
                                    Form("pdfCTAU_Bkg_%s", (isPbPb?"PbPb":"PP")),
                                    } else {
                                 ws.factory(Form("SUM::%s(%s*%s, %s*%s)", Form("pdfMASS_Tot_%s", (isPbPb?"PbPb":"PP")),
                                                 parIni[Form("N_Jpsi_%s", (isPbPb?"PbPb":"PP"))].c_str(),
                                                 Form("pdfMASS_Jpsi_%s", (isPbPb?"PbPb":"PP")),
                                                 parIni[Form("N_Bkg_%s", (isPbPb?"PbPb":"PP"))].c_str(),
                                                 Form("pdfMASS_Bkg_%s", (isPbPb?"PbPb":"PP"))
                                                 ));
     
                                 // Total PDF = Signal + Background
                                 if (opt.inExcStat) {
                                   ws.factory(Form("SUM::%s(%s*%s, %s*%s, %s*%s)", Form("pdfMASS_Tot_%s", (isPbPb?"PbPb":"PP")),
                                                   parIni[Form("N_Jpsi_%s", (isPbPb?"PbPb":"PP"))].c_str(),
                                                   Form("pdfMASS_Jpsi_%s", (isPbPb?"PbPb":"PP")),
                                                   parIni[Form("N_Psi2S_%s", (isPbPb?"PbPb":"PP"))].c_str(),
                                                   Form("pdfMASS_Psi2S_%s", (isPbPb?"PbPb":"PP")),
                                                   parIni[Form("N_Bkg_%s", (isPbPb?"PbPb":"PP"))].c_str(),
                                                   Form("pdfMASS_Bkg_%s", (isPbPb?"PbPb":"PP"))
                                                   ));
                                   ws.factory(Form("SUM::%s(%s*%s, %s*%s, %s*%s)", Form("pdfCTAUMASS_Tot_%s", (isPbPb?"PbPb":"PP")),
                                                   parIni[Form("N_Jpsi_%s", (isPbPb?"PbPb":"PP"))].c_str(),
                                                   Form("pdfCTAUMASS_Jpsi_%s", (isPbPb?"PbPb":"PP")),
                                                   parIni[Form("N_Psi2S_%s", (isPbPb?"PbPb":"PP"))].c_str(),
                                                   Form("pdfCTAUMASS_Psi2S_%s", (isPbPb?"PbPb":"PP")),
                                                   parIni[Form("N_Bkg_%s", (isPbPb?"PbPb":"PP"))].c_str(),
                                                   Form("pdfCTAUMASS_Bkg_%s", (isPbPb?"PbPb":"PP"))
                                                   ));
                                 } else {
                                   ws.factory(Form("SUM::%s(%s*%s, %s*%s)", Form("pdfMASSTot%s", (isPbPb?"PbPb":"PP")),
                                                   parIni[Form("N_Jpsi_%s", (isPbPb?"PbPb":"PP"))].c_str(),
                                                   Form("pdfMASS_Jpsi_%s", (isPbPb?"PbPb":"PP")),
                                                   parIni[Form("N_Bkg_%s", (isPbPb?"PbPb":"PP"))].c_str(),
                                                   Form("pdfMASS_Bkg_%s", (isPbPb?"PbPb":"PP"))
                                                   ));
                                   ws.factory(Form("SUM::%s(%s*%s, %s*%s)", Form("pdfCTAUMASS_Tot_%s", (isPbPb?"PbPb":"PP")),
                                                   parIni[Form("N_Jpsi_%s", (isPbPb?"PbPb":"PP"))].c_str(),
                                                   Form("pdfCTAUMASS_Jpsi_%s", (isPbPb?"PbPb":"PP")),
                                                   parIni[Form("N_Bkg_%s", (isPbPb?"PbPb":"PP"))].c_str(),
                                                   Form("pdfCTAUMASS_Bkg_%s", (isPbPb?"PbPb":"PP"))
                                                   ));
                                 }
                                 ws.pdf(Form("pdfMASS_Tot_%s", (isPbPb?"PbPb":"PP")))->setNormRange("MassWindow");
                                 ws.pdf(Form("pdfCTAUMASS_Tot_%s", (isPbPb?"PbPb":"PP")))->setNormRange("MassWindow");
 
                                 

  // C r e a t e   m o d e l  

  if (incJpsi) {
    if(!addSignalMassModel(ws, "Jpsi", model.Jpsi.Mass, parIni, isPbPb)) { cout << "[ERROR] Adding Jpsi Mass Model failed" << endl; return false; }
  }
  if (incPsi2S) { 
    if (!addSignalMassModel(ws, "Psi2S", model.Psi2S.Mass, parIni, isPbPb)) { cout << "[ERROR] Adding Psi(2S) Mass Model failed" << endl; return false; }  
  }
  if (incBkg) {
    if(!addBackgroundMassModel(ws, "Bkg", model.Bkg.Mass, parIni, isPbPb)) { cout << "[ERROR] Adding Background Mass Model failed" << endl; return false; }
  }
  // Total PDF
  string pdfName = Form("pdfMASS_Tot_%s", (isPbPb?"PbPb":"PP"));
  RooAbsPdf *themodel = NULL;
  if (incJpsi && incPsi2S && incBkg) {
    themodel = new RooAddPdf(pdfName.c_str(), pdfName.c_str(), 
                      RooArgList( 
                                 *ws.pdf(Form("pdfMASSTot_Jpsi_%s", (isPbPb?"PbPb":"PP"))), 
                                 *ws.pdf(Form("pdfMASSTot_Psi2S_%s", (isPbPb?"PbPb":"PP"))), 
                                 *ws.pdf(Form("pdfMASSTot_Bkg_%s", (isPbPb?"PbPb":"PP"))) 
                                  )
                      );
  }
  if (incJpsi && incPsi2S && !incBkg) {
    themodel = new RooAddPdf(pdfName.c_str(), pdfName.c_str(), 
                      RooArgList( 
                                 *ws.pdf(Form("pdfMASSTot_Jpsi_%s", (isPbPb?"PbPb":"PP"))), 
                                 *ws.pdf(Form("pdfMASSTot_Psi2S_%s", (isPbPb?"PbPb":"PP")))
                                  )
                      );
  }
  if (incJpsi && !incPsi2S && incBkg) {
    themodel = new RooAddPdf(pdfName.c_str(), pdfName.c_str(), 
                      RooArgList( 
                                 *ws.pdf(Form("pdfMASSTot_Jpsi_%s", (isPbPb?"PbPb":"PP"))), 
                                 *ws.pdf(Form("pdfMASSTot_Bkg_%s", (isPbPb?"PbPb":"PP")))
                                  )
                      );
  }
  if (!incJpsi && incPsi2S && incBkg) {
    themodel = new RooAddPdf(pdfName.c_str(), pdfName.c_str(), 
                      RooArgList( 
                                 *ws.pdf(Form("pdfMASSTot_Psi2S_%s", (isPbPb?"PbPb":"PP"))), 
                                 *ws.pdf(Form("pdfMASSTot_Bkg_%s", (isPbPb?"PbPb":"PP")))
                                  )
                      );
  }
  if (incJpsi && !incPsi2S && !incBkg) {
    themodel = new RooAddPdf(pdfName.c_str(), pdfName.c_str(), 
                      RooArgList( 
                                 *ws.pdf(Form("pdfMASSTot_Jpsi_%s", (isPbPb?"PbPb":"PP")))
                                  )
                      );
  }
  if (!incJpsi && incPsi2S && !incBkg) {
    themodel = new RooAddPdf(pdfName.c_str(), pdfName.c_str(), 
                      RooArgList( 
                                 *ws.pdf(Form("pdfMASSTot_Psi2S_%s", (isPbPb?"PbPb":"PP")))
                                  )
                      );
  }
  if (!incJpsi && !incPsi2S && incBkg) {
    themodel = new RooAddPdf(pdfName.c_str(), pdfName.c_str(), 
                      RooArgList( 
                                 *ws.pdf(Form("pdfMASSTot_Bkg_%s", (isPbPb?"PbPb":"PP")))
                                  )
                      );
  }
  if (!incJpsi && !incPsi2S && !incBkg) {
    cout << "[ERROR] User did not include any model, please fix your input settings!" << endl; return false;
  }
  ws.import(*themodel);
  ws.pdf(pdfName.c_str())->setNormRange("MassWindow");

  // save the initial values of the model we've just created
  RooRealVar *x = ws.var("invMass");
  RooArgSet* params = (RooArgSet*) themodel->getParameters(*x) ;
  pdfName+="_parIni";
  ws.saveSnapshot(pdfName.c_str(),*params,kTRUE) ;
  
  //ws.Print();
  return true;
};

bool addBackgroundMassModel(RooWorkspace& ws, string object, MassModel model, map<string,string> parIni, bool isPbPb) 
{
  cout << Form("[INFO] Implementing %s Background Mass Model", object.c_str()) << endl;
  
  switch(model) 
    {  
    case (MassModel::Chebychev1): 

      // check that all input parameters are defined 
      if (!( 
            parIni.count(Form("lambda1_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))) 
             )) { 
	cout << Form("[ERROR] Initial parameters where not found for %s Background First Order Chebychev in %s", object.c_str(), (isPbPb?"PbPb":"PP")) << endl; return false;
      } 

      // create the variables for this model
      ws.factory( parIni[Form("lambda1_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str() );

      // create the PDF           
      ws.factory(Form("Chebychev::%s(%s, {%s})", Form("pdfMASS_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")), "invMass", 
		      Form("lambda1_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))
		      ));

      ws.factory(Form("RooExtendPdf::%s(%s,%s)", Form("pdfMASSTot_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")),
                      Form("pdfMASS_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")),
                      parIni[Form("N_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str()
                      ));

      cout << Form("[INFO] %s Background 1st Order Chebychev PDF in %s included", object.c_str(), (isPbPb?"PbPb":"PP")) << endl; break;
 
    case (MassModel::Chebychev2): 

      // check that all input parameters are defined 
      if (!( 
            parIni.count(Form("lambda1_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))) && 
            parIni.count(Form("lambda2_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))) 
             )) { 
	cout << Form("[ERROR] Initial parameters where not found for %s Background Second Order Chebychev in %s", object.c_str(), (isPbPb?"PbPb":"PP")) << endl; return false;
      } 

      // create the variables for this model
      ws.factory( parIni[Form("lambda1_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str() );
      ws.factory( parIni[Form("lambda2_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str() );

      // create the PDF           
      ws.factory(Form("Chebychev::%s(%s, {%s, %s})", Form("pdfMASS_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")), "invMass", 
		      Form("lambda1_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")), 
		      Form("lambda2_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))
		      ));

      ws.factory(Form("RooExtendPdf::%s(%s,%s)", Form("pdfMASSTot_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")),
                      Form("pdfMASS_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")),
                      parIni[Form("N_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str()
                      ));

      cout << Form("[INFO] %s Background 2nd Order Chebychev PDF in %s included", object.c_str(), (isPbPb?"PbPb":"PP")) << endl; break; 

    case (MassModel::Chebychev3): 

      // check that all input parameters are defined 
      if (!( 
            parIni.count(Form("lambda1_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))) && 
            parIni.count(Form("lambda2_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))) && 
            parIni.count(Form("lambda3_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))) 
             )) { 
	cout << Form("[ERROR] Initial parameters where not found for %s Background Third Order Chebychev in %s", object.c_str(), (isPbPb?"PbPb":"PP")) << endl; return false;
      }

      // create the variables for this model 
      ws.factory( parIni[Form("lambda1_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str() );
      ws.factory( parIni[Form("lambda2_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str() );
      ws.factory( parIni[Form("lambda3_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str() );

      // create the PDF                 
      ws.factory(Form("Chebychev::%s(%s, {%s, %s, %s})", Form("pdfMASS_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")), "invMass", 
		      Form("lambda1_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")), 
		      Form("lambda2_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")), 
		      Form("lambda3_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))
		      ));

      ws.factory(Form("RooExtendPdf::%s(%s,%s)", Form("pdfMASSTot_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")),
                      Form("pdfMASS_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")),
                      parIni[Form("N_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str()
                      ));

      cout << Form("[INFO] %s Background 3rd Order Chebychev PDF in %s included", object.c_str(), (isPbPb?"PbPb":"PP")) << endl; break; 

    case (MassModel::Chebychev4): 

      // check that all input parameters are defined 
      if (!( 
            parIni.count(Form("lambda1_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))) && 
            parIni.count(Form("lambda2_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))) && 
            parIni.count(Form("lambda3_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))) && 
            parIni.count(Form("lambda4_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))) 
             )) { 
	cout << Form("[ERROR] Initial parameters where not found for %s Background Fourth Order Chebychev in %s", object.c_str(), (isPbPb?"PbPb":"PP")) << endl; return false;
      }

      // create the variables for this model        
      ws.factory( parIni[Form("lambda1_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str() );
      ws.factory( parIni[Form("lambda2_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str() );
      ws.factory( parIni[Form("lambda3_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str() );
      ws.factory( parIni[Form("lambda4_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str() );

      // create the PDF                 
      ws.factory(Form("Chebychev::%s(%s, {%s, %s, %s, %s})", Form("pdfMASS_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")), "invMass", 
		      Form("lambda1_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")), 
		      Form("lambda2_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")), 
		      Form("lambda3_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")), 
		      Form("lambda4_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))
		      ));

      ws.factory(Form("RooExtendPdf::%s(%s,%s)", Form("pdfMASSTot_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")),
                      Form("pdfMASS_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")),
                      parIni[Form("N_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str()
                      ));

      cout << Form("[INFO] %s Background 4th Order Chebychev PDF in %s included", object.c_str(), (isPbPb?"PbPb":"PP")) << endl; break; 

    case (MassModel::Chebychev5): 

      // check that all input parameters are defined 
      if (!( 
            parIni.count(Form("lambda1_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))) && 
            parIni.count(Form("lambda2_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))) && 
            parIni.count(Form("lambda3_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))) && 
            parIni.count(Form("lambda4_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))) && 
            parIni.count(Form("lambda5_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))) 
             )) { 
	cout << Form("[ERROR] Initial parameters where not found for %s Background Fifth Order Chebychev in %s", object.c_str(), (isPbPb?"PbPb":"PP")) << endl; return false;
      }

      // create the variables for this model        
      ws.factory( parIni[Form("lambda1_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str() );
      ws.factory( parIni[Form("lambda2_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str() );
      ws.factory( parIni[Form("lambda3_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str() );
      ws.factory( parIni[Form("lambda4_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str() );
      ws.factory( parIni[Form("lambda5_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str() );

      // create the PDF                 
      ws.factory(Form("Chebychev::%s(%s, {%s, %s, %s, %s, %s})", Form("pdfMASS_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")), "invMass", 
		      Form("lambda1_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")), 
		      Form("lambda2_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")), 
		      Form("lambda3_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")), 
		      Form("lambda4_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")), 
		      Form("lambda5_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))
		      ));

      ws.factory(Form("RooExtendPdf::%s(%s,%s)", Form("pdfMASSTot_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")),
                      Form("pdfMASS_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")),
                      parIni[Form("N_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str()
                      ));

      cout << Form("[INFO] %s Background 5th Order Chebychev PDF in %s included", object.c_str(), (isPbPb?"PbPb":"PP")) << endl; break; 

    case (MassModel::Chebychev6): 

      // check that all input parameters are defined 
      if (!( 
            parIni.count(Form("lambda1_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))) && 
            parIni.count(Form("lambda2_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))) && 
            parIni.count(Form("lambda3_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))) && 
            parIni.count(Form("lambda4_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))) && 
            parIni.count(Form("lambda5_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))) && 
            parIni.count(Form("lambda6_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))) 
             )) { 
	cout << Form("[ERROR] Initial parameters where not found for %s Background Sixth Order Chebychev in %s", object.c_str(), (isPbPb?"PbPb":"PP")) << endl; return false;
      }

      // create the variables for this model        
      ws.factory( parIni[Form("lambda1_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str() );
      ws.factory( parIni[Form("lambda2_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str() );
      ws.factory( parIni[Form("lambda3_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str() );
      ws.factory( parIni[Form("lambda4_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str() );
      ws.factory( parIni[Form("lambda5_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str() );
      ws.factory( parIni[Form("lambda6_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str() );

      // create the PDF                 
      ws.factory(Form("Chebychev::%s(%s, {%s, %s, %s, %s, %s, %s})", Form("pdfMASS_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")), "invMass", 
		      Form("lambda1_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")), 
		      Form("lambda2_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")), 
		      Form("lambda3_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")), 
		      Form("lambda4_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")), 
		      Form("lambda5_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")), 
		      Form("lambda6_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))
		      ));

      ws.factory(Form("RooExtendPdf::%s(%s,%s)", Form("pdfMASSTot_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")),
                      Form("pdfMASS_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")),
                      parIni[Form("N_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str()
                      ));

      cout << Form("[INFO] %s Background 6th Order Chebychev PDF in %s included", object.c_str(), (isPbPb?"PbPb":"PP")) << endl; break; 

    case (MassModel::ExpChebychev1): 

      // check that all input parameters are defined 
      if (!( 
            parIni.count(Form("lambda1_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))) 
             )) { 
	cout << Form("[ERROR] Initial parameters where not found for %s Background First Order Exponential Chebychev in %s", object.c_str(), (isPbPb?"PbPb":"PP")) << endl; return false;
      }

      // create the variables for this model        
      ws.factory( parIni["invMassNorm"].c_str() );
      ws.factory( parIni[Form("lambda1_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str() );

      // create the PDF                 
      ws.factory(Form("RooFormulaVar::%s('@1*(@0) + 1.0', {%s, %s})", 
                      Form("pdfMASSPol_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")), "invMassNorm", 
		      Form("lambda1_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))
		      ));               

      ws.factory(Form("Exponential::%s(%s, One[1.0])", Form("pdfMASS_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")),
                      Form("pdfMASSPol_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))
                      ));

      ws.factory(Form("RooExtendPdf::%s(%s,%s)", Form("pdfMASSTot_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")),
                      Form("pdfMASS_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")),
                      parIni[Form("N_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str()
                      ));

      cout << Form("[INFO] %s Background 1st Order Exponential Chebychev PDF in %s included", object.c_str(), (isPbPb?"PbPb":"PP")) << endl; break; 
   
    case (MassModel::ExpChebychev2): 

      // check that all input parameters are defined 
      if (!( 
            parIni.count(Form("lambda1_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))),
            parIni.count(Form("lambda2_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")))  
             )) { 
	cout << Form("[ERROR] Initial parameters where not found for %s Background Second Order Exponential Chebychev in %s", object.c_str(), (isPbPb?"PbPb":"PP")) << endl; return false;
      }

      // create the variables for this model    
      ws.factory( parIni["invMassNorm"].c_str() );    
      ws.factory( parIni[Form("lambda1_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str() );
      ws.factory( parIni[Form("lambda2_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str() );

      // create the PDF            
      ws.factory(Form("RooFormulaVar::%s('@2*(2.0*@0*@0 - 1.0) + @1*(@0) + 1.0', {%s, %s, %s})", 
                      Form("pdfMASSPol_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")), "invMassNorm",
		      Form("lambda1_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")), 
		      Form("lambda2_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))
		      ));             

      ws.factory(Form("Exponential::%s(%s, One[1.0])", Form("pdfMASS_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")),
                      Form("pdfMASSPol_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))
                      ));

      ws.factory(Form("RooExtendPdf::%s(%s,%s)", Form("pdfMASSTot_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")),
                      Form("pdfMASS_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")),
                      parIni[Form("N_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str()
                      ));

      cout << Form("[INFO] %s Background 2nd Order Exponential Chebychev PDF in %s included", object.c_str(), (isPbPb?"PbPb":"PP")) << endl; break; 
   
    case (MassModel::ExpChebychev3): 

      // check that all input parameters are defined 
      if (!( 
            parIni.count(Form("lambda1_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))),
            parIni.count(Form("lambda2_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))),
            parIni.count(Form("lambda3_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")))  
             )) { 
	cout << Form("[ERROR] Initial parameters where not found for %s Background Third Order Exponential Chebychev in %s", object.c_str(), (isPbPb?"PbPb":"PP")) << endl; return false;
      }

      // create the variables for this model  
      ws.factory( parIni["invMassNorm"].c_str() );      
      ws.factory( parIni[Form("lambda1_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str() );
      ws.factory( parIni[Form("lambda2_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str() );
      ws.factory( parIni[Form("lambda3_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str() );

      // create the PDF
      ws.factory(Form("RooFormulaVar::%s('@3*(4.0*@0*@0*@0 - 3.0*@0) + @2*(2.0*@0*@0 - 1.0) + @1*(@0) + 1.0', {%s, %s, %s, %s})", 
                      Form("pdfMASSPol_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")), "invMassNorm",
		      Form("lambda1_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")), 
		      Form("lambda2_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")), 
		      Form("lambda3_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))
		      ));        

      ws.factory(Form("Exponential::%s(%s, One[1.0])", Form("pdfMASS_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")),
                      Form("pdfMASSPol_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))
                      ));

      ws.factory(Form("RooExtendPdf::%s(%s,%s)", Form("pdfMASSTot_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")),
                      Form("pdfMASS_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")),
                      parIni[Form("N_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str()
                      ));

      cout << Form("[INFO] %s Background 3rd Order Exponential Chebychev PDF in %s included", object.c_str(), (isPbPb?"PbPb":"PP")) << endl; break; 
   
    case (MassModel::ExpChebychev4): 

      // check that all input parameters are defined 
      if (!( 
            parIni.count(Form("lambda1_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))),
            parIni.count(Form("lambda2_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))),
            parIni.count(Form("lambda3_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))),
            parIni.count(Form("lambda4_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")))    
             )) { 
	cout << Form("[ERROR] Initial parameters where not found for %s Background Fourth Order Exponential Chebychev in %s", object.c_str(), (isPbPb?"PbPb":"PP")) << endl; return false;
      }

      // create the variables for this model   
      ws.factory( parIni["invMassNorm"].c_str() );     
      ws.factory( parIni[Form("lambda1_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str() );
      ws.factory( parIni[Form("lambda2_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str() );
      ws.factory( parIni[Form("lambda3_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str() );
      ws.factory( parIni[Form("lambda4_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str() );

      // create the PDF                 
      ws.factory(Form("RooFormulaVar::%s('@4*(8.0*@0*@0*@0*@0 - 8.0*@0*@0 + 1.0) + @3*(4.0*@0*@0*@0 - 3.0*@0) + @2*(2.0*@0*@0 - 1.0) + @1*(@0) + 1.0', {%s, %s, %s, %s, %s})", 
                      Form("pdfMASSPol_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")), "invMassNorm",
		      Form("lambda1_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")), 
		      Form("lambda2_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")), 
		      Form("lambda3_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")), 
		      Form("lambda4_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))
		      ));

      ws.factory(Form("Exponential::%s(%s, One[1.0])", Form("pdfMASS_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")),
                      Form("pdfMASSPol_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))
                      ));

      ws.factory(Form("RooExtendPdf::%s(%s,%s)", Form("pdfMASSTot_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")),
                      Form("pdfMASS_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")),
                      parIni[Form("N_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str()
                      ));

      cout << Form("[INFO] %s Background 4th Order Exponential Chebychev PDF in %s included", object.c_str(), (isPbPb?"PbPb":"PP")) << endl; break; 
   
    case (MassModel::ExpChebychev5): 

      // check that all input parameters are defined 
      if (!( 
            parIni.count(Form("lambda1_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))),
            parIni.count(Form("lambda2_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))),
            parIni.count(Form("lambda3_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))),
            parIni.count(Form("lambda4_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))),
            parIni.count(Form("lambda5_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")))    
             )) { 
	cout << Form("[ERROR] Initial parameters where not found for %s Background Fifth Order Exponential Chebychev in %s", object.c_str(), (isPbPb?"PbPb":"PP")) << endl; return false;
      }

      // create the variables for this model     
      ws.factory( parIni["invMassNorm"].c_str() );   
      ws.factory( parIni[Form("lambda1_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str() );
      ws.factory( parIni[Form("lambda2_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str() );
      ws.factory( parIni[Form("lambda3_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str() );
      ws.factory( parIni[Form("lambda4_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str() );
      ws.factory( parIni[Form("lambda5_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str() );

      // create the PDF       
      ws.factory(Form("RooFormulaVar::%s('@5*(16.0*@0*@0*@0*@0*@0 - 20.0*@0*@0*@0 + 5.0*@0) + @4*(8.0*@0*@0*@0*@0 - 8.0*@0*@0 + 1.0) + @3*(4.0*@0*@0*@0 - 3.0*@0) + @2*(2.0*@0*@0 - 1.0) + @1*(@0) + 1.0', {%s, %s, %s, %s, %s, %s})", 
                      Form("pdfMASSPol_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")), "invMassNorm", 
		      Form("lambda1_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")), 
		      Form("lambda2_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")), 
		      Form("lambda3_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")), 
		      Form("lambda4_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")), 
		      Form("lambda5_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))
		      ));

      ws.factory(Form("Exponential::%s(%s, One[1.0])", Form("pdfMASS_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")),
                      Form("pdfMASSPol_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))
                      ));

      ws.factory(Form("RooExtendPdf::%s(%s,%s)", Form("pdfMASSTot_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")),
                      Form("pdfMASS_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")),
                      parIni[Form("N_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str()
                      ));

      cout << Form("[INFO] %s Background 5th Order Exponential Chebychev PDF in %s included", object.c_str(), (isPbPb?"PbPb":"PP")) << endl; break; 
   
    case (MassModel::ExpChebychev6): 

      // check that all input parameters are defined 
      if (!( 
            parIni.count(Form("lambda1_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))),
            parIni.count(Form("lambda2_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))),
            parIni.count(Form("lambda3_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))),
            parIni.count(Form("lambda4_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))),
            parIni.count(Form("lambda5_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))),
            parIni.count(Form("lambda6_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")))     
             )) { 
	cout << Form("[ERROR] Initial parameters where not found for %s Background Sixth Order Exponential Chebychev in %s", object.c_str(), (isPbPb?"PbPb":"PP")) << endl; return false;
      }

      // create the variables for this model    
      ws.factory( parIni["invMassNorm"].c_str() );    
      ws.factory( parIni[Form("lambda1_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str() );
      ws.factory( parIni[Form("lambda2_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str() );
      ws.factory( parIni[Form("lambda3_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str() );
      ws.factory( parIni[Form("lambda4_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str() );
      ws.factory( parIni[Form("lambda5_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str() );
      ws.factory( parIni[Form("lambda6_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str() );

      // create the PDF
      ws.factory(Form("RooFormulaVar::%s('@6*(32.0*@0*@0*@0*@0*@0*@0 - 48.0*@0*@0*@0*@0 + 18.0*@0*@0 - 1.0) + @5*(16.0*@0*@0*@0*@0*@0 - 20.0*@0*@0*@0 + 5.0*@0) + @4*(8.0*@0*@0*@0*@0 - 8.0*@0*@0 + 1.0) + @3*(4.0*@0*@0*@0 - 3.0*@0) + @2*(2.0*@0*@0 - 1.0) + @1*(@0) + 1.0', {%s, %s, %s, %s, %s, %s, %s})", 
                      Form("pdfMASSPol_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")), "invMassNorm",
		      Form("lambda1_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")), 
		      Form("lambda2_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")), 
		      Form("lambda3_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")), 
		      Form("lambda4_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")), 
		      Form("lambda5_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")), 
		      Form("lambda6_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))
		      ));

      ws.factory(Form("Exponential::%s(%s, One[1.0])", Form("pdfMASS_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")),
                      Form("pdfMASSPol_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))
                      ));

      ws.factory(Form("RooExtendPdf::%s(%s,%s)", Form("pdfMASSTot_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")),
                      Form("pdfMASS_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")),
                      parIni[Form("N_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str()
                      ));

      cout << Form("[INFO] %s Background 6th Order Exponential Chebychev PDF in %s included", object.c_str(), (isPbPb?"PbPb":"PP")) << endl; break; 

    case (MassModel::Exponential): 

      // check that all input parameters are defined 
      if (!( 
            parIni.count(Form("lambda1_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))) 
             )) { 
	cout << Form("[ERROR] Initial parameters where not found for %s Background Exponential in %s", object.c_str(), (isPbPb?"PbPb":"PP")) << endl; return false;
      }

      // create the variables for this model        
      ws.factory( parIni[Form("lambda1_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str() );

      // create the PDF                 
      ws.factory(Form("Exponential::%s(%s, %s)", Form("pdfMASS_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")), "invMass", 
		      Form("lambda1_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))
		      ));

      ws.factory(Form("RooExtendPdf::%s(%s,%s)", Form("pdfMASSTot_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")),
                      Form("pdfMASS_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")),
                      parIni[Form("N_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str()
                      ));

      cout << Form("[INFO] %s Background Exponential PDF in %s included", object.c_str(), (isPbPb?"PbPb":"PP")) << endl; break;
   
    default :
      
      cout << "[ERROR] Selected Background Mass Model: " << parIni[Form("Model_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))] << " has not been implemented" << endl; return false;
    
    }
  
  return true;
};



bool defineCtauResolModel(RooWorkspace& ws, CtauModel model, map<string,string> parIni, bool isPbPb) 
{ 
  cout << "[INFO] Implementing Ctau Resolution Model" << endl;
  
  switch(model) 
    {  
    case (CtauModel::SingleGaussianResolution):  
      if (!( 
            parIni.count(Form("ctau1_ctauRes_%s", (isPbPb?"PbPb":"PP"))) && 
            parIni.count(Form("sigma1_ctauRes_%s", (isPbPb?"PbPb":"PP"))) 
             )) { 
 	cout << Form("[ERROR] Initial parameters where not found for Single Gaussian Ctau Resolution Model in %s", (isPbPb?"PbPb":"PP")) << endl; return false; 
      }

      // create the variables for this model  
      ws.factory( parIni[Form("ctau1_ctauRes_%s", (isPbPb?"PbPb":"PP"))].c_str() );
      ws.factory( parIni[Form("sigma1_ctauRes_%s", (isPbPb?"PbPb":"PP"))].c_str() );      

      // create the two PDFs
      ws.factory(Form("GaussModel::%s(%s, %s, %s, one[1.0], %s)", Form("pdfCTAU_ctauRes_%s", (isPbPb?"PbPb":"PP")), "ctau", 
 		      Form("ctau1_ctauRes_%s", (isPbPb?"PbPb":"PP")), 
 		      Form("sigma1_ctauRes_%s", (isPbPb?"PbPb":"PP")),
 		      "ctauErr"
 		      ));

      cout << Form("[INFO] Single Gaussian Ctau Resolution PDF in %s included", (isPbPb?"PbPb":"PP")) << endl; break;
       
    case (CtauModel::DoubleGaussianResolution):  

      if (!( 
            parIni.count(Form("ctau1_ctauRes_%s", (isPbPb?"PbPb":"PP"))) && 
            parIni.count(Form("sigma1_ctauRes_%s", (isPbPb?"PbPb":"PP"))) && 
            parIni.count(Form("ctau2_ctauRes_%s", (isPbPb?"PbPb":"PP"))) && 
            parIni.count(Form("sigma2_ctauRes_%s", (isPbPb?"PbPb":"PP"))) && 
            parIni.count(Form("f_ctauRes_%s", (isPbPb?"PbPb":"PP"))) 
             )) { 
 	cout << Form("[ERROR] Initial parameters where not found for Double Gaussian Ctau Resolution Model in %s", (isPbPb?"PbPb":"PP")) << endl; return false; 
      }
      
      // create the variables for this model  
      ws.factory( parIni[Form("ctau1_ctauRes_%s", (isPbPb?"PbPb":"PP"))].c_str()  );
      ws.factory( parIni[Form("sigma1_ctauRes_%s", (isPbPb?"PbPb":"PP"))].c_str() );
      ws.factory( parIni[Form("ctau2_ctauRes_%s", (isPbPb?"PbPb":"PP"))].c_str()  );
      ws.factory( parIni[Form("sigma2_ctauRes_%s", (isPbPb?"PbPb":"PP"))].c_str() );

      // create the two PDFs
      ws.factory(Form("GaussModel::%s(%s, %s, %s, one[1.0], %s)", Form("pdfCTAU1_ctauRes_%s", (isPbPb?"PbPb":"PP")), "ctau", 
 		      Form("ctau1_ctauRes_%s", (isPbPb?"PbPb":"PP")), 
 		      Form("sigma1_ctauRes_%s", (isPbPb?"PbPb":"PP")),
 		      "ctauErr"
 		      ));
      ws.factory(Form("GaussModel::%s(%s, %s, %s, one[1.0], %s)", Form("pdfCTAU2_ctauRes_%s", (isPbPb?"PbPb":"PP")), "ctau", 
 		      Form("ctau2_ctauRes_%s", (isPbPb?"PbPb":"PP")).c_str(), 
 		      Form("sigma2_ctauRes_%s", (isPbPb?"PbPb":"PP")).c_str(),
 		      "ctauErr"
 		      ));

      // combine the two PDFs
      ws.factory(Form("AddModel::%s({%s, %s}, {%s})", Form("pdfCTAU_ctauRes_%s", (isPbPb?"PbPb":"PP")), 
 		      Form("pdfCTAUG1_ctauRes_%s", (isPbPb?"PbPb":"PP")), 
 		      Form("pdfCTAUG2_ctauRes_%s", (isPbPb?"PbPb":"PP")),  
 		      parIni[Form("f_ctauRes_%s", (isPbPb?"PbPb":"PP"))].c_str()
 		      ));
      cout << Form("[INFO] Double Gaussian Ctau Resolution PDF in %s included", (isPbPb?"PbPb":"PP")) << endl; break;
 
    default :
      cout<< "[ERROR] Selected Ctau Resolution Model has not been implemented"<< endl; return false;

    }
 
  return true;

};

bool addBackgroundCtauModel(RooWorkspace& ws, string object, CtauModel model, map<string,string> parIni, bool isPbPb) 
{
  cout << Form("[INFO] Implementing %s Background Ctau Model", object.c_str()) << endl;
   
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
      ws.factory( parIni[Form("lambdaDSS_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str() ); 
      ws.factory( parIni[Form("lambdaDF_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str()  );
      ws.factory( parIni[Form("lambdaDDS_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str() ); 
      ws.factory( parIni[Form("fDFSS_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str()     ); 
      ws.factory( parIni[Form("fDLIV_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str()     );

      // create the three PDFs
      ws.factory(Form("Decay::%s(%s, %s, %s, RooDecay::SingleSided)", Form("pdfCTAUDSS_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")), "ctau", 
 		      Form("lambdaDSS_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")),
 		      Form("pdfCTAU_ctauRes_%s", (isPbPb?"PbPb":"PP"))
 		      ));
      ws.factory(Form("Decay::%s(%s, %s, %s, RooDecay::Flipped)", Form("pdfCTAUDF_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")), "ctau", 
 		      Form("lambdaDF_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")),
 		      Form("pdfCTAU_ctauRes_%s", (isPbPb?"PbPb":"PP"))
 		      ));
      ws.factory(Form("Decay::%s(%s, %s, %s, RooDecay::DoubleSided)", Form("pdfCTAUDDS_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")), "ctau", 
 		      Form("lambdaDDS_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")),
 		      Form("pdfCTAU_ctauRes_%s", (isPbPb?"PbPb":"PP"))
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
 
      cout << Form("[INFO] %s Background Triple Decay Ctau PDF in %s included", object.c_str(), (isPbPb?"PbPb":"PP")) << endl; break; 
 
    case (CtauModel::Delta): 
      
      // create the PDF
      ws.factory(Form("SUM::%s(%s)", Form("pdfCTAU_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")), 
 		      Form("pdfCTAU_ctauRes_%s", (isPbPb?"PbPb":"PP"))
 		      ));
      
      cout << Form("[INFO] %s Background Delta Ctau PDF in %s included", object.c_str(), (isPbPb?"PbPb":"PP")) << endl; break; 
      
    default :
      cout<< "[ERROR] Selected Background Ctau Model has not been implemented"<< endl; return false;

    }
   
  return true;
};


bool addSignalCtauModel(RooWorkspace& ws, string object, CtauModel model, map<string,string> parIni, bool isPbPb) 
{
  cout << Form("[INFO] Implementing %s Signal Ctau Model", object.c_str()) << endl;
   
  switch(model) 
    {  
    case (CtauModel::SingleSidedDecay): 

      if (!( 
            parIni.count(Form("lambdaDSS_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))) 
             )) { 
 	cout << Form("[ERROR] Initial parameters where not found for %s Signal Single Sided Decay Ctau Model in %s", object.c_str(), (isPbPb?"PbPb":"PP")) << endl; return false; 
      }
      
      // create the variables for this model 
      ws.factory( parIni[Form("lambdaDSS_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP"))].c_str() ); 
            
      // create the PDF
      ws.factory(Form("Decay::%s(%s, %s, %s, RooDecay::SingleSided)", Form("pdfCTAU_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")), "ctau", 
 		      Form("lambdaDSS_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")),
 		      Form("pdfCTAU_ctauRes_%s", (isPbPb?"PbPb":"PP"))
 		      ));
 
      cout << Form("[INFO] %s Signal Single Sided Decay Ctau PDF in %s included", object.c_str(), (isPbPb?"PbPb":"PP")) << endl; break; 
 
    case (CtauModel::Delta):

      // create the three PDFs 
      ws.factory(Form("SUM::%s(%s)", Form("pdfCTAU_%s_%s", object.c_str(), (isPbPb?"PbPb":"PP")), 
 		      Form("pdfCTAU_ctauRes_%s", (isPbPb?"PbPb":"PP"))
 		      ));
 		      
      cout << Form("[INFO] %s Signal Delta Ctau PDF in %s included", object.c_str(), (isPbPb?"PbPb":"PP")) << endl; break; 
 
    default :
      cout<< "[ERROR] Selected Signal Ctau Model has not been implemented"<< endl; return false;

    }
   
  return true;
};

void setDefaultParameters(map<string, string> &parIni, bool isPbPb, double numEntries)
{

  cout << "[INFO] Setting user undefined initial parameters to their default values" << endl;

 // CTAU FIT PARAMETERS, TO BE REMOVED SOON
  parIni[Form("ctau1_ctauRes_%s", (isPbPb?"PbPb":"PP"))] = Form("%s[%.4f,%.4f,%.4f]", Form("ctau1_Res_%s", (isPbPb?"PbPb":"PP")), 0., -0.01, 0.01);
  parIni[Form("sigma1_ctauRes_%s", (isPbPb?"PbPb":"PP"))] = Form("%s[%.4f,%.4f,%.4f]", Form("sigma1_ctauRes_%s", (isPbPb?"PbPb":"PP")), 0.8, 0.6, 2.0);
  parIni[Form("ctau2_ctauRes_%s", (isPbPb?"PbPb":"PP"))] = Form("%s[%.4f,%.4f,%.4f]", Form("ctau2_ctauRes_%s", (isPbPb?"PbPb":"PP")), 0., -0.01, 0.01);
  parIni[Form("sigma2_ctauRes_%s", (isPbPb?"PbPb":"PP"))] = Form("%s[%.4f,%.4f,%.4f]", Form("sigma2_ctauRes_%s", (isPbPb?"PbPb":"PP")), 2.3, 1.1, 5.5);
  parIni[Form("f_ctauRes_%s", (isPbPb?"PbPb":"PP"))] = Form("%s[%.4f,%.4f,%.4f]", Form("f_ctauRes_%s", (isPbPb?"PbPb":"PP")), 0.05, 0., 1.);

  parIni[Form("f_JpsiNP_%s", (isPbPb?"PbPb":"PP"))] = Form("%s[%.4f,%.4f,%.4f]", Form("f_JpsiNP_%s", (isPbPb?"PbPb":"PP")), 0.2, 0., 1.);
  parIni[Form("f_Psi2SNP_%s", (isPbPb?"PbPb":"PP"))] = Form("%s[%.4f,%.4f,%.4f]", Form("f_Psi2SNP_%s", (isPbPb?"PbPb":"PP")), 0.2, 0., 1.);
  parIni[Form("lambdaDSS_JpsiNP_%s", (isPbPb?"PbPb":"PP"))] = Form("%s[%.4f,%.4f,%.4f]", Form("lambdaDSS_JpsiNP_%s", (isPbPb?"PbPb":"PP")), 0.8, 0.01, 2.0);
  parIni[Form("lambdaDSS_Psi2SNP_%s", (isPbPb?"PbPb":"PP"))] = Form("%s[%.4f,%.4f,%.4f]", Form("lambdaDSS_Psi2SNP_%s", (isPbPb?"PbPb":"PP")), 0.8, 0.01, 2.0);

  parIni[Form("f_BkgNP_%s", (isPbPb?"PbPb":"PP"))] = Form("%s[%.4f,%.4f,%.4f]", Form("f_BkgNP_%s", (isPbPb?"PbPb":"PP")), 0.3, 0., 1.);
  parIni[Form("lambdaDSS_BkgNP_%s", (isPbPb?"PbPb":"PP"))] = Form("%s[%.4f,%.4f,%.4f]", Form("lambdaDSS_BkgNP_%s", (isPbPb?"PbPb":"PP")), 0.42, 0.05, 1.5);
  parIni[Form("lambdaDF_BkgNP_%s", (isPbPb?"PbPb":"PP"))] = Form("%s[%.4f,%.4f,%.4f]", Form("lambdaDF_BkgNP_%s", (isPbPb?"PbPb":"PP")), 0.79, 0.001, 1.5);
  parIni[Form("lambdaDDS_BkgNP_%s", (isPbPb?"PbPb":"PP"))] = Form("%s[%.4f,%.4f,%.4f]", Form("lambdaDDS_BkgNP_%s", (isPbPb?"PbPb":"PP")), 0.69, 0.02, 5.0);
  parIni[Form("fDFSS_BkgNP_%s", (isPbPb?"PbPb":"PP"))] = Form("%s[%.4f,%.4f,%.4f]", Form("fDFSS_BkgNP_%s", (isPbPb?"PbPb":"PP")), 0.9, 0., 1.);
  parIni[Form("fDLIV_BkgNP_%s", (isPbPb?"PbPb":"PP"))] = Form("%s[%.4f,%.4f,%.4f]", Form("fDLIV_BkgNP_%s", (isPbPb?"PbPb":"PP")), 0.9, 0., 1.);
 
};
