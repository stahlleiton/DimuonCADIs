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

#ifndef poiname_check
#define poiname_check
// const char* poiname = "RFrac2Svs1S"; // for double ratios
const char* poiname       = "N_Jpsi"; // for RAA (will correct automatically for efficiency)
// const char* poiname = "N_Psi2S"; // for RAA (will correct automatically for efficiency)
#endif
const char* ylabel        = "R_{AA}";
const bool  fiterrors     = true;  // statistical errors are from the fit
const bool  FCerrors      = false; // statistical errors are from the Feldman-Cousins intervals ("limits")
const bool  plotlimits95  = true;  // display 95% CL limits (when the lower limit is 0)
const bool  plotsysts     = true;  // display systematics
const char* nameTag       = "";    // can put here e.g. "_prompt", "_nonprompt", ...

//////////////////
// DECLARATIONS //
//////////////////

// plot
void plotGraph(map<anabin, TGraphAsymmErrors*> theGraphs, map<anabin, TGraphAsymmErrors*> theGraphs_syst, string xaxis, string outputDir, map<anabin, syst> gsyst);
void plot(vector<anabin> thecats, string xaxis, string workDirName);
void centrality2npart(TGraphAsymmErrors* tg, bool issyst=false, bool isMB=false, double xshift=0.);
void plotLimits(vector<anabin> theCats, string xaxis, const char* filename="../Limits/csv/Limits_95.csv", double xshift=0, bool ULonly=true);
void drawArrow(double x, double ylow, double yhigh, double dx, Color_t color);

class raa_input {
   public:
      double npp;
      double dnpp_stat;
      double dnpp_syst;
      double naa;
      double dnaa_stat;
      double dnaa_syst;
      double effpp;
      double effaa;
      double taa;
      double lumipp;
      double lumiaa;
      double ncoll;
      syst   statpp;
};


/////////////////////////////////////////////
// MAIN FUNCTIONS TO BE CALLED BY THE USER //
/////////////////////////////////////////////

void plotPt(string workDirName) {
   string xaxis = "pt";
   vector<anabin> theCats;
   theCats.push_back(anabin(0,0.6,6.5,50,0,200));
   theCats.push_back(anabin(0.6,1.2,6.5,50,0,200));
   theCats.push_back(anabin(1.2,1.8,6.5,50,0,200));
   theCats.push_back(anabin(1.8,2.4,6.5,50,0,200));

   plot(theCats,xaxis,workDirName);
};

void plotCent(string workDirName) {
   string xaxis = "cent";
   vector<anabin> theCats;

   // centrality dependence
   theCats.push_back(anabin(0,0.6,6.5,50,0,200));
   theCats.push_back(anabin(0.6,1.2,6.5,50,0,200));
   theCats.push_back(anabin(1.2,1.8,6.5,50,0,200));
   theCats.push_back(anabin(1.8,2.4,6.5,50,0,200));

   plot(theCats,xaxis,workDirName);
};

// to be implemented!
// void plotRap(string workDirName) {
// }

/////////////////////
// OTHER FUNCTIONS //
/////////////////////

