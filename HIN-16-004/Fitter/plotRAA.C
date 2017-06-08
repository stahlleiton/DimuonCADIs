#include "Macros/CMS/CMS_lumi.C"
#include "Macros/CMS/tdrstyle.C"
#include "Macros/Utilities/bin.h"
#include "Macros/Utilities/bfrac.h"
#include "Macros/Utilities/EVENTUTILS.h"
#include "Macros/Utilities/initClasses.h"
#include "Macros/Utilities/resultUtils.h"
#include "Macros/Utilities/texUtils.h"
#include "Macros/Utilities/monster.h"
#include "Systematics/syst.h"

#include <vector>
#include <map>
#include <string>
#include <sstream>
#include "TGraphAsymmErrors.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TLegend.h"
#include "TLine.h"
#include "RooRealVar.h"
#include "RooWorkspace.h"
#include "TSystemDirectory.h"
#include "TSystemFile.h"
#include "TArrow.h"
#include "TString.h"

using namespace std;

////////////////
// PARAMETERS //
////////////////

//#ifndef poiname_check
//#define poiname_check
//const char* poiname       = "N_Jpsi"; // for RAA (will correct automatically for efficiency)
//#endif
//const char* ylabel        = "R_{AA}";

bool  doprompt      = true;  // prompt Jpsi
bool  dononprompt   = false;  // nonprompt Jpsi
bool  is16004       = false; // plot results in 16-004 bins. If false use 16-025 bins
bool  plotPsi2S     = false; // plot Psi2S
bool  plot14005     = false; // plot results at 2.76 TeV
bool  applyEff      = true;
bool  applyAcc      = false;
bool  doLogPt       = false;
string nameTag_base = "_prompt";    // can put here e.g. "_prompt", "_nonprompt", ...

const bool useNcoll = false; // false -> use TAA / NMB, true -> use Ncoll / lumiPbPb

//////////////////
// DECLARATIONS //
//////////////////

void printOptions();
void setOptions(bool adoprompt, bool adononprompt, bool  ais16004, bool aplotPsi2S, bool aplot14005, string anameTag_base);
void plotGraphRAA(map<anabin, TGraphAsymmErrors*> theGraphs, map<anabin, TGraphAsymmErrors*> theGraphs_syst, string xaxis, string outputDir, map<anabin, syst> gsyst);
void plotGraphXS(map<anabin, TGraphAsymmErrors*> theGraphs, map<anabin, TGraphAsymmErrors*> theGraphs_syst, string xaxis, string outputDir, map<anabin, syst> gsyst, const char* collName);
void plotGraphBF(map<anabin, TGraphAsymmErrors*> theGraphs, map<anabin, TGraphAsymmErrors*> theGraphs_syst, string xaxis, string outputDir, const char* collName);
void plotRAA(vector<anabin> thecats, string xaxis, string workDirName);
void plotXS(vector<anabin> thecats, string xaxis, string workDirName);
void plotBF(vector<anabin> thecats, string xaxis, string workDirName);
void centrality2npart(TGraphAsymmErrors* tg, bool issyst=false, double xshift=0.);
int color(int i);
int markerstyle(int i);
string nameTag;

class raa_input {
public:
  double npp;
  double dnpp_stat;
  double systpp;
  double naa;
  double dnaa_stat;
  double systaa;
  double effpp;
  double effaa;
  double accpp;
  double accaa;
  double taa;
  double lumipp;
  double lumiaa;
  double ncoll;
  double bfracpp;
  double dbfracpp;
  double systbfracpp;
  double bfracaa;
  double dbfracaa;
  double systbfracaa;
  double correlaa;
  double correlpp;
  syst   statpp;
};

map<anabin, raa_input > readResults(const char* resultsFile);
void drawArrow(double x, double ylow, double yhigh, double dx, Color_t color);

/////////////////////////////////////////////
// MAIN FUNCTIONS TO BE CALLED BY THE USER //
/////////////////////////////////////////////

void plotPt(string workDirName, string poiname, int iplot) {
  // poiname is the observable to be plotted = "RAA", "BF" and "XS" (For RAA, b-fraction and cross-section)
  
  string xaxis = "pt";
  vector<anabin> theCats;
  
  // 4 rapidity intervals
  if (iplot==0) {
    theCats.push_back(anabin(0,0.6,6.5,50,0,200));
    theCats.push_back(anabin(0.6,1.2,6.5,50,0,200));
    theCats.push_back(anabin(1.2,1.8,6.5,50,0,200));
    theCats.push_back(anabin(1.8,2.4,6.5,50,0,200));
  }
  
  // 3 centrality intervals
  if (iplot==1) {
    theCats.push_back(anabin(0,2.4,6.5,50,0,20));
    theCats.push_back(anabin(0,2.4,6.5,50,20,60));
    theCats.push_back(anabin(0,2.4,6.5,50,60,200));
  }
  
  // 1 rapidity interval
  if (iplot==2) {
    theCats.push_back(anabin(0,2.4,6.5,50,0,200));
  }
  
  // 2 rapidity intervals for 16-004 bins
  if (iplot==3) {
    theCats.push_back(anabin(0,1.6,6.5,30,0,200));
    theCats.push_back(anabin(1.6,2.4,3.0,30,0,200));
  }
  
  
  nameTag = nameTag_base + Form("_%i",iplot);
  
  if (poiname.find("RAA")!=std::string::npos) plotRAA(theCats,xaxis,workDirName);
  else if (poiname.find("BF")!=std::string::npos && !plotPsi2S) plotBF(theCats,xaxis,workDirName);
  else if (poiname.find("XS")!=std::string::npos && !plotPsi2S) plotXS(theCats,xaxis,workDirName);
  else {
    cout << "[ERROR] : The observable you want to plot is not supported" << endl;
    return;
  }
  
};

void plotCent(string workDirName, string poiname, int iplot) {
  // poiname is the observable to be plotted = "RAA", "BF" and "XS" (For RAA, b-fraction and cross-section)
  
  string xaxis = "cent";
  vector<anabin> theCats;
  
  // 4 rapidity intervals
  if (iplot==0) {
    theCats.push_back(anabin(0,0.6,6.5,50,0,200));
    theCats.push_back(anabin(0.6,1.2,6.5,50,0,200));
    theCats.push_back(anabin(1.2,1.8,6.5,50,0,200));
    theCats.push_back(anabin(1.8,2.4,6.5,50,0,200));
  }
  
  // 1 rapidity interval
  if (iplot==1) {
    theCats.push_back(anabin(0,2.4,6.5,50,0,200));
  }
  
  // fwd and low pt
  if (iplot==2) {
    theCats.push_back(anabin(1.8,2.4,3,6.5,0,200));
    theCats.push_back(anabin(1.8,2.4,6.5,50,0,200));
  }
  
  // 2 rapidity intervals for 16-004 bins
  if (iplot==3) {
    theCats.push_back(anabin(0,1.6,6.5,30,0,200));
    theCats.push_back(anabin(1.6,2.4,3.0,30,0,200));
  }
  
  nameTag = nameTag_base + Form("_%i",iplot);
  if (poiname.find("RAA")!=std::string::npos) plotRAA(theCats,xaxis,workDirName);
  else if (poiname.find("BF")!=std::string::npos && !plotPsi2S) plotBF(theCats,xaxis,workDirName);
  else if (poiname.find("XS")!=std::string::npos && !plotPsi2S) plotXS(theCats,xaxis,workDirName);
  else {
    cout << "[ERROR] : The observable you want to plot is not supported" << endl;
    return;
  }
};

void plotRap(string workDirName, string poiname) {
  // poiname is the observable to be plotted = "RAA", "BF" and "XS" (For RAA, b-fraction and cross-section)
  
  string xaxis = "rap";
  vector<anabin> theCats;
  
  theCats.push_back(anabin(0,2.4,6.5,50,0,200));
  
  nameTag = nameTag_base;
  if (poiname.find("RAA")!=std::string::npos) plotRAA(theCats,xaxis,workDirName);
  else if (poiname.find("BF")!=std::string::npos && !plotPsi2S) plotBF(theCats,xaxis,workDirName);
  else if (poiname.find("XS")!=std::string::npos && !plotPsi2S) plotXS(theCats,xaxis,workDirName);
  else {
    cout << "[ERROR] : The observable you want to plot is not supported" << endl;
    return;
  }
};

void plotAll(string workDirName, string poiname) {
  // poiname is the observable to be plotted = "RAA", "BF" and "XS" (For RAA, b-fraction and cross-section)
  if (dononprompt) nameTag_base = "_nonprompt";
  if (is16004)
  {
    plotPt(workDirName,poiname,3);
    plotCent(workDirName,poiname,3);
  }
  else{
    plotPt(workDirName,poiname,0);
    plotPt(workDirName,poiname,1);
    plotPt(workDirName,poiname,2);
    if (poiname.find("XS")==std::string::npos)
    {
      plotCent(workDirName,poiname,0);
      plotCent(workDirName,poiname,1);
      plotCent(workDirName,poiname,2);
    }
    plotRap(workDirName,poiname);
  }
};

/////////////////////
// OTHER FUNCTIONS //
/////////////////////

