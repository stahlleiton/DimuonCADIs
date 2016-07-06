#include "TFile.h"
#include "TH1F.h"
#include "TGraphAsymmErrors.h"
#include "TString.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TObjArray.h"
#include "TProfile.h"
#include "TRandom3.h"

#include "../Fitter/Macros/CMS/CMS_lumi.C"
#include "../Fitter/Macros/CMS/tdrstyle.C"
#include "../Fitter/Macros/Utilities/texUtils.h"

#include <iostream>
#include <fstream>

using namespace std;

void setErr(TH1F *hist);
void fixCentPp(TH1F *hist);
TH1F* integrateHist(TH1F *hist);

TProfile* SimpleEff(TObjArray *anum, TObjArray *aden, TString tag);
TProfile* SingleRatio(TObjArray *anum2, TObjArray *aden2, TObjArray *anum1, TObjArray *aden1, TString tag);
TProfile* DoubleRatio(TObjArray *anum2pbpb, TObjArray *aden2pbpb, TObjArray *anum1pbpb, TObjArray *aden1pbpb, TObjArray *anum2pp, TObjArray *aden2pp, TObjArray *anum1pp, TObjArray *aden1pp, TString tag);
void fillTProf(TProfile *tp, TH1F *th);
TProfile* newTProfile(TH1F *hist);

