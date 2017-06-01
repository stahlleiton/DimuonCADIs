#include "../plotVars.C"
#include "syst.C"

#include "TString.h"

TString spoiname("NJpsi"); // It can be NJpsi or BJpsi
bool bins16004=false;

// DECLARATIONS
void plotSysts(anabin thebin, string xaxis, string collTag, bool plotGlobalSysts=false);
void plotSystsAll(bool plotGlobalSysts=false, bool is16004=false);


void plotSystsAll(bool plotGlobalSysts, bool is16004) {
  if (!is16004)
  {
    //pt dependence in rapidity bins
    plotSysts(anabin(0,0.6,6.5,50,0,200),"pt","PbPb",plotGlobalSysts);
    plotSysts(anabin(0.6,1.2,6.5,50,0,200),"pt","PbPb",plotGlobalSysts);
    plotSysts(anabin(1.2,1.8,6.5,50,0,200),"pt","PbPb",plotGlobalSysts);
    plotSysts(anabin(1.8,2.4,6.5,50,0,200),"pt","PbPb",plotGlobalSysts);
    plotSysts(anabin(0,2.4,6.5,50,0,200),"pt","PbPb",plotGlobalSysts);
    
    plotSysts(anabin(0,0.6,6.5,50,0,200),"pt","PP",plotGlobalSysts);
    plotSysts(anabin(0.6,1.2,6.5,50,0,200),"pt","PP",plotGlobalSysts);
    plotSysts(anabin(1.2,1.8,6.5,50,0,200),"pt","PP",plotGlobalSysts);
    plotSysts(anabin(1.8,2.4,6.5,50,0,200),"pt","PP",plotGlobalSysts);
    plotSysts(anabin(0,2.4,6.5,50,0,200),"pt","PP",plotGlobalSysts);
    
    //pt dependence in centrality bins
    plotSysts(anabin(0,2.4,6.5,50,0,20),"pt","PbPb",plotGlobalSysts);
    plotSysts(anabin(0,2.4,6.5,50,20,60),"pt","PbPb",plotGlobalSysts);
    plotSysts(anabin(0,2.4,6.5,50,60,200),"pt","PbPb",plotGlobalSysts);
    
    //centrality dependence in rapidity bins
    plotSysts(anabin(0,0.6,6.5,50,0,200),"cent","PbPb",plotGlobalSysts);
    plotSysts(anabin(0.6,1.2,6.5,50,0,200),"cent","PbPb",plotGlobalSysts);
    plotSysts(anabin(1.2,1.8,6.5,50,0,200),"cent","PbPb",plotGlobalSysts);
    plotSysts(anabin(1.8,2.4,6.5,50,0,200),"cent","PbPb",plotGlobalSysts);
    plotSysts(anabin(0,2.4,6.5,50,0,200),"cent","PbPb",plotGlobalSysts);
    
    //centrality dependence at fwd rapidity and low pt
    plotSysts(anabin(1.8,2.4,3,6.5,0,200),"cent","PbPb",plotGlobalSysts);
    
    //rapidity dependence
    plotSysts(anabin(0,2.4,6.5,50,0,200),"rap","PbPb",plotGlobalSysts);
    plotSysts(anabin(0,2.4,6.5,50,0,200),"rap","PP",plotGlobalSysts);
  }
  else
  {
    bins16004 = true;
    cout << "Not supported yet" << endl;
    return;
  }
  
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
  systs.push_back(readSyst_all(collTag.c_str(),spoiname.Data(),"../"));
  tags.push_back(systs.back().begin()->second.name);
  
  // detail
  if (collTag=="PbPb") {
    
    systs.push_back(readSyst(Form("csv/syst_%s_PbPb_massBkg.csv",spoiname.Data())));
    tags.push_back(systs.back().begin()->second.name);
    systs.push_back(readSyst(Form("csv/syst_%s_PbPb_massSig.csv",spoiname.Data())));
    tags.push_back(systs.back().begin()->second.name);
    systs.push_back(readSyst(Form("csv/syst_%s_PbPb_ctauErr.csv",spoiname.Data())));
    tags.push_back(systs.back().begin()->second.name);
    systs.push_back(readSyst(Form("csv/syst_%s_PbPb_ctauTrue.csv",spoiname.Data())));
    tags.push_back(systs.back().begin()->second.name);
    systs.push_back(readSyst(Form("csv/syst_%s_PbPb_ctauRes.csv",spoiname.Data())));
    tags.push_back(systs.back().begin()->second.name);
    systs.push_back(readSyst(Form("csv/syst_%s_PbPb_ctauBkg.csv",spoiname.Data())));
    tags.push_back(systs.back().begin()->second.name);
//    systs.push_back(readSyst("csv/syst_PbPb_fulltnp.csv"));
//    tags.push_back(systs.back().begin()->second.name);
//    systs.push_back(readSyst("csv/syst_PbPb_muidtnp.csv"));
//    tags.push_back(systs.back().begin()->second.name);
//    systs.push_back(readSyst("csv/syst_PbPb_statnp.csv"));
//    tags.push_back(systs.back().begin()->second.name);
  } else {
    systs.push_back(readSyst(Form("csv/syst_%s_PP_massBkg.csv",spoiname.Data())));
    tags.push_back(systs.back().begin()->second.name);
    systs.push_back(readSyst(Form("csv/syst_%s_PP_massSig.csv",spoiname.Data())));
    tags.push_back(systs.back().begin()->second.name);
    systs.push_back(readSyst(Form("csv/syst_%s_PP_ctauErr.csv",spoiname.Data())));
    tags.push_back(systs.back().begin()->second.name);
    systs.push_back(readSyst(Form("csv/syst_%s_PP_ctauTrue.csv",spoiname.Data())));
    tags.push_back(systs.back().begin()->second.name);
    systs.push_back(readSyst(Form("csv/syst_%s_PP_ctauRes.csv",spoiname.Data())));
    tags.push_back(systs.back().begin()->second.name);
    systs.push_back(readSyst(Form("csv/syst_%s_PP_ctauBkg.csv",spoiname.Data())));
    tags.push_back(systs.back().begin()->second.name);
//    systs.push_back(readSyst("csv/syst_PP_fulltnp.csv"));
//    tags.push_back(systs.back().begin()->second.name);
//    systs.push_back(readSyst("csv/syst_PP_muidtnp.csv"));
//    tags.push_back(systs.back().begin()->second.name);
//    systs.push_back(readSyst("csv/syst_PP_statnp.csv"));
//    tags.push_back(systs.back().begin()->second.name);
  }
  
  // global systs
  if (plotGlobalSysts) {
    systs.push_back(readSyst("csv/syst_PbPb_taa.csv"));
    tags.push_back(systs.back().begin()->second.name);
    systs.push_back(readSyst("csv/syst_PbPb_Nmb.csv"));
    tags.push_back(systs.back().begin()->second.name);
    systs.push_back(readSyst("csv/syst_PP_lumi.csv"));
    tags.push_back(systs.back().begin()->second.name);
    systs.push_back(readSyst("csv/syst_PP_trk.csv"));
    tags.push_back(systs.back().begin()->second.name);
    systs.push_back(readSyst("csv/syst_PbPb_trk.csv"));
    tags.push_back(systs.back().begin()->second.name);
  }
  
  set<anabin> sb;
  if (bins16004) sb = allbins16004();
  else sb = allbins();
  
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
        if ((low==0 && high<=0.61 && high>=0.59 ) || (low>=0.59 && low<=0.61 && high>=1.19 && high <=1.21) || (low>=1.19 && low<=1.21 && high>=1.79 && high <=1.81) || (low>=1.79 && low<=1.81 && high>=2.39 && high <=2.41)) continue;
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
      haxes = new TH1F(tags[i].c_str(),Form(";p_{T} (GeV/c);Syst_%s",spoiname.Data()),1,0,50);
    } else if (xaxis=="cent") {
      haxes = new TH1F(tags[i].c_str(),Form(";Centrality percentile;Syst_%s",spoiname.Data()),1,0,100);
    } else { // if (xaxis=="rap")
      haxes = new TH1F(tags[i].c_str(),Form(";|y|;Syst_%s",spoiname.Data()),1,0,2.4);
    }
    haxes->GetYaxis()->SetLimits(-1.3*valmax, 2.*valmax);
    haxes->GetYaxis()->SetRangeUser(-1.3*valmax, 2.*valmax);
    haxes->GetYaxis()->SetTitleOffset(1.4);
    thegraph->SetHistogram(haxes);

    thegraph->Sort();
    graphs.push_back(thegraph);
  }
  
  
  plotGraphs(graphs, tags, "systematics", collTag,
             Form("pt%i%i_rap%i%i_cent%i%i",(int)ptmin*10,(int)ptmax*10,(int)rapmin*10,(int)rapmax*10,centmin,centmax),
             Form("%.1f<|y|<%.1f, %.1f<p_{T}<%.1f, %i-%i%s",rapmin,rapmax,ptmin,ptmax,centmin/2,centmax/2,"%"));
}