void plotRAA(vector<anabin> thecats, string xaxis, string outputDir) {
  // thecats contains the categories. eg 0<y<1.6 and 1.6<y<2.4
  // xaxis is the variable to be plotted. "pt", "rap" or "cent"
  // outputDir is the directory to save the plots
  
  if (doprompt && dononprompt) {
    cout << "ERROR you can't set both doprompt and dononprompt to true." << endl;
    return;
  }
  
  TFile *f = new TFile(treeFileName(outputDir.c_str(),"DATA"));
  if (!f || !f->IsOpen()) {
    results2tree(outputDir.c_str(),"DATA");
    f = new TFile(treeFileName(outputDir.c_str(),"DATA"));
    if (!f) return;
  }
  TTree *tr = (TTree*) f->Get("fitresults");
  if (!tr) return;
  
  TString sTag("16025");
  if (is16004) sTag = "16004";
  
  map<anabin, raa_input> theVars_inputs;
  
  map<anabin, syst> syst_PP = readSyst_all("PP","NJpsi",sTag.Data());
  map<anabin, syst> syst_PbPb = readSyst_all("PbPb","NJpsi",sTag.Data());
  map<anabin, syst> syst_taa = readSyst("Systematics/csv/syst_PbPb_taa.csv");
  map<anabin, syst> syst_Nmb = readSyst("Systematics/csv/syst_PbPb_Nmb.csv");
  map<anabin, syst> syst_lumipp = readSyst("Systematics/csv/syst_PP_lumi.csv");
  map<anabin, syst> syst_trkpp = readSyst("Systematics/csv/syst_PP_trk.csv");
  map<anabin, syst> syst_trkpbpb = readSyst("Systematics/csv/syst_PbPb_trk.csv");
  map<anabin, syst> stat_PP; // for PP statistics
  map<anabin, syst> syst_glb; // for the boxes at 1
  
  vector<double> x, ex, y, ey;
  float ptmin, ptmax, ymin, ymax, centmin, centmax;
  float eff, acc, lumi, taa, ncoll;
  float val, errL=0, errH=0;
  float bfrac, bfrac_errL,bfrac_errH;
  float correl=0;
  int ival=-999;
  char collSystem[5];
  tr->SetBranchAddress("ptmin",&ptmin);
  tr->SetBranchAddress("ptmax",&ptmax);
  tr->SetBranchAddress("ymin",&ymin);
  tr->SetBranchAddress("ymax",&ymax);
  tr->SetBranchAddress("centmin",&centmin);
  tr->SetBranchAddress("centmax",&centmax);
  tr->SetBranchAddress("N_Jpsi_val",&val);
  tr->SetBranchAddress("N_Jpsi_errL",&errL);
  tr->SetBranchAddress("N_Jpsi_errH",&errH);
  tr->SetBranchAddress("collSystem",collSystem);
  if (!dononprompt)
  {
    tr->SetBranchAddress("eff_val",&eff);
    tr->SetBranchAddress("acc_val",&acc);
  }
  else
  {
    tr->SetBranchAddress("effnp_val",&eff);
    tr->SetBranchAddress("accnp_val",&acc);
  }
  tr->SetBranchAddress("lumi_val",&lumi);
  tr->SetBranchAddress("taa_val",&taa);
  tr->SetBranchAddress("ncoll_val",&ncoll);
  tr->SetBranchAddress("b_Jpsi_val",&bfrac);
  tr->SetBranchAddress("b_Jpsi_errL",&bfrac_errL);
  tr->SetBranchAddress("b_Jpsi_errH",&bfrac_errH);
  tr->SetBranchAddress("correl_N_Jpsi_vs_b_Jpsi_val",&correl);
  
  int ntr = tr->GetEntries();
  for (int i=0; i<ntr; i++) {
    tr->GetEntry(i);
    
    if (xaxis=="rap" && ((ymin==0 && ymax<=0.61 && ymax>=0.59 ) || (ymin>=0.59 && ymin<=0.61 && ymax>=1.19 && ymax <=1.21) || (ymin>=1.19 && ymin<=1.21 && ymax>=1.79 && ymax <=1.81) || (ymin>=1.79 && ymin<=1.81 && ymax>=2.39 && ymax <=2.41))) continue;
    
    anabin thebin(ymin, ymax, ptmin, ptmax, centmin, centmax);
    
    bool ispp = (TString(collSystem)=="PP");
    
    if (ispp) {
      theVars_inputs[thebin].npp = val;
      theVars_inputs[thebin].dnpp_stat = errL;
      theVars_inputs[thebin].bfracpp = bfrac;
      theVars_inputs[thebin].dbfracpp = bfrac_errL;
      theVars_inputs[thebin].systpp = syst_PP[thebin].value;
      syst thestat_PP; thestat_PP.name = "stat_PP"; thestat_PP.value = errL/val;
      stat_PP[thebin] = thestat_PP;
      theVars_inputs[thebin].statpp = thestat_PP;
      theVars_inputs[thebin].lumipp = lumi;
      theVars_inputs[thebin].effpp = eff;
      theVars_inputs[thebin].accpp = acc;
      theVars_inputs[thebin].correlpp = correl;
    } else {
      theVars_inputs[thebin].naa = val;
      theVars_inputs[thebin].dnaa_stat = errL;
      theVars_inputs[thebin].bfracaa = bfrac;
      theVars_inputs[thebin].dbfracaa = bfrac_errL;
      theVars_inputs[thebin].systaa = syst_PbPb[thebin].value;
      theVars_inputs[thebin].lumiaa = lumi;
      theVars_inputs[thebin].effaa = eff;
      theVars_inputs[thebin].accaa = acc;
      theVars_inputs[thebin].taa = taa;
      theVars_inputs[thebin].ncoll = ncoll;
      theVars_inputs[thebin].correlaa = correl;
    }
  }
  
  map<anabin, vector<anabin> > theBins;
  map<anabin, vector<double> > theVarsBinned;
  map<anabin, vector<double> > theVarsBinned_stat;
  map<anabin, vector<double> > theVarsBinned_syst;
  map<anabin, TGraphAsymmErrors* > theGraphs;
  map<anabin, TGraphAsymmErrors* > theGraphs_syst;
  
  map<anabin, raa_input > theResults16004; //In case they are needed to compute the Psi2S Raa
  if (is16004 && plotPsi2S) theResults16004 = readResults("raa_16004.csv");
  
  // initialize the maps
  for (vector<anabin>::const_iterator it=thecats.begin(); it!=thecats.end(); it++) {
    theBins[*it] = vector<anabin>();
    theVarsBinned[*it] = vector<double>();
  }
  
  for (map<anabin, raa_input>::const_iterator it=theVars_inputs.begin(); it!=theVars_inputs.end(); it++) {
    anabin thebin = it->first;
    anabin thebinOrig = it->first; // Original bin to retrieve results later if needed (cause binok() will overwrite thebin)
    raa_input s = it->second;
    if (!binok(thecats,xaxis,thebin)) continue;
    anabin thebinPP = it->first; thebinPP.setcentbin(binI(0,200));
    raa_input spp = theVars_inputs[thebinPP];
//    cout << "Result: " << endl;
//    thebinOrig.print();
//    cout << s.naa  << " +- " << s.dnaa_stat << " ; " << spp.npp << " +- " << spp.dnpp_stat << " ; " << s.bfracaa << " ; " << spp.bfracpp << endl;
    if (s.naa <= 0 || spp.npp <= 0) continue;
    if (applyEff && (s.effaa <= 0 || spp.effpp <= 0)) continue;
    if (applyAcc && (s.accaa <= 0 || spp.accpp <= 0)) continue;
    if ((doprompt || dononprompt) && (spp.bfracpp<=0 || s.bfracaa<=0)) continue;
    
    theBins[thebin].push_back(it->first);
    
    double normfactorpp = 1., normfactoraa = 1.;
    normfactorpp = 1./spp.lumipp;
    
    if (useNcoll) {
      normfactoraa = 1./s.lumiaa;
      normfactoraa *= 1./(208.*208.*(HI::findNcollAverage(it->first.centbin().low(),it->first.centbin().high())/HI::findNcollAverage(0,200)));
    } else {
      double myNmb = NMB * s.lumiaa / lumipbpb_ABCD;
      normfactoraa = 1./(myNmb*s.taa*1e-3); // the 1e-3 factor is because taa is in mb-1 while lumis are in mub-1
    }
    normfactoraa *= 200./(it->first.centbin().high()-it->first.centbin().low());
    
    if (applyEff && applyAcc)
    {
      normfactorpp = normfactorpp / (spp.effpp*spp.accpp);
      normfactoraa = normfactoraa / (s.effaa*s.accaa);
    }
    if (applyEff)
    {
      normfactorpp = normfactorpp / (spp.effpp);
      normfactoraa = normfactoraa / (s.effaa);
    }
//    cout << "normFact = " << normfactoraa << " ; " << normfactorpp << endl;
    double naa = s.naa;
    double npp = spp.npp;
    double dnaa = s.dnaa_stat;
    double dnpp = spp.dnpp_stat;
//    cout << "correl b-NJpsi = " << spp.bfracpp << " ; " << s.correlaa << endl;
    if (doprompt) {
      naa = s.naa*(1.-s.bfracaa);
      npp = spp.npp*(1.-spp.bfracpp);
      dnaa = naa*sqrt(pow(s.dnaa_stat/s.naa,2)
                      - 2.*s.correlaa*s.dnaa_stat*s.dbfracaa/(s.naa*s.bfracaa)
                      + pow(s.dbfracaa/s.bfracaa,2));
      dnpp = npp*sqrt(pow(spp.dnpp_stat/spp.npp,2)
                      - 2.*spp.correlpp*spp.dnpp_stat*spp.dbfracpp/(spp.npp*spp.bfracpp)
                      + pow(spp.dbfracpp/spp.bfracpp,2));
    }
    if (dononprompt) {
      naa = s.naa*s.bfracaa;
      npp = spp.npp*spp.bfracpp;
      dnaa = naa*sqrt(pow(s.dnaa_stat/s.naa,2)
                      + 2.*s.correlaa*s.dnaa_stat*s.dbfracaa/(s.naa*s.bfracaa)
                      + pow(s.dbfracaa/s.bfracaa,2));
      dnpp = npp*sqrt(pow(spp.dnpp_stat/spp.npp,2)
                      + 2.*spp.correlpp*spp.dnpp_stat*spp.dbfracpp/(spp.npp*spp.bfracpp)
                      + pow(spp.dbfracpp/spp.bfracpp,2));
    }
//    cout << "after prompt = " << naa << " ; " << npp << " ; " << dnaa << " ; " << dnpp << endl;
    naa *= normfactoraa;
    npp *= normfactorpp;
    dnaa *= normfactoraa;
    dnpp *= normfactorpp;
    
    double raa = npp>0 ? naa / npp : 0;
    double draa = raa>0 ? raa*sqrt(pow(dnaa/naa,2) + pow(dnpp/npp,2)) : 0;
//    cout << it->first.ptbin().low() << " " << it->first.ptbin().high() << " -> " << raa << " " << s.naa << " " << spp.npp << " " << s.effaa << " "  << spp.effpp << " " << s.accaa << " " << s.accpp << ", " << s.bfracaa << " " << spp.bfracpp <<  " " << s.taa << endl;
    double syst = raa*sqrt(pow(spp.systpp,2)+pow(s.systaa,2));
    
    // case of the centrality dependence: factor out pp uncertainties, but include taa
    if (xaxis=="cent") {
      draa = raa*dnaa/naa;
      syst = raa*sqrt(pow(s.systaa,2)+pow(syst_taa[it->first].value,2));
    }
    
    if (is16004 && plotPsi2S) // Compute Psi2S Raa from the product of Jpsi Raa and 16-004 double ratio
    {
//      thebin.print();
      raa_input doubleR16004 = theResults16004[thebinOrig];
      
      if (doubleR16004.naa == 0) // It means that the bin has confidence intervals
      {
//        thebinOrig.print();
//        cout << "raa = " << raa << " ; dnaa_stat = " << doubleR16004.dnaa_stat << " ; systaa = " << doubleR16004.systaa << endl;
        draa = raa*doubleR16004.dnaa_stat; // Lower limit
        syst = raa*doubleR16004.systaa; // Upper limit
        raa = 0.;
        
//        cout << raa << " ; " << draa << " ; " << syst << endl;
      }
      else
      {
        draa = TMath::Sqrt(TMath::Power(doubleR16004.dnaa_stat/doubleR16004.naa,2.) + TMath::Power(draa/raa,2.));
        syst = TMath::Sqrt(TMath::Power(doubleR16004.systaa/doubleR16004.naa,2.) + TMath::Power(syst/raa,2.));
        
        raa = raa*doubleR16004.naa;
        draa = raa*draa;
        syst = raa*syst;
      }
    }
//    thebinOrig.print();
//    cout << "Raa = " << raa << " +- " << draa << " +- " << syst << endl;
    theVarsBinned[thebin].push_back(raa);
    theVarsBinned_stat[thebin].push_back(draa);
    theVarsBinned_syst[thebin].push_back(syst);
  }
  
  // systematics
  if (xaxis=="cent") { // put the PP stat error into the PP syst, that will go into a box at 1
    vector< map<anabin, syst> > all_glb;
    all_glb.push_back(syst_PP);
    all_glb.push_back(stat_PP);
    all_glb.push_back(syst_lumipp);
    all_glb.push_back(syst_Nmb);
    all_glb.push_back(syst_trkpp);
    all_glb.push_back(syst_trkpbpb);
    syst_glb = combineSyst(all_glb,"global");
  } else {
    vector< map<anabin, syst> > all_glb;
    all_glb.push_back(syst_taa);
    all_glb.push_back(syst_lumipp);
    all_glb.push_back(syst_Nmb);
    all_glb.push_back(syst_trkpp);
    all_glb.push_back(syst_trkpbpb);
    syst_glb = combineSyst(all_glb,"global");
  }
  
  // make TGraphAsymmErrors
  int cnt=0;
  for (vector<anabin>::const_iterator it=thecats.begin(); it!=thecats.end(); it++) {
    int n = theBins[*it].size();
    if(n==0) {
      cout << "Error, nothing found for category" << endl;
      theGraphs[*it] = NULL;
      continue;
    }
    
    theGraphs[*it] = new TGraphAsymmErrors(n);
    theGraphs[*it]->SetName(Form("bin_%i",cnt));
    theGraphs_syst[*it] = new TGraphAsymmErrors(n);
    theGraphs_syst[*it]->SetName(Form("bin_%i_syst",cnt));
    
    for (int i=0; i<n; i++) {
      double x=0, exl=0, exh=0, y=0, eyl=0, eyh=0;
      double exsyst=0, eysyst=0;
      double low=0, high=0;
      anabin thebin = theBins[*it][i];
      y = theVarsBinned[*it][i];
      if (xaxis=="pt" || xaxis=="rap") {
        if (xaxis=="pt") {
          low= thebin.ptbin().low();
          high = thebin.ptbin().high();
        } else {
          low= thebin.rapbin().low();
          high = thebin.rapbin().high();
        }
        x = (low+high)/2.;
        exh = (high-low)/2.;
        exl = (high-low)/2.;
        exsyst = (xaxis=="pt") ? 0.5 : 0.05;
//        eysyst = y*sqrt(pow(syst_PP[thebin].value,2) + pow(syst_PbPb[thebin].value,2)); // quadratic sum of PP and PbPb systs
//        if (plotPsi2S && (y == 0)) eysyst = theVarsBinned_syst[*it][i];
        eysyst = theVarsBinned_syst[*it][i];
      }
      if (xaxis=="cent") {
        low= thebin.centbin().low();
        high = thebin.centbin().high();
        // exl = 0.;
        // exh = 0.;
        x = (low+high)/2./2.;
        exh = (high-low)/2./2.;
        exl = (high-low)/2./2.;
        exsyst = exl;
//        eysyst = y*syst_PbPb[thebin].value; // only PbPb syst: the PP one will go to a dedicated box
//        if (plotPsi2S && (y == 0)) eysyst = theVarsBinned_syst[*it][i];
        eysyst = theVarsBinned_syst[*it][i];
      }
      eyl = fabs(theVarsBinned_stat[*it][i]);
      eyh = eyl;
      
      // eysyst = y*eysyst;
      
      theGraphs[*it]->SetPoint(i,x,y);
      theGraphs[*it]->SetPointError(i,exl,exh,eyl,eyh);
      theGraphs_syst[*it]->SetPoint(i,x,y);
      theGraphs_syst[*it]->SetPointError(i,exsyst,exsyst,eysyst,eysyst);
//       cout << "final = " << x << " " << y << " " << eyl << " " << eyh << " " << eysyst << endl;
      
      // theGraphs[*it]->Sort();
      // theGraphs_syst[*it]->Sort();
    }
    cnt++;
  }
  
  // plot
  plotGraphRAA(theGraphs, theGraphs_syst, xaxis, outputDir, syst_glb);
}

