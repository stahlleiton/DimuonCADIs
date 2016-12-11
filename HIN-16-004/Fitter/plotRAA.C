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

using namespace std;

////////////////
// PARAMETERS //
////////////////

#ifndef poiname_check
#define poiname_check
const char* poiname       = "N_Jpsi"; // for RAA (will correct automatically for efficiency)
#endif
const char* ylabel        = "R_{AA}";

bool  fiterrors     = true;  // statistical errors are from the fit
bool  plotsysts     = true;  // display systematics
bool  doprompt      = true;  // prompt Jpsi
bool  dononprompt   = false;  // nonprompt Jpsi
string nameTag_base = "_prompt";    // can put here e.g. "_prompt", "_nonprompt", ...

//////////////////
// DECLARATIONS //
//////////////////

void printOptions();
void setOptions(bool afiterrors, bool aplotsysts, bool adoprompt, bool adononprompt, string anameTag_base);
void plotGraph(map<anabin, TGraphAsymmErrors*> theGraphs, map<anabin, TGraphAsymmErrors*> theGraphs_syst, string xaxis, string outputDir, map<anabin, syst> gsyst);
void plot(vector<anabin> thecats, string xaxis, string workDirName);
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
      double taa;
      double lumipp;
      double lumiaa;
      double ncoll;
      double bfracpp;
      double dbfracpp;
      double bfracaa;
      double dbfracaa;
      syst   statpp;
};


/////////////////////////////////////////////
// MAIN FUNCTIONS TO BE CALLED BY THE USER //
/////////////////////////////////////////////

void plotPt(string workDirName, int iplot) {
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

   nameTag = nameTag_base + Form("_%i",iplot);
   plot(theCats,xaxis,workDirName);
};

void plotCent(string workDirName, int iplot) {
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

   nameTag = nameTag_base + Form("_%i",iplot);
   plot(theCats,xaxis,workDirName);
};

void plotRap(string workDirName) {
   string xaxis = "rap";
   vector<anabin> theCats;

   theCats.push_back(anabin(0,2.4,6.5,30,0,200));

   nameTag = nameTag_base;
   plot(theCats,xaxis,workDirName);
};

void plotAll(string workDirName) {
   plotPt(workDirName,0);
   plotPt(workDirName,1);
   plotPt(workDirName,2);
   plotCent(workDirName,0);
   plotCent(workDirName,1);
   plotCent(workDirName,2);
   plotRap(workDirName);
};

/////////////////////
// OTHER FUNCTIONS //
/////////////////////

