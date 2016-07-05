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

#include "test_combine_4WS.C"
#include "../Fitter/Systematics/syst.h"

using namespace RooFit;
using namespace RooStats;

void buildSystWS(RooWorkspace *ws, const char* name, double val, bool isadd);

void combinedWorkspace_4WS(const char* name_pbpb_pass="fitresult_pbpb_pass.root", const char* name_pbpb_fail="fitresult_pbpb_fail.root", const char* name_pp_pass="fitresult_pp_pass.root", const char* name_pp_fail="fitresult_pp_fail.root", const char* name_out="fitresult_combo.root", const float systval = 0., const char* subDirName ="wsTest", int nCPU=2){
   // subdir: Directory to save workspaces under currentPATH/CombinedWorkspaces/subDir/

   // set things silent
   gErrorIgnoreLevel=kError;
   RooMsgService::instance().setGlobalKillBelow(RooFit::ERROR);
  
   bool dosyst = (systval > 0.);

   TString nameOut(name_out);
  
   RooWorkspace * ws = test_combine_4WS(name_pbpb_pass, name_pp_pass, name_pbpb_fail, name_pp_fail, false, nCPU);
   RooAbsData * data = ws->data("dOS_DATA");

   RooRealVar* RFrac2Svs1S_PbPbvsPP_P = ws->var("RFrac2Svs1S_PbPbvsPP_P");
   RooRealVar* leftEdge = new RooRealVar("leftEdge","leftEdge",-10);
   RooRealVar* rightEdge = new RooRealVar("rightEdge","rightEdge",10);
   RooGenericPdf step("step", "step", "(@0 >= @1) && (@0 < @2)", RooArgList(*RFrac2Svs1S_PbPbvsPP_P, *leftEdge, *rightEdge));
   ws->import(step);
   ws->factory( "Uniform::flat(RFrac2Svs1S_PbPbvsPP_P)" );

   // systematics
   if (dosyst) {
     ws->factory( Form("kappa_syst[%f]",systval) );
     ws->factory( "expr::alpha_syst('kappa_syst*beta_syst',kappa_syst,beta_syst[0,-5,5])" );
     ws->factory( "Gaussian::constr_syst(beta_syst,glob_syst[0,-5,5],1)" );
     
     // add systematics into the double ratio
     ws->factory( "expr::RFrac2Svs1S_PbPbvsPP_P_syst('@0+@1',RFrac2Svs1S_PbPbvsPP_P,alpha_syst)" );
     
     // build the pbpb pdf
     RooRealVar* effjpsi_pp_P = (RooRealVar*)ws->var("effjpsi_pp_P");
     RooRealVar* effpsip_pp_P = (RooRealVar*)ws->var("effpsip_pp_P");
     RooRealVar* effjpsi_pp_NP = (RooRealVar*)ws->var("effjpsi_pp_NP");
     Double_t Npsi2SPbPbPass = npsip_pbpb_pass_from_doubleratio_prompt(ws, RooArgList(*effjpsi_pp_P,*effpsip_pp_P,*effjpsi_pp_NP),true); // Create and import N_Psi2S_PbPb_pass_syst
     
     ws->factory( "SUM::pdfMASS_Tot_PbPb_pass_syst(N_Jpsi_PbPb_pass * pdfMASS_Jpsi_PbPb_pass, N_Psi2S_PbPb_pass_syst * pdfMASS_Psi2S_PbPb_pass, N_Bkg_PbPb_pass * pdfMASS_Bkg_PbPb_pass)" );
     ws->factory( "PROD::pdfMASS_Tot_PbPb_pass_constr(pdfMASS_Tot_PbPb_syst,constr_syst)" );
     
     // build the combined pdf
     ws->factory("SIMUL::simPdf_syst_noconstr(sample,PbPb_pass=pdfMASS_Tot_PbPb_pass_syst,PbPb_fail=pdfMASS_Tot_PbPb_fail,PP_pass=pdfMASS_Tot_PP_pass,PP_fail=pdfMASS_Tot_PP_fail)");
     RooSimultaneous *simPdf = (RooSimultaneous*) ws->pdf("simPdf_syst_noconstr");
     RooGaussian *constr_syst = (RooGaussian*) ws->pdf("constr_syst");
     RooProdPdf *simPdf_constr = new RooProdPdf("simPdf_syst","simPdf_syst",RooArgSet(*simPdf,*constr_syst));
     ws->import(*simPdf_constr);
     
   } else {
      ws->factory("SIMUL::simPdf_syst(sample,PbPb_pass=pdfMASS_Tot_PbPb_pass,PbPb_fail=pdfMASS_Tot_PbPb_fail,PP_pass=pdfMASS_Tot_PP_pass,PP_fail=pdfMASS_Tot_PP_fail)");
   }

   ws->Print();

   if (dosyst) ws->var("beta_syst")->setConstant(kFALSE);


   /////////////////////////////////////////////////////////////////////
   RooRealVar * pObs = ws->var("invMass"); // get the pointer to the observable
   RooArgSet obs("observables");
   obs.add(*pObs);
   obs.add( *ws->cat("sample"));    
   //  /////////////////////////////////////////////////////////////////////

   if (dosyst) ws->var("glob_syst")->setConstant(true);
   RooArgSet globalObs("global_obs");
   if (dosyst) globalObs.add( *ws->var("glob_syst") );

   // ws->Print();

   RooArgSet poi("poi");
   poi.add( *ws->var("RFrac2Svs1S_PbPbvsPP_P") );



   // create set of nuisance parameters
   RooArgSet nuis("nuis");
   if (dosyst) nuis.add( *ws->var("beta_syst") );

   // set parameters constant
   RooArgSet allVars = ws->allVars();
   TIterator* it = allVars.createIterator();
   RooRealVar *theVar = (RooRealVar*) it->Next();
   while (theVar) {
      TString varname(theVar->GetName());
//      if (varname != "RFrac2Svs1S_PbPbvsPP"
//            && varname != "invMass"
//            && varname != "sample"
//            )
//         theVar->setConstant();
     if ( varname.Contains("f_Jpsi_PP") || varname.Contains("f_Jpsi_PbPb") || varname.Contains("rSigma21_Jpsi_PP") || varname.Contains("m_Jpsi_PP") || varname.Contains("m_Jpsi_PbPb") || varname.Contains("sigma1_Jpsi_PP") || varname.Contains("sigma1_Jpsi_PbPb") || (varname.Contains("lambda"))  )
         {
           theVar->setConstant();
         }
      if (varname=="glob_syst"
            || varname=="beta_syst"
         ) {
         cout << varname << endl;
         theVar->setConstant(!dosyst);
      }
      theVar = (RooRealVar*) it->Next();
   }

   // create signal+background Model Config
   RooStats::ModelConfig sbHypo("SbHypo");
   sbHypo.SetWorkspace( *ws );
   sbHypo.SetPdf( *ws->pdf("simPdf_syst") );
   sbHypo.SetObservables( obs );
   sbHypo.SetGlobalObservables( globalObs );
   sbHypo.SetParametersOfInterest( poi );
   sbHypo.SetNuisanceParameters( nuis );
   sbHypo.SetPriorPdf( *ws->pdf("step") ); // this is optional

   // ws->Print();
   /////////////////////////////////////////////////////////////////////
   RooAbsReal * pNll = sbHypo.GetPdf()->createNLL( *data,NumCPU(nCPU) );
   RooMinuit(*pNll).migrad(); // minimize likelihood wrt all parameters before making plots
   RooPlot *framepoi = ((RooRealVar *)poi.first())->frame(Bins(10),Range(0.,1),Title("LL and profileLL in RFrac2Svs1S_PbPbvsPP_Prompt"));
   pNll->plotOn(framepoi,ShiftToZero());
   
   RooAbsReal * pProfile = pNll->createProfile( globalObs ); // do not profile global observables
   pProfile->getVal(); // this will do fit and set POI and nuisance parameters to fitted values
   pProfile->plotOn(framepoi,LineColor(kRed));
   framepoi->SetMinimum(0);
   framepoi->SetMaximum(10);
   TCanvas *cpoi = new TCanvas();
   cpoi->cd(); framepoi->Draw();
//   cpoi->SaveAs("cpoi.pdf");

   ((RooRealVar *)poi.first())->setMin(0.);
   RooArgSet * pPoiAndNuisance = new RooArgSet("poiAndNuisance");
   // pPoiAndNuisance->add(*sbHypo.GetNuisanceParameters());
   // pPoiAndNuisance->add(*sbHypo.GetParametersOfInterest());
   pPoiAndNuisance->add( nuis );
   pPoiAndNuisance->add( poi );
   sbHypo.SetSnapshot(*pPoiAndNuisance);

   RooPlot* xframeSB = pObs->frame(Title("SBhypo"));
   data->plotOn(xframeSB,Cut("sample==sample::PP"));
   RooAbsPdf *pdfSB = sbHypo.GetPdf();
   RooCategory *sample = ws->cat("sample");
   pdfSB->plotOn(xframeSB,Slice(*sample,"PP"),ProjWData(*sample,*data));
   TCanvas *c1 = new TCanvas();
   c1->cd(); xframeSB->Draw();
//   c1->SaveAs("c1.pdf");
   RooPlot* xframeB = pObs->frame(Title("SBhypo_PbPb"));
   data->plotOn(xframeB,Cut("sample==sample::PbPb"));
   RooAbsPdf *pdfB = sbHypo.GetPdf();
   pdfB->plotOn(xframeB,Slice(*sample,"PbPb"),ProjWData(*sample,*data));
   TCanvas *c2 = new TCanvas();
   c2->cd(); xframeB->Draw();
   c2->SetLogy();
//   c2->SaveAs("c2.pdf");
//   c2->SaveAs("c2.root");

   delete pProfile;
   delete pNll;
   delete pPoiAndNuisance;
   ws->import( sbHypo );
   /////////////////////////////////////////////////////////////////////
   RooStats::ModelConfig bHypo = sbHypo;
   bHypo.SetName("BHypo");
   bHypo.SetWorkspace(*ws);
   pNll = bHypo.GetPdf()->createNLL( *data,NumCPU(nCPU) );
   RooArgSet poiAndGlobalObs("poiAndGlobalObs");
   poiAndGlobalObs.add( poi );
   poiAndGlobalObs.add( globalObs );
   pProfile = pNll->createProfile( poiAndGlobalObs ); // do not profile POI and global observables
   ((RooRealVar *)poi.first())->setVal( 0 );  // set RFrac2Svs1S_PbPbvsPP=0 here
   pProfile->getVal(); // this will do fit and set nuisance parameters to profiled values
   pPoiAndNuisance = new RooArgSet( "poiAndNuisance" );
   pPoiAndNuisance->add( nuis );
   pPoiAndNuisance->add( poi );
   bHypo.SetSnapshot(*pPoiAndNuisance);

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
   string mainDIR = gSystem->ExpandPathName(gSystem->pwd());
   string wsDIR = mainDIR + "/CombinedWorkspaces/";
   string ssubDirName="";
   if (subDirName) ssubDirName.append(subDirName);
   string subDIR = wsDIR + ssubDirName;
  
   void * dirp = gSystem->OpenDirectory(wsDIR.c_str());
   if (dirp) gSystem->FreeDirectory(dirp);
   else gSystem->mkdir(wsDIR.c_str(), kTRUE);

   void * dirq = gSystem->OpenDirectory(subDIR.c_str());
   if (dirq) gSystem->FreeDirectory(dirq);
   else gSystem->mkdir(subDIR.c_str(), kTRUE);
  
   const char* saveName = Form("%s/%s",subDIR.c_str(),nameOut.Data());
   ws->writeToFile(saveName);
}