void plotXS(vector<anabin> thecats, string xaxis, string outputDir) {
  // thecats contains the categories. eg 0<y<1.6 and 1.6<y<2.4
  // xaxis is the variable to be plotted. "pt", "rap" or "cent"
  // outputDir is the directory to save the plots
  
  if (doprompt && dononprompt) {
    cout << "ERROR you can't set both doprompt and dononprompt to true." << endl;
    return;
  }
  
  if (xaxis=="cent") {
    cout << "ERROR cross-sections vs centrality not implemented" << endl;
    return;
  }
  
  TFile *f = new TFile(treeFileName(outputDir.c_str(),"DATA"));
  if (!f || !f->IsOpen()) {
    results2tree(outputDir.c_str(),"DATA");
    f = new TFile(treeFileName(outputDir.c_str(),"DATA"));
    if (!f) return;
  }
  TTree *tr = (TTree*) f->Get("fitresults");
  if (!tr) return;
  
  TString sTag("16025");
  if (is16004) sTag = "16004";
  
  map<anabin, raa_input> theVars_inputs;
  
  map<anabin, syst> syst_PP = readSyst_all("PP","NJpsi",sTag.Data());
  map<anabin, syst> syst_PbPb = readSyst_all("PbPb","NJpsi",sTag.Data());
  map<anabin, syst> syst_taa = readSyst("Systematics/csv/syst_PbPb_taa.csv");
  map<anabin, syst> syst_Nmb = readSyst("Systematics/csv/syst_PbPb_Nmb.csv");
  map<anabin, syst> syst_lumipp = readSyst("Systematics/csv/syst_PP_lumi.csv");
  map<anabin, syst> syst_trkpp = readSyst("Systematics/csv/syst_PP_trk.csv");
  map<anabin, syst> syst_trkpbpb = readSyst("Systematics/csv/syst_PbPb_trk.csv");
  map<anabin, syst> syst_glb_aa; // for the boxes at 1
  map<anabin, syst> syst_glb_pp; // for the boxes at 1
  
  vector<double> x, ex, y, ey;
  float ptmin, ptmax, ymin, ymax, centmin, centmax;
  float eff, acc, lumi, taa, ncoll;
  float val, errL=0, errH=0;
  float bfrac, bfrac_errL,bfrac_errH;
  float correl=0;
  int ival=-999;
  char collSystem[5];
  tr->SetBranchAddress("ptmin",&ptmin);
  tr->SetBranchAddress("ptmax",&ptmax);
  tr->SetBranchAddress("ymin",&ymin);
  tr->SetBranchAddress("ymax",&ymax);
  tr->SetBranchAddress("centmin",&centmin);
  tr->SetBranchAddress("centmax",&centmax);
  tr->SetBranchAddress("N_Jpsi_val",&val);
  tr->SetBranchAddress("N_Jpsi_errL",&errL);
  tr->SetBranchAddress("N_Jpsi_errH",&errH);
  tr->SetBranchAddress("collSystem",collSystem);
  if (!dononprompt)
  {
    tr->SetBranchAddress("eff_val",&eff);
    tr->SetBranchAddress("acc_val",&acc);
  }
  else
  {
    tr->SetBranchAddress("effnp_val",&eff);
    tr->SetBranchAddress("accnp_val",&acc);
  }
  tr->SetBranchAddress("lumi_val",&lumi);
  tr->SetBranchAddress("taa_val",&taa);
  tr->SetBranchAddress("ncoll_val",&ncoll);
  tr->SetBranchAddress("b_Jpsi_val",&bfrac);
  tr->SetBranchAddress("b_Jpsi_errL",&bfrac_errL);
  tr->SetBranchAddress("b_Jpsi_errH",&bfrac_errH);
  tr->SetBranchAddress("correl_N_Jpsi_vs_b_Jpsi_val",&correl);
  
  int ntr = tr->GetEntries();
  for (int i=0; i<ntr; i++) {
    tr->GetEntry(i);
    anabin thebin(ymin, ymax, ptmin, ptmax, centmin, centmax);
    
    bool ispp = (TString(collSystem)=="PP");
    
    if (ispp) {
      theVars_inputs[thebin].npp = val;
      theVars_inputs[thebin].dnpp_stat = errL;
      theVars_inputs[thebin].bfracpp = bfrac;
      theVars_inputs[thebin].dbfracpp = bfrac_errL;
      theVars_inputs[thebin].systpp = syst_PP[thebin].value;
      theVars_inputs[thebin].lumipp = lumi;
      theVars_inputs[thebin].effpp = eff;
      theVars_inputs[thebin].accpp = acc;
      theVars_inputs[thebin].correlpp = correl;
    } else {
      theVars_inputs[thebin].naa = val;
      theVars_inputs[thebin].dnaa_stat = errL;
      theVars_inputs[thebin].bfracaa = bfrac;
      theVars_inputs[thebin].dbfracaa = bfrac_errL;
      theVars_inputs[thebin].systaa = syst_PbPb[thebin].value;
      theVars_inputs[thebin].lumiaa = lumi;
      theVars_inputs[thebin].effaa = eff;
      theVars_inputs[thebin].accaa = acc;
      theVars_inputs[thebin].taa = taa;
      theVars_inputs[thebin].ncoll = ncoll;
      theVars_inputs[thebin].correlaa = correl;
    }
  }
  
  map<anabin, vector<anabin> > theBins;
  map<anabin, vector<double> > theVarsBinned_aa;
  map<anabin, vector<double> > theVarsBinned_pp;
  map<anabin, vector<double> > theVarsBinned_stat_aa;
  map<anabin, vector<double> > theVarsBinned_stat_pp;
  map<anabin, vector<double> > theVarsBinned_syst_aa;
  map<anabin, vector<double> > theVarsBinned_syst_pp;
  map<anabin, TGraphAsymmErrors* > theGraphs_aa;
  map<anabin, TGraphAsymmErrors* > theGraphs_pp;
  map<anabin, TGraphAsymmErrors* > theGraphs_syst_aa;
  map<anabin, TGraphAsymmErrors* > theGraphs_syst_pp;
  
  // initialize the maps
  for (vector<anabin>::const_iterator it=thecats.begin(); it!=thecats.end(); it++) {
    theBins[*it] = vector<anabin>();
    theVarsBinned_aa[*it] = vector<double>();
    theVarsBinned_pp[*it] = vector<double>();
  }
  
  for (map<anabin, raa_input>::const_iterator it=theVars_inputs.begin(); it!=theVars_inputs.end(); it++) {
    anabin thebin = it->first;
    raa_input s = it->second;
    if (!binok(thecats,xaxis,thebin)) continue;
    anabin thebinPP = it->first; thebinPP.setcentbin(binI(0,200));
    raa_input spp = theVars_inputs[thebinPP];
    
    if (s.naa <= 0 || spp.npp <= 0) continue;
    if (applyEff && (s.effaa <= 0 || spp.effpp <= 0)) continue;
    if (applyAcc && (s.accaa <= 0 || spp.accpp <= 0)) continue;
    if ((doprompt || dononprompt) && (spp.bfracpp<=0 || s.bfracaa<=0)) continue;
    
    theBins[thebin].push_back(it->first);
    
    double deltaPt = it->first.ptbin().high() - it->first.centbin().low();
    double deltaRap = it->first.rapbin().high() - it->first.rapbin().low();
    
    double normfactorpp = 1., normfactoraa = 1.;
    normfactorpp = 1./(spp.lumipp*deltaPt*deltaRap);
    
    if (useNcoll) {
      normfactoraa = 1./s.lumiaa;
      normfactoraa *= 1./(208.*208.*(HI::findNcollAverage(it->first.centbin().low(),it->first.centbin().high())/HI::findNcollAverage(0,200)));
    } else {
      double myNmb = NMB * s.lumiaa / lumipbpb_ABCD;
      normfactoraa = 1./(myNmb*s.taa*1e-3*deltaPt*deltaRap); // the 1e-3 factor is because taa is in mb-1 while lumis are in mub-1
    }
    normfactoraa *= 200./(it->first.centbin().high()-it->first.centbin().low());
    
    if (applyEff && applyAcc)
    {
      normfactorpp = normfactorpp / (spp.effpp*spp.accpp);
      normfactoraa = normfactoraa / (s.effaa*s.accaa);
    }
    if (applyEff)
    {
      normfactorpp = normfactorpp / (spp.effpp);
      normfactoraa = normfactoraa / (s.effaa);
    }
    
    double naa = s.naa;
    double npp = spp.npp;
    double dnaa = s.dnaa_stat;
    double dnpp = spp.dnpp_stat;
    
    if (doprompt) {
      naa = s.naa*(1.-s.bfracaa);
      npp = spp.npp*(1.-spp.bfracpp);
      dnaa = naa*sqrt(pow(s.dnaa_stat/s.naa,2)
                      - 2.*s.correlaa*s.dnaa_stat*s.dbfracaa/(s.naa*s.bfracaa)
                      + pow(s.dbfracaa/s.bfracaa,2));
      dnpp = npp*sqrt(pow(spp.dnpp_stat/spp.npp,2)
                      - 2.*spp.correlpp*spp.dnpp_stat*spp.dbfracpp/(spp.npp*spp.bfracpp)
                      + pow(spp.dbfracpp/spp.bfracpp,2));
    }
    if (dononprompt) {
      naa = s.naa*s.bfracaa;
      npp = spp.npp*spp.bfracpp;
      dnaa = naa*sqrt(pow(s.dnaa_stat/s.naa,2)
                      + 2.*s.correlaa*s.dnaa_stat*s.dbfracaa/(s.naa*s.bfracaa)
                      + pow(s.dbfracaa/s.bfracaa,2));
      dnpp = npp*sqrt(pow(spp.dnpp_stat/spp.npp,2)
                      + 2.*spp.correlpp*spp.dnpp_stat*spp.dbfracpp/(spp.npp*spp.bfracpp)
                      + pow(spp.dbfracpp/spp.bfracpp,2));
    }
    
    
    double xsec_aa = naa*normfactoraa;
    double delta_xsec_aa = dnaa*normfactoraa;
    
    double xsec_pp = npp*normfactorpp;
    double delta_xsec_pp = dnpp*normfactorpp;
    
    double syst_xsec_aa = xsec_aa*sqrt(pow(s.systaa,2));
    double syst_xsec_pp = xsec_pp*sqrt(pow(spp.systpp,2));
    
    theVarsBinned_aa[thebin].push_back(xsec_aa);
    theVarsBinned_pp[thebin].push_back(xsec_pp);
    theVarsBinned_stat_aa[thebin].push_back(delta_xsec_aa);
    theVarsBinned_stat_pp[thebin].push_back(delta_xsec_pp);
    theVarsBinned_syst_aa[thebin].push_back(syst_xsec_aa);
    theVarsBinned_syst_pp[thebin].push_back(syst_xsec_pp);
  }
  
  // systematics
  vector< map<anabin, syst> > all_glb_aa;
  all_glb_aa.push_back(syst_taa);
  all_glb_aa.push_back(syst_Nmb);
  all_glb_aa.push_back(syst_trkpbpb);
  syst_glb_aa = combineSyst(all_glb_aa,"global");
  
  vector< map<anabin, syst> > all_glb_pp;
  all_glb_pp.push_back(syst_lumipp);
  all_glb_pp.push_back(syst_trkpp);
  syst_glb_pp = combineSyst(all_glb_pp,"global");
  
  // make TGraphAsymmErrors
  int cnt=0;
  for (vector<anabin>::const_iterator it=thecats.begin(); it!=thecats.end(); it++) {
    int n = theBins[*it].size();
    if(n==0) {
      cout << "Error, nothing found for category" << endl;
      theGraphs_aa[*it] = NULL;
      theGraphs_pp[*it] = NULL;
      continue;
    }
    
    theGraphs_aa[*it] = new TGraphAsymmErrors(n);
    theGraphs_aa[*it]->SetName(Form("bin_%i_aa",cnt));
    theGraphs_syst_aa[*it] = new TGraphAsymmErrors(n);
    theGraphs_syst_aa[*it]->SetName(Form("bin_%i_syst_aa",cnt));
    
    theGraphs_pp[*it] = new TGraphAsymmErrors(n);
    theGraphs_pp[*it]->SetName(Form("bin_%i_pp",cnt));
    theGraphs_syst_pp[*it] = new TGraphAsymmErrors(n);
    theGraphs_syst_pp[*it]->SetName(Form("bin_%i_syst_pp",cnt));
    
    for (int i=0; i<n; i++) {
      double x=0, exl=0, exh=0, yaa=0, eylaa=0, eyhaa=0, ypp=0, eylpp=0, eyhpp=0;
      double exsyst=0;
      double eysyst_aa=0, eysyst_pp=0;
      double low=0, high=0;
      anabin thebin = theBins[*it][i];
      yaa = theVarsBinned_aa[*it][i];
      ypp = theVarsBinned_pp[*it][i];
      if (xaxis=="pt" || xaxis=="rap") {
        if (xaxis=="pt") {
          low= thebin.ptbin().low();
          high = thebin.ptbin().high();
        } else {
          low= thebin.rapbin().low();
          high = thebin.rapbin().high();
        }
        x = (low+high)/2.;
        exh = (high-low)/2.;
        exl = (high-low)/2.;
        exsyst = (xaxis=="pt") ? 0.5 : 0.05;
//        eysyst_aa = yaa*sqrt(pow(syst_PbPb[thebin].value,2)); // PbPb systs
//        eysyst_pp = ypp*sqrt(pow(syst_PP[thebin].value,2)); // PP systs
        eysyst_aa = theVarsBinned_syst_aa[*it][i];
        eysyst_pp = theVarsBinned_syst_pp[*it][i];
      }
      eylaa = fabs(theVarsBinned_stat_aa[*it][i]);
      eyhaa = eylaa;
      eylpp = fabs(theVarsBinned_stat_pp[*it][i]);
      eyhpp = eylpp;
      
      theGraphs_aa[*it]->SetPoint(i,x,yaa);
      theGraphs_aa[*it]->SetPointError(i,exl,exh,eylaa,eyhaa);
      theGraphs_syst_aa[*it]->SetPoint(i,x,yaa);
      theGraphs_syst_aa[*it]->SetPointError(i,exsyst,exsyst,eysyst_aa,eysyst_aa);
      // cout << x << " " << yaa << " " << eylaa << " " << eyhaa << " " << eysyst_aa << endl;
      
      theGraphs_pp[*it]->SetPoint(i,x,ypp);
      theGraphs_pp[*it]->SetPointError(i,exl,exh,eylpp,eyhpp);
      theGraphs_syst_pp[*it]->SetPoint(i,x,ypp);
      theGraphs_syst_pp[*it]->SetPointError(i,exsyst,exsyst,eysyst_pp,eysyst_pp);
      // cout << x << " " << ypp << " " << eylpp << " " << eyhpp << " " << eysyst_pp << endl;
      
      // theGraphs[*it]->Sort();
      // theGraphs_syst[*it]->Sort();
    }
    cnt++;
  }
  
  // plot
  plotGraphXS(theGraphs_aa, theGraphs_syst_aa, xaxis, outputDir, syst_glb_aa,"pbpb");
  plotGraphXS(theGraphs_pp, theGraphs_syst_pp, xaxis, outputDir, syst_glb_pp,"pp");
}

