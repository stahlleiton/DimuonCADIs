#include "Macros/CMS/CMS_lumi.C"
#include "Macros/CMS/tdrstyle.C"
#include "Macros/Utilities/bin.h"
#include "Macros/Utilities/bfrac.h"
#include "Macros/Utilities/EVENTUTILS.h"
#include "Macros/Utilities/initClasses.h"
#include "Macros/Utilities/resultUtils.h"
#include "Macros/Utilities/texUtils.h"
#include "Macros/Utilities/monster.h"
#include "Macros/Utilities/rappGraph.h"
#include "Systematics/syst.h"
#include "../Limits/limits.h"

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

using namespace std;

////////////////
// PARAMETERS //
////////////////

// default values given between square brackets []

#ifndef poiname_check
#define poiname_check
const char* poiname = "RFrac2Svs1S"; // for double ratios
// const char* poiname = "N_Jpsi"; // for RAA (will correct automatically for efficiency)
// const char* poiname = "N_Psi2S"; // for RAA (will correct automatically for efficiency)
#endif
const char* ylabel = "(#psi(2S)/J/#psi)_{PbPb} / (#psi(2S)/J/#psi)_{pp}";
const bool  plot12007_mid = false; // [false] plot 12-007, midrapidity
const bool  plot12007_fwd = false; // [false] plot 12-007, fwdrapidity
const bool  fiterrors     = true;  // [true]  statistical errors are from the fit
const bool  FCerrors      = false; // [false] statistical errors are from the Feldman-Cousins intervals ("limits")
const bool  promptonly    = true;  // [true]  plot the prompt only double ratio
const bool  nonpromptonly = false; // [false] plot the non-prompt only double ratio
const bool  plotlimits95  = true;  // [true]  display 95% CL limits (when the lower limit is 0)
const bool  plotsysts     = true;  // [true]  display systematics
const bool  plotrapp      = false; // [false] plot Rapp and Du's predictions
const char* nameTag="_rapp";            // [""]    can put here e.g. "_prompt", "_nonprompt", ...

const bool plot12007 = plot12007_mid || plot12007_fwd;

const double gTextSize = 0.034;

///////////////
// CONSTANTS //
///////////////

const double xfrac = 0.8; // for the 2-panel plot for the centrality dependence.
const double lumipp = 27.7e6;
const double lumipbpb_ABCD = 351;
const double lumipbpb_peri = 464;

//////////////////
// DECLARATIONS //
//////////////////

RooRealVar* poiFromFile(const char* filename, const char* token="");
// plot
void plotGraph(map<anabin, TGraphAsymmErrors*> theGraphs, map<anabin, TGraphAsymmErrors*> theGraphs_syst, string xaxis, string outputDir, map<anabin, syst> gsyst);
void plot(vector<anabin> thecats, string xaxis, string workDirName, string workDirNameFail="");
void centrality2npart(TGraphAsymmErrors* tg, bool issyst=false, bool isMB=false, double xshift=0.);
void plotLimits(map<anabin, TGraphAsymmErrors*> theGraphs, string xaxis, const char* filename="../Limits/csv/Limits_95.csv", double xshift=0, bool ULonly=true, bool isInclusive=false);
void drawArrow(double x, double ylow, double yhigh, double dx, Color_t color, int style, double xshift_text=0, double yshift_text=0.03);
bool isSignificant(map<anabin, TGraphAsymmErrors*> theGraphs, anabin thebin);


/////////////////////////////////////////////
// MAIN FUNCTIONS TO BE CALLED BY THE USER //
/////////////////////////////////////////////

void plotPt(string workDirName, string workDirNameFail="") {
   string xaxis = "pt";
   vector<anabin> theCats;
   if (!plot12007 || plot12007_mid) theCats.push_back(anabin(0,1.6,6.5,30,0,200));
   if (!plot12007 || plot12007_fwd) theCats.push_back(anabin(1.6,2.4,3,30,0,200));

   plot(theCats,xaxis,workDirName, workDirNameFail);
};

void plotCent(string workDirName, string workDirNameFail="") {
   string xaxis = "cent";
   vector<anabin> theCats;

   // centrality dependence
   if (!plot12007 || plot12007_mid) theCats.push_back(anabin(0,1.6,6.5,30,0,200));
   if (!plot12007 || plot12007_fwd) theCats.push_back(anabin(1.6,2.4,3,30,0,200));

   // for minimum bias
   if (!plot12007 || plot12007_mid) theCats.push_back(anabin(0,1.6,6.5,30,0,-200));
   if (!plot12007 || plot12007_fwd) theCats.push_back(anabin(1.6,2.4,3,30,0,-200));

   plot(theCats,xaxis,workDirName, workDirNameFail);
};

/////////////////////
// OTHER FUNCTIONS //
/////////////////////

