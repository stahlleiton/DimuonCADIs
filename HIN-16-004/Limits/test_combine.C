#include "RooGlobalFunc.h"
#include "RooWorkspace.h"
#include "RooExtendPdf.h"

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
   data_pp =(RooDataSet *) ws_pp->data("dOS_DATA_PP");

   // make the combined dataset
	RooCategory sample("sample", "sample");
	sample.defineType("PbPb");
	sample.defineType("PP");

   RooRealVar *pt = ws->var("pt");
   RooRealVar *rap = ws->var("rap");
   RooRealVar *cent = ws->var("cent");
   RooRealVar *ctau = ws->var("ctau");
   RooRealVar *ctauErr = ws->var("ctauErr");
   RooRealVar *invMass = ws->var("invMass");

	RooArgSet cols(*invMass,*ctau,*ctauErr,*pt,*rap,*cent);

	RooDataSet data_combo("dOS_DATA", "dOS_DATA", cols, RooFit::Index(sample),
/*Only for track rotation*/
		RooFit::Import("PbPb", *data), RooFit::Import("PP", *data_pp));

   RooWorkspace *wcombo = new RooWorkspace("wcombo","workspace for PbPb + pp");
   wcombo->import(data_combo);

   // build the pp pdf
   // pdf_pp = ws_pp->pdf("pdfMASS_Tot_PP");
   RooAbsPdf *sig1S_PP = ws_pp->pdf("pdfMASS_Jpsi_PP");
   RooAbsPdf *sig2S_PP = ws_pp->pdf("pdfMASS_Psi2S_PP");
   RooAbsPdf *pdf_combinedbkgd_PP = ws_pp->pdf("pdfMASS_Bkg_PP");
   RooRealVar *nsig1f_PP = ws_pp->var("N_Jpsi_PP");
   RooRealVar *RFrac2Svs1S_PP = ws_pp->var("RFrac2Svs1S_PP");
   RooRealVar *nbkgd_PP = ws_pp->var("N_Bkg_PP");
   RooFormulaVar *nsig2f_PP = new RooFormulaVar("N_Psi2S_PP","@0*@1",RooArgList(*RFrac2Svs1S_PP,*nsig1f_PP));
   cout << sig1S_PP << " " << sig2S_PP << " " << pdf_combinedbkgd_PP << " " << nsig1f_PP << " " << nbkgd_PP << " " << RFrac2Svs1S_PP << endl;

   pdf_pp = new RooAddPdf ("pdfMASS_Tot_PP","new total p.d.f. PP",
         RooArgList(*sig1S_PP,*sig2S_PP,*pdf_combinedbkgd_PP),
         RooArgList(*nsig1f_PP,*nsig2f_PP,*nbkgd_PP));



   cout << "******** PP ********" << endl;
   cout << "1S+2S+bkg = " << nsig1f_PP->getVal() << "+" << nsig2f_PP->getVal() << "+" << nbkgd_PP->getVal() << " = " << nsig1f_PP->getVal()+nsig2f_PP->getVal()+nbkgd_PP->getVal()<< endl;
   cout << "data = " << data_pp->numEntries() << endl;
   cout << "********************" << endl;

   // build the pbpb pdf
   RooAbsPdf *sig1S = ws->pdf("pdfMASS_Jpsi_PbPb");
   RooAbsPdf *sig2S = ws->pdf("pdfMASS_Psi2S_PbPb");
   RooAbsPdf *pdf_combinedbkgd = ws->pdf("pdfMASS_Bkg_PbPb");
   RooRealVar *nsig1f = ws->var("N_Jpsi_PbPb");
   RooRealVar *nbkgd = ws->var("N_Bkg_PbPb");
   RooRealVar *RFrac2Svs1S_PbPbvsPP = new RooRealVar("RFrac2Svs1S_PbPbvsPP","RFrac2Svs1S_PbPbvsPP",0.5,-10,10);
   // RooFormulaVar *RFrac2Svs1S_PbPb = new RooFormulaVar("RFrac2Svs1S_PbPb","@0*@1",RooArgList(*RFrac2Svs1S_PbPbvsPP,*RFrac2Svs1S_PP));
   RooFormulaVar *nsig2f = new RooFormulaVar("N_Psi2S_PbPb","@0*@1*@2",RooArgList(*RFrac2Svs1S_PbPbvsPP,*RFrac2Svs1S_PP,*nsig1f));
   // RooExtendPdf *sig1S_tot = new RooExtendPdf("pdfMASSTot_Jpsi_PbPb","pdfMASSTot_Jpsi_PbPb",*sig1S,*nsig1f);
   // RooExtendPdf *sig2S_tot = new RooExtendPdf("pdfMASSTot_Psi2S_PbPb","pdfMASSTot_Psi2S_PbPb",*sig2S,*nsig2f);
   // RooExtendPdf *pdf_combinedbkgd_tot = new RooExtendPdf("pdfMASSTot_Bkg_PbPb","pdfMASSTot_Bkg_PbPb",*pdf_combinedbkgd,*nbkgd);
   
   cout << "******** PbPb ********" << endl;
   cout << "1S+2S+bkg = " << nsig1f->getVal() << "+" << nsig2f->getVal() << "+" << nbkgd->getVal() << " = " << nsig1f->getVal()+nsig2f->getVal()+nbkgd->getVal()<< endl;
   cout << "data = " << data->numEntries() << endl;
   cout << "********************" << endl;

   RooAbsPdf *pdf_pbpb = new RooAddPdf ("pdfMASS_Tot_PbPb","new total p.d.f.",
         RooArgList(*sig1S,*sig2S,*pdf_combinedbkgd),
         RooArgList(*nsig1f,*nsig2f,*nbkgd));
	RooSimultaneous simPdf("simPdf", "simPdf", sample);
	simPdf.addPdf(*pdf_pbpb, "PbPb");
	simPdf.addPdf(*pdf_pp, "PP");

   // // not sure this is really needed since we will fit again in the later workspace creation
   // RooFitResult* fit_2nd;// fit results
   // fit_2nd = simPdf.fitTo(data_combo,
   //       // RooFit::Constrained(),
   //       RooFit::Save(kTRUE),
   //       RooFit::Extended(kTRUE),
   //       // RooFit::Minos(kTRUE),
   //       RooFit::NumCPU(2));

   cout << "*********POST-FIT**********" << endl;
   cout << "******** PP ********" << endl;
   cout << "1S+2S+bkg = " << nsig1f_PP->getVal() << "+" << nsig2f_PP->getVal() << "+" << nbkgd_PP->getVal() << " = " << nsig1f_PP->getVal()+nsig2f_PP->getVal()+nbkgd_PP->getVal()<< endl;
   cout << "******** PbPb ********" << endl;
   cout << "1S+2S+bkg = " << nsig1f->getVal() << "+" << nsig2f->getVal() << "+" << nbkgd->getVal() << " = " << nsig1f->getVal()+nsig2f->getVal()+nbkgd->getVal()<< endl;
   cout << "data = " << data_combo.numEntries() << endl;
   cout << "********************" << endl;

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

   // import PDFs to the workspace
   if (!commonParams) {
      wcombo->import(*pdf_pp,	RecycleConflictNodes());
   } else {
      wcombo->import(*pdf_pp,	RecycleConflictNodes(), RenameVariable("f_Jpsi_PP","f_Jpsi"), RenameVariable("m_Jpsi_PP","m_Jpsi"), RenameVariable("sigma1_Jpsi_PP","sigma1_Jpsi"), RenameVariable("rSigma21_Jpsi_PP","rSigma21_Jpsi"));
   }
   if (!commonParams) {
      wcombo->import(*pdf_pbpb, RecycleConflictNodes());
   } else {
      wcombo->import(*pdf_pbpb,	RecycleConflictNodes(), RenameVariable("f_Jpsi_PbPb","f_Jpsi"), RenameVariable("m_Jpsi_PbPb","m_Jpsi"), RenameVariable("sigma1_Jpsi_PbPb","sigma1_Jpsi"), RenameVariable("rSigma21_Jpsi_PbPb","rSigma21_Jpsi"));
   }
	wcombo->import(simPdf, RecycleConflictNodes());

   wcombo->writeToFile("fitresult_combo.root");
   return wcombo;
}