void plotBF(vector<anabin> thecats, string xaxis, string outputDir) {
  // thecats contains the categories. eg 0<y<1.6 and 1.6<y<2.4
  // xaxis is the variable to be plotted. "pt", "rap" or "cent"
  // outputDir is the directory to save the plots
  
  if (doprompt && dononprompt) {
    cout << "ERROR you can't set both doprompt and dononprompt to true." << endl;
    return;
  }
  
  TFile *f = new TFile(treeFileName(outputDir.c_str(),"DATA"));
  if (!f || !f->IsOpen()) {
    results2tree(outputDir.c_str(),"DATA");
    f = new TFile(treeFileName(outputDir.c_str(),"DATA"));
    if (!f) return;
  }
  TTree *tr = (TTree*) f->Get("fitresults");
  if (!tr) return;
  
  TString sTag("16025");
  if (is16004) sTag = "16004";
  
  map<anabin, raa_input> theVars_inputs;
  
  map<anabin, syst> syst_PP = readSyst_all("PP","BJpsi",sTag.Data());
  map<anabin, syst> syst_PbPb = readSyst_all("PbPb","BJpsi",sTag.Data());
  
  vector<double> x, ex, y, ey;
  float ptmin, ptmax, ymin, ymax, centmin, centmax;
  float val, err=0;
  float bfrac, bfrac_err;
  float correl=0;
  int ival=-999;
  char collSystem[5];
  tr->SetBranchAddress("ptmin",&ptmin);
  tr->SetBranchAddress("ptmax",&ptmax);
  tr->SetBranchAddress("ymin",&ymin);
  tr->SetBranchAddress("ymax",&ymax);
  tr->SetBranchAddress("centmin",&centmin);
  tr->SetBranchAddress("centmax",&centmax);
  tr->SetBranchAddress("collSystem",collSystem);
  tr->SetBranchAddress("b_Jpsi_val",&bfrac);
  tr->SetBranchAddress("b_Jpsi_errL",&bfrac_err);
  
  int ntr = tr->GetEntries();
  for (int i=0; i<ntr; i++) {
    tr->GetEntry(i);
    anabin thebin(ymin, ymax, ptmin, ptmax, centmin, centmax);
    
    bool ispp = (TString(collSystem)=="PP");
    
    if (ispp) {
      theVars_inputs[thebin].bfracpp = bfrac;
      theVars_inputs[thebin].dbfracpp = bfrac_err;
      theVars_inputs[thebin].systbfracpp = syst_PP[thebin].value;
    } else {
      theVars_inputs[thebin].bfracaa = bfrac;
      theVars_inputs[thebin].dbfracaa = bfrac_err;
      theVars_inputs[thebin].systbfracaa = syst_PbPb[thebin].value;
    }
  }
  
  map<anabin, vector<anabin> > theBins;
  map<anabin, vector<double> > theVarsBinned_aa;
  map<anabin, vector<double> > theVarsBinned_pp;
  map<anabin, vector<double> > theVarsBinned_stat_aa;
  map<anabin, vector<double> > theVarsBinned_stat_pp;
  map<anabin, vector<double> > theVarsBinned_syst_aa;
  map<anabin, vector<double> > theVarsBinned_syst_pp;
  map<anabin, TGraphAsymmErrors* > theGraphs_aa;
  map<anabin, TGraphAsymmErrors* > theGraphs_pp;
  map<anabin, TGraphAsymmErrors* > theGraphs_syst_aa;
  map<anabin, TGraphAsymmErrors* > theGraphs_syst_pp;
  
  // initialize the maps
  for (vector<anabin>::const_iterator it=thecats.begin(); it!=thecats.end(); it++) {
    theBins[*it] = vector<anabin>();
    theVarsBinned_aa[*it] = vector<double>();
    theVarsBinned_pp[*it] = vector<double>();
  }
  
  for (map<anabin, raa_input>::const_iterator it=theVars_inputs.begin(); it!=theVars_inputs.end(); it++) {
    anabin thebin = it->first;
    raa_input s = it->second;
    if (!binok(thecats,xaxis,thebin)) continue;
    anabin thebinPP = it->first; thebinPP.setcentbin(binI(0,200));
    raa_input spp = theVars_inputs[thebinPP];
    
    if ((doprompt || dononprompt) && (spp.bfracpp<=0 || s.bfracaa<=0)) continue;
    
    theBins[thebin].push_back(it->first);
    
    double bf_aa = s.bfracaa;
    double delta_bf_aa = s.dbfracaa;
    
    double bf_pp = spp.bfracpp;
    double delta_bf_pp = spp.dbfracpp;
    
    double syst_bf_aa = bf_aa*sqrt(pow(s.systbfracaa,2));
    double syst_bf_pp = bf_pp*sqrt(pow(spp.systbfracpp,2));
    
    theVarsBinned_aa[thebin].push_back(bf_aa);
    theVarsBinned_pp[thebin].push_back(bf_pp);
    theVarsBinned_stat_aa[thebin].push_back(delta_bf_aa);
    theVarsBinned_stat_pp[thebin].push_back(delta_bf_pp);
    theVarsBinned_syst_aa[thebin].push_back(syst_bf_aa);
    theVarsBinned_syst_pp[thebin].push_back(syst_bf_pp);
  }
  
  // make TGraphAsymmErrors
  int cnt=0;
  for (vector<anabin>::const_iterator it=thecats.begin(); it!=thecats.end(); it++) {
    int n = theBins[*it].size();
    if(n==0) {
      cout << "Error, nothing found for category" << endl;
      theGraphs_aa[*it] = NULL;
      theGraphs_pp[*it] = NULL;
      continue;
    }
    
    theGraphs_aa[*it] = new TGraphAsymmErrors(n);
    theGraphs_aa[*it]->SetName(Form("bin_%i_aa",cnt));
    theGraphs_syst_aa[*it] = new TGraphAsymmErrors(n);
    theGraphs_syst_aa[*it]->SetName(Form("bin_%i_syst_aa",cnt));
    
    theGraphs_pp[*it] = new TGraphAsymmErrors(n);
    theGraphs_pp[*it]->SetName(Form("bin_%i_pp",cnt));
    theGraphs_syst_pp[*it] = new TGraphAsymmErrors(n);
    theGraphs_syst_pp[*it]->SetName(Form("bin_%i_syst_pp",cnt));
    
    for (int i=0; i<n; i++) {
      double x=0, exl=0, exh=0, yaa=0, eylaa=0, eyhaa=0, ypp=0, eylpp=0, eyhpp=0;
      double exsyst=0;
      double eysyst_aa=0, eysyst_pp=0;
      double low=0, high=0;
      anabin thebin = theBins[*it][i];
      yaa = theVarsBinned_aa[*it][i];
      ypp = theVarsBinned_pp[*it][i];
      if (xaxis=="pt" || xaxis=="rap") {
        if (xaxis=="pt") {
          low= thebin.ptbin().low();
          high = thebin.ptbin().high();
        } else {
          low= thebin.rapbin().low();
          high = thebin.rapbin().high();
        }
        x = (low+high)/2.;
        exh = (high-low)/2.;
        exl = (high-low)/2.;
        exsyst = (xaxis=="pt") ? 0.5 : 0.05;
//        eysyst_aa = yaa*sqrt(pow(syst_PbPb[thebin].value,2)); // PbPb systs
//        eysyst_pp = ypp*sqrt(pow(syst_PP[thebin].value,2)); // PP systs
        eysyst_aa = theVarsBinned_syst_aa[*it][i];
        eysyst_pp = theVarsBinned_syst_pp[*it][i];
      }
      eylaa = fabs(theVarsBinned_stat_aa[*it][i]);
      eyhaa = eylaa;
      eylpp = fabs(theVarsBinned_stat_pp[*it][i]);
      eyhpp = eylpp;
      
      theGraphs_aa[*it]->SetPoint(i,x,yaa);
      theGraphs_aa[*it]->SetPointError(i,exl,exh,eylaa,eyhaa);
      theGraphs_syst_aa[*it]->SetPoint(i,x,yaa);
      theGraphs_syst_aa[*it]->SetPointError(i,exsyst,exsyst,eysyst_aa,eysyst_aa);
      // cout << x << " " << yaa << " " << eylaa << " " << eyhaa << " " << eysyst_aa << endl;
      
      theGraphs_pp[*it]->SetPoint(i,x,ypp);
      theGraphs_pp[*it]->SetPointError(i,exl,exh,eylpp,eyhpp);
      theGraphs_syst_pp[*it]->SetPoint(i,x,ypp);
      theGraphs_syst_pp[*it]->SetPointError(i,exsyst,exsyst,eysyst_pp,eysyst_pp);
      // cout << x << " " << ypp << " " << eylpp << " " << eyhpp << " " << eysyst_pp << endl;
      
      // theGraphs[*it]->Sort();
      // theGraphs_syst[*it]->Sort();
    }
    cnt++;
  }
  
  // plot
  plotGraphBF(theGraphs_aa, theGraphs_syst_aa, xaxis, outputDir,"pbpb");
  plotGraphBF(theGraphs_pp, theGraphs_syst_pp, xaxis, outputDir,"pp");
}

