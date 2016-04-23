#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif
#include "TCanvas.h"
#include "RooWorkspace.h"
#include "RooAbsReal.h"
#include "RooCategory.h"
#include "RooAddPdf.h"
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

#include "test_combine.C"

using namespace RooFit;
using namespace RooStats;

void combinedWorkspace(const char* name_pbpb="fitresult.root", const char* name_PP="fitresult_PP.root", const char* name_out="fitresult_combo.root"){

   // TFile File(filename);

   RooWorkspace * ws = test_combine(name_pbpb, name_PP);
   // File.GetObject("wcombo", ws);
   // ws->Print();
   RooAbsData * data = ws->data("dOS_DATA");

   RooRealVar* doubleRatio = ws->var("doubleRatio");
   RooRealVar* leftEdge = new RooRealVar("leftEdge","leftEdge",0);
   RooRealVar* rightEdge = new RooRealVar("rightEdge","rightEdge",10);
   RooGenericPdf step("step", "step", "(@0 >= @1) && (@0 < @2)", RooArgList(*doubleRatio, *leftEdge, *rightEdge));
   ws->import(step);
   ws->factory( "Uniform::flat(doubleRatio)" );

   // systematics
   ws->factory( "syst_PP_kappa[1.10]" );
   ws->factory( "expr::alpha_syst_PP('pow(syst_PP_kappa,beta_syst_PP)',syst_PP_kappa,beta_syst_PP[0,-5,5])" );
   ws->factory( "SUM::syst_PP_nom(alpha_syst_PP*pdfMASS_Bkg_PP)" );
   ws->factory( "Gaussian::constr_syst_PP(beta_syst_PP,glob_syst_PP[0,-5,5],1)" );
   ws->factory("RooExtendPdf::pdfMASSTot_Bkg_PP_syst(syst_PP_nom,N_Bkg_PP)");
   RooAddPdf *pdftot_PP = new RooAddPdf("pdfMASS_Tot_PP_syst","pdfMASS_Tot_PP_syst",
         RooArgList(*ws->pdf("pdfMASSTot_Jpsi_PP"),*ws->pdf("pdfMASSTot_Psi2S_PP"),*ws->pdf("pdfMASSTot_Bkg_PP_syst")));
   ws->import(*pdftot_PP);
   // ws->factory("SUM::pdfMASS_Tot_PP_syst(pdfMASSTot_Jpsi_PP,pdfMASSTot_Psi2S_PP,pdfMASSTot_Bkg_PP_syst)");
   // RooAbsPdf *pdfJpsiPP = ws->pdf("pdfMASS_Jpsi_PP");
   // RooAbsPdf *pdfPsi2SPP = ws->pdf("pdfMASS_Psi2S_PP");
   // RooAbsPdf *pdfBkgPP = ws->pdf("pdfMASS_Bkg_PP_syst");
   // RooArgList pdfs_PP(*pdfJpsiPP,*pdfPsi2SPP,*pdfBkgPP);
   // RooRealVar *nJpsiPP = ws->var("N_Jpsi_PP");
   // RooRealVar *nPsi2SPP = ws->var("N_Psi2S_PP");
   // RooRealVar *nBkgPP = ws->var("N_Bkg_PP");
   // RooArgList norms_PP(*nJpsiPP,*nPsi2SPP,*nBkgPP);
   // RooAddPdf *pdfpp_tot_syst = new RooAddPdf("pdfMASS_Tot_PP_syst","pdfMASS_Tot_PP_syst",pdfs_PP,norms_PP);
   // ws->import(*pdfpp_tot_syst);

   // ws->factory("SIMUL::pdfMASS_Tot_syst(dataCat,PbPb=pdfMASS_Tot_PbPb,PP=pdfMASS_Tot_PP_syst)");
   ws->factory("SIMUL::pdfMASS_Tot_syst(dataCat,PbPb=pdfMASS_Tot_PbPb,PP=pdfMASS_Tot_PP)");




   /////////////////////////////////////////////////////////////////////
   RooRealVar * pObs = ws->var("invMass"); // get the pointer to the observable
   RooArgSet obs("observables");
   obs.add(*pObs);
   obs.add( *ws->cat("dataCat"));    
   //  /////////////////////////////////////////////////////////////////////
   ws->var("glob_syst_PP")->setConstant(true);
   // ws->var("glob_syst_pbpb")->setConstant(true);
   RooArgSet globalObs("global_obs");
   // globalObs.add( *ws->var("glob_syst_pbpb") );
   globalObs.add( *ws->var("glob_syst_PP") );

   // ws->Print();

   RooArgSet poi("poi");
   poi.add( *ws->var("doubleRatio") );



   // create set of nuisance parameters
   RooArgSet nuis("nuis");
   // nuis.add( *ws->var("beta_syst_pbpb") );
   nuis.add( *ws->var("beta_syst_PP") );

   // set parameters constant
   RooArgSet allVars = ws->allVars();
   TIterator* it = allVars.createIterator();
   RooRealVar *theVar = (RooRealVar*) it->Next();
   while (theVar) {
      TString varname(theVar->GetName());
      if (varname != "doubleRatio"
            && varname != "invMass"
            && varname != "beta_syst_PP"
            && varname != "glob_syst_PP") 
         theVar->setConstant();
      theVar = (RooRealVar*) it->Next();
   }

   // create signal+background Model Config
   RooStats::ModelConfig sbHypo("SbHypo");
   sbHypo.SetWorkspace( *ws );
   sbHypo.SetPdf( *ws->pdf("pdfMASS_Tot_syst") );
   sbHypo.SetObservables( obs );
   sbHypo.SetGlobalObservables( globalObs );
   sbHypo.SetParametersOfInterest( poi );
   sbHypo.SetNuisanceParameters( nuis );
   sbHypo.SetPriorPdf( *ws->pdf("step") ); // this is optional

   // ws->Print();
   /////////////////////////////////////////////////////////////////////
   RooAbsReal * pNll = sbHypo.GetPdf()->createNLL( *data,NumCPU(2) );
   RooMinuit(*pNll).migrad(); // minimize likelihood wrt all parameters before making plots
   RooPlot *framepoi = ((RooRealVar *)poi.first())->frame(Bins(10),Range(0.,0.2),Title("LL and profileLL in doubleRatio"));
   pNll->plotOn(framepoi,ShiftToZero());
   
   RooAbsReal * pProfile = pNll->createProfile( globalObs ); // do not profile global observables
   pProfile->getVal(); // this will do fit and set POI and nuisance parameters to fitted values
   pProfile->plotOn(framepoi,LineColor(kRed));
   framepoi->SetMinimum(0);
   framepoi->SetMaximum(3);
   TCanvas *cpoi = new TCanvas();
   cpoi->cd(); framepoi->Draw();
   cpoi->SaveAs("cpoi.pdf");

   ((RooRealVar *)poi.first())->setMin(0.);
   RooArgSet * pPoiAndNuisance = new RooArgSet("poiAndNuisance");
   // pPoiAndNuisance->add(*sbHypo.GetNuisanceParameters());
   // pPoiAndNuisance->add(*sbHypo.GetParametersOfInterest());
   pPoiAndNuisance->add( nuis );
   pPoiAndNuisance->add( poi );
   sbHypo.SetSnapshot(*pPoiAndNuisance);

   RooPlot* xframeSB = pObs->frame(Title("SBhypo"));
   data->plotOn(xframeSB,Cut("dataCat==dataCat::PP"));
   RooAbsPdf *pdfSB = sbHypo.GetPdf();
   RooCategory *dataCat = ws->cat("dataCat");
   pdfSB->plotOn(xframeSB,Slice(*dataCat,"PP"),ProjWData(*dataCat,*data));
   TCanvas *c1 = new TCanvas();
   c1->cd(); xframeSB->Draw();
   c1->SaveAs("c1.pdf");

   delete pProfile;
   delete pNll;
   delete pPoiAndNuisance;
   ws->import( sbHypo );
   /////////////////////////////////////////////////////////////////////
   RooStats::ModelConfig bHypo = sbHypo;
   bHypo.SetName("BHypo");
   bHypo.SetWorkspace(*ws);
   pNll = bHypo.GetPdf()->createNLL( *data,NumCPU(2) );
   RooArgSet poiAndGlobalObs("poiAndGlobalObs");
   poiAndGlobalObs.add( poi );
   poiAndGlobalObs.add( globalObs );
   pProfile = pNll->createProfile( poiAndGlobalObs ); // do not profile POI and global observables
   ((RooRealVar *)poi.first())->setVal( 0 );  // set doubleRatio=0 here
   pProfile->getVal(); // this will do fit and set nuisance parameters to profiled values
   pPoiAndNuisance = new RooArgSet( "poiAndNuisance" );
   pPoiAndNuisance->add( nuis );
   pPoiAndNuisance->add( poi );
   bHypo.SetSnapshot(*pPoiAndNuisance);

   RooPlot* xframeB = pObs->frame(Title("Bhypo"));
   data->plotOn(xframeB,Cut("dataCat==dataCat::PbPb"));
   RooAbsPdf *pdfB = bHypo.GetPdf();
   pdfB->plotOn(xframeB,Slice(*dataCat,"PbPb"),ProjWData(*dataCat,*data));
   TCanvas *c2 = new TCanvas();
   c2->cd(); xframeB->Draw();
   c2->SaveAs("c2.pdf");

   delete pProfile;
   delete pNll;
   delete pPoiAndNuisance;

   // import model config into workspace
   bHypo.SetWorkspace(*ws);
   ws->import( bHypo );
   /////////////////////////////////////////////////////////////////////
   ws->Print();
   bHypo.Print();
   sbHypo.Print();

   // save workspace to file
   ws -> SaveAs(name_out);

   return;
}