void plot(vector<anabin> thecats, string xaxis, string outputDir) {
   // thecats contains the categories. eg 0<y<1.6 and 1.6<y<2.4
   // xaxis is the variable to be plotted. "pt", "rap" or "cent"

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

   map<anabin, raa_input> theVars_inputs;

   map<anabin, syst> syst_PP = readSyst_all("PP");
   map<anabin, syst> syst_PbPb = readSyst_all("PbPb");
   map<anabin, syst> syst_taa = readSyst("Systematics/csv/syst_PbPb_taa.csv");
   map<anabin, syst> syst_lumipp = readSyst("Systematics/csv/syst_PP_lumi.csv");
   map<anabin, syst> stat_PP; // for PP statistics
   map<anabin, syst> syst_glb; // for the boxes at 1

   vector<double> x, ex, y, ey;
   float ptmin, ptmax, ymin, ymax, centmin, centmax;
   float eff, lumi, taa, ncoll;
   float val, err=0;
   float bfrac, bfrac_err;
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
   if (!dononprompt) tr->SetBranchAddress("eff_val",&eff);
   else tr->SetBranchAddress("effnp_val",&eff);
   tr->SetBranchAddress("lumi_val",&lumi);
   tr->SetBranchAddress("taa_val",&taa);
   tr->SetBranchAddress("ncoll_val",&ncoll);
   tr->SetBranchAddress("b_Jpsi_val",&bfrac);
   tr->SetBranchAddress("b_Jpsi_err",&bfrac_err);

   int ntr = tr->GetEntries();
   for (int i=0; i<ntr; i++) {
      tr->GetEntry(i);
      anabin thebin(ymin, ymax, ptmin, ptmax, centmin, centmax);

      bool ispp = (TString(collSystem)=="PP");

      if (ispp) {
         theVars_inputs[thebin].npp = val;
         theVars_inputs[thebin].dnpp_stat = err;
         theVars_inputs[thebin].bfracpp = bfrac;
         theVars_inputs[thebin].dbfracpp = bfrac_err;
         theVars_inputs[thebin].systpp = syst_PP[thebin].value;
         syst thestat_PP; thestat_PP.name = "stat_PP"; thestat_PP.value = err/val;
         stat_PP[thebin] = thestat_PP;
         theVars_inputs[thebin].statpp = thestat_PP;
         theVars_inputs[thebin].lumipp = lumi;
         theVars_inputs[thebin].effpp = eff; 
      } else {
         theVars_inputs[thebin].naa = val;
         theVars_inputs[thebin].dnaa_stat = err;
         theVars_inputs[thebin].bfracaa = bfrac;
         theVars_inputs[thebin].dbfracaa = bfrac_err;
         theVars_inputs[thebin].systaa = syst_PbPb[thebin].value;
         theVars_inputs[thebin].lumiaa = lumi;
         theVars_inputs[thebin].effaa = eff; 
         theVars_inputs[thebin].taa = taa; 
         theVars_inputs[thebin].ncoll = ncoll; 
      }
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

   for (map<anabin, raa_input>::const_iterator it=theVars_inputs.begin(); it!=theVars_inputs.end(); it++) {
      anabin thebin = it->first;
      raa_input s = it->second;
      if (!binok(thecats,xaxis,thebin)) continue;
      anabin thebinPP = it->first; thebinPP.setcentbin(binI(0,200));
      raa_input spp = theVars_inputs[thebinPP];

      if (s.effaa <= 0 || spp.effpp <= 0 || s.naa <= 0 || spp.npp <= 0) continue;
      if ((doprompt || dononprompt) && (spp.bfracpp<=0 || s.bfracaa<=0)) continue;

      theBins[thebin].push_back(it->first);

      double normfactorpp = 1., normfactoraa = 1.;
      normfactorpp = 1./spp.lumipp;
      normfactoraa = 1./s.lumiaa;
      normfactoraa *= 1./(208.*208.*(HI::findNcollAverage(it->first.centbin().low(),it->first.centbin().high())/HI::findNcollAverage(0,200)));
      normfactoraa *= 200./(it->first.centbin().high()-it->first.centbin().low());

      normfactorpp = normfactorpp / spp.effpp;
      normfactoraa = normfactoraa / s.effaa;

      double naa = s.naa;
      double npp = spp.npp;
      double dnaa = s.dnaa_stat;
      double dnpp = spp.dnpp_stat;

      if (doprompt) {
         naa = s.naa*(1.-s.bfracaa);
         npp = spp.npp*(1.-spp.bfracpp);
         dnaa = naa*sqrt(pow(s.dnaa_stat/s.naa,2) + pow(s.dbfracaa/s.bfracaa,2));
         dnpp = npp*sqrt(pow(spp.dnpp_stat/spp.npp,2) + pow(spp.dbfracpp/spp.bfracpp,2));
      }
      if (dononprompt) {
         naa = s.naa*s.bfracaa;
         npp = spp.npp*spp.bfracpp;
         dnaa = naa*sqrt(pow(s.dnaa_stat/s.naa,2) + pow(s.dbfracaa/s.bfracaa,2));
         dnpp = npp*sqrt(pow(spp.dnpp_stat/spp.npp,2) + pow(spp.dbfracpp/spp.bfracpp,2));
      }

      naa *= normfactoraa;
      npp *= normfactorpp;
      dnaa *= normfactoraa;
      dnpp *= normfactorpp;

      double raa = npp>0 ? naa / npp : 0;
      double draa = raa>0 ? raa*sqrt(pow(dnaa/naa,2) + pow(dnpp/npp,2)) : 0;
      // cout << it->first.ptbin().low() << " " << it->first.ptbin().high() << " -> " << raa << " " << s.naa << " " << spp.npp << " " << s.effaa << " " << spp.effpp << ", " << s.bfracaa << " " << spp.bfracpp << endl;
      double syst = raa*sqrt(pow(spp.systpp,2)+pow(s.systaa,2));

      // case of the centrality dependence: factor out pp uncertainties, but include taa
      if (xaxis=="cent") {
         draa = raa*dnaa/naa;
         syst = raa*sqrt(pow(s.systaa,2)+pow(syst_taa[it->first].value,2));
      }

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
      syst_glb = combineSyst(all_glb,"global");
   } else {
      vector< map<anabin, syst> > all_glb;
      all_glb.push_back(syst_taa);
      all_glb.push_back(syst_lumipp);
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
            eysyst = plotsysts ? y*sqrt(pow(syst_PP[thebin].value,2) + pow(syst_PbPb[thebin].value,2)) : 0; // quadratic sum of PP and PbPb systs
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
            eysyst = plotsysts ? y*syst_PbPb[thebin].value : 0; // only PbPb syst: the PP one will go to a dedicated box
         }
         eyl = fabs(theVarsBinned_stat[*it][i]);
         eyh = eyl;

         // eysyst = y*eysyst;
         // add the additive part of the NP contamination syst
         // if (plotsysts && syst_PbPb_NP_add.find(thebin) != syst_PbPb_NP_add.end()) eysyst = sqrt(pow(syst_PbPb_NP_add[thebin].value,2) + pow(eysyst,2)); 

         theGraphs[*it]->SetPoint(i,x,y);
         theGraphs[*it]->SetPointError(i,exl,exh,eyl,eyh);
         theGraphs_syst[*it]->SetPoint(i,x,y);
         theGraphs_syst[*it]->SetPointError(i,exsyst,exsyst,eysyst,eysyst);
         // cout << x << " " << y << " " << eyl << " " << eyh << " " << eysyst << endl;

         // theGraphs[*it]->Sort();
         // theGraphs_syst[*it]->Sort();
      }
      cnt++;
   }

   // plot
   plotGraph(theGraphs, theGraphs_syst, xaxis, outputDir, syst_glb);
}