void plotGraphRAA(map<anabin, TGraphAsymmErrors*> theGraphs, map<anabin, TGraphAsymmErrors*> theGraphs_syst, string xaxis, string outputDir, map<anabin, syst> gsyst) {
  setTDRStyle();
  
  const char* ylabel = "R_{AA}";
  
  vector<anabin> theCats;
  
  TCanvas *c1 = NULL;
  c1 = new TCanvas("c1","c1",600,600);
  
  // in the case of the centrality dependence, we need the minimum bias panel on the right
  // the axes
  TH1F *haxes=NULL; TLine line;
  if (xaxis=="pt") {
    haxes = new TH1F("haxes","haxes",1,doLogPt ? 3 : 0,is16004 ? 30 : 50);
    line = TLine(doLogPt ? 3 : 0,1,is16004 ? 30 : 50,1);
    if (doLogPt) c1->SetLogx();
  }
  if (xaxis=="rap") {
    haxes = new TH1F("haxes","haxes",1,0,2.4);
    line = TLine(0,1,2.4,1);
  }
  if (xaxis=="cent") {
    haxes = new TH1F("haxes","haxes",1,0,420);
    haxes->GetXaxis()->SetTickLength(gStyle->GetTickLength("X"));
    line = TLine(0,1,420,1);
  }
  haxes->GetYaxis()->SetRangeUser(0,1.5);
  haxes->GetYaxis()->SetTitle(ylabel);
  const char* xlabel = (xaxis=="pt") ? "p_{T} (GeV/c)" : ((xaxis=="rap") ? "|y|" : "N_{part}");
  haxes->GetXaxis()->SetTitle(xlabel);
  haxes->Draw();
  line.Draw();
  
  double xshift=0.;
  if (xaxis=="cent") xshift=0.05;
  TLegend *tleg = new TLegend(0.16+xshift,0.73,0.50,0.89);
  tleg->SetBorderSize(0);
  tleg->SetTextSize(0.03);
  
  // prepare for the printing of the result tables
  const char* xname = (xaxis=="cent") ? "Centrality" : (xaxis=="pt" ? "\\pt" : "$|y|$");
  gSystem->mkdir(Form("Output/%s/tex/", outputDir.c_str()), kTRUE);
  char texname[2048]; sprintf(texname, "Output/%s/tex/result_%s_RAA_%s%s%s_%s.tex",outputDir.c_str(),plotPsi2S ? "Psi2S" : "JPsi",xaxis.c_str(),nameTag.c_str(), (xaxis=="pt") ? (doLogPt ? "_logX" :"_linearX") : "",applyEff ? (applyAcc ? "accEffCorr" : "effCorr") : "noCorr");
  string yname(plotPsi2S ? "$\\raa (\\psiP)$" : "$\\raa (\\Jpsi)$");
  inittex(texname, xname, yname);
  
  int cnt=0;
  map<anabin, TGraphAsymmErrors*>::const_iterator it=theGraphs.begin();
  map<anabin, TGraphAsymmErrors*>::const_iterator it_syst=theGraphs_syst.begin();
  for (; it!=theGraphs.end(); it++) {
    anabin thebin = it->first;
    TGraphAsymmErrors* tg = it->second;
    TGraphAsymmErrors* tg_syst = it_syst->second;
    if (!tg || !tg_syst) continue;
    
    theCats.push_back(thebin);
    
    // if needed, draw 14-005 first
    if (plot14005) {
      TString name14005 = Form("/afs/cern.ch/work/m/miheejo/public/2015JpsiRAA5TeV/276TeVRaa/histRaa/makeRaa_%s.root",xaxis=="rap" ? "y" : xaxis.c_str());
      TString name14005_syst = Form("/afs/cern.ch/work/m/miheejo/public/2015JpsiRAA5TeV/276TeVRaa/histSyst/raaSystUncert_%s.root",xaxis=="rap" ? "y" : xaxis.c_str());
      TFile *fraa = new TFile(name14005);
      TFile *fraa_syst = new TFile(name14005_syst);
      if (fraa && fraa_syst) {
        // the main graph
        name14005 = Form("g%sJpsi",dononprompt ? "NonPr" : "Pr");
        if (thebin != anabin(0,2.4,6.5,50,0,200) && thebin != anabin(0,2.4,6.5,30,0,200)) {
          int ptlowi = thebin.ptbin().low()*10;
          if (ptlowi == ((int) thebin.ptbin().low())*10) ptlowi = thebin.ptbin().low();
          int pthighi = (thebin.ptbin().high()<30) ? thebin.ptbin().high() : 30;
          int raplowi = thebin.rapbin().low()*10;
          int raphighi = thebin.rapbin().high()*10;
          name14005 += Form("_pt%i%iy%i%i", ptlowi, pthighi, raplowi, raphighi);
        }
        TGraphErrors *tg = (TGraphErrors*) fraa->Get(name14005);
        
        // the syst graph
        name14005 = Form("g%sJpsiSyst",dononprompt ? "NonPr" : "Pr");
        if (thebin != anabin(0,2.4,6.5,50,0,200) && thebin != anabin(0,2.4,6.5,30,0,200)) {
          int ptlowi = thebin.ptbin().low()*10;
          if (ptlowi == ((int) thebin.ptbin().low())*10) ptlowi = thebin.ptbin().low();
          int pthighi = (thebin.ptbin().high()<30) ? thebin.ptbin().high() : 30;
          int raplowi = thebin.rapbin().low()*10;
          int raphighi = thebin.rapbin().high()*10;
          name14005 += Form("_pt%i%iy%i%i", ptlowi, pthighi, raplowi, raphighi);
        }
        TGraphErrors *tg_syst = (TGraphErrors*) fraa_syst->Get(name14005);
        
        if (tg && tg_syst) {
          
          // the syst box
          name14005 = Form("lumi_%s_y024_pt6530",dononprompt ? "npr" : "pr");
          if (xaxis == "pt" || xaxis == "rap") name14005 = "lumi";
          TBox *b14005 = (TBox*) fraa_syst->Get(name14005);
          double glob_14005;
          if (!b14005) glob_14005 = 0;
          else glob_14005 = b14005->GetY2() - 1;
          
          // now we have all the ingredients! Let's draw them.
          tg->SetMarkerStyle(markerstyle(cnt+4));
          tg->SetMarkerColor(13+cnt);
          tg->SetLineColor(13+cnt);
          tg_syst->SetLineColor(13+cnt);
          tg_syst->SetFillColorAlpha(13+cnt, 0.5);
          tg->SetMarkerSize(1.5);
          tg->SetLineWidth(tg->GetLineWidth()*2);
          
          if (xaxis=="cent") {
            // do not plot wide centrality bins
            prune(tg, tg_syst);
          }
          tg_syst->Draw("5");
          gStyle->SetEndErrorSize(5);
          tg->Draw("P");
          // tg->Draw("[]");
          
          TString raplabel = Form("%.1f < |y| < %.1f, ",it->first.rapbin().low(),it->first.rapbin().high());
          if (it->first.rapbin().low()<0.1) raplabel = Form("|y| < %.1f, ",it->first.rapbin().high());
          TString otherlabel = "";
          if (xaxis == "pt") otherlabel.Form("%i-%i%s",(int) (it->first.centbin().low()/2.), (int) (it->first.centbin().high()/2.), "%");
          if (xaxis == "cent" || xaxis == "rap") otherlabel.Form("%g < p_{T} < %g GeV/c",it->first.ptbin().low(), it->first.ptbin().high());
          tleg->AddEntry(tg, (raplabel + otherlabel + ", #sqrt{s_{NN}}=2.76TeV"), "p");
          
          // in the case where the centrality dependence is plotted: treat the PP uncertainties as global systematics
          double x, dx, y, dy;
          if (xaxis=="cent") {
            dx = 10;
          } else if (xaxis=="pt") {
            dx = 1.25;
          } else if (xaxis=="rap") {
            dx = 0.06;
          }
          x = 2*dx*cnt + dx;
          y = 1;
          dy = glob_14005;
          TBox *tbox = new TBox(x-dx,y-dy,x+dx,y+dy);
          tbox->SetFillColorAlpha(13+cnt, 1);
          tbox->SetLineColor(kBlack);
          tbox->Draw("l");
          TBox *tboxl = (TBox*) tbox->Clone("tboxl");
          tboxl->SetFillStyle(0);
          tboxl->Draw("l");
          
          cnt++;
        } // if (tg && tg_syst)
      } // if (fraa && fraa_syst)
      
      if (fraa) delete fraa;
      if (fraa_syst) delete fraa_syst;
    } // if (plot14005)
    
    
    tg->SetMarkerStyle(markerstyle(cnt));
    tg->SetMarkerColor(color(cnt));
    tg->SetLineColor(color(cnt));
    tg_syst->SetLineColor(color(cnt));
    tg_syst->SetFillColorAlpha(color(cnt)-11, 1);
    tg->SetMarkerSize(1.5);
    tg->SetLineWidth(tg->GetLineWidth()*2);
    
    if (xaxis=="cent") {
      // do not plot wide centrality bins
      prune(tg, tg_syst);
    }
    if (is16004 && plotPsi2S)
    {
      Int_t nPoints = tg->GetN();
      double x, y, exl, exh;
//      double x_sys, y_sys;
      double liml, limh;
      for (Int_t i = 0 ; i < nPoints ; i++)
      {
        tg->GetPoint(i,x,y);
        if (y == 0)
        {
          exl = tg->GetErrorXlow(i);
          exh = tg->GetErrorXhigh(i);
          
          liml = tg->GetErrorYhigh(i);
//          tg_syst->GetPoint(i,x_sys,y_sys);
          limh = tg_syst->GetErrorYhigh(i);;
          
          double dx(0.);
          if (xaxis=="cent")
          {
            x = HI::findNpartAverage(2.*(x-exl),2.*(x+exh));
            dx =10;
          }
          if (xaxis=="pt") dx = 0.5;
          
          drawArrow(x, liml, limh, dx, color(cnt));
          tg->SetPoint(i,-1000.,0);
          tg->SetPointError(i,0,0,0,0);
          tg_syst->SetPoint(i,-1000.,0);
          tg_syst->SetPointError(i,0,0,0,0);
            
          //delete point in res and syst
        }
      }
    }
    tg_syst->Draw("5");
    gStyle->SetEndErrorSize(5);
    tg->Draw("P");
    // tg->Draw("[]");
    
    TString raplabel = Form("%.1f < |y| < %.1f, ",it->first.rapbin().low(),it->first.rapbin().high());
    if (it->first.rapbin().low()<0.1) raplabel = Form("|y| < %.1f, ",it->first.rapbin().high());
    TString otherlabel = "";
    if (xaxis == "pt") otherlabel.Form("%i-%i%s",(int) (it->first.centbin().low()/2.), (int) (it->first.centbin().high()/2.), "%");
    if (xaxis == "cent" || xaxis == "rap") otherlabel.Form("%g < p_{T} < %g GeV/c",it->first.ptbin().low(), it->first.ptbin().high());
    tleg->AddEntry(tg, (raplabel + otherlabel), "p");
    
    // print tex
    ostringstream oss;
    oss.precision(1); oss.setf(ios::fixed);
    oss << "$" << it->first.rapbin().low() << "<|y|<" << it->first.rapbin().high() << "$, ";
    if (xaxis == "pt") oss << (int) (it->first.centbin().low()/2.) << "\\% - " << (int) (it->first.centbin().high()/2.) << "\\%";
    if (xaxis == "cent" || xaxis == "rap") oss << "$" << it->first.ptbin().low() << "<\\pt<" << it->first.ptbin().high() << "\\GeVc $";
    
    addline(texname,oss.str());
    printGraph(tg, tg_syst, texname);
    
    // for the centrality dependence: we want Npart plotted, not the centrality
    if (xaxis == "cent") {
      centrality2npart(tg, false, (150./1.6)*it->first.rapbin().low());
      centrality2npart(tg_syst, true, (150./1.6)*it->first.rapbin().low());
    }
    
    // in the case where the centrality dependence is plotted: treat the PP uncertainties as global systematics
    // if (xaxis == "cent") {
    double x, dx, y, dy;
    if (xaxis=="cent") {
      dx = 10;
    } else if (xaxis=="pt") {
      dx = 1.25;
    } else if (xaxis=="rap") {
      dx = 0.06;
    }
    x = 2*dx*cnt + dx;
    y = 1;
    anabin thebinglb(it->first.rapbin().low(),
                     it->first.rapbin().high(),
                     it->first.ptbin().low(),
                     it->first.ptbin().high(),
                     0,200);
    dy = gsyst[thebinglb].value;
    cout << "global syst: " << dy << endl;
    TBox *tbox = new TBox(x-dx,y-dy,x+dx,y+dy);
    tbox->SetFillColorAlpha(color(cnt)-11, 1);
    tbox->SetLineColor(color(cnt));
    tbox->Draw("l");
    TBox *tboxl = (TBox*) tbox->Clone("tboxl");
    tboxl->SetFillStyle(0);
    tboxl->Draw("l");
    // }
    
    cnt++;
    it_syst++;
  }
  
  
  tleg->Draw();
  
  TLatex tl;
  double tlx = 0.92;
  double tly = 0.69;
  tl.SetTextAlign(32); // right adjusted
  if (doprompt) tl.DrawLatexNDC(tlx,tly,plotPsi2S ? "Prompt #psi(2S)" : "Prompt J/#psi");
  if (dononprompt) tl.DrawLatexNDC(tlx,tly,"Nonprompt J/#psi");
  
  int iPos = 33;
  CMS_lumi( (TPad*) gPad, 106, iPos, "" );
  // CMS_lumi( (TPad*) gPad, 103, iPos, "" );
  
  c1->cd();
  c1->Update();
  c1->RedrawAxis();
  gSystem->mkdir(Form("Output/%s/plot/RESULT/root/", outputDir.c_str()), kTRUE);
  c1->SaveAs(Form("Output/%s/plot/RESULT/root/result_%s_RAA_%s%s%s_%s.root",outputDir.c_str(), plotPsi2S ? "Psi2S" : "JPsi", xaxis.c_str(), nameTag.c_str(),(xaxis=="pt") ? (doLogPt ? "_logX" :"_linearX") : "",applyEff ? (applyAcc ? "accEffCorr" : "effCorr") : "noCorr"));
  gSystem->mkdir(Form("Output/%s/plot/RESULT/png/", outputDir.c_str()), kTRUE);
  c1->SaveAs(Form("Output/%s/plot/RESULT/png/result_%s_RAA_%s%s%s_%s.png",outputDir.c_str(), plotPsi2S ? "Psi2S" : "JPsi", xaxis.c_str(), nameTag.c_str(),(xaxis=="pt") ? (doLogPt ? "_logX" :"_linearX") : "",applyEff ? (applyAcc ? "accEffCorr" : "effCorr") : "noCorr"));
  gSystem->mkdir(Form("Output/%s/plot/RESULT/pdf/", outputDir.c_str()), kTRUE);
  c1->SaveAs(Form("Output/%s/plot/RESULT/pdf/result_%s_RAA_%s%s%s_%s.pdf",outputDir.c_str(), plotPsi2S ? "Psi2S" : "JPsi", xaxis.c_str(), nameTag.c_str(),(xaxis=="pt") ? (doLogPt ? "_logX" :"_linearX") : "",applyEff ? (applyAcc ? "accEffCorr" : "effCorr") : "noCorr"));
  
  delete tleg;
  delete haxes;
  delete c1;
  
  // close tex
  closetex(texname);
  cout << "Closed " << texname << endl;
}

