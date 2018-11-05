#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif
#include "TCanvas.h"
#include "RooWorkspace.h"
#include "RooAbsReal.h"
#include "RooCategory.h"
#include "RooAddPdf.h"
#include "RooProdPdf.h"
#include "RooSimultaneous.h"
#include "RooGaussian.h"
#include "RooGenericPdf.h"
#include "RooSimultaneous.h"
#include "RooMinuit.h"
#include "RooRealVar.h"
#include "RooArgSet.h"
#include "RooArgList.h"
#include "RooDataSet.h"
#include "RooPlot.h"
#include "RooStats/BayesianCalculator.h"
#include "RooStats/ModelConfig.h"
#include "RooStats/SimpleInterval.h"
#include "TAxis.h"
#include <iostream>
#include <sstream>
#include <TString.h>
#include <TH1F.h>
#include <TTree.h>
#include <TFile.h>
#include <TChain.h>
#include <TNtuple.h>
#include <TLegend.h>
#include <TCanvas.h>
#include <TLine.h>
#include <TROOT.h>
#include <fstream>
#include <TGraph.h>
#include "TMath.h"
#include "TF1.h"
#include "RooFitResult.h"
#include "TSystem.h"
#include "RooWorkspace.h"
#include "RooExtendPdf.h"
#include "TFile.h"
#include "RooDataSet.h"
#include "RooRealVar.h"
#include "RooCategory.h"
#include "RooAddPdf.h"
#include "RooSimultaneous.h"

#include <list>
#include <string>
#include <iostream>

using namespace RooFit;
using namespace std;

double scalemass(double m) {return m>2.2 ? 2.2 + ((4.5-2.2)/(4.2-2.2))*(m-2.2) : m*(3231.4534/3096.916);};
void scalemass(RooRealVar* m, bool scaleall=true) {
   if (!m) return;
   double oldm = m->getVal();
   double newm = scalemass(oldm);
   if (scaleall) {
      m->setRange((newm/oldm)*m->getMin(),(newm/oldm)*m->getMax());
      m->setError((newm/oldm)*m->getError());
   }
   m->setVal(newm);
};

