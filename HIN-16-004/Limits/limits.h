#ifndef limits_h
#define limits_h

#include <algorithm>
#include "../Fitter/Macros/Utilities/bin.h"
#include "TString.h"
#include "TSystemFile.h"
#include "TSystemDirectory.h"
#include <map>
#include <string>
#include <fstream>
#include <iostream>

struct limits {
   double cl;
   pair<double,double> val;
};

using namespace std;

map<anabin, limits> readLimits(const char* limitsfile);
void printTex(vector< map<anabin, limits> > theLims, const char* texName="tex/limits.tex", bool isLastTotal=false);
map<anabin, vector<limits> > vm2mv(vector< map<anabin,limits> > v);


map<anabin, limits> readLimits(const char* limitsfile) {
   map<anabin, limits> ans;

   ifstream file(limitsfile);
   if (!(file.good())) return ans;

   string limitscl; getline(file,limitscl);
   double cl = atof(limitscl.c_str());

   string line;
   double rapmin=0, rapmax=0, ptmin=0, ptmax=0, centmin=0, centmax=0, value=0;
   limits lim;
   lim.cl = cl;

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
         else if (cnt==6) lim.val.first = value;
         else if (cnt==7) lim.val.second = value;
         else if (cnt>7) {
            cout << "Warning, too many fields, I'll ignore the last ones." << endl;
            continue;
         }
         cnt++;
      }
      anabin thebin(rapmin, rapmax, ptmin, ptmax, centmin, centmax);
      ans[thebin] = lim;
   }

   return ans;
};

void printTex(vector< map<anabin, limits> > theLims, const char* texName, bool isLastTotal) {
   unsigned int nlimits = theLims.size();

   ofstream file(texName);
   file << "\\begin{tabular}{|ccc|"; 
   for (unsigned int i=0; i<nlimits; i++) {
      if (i==nlimits-1) file << "|";
      file << "c|";
   }
   file << "}" << endl;
   file << "\\hline" << endl;
   file << "$|y|$ & \\pt & Centrality";
   for (unsigned int i=0; i<nlimits; i++) file << " & " << Form("%.0f%s C.L.",100.*theLims[i].begin()->second.cl,"\\%");
   file<< "\\\\" << endl;
   file << "\\hline" << endl;

   anabin oldbin(0,0,0,0,0,0);
   map<anabin, vector<limits> > themap = vm2mv(theLims);
   map<anabin, vector<limits> >::const_iterator itm;
   for (itm=themap.begin(); itm!=themap.end(); itm++) {
      vector<limits> v = itm->second;
      if (v.size() != nlimits) {
         cout << "Error, not all limitsematics have the same size. Aborting." << endl;
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

      for (unsigned int i=0; i<nlimits; i++) {
         file << " & $[" << v[i].val.first << ", " << v[i].val.second << "]$";
      }
      file << " \\\\" << endl;

      oldbin = thebin;
   }

   file << "\\hline" << endl;
   file << "\\end{tabular}" << endl;
   file.close();
   cout << "Closed " << texName << endl;
}

map<anabin, vector<limits> > vm2mv(vector< map<anabin,limits> > v) {
   map<anabin, vector<limits> > ans;

   vector<map<anabin,limits> >::const_iterator itv;
   map<anabin,limits>::const_iterator itm;
   for (itv=v.begin(); itv!=v.end(); itv++) {
      for (itm=itv->begin(); itm!=itv->end(); itm++) {
         anabin thebin = itm->first;
         if (ans.find(thebin)==ans.end()) ans[thebin] = vector<limits>();
         ans[thebin].push_back(itm->second);
      }
   }

   return ans;
}

#endif // ifndef limits_h