void plotGraphXS(map<anabin, TGraphAsymmErrors*> theGraphs, map<anabin, TGraphAsymmErrors*> theGraphs_syst, string xaxis, string outputDir, map<anabin, syst> gsyst, const char* collName) {
  setTDRStyle();
  
  const char* ylabel = "";
  TString scollName(collName);
  bool isPP = scollName.Contains("pp");
  
  vector<anabin> theCats;
  
  TCanvas *c1 = NULL;
  c1 = new TCanvas("c1","c1",600,600);
  
  // in the case of the centrality dependence, we need the minimum bias panel on the right
  // the axes
  TH1F *haxes=NULL;
  if (xaxis=="pt") {
    ylabel = "B #times d#sigma/dp_{T} (#mub / GeV/#it{c})";
    haxes = new TH1F("haxes","haxes",1,doLogPt ? 3 : 0,is16004 ? 30 : 50);
    if (doLogPt) c1->SetLogx();
  }
  if (xaxis=="rap") {
    //    ylabel = "B #cdot #frac{d#sigma}{d#it{y}} (#mu b)";
    ylabel = "B #times d#sigma/d#it{y} (#mub)";
    haxes = new TH1F("haxes","haxes",1,0,2.4);
  }
  if (xaxis=="cent") {
    cout << "ERROR centrality plots are not implemented for cross section" << endl;
    return;
  }
  haxes->GetYaxis()->SetRangeUser(0.0000001,100);
  c1->SetLogy();
  haxes->GetYaxis()->SetTitle(ylabel);
  //  haxes->GetYaxis()->SetTitleSize(0.05);
  const char* xlabel = (xaxis=="pt") ? "p_{T} (GeV/c)" : "|y|";
  haxes->GetXaxis()->SetTitle(xlabel);
  haxes->Draw();
  
  double xshift=0.;
  //  if (xaxis=="cent") xshift=0.05;
  TLegend *tleg = new TLegend(0.16+xshift,0.73,0.50,0.89);
  tleg->SetBorderSize(0);
  tleg->SetTextSize(0.03);
  
  // prepare for the printing of the result tables
  const char* xname = xaxis=="pt" ? "\\pt" : "$|y|$";
  gSystem->mkdir(Form("Output/%s/tex/", outputDir.c_str()), kTRUE);
  char texname[2048]; sprintf(texname, "Output/%s/tex/result_%s_XS_%s_%s%s%s_%s.tex",outputDir.c_str(),"JPsi", scollName.Data(),xaxis.c_str(),nameTag.c_str(), (xaxis=="pt") ? (doLogPt ? "_logX" :"_linearX") : "",applyEff ? (applyAcc ? "accEffCorr" : "effCorr") : "noCorr");
  string yname(Form("$\\dsd%s_%s (\\Jpsi)$",xaxis.c_str(),scollName.Data()));
  inittex(texname, xname, yname);
  
  int cnt=0;
  map<anabin, TGraphAsymmErrors*>::const_iterator it=theGraphs.begin();
  map<anabin, TGraphAsymmErrors*>::const_iterator it_syst=theGraphs_syst.begin();
  for (; it!=theGraphs.end(); it++) {
    anabin thebin = it->first;
    TGraphAsymmErrors* tg = it->second;
    TGraphAsymmErrors* tg_syst = it_syst->second;
    if (!tg || !tg_syst) continue;
    
    theCats.push_back(thebin);
    
    tg->SetMarkerStyle(markerstyle(cnt));
    tg->SetMarkerColor(color(cnt));
    tg->SetLineColor(color(cnt));
    tg_syst->SetLineColor(color(cnt));
    tg_syst->SetFillColorAlpha(color(cnt)-11, 1);
    tg->SetMarkerSize(1.5);
    tg->SetLineWidth(tg->GetLineWidth()*2);
    
    tg_syst->Draw("5");
    gStyle->SetEndErrorSize(5);
    tg->Draw("P");
    // tg->Draw("[]");
    
    TString raplabel = Form("%.1f < |y| < %.1f, ",it->first.rapbin().low(),it->first.rapbin().high());
    if (it->first.rapbin().low()<0.1) raplabel = Form("|y| < %.1f, ",it->first.rapbin().high());
    TString otherlabel = "";
    if (xaxis == "pt") otherlabel.Form("%i-%i%s",(int) (it->first.centbin().low()/2.), (int) (it->first.centbin().high()/2.), "%");
    if (xaxis == "cent" || xaxis == "rap") otherlabel.Form("%g < p_{T} < %g GeV/c",it->first.ptbin().low(), it->first.ptbin().high());
    tleg->AddEntry(tg, (raplabel + otherlabel), "p");
    
    // print tex
    ostringstream oss;
    oss.precision(1); oss.setf(ios::fixed);
    oss << "$" << it->first.rapbin().low() << "<|y|<" << it->first.rapbin().high() << "$, ";
    if (xaxis == "pt") oss << (int) (it->first.centbin().low()/2.) << "\\% - " << (int) (it->first.centbin().high()/2.) << "\\%";
    
    addline(texname,oss.str());
    printGraph(tg, tg_syst, texname);
    
    // in the case where the centrality dependence is plotted: treat the PP uncertainties as global systematics
    // if (xaxis == "cent") {
    double x, dx, y, dy;
    if (xaxis=="pt") {
      dx = 1.25;
    } else if (xaxis=="rap") {
      dx = 0.06;
    }
    x = 2*dx*cnt + dx;
    y = 1;
    anabin thebinglb(it->first.rapbin().low(),
                     it->first.rapbin().high(),
                     it->first.ptbin().low(),
                     it->first.ptbin().high(),
                     0,200);
    //    dy = gsyst[thebinglb].value;
    //    cout << "global syst: " << dy << endl;
    //    TBox *tbox = new TBox(x-dx,y-dy,x+dx,y+dy);
    //    tbox->SetFillColorAlpha(color(cnt)-11, 1);
    //    tbox->SetLineColor(color(cnt));
    //    tbox->Draw("l");
    //    TBox *tboxl = (TBox*) tbox->Clone("tboxl");
    //    tboxl->SetFillStyle(0);
    //    tboxl->Draw("l");
    // }
    
    cnt++;
    it_syst++;
  }
  
  
  tleg->Draw();
  
  TLatex tl;
  double tlx = 0.92;
  double tly = 0.69;
  tl.SetTextAlign(32); // right adjusted
  if (doprompt) tl.DrawLatexNDC(tlx,tly,"Prompt J/#psi");
  if (dononprompt) tl.DrawLatexNDC(tlx,tly,"Nonprompt J/#psi");
  
  int iPos = 33;
  CMS_lumi( (TPad*) gPad, isPP ? 107 : 108, iPos, "" );
  // CMS_lumi( (TPad*) gPad, 103, iPos, "" );
  
  c1->cd();
  c1->Update();
  c1->RedrawAxis();
  gSystem->mkdir(Form("Output/%s/plot/RESULT/root/", outputDir.c_str()), kTRUE);
  c1->SaveAs(Form("Output/%s/plot/RESULT/root/result_%s_XS_%s_%s%s%s_%s.root",outputDir.c_str(), "JPsi", scollName.Data(), xaxis.c_str(), nameTag.c_str(),(xaxis=="pt") ? (doLogPt ? "_logX" :"_linearX") : "",applyEff ? (applyAcc ? "accEffCorr" : "effCorr") : "noCorr"));
  gSystem->mkdir(Form("Output/%s/plot/RESULT/png/", outputDir.c_str()), kTRUE);
  c1->SaveAs(Form("Output/%s/plot/RESULT/png/result_%s_XS_%s_%s%s%s_%s.png",outputDir.c_str(), "JPsi", scollName.Data(), xaxis.c_str(), nameTag.c_str(),(xaxis=="pt") ? (doLogPt ? "_logX" :"_linearX") : "",applyEff ? (applyAcc ? "accEffCorr" : "effCorr") : "noCorr"));
  gSystem->mkdir(Form("Output/%s/plot/RESULT/pdf/", outputDir.c_str()), kTRUE);
  c1->SaveAs(Form("Output/%s/plot/RESULT/pdf/result_%s_XS_%s_%s%s%s_%s.pdf",outputDir.c_str(), "JPsi", scollName.Data(), xaxis.c_str(), nameTag.c_str(),(xaxis=="pt") ? (doLogPt ? "_logX" :"_linearX") : "",applyEff ? (applyAcc ? "accEffCorr" : "effCorr") : "noCorr"));
  
  delete tleg;
  delete haxes;
  delete c1;
  
  // close tex
  closetex(texname);
  cout << "Closed " << texname << endl;
}