void pvalue_16004vs12007_rescale(const char* name_16004, const char* name_12007, const char* name_out="output.root", bool commonParams=false, int nCPU=2) {
   // RooMsgService::instance().addStream(RooFit::MsgLevel::DEBUG); 
   TFile *f16004 = TFile::Open(name_16004) ;
   TFile *f12007 = TFile::Open(name_12007) ;
   if (!f16004 || !f12007 || !f16004->IsOpen() || !f12007->IsOpen()) return;

   // Retrieve workspace from file
   RooWorkspace* ws16004 = (RooWorkspace*) f16004->Get("workspace");
   RooWorkspace* ws12007 = (RooWorkspace*) f12007->Get("workspace");

   ws16004->loadSnapshot("SbHypo_poiAndNuisance_snapshot");

   RooDataSet *data16004=NULL;
   RooDataSet *data12007=NULL;

   // let's massively clone everything into the new workspace
   RooWorkspace *wcombo = new RooWorkspace("workspace","workspace for 16004 + 12007");

   // category
   RooCategory *sample = ws16004->cat("sample");
   sample->defineType("HI020");
   sample->defineType("HI2040");
   sample->defineType("HI40100");
   sample->defineType("pp");
   sample->defineType("PP_pass");
   sample->defineType("PP_fail");
   sample->defineType("PbPb_pass");
   sample->defineType("PbPb_fail");
   wcombo->import(*sample);

   // first, variables
   RooArgSet allVars = ws16004->allVars(); allVars.add(ws12007->allVars());
   TIterator* it = allVars.createIterator();
   RooRealVar *theVar = (RooRealVar*) it->Next();
   while (theVar) {
      TString tname(theVar->GetName());
      // cout << tname << " " << theVar->isConstant() << endl;
      if (tname == "Jpsi_Mass") theVar->SetName("invMass");
      if (
            tname == "meanSig1"
            || tname == "meanSig1_HI"
            || tname == "sigmaSig1"
            || tname == "sigmaSig1_HI"
         ) {
         cout << theVar->GetName() << " " << theVar->getVal();
         scalemass(theVar);
         cout << " --> " << theVar->getVal() << endl;
      }
      if (tname == "RFrac2Svs1S_PbPbvsPP_P") theVar->setRange(-10,10);
      if (tname == "RFrac2Svs1S_PbPb_fail") theVar->setRange(theVar->getMin()-1,theVar->getMax()+1);
      if (tname.Contains("xi_")) { // 12-007 systematics
         theVar->setVal(0);
         theVar->setError(1);
         theVar->setConstant(kTRUE);
         theVar->setRange(-5,5);
      }
      wcombo->import(*theVar);
      theVar = (RooRealVar*) it->Next();
   }
   // return;

   // then, the formulae.
   RooArgSet allFunctions = ws16004->allFunctions(); allFunctions.add(ws12007->allFunctions());
   it = allFunctions.createIterator();
   RooFormulaVar *theFunc = (RooFormulaVar*) it->Next();
   while (theFunc) {
      TString tfname(theFunc->GetName());
      if (!(tfname=="meanSig1P"
            || tfname=="meanSig1P_HI"
            || tfname=="sigmaSig1P"
            || tfname=="sigmaSig1P_HI"
            || tfname=="sigmaSig2P"
            || tfname=="sigmaSig2P_HI"))
         wcombo->import(*theFunc,RecycleConflictNodes());
      theFunc = (RooFormulaVar*) it->Next();
   }
   wcombo->factory("expr::meanSig1P('@0*1.20967',meanSig1)"); // correct for the mass rescaling
   wcombo->factory("expr::meanSig1P_HI('@0*1.20967',meanSig1_HI)"); // correct for the mass rescaling
   wcombo->factory("expr::sigmaSig1P('@0*1.20967',sigmaSig1)"); // correct for the mass rescaling
   wcombo->factory("expr::sigmaSig1P_HI('@0*1.20967',sigmaSig1_HI)"); // correct for the mass rescaling
   wcombo->factory("expr::sigmaSig2P('@0*1.20967',sigmaSig2)"); // correct for the mass rescaling
   wcombo->factory("expr::sigmaSig2P_HI('@0*1.20967',sigmaSig2_HI)"); // correct for the mass rescaling

   // then, import all pdf's
   RooArgSet allPdfs = ws16004->allPdfs();
   it = allPdfs.createIterator();
   RooAbsPdf* thePdf = (RooAbsPdf*) it->Next();
   while (thePdf) {
      // if (TString(thePdf->ClassName()) == "RooAddPdf") ((RooAddPdf*) thePdf)->fixCoefRange("fit_16004");
      wcombo->import(*thePdf,RecycleConflictNodes());
      thePdf = (RooAbsPdf*) it->Next();
   }
   RooArgSet allPdfs2 = ws12007->allPdfs();
   it = allPdfs2.createIterator();
   thePdf = (RooAbsPdf*) it->Next();
   while (thePdf) {
      // if (TString(thePdf->ClassName()) == "RooAddPdf") ((RooAddPdf*) thePdf)->fixCoefRange("fit_12007");
      wcombo->import(*thePdf,RecycleConflictNodes());
      thePdf = (RooAbsPdf*) it->Next();
   }

   // now, data
   list<RooAbsData*> alldata = ws16004->allData();
   list<RooAbsData*> alldata2 = ws12007->allData();
   alldata.insert(alldata.end(),alldata2.begin(),alldata2.end());
   for (list<RooAbsData*>::const_iterator it=alldata.begin(); it!=alldata.end(); it++) {
       RooAbsData *dat = *it;
       TString datname(dat->GetName());
       RooRealVar* mass = (RooRealVar*) dat->get()->find("Jpsi_Mass");
       if (mass) mass->SetName("invMass");
       if (datname=="data1" || datname=="data2" || datname=="data3" || datname=="data4") {
          RooDataSet *newdat = new RooDataSet(datname,datname,RooArgSet(*(wcombo->var("invMass"))));
          for (int i=0; i<dat->numEntries(); i++) {
             RooRealVar* mass_i = (RooRealVar*) dat->get(i)->find("invMass");
             mass_i->setRange(2.2,4.5);
             scalemass(mass_i,false);
             newdat->add(RooArgSet(*mass_i));
          }
          dat = newdat;
       }
       wcombo->import(*dat);
   }

   RooDataSet *redDataSim2 = new RooDataSet("redDataSim2",
         "redDataSim2",
         RooArgSet(*(wcombo->var("invMass"))),
         Index(*(wcombo->cat("sample"))),
         Import("HI020",*((RooDataSet*) wcombo->data("data1"))),
         Import("HI2040",*((RooDataSet*) wcombo->data("data2"))),
         Import("HI40100",*((RooDataSet*) wcombo->data("data3"))),
         Import("pp",*((RooDataSet*) wcombo->data("data4")))
         );
   wcombo->import(*redDataSim2);

   RooDataSet *datacombo = (RooDataSet*) wcombo->data("redDataSim2")->Clone("datacombo");
   datacombo->append(*((RooDataSet*) wcombo->data("dOS_DATA")));
   wcombo->import(*datacombo);


   // let's build proper constrains for the systs of 12-007. Use log-normal!
   wcombo->factory( "expr::alpha_b_HI020('pow(1+sigma_b,xi_b_HI020)',sigma_b,xi_b_HI020)" );
   wcombo->factory( "Gaussian::constr_b_HI020(xi_b_HI020,glob_b_HI020[0,-5,5],1)" );
   wcombo->factory( "expr::alpha_b_HI2040('pow(1+sigma_b,xi_b_HI2040)',sigma_b,xi_b_HI2040)" );
   wcombo->factory( "Gaussian::constr_b_HI2040(xi_b_HI2040,glob_b_HI2040[0,-5,5],1)" );
   wcombo->factory( "expr::alpha_b_HI40100('pow(1+sigma_b,xi_b_HI40100)',sigma_b,xi_b_HI40100)" );
   wcombo->factory( "Gaussian::constr_b_HI40100(xi_b_HI40100,glob_b_HI40100[0,-5,5],1)" );
   wcombo->factory( "expr::alpha_eff_HI020('pow(1+sigma_eff_HI020,xi_eff_HI020)',sigma_eff_HI020,xi_eff_HI020)" );
   wcombo->factory( "Gaussian::constr_eff_HI020(xi_eff_HI020,glob_eff_HI020[0,-5,5],1)" );
   wcombo->factory( "expr::alpha_eff_HI2040('pow(1+sigma_eff_HI2040,xi_eff_HI2040)',sigma_eff_HI2040,xi_eff_HI2040)" );
   wcombo->factory( "Gaussian::constr_eff_HI2040(xi_eff_HI2040,glob_eff_HI2040[0,-5,5],1)" );
   wcombo->factory( "expr::alpha_eff_HI40100('pow(1+sigma_eff_HI40100,xi_eff_HI40100)',sigma_eff_HI40100,xi_eff_HI40100)" );
   wcombo->factory( "Gaussian::constr_eff_HI40100(xi_eff_HI40100,glob_eff_HI40100[0,-5,5],1)" );
   wcombo->factory( "expr::alpha_fit_HI020('pow(1+sigma_fit_HI020,xi_fit_HI020)',sigma_fit_HI020,xi_fit_HI020)" );
   wcombo->factory( "Gaussian::constr_fit_HI020(xi_fit_HI020,glob_fit_HI020[0,-5,5],1)" );
   wcombo->factory( "expr::alpha_fit_HI2040('pow(1+sigma_fit_HI2040,xi_fit_HI2040)',sigma_fit_HI2040,xi_fit_HI2040)" );
   wcombo->factory( "Gaussian::constr_fit_HI2040(xi_fit_HI2040,glob_fit_HI2040[0,-5,5],1)" );
   wcombo->factory( "expr::alpha_fit_HI40100('pow(1+sigma_fit_HI40100,xi_fit_HI40100)',sigma_fit_HI40100,xi_fit_HI40100)" );
   wcombo->factory( "Gaussian::constr_fit_HI40100(xi_fit_HI40100,glob_fit_HI40100[0,-5,5],1)" );


   // now let's redefine things in terms of a Quadruple Ratio (the ratio of double ratios at 2.76 and 5.02 TeV)
   // first define the quadruple ratio
   RooRealVar *QR = new RooRealVar("QR","quadruple ratio",1,-1,20);
   QR->setVal(wcombo->function("doubleRatio_HI0100")->getVal() / wcombo->var("RFrac2Svs1S_PbPbvsPP_P")->getVal());
   cout << QR->getVal() << endl;
   wcombo->import(*QR);

   // now the double ratio at 2.76TeV
   RooFormulaVar *DR12007 = new RooFormulaVar("doubleRatio_HI0100new","@0*@1",RooArgList(*QR,*wcombo->var("RFrac2Svs1S_PbPbvsPP_P")));
   wcombo->import(*DR12007,RecycleConflictNodes());

   // the inclusive number of psi2S in pbpb at 2.76TeV
   RooFormulaVar *NPsiP_HI0100new = new RooFormulaVar("NPsiP_HI0100new","@0*@1*@2",RooArgList(*DR12007,*wcombo->function("NJpsi_HI0100"),*wcombo->var("fracP_pp")));
   wcombo->import(*NPsiP_HI0100new,RecycleConflictNodes());

   // the double ratios with systematics
   RooFormulaVar *NPsiP_HI2040syst = new RooFormulaVar("NPsiP_HI2040syst","@0*@1*@2*@3*@4",RooArgList(*wcombo->function("NJpsi_HI2040"),*wcombo->function("fracP_HI2040"),*wcombo->function("alpha_b_HI2040"),*wcombo->function("alpha_eff_HI2040"),*wcombo->function("alpha_fit_HI2040")));
   wcombo->import(*NPsiP_HI2040syst);
   RooFormulaVar *NPsiP_HI40100syst = new RooFormulaVar("NPsiP_HI40100syst","@0*@1*@2*@3*@4",RooArgList(*wcombo->function("NJpsi_HI40100"),*wcombo->function("fracP_HI40100"),*wcombo->function("alpha_b_HI40100"),*wcombo->function("alpha_eff_HI40100"),*wcombo->function("alpha_fit_HI40100")));
   wcombo->import(*NPsiP_HI40100syst);

   // the number of psi2s for 0-20
   RooFormulaVar *NPsiP_HI020new = new RooFormulaVar("NPsiP_HI020new","(@0-@1-@2)*@3*@4*@5",RooArgList(*NPsiP_HI0100new,*wcombo->function("NPsiP_HI2040syst"),*wcombo->function("NPsiP_HI40100syst"),*wcombo->function("alpha_b_HI020"),*wcombo->function("alpha_eff_HI020"),*wcombo->function("alpha_fit_HI020")));
   wcombo->import(*NPsiP_HI020new,RecycleConflictNodes());

   // now, rewire the pdfs that need to be rewired
   wcombo->factory("SUM::pdf_HI020new(NJpsi_HI020 * sigCB1G2_HI, NPsiP_HI020new * sigCB1G2P_HI, NBkg_HI020 * pol3_HI020)");
   wcombo->factory("SUM::pdf_HI2040new(NJpsi_HI2040 * sigCB1G2_HI, NPsiP_HI2040syst * sigCB1G2P_HI, NBkg_HI2040 * pol2_HI2040)");
   wcombo->factory("SUM::pdf_HI40100new(NJpsi_HI40100 * sigCB1G2_HI, NPsiP_HI40100syst * sigCB1G2P_HI, NBkg_HI40100 * pol1_HI40100)");

   wcombo->factory("PROD::sigMassPDF_HI020new(pdf_HI020new, constr_fit_HI020, constr_eff_HI020, constr_b_HI020)");
   wcombo->factory("PROD::sigMassPDF_HI2040new(pdf_HI2040new, constr_fit_HI2040, constr_eff_HI2040, constr_b_HI2040)");
   wcombo->factory("PROD::sigMassPDF_HI40100new(pdf_HI40100new, constr_fit_HI40100, constr_eff_HI40100, constr_b_HI40100)");
   
   // wcombo->factory("SIMUL::theBigSimPdf_tmp(sample,HI020=sigMassPDF_HI020new,HI2040=sigMassPDF_HI2040,HI40100=sigMassPDF_HI40100,pp=sigMassPDF_pp,PP_fail=pdfMASS_Tot_PP_fail, PP_pass=pdfMASS_Tot_PP_pass, PbPb_fail=pdfMASS_Tot_PbPb_fail, PbPb_pass=pdfMASS_Tot_PbPb_pass_syst)");
   wcombo->factory("SIMUL::theBigSimPdf(sample,HI020=sigMassPDF_HI020new,HI2040=sigMassPDF_HI2040new,HI40100=sigMassPDF_HI40100new,pp=sigMassPDF_pp,PP_fail=pdfMASS_Tot_PP_fail, PP_pass=pdfMASS_Tot_PP_pass, PbPb_fail=pdfMASS_Tot_PbPb_fail, PbPb_pass=pdfMASS_Tot_PbPb_pass_constr)");
   // wcombo->factory("SIMUL::theBigSimPdf_tmp(sample,HI020=sigMassPDF_HI020new,HI2040=sigMassPDF_HI2040,HI40100=sigMassPDF_HI40100,pp=sigMassPDF_pp)");
   // wcombo->factory("PROD::theBigSimPdf(theBigSimPdf_tmp, constr_syst)");


   // now... let's FIT
   // after having fixed a few variables
   // wcombo->var("coeffGaus")->setConstant(kTRUE);
   // wcombo->var("coeffGaus_HI")->setConstant(kTRUE);
   // wcombo->var("coeffPol1")->setConstant(kTRUE);
   // wcombo->var("coeffPol1_HI020")->setConstant(kTRUE);
   // wcombo->var("coeffPol1_HI2040")->setConstant(kTRUE);
   // wcombo->var("coeffPol1_HI40100")->setConstant(kTRUE);
   // wcombo->var("coeffPol2_HI020")->setConstant(kTRUE);
   // wcombo->var("coeffPol2_HI2040")->setConstant(kTRUE);
   // wcombo->var("coeffPol3_HI020")->setConstant(kTRUE);
   // wcombo->var("meanSig1")->setConstant(kTRUE);
   // wcombo->var("meanSig1_HI")->setConstant(kTRUE);
   // wcombo->var("sigmaSig1")->setConstant(kTRUE);
   // wcombo->var("sigmaSig1_HI")->setConstant(kTRUE);
   // wcombo->var("wideFactor")->setConstant(kTRUE);

   // wcombo->var("NBkg_HI020")->setConstant(kTRUE);
   // wcombo->var("NBkg_HI2040")->setConstant(kTRUE);
   // wcombo->var("NBkg_HI40100")->setConstant(kTRUE);
   // wcombo->var("NBkg_pp")->setConstant(kTRUE);
   // wcombo->var("NJpsi_HI020")->setConstant(kTRUE);
   // wcombo->var("NJpsi_HI2040")->setConstant(kTRUE);
   // wcombo->var("NJpsi_HI40100")->setConstant(kTRUE);
   // wcombo->var("NJpsi_pp")->setConstant(kTRUE);
   // wcombo->var("N_Bkg_PP_pass")->setConstant(kTRUE);
   // wcombo->var("N_Bkg_PbPb_pass")->setConstant(kTRUE);
   // wcombo->var("N_Jpsi_PP_pass")->setConstant(kTRUE);
   // wcombo->var("N_Jpsi_PbPb_pass")->setConstant(kTRUE);
   wcombo->var("QR")->setConstant(kTRUE);
   wcombo->var("RFrac2Svs1S_PP_fail")->setConstant(kTRUE);
   wcombo->var("RFrac2Svs1S_PP_pass")->setConstant(kTRUE);
   wcombo->var("RFrac2Svs1S_PbPb_fail")->setConstant(kTRUE);
   wcombo->var("RFrac2Svs1S_PbPbvsPP_P")->setConstant(kTRUE);
   wcombo->var("doubleRatio_HI2040")->setConstant(kTRUE);
   wcombo->var("doubleRatio_HI40100")->setConstant(kTRUE);
   wcombo->var("fracP_pp")->setConstant(kTRUE);

   wcombo->var("beta_syst")->setConstant(kTRUE);
   wcombo->var("glob_b_HI020")->setConstant(kTRUE);
   wcombo->var("glob_b_HI2040")->setConstant(kTRUE);
   wcombo->var("glob_b_HI40100")->setConstant(kTRUE);
   wcombo->var("glob_eff_HI020")->setConstant(kTRUE);
   wcombo->var("glob_eff_HI2040")->setConstant(kTRUE);
   wcombo->var("glob_eff_HI40100")->setConstant(kTRUE);
   wcombo->var("glob_fit_HI020")->setConstant(kTRUE);
   wcombo->var("glob_fit_HI2040")->setConstant(kTRUE);
   wcombo->var("glob_fit_HI40100")->setConstant(kTRUE);

   wcombo->var("sigma_pol")->setVal(1e-6); // originally 0 but gives errors

   // wcombo->Print();
   // return;

   RooAbsPdf *simPdf = wcombo->pdf("theBigSimPdf");
   RooFitResult *fitM;
   fitM = simPdf->fitTo(*datacombo,Extended(1),Hesse(1),Minos(0),Save(1),NumCPU(nCPU));
   // fitM = simPdf->fitTo(*redDataSim2,Extended(1),Hesse(1),Minos(0),Save(1),NumCPU(nCPU),Range("fit"),SplitRange(),SumCoefRange("fit_HI020"));
   // fitM = simPdf->fitTo(*redDataSim2,Extended(1),Hesse(1),Minos(0),Save(1),NumCPU(nCPU),PrintEvalErrors(0),Range("fit_HI020"),SumCoefRange("fit_HI020"));
   fitM->printMultiline(cout,1,1,"");

   RooPlot *p = wcombo->var("invMass")->frame();
   wcombo->data("data4")->plotOn(p);
   wcombo->pdf("sigMassPDF_pp")->plotOn(p);
   TCanvas *c1 = new TCanvas();
   p->Draw();
   c1->SaveAs("c1.pdf");


   // Good. Now let's go to stuff limits for the hypothesis testing.
   // first unfix variables

   wcombo->var("coeffGaus")->setConstant(kTRUE);
   wcombo->var("coeffGaus_HI")->setConstant(kTRUE);
   wcombo->var("coeffPol1")->setConstant(kTRUE);
   wcombo->var("coeffPol1_HI020")->setConstant(kTRUE);
   wcombo->var("coeffPol1_HI2040")->setConstant(kTRUE);
   wcombo->var("coeffPol1_HI40100")->setConstant(kTRUE);
   wcombo->var("coeffPol2_HI020")->setConstant(kTRUE);
   wcombo->var("coeffPol2_HI2040")->setConstant(kTRUE);
   wcombo->var("coeffPol3_HI020")->setConstant(kTRUE);
   wcombo->var("meanSig1")->setConstant(kTRUE);
   wcombo->var("meanSig1_HI")->setConstant(kTRUE);
   wcombo->var("sigmaSig1")->setConstant(kTRUE);
   wcombo->var("sigmaSig1_HI")->setConstant(kTRUE);
   wcombo->var("wideFactor")->setConstant(kTRUE);

   wcombo->var("NBkg_HI020")->setConstant(kFALSE);
   wcombo->var("NBkg_HI2040")->setConstant(kFALSE);
   wcombo->var("NBkg_HI40100")->setConstant(kFALSE);
   wcombo->var("NBkg_pp")->setConstant(kTRUE);
   wcombo->var("NJpsi_HI020")->setConstant(kTRUE);
   wcombo->var("NJpsi_HI2040")->setConstant(kTRUE);
   wcombo->var("NJpsi_HI40100")->setConstant(kTRUE);
   wcombo->var("NJpsi_pp")->setConstant(kTRUE);
   wcombo->var("N_Bkg_PP_pass")->setConstant(kTRUE);
   wcombo->var("N_Bkg_PbPb_pass")->setConstant(kFALSE);
   wcombo->var("N_Jpsi_PP_pass")->setConstant(kTRUE);
   wcombo->var("N_Jpsi_PbPb_pass")->setConstant(kTRUE);
   wcombo->var("N_Bkg_PP_fail")->setConstant(kTRUE);
   wcombo->var("N_Bkg_PbPb_fail")->setConstant(kFALSE);
   wcombo->var("N_Jpsi_PP_fail")->setConstant(kTRUE);
   wcombo->var("N_Jpsi_PbPb_fail")->setConstant(kTRUE);

   wcombo->var("QR")->setConstant(kFALSE);
   wcombo->var("RFrac2Svs1S_PP_fail")->setConstant(kFALSE);
   wcombo->var("RFrac2Svs1S_PP_pass")->setConstant(kFALSE);
   wcombo->var("RFrac2Svs1S_PbPb_fail")->setConstant(kFALSE);
   wcombo->var("RFrac2Svs1S_PbPbvsPP_P")->setConstant(kFALSE);
   wcombo->var("doubleRatio_HI2040")->setConstant(kFALSE);
   wcombo->var("doubleRatio_HI40100")->setConstant(kFALSE);
   wcombo->var("fracP_pp")->setConstant(kFALSE);

   wcombo->var("beta_syst")->setConstant(kFALSE);
   wcombo->var("xi_b_HI020")->setConstant(kFALSE);
   wcombo->var("xi_b_HI2040")->setConstant(kFALSE);
   wcombo->var("xi_b_HI40100")->setConstant(kFALSE);
   wcombo->var("xi_eff_HI020")->setConstant(kFALSE);
   wcombo->var("xi_eff_HI2040")->setConstant(kFALSE);
   wcombo->var("xi_eff_HI40100")->setConstant(kFALSE);
   wcombo->var("xi_fit_HI020")->setConstant(kFALSE);
   wcombo->var("xi_fit_HI2040")->setConstant(kFALSE);
   wcombo->var("xi_fit_HI40100")->setConstant(kFALSE);
   
   RooStats::ModelConfig sbHypo("SbHypo");
   sbHypo.SetWorkspace(*wcombo);
   sbHypo.SetPdf( *simPdf );

   RooArgSet obs("observables");
   obs.add(*wcombo->var("invMass"));
   obs.add( *wcombo->cat("sample"));    
   sbHypo.SetObservables( obs );
   
   RooArgSet globalObs("global_obs");
   globalObs.add( *wcombo->var("glob_syst") );
   globalObs.add( *wcombo->var("glob_fit_HI020") );
   globalObs.add( *wcombo->var("glob_eff_HI020") );
   globalObs.add( *wcombo->var("glob_b_HI020") );
   globalObs.add( *wcombo->var("glob_fit_HI2040") );
   globalObs.add( *wcombo->var("glob_eff_HI2040") );
   globalObs.add( *wcombo->var("glob_b_HI2040") );
   globalObs.add( *wcombo->var("glob_fit_HI40100") );
   globalObs.add( *wcombo->var("glob_eff_HI40100") );
   globalObs.add( *wcombo->var("glob_b_HI40100") );
   // globalObs.add( *wcombo->var("glob_fit_pp") );
   // globalObs.add( *wcombo->var("glob_eff_pp") );
   // globalObs.add( *wcombo->var("glob_pol_pp") );
   // globalObs.add( *wcombo->var("glob_b_pp") );
   sbHypo.SetGlobalObservables( globalObs );
   
   RooArgSet poi("poi");
   poi.add( *QR );
   sbHypo.SetParametersOfInterest( poi );
   
   RooArgSet nuis("nuis");
   nuis.add( *wcombo->var("beta_syst") );
   nuis.add( *wcombo->var("xi_fit_HI020") );
   nuis.add( *wcombo->var("xi_eff_HI020") );
   nuis.add( *wcombo->var("xi_pol_HI020") );
   nuis.add( *wcombo->var("xi_b_HI020") );
   nuis.add( *wcombo->var("xi_fit_HI2040") );
   nuis.add( *wcombo->var("xi_eff_HI2040") );
   nuis.add( *wcombo->var("xi_pol_HI2040") );
   nuis.add( *wcombo->var("xi_b_HI2040") );
   nuis.add( *wcombo->var("xi_fit_HI40100") );
   nuis.add( *wcombo->var("xi_eff_HI40100") );
   nuis.add( *wcombo->var("xi_pol_HI40100") );
   nuis.add( *wcombo->var("xi_b_HI40100") );
   nuis.add(*wcombo->var("NBkg_HI020"));
   nuis.add(*wcombo->var("NBkg_HI2040"));
   nuis.add(*wcombo->var("NBkg_HI40100"));
   nuis.add(*wcombo->var("NBkg_pp"));
   nuis.add(*wcombo->var("NJpsi_HI020"));
   nuis.add(*wcombo->var("NJpsi_HI2040"));
   nuis.add(*wcombo->var("NJpsi_HI40100"));
   nuis.add(*wcombo->var("NJpsi_pp"));
   nuis.add(*wcombo->var("alpha"));
   nuis.add(*wcombo->var("alpha_HI"));
   nuis.add(*wcombo->var("coeffGaus"));
   nuis.add(*wcombo->var("coeffGaus_HI"));
   nuis.add(*wcombo->var("coeffPol1"));
   nuis.add(*wcombo->var("coeffPol1_HI020"));
   nuis.add(*wcombo->var("coeffPol1_HI2040"));
   nuis.add(*wcombo->var("coeffPol1_HI40100"));
   nuis.add(*wcombo->var("coeffPol2_HI020"));
   nuis.add(*wcombo->var("coeffPol2_HI2040"));
   nuis.add(*wcombo->var("coeffPol3_HI020"));
   nuis.add(*wcombo->var("doubleRatio_HI2040"));
   nuis.add(*wcombo->var("doubleRatio_HI40100"));
   nuis.add(*wcombo->var("enne"));
   nuis.add(*wcombo->var("enne_HI"));
   nuis.add(*wcombo->var("fracP_pp"));
   nuis.add(*wcombo->var("meanSig1"));
   nuis.add(*wcombo->var("meanSig1_HI"));
   nuis.add(*wcombo->var("sigmaSig1"));
   nuis.add(*wcombo->var("sigmaSig1_HI"));
   nuis.add(*wcombo->var("sigma_b"));
   nuis.add(*wcombo->var("sigma_eff_HI020"));
   nuis.add(*wcombo->var("sigma_eff_HI2040"));
   nuis.add(*wcombo->var("sigma_eff_HI40100"));
   nuis.add(*wcombo->var("sigma_fit_HI020"));
   nuis.add(*wcombo->var("sigma_fit_HI2040"));
   nuis.add(*wcombo->var("sigma_fit_HI40100"));
   nuis.add(*wcombo->var("sigma_pol"));
   // nuis.add(*wcombo->var("wideFactor"));
   // nuis.add(*wcombo->var("wideFactor_HI"));
   
   RooArgSet nuis2("nuis2");
   nuis2.add(*wcombo->var("NJpsi_HI020"));
   nuis2.add(*wcombo->var("NJpsi_HI2040"));
   nuis2.add(*wcombo->var("NJpsi_HI40100"));
   nuis2.add(*wcombo->var("NJpsi_pp"));
   nuis2.add(*wcombo->var("N_Bkg_PP_pass"));
   nuis2.add(*wcombo->var("N_Bkg_PbPb_pass"));
   nuis2.add(*wcombo->var("N_Jpsi_PP_pass"));
   nuis2.add(*wcombo->var("N_Jpsi_PbPb_pass"));
   nuis2.add(*wcombo->var("N_Bkg_PP_fail"));
   nuis2.add(*wcombo->var("N_Bkg_PbPb_fail"));
   nuis2.add(*wcombo->var("N_Jpsi_PP_fail"));
   nuis2.add(*wcombo->var("N_Jpsi_PbPb_fail"));
   nuis2.add(*wcombo->var("RFrac2Svs1S_PP_fail"));
   nuis2.add(*wcombo->var("RFrac2Svs1S_PP_pass"));
   nuis2.add(*wcombo->var("RFrac2Svs1S_PbPb_fail"));
   nuis2.add(*wcombo->var("RFrac2Svs1S_PbPbvsPP_P"));

   nuis.add(nuis2);
   sbHypo.SetNuisanceParameters( nuis );

   // sbHypo.SetPriorPdf( *wcombo->pdf("step") ); // this is optional

   RooAbsReal * pNll = sbHypo.GetPdf()->createNLL( *datacombo,NumCPU(nCPU) );
   RooMinuit(*pNll).migrad(); // minimize likelihood wrt all parameters before making plots
   // ((RooRealVar *)poi.first())->setMin(0.);
   RooArgSet * pPoiAndNuisance = new RooArgSet("poiAndNuisance");
   pPoiAndNuisance->add( poi );
   // pPoiAndNuisance->add( nuis );
   // pPoiAndNuisance->add( nuis2 );
   sbHypo.SetSnapshot(*pPoiAndNuisance);

   wcombo->import(sbHypo);

   /////////////////////////////////////////////////////////////////////
   RooStats::ModelConfig bHypo = sbHypo;
   bHypo.SetName("BHypo");
   bHypo.SetWorkspace(*wcombo);
   pNll = bHypo.GetPdf()->createNLL( *datacombo,NumCPU(nCPU) );
   // RooMinuit(*pNll).migrad(); // minimize likelihood wrt all parameters before making plots
   RooArgSet poiAndGlobalObs("poiAndGlobalObs");
   poiAndGlobalObs.add( poi );
   poiAndGlobalObs.add( globalObs );
   RooAbsReal * pProfile = pNll->createProfile( poiAndGlobalObs ); // do not profile POI and global observables
   ((RooRealVar *)poi.first())->setVal( 1 );  // set QR=1 here
   pProfile->getVal(); // this will do fit and set nuisance parameters to profiled values
   pPoiAndNuisance = new RooArgSet( "poiAndNuisance" );
   pPoiAndNuisance->add( poi );
   // pPoiAndNuisance->add( nuis );
   // pPoiAndNuisance->add( nuis2 );
   bHypo.SetSnapshot(*pPoiAndNuisance);
   wcombo->import(bHypo);

   wcombo->writeToFile(name_out);
   wcombo->Print();
   sbHypo.Print();
}