void plotEffs_DataVary(const char* dir = "files_100vars_mine") {
   TFile *fjpsi_pp = new TFile(Form("%s/histos_jpsi_pp.root",dir));
   TFile *fpsi2s_pp = new TFile(Form("%s/histos_psi2s_pp.root",dir));
   // TFile *fnpjpsi_pp = new TFile("files_100vars/histos_npjpsi_pp.root");
   TFile *fjpsi_pbpb = new TFile(Form("%s/histos_jpsi_pbpb.root",dir));
   TFile *fpsi2s_pbpb = new TFile(Form("%s/histos_psi2s_pbpb.root",dir));
   // TFile *fnpjpsi_pbpb = new TFile("files_100vars/histos_npjpsi_pbpb.root");

   ofstream file_ctauptdepcut(Form("%s/syst_PbPb_eff_MCstat_ctauptdepcut.csv",dir));
   file_ctauptdepcut << "Eff. cancellation: 100 vars" << endl;

   // initialize the random generatior
   gRandom = new TRandom3();

   // first, let's draw simple efficiencies
   // we'll draw on the same plot the efficiencies for prompt and non-prompt J/psi, and psi(2S)
   // both for pp and pbpb, and both for the pp and centrality dependences, and both for midrapidity and forward rapidities, and both with and without ctau cut (2*2*2*3 = 24 plots)

   TString colltag, deptag, raptag, cuttag;
   for (int icoll=0; icoll<2; icoll++) {
      colltag = (icoll==0) ? "pp" : "pbpb";
      TString name = TString(dir) + "/histos_jpsi_" + colltag + ".root";
      TFile *fjpsi = new TFile(name);
      name = TString(dir) + "/histos_psi2s_" + colltag + ".root";
      TFile *fpsi2s = new TFile(name);
      // name = "files_100vars/histos_npjpsi_" + colltag + ".root";
      // TFile *fnpjpsi = new TFile(name);

      for (int idep=0; idep<3; idep++) { // idep=2 -> integrated
         deptag = (idep==0) ? "pt" : "cent";

         for (int irap=0; irap<2; irap++) {
            raptag = (irap==0) ? "mid" : "fwd";

            int icut=2; //-> pt-integrated cut efficiency, icut=4 -> pt-dep cut efficiency
            cuttag = "ptdepcut_";

            setTDRStyle();
            gStyle->SetEndErrorSize(3);
            TCanvas *c1 = new TCanvas();

            TString aname;
            aname = "wtHisto_num_" + deptag + raptag;
            TObjArray *hjpsinum = (TObjArray*) fjpsi->Get(aname);
            TObjArray *hpsi2snum = (TObjArray*) fpsi2s->Get(aname);
            // TObjArray *hnpjpsinum = (TObjArray*) fnpjpsi->Get(aname);
            aname = "wtHisto_den_" + deptag + raptag;
            TObjArray *hjpsiden = (TObjArray*) fjpsi->Get(aname);
            TObjArray *hpsi2sden = (TObjArray*) fpsi2s->Get(aname);
            // TObjArray *hnpjpsiden = (TObjArray*) fnpjpsi->Get(aname);

            TProfile *tp_jpsi = SimpleEff(hjpsinum,hjpsiden,aname + colltag);
            tp_jpsi->SetMarkerColor(kBlack);
            tp_jpsi->SetLineColor(kBlack);
            TProfile *tp_psi2s = SimpleEff(hpsi2snum,hpsi2sden,aname + colltag);
            tp_psi2s->SetMarkerColor(kRed);
            tp_psi2s->SetLineColor(kRed);
            // TProfile *tp_npjpsi = SimpleEff(hnpjpsinum,hnpjpsiden,aname);
            // tp_npjpsi->SetMarkerColor(kBlue);
            // tp_npjpsi->SetLineColor(kBlue);

            TH1F *haxes = new TH1F("haxes","haxes",1,0,(idep==1) ? 100 : 30);
            haxes->GetYaxis()->SetTitle("Efficiency");
            if (icut>=3) haxes->GetYaxis()->SetTitle("#font[12]{l}_{J/#psi}^{3D} cut efficiency");
            haxes->GetXaxis()->SetTitle((idep==1) ? "Centrality bin" : "p_{T}");
            TLatex tl; TString cname;
            TString effname = (icut<3) ? "singleff_" : "ctaucuteff_";
            cname = TString(dir) + "/" + effname + colltag + "_" + deptag + "_" + raptag + "_" + cuttag;
            TString texname = cname + ".tex";

            if (idep<2) {
               // plot
               haxes->Draw();
               if (icut==3 || icut==4) { // draw a line at 90%
                  TLine *tl = new TLine(haxes->GetXaxis()->GetXmin(),0.9,haxes->GetXaxis()->GetXmax(),0.9);
                  tl->SetLineStyle(3);
                  tl->SetLineColor(kBlack);
                  tl->SetLineWidth(5);
                  tl->Draw();
               }
               tp_jpsi->Draw("same");
               tp_psi2s->Draw("same");
               // tp_npjpsi->Draw("same");

               double yshift =0; if (icoll==1 && irap==1) yshift=0.3;
               if (icut>=3) yshift=0;
               TLegend *tleg = new TLegend(0.5,0.26+yshift,0.88,0.46+yshift);
               tleg->SetBorderSize(0);
               tleg->AddEntry(tp_jpsi,"J/#psi (prompt)","lp");
               tleg->AddEntry(tp_psi2s,"#psi(2S)","lp");
               // tleg->AddEntry(tp_npjpsi,"J/#psi (non-prompt)","lp");
               tleg->Draw();

               tl.DrawLatex((idep==0) ? 1.5 : 10, (icut<3) ? 0.9 : 0.7, colltag + TString(", ") 
                     + ((irap==0) ? "|y|<1.6" : "|y|>1.6") + TString(", ") 
                     + ((icut==0) ? "no #font[12]{l}_{J/#psi}^{3D} cut" : ((icut==1 || icut==3) ? "cst #font[12]{l}_{J/#psi}^{3D} cut" : "pt-dep #font[12]{l}_{J/#psi}^{3D} cut")));


               c1->SaveAs(cname + ".root");
               c1->SaveAs(cname + ".png");
               c1->SaveAs(cname + ".pdf");

               // print tex
               vector<string> yname;
               yname.push_back("prompt \\Jpsi");
               yname.push_back("\\psiP");
               // yname.push_back("non-prompt \\Jpsi");
               vector<TH1*> tps;
               tps.push_back(tp_jpsi); tps.push_back(tp_psi2s); //tps.push_back(tp_npjpsi);
               inittex(texname.Data(), deptag=="pt" ? "\\pt" : "Centrality bin", yname);
               printHist(tps, texname.Data());
               if (idep==0) closetex(texname.Data());
            } else {
               vector<TH1*> tps;
               tps.push_back(tp_jpsi); tps.push_back(tp_psi2s); //tps.push_back(tp_npjpsi);
               printHist(tps, texname.Data());
               closetex(texname.Data());
            }


            // now, let's draw simple ratios of efficiencies: psi(2S)/J/psi
            // but do it only once
            if (icoll>0 || icut>=3) {
               delete haxes;
               delete c1;
               delete tp_jpsi; delete tp_psi2s; //delete tp_npjpsi;
               continue;
            }

            aname = "wtHisto_num_" + deptag + raptag;
            TObjArray *hjpsipp = (TObjArray*) fjpsi_pp->Get(aname);
            TObjArray *hpsi2spp = (TObjArray*) fpsi2s_pp->Get(aname);
            TObjArray *hjpsipbpb = (TObjArray*) fjpsi_pbpb->Get(aname);
            TObjArray *hpsi2spbpb = (TObjArray*) fpsi2s_pbpb->Get(aname);
            aname = "wtHisto_den_" + deptag + raptag;
            TObjArray *hjpsidenpp = (TObjArray*) fjpsi_pp->Get(aname);
            TObjArray *hpsi2sdenpp = (TObjArray*) fpsi2s_pp->Get(aname);
            TObjArray *hjpsidenpbpb = (TObjArray*) fjpsi_pbpb->Get(aname);
            TObjArray *hpsi2sdenpbpb = (TObjArray*) fpsi2s_pbpb->Get(aname);

            TProfile *tpsr_pp = SingleRatio(hpsi2spp,hpsi2sdenpp,hjpsipp,hjpsidenpp,aname + "_pp");
            TProfile *tpsr_pbpb = SingleRatio(hpsi2spbpb,hpsi2sdenpbpb,hjpsipbpb,hjpsidenpbpb,aname + "_pbpb");
            tpsr_pp->SetMarkerColor(kBlack);
            tpsr_pp->SetLineColor(kBlack);
            tpsr_pbpb->SetMarkerColor(kRed);
            tpsr_pbpb->SetLineColor(kRed);

            cname = TString(dir) + "/simpleratio_" + deptag + "_" + raptag + "_" + cuttag;
            texname = cname + ".tex";

            if (idep<2) {
               // plot
               haxes->GetYaxis()->SetTitle("Eff(#psi(2S)) / Eff(J/#psi)");
               haxes->GetYaxis()->SetRangeUser(0.5,1.5);
               haxes->SetBinContent(1,1);
               haxes->Draw();
               tpsr_pp->Draw("same");
               tpsr_pbpb->Draw("same");

               TLegend *tleg2 = new TLegend(0.7,0.17,0.89,0.31);
               tleg2->SetBorderSize(0);
               tleg2->AddEntry(tpsr_pp,"pp","lp");
               tleg2->AddEntry(tpsr_pbpb,"pbpb","lp");
               tleg2->Draw();

               tl.DrawLatex((idep==0) ? 1.5 : 10, 1.4, ((irap==0) ? "|y|<1.6" : "|y|>1.6") + TString(", ") 
                     + ((icut==0) ? "no #font[12]{l}_{J/#psi}^{3D} cut" : ((icut==1) ? "cst #font[12]{l}_{J/#psi}^{3D} cut" : "pt-dep #font[12]{l}_{J/#psi}^{3D} cut")));

               c1->SaveAs(cname + ".root");
               c1->SaveAs(cname + ".png");
               c1->SaveAs(cname + ".pdf");

               // print tex
               vector<string> yname;
               yname.push_back("\\pp");
               yname.push_back("\\pbpb");
               vector<TH1*> ths;
               ths.push_back(tpsr_pp); ths.push_back(tpsr_pbpb);
               inittex(texname.Data(), deptag=="pt" ? "\\pt" : "Centrality bin", yname);
               printHist(ths, texname.Data());
               if (idep==0) closetex(texname.Data());
            } else {
               vector<TH1*> ths;
               ths.push_back(tpsr_pp); ths.push_back(tpsr_pbpb);
               printHist(ths, texname.Data());
               closetex(texname.Data());
            }



            // at last, the double ratio
            TProfile *tp_dr = DoubleRatio(hpsi2spbpb,hpsi2sdenpbpb,hjpsipbpb,hjpsidenpbpb,hpsi2spp,hpsi2sdenpp,hjpsipp,hjpsidenpp,aname);
            cname = TString(dir) + "/doubleratio_" + deptag + "_" + raptag + "_" + cuttag;
            texname = cname + ".tex";

            if (idep<2) {
               // plot
               haxes->GetYaxis()->SetTitle("[Eff(#psi(2S)) / Eff(J/#psi)]_{PbPb} / [Eff(#psi(2S)) / Eff(J/#psi)]_{pp}");
               haxes->GetYaxis()->SetTitleSize(0.04);
               haxes->GetYaxis()->SetTitleOffset(2);
               haxes->GetYaxis()->SetRangeUser(0.5,1.5);
               haxes->Draw();
               tp_dr->Draw("same");
               tl.DrawLatex((idep==0) ? 1.5 : 10, 1.4, ((irap==0) ? "|y|<1.6" : "|y|>1.6") + TString(", ") 
                     + ((icut==0) ? "no #font[12]{l}_{J/#psi}^{3D} cut" : ((icut==1) ? "cst #font[12]{l}_{J/#psi}^{3D} cut" : "pt-dep #font[12]{l}_{J/#psi}^{3D} cut")));
               c1->SaveAs(cname + ".root");
               c1->SaveAs(cname + ".png");
               c1->SaveAs(cname + ".pdf");

               // print tex
               vector<string> yname;
               yname.push_back("Double ratio of efficiencies");
               vector<TH1*> ths;
               ths.push_back(tp_dr);
               inittex(texname.Data(), deptag=="pt" ? "\\pt" : "Centrality bin", yname);
               printHist(ths, texname.Data());
               if (idep==0) closetex(texname.Data());
            } else {
               vector<TH1*> ths;
               ths.push_back(tp_dr);
               printHist(ths, texname.Data());
               closetex(texname.Data());
            }

            // print the uncertainty values to the csv
            ofstream *file = NULL;
            file = &file_ctauptdepcut;
            double rapmin, rapmax, ptmin, ptmax, centmin, centmax, value, valueErr;
            rapmin = (irap==0) ? 0 : 1.6;
            rapmax = (irap==0) ? 1.6 : 2.4;
            if (idep==0) {
               centmin = 0;
               centmax = 100;
               for (int ibin=1; ibin<tp_dr->GetNbinsX()+1; ibin++) {
                  ptmin = tp_dr->GetXaxis()->GetBinLowEdge(ibin);
                  ptmax = tp_dr->GetXaxis()->GetBinUpEdge(ibin);
                  value = tp_dr->GetBinContent(ibin);
                  // valueErr = max(fabs(tp_dr->GetBinContent(ibin)-1),tp_dr->GetBinError(ibin));
                  valueErr = tp_dr->GetBinError(ibin);
                  *file << rapmin << ", " << rapmax << ", " << ptmin << ", " << ptmax << ", " << centmin << ", " << centmax << ", " << value << ", " << valueErr << endl;
               }
            } else if (idep==1) {
               ptmin = (irap==0) ? 6.5 : 3;
               ptmax = 30;
               for (int ibin=1; ibin<tp_dr->GetNbinsX()+1; ibin++) {
                  centmin = tp_dr->GetXaxis()->GetBinLowEdge(ibin);
                  centmax = tp_dr->GetXaxis()->GetBinUpEdge(ibin);
                  value = tp_dr->GetBinContent(ibin);
                  valueErr = tp_dr->GetBinError(ibin);
                  *file << rapmin << ", " << rapmax << ", " << ptmin << ", " << ptmax << ", " << centmin << ", " << centmax << ", " << value << ", " << valueErr << endl;
               }
            }
            else {
               ptmin = (irap==0) ? 6.5 : 3;
               ptmax = 30;
               centmin = 0;
               centmax = 100;
               value = tp_dr->GetBinContent(1);
               valueErr = tp_dr->GetBinError(1);
               *file << rapmin << ", " << rapmax << ", " << ptmin << ", " << ptmax << ", " << centmin << ", " << centmax << ", " << value << ", " << valueErr << endl;
            }

            // clean behind ourselves
            delete c1;
            delete tp_jpsi; delete tp_psi2s; //delete tp_npjpsi;
            delete haxes;
            //delete hjpsinum;
            //delete hpsi2snum;
            //delete hnpjpsinum;
            //delete hjpsiden;
            //delete hpsi2sden;
            //delete hnpjpsiden;
            //delete hname;
         } // irap loop (mid / fwd)
      } // idep loop (pt / centrality)
   } // icoll loop (pp / pbpb)

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

TProfile* SimpleEff(TObjArray *anum, TObjArray *aden, TString tag) {
   TProfile *tprof = newTProfile((TH1F*) anum->At(0));

   bool fixcent = (tag.Contains("pp") && tag.Contains("cent"));

   for (int i=1; i<anum->GetSize(); i++) {
      TH1F *hnum = (TH1F*) anum->At(i);
      TH1F *hden = (TH1F*) aden->At(i);
      if (fixcent) {
         hnum = integrateHist(hnum);
         hden = integrateHist(hden);
         fixCentPp(hnum);
         fixCentPp(hden);
      }
      hnum->Sumw2(true);
      hden->Sumw2(true);
      TH1F *hratio = (TH1F*) hnum->Clone("hratio");
      hratio->Divide(hnum,hden,1,1,"B");
      fillTProf(tprof,hratio);
      if (fixcent) {
         delete hnum;
         delete hden;
      }
      delete hratio;
   }

   return tprof;
}

TProfile* SingleRatio(TObjArray *anum2, TObjArray *aden2, TObjArray *anum1, TObjArray *aden1, TString tag) {

   bool fixcent = (tag.Contains("pp") && tag.Contains("cent"));
   bool doint = (tag.Contains("int"));

   if (fixcent) fixCentPp((TH1F*) anum2->At(0));
   TProfile *tprof = newTProfile((TH1F*) anum2->At(0));

   int nvars = anum2->GetSize()-1;
   for (int i=1; i<4.*nvars; i++) {
      int i1 = gRandom->Integer(nvars)+1;
      int i2 = gRandom->Integer(nvars)+1;
      TH1F *hnum2 = (TH1F*) anum2->At(i2);
      TH1F *hden2 = (TH1F*) aden2->At(i2);
      TH1F *hnum1 = (TH1F*) anum1->At(i1);
      TH1F *hden1 = (TH1F*) aden1->At(i1);
      if (doint) {
         hnum2 = integrateHist(hnum2);
         hden2 = integrateHist(hden2);
         hnum1 = integrateHist(hnum1);
         hden1 = integrateHist(hden1);
      }
      if (fixcent) {
         fixCentPp(hnum2);
         fixCentPp(hden2);
         fixCentPp(hnum1);
         fixCentPp(hden1);
      }
      hnum2->Sumw2(true);
      hden2->Sumw2(true);
      hnum1->Sumw2(true);
      hden1->Sumw2(true);
      TH1F *hratio2 = (TH1F*) hnum2->Clone("hratio");
      TH1F *hratio1 = (TH1F*) hnum1->Clone("hratio");
      hratio2->Divide(hnum2,hden2,1,1,"B");
      hratio1->Divide(hnum1,hden1,1,1,"B");
      hratio2->Divide(hratio1);
      fillTProf(tprof,hratio2);
      if (doint) {
         delete hnum2;
         delete hden2;
         delete hnum1;
         delete hden1;
      }
      delete hratio2; delete hratio1;
   }

   return tprof;
}

TProfile* DoubleRatio(TObjArray *anum2pbpb, TObjArray *aden2pbpb, TObjArray *anum1pbpb, TObjArray *aden1pbpb, TObjArray *anum2pp, TObjArray *aden2pp, TObjArray *anum1pp, TObjArray *aden1pp, TString tag) {
   TProfile *tprof = newTProfile((TH1F*) anum2pbpb->At(0));

   bool fixcent = (tag.Contains("cent"));

   int nvars = anum2pbpb->GetSize()-1;
   for (int i=1; i<8.*nvars; i++) {
      int i1pp = gRandom->Integer(nvars)+1;
      int i2pp = gRandom->Integer(nvars)+1;
      int i1pbpb = gRandom->Integer(nvars)+1;
      int i2pbpb = gRandom->Integer(nvars)+1;
      TH1F *hnum2pbpb = (TH1F*) anum2pbpb->At(i2pbpb);
      TH1F *hden2pbpb = (TH1F*) aden2pbpb->At(i2pbpb);
      TH1F *hnum1pbpb = (TH1F*) anum1pbpb->At(i1pbpb);
      TH1F *hden1pbpb = (TH1F*) aden1pbpb->At(i1pbpb);
      TH1F *hnum2pp = (TH1F*) anum2pp->At(i2pp);
      TH1F *hden2pp = (TH1F*) aden2pp->At(i2pp);
      TH1F *hnum1pp = (TH1F*) anum1pp->At(i1pp);
      TH1F *hden1pp = (TH1F*) aden1pp->At(i1pp);
      if (fixcent) {
         fixCentPp(hnum2pp);
         fixCentPp(hden2pp);
         fixCentPp(hnum1pp);
         fixCentPp(hden1pp);
      }
      hnum2pbpb->Sumw2(true);
      hden2pbpb->Sumw2(true);
      hnum1pbpb->Sumw2(true);
      hden1pbpb->Sumw2(true);
      TH1F *hratiopbpb2 = (TH1F*) hnum2pbpb->Clone("hratiopbpb");
      TH1F *hratiopbpb1 = (TH1F*) hnum1pbpb->Clone("hratiopbpb");
      hratiopbpb2->Divide(hnum2pbpb,hden2pbpb,1,1,"B");
      hratiopbpb1->Divide(hnum1pbpb,hden1pbpb,1,1,"B");
      hratiopbpb2->Divide(hratiopbpb1);
      hnum2pp->Sumw2(true);
      hden2pp->Sumw2(true);
      hnum1pp->Sumw2(true);
      hden1pp->Sumw2(true);
      TH1F *hratiopp2 = (TH1F*) hnum2pp->Clone("hratiopp");
      TH1F *hratiopp1 = (TH1F*) hnum1pp->Clone("hratiopp");
      hratiopp2->Divide(hnum2pp,hden2pp,1,1,"B");
      hratiopp1->Divide(hnum1pp,hden1pp,1,1,"B");
      hratiopp2->Divide(hratiopp1);
      hratiopbpb2->Divide(hratiopp2);
      fillTProf(tprof,hratiopbpb2);
      delete hratiopbpb2; delete hratiopbpb1;
      delete hratiopp2; delete hratiopp1;
   }

   return tprof;
}

void fillTProf(TProfile *tp, TH1F *th) {
   if (tp->GetNbinsX() != th->GetNbinsX()) {
      cout << "Error, tp and th have different number of bins" << endl;
      return;
   }
   for (int i=0; i<=tp->GetNbinsX()+1; i++) {
      tp->Fill(th->GetBinCenter(i),th->GetBinContent(i));
   }
}

TProfile* newTProfile(TH1F *hist) {
   int nbins = hist->GetNbinsX();
   double *xbins = new double[nbins+1];
   xbins[0] = hist->GetXaxis()->GetBinLowEdge(1);
   for (int i=1; i<nbins+1; i++) xbins[i] = hist->GetXaxis()->GetBinUpEdge(i);
   return new TProfile(Form("tprof_%s",hist->GetName()),"",nbins,xbins,"S");
}

// plot distribs
void plotAll(const char* dir, bool ispsip, bool ispbpb, bool iscent, bool isfwd) {
   TString part = ispsip ? "psi2s" : "jpsi";
   TString part2 = ispsip ? "Psi2S" : "JPsi";
   TString coll = ispbpb ? "pbpb" : "pp";
   TString coll2 = ispbpb ? "PbPb" : "PP";
   TString dep = iscent ? "cent" : "pt";
   TString rap = isfwd ? "fwd" : "mid";
   TString rap2 = isfwd ? "Fwd" : "Mid";
   TString fname = TString(dir) + "/histos_" + part + "_" + coll + ".root";
   TString fname2 = TString("files") + "/histos_" + part + "_" + coll + ".root";
   TString fwname = TString("wFunctions/weights_") + part2 + "_" + coll2 + "_" + rap2 + ".root";
   TString histname = TString("wtHisto_num_") + dep + rap;
   TString histname2 = TString("hnumptdepcut_") + dep + rap;
   TString hdataname("hRandData");

   TFile *fh = new TFile(fname);
   if (!fh) return;
   TObjArray *histsMC = (TObjArray*) fh->Get(histname);
   if (!histsMC) return;
   TFile *fw = new TFile(fwname);
   if (!fw) return;
   TObjArray *histsData = (TObjArray*) fw->Get(hdataname);
   if (!histsData) return;
   TFile *fh2 = new TFile(fname2);
   if (!fh2) return;
   TH1F *hnom = (TH1F*) fh2->Get(histname2);
   if (!hnom) return;

   setTDRStyle();
   TCanvas *c1 = new TCanvas();
   TLegend *tleg = new TLegend(0.6,0.51,0.88,0.8);
   tleg->SetBorderSize(0);
   TString header;
   header = coll + TString(isfwd ? ", |y|>1.6" : ", |y|<1.6") + ", " + part2;
   tleg->SetHeader(header);
   for (int i=100; i>=0; i--) {
      TH1F *h = (TH1F*) histsMC->At(i);
      if (i>0) {
         h->SetLineColor(kRed);
      } else {
         h->SetLineColor(kBlue);
         h->SetLineWidth(2);
         tleg->AddEntry(h, "Nominal MC","l");
      }
      h->SetMarkerSize(0);
      if (i==100) { // case of the first hist
         h->GetXaxis()->SetTitle(iscent ? "Centrality percentile" : "p_{T} [GeV/c]");
         h->GetYaxis()->SetTitle("a.u.");
         tleg->AddEntry(h,"Reweighted MC","l");
         TH1F *haxes = (TH1F*) h->Clone("axes"); haxes->GetYaxis()->SetRangeUser(0,0.65);
         haxes->Draw("AXIS");
         h->DrawNormalized("same");
      } else {
         h->DrawNormalized("same");
      }
   }

   hnom->SetLineColor(kCyan);
   hnom->SetLineWidth(2);
   hnom->SetMarkerSize(0);
   tleg->AddEntry(hnom,"Unweighted MC","l");
   hnom->DrawNormalized("same");

   TH1F *h = (TH1F*) histsData->At(0);
   h->SetLineColor(kBlack);
   h->SetLineWidth(2);
   h->SetMarkerStyle(kFullCircle);
   h->SetMarkerSize(1);
   h->DrawNormalized("same");
   tleg->AddEntry(h,"Data","lp");
   tleg->Draw();

   TString cname = TString(dir) + TString("/datamc_") + coll + "_" + part + "_" + rap;
   c1->SaveAs(cname + ".pdf");
   c1->SaveAs(cname + ".png");
   c1->SaveAs(cname + ".root");

   delete c1;
   delete fh; delete fw; delete fh2;
}

void plotAll(const char* dir) {
   for (int ispsip=0; ispsip<2; ispsip++)
      for (int ispbpb=0; ispbpb<2; ispbpb++)
         for (int isfwd=0; isfwd<2; isfwd++)
            plotAll(dir,ispsip,ispbpb,false,isfwd);
}