void plot(vector<anabin> thecats, string xaxis, string outputDir, string outputDirFail) {
   // thecats contains the categories. eg 0<y<1.6 and 1.6<y<2.4
   // xaxis is the variable to be plotted. "pt", "rap" or "cent"

   TFile *f = new TFile(treeFileName(outputDir.c_str(),"DATA"));
   if (!f || !f->IsOpen()) {
      results2tree(outputDir.c_str(),"DATA");
      f = new TFile(treeFileName(outputDir.c_str(),"DATA"));
      if (!f) return;
   }
   TTree *tr = (TTree*) f->Get("fitresults");
   if (!tr) return;

   map<anabin, double> theVars_val;
   map<anabin, double> theVars_stat;
   map<anabin, double> theVars_syst;
   map<anabin,syst> stat_PP; // statistical uncertainty on PP fits (useful for the case of the centrality dependence)

   vector<double> x, ex, y, ey;
   float ptmin, ptmax, ymin, ymax, centmin, centmax;
   float val, err=0;
   int ival=-999;
   char collSystem[5];
   tr->SetBranchAddress("ptmin",&ptmin);
   tr->SetBranchAddress("ptmax",&ptmax);
   tr->SetBranchAddress("ymin",&ymin);
   tr->SetBranchAddress("ymax",&ymax);
   tr->SetBranchAddress("centmin",&centmin);
   tr->SetBranchAddress("centmax",&centmax);
   tr->SetBranchAddress(Form("%s_val",poiname),&val);
   tr->SetBranchAddress(Form("%s_err",poiname),&err);
   tr->SetBranchAddress("collSystem",collSystem);

   int ntr = tr->GetEntries();
   for (int i=0; i<ntr; i++) {
      tr->GetEntry(i);
      anabin thebin(ymin, ymax, ptmin, ptmax, centmin, centmax);

      if (TString(collSystem)=="PP") continue;

      double dR, dR_stat, dR_statPP, dR_syst;
      const char* opt = (xaxis=="cent") ? "PbPb" : ""; // for the centrality dependence, exclude PbPb errors
      if (!promptonly && !nonpromptonly) {
         dR = doubleratio_pass_nominal(outputDir.c_str(), thebin);
         dR_stat = doubleratio_pass_stat(outputDir.c_str(), thebin, "", opt);
         dR_statPP = doubleratio_pass_stat(outputDir.c_str(), thebin, "", "PP");
         dR_syst = doubleratio_pass_syst(outputDir.c_str(), thebin, "", opt);
      } else if (promptonly) {
         dR = doubleratio_prompt_nominal(outputDir.c_str(), outputDirFail.c_str(), thebin);
         dR_stat = doubleratio_prompt_stat(outputDir.c_str(), outputDirFail.c_str(), thebin, "", opt);
         dR_statPP = doubleratio_prompt_stat(outputDir.c_str(), outputDirFail.c_str(), thebin, "", "PP");
         dR_syst = doubleratio_prompt_syst(outputDir.c_str(), outputDirFail.c_str(), thebin, "", opt);
      } else {
         dR = doubleratio_nonprompt_nominal(outputDir.c_str(), outputDirFail.c_str(), thebin);
         dR_stat = doubleratio_nonprompt_stat(outputDir.c_str(), outputDirFail.c_str(), thebin, "", opt);
         dR_statPP = doubleratio_nonprompt_stat(outputDir.c_str(), outputDirFail.c_str(), thebin, "", "PP");
         dR_syst = doubleratio_nonprompt_syst(outputDir.c_str(), outputDirFail.c_str(), thebin, "", opt);
      }

      theVars_val[thebin] = dR;
      theVars_stat[thebin] = dR_stat;
      theVars_syst[thebin] = dR_syst;

      syst thestat_PP;
      thestat_PP.name = "stat_PP";
      thestat_PP.value_dR = dR_statPP;
      thestat_PP.value = thestat_PP.value_dR / dR; 
      thestat_PP.value_dR_rel = thestat_PP.value; 
      stat_PP[thebin] = thestat_PP;
   }

   map<anabin, vector<anabin> > theBins;
   map<anabin, vector<double> > theVarsBinned;
   map<anabin, vector<double> > theVarsBinned_stat;
   map<anabin, vector<double> > theVarsBinned_syst;
   map<anabin, TGraphAsymmErrors* > theGraphs;
   map<anabin, TGraphAsymmErrors* > theGraphs_syst;

   // initialize the maps
   for (vector<anabin>::const_iterator it=thecats.begin(); it!=thecats.end(); it++) {
      theBins[*it] = vector<anabin>();
      theVarsBinned[*it] = vector<double>();
   }

   for (map<anabin, double>::const_iterator it=theVars_val.begin(); it!=theVars_val.end(); it++) {
      anabin thebin = it->first;
      if (!binok(thecats,xaxis,thebin)) continue;
      theBins[thebin].push_back(it->first);
      theVarsBinned[thebin].push_back(it->second);
      theVarsBinned_stat[thebin].push_back(theVars_stat[it->first]);
      theVarsBinned_syst[thebin].push_back(theVars_stat[it->first]);
   }

   // systematics
   map<anabin, syst> syst_PP; 
   if (plotsysts) {
      if (!promptonly && !nonpromptonly) syst_PP = readSyst_all_pass("PP","",outputDir.c_str());
      else if (promptonly) syst_PP = readSyst_all_prompt("PP","",outputDir.c_str(),outputDirFail.c_str());
      else syst_PP = readSyst_all_nonprompt("PP","",outputDir.c_str(),outputDirFail.c_str());
   }
   if (xaxis=="cent") { // put the PP stat error into the PP syst, that will go into a box at 1
      vector< map<anabin, syst> > all_PP;
      all_PP.push_back(syst_PP);
      all_PP.push_back(stat_PP);
      syst_PP = combineSyst(all_PP,"statsyst_PP");
   }
   map<anabin, syst> syst_PbPb;
   if (plotsysts) {
      if (!promptonly && !nonpromptonly) syst_PbPb = readSyst_all_pass("PbPb","",outputDir.c_str());
      else if (promptonly) syst_PbPb = readSyst_all_prompt("PbPb","",outputDir.c_str(),outputDirFail.c_str());
      else syst_PbPb = readSyst_all_nonprompt("PbPb","",outputDir.c_str(),outputDirFail.c_str());
   }
   // map<anabin, syst> syst_PbPb_NP_add = readSyst("Systematics/csv/syst_PbPb_bhad_add.csv");

   // make TGraphAsymmErrors
   int cnt=0;
   for (vector<anabin>::const_iterator it=thecats.begin(); it!=thecats.end(); it++) {
      int n = theBins[*it].size();
      if(n==0) {
         cout << "Error, nothing found for category" << endl;
         theGraphs[*it] = NULL;
         continue;
      }
      bool isMB = (it->centbin().low()<=0 && it->centbin().high()<=0);
      if (isMB && n!=1) cout << "Warning, I have " << n << " bins for a MinBias category, I expected 1." << endl;

      theGraphs[*it] = new TGraphAsymmErrors(n);
      theGraphs[*it]->SetName(Form("bin_%i",cnt));
      theGraphs_syst[*it] = new TGraphAsymmErrors(n);
      theGraphs_syst[*it]->SetName(Form("bin_%i_syst",cnt));

      for (int i=0; i<n; i++) {
         double x=0, exl=0, exh=0, y=0, eyl=0, eyh=0;
         double exsyst=0, eysyst=0;
         double low=0, high=0; 
         anabin thebin = theBins[*it][i];
         if (xaxis=="pt") {
            low= thebin.ptbin().low();
            high = thebin.ptbin().high();
            x = (low+high)/2.;
            exh = (high-low)/2.;
            exl = (high-low)/2.;
            exsyst = 0.5;
            eysyst = plotsysts ? sqrt(pow(syst_PP[thebin].value_dR,2) + pow(syst_PbPb[thebin].value_dR,2)) : 0; // quadratic sum of PP and PbPb systs
         }
         if (xaxis=="cent") {
            low= thebin.centbin().low();
            high = thebin.centbin().high();
            // x = isMB ? 150 + (150./1.6)*it->rapbin().low() : HI::findNpartAverage(low,high);
            // exl = 0.;
            // exh = 0.;
            x = (low+high)/2./2.;
            exh = (high-low)/2./2.;
            exl = (high-low)/2./2.;
            // exsyst = !isMB ? 5 : 5./(1.-xfrac);
            exsyst = exl;
            eysyst = plotsysts ? syst_PbPb[thebin].value_dR : 0; // only PbPb syst: the PP one will go to a dedicated box
            if (plotsysts && isMB) eysyst = sqrt(pow(syst_PP[thebin].value_dR,2) + pow(syst_PbPb[thebin].value_dR,2));
         }
         y = theVarsBinned[*it][i];
         if (fiterrors) {
            eyl = fabs(theVarsBinned_stat[*it][i]);
            if (isMB) eyl = sqrt(pow(theVarsBinned_stat[*it][i],2) + pow(stat_PP[*it].value_dR,2));
            eyh = eyl;
         } else {
            map<anabin, limits> maplim = readLimits("../Limits/csv/Limits_68.csv");
            if (maplim.find(*it) != maplim.end()) {
               limits lim = maplim[*it];
               eyl = lim.val.first;
               eyh = lim.val.second;
            } else {
               eyl = fabs(theVarsBinned_stat[*it][i]);
               eyh = theVarsBinned_stat[*it][i];
            }
         }

         // eysyst = y*eysyst;
         // add the additive part of the NP contamination syst
         // if (plotsysts && syst_PbPb_NP_add.find(thebin) != syst_PbPb_NP_add.end()) eysyst = sqrt(pow(syst_PbPb_NP_add[thebin].value,2) + pow(eysyst,2)); 

         // if we are plotting 95% CL limits, do not plot points that touch 0
         if (y-fabs(eyl)<0) {
            y = -99.;
            eyl=0;
            eyh=0;
            eysyst=0;
         }

         theGraphs[*it]->SetPoint(i,x,y);
         theGraphs[*it]->SetPointError(i,exl,exh,eyl,eyh);
         theGraphs_syst[*it]->SetPoint(i,x,y);
         // theGraphs_syst[*it]->SetPointError(i,exsyst,exsyst,eysyst,eysyst);
         theGraphs_syst[*it]->SetPointError(i,exsyst,exsyst,eysyst,eysyst);
         // cout << x << " " << y << " " << eyl << " " << eyh << " " << eysyst << endl;

         // theGraphs[*it]->Sort();
         // theGraphs_syst[*it]->Sort();
      }
      cnt++;
   }

   // plot
   plotGraph(theGraphs, theGraphs_syst, xaxis, outputDir, syst_PP);
}

