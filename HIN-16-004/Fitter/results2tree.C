#ifndef results2tree_C
#define results2tree_C

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TH1.h"
#include "TMath.h"
#include "RooRealVar.h"
#include "RooAbsPdf.h"
#include "RooAbsData.h"
#include "RooWorkspace.h"
#include "RooPlot.h"
#include "RooHist.h"

#include <vector>
#include <cstring>

#include "Macros/Utilities/resultUtils.h"

using namespace std;
using namespace RooFit;

struct poi {
   Char_t name[64];
   float val;
   float err;
};

const int nBins = 46;

void results2tree(
      const char* workDirName, 
      const char* DSTag="DATA", // Data Set tag can be: "DATA","MCPSI2SP", "MCJPSIP" ...
      const char* thePoiNames="RFrac2Svs1S,N_Jpsi,N_Psi2S,N_Psi2S_intpl,f_Jpsi,m_Jpsi,sigma1_Jpsi,alpha_Jpsi,n_Jpsi,sigma2_Jpsi,MassRatio,rSigma21_Jpsi,lambda1_Bkg,lambda2_Bkg,lambda3_Bkg,lambda4_Bkg,lambda5_Bkg,N_Bkg",
      bool wantPureSMC=false
      ) {
   // workDirName: usual tag where to look for files in Output
   // thePoiNames: comma-separated list of parameters to store ("par1,par2,par3"). Default: all

   TFile *f = new TFile(treeFileName(workDirName,DSTag),"RECREATE");
   TTree *tr = new TTree("fitresults","fit results");


   // bin edges
   float ptmin, ptmax, ymin, ymax, centmin, centmax;
   // model names
   Char_t jpsiName[128], psipName[128], bkgName[128];
   // collision system
   Char_t collSystem[8];
   // goodness of fit
   float nll, chi2, chi2prob, normchi2; int npar, nparbkg, ndof;
   // parameters to store: make it a vector
   vector<poi> thePois;
   TString thePoiNamesStr(thePoiNames);
   TString t; Int_t from = 0;
   while (thePoiNamesStr.Tokenize(t, from , ",")) {
      poi p; strcpy(p.name, t.Data());
      cout << p.name << endl;
      thePois.push_back(p);
   }

   // create tree branches
   tr->Branch("ptmin",&ptmin,"ptmin/F");
   tr->Branch("ptmax",&ptmax,"ptmax/F");
   tr->Branch("ymin",&ymin,"ymin/F");
   tr->Branch("ymax",&ymax,"ymax/F");
   tr->Branch("centmin",&centmin,"centmin/F");
   tr->Branch("centmax",&centmax,"centmax/F");
   tr->Branch("jpsiName",jpsiName,"jpsiName/C");
   tr->Branch("psipName",psipName,"psipName/C");
   tr->Branch("bkgName",bkgName,"bkgName/C");
   tr->Branch("collSystem",collSystem,"collSystem/C");
   tr->Branch("nll",&nll,"nll/F");
   tr->Branch("chi2",&chi2,"chi2/F");
   tr->Branch("chi2prob",&chi2prob,"chi2prob/F");
   tr->Branch("normchi2",&normchi2,"normchi2/F");
   tr->Branch("npar",&npar,"npar/I");
   tr->Branch("nparbkg",&nparbkg,"nparbkg/I");
   tr->Branch("ndof",&ndof,"ndof/I");

   for (vector<poi>::iterator it=thePois.begin(); it!=thePois.end(); it++) {
      tr->Branch(Form("%s_val",it->name),&(it->val),Form("%s_val/F",it->name));
      tr->Branch(Form("%s_err",it->name),&(it->err),Form("%s_err/F",it->name));
   }

   // list of files
   vector<TString> theFiles = fileList(workDirName,"",DSTag);

   int cnt=0;
   for (vector<TString>::const_iterator it=theFiles.begin(); it!=theFiles.end(); it++) {
      cout << "Parsing file " << cnt << " / " << theFiles.size() << ": " << *it << endl;

      // parse the file name to get info
      anabin thebin = binFromFile(*it);
      ptmin = thebin.ptbin().low();
      ptmax = thebin.ptbin().high();
      ymin = thebin.rapbin().low();
      ymax = thebin.rapbin().high();
      centmin = thebin.centbin().low();
      centmax = thebin.centbin().high();
      strcpy(collSystem, (it->Index("PbPb")>0) ? "PbPb" : "PP");

      // get the model names
      from = 0;
      bool catchjpsi=false, catchpsip=false, catchbkg=false;
      while (it->Tokenize(t, from, "_")) {
         if (catchjpsi) {strcpy(jpsiName, t.Data()); catchjpsi=false;}
         if (catchpsip) {strcpy(psipName, t.Data()); catchpsip=false;}
         if (catchbkg) {strcpy(bkgName, t.Data()); catchbkg=false;}
         if (t=="Jpsi") catchjpsi=true;
         if (t=="Psi2S") catchpsip=true;
         if (t=="Bkg") catchbkg=true;
      }

      TFile *f = TFile::Open(*it); RooWorkspace *ws = NULL;
      if (!f) {
         cout << "Error, file " << *it << " does not exist." << endl;
      } else {
         ws = (RooWorkspace*) f->Get("workspace");
         if (!ws) {
            cout << "Error, workspace not found in " << *it << "." << endl;
         }
      }

      nll=0; chi2=0; npar=0; ndof=0;
      if (f && ws) {
         // get the model for nll and npar
         RooAbsPdf *model = pdfFromWS(ws, Form("_%s",collSystem), "pdfMASS_Tot");

         RooAbsPdf *model_bkg = pdfFromWS(ws, Form("_%s",collSystem), "pdfMASS_Bkg");
          const char* token = (strcmp(DSTag,"DATA") && wantPureSMC) ? Form("_%s_NoBkg",collSystem) : Form("_%s",collSystem);
         RooAbsData *dat = dataFromWS(ws, token, Form("dOS_%s", DSTag));
         if (dat) {
            if (model) {
               RooAbsReal *NLL = model->createNLL(*dat);
               if (NLL) nll = NLL->getVal();
               npar = model->getParameters(dat)->selectByAttrib("Constant",kFALSE)->getSize();

               // compute the chi2 and the ndof
               RooRealVar *chi2var = ws->var("chi2");
               RooRealVar *ndofvar = ws->var("ndof");
               if (chi2var && ndofvar) {
                  chi2 = chi2var->getVal();
                  ndof = ndofvar->getVal();
               } else {
                  RooPlot* frame = ws->var("invMass")->frame(Bins(nBins));
                  dat->plotOn(frame, DataError(RooAbsData::SumW2), XErrorSize(0));
                  model->plotOn(frame, Precision(1e-4), Range("invMass"));
                  TH1 *hdatact = dat->createHistogram("hdatact", *(ws->var("invMass")), Binning(nBins));
                  RooHist *hpull = frame->pullHist(0,0, true);
                  double* ypulls = hpull->GetY();
                  unsigned int nFullBins = 0;
                  for (int i = 0; i < nBins; i++) {
                     if (hdatact->GetBinContent(i+1) > 0.0) {
                        chi2 += ypulls[i]*ypulls[i];
                        nFullBins++;
                     }
                  }
                  ndof = nFullBins - npar;
               }

               normchi2 = chi2/ndof;
               chi2prob = TMath::Prob(chi2,ndof);
            }
            if (model_bkg) {
               nparbkg = model_bkg->getParameters(dat)->selectByAttrib("Constant",kFALSE)->getSize();
            }
         }

         // get the POIs
         for (vector<poi>::iterator itpoi=thePois.begin(); itpoi!=thePois.end(); itpoi++) {
            // special case of the interpolated number of psi2S events
            if (TString(itpoi->name) == "N_Psi2S_intpl") {
               if (!dat) {
                 cout << "N_Psi2S_intpl cannot be computed, no dataset found" << endl;
                  itpoi->val=0; itpoi->err=0;
               } else {
                  double mass1=(ymax<2) ? 3.25 : 3.3, mass2=3.5, mass3=3.9, mass4=4.2;
                  double nsig = dat->reduce(Form("invMass>%f&&invMass<=%f",mass2,mass3))->numEntries();
                  double nbkg1 = dat->reduce(Form("invMass>%f&&invMass<=%f",mass1,mass2))->numEntries();
                  double nbkg2 = dat->reduce(Form("invMass>%f&&invMass<=%f",mass3,mass4))->numEntries();
                  itpoi->val = nsig - (mass3-mass2)*(nbkg1/(mass2-mass1) + nbkg2/(mass4-mass3))/2.;
                  itpoi->err = sqrt(nsig);
               }
            } else {
               RooRealVar *thevar = poiFromWS(ws, Form("_%s",collSystem), itpoi->name);
               itpoi->val = thevar ? thevar->getVal() : 0;
               itpoi->err = thevar ? thevar->getError() : 0;
            }
         }

         // delete model;
         // delete model_bkg;
         // delete dat;
         delete ws;
         delete f;
      } else {
         for (vector<poi>::iterator itpoi=thePois.begin(); itpoi!=thePois.end(); itpoi++) {
            itpoi->val = 0;
            itpoi->err = 0;
         }
      }

      // fill the tree
      tr->Fill();
      cnt++;
   } // loop on the files

   f->Write();
   f->Close();
}

#endif // #ifndef results2tree_C
