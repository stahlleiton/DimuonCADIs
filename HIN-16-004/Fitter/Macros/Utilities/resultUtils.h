#ifndef resultUtils_h
#define resultUtils_h

#include "bin.h"

#include "RooRealVar.h"
#include "RooAbsPdf.h"
#include "RooAbsData.h"
#include "RooWorkspace.h"
#include "TString.h"
#include "TFile.h"
#include "TSystemFile.h"
#include "TSystemDirectory.h"
#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"

#include <string>
#include <vector>
#include <iostream>

using namespace std;

RooRealVar* poiFromFile(const char* filename, const char* token, const char* thepoiname);
RooRealVar* poiFromWS(RooWorkspace* ws, const char* token, const char* thepoiname);
RooAbsPdf* pdfFromWS(RooWorkspace* ws, const char* token, const char* thepdfname);
RooAbsData* dataFromWS(RooWorkspace* ws, const char* token, const char* thedataname);
vector<TString> fileList(const char* input, const char* token="", bool isMC=false);
RooRealVar* ratioVar(RooRealVar *num, RooRealVar *den, bool usedenerror=true);
anabin binFromFile(const char* filename);
bool binok(vector<anabin> thecats, string xaxis, anabin &tocheck);
bool binok(anabin thecat, string xaxis, anabin &tocheck);
TString treeFileName(const char* workDirName, bool isMC=false);
void prune(vector<anabin> &v, bool keppshortest=true);
void prune(TGraphAsymmErrors *g, TGraphAsymmErrors *gsyst=NULL, bool keppshortest=true);
void prune(TGraphErrors *g, bool keppshortest=true);

RooRealVar* poiFromFile(const char* filename, const char* token, const char* thepoiname) {
   TFile *f = TFile::Open(filename);
   if (!f) {
      cout << "Error, file " << filename << " does not exist." << endl;
      return NULL;
   }
   RooWorkspace *ws = (RooWorkspace*) f->Get("workspace");
   if (!ws) {
      cout << "Error, file " << filename << " is bad." << endl;
      return NULL;
   }
   RooRealVar *ans = poiFromWS(ws, token, thepoiname);
   if (!ans) return NULL;
   TString poiname_and_token = TString(thepoiname) + TString(token);
   RooRealVar* ansc = new RooRealVar(*ans,poiname_and_token + Form("_from_%s",filename));
   delete ws;
   delete f;
   return ansc;
}

RooRealVar* poiFromWS(RooWorkspace* ws, const char* token, const char* thepoiname) {
   TString poiname_and_token = TString(thepoiname) + TString(token);
   RooRealVar *ans = (RooRealVar*) ws->var(poiname_and_token);
   return ans;
}

RooAbsPdf* pdfFromWS(RooWorkspace* ws, const char* token, const char* thepdfname) {
   TString pdfname_and_token = TString(thepdfname) + TString(token);
   RooAbsPdf *ans = (RooAbsPdf*) ws->pdf(pdfname_and_token);
   return ans;
}

RooAbsData* dataFromWS(RooWorkspace* ws, const char* token, const char* thedataname) {
   TString dataname_and_token = TString(thedataname) + TString(token);
   RooAbsData *ans = (RooAbsData*) ws->data(dataname_and_token);
   return ans;
}

vector<TString> fileList(const char* input, const char* token, bool isMC) {
   vector<TString> ans;

   TString basedir(Form("Output/%s/result/DATA/",input));
   if (isMC) basedir = TString(Form("Output/%s/result/MC/",input));
   TSystemDirectory dir(input,basedir);

   TList *files = dir.GetListOfFiles();

   if (files) {
      TIter next(files);
      TSystemFile *file;
      TString fname;

      while ((file=(TSystemFile*)next())) {
         fname = file->GetName();
         if (fname.EndsWith(".root") && fname.Index("FIT") != kNPOS
               && (TString(token) == "" || fname.Index(token) != kNPOS)) {
            ans.push_back(basedir+fname);
         }
      }
   }

   return ans;
}

RooRealVar* ratioVar(RooRealVar *num, RooRealVar *den, bool usedenerror) {
   double n = num->getVal();
   double d = den->getVal();
   double dn = num->getError();
   double dd = den->getError();

   double r = d!=0 ? n/d : 0;
   double dr = n!=0 && d!=0 ? r * sqrt(pow(dn/n,2) + pow(dd/d,2)) : 0;
   if (!usedenerror && n!=0) dr = (dn/n)*r;
   RooRealVar *ans = new RooRealVar(Form("%s_over_%s",num->GetName(),den->GetName()), Form("%s / %s",num->GetTitle(),den->GetTitle()), r);
   ans->setError(dr);

   return ans;
}

