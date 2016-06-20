#include "TFile.h"
#include "TH1F.h"
#include "TGraphAsymmErrors.h"
#include "TString.h"
#include "TCanvas.h"
#include "TLegend.h"

#include "../Fitter/Macros/CMS/CMS_lumi.C"
#include "../Fitter/Macros/CMS/tdrstyle.C"
#include "../Fitter/Macros/Utilities/texUtils.h"

#include <iostream>
#include <fstream>

using namespace std;

void setErr(TH1F *hist);
void fixCentPp(TH1F *hist);
TH1F* integrateHist(TH1F *hist);

void plotEffs_data() {
   TFile *fjpsi_pp = new TFile("files_data/histos_pp_jpsi.root");
   TFile *fpsi2s_pp = new TFile("files_data/histos_pp_psip.root");
   TFile *fbkg_pp = new TFile("files_data/histos_pp_bkg.root");
   TFile *fjpsi_pbpb = new TFile("files_data/histos_pbpb_jpsi.root");
   TFile *fpsi2s_pbpb = new TFile("files_data/histos_pbpb_psip.root");
   TFile *fbkg_pbpb = new TFile("files_data/histos_pbpb_bkg.root");

   ofstream file_nocut("files_data/syst_PbPb_eff_MCstat_nocut.csv");
   ofstream file_ctaucut("files_data/syst_PbPb_eff_MCstat_ctaucut.csv");
   ofstream file_ctauptdepcut("files_data/syst_PbPb_eff_MCstat_ctauptdepcut.csv");
   file_nocut << "MC statistics in efficiency (no ctau cut)" << endl;
   file_ctaucut << "MC statistics in efficiency (with ctau pt-independent cut)" << endl;
   file_ctauptdepcut << "MC statistics in efficiency (with ctau pt-dependent cut)" << endl;

   // first, let's draw simple efficiencies
   // we'll draw on the same plot the efficiencies for prompt J/psi, psi(2S) and background
   // both for pp and pbpb, and both for the pp and centrality dependences, and both for midrapidity and forward rapidities, and both with and without ctau cut (2*2*2*3 = 24 plots)

   TString colltag, deptag, raptag, cuttag;
   for (int icoll=0; icoll<2; icoll++) {
      colltag = (icoll==0) ? "pp" : "pbpb";
      TString name = "files_data/histos_" + colltag + "_jpsi.root";
      TFile *fjpsi = new TFile(name);
      name = "files_data/histos_" + colltag + "_psip.root";
      TFile *fpsi2s = new TFile(name);
      name = "files_data/histos_" + colltag + "_bkg.root";
      TFile *fbkg = new TFile(name);

      for (int idep=0; idep<3; idep++) { // idep=2 -> integrated
         deptag = (idep==0) ? "pt" : "cent";

         for (int irap=0; irap<2; irap++) {
            raptag = (irap==0) ? "mid" : "fwd";

            cuttag = "ptdepcut_";

            setTDRStyle();
            gStyle->SetEndErrorSize(3);
            TCanvas *c1 = new TCanvas();

            TString hname;
            hname = "hnum" + cuttag + deptag + raptag;
            TH1F *hjpsinum = (TH1F*) fjpsi->Get(hname);
            TH1F *hpsi2snum = (TH1F*) fpsi2s->Get(hname);
            TH1F *hbkgnum = (TH1F*) fbkg->Get(hname);
            hname =  "hnum_" + deptag + raptag;
            TH1F *hjpsiden = (TH1F*) fjpsi->Get(hname);
            TH1F *hpsi2sden = (TH1F*) fpsi2s->Get(hname);
            TH1F *hbkgden = (TH1F*) fbkg->Get(hname);

            if (idep==2) {
               hjpsinum = integrateHist(hjpsinum);
               hpsi2snum = integrateHist(hpsi2snum);
               hbkgnum = integrateHist(hbkgnum);
               hjpsiden = integrateHist(hjpsiden);
               hpsi2sden = integrateHist(hpsi2sden);
               hbkgden = integrateHist(hbkgden);
            }

            if (icoll==0 && idep==1) { // centrality for pp... fill all the bins
               fixCentPp(hjpsinum);
               fixCentPp(hpsi2snum);
               fixCentPp(hbkgnum);
               fixCentPp(hjpsiden);
               fixCentPp(hpsi2sden);
               fixCentPp(hbkgden);
            }

            TGraphAsymmErrors *tg_jpsi = new TGraphAsymmErrors(hjpsinum,hjpsiden,(icoll==0) ? "" : "norm");
            tg_jpsi->SetMarkerColor(kBlack);
            tg_jpsi->SetLineColor(kBlack);
            TGraphAsymmErrors *tg_psi2s = new TGraphAsymmErrors(hpsi2snum,hpsi2sden,(icoll==0) ? "" : "norm");
            tg_psi2s->SetMarkerColor(kRed);
            tg_psi2s->SetLineColor(kRed);
            TGraphAsymmErrors *tg_bkg = new TGraphAsymmErrors(hbkgnum,hbkgden,(icoll==0) ? "" : "norm");
            tg_bkg->SetMarkerColor(kBlue);
            tg_bkg->SetLineColor(kBlue);

            TH1F *haxes = new TH1F("haxes","haxes",1,0,(idep==1) ? 100 : 30);
            haxes->GetYaxis()->SetTitle("#font[12]{l}_{J/#psi}^{3D} cut efficiency");
            haxes->GetXaxis()->SetTitle((idep==1) ? "Centrality bin" : "p_{T}");
            TLatex tl; TString cname;
            TString effname = "ctaucuteff_";
            cname = "files_data/" + effname + colltag + "_" + deptag + "_" + raptag + "_" + cuttag;
            TString texname = cname + ".tex";

            if (idep<2) {
               // plot
               haxes->Draw();
               TLine *tline = new TLine(haxes->GetXaxis()->GetXmin(),0.9,haxes->GetXaxis()->GetXmax(),0.9);
               tline->SetLineStyle(3);
               tline->SetLineColor(kBlack);
               tline->SetLineWidth(5);
               tline->Draw();
               tg_jpsi->Draw("P");
               tg_psi2s->Draw("P");
               tg_bkg->Draw("P");

               double yshift =0;
               TLegend *tleg = new TLegend(0.5,0.26+yshift,0.88,0.46+yshift);
               tleg->SetBorderSize(0);
               tleg->AddEntry(tg_jpsi,"J/#psi (prompt)","lp");
               tleg->AddEntry(tg_psi2s,"#psi(2S)","lp");
               tleg->AddEntry(tg_bkg,"Background","lp");
               tleg->Draw();

               tl.DrawLatex((idep==0) ? 1.5 : 10, 0.7, colltag + TString(", ") 
                     + ((irap==0) ? "|y|<1.6" : "|y|>1.6")); 


               c1->SaveAs(cname + ".root");
               c1->SaveAs(cname + ".png");
               c1->SaveAs(cname + ".pdf");

               // print tex
               vector<string> yname;
               yname.push_back("prompt \\Jpsi");
               yname.push_back("\\psiP");
               yname.push_back("background");
               vector<TGraphAsymmErrors*> tgs;
               tgs.push_back(tg_jpsi); tgs.push_back(tg_psi2s); tgs.push_back(tg_bkg);
               inittex(texname.Data(), deptag=="pt" ? "\\pt" : "Centrality bin", yname);
               printGraph(tgs, texname.Data());
               if (idep==0) closetex(texname.Data());
            } else {
               vector<TGraphAsymmErrors*> tgs;
               tgs.push_back(tg_jpsi); tgs.push_back(tg_psi2s); tgs.push_back(tg_bkg);
               printGraph(tgs, texname.Data());
               closetex(texname.Data());
            }


            // now, let's draw simple ratios of efficiencies: psi(2S)/J/psi
            // but do it only once
            delete haxes;
            delete c1;
            delete tg_jpsi; delete tg_psi2s; delete tg_bkg;
            continue;
         } // irap loop (mid / fwd)
      } // idep loop (pt / centrality)
   } // icoll loop (pp / pbpb)

   file_nocut.close();
   file_ctaucut.close();
   file_ctauptdepcut.close();
}

void setErr(TH1F *hist) {
   int nbins = hist->GetNbinsX();
   for (int i=1; i<nbins+1; i++) {
      hist->SetBinError(i,sqrt(hist->GetBinContent(i)));
   }
}

void fixCentPp(TH1F *hist) {
   int nbins = hist->GetNbinsX();
   float y = hist->GetBinContent(1);
   float dy = hist->GetBinError(1);
   for (int i=2; i<nbins+1; i++) {
      hist->SetBinContent(i,y);
      hist->SetBinError(i,dy);
   }
}

TH1F* integrateHist(TH1F *hist) {
   TString name = hist->GetName(); name = name + "_int";
   TString title = hist->GetTitle(); title = title + " integrated";
   double integral, integralerror;
   int nbins = hist->GetNbinsX();
   integral = hist->IntegralAndError(1,nbins,integralerror);
   TH1F *ans = new TH1F(name, title, 1, hist->GetXaxis()->GetBinLowEdge(1), hist->GetXaxis()->GetBinUpEdge(nbins));
   ans->SetBinContent(1,integral);
   ans->SetBinError(1,integralerror);
   return ans;
}
