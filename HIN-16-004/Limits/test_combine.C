#include "RooGlobalFunc.h"
#include "RooWorkspace.h"
#include "RooExtendPdf.h"

#include <list>
#include <string>
#include <iostream>

using namespace RooFit;
using namespace std;

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

   RooRealVar *invMass = ws->var("invMass");
   // RooRealVar *invMass = (RooRealVar*) ws->var("invMass")->Clone("invMass");
   // RooRealVar *invMass_pp = ws_pp->var("invMass");
   // add missing binnings
   // list<string> binningNames = invMass_pp->getBinningNames();
   // for (list<string>::const_iterator it=binningNames.begin(); it!=binningNames.end(); it++) {
   //    if (invMass->hasBinning(it->c_str())) continue;
   //    invMass->setBinning(invMass_pp->getBinning(it->c_str()));
   // }

   RooArgSet cols(*invMass);

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

   // build the pbpb pdf
   RooAbsPdf *sig1S = ws->pdf("pdfMASS_Jpsi_PbPb");
   RooAbsPdf *sig2S = ws->pdf("pdfMASS_Psi2S_PbPb");
   RooAbsPdf *pdf_combinedbkgd = ws->pdf("pdfMASS_Bkg_PbPb");
   RooRealVar *nsig1f = ws->var("N_Jpsi_PbPb");
   RooRealVar *nbkgd = ws->var("N_Bkg_PbPb");
   RooRealVar *RFrac2Svs1S_PbPbvsPP = new RooRealVar("RFrac2Svs1S_PbPbvsPP","RFrac2Svs1S_PbPbvsPP",0.5,-10,10);
   RooFormulaVar *nsig2f = new RooFormulaVar("N_Psi2S_PbPb","@0*@1*@2",RooArgList(*RFrac2Svs1S_PbPbvsPP,*RFrac2Svs1S_PP,*nsig1f));

   RooAbsPdf *pdf_pbpb = new RooAddPdf ("pdfMASS_Tot_PbPb","new total p.d.f.",
         RooArgList(*sig1S,*sig2S,*pdf_combinedbkgd),
         RooArgList(*nsig1f,*nsig2f,*nbkgd));
	RooSimultaneous simPdf("simPdf", "simPdf", sample);
	simPdf.addPdf(*pdf_pbpb, "PbPb");
	simPdf.addPdf(*pdf_pp, "PP");

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

   // simPdf.fitTo(data_combo); // crashes sometimes but not always?? adding Range("MassWindow") or NumCPU(2) improves stability
   simPdf.fitTo(data_combo,NumCPU(3),Range("MassWindow"));

   // wcombo->writeToFile("fitresult_combo.root");
   return wcombo;
}