void plot(vector<anabin> thecats, string xaxis, string outputDir) {
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

   map<anabin, raa_input> theVars_inputs;

   vector<double> x, ex, y, ey;
   float ptmin, ptmax, ymin, ymax, centmin, centmax;
   float eff, lumi, taa, ncoll;
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
   tr->SetBranchAddress("eff_val",&eff);
   tr->SetBranchAddress("lumi_val",&lumi);
   tr->SetBranchAddress("taa_val",&taa);
   tr->SetBranchAddress("ncoll_val",&ncoll);

   int ntr = tr->GetEntries();
   for (int i=0; i<ntr; i++) {
      tr->GetEntry(i);
      anabin thebin(ymin, ymax, ptmin, ptmax, centmin, centmax);

      bool ispp = (TString(collSystem)=="PP");

      if (ispp) {
         theVars_inputs[thebin].npp = val;
         theVars_inputs[thebin].dnpp_stat = err;
         theVars_inputs[thebin].dnpp_syst = 0; // FIXME
         syst thestat_PP; // FIXME
         theVars_inputs[thebin].statpp = thestat_PP;
         theVars_inputs[thebin].lumipp = lumi;
         theVars_inputs[thebin].effpp = eff; 
      } else {
         theVars_inputs[thebin].naa = val;
         theVars_inputs[thebin].dnaa_stat = err;
         theVars_inputs[thebin].dnaa_syst = 0; // FIXME
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
      anabin thebinPP = thebin; thebinPP.setcentbin(binI(0,200));
      theBins[thebin].push_back(it->first);

      double normfactorpp = 1., normfactoraa = 1.;
      normfactorpp = 1./s.lumipp;
      normfactoraa = 1./s.lumiaa;
      normfactoraa *= 1./(208.*208.*(HI::findNcollAverage(it->first.centbin().low(),it->first.centbin().high())/HI::findNcollAverage(0,200)));
      normfactoraa *= 200./(it->first.centbin().high()-it->first.centbin().low());

      normfactorpp = normfactorpp / s.effpp;
      normfactoraa = normfactoraa / s.effaa;

      double naa = s.naa * normfactoraa;
      double npp = s.npp * normfactorpp;
      double dnaa = s.dnaa_stat * normfactoraa;
      double dnpp = s.dnpp_stat * normfactorpp;
      double raa = npp>0 ? naa / npp : 0;
      cout << s.naa << " " << s.npp <<  " " << s.lumiaa << " " << s.lumipp << " " << s.effaa << "  " << s.effpp << " " << normfactoraa << "  " << normfactorpp << " -> " << raa << endl;
      double draa = raa>0 ? raa*sqrt(pow(dnaa/naa,2) + pow(dnpp/npp,2)) : 0;
      theVarsBinned[thebin].push_back(raa);
      theVarsBinned_stat[thebin].push_back(draa);
      theVarsBinned_syst[thebin].push_back(0); // FIXME
   }

   // systematics
   map<anabin, syst> syst_PP; 
   if (plotsysts) {
      syst_PP = readSyst_all_pass("PP","",outputDir.c_str());
   }
   if (xaxis=="cent") { // put the PP stat error into the PP syst, that will go into a box at 1
      vector< map<anabin, syst> > all_PP;
      all_PP.push_back(syst_PP);
      // all_PP.push_back(stat_PP); // FIXME
      syst_PP = combineSyst(all_PP,"statsyst_PP");
   }
   map<anabin, syst> syst_PbPb;
   if (plotsysts) {
      syst_PbPb = readSyst_all_pass("PbPb","",outputDir.c_str());
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
            exsyst = exl;
            eysyst = plotsysts ? syst_PbPb[thebin].value_dR : 0; // only PbPb syst: the PP one will go to a dedicated box
            if (plotsysts && isMB) eysyst = sqrt(pow(syst_PP[thebin].value_dR,2) + pow(syst_PbPb[thebin].value_dR,2));
         }
         y = theVarsBinned[*it][i];
         if (fiterrors) {
            eyl = fabs(theVarsBinned_stat[*it][i]);
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


void plotGraph(map<anabin, TGraphAsymmErrors*> theGraphs, map<anabin, TGraphAsymmErrors*> theGraphs_syst, string xaxis, string outputDir, map<anabin, syst> gsyst) {
   setTDRStyle();

   vector<anabin> theCats;

   TCanvas *c1 = NULL;
   c1 = new TCanvas("c1","c1",600,600);

   // in the case of the centrality dependence, we need the minimum bias panel on the right
   TPad *padl=NULL;
   // the axes
   TH1F *haxes=NULL; TLine line;
   if (xaxis=="pt") {
      haxes = new TH1F("haxes","haxes",1,0,50);
      line = TLine(0,1,50,1);
   }
   if (xaxis=="cent") {
      haxes = new TH1F("haxesl","haxesl",1,0,420);
      haxes->GetXaxis()->SetTickLength(gStyle->GetTickLength("X"));
      line = TLine(0,1,420,1);
   }
   haxes->GetYaxis()->SetRangeUser(0,1.5);
   haxes->GetYaxis()->SetTitle(ylabel);
   const char* xlabel = (xaxis=="pt") ? "p_{T} (GeV/c)" : "N_{part}";
   haxes->GetXaxis()->SetTitle(xlabel);
   haxes->Draw();
   line.Draw();

   double xshift=0.;
   if (xaxis=="cent") xshift=0.05;
   TLegend *tleg = new TLegend(0.16+xshift,0.73,0.50,0.89);
   tleg->SetBorderSize(0);
   tleg->SetTextSize(0.03);

   // prepare for the printing of the result tables
   const char* xname = (xaxis=="cent") ? "Centrality" : "\\pt";
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

      if (thebin.rapbin() == binF(0.,0.6) || thebin.centbin() == binI(0,20)) {
         tg->SetMarkerStyle(kFullSquare);
         tg->SetMarkerColor(kBlue);
         tg->SetLineColor(kBlue);
         tg_syst->SetFillColorAlpha(kBlue, 0.5);
      } else if (thebin.rapbin() == binF(0.6,1.2) || thebin.centbin() == binI(20,60)) {
         tg->SetMarkerStyle(kFullCircle);
         tg->SetMarkerColor(kRed);
         tg->SetLineColor(kRed);
         tg_syst->SetFillColorAlpha(kRed, 0.5);
      } else if (thebin.rapbin() == binF(1.2,1.8) || thebin.centbin() == binI(20,60)) {
         tg->SetMarkerStyle(kFullTriangleUp);
         tg->SetMarkerColor(kGreen);
         tg->SetLineColor(kGreen);
         tg_syst->SetFillColorAlpha(kGreen, 0.5);
      }
      tg->SetMarkerSize(1.5);
      tg->SetLineWidth(tg->GetLineWidth()*2);

      if (xaxis=="cent") {
         // do not plot wide centrality bins
         prune(tg, tg_syst);
      }
      tg_syst->Draw("2");      
      gStyle->SetEndErrorSize(5);
      tg->Draw("P");
      // tg->Draw("[]");

      TString raplabel = Form("%.1f < |y| < %.1f, ",it->first.rapbin().low(),it->first.rapbin().high());
      if (it->first.rapbin().low()<0.1) raplabel = Form("|y| < %.1f, ",it->first.rapbin().high());
      TString otherlabel = "BWAA";
      if (xaxis == "pt") otherlabel.Form("%i-%i%s",(int) (it->first.centbin().low()/2.), (int) (it->first.centbin().high()/2.), "%");
      if (xaxis == "cent") otherlabel.Form("%g < p_{T} < %g GeV/c",it->first.ptbin().low(), it->first.ptbin().high());
      if (!isMB) {
         tleg->AddEntry(tg, (raplabel + otherlabel), "p");
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
         dx = 10;
         x = 2*dx*(thebin.rapbin().low()/1.6) + dx;
         y = 1;
         anabin thebin(it->first.rapbin().low(),
               it->first.rapbin().high(),
               it->first.ptbin().low(),
               it->first.ptbin().high(),
               0,200);
         dy = gsyst[thebin].value_dR_rel;
         cout << "global syst: " << dy << endl;
         TBox *tbox = new TBox(x-dx,y-dy,x+dx,y+dy);
         if (thebin.rapbin() == binF((float) 0.,1.6)) tbox->SetFillColorAlpha(kBlue, 0.5);
         else if (thebin.rapbin() == binF(1.6,2.4)) tbox->SetFillColorAlpha(kRed, 0.5);
         else tbox->SetFillColorAlpha(kGreen, 0.5);
         tbox->Draw();
      }

      cnt++;
      it_syst++;
   }


   if (plotlimits95) plotLimits(theCats,xaxis,"../Limits/csv/Limits_95.csv",0);
   if (fiterrors && FCerrors) plotLimits(theCats,xaxis,"../Limits/csv/Limits_68.csv",xaxis=="cent" ? 5 : 1, false);

   if (xaxis=="cent") padl->cd();
   tleg->Draw();

   TLatex tl;
   double tlx = 0.2+xshift;
   double tly = 0.69;
   // tl.DrawLatexNDC(tlx,tly,"Passing #font[12]{l}_{J/#psi}^{3D} cut");

   int iPos = 33;
   CMS_lumi( (TPad*) gPad, 106, iPos, "" );
   // CMS_lumi( (TPad*) gPad, 103, iPos, "" );

   c1->cd();
   c1->Update();
   c1->RedrawAxis();
   gSystem->mkdir(Form("Output/%s/plot/RESULT/root/", outputDir.c_str()), kTRUE); 
   c1->SaveAs(Form("Output/%s/plot/RESULT/root/result_%s%s.root",outputDir.c_str(), xaxis.c_str(), nameTag));
   gSystem->mkdir(Form("Output/%s/plot/RESULT/png/", outputDir.c_str()), kTRUE);
   c1->SaveAs(Form("Output/%s/plot/RESULT/png/result_%s%s.png",outputDir.c_str(), xaxis.c_str(), nameTag));
   gSystem->mkdir(Form("Output/%s/plot/RESULT/pdf/", outputDir.c_str()), kTRUE);
   c1->SaveAs(Form("Output/%s/plot/RESULT/pdf/result_%s%s.pdf",outputDir.c_str(), xaxis.c_str(), nameTag));

   delete tleg;
   delete haxes; 
   delete padl;
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
      if (!issyst) {
         if (isMB) {exl=0; exh=0;} else {
            exl = HI::findNpartSyst(2.*(x-exl),2.*(x+exh));//0.;
            exh = HI::findNpartSyst(2.*(x-exl),2.*(x+exh));//0.;
         }
      } else {
         exl = 5;
         exh = exl;
      }
      tg->SetPoint(i,x,y);
      tg->SetPointError(i,exl,exh,eyl,eyh);
   }
}

void plotLimits(vector<anabin> theCats, string xaxis, const char* filename, double xshift, bool ULonly) {
   map<anabin,limits> maplim = readLimits(filename);

   map<anabin,limits>::const_iterator it;
   for (it=maplim.begin(); it!=maplim.end(); it++) {
      anabin thebin = it->first;
      if (!binok(theCats,xaxis,thebin,false)) continue;
      limits lim = it->second;
      if (ULonly && lim.val.first>0) continue; // only draw upper limits, ie interval which lower limit is 0
      // draw arrow in the right place and with the right color...
      Color_t color=kBlack;
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
         x = HI::findNpartAverage(low,high);
         dx = 10;
      }
      if (thebin.rapbin() == binF(0.,1.6)) {
         color = kBlue;
      } else if (thebin.rapbin() == binF(1.6,2.4)) {
         color = kRed;
      }
      drawArrow(x+xshift, ylow, y, dx, color);
   }
}

void drawArrow(double x, double ylow, double yhigh, double dx, Color_t color) {
   TArrow *arrow = new TArrow(x,yhigh,x,ylow<=0. ? 0.05 : ylow,0.03,ylow<=0. ? ">" : "<>");
   arrow->SetLineColor(color);
   arrow->Draw();
   if (ylow<=0.) {
      TLine *line = new TLine(x-dx,yhigh,x+dx,yhigh);
      line->SetLineColor(color);
      line->Draw();
   }
}