anabin binFromFile(const char* filename) {
   TFile *f = TFile::Open(filename);
   if (!f) {
      cout << "Error, file " << filename << " does not exist." << endl;
      return anabin(0,0,0,0,0,0);
   }
   RooWorkspace *ws = (RooWorkspace*) f->Get("workspace");
   if (!ws) {
      cout << "Error, file " << filename << " is bad." << endl;
      return anabin(0,0,0,0,0,0);
   }
   RooRealVar *pt = (RooRealVar*) ws->var("pt");
   RooRealVar *rap = (RooRealVar*) ws->var("rap");
   RooRealVar *cent = (RooRealVar*) ws->var("cent");
   if (!pt || !rap || !cent) {
      cout << "Error, file " << filename << " is bad." << endl;
      return anabin(0,0,0,0,0,0);
   }
   anabin ans(rap->getMin(),rap->getMax(),pt->getMin(),pt->getMax(),cent->getMin(),cent->getMax());
   delete ws;
   delete f;
   return ans;
}

bool binok(vector<anabin> thecats, string xaxis, anabin &tocheck) {
   bool ok=false;

   for (vector<anabin>::const_iterator it=thecats.begin(); it!=thecats.end(); it++) {
      if (xaxis=="pt" && it->rapbin()==tocheck.rapbin() && it->centbin()==tocheck.centbin()
            && ! (it->ptbin()==tocheck.ptbin())) {
         ok=true;
         tocheck.setptbin(it->ptbin());
         break;
      } else if (xaxis=="cent" && it->rapbin()==tocheck.rapbin() && it->ptbin()==tocheck.ptbin()
            && ! (it->centbin()==tocheck.centbin())) {
         ok=true;
         tocheck.setcentbin(it->centbin());
         break;
      } else if (((it->centbin().low()<=0 && it->centbin().high()<=0) || xaxis=="rap")
            && it->rapbin()==tocheck.rapbin() && it->ptbin()==tocheck.ptbin()
            &&  (abs(it->centbin().low())==abs(tocheck.centbin().low()) && abs(it->centbin().high())==abs(tocheck.centbin().high()))) {
         ok=true;
         break;
      }
   }

   return ok;
}

bool binok(anabin thecat, string xaxis, anabin &tocheck) {
   vector<anabin> thecats; thecats.push_back(thecat);
   return binok(thecats, xaxis, tocheck);
}

TString treeFileName(const char* workDirName, bool isMC) {
   TString outputFileName = Form("Output/%s/result/DATA/tree_allvars.root",workDirName);
   if (isMC) outputFileName = Form("Output/%s/result/MC/tree_allvars.root",workDirName);
   return outputFileName;
}

void prune(vector<anabin> &v, bool keepshort) {
   vector<anabin> ans;

   vector<anabin>::const_iterator it1, it2;
   for (it1=v.begin(); it1!=v.end(); it1++) {
      bool binok=true;
      for (it2=v.begin(); it2!=v.end(); it2++) {
         if (*it1==*it2) continue;
         if (it1->rapbin()==it2->rapbin() && it1->ptbin()==it2->ptbin()) {
            binI cb1 = it1->centbin();
            binI cb2 = it2->centbin();
            if (!(cb1==binI(0,200)) && cb1.low()==cb2.low()) { // the bin is not MB and there is another bin with the same lower edge
               if (keepshort && cb1.high()>cb2.high()) binok=false;
               if (!keepshort && cb1.high()<cb2.high()) binok=false;
            }
         } // same pt and rap bins
      } // for it2
      if (binok) ans.push_back(*it1);
   } // for it1

   v = ans;
}

void prune(TGraphAsymmErrors *g, TGraphAsymmErrors *gsyst, bool keepshort) {
   int n = g->GetN();
   for (int i1=0; i1<n; i1++) {
      double xl1 = g->GetX()[i1]-g->GetErrorXlow(i1);
      double xh1 = g->GetX()[i1]+g->GetErrorXhigh(i1);
      bool binok=true;
      for (int i2=0; i2<n; i2++) {
         if (i2==i1) continue;
         double xl2 = g->GetX()[i2]-g->GetErrorXlow(i2);
         double xh2 = g->GetX()[i2]+g->GetErrorXhigh(i2);
         if (fabs(xl1-xl2)<1e-3 && keepshort && xh1>xh2) binok=false;
         if (fabs(xl1-xl2)<1e-3 && !keepshort && xh1<xh2) binok=false;
      } // for i2
      if (!binok) {
         g->SetPoint(i1,-g->GetX()[i1],g->GetY()[i1]);
         if (gsyst) gsyst->SetPoint(i1,-gsyst->GetX()[i1],gsyst->GetY()[i1]);
      }
   } // for i1
}

void prune(TGraphErrors *g, bool keepshort) {
   int n = g->GetN();
   for (int i1=0; i1<n; i1++) {
      double xl1 = g->GetX()[i1]-g->GetErrorX(i1);
      double xh1 = g->GetX()[i1]+g->GetErrorX(i1);
      bool binok=true;
      for (int i2=0; i2<n; i2++) {
         if (i2==i1) continue;
         double xl2 = g->GetX()[i2]-g->GetErrorX(i2);
         double xh2 = g->GetX()[i2]+g->GetErrorX(i2);
         if (fabs(xl1-xl2)<1e-3 && keepshort && xh1>xh2) binok=false;
         if (fabs(xl1-xl2)<1e-3 && !keepshort && xh1<xh2) binok=false;
      } // for i2
      if (!binok) g->SetPoint(i1,-g->GetX()[i1],g->GetY()[i1]);
   } // for i1
}

#endif // ifndef resultUtils_h