void plotGraph(map<anabin, TGraphAsymmErrors*> theGraphs, map<anabin, TGraphAsymmErrors*> theGraphs_syst, string xaxis, string outputDir, map<anabin, syst> gsyst) {
   setTDRStyle();

   vector<anabin> theCats;

   TCanvas *c1 = NULL;
   c1 = new TCanvas("c1","c1",600,600);

   // in the case of the centrality dependence, we need the minimum bias panel on the right
   // the axes
   TH1F *haxes=NULL; TLine line;
   if (xaxis=="pt") {
      haxes = new TH1F("haxes","haxes",1,0,50);
      line = TLine(0,1,50,1);
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
   char texname[2048]; sprintf(texname, "Output/%s/tex/result_%s%s.tex",outputDir.c_str(),xaxis.c_str(),nameTag.c_str());
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
   if (doprompt) tl.DrawLatexNDC(tlx,tly,"Prompt J/#psi");
   if (dononprompt) tl.DrawLatexNDC(tlx,tly,"Nonprompt J/#psi");

   int iPos = 33;
   CMS_lumi( (TPad*) gPad, 106, iPos, "" );
   // CMS_lumi( (TPad*) gPad, 103, iPos, "" );

   c1->cd();
   c1->Update();
   c1->RedrawAxis();
   gSystem->mkdir(Form("Output/%s/plot/RESULT/root/", outputDir.c_str()), kTRUE); 
   c1->SaveAs(Form("Output/%s/plot/RESULT/root/result_%s%s.root",outputDir.c_str(), xaxis.c_str(), nameTag.c_str()));
   gSystem->mkdir(Form("Output/%s/plot/RESULT/png/", outputDir.c_str()), kTRUE);
   c1->SaveAs(Form("Output/%s/plot/RESULT/png/result_%s%s.png",outputDir.c_str(), xaxis.c_str(), nameTag.c_str()));
   gSystem->mkdir(Form("Output/%s/plot/RESULT/pdf/", outputDir.c_str()), kTRUE);
   c1->SaveAs(Form("Output/%s/plot/RESULT/pdf/result_%s%s.pdf",outputDir.c_str(), xaxis.c_str(), nameTag.c_str()));

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
      double x, y, exl, exh, eyl, eyh;
      x = tg->GetX()[i];
      y = tg->GetY()[i];
      exl = tg->GetErrorXlow(i);
      exh = tg->GetErrorXhigh(i);
      eyl = tg->GetErrorYlow(i);
      eyh = tg->GetErrorYhigh(i);
      x = HI::findNpartAverage(2.*(x-exl),2.*(x+exh));
      if (!issyst) {
         exl = HI::findNpartSyst(2.*(x-exl),2.*(x+exh));//0.;
         exh = HI::findNpartSyst(2.*(x-exl),2.*(x+exh));//0.;
      } else {
         exl = 5;
         exh = exl;
      }
      tg->SetPoint(i,x,y);
      tg->SetPointError(i,exl,exh,eyl,eyh);
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
   else return kOpenStar;
}

void setOptions(bool afiterrors, bool aplotsysts, bool adoprompt, bool adononprompt, string anameTag_base) {
   fiterrors = afiterrors;
   plotsysts = aplotsysts;
   doprompt = adoprompt;
   dononprompt = adononprompt;
   nameTag_base = anameTag_base;
   if (doprompt) nameTag_base += "_prompt";
   if (dononprompt) nameTag_base += "_nonprompt";
}

void printOptions() {
   cout << 
      "fiterrors = " << fiterrors << ", "
      "plotsysts = " << plotsysts << ", "
      "doprompt = " << doprompt << ", "
      "dononprompt = " << dononprompt << ", "
      "nameTag_base = \"" << nameTag_base << "\"" << 
      endl;
}
