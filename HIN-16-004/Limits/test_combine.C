#include "RooGlobalFunc.h"
#include "RooWorkspace.h"

using namespace RooFit;

RooWorkspace* test_combine(const char* name_pbpb="fitresult.root", const char* name_pp="fitresult_pp.root", bool commonParams=false)
{
   // const char *poiname="N_{#Upsilon(3S)}";
   TFile *f = new TFile(name_pbpb) ;
   TFile *f_pp = new TFile(name_pp) ;

   // Retrieve workspace from file
   RooWorkspace* ws = (RooWorkspace*) f->Get("workspace");
   RooWorkspace* ws_pp = (RooWorkspace*) f_pp->Get("workspace");

   // RooRealVar *theVar; 
   RooDataSet *data; RooAbsPdf *pdf;
   RooRealVar *theVar_pp; RooDataSet *data_pp; RooAbsPdf *pdf_pp;

   // theVar = ws->var(poiname);
   // pdf = ws->pdf("pdf");
   data =(RooDataSet *) ws->data("dOS_DATA_PbPb");
   pdf_pp = ws_pp->pdf("pdfMASS_Tot_PP");
   data_pp =(RooDataSet *) ws_pp->data("dOS_DATA_PP");

	RooCategory dataCat("dataCat", "dataCat");
	dataCat.defineType("PbPb");
	dataCat.defineType("PP");

   RooRealVar *pt = ws->var("pt");
   RooRealVar *rap = ws->var("rap");
   RooRealVar *cent = ws->var("cent");
   RooRealVar *ctau = ws->var("ctau");
   RooRealVar *ctauErr = ws->var("ctauErr");
   RooRealVar *invMass = ws->var("invMass");

	RooArgSet cols(*invMass,*ctau,*ctauErr,*pt,*rap,*cent);

	RooDataSet data_combo("dOS_DATA", "dOS_DATA", cols, RooFit::Index(dataCat),
/*Only for track rotation*/
		RooFit::Import("PbPb", *data), RooFit::Import("PP", *data_pp));

   RooWorkspace *wcombo = new RooWorkspace("wcombo","workspace for PbPb + pp");
   wcombo->import(data_combo);
   if (!commonParams) {
      wcombo->import(*pdf_pp,	RecycleConflictNodes());
   } else {
      wcombo->import(*pdf_pp,	RecycleConflictNodes(), RenameVariable("f_Jpsi_PP","f_Jpsi"), RenameVariable("m_Jpsi_PP","m_Jpsi"), RenameVariable("sigma1_Jpsi_PP","sigma1_Jpsi"), RenameVariable("rSigma21_Jpsi_PP","rSigma21_Jpsi"));
   }

   // // create the combined variable
   // RooRealVar* n3shi = wcombo->var("N_{#Upsilon(3S)}_hi");
   // RooRealVar* n3spp = wcombo->var("N_{#Upsilon(3S)}_pp");
   // RooFormulaVar x3raw("x3raw","x3raw","@0/@1",RooArgList(*n3shi,*n3spp));
   // cout << x3raw.getVal() << endl;
   // wcombo->import(x3raw);
   // wcombo->Print();

   RooAbsPdf *sig1S = ws->pdf("pdfMASS_Jpsi_PbPb");
   RooAbsPdf *sig2S = ws->pdf("pdfMASS_Psi2S_PbPb");
   RooAbsPdf *pdf_combinedbkgd = ws->pdf("pdfMASS_Bkg_PbPb");
   RooRealVar *nsig1f = ws->var("N_Jpsi_PbPb");
   RooRealVar *RFrac2Svs1S_PP = ws_pp->var("RFrac2Svs1S_PP");
   RooRealVar *nbkgd = ws->var("N_Bkg_PbPb");
   RooRealVar *doubleRatio = new RooRealVar("doubleRatio","doubleRatio",0.5,-10,10);
   RooFormulaVar *RFrac2Svs1S_PbPb = new RooFormulaVar("RFrac2Svs1S_PbPb","@0*@1",RooArgList(*doubleRatio,*RFrac2Svs1S_PP));
   RooFormulaVar *nsig2f = new RooFormulaVar("N_Psi2S_PbPb","@0*@1",RooArgList(*RFrac2Svs1S_PbPb,*nsig1f));

   RooAbsPdf *pdf_pbpb = new RooAddPdf ("pdfMASS_Tot_PbPb","new total p.d.f.",
         RooArgList(*sig1S,*sig2S,*pdf_combinedbkgd),
         RooArgList(*nsig1f,*nsig2f,*nbkgd));
   if (!commonParams) {
      wcombo->import(*pdf_pbpb, RecycleConflictNodes());
   } else {
      wcombo->import(*pdf_pbpb,	RecycleConflictNodes(), RenameVariable("f_Jpsi_PbPb","f_Jpsi"), RenameVariable("m_Jpsi_PbPb","m_Jpsi"), RenameVariable("sigma1_Jpsi_PbPb","sigma1_Jpsi"), RenameVariable("rSigma21_Jpsi_PbPb","rSigma21_Jpsi"));
   }
   wcombo->Print();
	RooSimultaneous simPdf("pdfMASS_Tot", "pdfMASS_Tot", dataCat);
	simPdf.addPdf(*pdf_pbpb, "PbPb");
	simPdf.addPdf(*pdf_pp, "PP");
	wcombo->import(simPdf, RecycleConflictNodes());
   wcombo->Print();

   // not sure this is really needed since we will fit again in the later workspace creation
   // RooFitResult* fit_2nd;// fit results
   // fit_2nd = simPdf.fitTo(data_combo,
   //       // RooFit::Constrained(),
   //       RooFit::Save(kTRUE),
   //       RooFit::Extended(kTRUE),
   //       RooFit::Minos(kTRUE),
   //       RooFit::NumCPU(2));


   // fix all other variables in model:
   // everything except observables, POI, and nuisance parameters
   // must be constant
   // wcombo->var("#alpha_{CB}_hi")->setConstant(true);
   // wcombo->var("#alpha_{CB}_pp")->setConstant(true);
   // wcombo->var("#sigma_{CB1}_hi")->setConstant(true);
   // wcombo->var("#sigma_{CB1}_pp")->setConstant(true);
   // wcombo->var("#sigma_{CB2}/#sigma_{CB1}_hi")->setConstant(true);
   // wcombo->var("#sigma_{CB2}/#sigma_{CB1}_pp")->setConstant(true);
   // wcombo->var("N_{#Upsilon(1S)}_hi")->setConstant(true);
   // wcombo->var("N_{#Upsilon(1S)}_pp")->setConstant(true);
   // wcombo->var("N_{#Upsilon(2S)}_hi")->setConstant(true);
   // wcombo->var("N_{#Upsilon(2S)}_pp")->setConstant(true);
   // wcombo->var("N_{#Upsilon(3S)}_pp")->setConstant(true);
   // wcombo->var("decay_hi")->setConstant(true);
   // wcombo->var("decay_pp")->setConstant(true);
   // wcombo->var("mass1S_hi")->setConstant(true);
   // wcombo->var("mass1S_pp")->setConstant(true);
   // wcombo->var("n_{Bkgd}_hi")->setConstant(true);
   // wcombo->var("n_{Bkgd}_pp")->setConstant(true);
   // wcombo->var("npow")->setConstant(true);
   // wcombo->var("sigmaFraction_hi")->setConstant(true);
   // wcombo->var("sigmaFraction_pp")->setConstant(true);
   // wcombo->var("turnOn_hi")->setConstant(true);
   // wcombo->var("turnOn_pp")->setConstant(true);
   // wcombo->var("width_hi")->setConstant(true);
   // wcombo->var("width_pp")->setConstant(true);

   wcombo->writeToFile("fitresult_combo.root");
   return wcombo;
}