RooRealVar* poiFromFile(const char* filename, const char* token) {
   RooRealVar *ans = poiFromFile(filename,token,poiname);
   
   // case of N_Jpsi and N_Psi2S: we want the correct normalization because we'll make a RAA
   bool isjpsi = (TString(poiname) == "N_Jpsi");
   bool ispsip = (TString(poiname) == "N_Psi2S");
   if (isjpsi || ispsip) {
      bool isPP = (TString(token).Index("PP") != kNPOS);
      double normfactor = 1.;
      anabin thebin = binFromFile(filename);

      // luminosity and Ncoll
      if (isPP) {
         normfactor = 1./lumipp;
      } else {
         if (thebin.centbin().low()>=60) normfactor = 1./lumipbpb_peri;
         else normfactor = 1./lumipbpb_ABCD;
         normfactor *= 1./(208.*208.*(HI::findNcollAverage(thebin.centbin().low(),thebin.centbin().high())/HI::findNcollAverage(0,200)));
         normfactor *= 200./(thebin.centbin().high()-thebin.centbin().low());
      }

      // efficiency
      TFile *f = TFile::Open(Form("../Efficiency/files/histos_%s_%s.root", isjpsi ? "jpsi" : "psi2s", isPP ? "pp" : "pbpb"));
      bool ismid = (thebin.rapbin().low() < 1.);
      bool isptdep = (thebin.centbin() == binI(0,200));
      TH1F *hnum = (TH1F*) f->Get(Form("hnumptdepcut_%s%s", isptdep ? "pt" : "cent", ismid ? "mid" : "fwd"));
      TH1F *hden = (TH1F*) f->Get(Form("hden_%s%s", isptdep ? "pt" : "cent", ismid ? "mid" : "fwd"));
      double numval, numerr, denval, denerr;
      bool isintegrated = (isptdep && ((thebin.rapbin()==binF(0,1.6)&&thebin.ptbin()==binF(6.5,30)) || (thebin.rapbin()==binF(1.6,2.4)&&thebin.ptbin()==binF(3,30))));
      if (isintegrated) {
         numval = hnum->IntegralAndError(1,hnum->GetNbinsX(),numerr);
         denval = hden->IntegralAndError(1,hden->GetNbinsX(),denerr);
      } else {
         int ibin = hnum->FindBin((thebin.centbin().low()+thebin.centbin().high())/4.);
         if (isptdep) ibin = hnum->FindBin((thebin.ptbin().low()+thebin.ptbin().high())/2.);
         numval = hnum->GetBinContent(ibin);
         numerr = hnum->GetBinError(ibin);
         denval = hden->GetBinContent(ibin);
         denerr = hden->GetBinError(ibin);
      }
      double efficiency = numval / denval;
      normfactor = normfactor / efficiency;
      delete f;

      ans->setVal(ans->getVal()*normfactor);
      ans->setError(ans->getError()*normfactor);
   }

   return ans;
}