void plotGraphBF(map<anabin, TGraphAsymmErrors*> theGraphs, map<anabin, TGraphAsymmErrors*> theGraphs_syst, string xaxis, string outputDir, const char* collName) {
  setTDRStyle();
  
  const char* ylabel = "non prompt fraction";
  TString scollName(collName);
  bool isPP = scollName.Contains("pp");
  
  vector<anabin> theCats;
  
  TCanvas *c1 = NULL;
  c1 = new TCanvas("c1","c1",600,600);
  
  // in the case of the centrality dependence, we need the minimum bias panel on the right
  // the axes
  TH1F *haxes=NULL;
  if (xaxis=="pt") {
    haxes = new TH1F("haxes","haxes",1,0,50);
    haxes = new TH1F("haxes","haxes",1,doLogPt ? 3 : 0,is16004 ? 30 : 50);
    if (doLogPt) c1->SetLogx();
  }
  if (xaxis=="rap") {
    haxes = new TH1F("haxes","haxes",1,0,2.4);
  }
  if (xaxis=="cent") {
    haxes = new TH1F("haxes","haxes",1,0,420);
    haxes->GetXaxis()->SetTickLength(gStyle->GetTickLength("X"));
  }
  haxes->GetYaxis()->SetRangeUser(0,0.8);
  haxes->GetYaxis()->SetTitle(ylabel);
  const char* xlabel = (xaxis=="pt") ? "p_{T} (GeV/c)" : ((xaxis=="rap") ? "|y|" : "N_{part}");
  haxes->GetXaxis()->SetTitle(xlabel);
  haxes->Draw();
  
  double xshift=0.;
  if (xaxis=="cent") xshift=0.05;
  TLegend *tleg = new TLegend(0.16+xshift,0.73,0.50,0.89);
  tleg->SetBorderSize(0);
  tleg->SetTextSize(0.03);
  
  // prepare for the printing of the result tables
  const char* xname = (xaxis=="cent") ? "Centrality" : (xaxis=="pt" ? "\\pt" : "$|y|$");
  gSystem->mkdir(Form("Output/%s/tex/", outputDir.c_str()), kTRUE);
  char texname[2048]; sprintf(texname, "Output/%s/tex/result_%s_BF_%s_%s%s%s_%s.tex",outputDir.c_str(),"JPsi", scollName.Data(),xaxis.c_str(),nameTag.c_str(), (xaxis=="pt") ? (doLogPt ? "_logX" :"_linearX") : "",applyEff ? (applyAcc ? "accEffCorr" : "effCorr") : "noCorr");
  string yname("$\\raa (\\Jpsi)$");
  inittex(texname, xname, yname);
  
  int cnt=0;
  map<anabin, TGraphAsymmErrors*>::const_iterator it=theGraphs.begin();
  map<anabin, TGraphAsymmErrors*>::const_iterator it_syst=theGraphs_syst.begin();
  for (; it!=theGraphs.end(); it++) {
    anabin thebin = it->first;
    TGraphAsymmErrors* tg = it->second;
    TGraphAsymmErrors* tg_syst = it_syst->second;
    if (!tg || !tg_syst) continue;
    
    theCats.push_back(thebin);
    
    // if needed, draw 14-005 first
    if (plot14005) {
      TString name14005 = Form("/afs/cern.ch/work/m/miheejo/public/2015JpsiRAA5TeV/276TeVRaa/histRaa/makeRaa_%s.root",xaxis=="rap" ? "y" : xaxis.c_str());
      TString name14005_syst = Form("/afs/cern.ch/work/m/miheejo/public/2015JpsiRAA5TeV/276TeVRaa/histSyst/raaSystUncert_%s.root",xaxis=="rap" ? "y" : xaxis.c_str());
      TFile *fraa = new TFile(name14005);
      TFile *fraa_syst = new TFile(name14005_syst);
      if (fraa && fraa_syst) {
        // the main graph
        name14005 = Form("g%sJpsi",dononprompt ? "NonPr" : "Pr");
        if (thebin != anabin(0,2.4,6.5,50,0,200) && thebin != anabin(0,2.4,6.5,30,0,200)) {
          int ptlowi = thebin.ptbin().low()*10;
          if (ptlowi == ((int) thebin.ptbin().low())*10) ptlowi = thebin.ptbin().low();
          int pthighi = (thebin.ptbin().high()<30) ? thebin.ptbin().high() : 30;
          int raplowi = thebin.rapbin().low()*10;
          int raphighi = thebin.rapbin().high()*10;
          name14005 += Form("_pt%i%iy%i%i", ptlowi, pthighi, raplowi, raphighi);
        }
        TGraphErrors *tg = (TGraphErrors*) fraa->Get(name14005);
        
        // the syst graph
        name14005 = Form("g%sJpsiSyst",dononprompt ? "NonPr" : "Pr");
        if (thebin != anabin(0,2.4,6.5,50,0,200) && thebin != anabin(0,2.4,6.5,30,0,200)) {
          int ptlowi = thebin.ptbin().low()*10;
          if (ptlowi == ((int) thebin.ptbin().low())*10) ptlowi = thebin.ptbin().low();
          int pthighi = (thebin.ptbin().high()<30) ? thebin.ptbin().high() : 30;
          int raplowi = thebin.rapbin().low()*10;
          int raphighi = thebin.rapbin().high()*10;
          name14005 += Form("_pt%i%iy%i%i", ptlowi, pthighi, raplowi, raphighi);
        }
        TGraphErrors *tg_syst = (TGraphErrors*) fraa_syst->Get(name14005);
        
        if (tg && tg_syst) {
          
          // the syst box
          name14005 = Form("lumi_%s_y024_pt6530",dononprompt ? "npr" : "pr");
          if (xaxis == "pt" || xaxis == "rap") name14005 = "lumi";
          TBox *b14005 = (TBox*) fraa_syst->Get(name14005);
          double glob_14005;
          if (!b14005) glob_14005 = 0;
          else glob_14005 = b14005->GetY2() - 1;
          
          // now we have all the ingredients! Let's draw them.
          tg->SetMarkerStyle(markerstyle(cnt+4));
          tg->SetMarkerColor(13+cnt);
          tg->SetLineColor(13+cnt);
          tg_syst->SetLineColor(13+cnt);
          tg_syst->SetFillColorAlpha(13+cnt, 0.5);
          tg->SetMarkerSize(1.5);
          tg->SetLineWidth(tg->GetLineWidth()*2);
          
          if (xaxis=="cent") {
            // do not plot wide centrality bins
            prune(tg, tg_syst);
          }
          tg_syst->Draw("5");
          gStyle->SetEndErrorSize(5);
          tg->Draw("P");
          // tg->Draw("[]");
          
          TString raplabel = Form("%.1f < |y| < %.1f, ",it->first.rapbin().low(),it->first.rapbin().high());
          if (it->first.rapbin().low()<0.1) raplabel = Form("|y| < %.1f, ",it->first.rapbin().high());
          TString otherlabel = "";
          if (xaxis == "pt") otherlabel.Form("%i-%i%s",(int) (it->first.centbin().low()/2.), (int) (it->first.centbin().high()/2.), "%");
          if (xaxis == "cent" || xaxis == "rap") otherlabel.Form("%g < p_{T} < %g GeV/c",it->first.ptbin().low(), it->first.ptbin().high());
          tleg->AddEntry(tg, (raplabel + otherlabel + ", #sqrt{s_{NN}}=2.76TeV"), "p");
          
          // in the case where the centrality dependence is plotted: treat the PP uncertainties as global systematics
          double x, dx, y, dy;
          if (xaxis=="cent") {
            dx = 10;
          } else if (xaxis=="pt") {
            dx = 1.25;
          } else if (xaxis=="rap") {
            dx = 0.06;
          }
          x = 2*dx*cnt + dx;
          y = 1;
          dy = glob_14005;
          TBox *tbox = new TBox(x-dx,y-dy,x+dx,y+dy);
          tbox->SetFillColorAlpha(13+cnt, 1);
          tbox->SetLineColor(kBlack);
          tbox->Draw("l");
          TBox *tboxl = (TBox*) tbox->Clone("tboxl");
          tboxl->SetFillStyle(0);
          tboxl->Draw("l");
          
          cnt++;
        } // if (tg && tg_syst)
      } // if (fraa && fraa_syst)
      
      if (fraa) delete fraa;
      if (fraa_syst) delete fraa_syst;
    } // if (plot14005)
    
    
    tg->SetMarkerStyle(markerstyle(cnt));
    tg->SetMarkerColor(color(cnt));
    tg->SetLineColor(color(cnt));
    tg_syst->SetLineColor(color(cnt));
    tg_syst->SetFillColorAlpha(color(cnt)-11, 1);
    tg->SetMarkerSize(1.5);
    tg->SetLineWidth(tg->GetLineWidth()*2);
    
    if (xaxis=="cent") {
      // do not plot wide centrality bins
      prune(tg, tg_syst);
    }
    tg_syst->Draw("5");
    gStyle->SetEndErrorSize(5);
    tg->Draw("P");
    // tg->Draw("[]");
    
    TString raplabel = Form("%.1f < |y| < %.1f, ",it->first.rapbin().low(),it->first.rapbin().high());
    if (it->first.rapbin().low()<0.1) raplabel = Form("|y| < %.1f, ",it->first.rapbin().high());
    TString otherlabel = "";
    if (xaxis == "pt") otherlabel.Form("%i-%i%s",(int) (it->first.centbin().low()/2.), (int) (it->first.centbin().high()/2.), "%");
    if (xaxis == "cent" || xaxis == "rap") otherlabel.Form("%g < p_{T} < %g GeV/c",it->first.ptbin().low(), it->first.ptbin().high());
    tleg->AddEntry(tg, (raplabel + otherlabel), "p");
    
    // print tex
    ostringstream oss;
    oss.precision(1); oss.setf(ios::fixed);
    oss << "$" << it->first.rapbin().low() << "<|y|<" << it->first.rapbin().high() << "$, ";
    if (xaxis == "pt") oss << (int) (it->first.centbin().low()/2.) << "\\% - " << (int) (it->first.centbin().high()/2.) << "\\%";
    if (xaxis == "cent" || xaxis == "rap") oss << "$" << it->first.ptbin().low() << "<\\pt<" << it->first.ptbin().high() << "\\GeVc $";
    
    addline(texname,oss.str());
    printGraph(tg, tg_syst, texname);
    
    // for the centrality dependence: we want Npart plotted, not the centrality
    if (xaxis == "cent") {
      centrality2npart(tg, false, (150./1.6)*it->first.rapbin().low());
      centrality2npart(tg_syst, true, (150./1.6)*it->first.rapbin().low());
    }
    
    // in the case where the centrality dependence is plotted: treat the PP uncertainties as global systematics
    // if (xaxis == "cent") {
    double x, dx, y, dy;
    if (xaxis=="cent") {
      dx = 10;
    } else if (xaxis=="pt") {
      dx = 1.25;
    } else if (xaxis=="rap") {
      dx = 0.06;
    }
    x = 2*dx*cnt + dx;
    y = 1;
    anabin thebinglb(it->first.rapbin().low(),
                     it->first.rapbin().high(),
                     it->first.ptbin().low(),
                     it->first.ptbin().high(),
                     0,200);
    // }
    
    cnt++;
    it_syst++;
  }
  
  
  tleg->Draw();
  
  //  TLatex tl;
  //  double tlx = 0.92;
  //  double tly = 0.69;
  //  tl.SetTextAlign(32); // right adjusted
  //  if (doprompt) tl.DrawLatexNDC(tlx,tly,"Prompt J/#psi");
  //  if (dononprompt) tl.DrawLatexNDC(tlx,tly,"Nonprompt J/#psi");
  
  int iPos = 33;
  CMS_lumi( (TPad*) gPad, isPP ? 107 : 108, iPos, "" );
  
  c1->cd();
  c1->Update();
  c1->RedrawAxis();
  gSystem->mkdir(Form("Output/%s/plot/RESULT/root/", outputDir.c_str()), kTRUE);
  c1->SaveAs(Form("Output/%s/plot/RESULT/root/result_%s_BF_%s_%s%s%s_%s.root",outputDir.c_str(), "JPsi", scollName.Data(), xaxis.c_str(), nameTag.c_str(),(xaxis=="pt") ? (doLogPt ? "_logX" :"_linearX") : "",applyEff ? (applyAcc ? "accEffCorr" : "effCorr") : "noCorr"));
  gSystem->mkdir(Form("Output/%s/plot/RESULT/png/", outputDir.c_str()), kTRUE);
  c1->SaveAs(Form("Output/%s/plot/RESULT/png/result_%s_BF_%s_%s%s%s_%s.png",outputDir.c_str(), "JPsi", scollName.Data(), xaxis.c_str(), nameTag.c_str(),(xaxis=="pt") ? (doLogPt ? "_logX" :"_linearX") : "",applyEff ? (applyAcc ? "accEffCorr" : "effCorr") : "noCorr"));
  gSystem->mkdir(Form("Output/%s/plot/RESULT/pdf/", outputDir.c_str()), kTRUE);
  c1->SaveAs(Form("Output/%s/plot/RESULT/pdf/result_%s_BF_%s_%s%s%s_%s.pdf",outputDir.c_str(), "JPsi", scollName.Data(), xaxis.c_str(), nameTag.c_str(),(xaxis=="pt") ? (doLogPt ? "_logX" :"_linearX") : "",applyEff ? (applyAcc ? "accEffCorr" : "effCorr") : "noCorr"));
  
  delete tleg;
  delete haxes;
  delete c1;
  
  // close tex
  closetex(texname);
  cout << "Closed " << texname << endl;
}

