#ifndef syst_h
#define syst_h

#include "../Macros/Utilities/bin.h"
#include "../Macros/Utilities/resultUtils.h"
#include "TString.h"
#include "TSystemFile.h"
#include "TSystemDirectory.h"
#include "TCollection.h"
#include "TList.h"
#include <map>
#include <string>
#include <fstream>
#include <iostream>

#include "RooWorkspace.h"
#include "RooRealVar.h"

struct syst {
   string name;
   double value;
   double value_dR;
   double value_dR_rel;
};

using namespace std;

vector<TString> fileList_syst(const char* token, const char* prependPath="");
map<anabin, syst> readSyst(const char* systfile, const char* workDirName="", const char* path2Fitter="");
map<anabin, syst> combineSyst(vector< map<anabin, syst> > theSysts, string name="Total");
map<anabin, syst> readSyst_all(const char* token, const char* prependPath="", const char* workDirName="", bool doPrintTex=false, const char* texName="Systematics/systs.tex");
void printTex(vector< map<anabin, syst> > theSysts, const char* texName="Systematics/systs.tex", bool isLastTotal=false);
map<anabin, vector<syst> > vm2mv(vector< map<anabin,syst> > v);
RooWorkspace* getWorkspaceFromBin(anabin thebin, const char* workDirName, const char* token="", const char* path2Fitter="");

vector<TString> fileList_syst(const char* token, const char* prependPath) {
   vector<TString> ans;

   TString basedir("Systematics/csv/");
   if ( strcmp(prependPath,"") ) basedir.Prepend(Form("%s/",prependPath));
   TSystemDirectory dir(token,basedir);

   TList *files = dir.GetListOfFiles();

   if (files) {
      TIter next(files);
      TSystemFile *file;
      TString fname;

      while ((file=(TSystemFile*)next())) {
         fname = file->GetName();
         if (fname.EndsWith(".csv") && (TString(token) == "" || fname.Index(token) != kNPOS)) {
            ans.push_back(basedir+fname);
         }
      }
   }

   return ans;
};


map<anabin, syst> readSyst(const char* systfile, const char* workDirName, const char* path2Fitter) {
   map<anabin, syst> ans;

   ifstream file(systfile);
   if (!(file.good())) return ans;

   string systname; getline(file,systname);

   string line;
   double rapmin=0, rapmax=0, ptmin=0, ptmax=0, centmin=0, centmax=0, value=0;

   while (file.good()) {
      getline(file,line);
      if (line.size()==0) break;
      TString tline(line.c_str());
      TString t; Int_t from = 0, cnt=0;
      while (tline.Tokenize(t, from , ",")) {
         t.Strip(TString::kBoth,' ');
         value = atof(t.Data());
         if (cnt==0) rapmin = atof(t.Data());
         else if (cnt==1) rapmax = value;
         else if (cnt==2) ptmin = value;
         else if (cnt==3) ptmax = value;
         else if (cnt==4) centmin = value;
         else if (cnt==5) centmax = value;
         else if (cnt>6) {
            cout << "Warning, too many fields, I'll take the last one." << endl;
            continue;
         }
         cnt++;
      }
      anabin thebin(rapmin, rapmax, ptmin, ptmax, centmin, centmax);
      syst thesyst; thesyst.value = value; thesyst.name = systname;
     
     if ( strcmp(workDirName,"") )
     {
      RooWorkspace* wsPbPb = getWorkspaceFromBin(thebin,workDirName,"PbPb",path2Fitter);
      RooRealVar* singleR_PbPb = wsPbPb->var("RFrac2Svs1S_PbPb");
      double singleRPbPb = singleR_PbPb->getVal();
     
      RooWorkspace* wsPP = getWorkspaceFromBin(thebin,workDirName,"PP",path2Fitter);
      RooRealVar* singleR_PP = wsPP->var("RFrac2Svs1S_PP");
      double singleRPP = singleR_PP->getVal();
     
      RooRealVar* doubleRatio = ratioVar(singleR_PbPb,singleR_PP,1);
      double doubleR = doubleRatio->getVal();

      TString sfile(systfile);
      if (sfile.Contains("bhad_add")) thesyst.value_dR = value;
      else if (sfile.Contains("bhad") || sfile.Contains("eff_MCstat")) thesyst.value_dR = value*doubleR;
      else if (sfile.Contains("PbPb_fit")) thesyst.value_dR = value*doubleR/singleRPbPb;
      else if (sfile.Contains("PP_fit")) thesyst.value_dR = value*doubleR/singleRPP;
     
      thesyst.value_dR_rel = thesyst.value_dR/doubleR;
      }
      else
      {
        thesyst.value_dR = -99.;
        thesyst.value_dR_rel = -99.;
      }
     
      ans[thebin] = thesyst;
   }

   return ans;
};

map<anabin, syst> combineSyst(vector< map<anabin, syst> > theSysts, string name) {
   map<anabin, syst> ans;

   vector< map<anabin, syst> >::const_iterator it;
   for (it=theSysts.begin(); it!=theSysts.end(); it++) {
      map<anabin, syst>::const_iterator it2;
      for (it2=it->begin(); it2!=it->end(); it2++) {
         anabin thebin = it2->first;
         syst thesyst = it2->second;
         thesyst.name = name;

         // if we already have a syst for this bin, sum quadractically the existing syst and the new syst
         if (ans.find(thebin) != ans.end()) thesyst.value_dR = sqrt(pow(thesyst.value_dR,2) + pow(ans[thebin].value_dR,2));
         ans[thebin] = thesyst;
      }
   }

   return ans;
};