void plotGraph(map<anabin, TGraphAsymmErrors*> theGraphs, map<anabin, TGraphAsymmErrors*> theGraphs_syst, string xaxis, string outputDir, map<anabin, syst> gsyst) {
   setTDRStyle();
   gStyle->SetHatchesLineWidth(1);
   gStyle->SetHatchesSpacing(1);

   vector<anabin> theCats;

   TCanvas *c1 = NULL;
   if (xaxis=="cent") c1 = new TCanvas("c1","c1",600/xfrac,600);
   else c1 = new TCanvas("c1","c1",600,600);

   // in the case of the centrality dependence, we need the minimum bias panel on the right
   TPad *padl=NULL, *padr=NULL;
   if (xaxis=="cent") {
         padl = new TPad("padl_left","",0,0,xfrac,1);
         padl->SetLeftMargin(gStyle->GetPadLeftMargin()/xfrac);
         padl->SetRightMargin(0);
         // padl->SetBottomMargin(gStyle->GetPadBottomMargin());
         // padl->SetTopMargin(gStyle->GetPadTopMargin());
         padl->SetFrameBorderMode(0);
         padl->SetBorderMode(0);
         padl->SetBorderSize(0);
         padl->Draw();

         padr = new TPad("padr_right","",xfrac,0,1,1);
         padr->SetRightMargin(gStyle->GetPadRightMargin()/(1.-xfrac));
         padr->SetLeftMargin(0);
         // padr->SetBottomMargin(gStyle->GetPadBottomMargin());
         // padr->SetTopMargin(gStyle->GetPadTopMargin());
         padr->SetFrameBorderMode(0);
         padr->SetBorderMode(0);
         padr->SetBorderSize(0);
         padr->Draw();

         padl->cd();
   }

   // the axes
   TH1F *haxes=NULL, *haxesr=NULL; TLine line, liner;
   if (xaxis=="pt") {
      haxes = new TH1F("haxes","haxes",1,0,30);
      line = TLine(0,1,30,1);
   }
   if (xaxis=="cent") {
      haxes = new TH1F("haxesl","haxesl",1,0,420);
      haxes->GetXaxis()->SetTickLength(gStyle->GetTickLength("X")/xfrac);
      // haxes->GetYaxis()->SetTickLength(gStyle->GetTickLength("Y")*xfrac);
      line = TLine(0,1,420,1);
      haxesr = new TH1F("haxesr","haxesr",1,0,420);
      haxesr->GetXaxis()->SetTickLength(0);
      haxesr->GetYaxis()->SetTickLength(gStyle->GetTickLength("Y")/(1.-xfrac));
      haxesr->GetYaxis()->SetRangeUser(0,1.5);
      if (nonpromptonly) haxesr->GetYaxis()->SetRangeUser(0,2.5);
      if (plot12007_mid) haxesr->GetYaxis()->SetRangeUser(0,1.5);
      else if (plot12007_fwd) haxesr->GetYaxis()->SetRangeUser(0,4.5);
      if (plot12007_mid && plotrapp) haxesr->GetYaxis()->SetRangeUser(0,2.);
      haxesr->GetXaxis()->SetTitleSize(0);
      haxesr->GetXaxis()->SetLabelSize(0);
      liner = TLine(0,1,420,1);
   }
   haxes->GetYaxis()->SetRangeUser(0,1.5);
   if (xaxis=="pt" && plotrapp) haxes->GetYaxis()->SetRangeUser(0,2.5);
   if (nonpromptonly) haxes->GetYaxis()->SetRangeUser(0,2.5);
   if (plot12007_mid) haxes->GetYaxis()->SetRangeUser(0,1.5);
   if (plot12007_mid && plotrapp) haxes->GetYaxis()->SetRangeUser(0,2.);
   else if (plot12007_fwd) haxes->GetYaxis()->SetRangeUser(0,4.5);
   if (plot12007_fwd) haxes->GetYaxis()->SetRangeUser(0,4.5);
   haxes->GetYaxis()->SetTitle(ylabel);
   const char* xlabel = (xaxis=="pt") ? "p_{T} (GeV/c)" : "N_{part}";
   haxes->GetXaxis()->SetTitle(xlabel);
   haxes->Draw();
   line.Draw();
   if (xaxis=="cent") {
      padr->cd();
      haxesr->Draw();
      liner.Draw();
      TLatex tlr;
      tlr.SetTextSize(gTextSize*xfrac/(1.-xfrac));
      tlr.DrawLatexNDC(0.2,0.85,"Cent.");
      tlr.DrawLatexNDC(0.2,0.80,"0-100%");
      padl->cd();
   }

   double xshift=0.;
   if (xaxis=="cent") xshift=0.05;
   TLegend *tleg = new TLegend(0.16+xshift,0.73,0.50,0.89);
   tleg->SetBorderSize(0);
   tleg->SetTextSize(gTextSize);

   // plot Rapp's theory first if we have to
   if (plotrapp) {
      if (plot12007_mid) {
         // TGraphErrors *gmid_12007 = rapp_cent_mid_276();
         TGraphErrors *gmid_12007 = rapp_cent_mid_276_3bins(-5);
         gmid_12007->SetFillColorAlpha(kGray+2,1); 
         gmid_12007->SetLineColor(kGray+2); 
         gmid_12007->SetFillStyle(3354); 
         gmid_12007->Draw("5"); // was 3
         // for some reason I don't have the surrounding box... so, dirty trick.
         TGraphErrors *gmid_12007bis = (TGraphErrors*) gmid_12007->Clone("gmid_12007bis");
         gmid_12007bis->SetFillStyle(0);
         gmid_12007bis->Draw("2");
      }
      if (plot12007_fwd) {
         // TGraphErrors *gfwd_12007 = rapp_cent_fwd_276();
         TGraphErrors *gfwd_12007 = rapp_cent_fwd_276_3bins(5);
         gfwd_12007->SetFillColorAlpha(kGray+2,1); 
         gfwd_12007->SetLineColor(kGray+2); 
         gfwd_12007->SetFillStyle(3354); 
         gfwd_12007->Draw("5"); // was 3
         // for some reason I don't have the surrounding box... so, dirty trick.
         TGraphErrors *gfwd_12007bis = (TGraphErrors*) gfwd_12007->Clone("gfwd_12007bis");
         gfwd_12007bis->SetFillStyle(0);
         gfwd_12007bis->Draw("2");
      }

      TGraphErrors *gmid=NULL, *gfwd=NULL;
      TGraph *gmid_up=NULL, *gmid_down=NULL, *gfwd_up=NULL, *gfwd_down=NULL;
      if (xaxis=="cent") {
         // gmid = rapp_cent_mid_502();
         gmid = rapp_cent_mid_502_6bins(0);
         gmid_up = tge2tg(gmid,1);
         gmid_down = tge2tg(gmid,-1);
         // gfwd = rapp_cent_fwd_502();
         gfwd = rapp_cent_fwd_502_3bins(0);
         gfwd_up = tge2tg(gfwd,1);
         gfwd_down = tge2tg(gfwd,-1);
      }
      if (xaxis=="pt") {
         gmid = rapp_pt_mid_502();
         gmid_up = tge2tg(gmid,1);
         gmid_down = tge2tg(gmid,-1);
         gfwd = rapp_pt_fwd_502();
         gfwd_up = tge2tg(gfwd,1);
         gfwd_down = tge2tg(gfwd,-1);
      }
      if (gmid && !(plot12007_fwd && !plot12007_mid)) { // if asking for 2.76TeV fwd only, do not plot 5TeV mid
         gmid->SetFillColorAlpha(xaxis=="cent" ? kBlue+2 : kBlue-9,1); 
         gmid->SetLineColor(kBlue+2); 
         gmid->SetFillStyle(xaxis=="cent" ? 3345 : 1001);
         gmid->Draw(xaxis=="cent" ? "5" : "3");
         // for some reason I don't have the surrounding box... so, dirty trick.
         if (xaxis == "cent") {
            TGraphErrors *gmidbis = (TGraphErrors*) gmid->Clone("gmidbis");
            gmidbis->SetFillStyle(0);
            gmidbis->Draw("2");
         }
      }
      if (gfwd && !(plot12007_mid && !plot12007_fwd)) { // if asking for 2.76TeV mid only, do not plot 5TeV fwd
         gfwd->SetFillColorAlpha(xaxis=="cent" ? kRed+2 : kRed-9,1);
         gfwd->SetLineColor(kRed+2);
         gfwd->SetFillStyle(xaxis=="cent" ? 3345 : 1001);
         gfwd->Draw(xaxis=="cent" ? "5" : "3");
         // for some reason I don't have the surrounding box... so, dirty trick.
         if (xaxis == "cent") {
            TGraphErrors *gfwdbis = (TGraphErrors*) gfwd->Clone("gfwdbis");
            gfwdbis->SetFillStyle(0);
            gfwdbis->Draw("2");
         }
      }

      // if (plot12007_mid) {
      //    TGraphErrors *gmid_12007 = rapp_cent_mid_276();
      //    TGraph *gmid_12007_up = tge2tg(gmid_12007,1);
      //    TGraph *gmid_12007_down = tge2tg(gmid_12007,-1);
      //    gmid_12007_up->SetLineColor(kGray+2); 
      //    gmid_12007_down->SetLineColor(kGray+2); 
      //    gmid_12007_up->Draw("L");
      //    gmid_12007_down->Draw("L");
      // }
      // if (plot12007_fwd) {
      //    TGraphErrors *gfwd_12007 = rapp_cent_fwd_276();
      //    TGraph *gfwd_12007_up = tge2tg(gfwd_12007,1);
      //    TGraph *gfwd_12007_down = tge2tg(gfwd_12007,-1);
      //    gfwd_12007_up->SetLineColor(kGray+2); 
      //    gfwd_12007_down->SetLineColor(kGray+2); 
      //    gfwd_12007_up->Draw("L");
      //    gfwd_12007_down->Draw("L");
      // }
      if (gmid && !(plot12007_fwd && !plot12007_mid)) { // if asking for 2.76TeV fwd only, do not plot 5TeV mid
         gmid_up->SetLineColor(kBlue+2);
         gmid_down->SetLineColor(kBlue+2);
         gmid_up->Draw("L");
         gmid_down->Draw("L");
      }
      if (gfwd && !(plot12007_mid && !plot12007_fwd)) { // if asking for 2.76TeV mid only, do not plot 5TeV fwd
         gfwd_up->SetLineColor(kRed+2);
         gfwd_down->SetLineColor(kRed+2);
         gfwd_up->Draw("L");
         gfwd_down->Draw("L");
      }

      // now the legend
      // do it by hand because we need several colors in a single entry
      double x, y, dx, dy;
      EColor color1, color2;
      if (xaxis=="pt") {
         x = 1.;
         dx = 1.5;
         y = 1.8;
         dy = 0.08;
         color1 = kRed;
         color2 = kBlue;
      }
      if (xaxis=="cent") {
         x = 20.;
         dx = 20;
         if (plot12007_fwd) {
            y = 3.1;
            dy = 0.15;
            color1 = kRed;
            color2 = kGray;
         } else {
            y = 1.4;
            dy = 0.07;
            color1 = kBlue;
            color2 = kGray;
         }
      }
      TBox *b1 = new TBox(x,y,x+dx,y+dy);
      x = x+1.5*dx;
      TBox *b2 = new TBox(x,y,x+dx,y+dy);
      b1->SetLineColor(color1+2);
      b1->SetFillColor(xaxis=="cent" ? color1+2 : color1-9);
      b1->SetFillStyle(xaxis=="cent" ? 3345 : 1001); 
      b2->SetLineColor(color2+2);
      b2->SetFillColor(xaxis=="cent" ? color2+2 : color2-9);
      b2->SetFillStyle(xaxis=="cent" ? 3345 : 1001); 
      b1->Draw("l"); b2->Draw("l"); // "l"
      TBox *b1b = (TBox*) b1->Clone("b1b");
      b1b->SetFillStyle(0); b1b->Draw("l");
      TBox *b2b = (TBox*) b2->Clone("b2b");
      b2b->SetFillStyle(0); b2b->Draw("l");
      TLatex *tl = new TLatex(); tl->SetTextSize(gTextSize);
      // tl->DrawLatex(x+1.5*dx,y,"Du and Rapp");
      // tl->DrawLatex(x+1.5*dx,y-1.3*dy,"(NPA 943 (2015) 147 + priv. com.)");
      tl->DrawLatex(x+1.5*dx,y,"Du and Rapp (arXiv:1609.04868)");
   }

   // prepare for the printing of the result tables
   const char* xname = (xaxis=="cent") ? "Centrality" : "\\pt (\\GeVc)";
   gSystem->mkdir(Form("Output/%s/tex/", outputDir.c_str()), kTRUE); 
   char texname[2048]; sprintf(texname, "Output/%s/tex/result_%s%s.tex",outputDir.c_str(),xaxis.c_str(),nameTag);
   string yname("\\doubleRatio");
   inittex(texname, xname, yname);

   int cnt=0;
   map<anabin, TGraphAsymmErrors*>::const_iterator it=theGraphs.begin();
   map<anabin, TGraphAsymmErrors*>::const_iterator it_syst=theGraphs_syst.begin();
   for (; it!=theGraphs.end(); it++) {
      anabin thebin = it->first;
      bool isMB = (thebin.centbin().low()<=0 && thebin.centbin().high()<=0);
      TGraphAsymmErrors* tg = it->second;
      TGraphAsymmErrors* tg_syst = it_syst->second;
      if (!tg || !tg_syst) continue;

      theCats.push_back(thebin);

      if (thebin.rapbin() == binF(0.,1.6)) {
         tg->SetMarkerStyle(kFullSquare);
         tg->SetMarkerColor(kBlue+2);
         tg->SetLineColor(kBlue+2);
         tg_syst->SetLineColor(kBlue+2);
         tg_syst->SetFillColorAlpha(kBlue-9, 1);
      } else if (thebin.rapbin() == binF(1.6,2.4)) {
         tg->SetMarkerStyle(kFullCircle);
         tg->SetMarkerColor(kRed+2);
         tg->SetLineColor(kRed+2);
         tg_syst->SetLineColor(kRed+2);
         tg_syst->SetFillColorAlpha(kRed-9, 1);
      } else {
         tg->SetMarkerStyle(kOpenStar);
         tg->SetMarkerColor(kGreen);
         tg->SetLineColor(kGreen);
         tg_syst->SetLineColor(kGreen);
         tg_syst->SetFillColorAlpha(kGreen, 0.5);
      }
      tg->SetMarkerSize(1.5);
      tg->SetLineWidth(tg->GetLineWidth()); //*2);

      if (xaxis=="cent") {
         if (thebin.centbin().low()<=0 && thebin.centbin().high()<=0) padr->cd();
         else padl->cd();

         // do not plot wide centrality bins
         prune(tg, tg_syst);
      }
      tg_syst->Draw("5");      
      // gStyle->SetEndErrorSize(5);
      gStyle->SetEndErrorSize(0);
      tg->Draw("P");
      // tg->Draw("[]");

      TString raplabel = Form("%.1f < |y| < %.1f,  ",it->first.rapbin().low(),it->first.rapbin().high());
      if (it->first.rapbin().low()<0.1) raplabel = Form("|y| < %.1f,  ",it->first.rapbin().high());
      TString otherlabel = "BWAA";
      if (xaxis == "pt") otherlabel.Form("%i-%i%s",(int) (it->first.centbin().low()/2.), (int) (it->first.centbin().high()/2.), "%");
      if (xaxis == "cent") otherlabel.Form("%g < p_{T} < %g GeV/c",it->first.ptbin().low(), it->first.ptbin().high());
      if (!isMB) {
         if (!plot12007) tleg->AddEntry(tg, (raplabel + otherlabel), "p");
         else tleg->AddEntry(tg, "#sqrt{s_{NN}} = 5.02 TeV", "p");
      }

      // print tex
      ostringstream oss;
      oss.precision(1); oss.setf(ios::fixed);
      oss << "$" << it->first.rapbin().low() << "<|y|<" << it->first.rapbin().high() << "$, ";
      if (xaxis == "pt") oss << (int) (it->first.centbin().low()/2.) << "\\% - " << (int) (it->first.centbin().high()/2.) << "\\%";
      if (xaxis == "cent") oss << "$" << it->first.ptbin().low() << "<\\pt<" << it->first.ptbin().high() << "\\GeVc $";

      addline(texname,oss.str());
      printGraph(tg, tg_syst, texname);

      // for the centrality dependence: we want Npart plotted, not the centrality
      if (xaxis == "cent") {
         centrality2npart(tg, false, isMB, (150./1.6)*it->first.rapbin().low());
         centrality2npart(tg_syst, true, isMB, (150./1.6)*it->first.rapbin().low());
      }

      // in the case where the centrality dependence is plotted: treat the PP uncertainties as global systematics
      if (xaxis == "cent" && !isMB) {
         double x, dx, y, dy;
         dx = 5;
         x = 2*dx*(thebin.rapbin().low()/1.6) + dx;
         if (plot12007_fwd && !plot12007_mid) x = dx;
         y = 1;
         anabin thebin(it->first.rapbin().low(),
               it->first.rapbin().high(),
               it->first.ptbin().low(),
               it->first.ptbin().high(),
               0,200);
         dy = gsyst[thebin].value_dR_rel;
         cout << "global syst: " << dy << endl;
         TBox *tbox = new TBox(x-dx,y-dy,x+dx,y+dy);
         if (thebin.rapbin() == binF((float) 0.,1.6)) {tbox->SetFillColorAlpha(kBlue-9, 1); tbox->SetLineColor(kBlue+2);}
         else if (thebin.rapbin() == binF(1.6,2.4)) {tbox->SetFillColorAlpha(kRed-9, 1); tbox->SetLineColor(kRed+2);}
         else tbox->SetFillColorAlpha(kGreen, 0.5);
         tbox->Draw("l");
         line.Draw(); // to make sure the line at 1 is always on top
      }

      cnt++;
      it_syst++;
   }

   // now plot the 12007 if we need to
   if (plot12007 && xaxis == "cent") {
      padl->cd();
      if (plot12007_mid) {
         TGraphAsymmErrors *g_12007_mid_cent = result12007_mid_cent();
         TGraphAsymmErrors *g_12007_mid_cent_syst = result12007_mid_cent_syst();
         g_12007_mid_cent->SetMarkerStyle(kOpenStar);
         g_12007_mid_cent->SetMarkerSize(2);
         g_12007_mid_cent->SetMarkerColor(kGray+2);
         g_12007_mid_cent->SetLineColor(kGray+2);
         g_12007_mid_cent_syst->SetFillColorAlpha(kGray, 0);
         g_12007_mid_cent_syst->SetLineColor(kGray+2);
         g_12007_mid_cent_syst->Draw("5");
         g_12007_mid_cent->Draw("P");
         tleg->AddEntry(g_12007_mid_cent,"#splitline{#sqrt{s_{NN}} = 2.76 TeV}{(PRL113 (2014) 262301)}", "p");
         // draw the upper limit in the peripheral bin... by hand (ughhh dirty)
         if (!plotrapp) drawArrow(32.8-5, 0, 0.47, 10., kGray+2, 1, 25);
         else drawArrow(32.8-5, 0, 0.47, 10., kGray+2, 1, 25, -0.15);
         // global syst box
         double x, dx, y, dy;
         dx = 5;
         x = plot12007_fwd ? 5.*dx : 3.*dx;
         y = 1;
         dy = 0.057;
         TBox *tbox = new TBox(x-dx,y-dy,x+dx,y+dy);
         tbox->SetFillColorAlpha(kGray, 0);
         tbox->SetLineColor(kGray+2);
         tbox->Draw("l");
         line.Draw(); // to make sure the line at 1 is always on top
      }
      if (plot12007_fwd) {
         TGraphAsymmErrors *g_12007_fwd_cent = result12007_fwd_cent();
         TGraphAsymmErrors *g_12007_fwd_cent_syst = result12007_fwd_cent_syst();
         g_12007_fwd_cent->SetMarkerStyle(kOpenCross);
         g_12007_fwd_cent->SetMarkerSize(2);
         g_12007_fwd_cent->SetMarkerColor(kGray+2);
         g_12007_fwd_cent->SetLineColor(kGray+2);
         g_12007_fwd_cent_syst->SetFillColorAlpha(kGray, 0);
         g_12007_fwd_cent_syst->SetLineColor(kGray+2);
         g_12007_fwd_cent_syst->Draw("5");
         g_12007_fwd_cent->Draw("P");
         tleg->AddEntry(g_12007_fwd_cent,"#splitline{#sqrt{s_{NN}} = 2.76 TeV}{(PRL113 (2014) 262301)}", "p");
         double x, dx, y, dy;
         dx = 5;
         x = plot12007_mid ? 7.*dx : 3.*dx;
         y = 1;
         dy = 0.066;
         TBox *tbox = new TBox(x-dx,y-dy,x+dx,y+dy);
         tbox->SetFillColorAlpha(kGray, 0);
         tbox->SetLineColor(kGray+2);
         tbox->Draw("l");
         line.Draw(); // to make sure the line at 1 is always on top
      }

      padr->cd();
      if (plot12007_mid) {
         TGraphAsymmErrors *g_12007_mid = result12007_mid();
         TGraphAsymmErrors *g_12007_mid_syst = result12007_mid_syst();
         g_12007_mid->SetMarkerStyle(kOpenStar);
         g_12007_mid->SetMarkerSize(2);
         g_12007_mid->SetMarkerColor(kGray+2);
         g_12007_mid->SetLineColor(kGray+2);
         g_12007_mid_syst->SetFillColorAlpha(kGray, 0);
         g_12007_mid_syst->SetLineColor(kGray+2);
         g_12007_mid_syst->Draw("5");
         g_12007_mid->Draw("P");
      }
      if (plot12007_fwd) {
         TGraphAsymmErrors *g_12007_fwd = result12007_fwd();
         TGraphAsymmErrors *g_12007_fwd_syst = result12007_fwd_syst();
         g_12007_fwd->SetMarkerStyle(kOpenCross);
         g_12007_fwd->SetMarkerSize(2);
         g_12007_fwd->SetMarkerColor(kGray+2);
         g_12007_fwd->SetLineColor(kGray+2);
         g_12007_fwd_syst->SetFillColorAlpha(kGray, 0);
         g_12007_fwd_syst->SetLineColor(kGray+2);
         g_12007_fwd_syst->Draw("5");
         g_12007_fwd->Draw("P");
      }

      padl->cd();
   }

   if (plotlimits95) plotLimits(theGraphs,xaxis,"../Limits/csv/Limits_95.csv",0);
   if (fiterrors && FCerrors) plotLimits(theGraphs,xaxis,"../Limits/csv/Limits_68.csv",xaxis=="cent" ? 5 : 1, false);

   // limits for inclusive
   if (xaxis=="cent") {
      padr->cd();
      if (plotlimits95) plotLimits(theGraphs,xaxis,"../Limits/csv/Limits_95.csv", 0 , true, true);
      padl->cd();
   }

   if (xaxis=="cent") padl->cd();
   tleg->Draw();

   TLatex tl;
   tl.SetTextSize(gTextSize);
//   tl.DrawLatexNDC(0.2+xshift,0.69,"#mu in acceptance");
   double tlx = 0.26;//(xaxis=="cent") ? 0.77 : 0.92-xshift;
   double tly = 0.58;//0.69;
   if (plot12007_mid && !plotrapp) tly = 0.675;
   if (plot12007_fwd && !plotrapp) tly = 0.66;
   if (plot12007_fwd && plotrapp) tly = 0.56;
   if (!plot12007 && xaxis=="cent") tly = 0.65;
   if (xaxis=="pt") tly = 0.73;
   if (xaxis=="pt" && plotrapp) {tly = 0.52; tlx = 0.72;}
   // int alignement = (xaxis=="cent") ? 11 : 31; // left ajusted for centrality, right ajusted for pt
   int alignement = 11; // left ajusted for centrality, right ajusted for pt
   tl.SetTextAlign(alignement);
   double yshift = 0.05;
   if (plot12007_mid && !plotrapp) yshift = 0.08; // 0.07
   if (!promptonly && !nonpromptonly) tl.DrawLatexNDC(tlx,tly-yshift,"Passing #font[12]{l}_{J/#psi}^{3D} cut");
   else if (promptonly) tl.DrawLatexNDC(tlx,tly-yshift,"Prompt only");
   else tl.DrawLatexNDC(tlx,tly-yshift,"Non-prompt only");

   if (plot12007 && xaxis == "cent") {
      padl->cd();
      tlx = 0.26;
      if (plot12007_mid) {
        tl.DrawLatexNDC(tlx,tly,"|y| < 1.6,  6.5 < p_{T} < 30 GeV/c");
      }
      if (plot12007_fwd) {
        tl.DrawLatexNDC(tlx,tly,"1.6 < |y| < 2.4,  3 < p_{T} < 30 GeV/c");
      }
   }

   int iPos = 33;
   CMS_lumi( (TPad*) gPad, 106, iPos, "" );
   // CMS_lumi( (TPad*) gPad, 103, iPos, "" );

   c1->cd();
   c1->Update();
   c1->RedrawAxis();
   gSystem->mkdir(Form("Output/%s/plot/RESULT/root/", outputDir.c_str()), kTRUE); 
   c1->SaveAs(Form("Output/%s/plot/RESULT/root/result_%s%s%s.root",outputDir.c_str(), xaxis.c_str(), nameTag, plot12007 ? "_12007" : ""));
   gSystem->mkdir(Form("Output/%s/plot/RESULT/png/", outputDir.c_str()), kTRUE);
   c1->SaveAs(Form("Output/%s/plot/RESULT/png/result_%s%s%s.png",outputDir.c_str(), xaxis.c_str(), nameTag, plot12007 ? "_12007" : ""));
   gSystem->mkdir(Form("Output/%s/plot/RESULT/pdf/", outputDir.c_str()), kTRUE);
   c1->SaveAs(Form("Output/%s/plot/RESULT/pdf/result_%s%s%s.pdf",outputDir.c_str(), xaxis.c_str(), nameTag, plot12007 ? "_12007" : ""));

   delete tleg;
   delete haxes; delete haxesr;
   delete padl; delete padr;
   delete c1;

   // close tex
   closetex(texname);
   cout << "Closed " << texname << endl;
}

