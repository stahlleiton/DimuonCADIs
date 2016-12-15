#include "../plotVars.C"
#include "syst.C"

// DECLARATIONS
void plotSysts(anabin thebin, string xaxis, string collTag, bool plotGlobalSysts=false);
void plotSystsAll(bool plotGlobalSysts=false);


void plotSystsAll(bool plotGlobalSysts) {
   plotSysts(anabin(0,0.6,6.5,50,0,200),"pt","PbPb",plotGlobalSysts);
   plotSysts(anabin(0.6,1.2,6.5,50,0,200),"pt","PbPb",plotGlobalSysts);
   plotSysts(anabin(1.2,1.8,6.5,50,0,200),"pt","PbPb",plotGlobalSysts);
   plotSysts(anabin(1.8,2.4,6.5,50,0,200),"pt","PbPb",plotGlobalSysts);
   plotSysts(anabin(0,2.4,6.5,50,0,20),"pt","PbPb",plotGlobalSysts);
   plotSysts(anabin(0,2.4,6.5,50,20,60),"pt","PbPb",plotGlobalSysts);
   plotSysts(anabin(0,2.4,6.5,50,60,200),"pt","PbPb",plotGlobalSysts);
   plotSysts(anabin(0,2.4,6.5,50,0,200),"pt","PbPb",plotGlobalSysts);
   plotSysts(anabin(0,2.4,6.5,50,0,200),"pt","PP",plotGlobalSysts);
   plotSysts(anabin(0,0.6,6.5,50,0,200),"cent","PbPb",plotGlobalSysts);
   plotSysts(anabin(0.6,1.2,6.5,50,0,200),"cent","PbPb",plotGlobalSysts);
   plotSysts(anabin(1.2,1.8,6.5,50,0,200),"cent","PbPb",plotGlobalSysts);
   plotSysts(anabin(1.8,2.4,6.5,50,0,200),"cent","PbPb",plotGlobalSysts);
   plotSysts(anabin(0,2.4,6.5,50,0,200),"cent","PbPb",plotGlobalSysts);
   plotSysts(anabin(1.8,2.4,3,6.5,0,200),"cent","PbPb",plotGlobalSysts);
   plotSysts(anabin(1.8,2.4,6.5,50,0,200),"cent","PbPb",plotGlobalSysts);
   plotSysts(anabin(0,2.4,6.5,50,0,200),"rap","PbPb",plotGlobalSysts);
   plotSysts(anabin(0,2.4,6.5,50,0,200),"rap","PbPb",plotGlobalSysts);
}

void plotSysts(anabin thebin, string xaxis, string collTag, bool plotGlobalSysts) {
   float rapmin = thebin.rapbin().low();
   float rapmax = thebin.rapbin().high();
   float ptmin = thebin.ptbin().low();
   float ptmax = thebin.ptbin().high();
   int centmin = thebin.centbin().low();
   int centmax = thebin.centbin().high();

   vector<string> tags;
   vector<TGraphErrors*> graphs;
   vector<map<anabin, syst> > systs;

   // total
   systs.push_back(readSyst_all(collTag.c_str()));
   tags.push_back(systs.back().begin()->second.name);

   // detail
   if (collTag=="PbPb") {
      systs.push_back(readSyst("Systematics/csv/syst_PbPb_bkgsyst.csv"));
      tags.push_back(systs.back().begin()->second.name);
      systs.push_back(readSyst("Systematics/csv/syst_PbPb_sigsyst.csv"));
      tags.push_back(systs.back().begin()->second.name);
      systs.push_back(readSyst("Systematics/csv/syst_PbPb_dummytnp.csv"));
      tags.push_back(systs.back().begin()->second.name);
   } else {
      systs.push_back(readSyst("Systematics/csv/syst_PP_bkgsyst.csv"));
      tags.push_back(systs.back().begin()->second.name);
      systs.push_back(readSyst("Systematics/csv/syst_PP_sigsyst.csv"));
      tags.push_back(systs.back().begin()->second.name);
      systs.push_back(readSyst("Systematics/csv/syst_PP_dummytnp.csv"));
      tags.push_back(systs.back().begin()->second.name);
   }

   // global systs
   if (plotGlobalSysts) {
      systs.push_back(readSyst("Systematics/csv/syst_PbPb_taa.csv"));
      tags.push_back(systs.back().begin()->second.name);
      systs.push_back(readSyst("Systematics/csv/syst_PbPb_Nmb.csv"));
      tags.push_back(systs.back().begin()->second.name);
      systs.push_back(readSyst("Systematics/csv/syst_PP_lumi.csv"));
      tags.push_back(systs.back().begin()->second.name);
   }

   set<anabin> sb = allbins();

   for (unsigned int i=0; i<systs.size(); i++) {
      map<anabin, syst> thesyst = systs[i];
      vector<double> x, y, dx, dy;

      double valmax=-1e99;
      
      for (set<anabin>::const_iterator it=sb.begin(); it!=sb.end(); it++) {
         anabin it2 = *it;
         if (!binok(thebin,xaxis,it2,false)) continue;
         if (thesyst.find(*it)==thesyst.end()) continue;
         double low, high;
         if (xaxis=="pt") {
            low= it->ptbin().low();
            high = it->ptbin().high();
         } else if (xaxis=="rap") {
            low= it->rapbin().low();
            high = it->rapbin().high();
         } else {
            low= it->centbin().low()/2;
            high = it->centbin().high()/2;
         }
         x.push_back((low+high)/2.);
         dx.push_back((high-low)/2.);
         y.push_back(0);
         dy.push_back(thesyst[*it].value);
         valmax = max(valmax,dy.back());
      }

      TGraphErrors *thegraph = new TGraphErrors(x.size(),x.data(),y.data(),dx.data(),dy.data());
      TH1F *haxes=NULL;
      if (xaxis=="pt") {
         haxes = new TH1F(tags[i].c_str(),";p_{T} (GeV/c);Syst.",1,0,50);
      } else if (xaxis=="cent") {
         haxes = new TH1F(tags[i].c_str(),";Centrality percentile;Syst.",1,0,100);
      } else { // if (xaxis=="rap")
         haxes = new TH1F(tags[i].c_str(),";|y|;Syst.",1,0,2.4);
      }
      haxes->GetYaxis()->SetLimits(-1.3*valmax, 2.*valmax);
      haxes->GetYaxis()->SetRangeUser(-1.3*valmax, 2.*valmax);
      haxes->GetYaxis()->SetTitleOffset(1.4);
      thegraph->SetHistogram(haxes);

      thegraph->Sort();
      graphs.push_back(thegraph);
   }

   plotGraphs(graphs, tags, "systematics", collTag, 
         Form("%s_pt%i%i_rap%i%i_cent%i%i", collTag.c_str(),(int)ptmin*10,(int)ptmax*10,(int)rapmin*10,(int)rapmax*10,centmin,centmax),
         Form("%.1f<|y|<%.1f, %.1f<p_{T}<%.1f, %i-%i%s",rapmin,rapmax,ptmin,ptmax,centmin/2,centmax/2,"%"));
}