map<anabin, syst> readSyst_all(const char* token, const char* prependPath, const char* workDirName, bool doPrintTex, const char* texName) {
   // token should be PP or PbPb

   vector< map<anabin, syst> > systmap_all;
   vector< map<anabin, syst> > systmap_all_toprint;
   vector<TString> filelist = fileList_syst(token,prependPath);

   for (vector<TString>::const_iterator it=filelist.begin(); it!=filelist.end(); it++) {
      cout << "Reading file " << *it << endl;
      map<anabin,syst> systmap = readSyst(it->Data(),workDirName,prependPath);
      systmap_all_toprint.push_back(systmap);
      if ( !strcmp(workDirName,"") && (it->Index("_add") != kNPOS) ) continue; // do not combine nor return additive systematics (NP contamination)
      systmap_all.push_back(systmap);
   }

   map<anabin,syst> ans = combineSyst(systmap_all,token);
   systmap_all.push_back(ans);
   systmap_all_toprint.push_back(ans);

   if (doPrintTex) printTex(systmap_all_toprint, texName, true);

   return ans;
};

void printTex(vector< map<anabin, syst> > theSysts, const char* texName, bool isLastTotal) {
   unsigned int nsyst = theSysts.size();

   ofstream file(texName);
   file << "\\begin{tabular}{|ccc|"; 
   for (unsigned int i=0; i<nsyst; i++) {
      if (i==nsyst-1) file << "|";
      file << "c|";
   }
   file << "}" << endl;
   file << "\\hline" << endl;
   file << "$|y|$ & \\pt & Centrality";
   for (unsigned int i=0; i<nsyst; i++) file << " & " << theSysts[i].begin()->second.name;
   file<< "\\\\" << endl;
   file << "\\hline" << endl;

   anabin oldbin(0,0,0,0,0,0);
   map<anabin, vector<syst> > themap = vm2mv(theSysts);
   map<anabin, vector<syst> >::const_iterator itm;
   for (itm=themap.begin(); itm!=themap.end(); itm++) {
      vector<syst> v = itm->second;
      if (v.size() != nsyst) {
         cout << "Error, not all systematics have the same size. Aborting." << endl;
         file.close();
         return;
      }
      anabin thebin = itm->first;
      if (thebin.rapbin() == oldbin.rapbin()) {
         file << " - ";
      } else {
         if (itm != themap.begin()) file << "\\hline" << endl;
         file.precision(1); file.setf(ios::fixed);
         file << thebin.rapbin().low() << " $< |y| < $ " << thebin.rapbin().high();
      }
      file << " & ";
      if (thebin.ptbin() == oldbin.ptbin()) {
         file << " - ";
      } else {
         file.precision(1); file.setf(ios::fixed);
         file << thebin.ptbin().low() << " $< \\pt < $ " << thebin.ptbin().high();
      }
      file << " & ";
      file.unsetf(ios::fixed);
      file << thebin.centbin().low()/2 << "-" << thebin.centbin().high()/2 << "\\% ";
      file.precision(1);
      file.setf(ios::fixed);

      for (unsigned int i=0; i<nsyst; i++) {
         file << " & " << 100.*v[i].value;
      }
      file << " \\\\" << endl;

      oldbin = thebin;
   }

   file << "\\hline" << endl;
   file << "\\end{tabular}" << endl;
   file.close();
   cout << "Closed " << texName << endl;
}

map<anabin, vector<syst> > vm2mv(vector< map<anabin,syst> > v) {
   map<anabin, vector<syst> > ans;

   vector<map<anabin,syst> >::const_iterator itv;
   map<anabin,syst>::const_iterator itm;
   for (itv=v.begin(); itv!=v.end(); itv++) {
      for (itm=itv->begin(); itm!=itv->end(); itm++) {
         anabin thebin = itm->first;
         if (ans.find(thebin)==ans.end()) ans[thebin] = vector<syst>();
         ans[thebin].push_back(itm->second);
      }
   }

   return ans;
}


RooWorkspace* getWorkspaceFromBin(anabin thebin, const char* workDirName, const char* token, const char* path2Fitter){
  
  // List of files in workDirName
  vector<TString> theFiles = fileList(workDirName,token,"DATA",path2Fitter);
  if ( (theFiles.size() < 1) )
  {
    cout << "#[Error]: No files found in " << workDirName << endl;
    return 0x0;
  }
  
  if (!strcmp(token,"PP"))
  {
    bin<int> bincent(0,200);
    thebin.setcentbin(bincent);
  }
  
  // Find the name of the file containing the workspace
  int cnt=1;
  bool wsFound(false);
  vector<TString>::const_iterator it;
  for (it=theFiles.begin(); it!=theFiles.end(); it++)
  {
    anabin thebinTest = binFromFile(*it);
    if (thebinTest==thebin)
    {
      wsFound = true;
      break;
    }
  }
  if (!wsFound)
  {
    cout << "No workspace root file found in " << workDirName << endl;
    return 0x0;
  }
  
  // Read file
  TFile *f = TFile::Open(*it,"READ");
  if (!f)
  {
    cout << "#[Error]: Unable to read file " << *it << endl;
    return 0x0;
  }
  
  // Retrieve workspace from file
  RooWorkspace* ws = (RooWorkspace*) f->Get("workspace");
  if (!ws)
  {
    cout << "#[Error]: Unable to retrieve workspace" << endl;
    return 0x0;
  }

  return ws;
}

#endif // ifndef syst_h