void centrality2npart(TGraphAsymmErrors* tg, bool issyst, bool isMB, double xshift) {
   int n = tg->GetN();
   for (int i=0; i<n; i++) {
      double x, y, exl, exh, eyl, eyh;
      x = tg->GetX()[i];
      y = tg->GetY()[i];
      exl = tg->GetErrorXlow(i);
      exh = tg->GetErrorXhigh(i);
      eyl = tg->GetErrorYlow(i);
      eyh = tg->GetErrorYhigh(i);
      x = isMB ? 150 + xshift : HI::findNpartAverage(2.*(x-exl),2.*(x+exh));
      if (isMB && !plot12007_mid && plot12007_fwd) x = 200.1;
      if (!issyst) {
         if (isMB) {exl=0; exh=0;} else {
            exl = HI::findNpartSyst(2.*(x-exl),2.*(x+exh));//0.;
            exh = HI::findNpartSyst(2.*(x-exl),2.*(x+exh));//0.;
         }
      } else {
         exl = !isMB ? 5 : 5.*xfrac/(1.-xfrac);
         exh = exl;
      }
      tg->SetPoint(i,x,y);
      tg->SetPointError(i,exl,exh,eyl,eyh);
   }
}

void plotLimits(map<anabin, TGraphAsymmErrors*> theGraphs, string xaxis, const char* filename, double xshift, bool ULonly, bool isInclusive) {
   map<anabin,limits> maplim = readLimits(filename);

   vector<anabin> theCats;
   for (map<anabin, TGraphAsymmErrors*>::const_iterator it=theGraphs.begin(); it!=theGraphs.end(); it++) {
      theCats.push_back(it->first);
   }

   map<anabin,limits>::const_iterator it;
   for (it=maplim.begin(); it!=maplim.end(); it++) {
      anabin thebin = it->first;
      if (!binok(theCats,xaxis,thebin,false)) continue;
      limits lim = it->second;

      // only draw upper limits, ie interval which lower limit is 0
      // if (ULonly && lim.val.first>0) continue; 
      if (ULonly && isSignificant(theGraphs,thebin)) continue;

      bool isInclusiveBin = (xaxis=="cent" && thebin.centbin()==binI(0,200));
      if (isInclusiveBin && !isInclusive) continue;
      if (!isInclusiveBin && isInclusive) continue;
      // draw arrow in the right place and with the right color...
      Color_t color=kBlack;
      int style=1;
      double x=0, y=0, dx=0;
      double ylow = lim.val.first;
      y = lim.val.second;
      if (xaxis=="pt") {
         double low= thebin.ptbin().low();
         double high = thebin.ptbin().high();
         x = (low+high)/2.;
         dx = 0.5;
      } else if (xaxis=="cent") {
         double low= thebin.centbin().low();
         double high = thebin.centbin().high();
         x = isInclusive ? 150 + (150./1.6)*thebin.rapbin().low(): HI::findNpartAverage(low,high);
         dx = isInclusive ? 10./(1.-xfrac) : 10;
      }
      if (thebin.rapbin() == binF(0.,1.6)) {
         color = kBlue+2;
         // style = 3;
      } else if (thebin.rapbin() == binF(1.6,2.4)) {
         color = kRed+2;
         // style = 7;
      }
      double textshift=0;
      if (plot12007_fwd && !plot12007_mid) textshift = 40;
      drawArrow(x+xshift, ylow, y, dx, color, style, textshift);
   }
}