void centrality2npart(TGraphAsymmErrors* tg, bool issyst, double xshift) {
  int n = tg->GetN();
  for (int i=0; i<n; i++) {
    double x, y, exl, exh, exlC, exhC, eyl, eyh;
    x = tg->GetX()[i];
    if (x>0)
    {
      y = tg->GetY()[i];
      exlC = tg->GetErrorXlow(i);
      exhC = tg->GetErrorXhigh(i);
      eyl = tg->GetErrorYlow(i);
      eyh = tg->GetErrorYhigh(i);
      if (!issyst) {
        exl = HI::findNpartSyst_low(2.*(x-exlC),2.*(x+exhC));//0.;
        exh = HI::findNpartSyst_high(2.*(x-exlC),2.*(x+exhC));//0.;
      } else {
        exl = 5;
        exh = exl;
      }
      x = HI::findNpartAverage(2.*(x-exlC),2.*(x+exhC));
      tg->SetPoint(i,x,y);
      tg->SetPointError(i,exl,exh,eyl,eyh);
    }
    else
    {
      tg->SetPoint(i,-1000,0);
      tg->SetPointError(i,0,0,0,0);
    }
  }
}

int color(int i) {
  if (i==0) return kRed+2;
  else if (i==1) return kBlue+2;
  else if (i==2) return kGreen+2;
  else if (i==3) return kCyan+2;
  else if (i==4) return kMagenta+2;
  else if (i==5) return kOrange+2;
  else return kBlack;
}

int markerstyle(int i) {
  if (i==0) return kFullSquare;
  else if (i==1) return kFullCircle;
  else if (i==2) return kFullStar;
  else if (i==3) return kFullCross;
  else if (i==4) return kOpenSquare;
  else if (i==5) return kOpenCircle;
  else if (i==6) return kOpenStar;
  else return kOpenCross;
}

void setOptions(bool adoprompt, bool adononprompt, bool ais16004, bool aplotPsi2S, bool aplot14005, string anameTag_base) {
  doprompt = adoprompt;
  dononprompt = adononprompt;
  is16004 = ais16004;
  aplotPsi2S = plotPsi2S;
  plot14005 = aplot14005;
  nameTag_base = anameTag_base;
  
  if (plotPsi2S && !is16004)
  {
    cout << "[ERROR] Options not set: you want to plot Psi2 but bins are not 16004 ones" << endl;
    printOptions();
    return;
  }
  if (doprompt) nameTag_base += "_prompt";
  if (dononprompt) nameTag_base += "_nonprompt";
  if (aplotPsi2S) nameTag_base += "_Psi2S";
  else nameTag_base += "_JPsi";
  if (is16004)  nameTag_base += "_16004";
  if (plot14005) nameTag_base += "_14005";
}

void printOptions() {
  cout <<
  "doprompt = " << doprompt << ", " <<
  "dononprompt = " << dononprompt << ", " <<
  "is16004 = " << is16004 << ", " <<
  "plotPsi2S = " << plotPsi2S << ", " <<
  "plot14005 = " << plot14005 << ", " <<
  "nameTag_base = \"" << nameTag_base << "\"" <<
  endl;
}

map<anabin, raa_input > readResults(const char* resultsFile)
{
  map<anabin, raa_input> ans;
  raa_input theresult;
  
  ifstream file(resultsFile);
  if (!(file.good())) return ans;
  
  string resultname; getline(file,resultname);
  
  cout << "[INFO] Reading results : " << resultname.c_str() << endl;
  
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
      else if (cnt==6) theresult.naa = value;
      else if (cnt==7) theresult.dnaa_stat = value;
      else if (cnt==8) theresult.systaa = value;
      else if (cnt>8) {
        cout << "Warning, too many fields, I'll take the last one." << endl;
        continue;
      }
      cnt++;
    }
    anabin thebin(rapmin, rapmax, ptmin, ptmax, centmin, centmax);
    ans[thebin] = theresult;
  }
  
  file.close();
  
  return ans;
}

void drawArrow(double x, double ylow, double yhigh, double dx, Color_t color) {
  TArrow *arrow = new TArrow(x,yhigh,x,ylow<=0. ? 0.01 : ylow,0.03,ylow<=0. ? ">" : "<>");
  arrow->SetLineColor(color);
  arrow->Draw();
  if (ylow<=0.) {
    TLine *line = new TLine(x-dx,yhigh,x+dx,yhigh);
    line->SetLineColor(color);
    line->Draw();
  }
}