void drawArrow(double x, double ylow, double yhigh, double dx, Color_t color, int style, double xshift_text, double yshift_text) {
   TArrow *arrow = new TArrow(x,yhigh,x,ylow<=0. ? 0.05 : ylow,0.03,ylow<=0. ? ">" : "<>");
   arrow->SetLineColor(color);
   arrow->SetLineStyle(style);
   arrow->SetLineWidth(2);
   arrow->Draw();
   if (ylow<=0.) {
      TLine *line = new TLine(x-dx,yhigh,x+dx,yhigh);
      line->SetLineColor(color);
      line->SetLineWidth(2);
      line->Draw();
   }
   if (plotlimits95 && !FCerrors) {
      TLatex tl;
      tl.SetTextSize(gTextSize*0.65);
      tl.SetTextAlign(21);
      tl.SetTextColor(color);
      tl.DrawLatex(x+xshift_text,yhigh+yshift_text,"95\% CL");
   }
}

bool isSignificant(map<anabin, TGraphAsymmErrors*> theGraphs, anabin thebin) {
   bool ans=false;
   for (map<anabin, TGraphAsymmErrors*>::const_iterator it=theGraphs.begin(); it!=theGraphs.end(); it++) {
      TGraphAsymmErrors *tg = it->second;
      anabin thecat = it->first;
      for (int i=0; i<tg->GetN(); i++) {
         double x = tg->GetX()[i];
         bool yok = (tg->GetY()[i] - fabs(tg->GetEYlow()[i]))>0;
         double xbin = (thebin.ptbin().low()+thebin.ptbin().high())/2.;
         if (fabs(x-xbin)<1e-2 && yok) {ans=true; break;}
         bool isMB = (thebin.centbin() == binI(0,200));
         xbin = isMB ?  150 + (150./1.6)*thebin.rapbin().low() : HI::findNpartAverage(thebin.centbin().low(),thebin.centbin().high());
         if (isMB && !plot12007_mid && plot12007_fwd) xbin = 200.1;
         if (fabs(x-xbin)<1e-2 && yok) {ans=true; break;}
      }
   }

   return ans;
}
