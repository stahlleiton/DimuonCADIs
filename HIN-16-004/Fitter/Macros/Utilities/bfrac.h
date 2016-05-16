#ifndef bfrac_h
#define bfrac_h

#include "bin.h"
#include "resultUtils.h"
#include "../../../Efficiency/plotEffs.C"

#include <vector>
#include "TString.h"
#include "RooStats/SamplingDistribution.h"
#include "RooDataSet.h"
#include "RooFitResult.h"
#include "TMath.h"

using namespace std;
using namespace RooFit;
using namespace RooStats;

SamplingDistribution combDist(RooFormulaVar formula, RooArgSet pdfs, int nevts=1e4);
double mean(SamplingDistribution* s);
void testCombDist();

RooRealVar bfrac (const char *pr_fits, // name of the prompt fits directory
      const char* npr_fits,         // name of the non-prompt fits directory
      anabin bin,                  // bin to be considered
      bool isPbPb,                 // true -> PbPb, false -> pp
      bool is2S,                   // true -> psi(2S), false -> J/psi
      bool doNprompt               // true -> return N_prompt, false -> return B-fraction
      ) {
   RooRealVar bfrac("bfrac","bfrac",0);

   const char* token = isPbPb ? "PbPb" : "PP";
   vector<TString> files_pr = fileList(pr_fits, token);
   vector<TString> files_npr = fileList(npr_fits, token);

   vector<TString>::const_iterator it;
   TString file_pr, file_npr;
   int nok=0;
   TString thebin_str(Form("pt%i%i_rap%i%i_cent%i%i",
            (int) (10*bin.ptbin().low()), (int) (10*bin.ptbin().high()),
            (int) (10*bin.rapbin().low()), (int) (10*bin.rapbin().high()),
            bin.centbin().low(),bin.centbin().high()));
   for (vector<TString>::const_iterator it=files_pr.begin(); it!=files_pr.end(); it++) {
      if (it->Index(thebin_str) != TString::kNPOS) {
         file_pr = *it;
         nok++;
      }
   }
   if (nok != 1) {
      cout << "Error, found " << nok << " files corresponding to the requested bin. Exiting." << endl;
      return bfrac;
   }
   nok=0;
   for (vector<TString>::const_iterator it=files_npr.begin(); it!=files_npr.end(); it++) {
      if (it->Index(thebin_str) != TString::kNPOS) {
         file_npr = *it;
         nok++;
      }
   }
   if (nok != 1) {
      cout << "Error, found " << nok << " files corresponding to the requested bin. Exiting." << endl;
      return bfrac;
   }

   // get the different ingredients of the formula
   // first, single ratios
   TFile *fpr = TFile::Open(file_pr); if (!fpr || !fpr->IsOpen()) return bfrac;
   TFile *fnpr = TFile::Open(file_npr); if (!fnpr || !fnpr->IsOpen()) return bfrac;
   RooWorkspace *wspr = (RooWorkspace*) fpr->Get("workspace"); if (!wspr) return bfrac;
   RooWorkspace *wsnpr = (RooWorkspace*) fnpr->Get("workspace"); if (!wsnpr) return bfrac;
   RooRealVar *Njpsipr = wspr->var(Form("N_Jpsi_%s", isPbPb ? "PbPb" : "PP"));
   RooRealVar *Njpsinpr = wsnpr->var(Form("N_Jpsi_%s", isPbPb ? "PbPb" : "PP"));
   RooRealVar *Nprtmp=NULL,*Nnprtmp=NULL;
   if (!is2S) {
      Nprtmp = Njpsipr;
      Nnprtmp = Njpsinpr;
   } else { // the number of psi' is a RooFormula, so we have to build it and compute its uncertainty
      RooRealVar *rfracpr = wspr->var(Form("RFrac2Svs1S_%s", isPbPb ? "PbPb" : "PP"));
      RooRealVar *rfracnpr = wsnpr->var(Form("RFrac2Svs1S_%s", isPbPb ? "PbPb" : "PP"));
      RooFitResult *frpr = (RooFitResult*) wspr->obj(Form("fitResult_pdfMASS_Tot_%s", isPbPb ? "PbPb" : "PP"));
      RooFitResult *frnpr = (RooFitResult*) wsnpr->obj(Form("fitResult_pdfMASS_Tot_%s", isPbPb ? "PbPb" : "PP"));
      double corrpr = frpr->correlation(Form("N_Jpsi_%s", isPbPb ? "PbPb" : "PP"), Form("RFrac2Svs1S_%s", isPbPb ? "PbPb" : "PP"));
      double corrnpr = frnpr->correlation(Form("N_Jpsi_%s", isPbPb ? "PbPb" : "PP"), Form("RFrac2Svs1S_%s", isPbPb ? "PbPb" : "PP"));
      double valpr = Njpsipr->getVal() * rfracpr->getVal();
      double valnpr = Njpsinpr->getVal() * rfracnpr->getVal();
      // cf https://en.wikipedia.org/wiki/Propagation_of_uncertainty#Example_formulas
      double errpr = fabs(valpr) * sqrt(pow(Njpsipr->getError()/Njpsipr->getVal(),2)
            +pow(rfracpr->getError()/rfracpr->getVal(),2)
            +2.*fabs(corrpr*Njpsipr->getError()*rfracpr->getError()/Njpsipr->getVal()/rfracpr->getVal()));
      double errnpr = fabs(valnpr) * sqrt(pow(Njpsinpr->getError()/Njpsinpr->getVal(),2)
            +pow(rfracnpr->getError()/rfracnpr->getVal(),2)
            +2.*fabs(corrnpr*Njpsinpr->getError()*rfracnpr->getError()/Njpsinpr->getVal()/rfracnpr->getVal()));
      Nprtmp = new RooRealVar("Nprtmp","",valpr); Nprtmp->setError(errpr);
      Nnprtmp = new RooRealVar("Nnprtmp","",valnpr); Nnprtmp->setError(errnpr);
   }

   RooRealVar Npr(*Nprtmp);
   RooRealVar Nnpr(*Nnprtmp);
   RooRealVar Npr_err("Npr_err","Npr_err",Npr.getError());
   RooRealVar Nnpr_err("Nnpr_err","Nnpr_err",Nnpr.getError());

   // then efficiencies
   TString feffpr("../Efficiency/files/histos_"); feffpr += is2S ? "psi2s_" : "jpsi_"; feffpr += isPbPb ? "pbpb.root" : "pp.root";
   TString feffnpr("../Efficiency/files/histos_npjpsi_"); feffnpr += isPbPb ? "pbpb.root" : "pp.root";
   TString numname("hnumptdepcut_"); numname += (bin.centbin() == binI(0,200)) ? "pt" : "cent"; numname += (bin.rapbin() == binF(0,1.6)) ? "mid" : "fwd";
   TString denname("hnum_"); denname += (bin.centbin() == binI(0,200)) ? "pt" : "cent"; denname += (bin.rapbin() == binF(0,1.6)) ? "mid" : "fwd";
   TFile *tfeffpr = TFile::Open(feffpr);
   TFile *tfeffnpr = TFile::Open(feffnpr);

   TH1F *hnumpr = (TH1F*) tfeffpr->Get(numname);
   TH1F *hnumnpr = (TH1F*) tfeffnpr->Get(numname);
   TH1F *hdenpr = (TH1F*) tfeffpr->Get(denname);
   TH1F *hdennpr = (TH1F*) tfeffnpr->Get(denname);

   RooRealVar numpr("numpr","",0), numpr_err("numpr_err","",0), numnpr("numnpr","",0), numnpr_err("numnpr_err","",0);
   RooRealVar failpr("failpr","",0), failpr_err("failpr_err","",0), failnpr("failnpr","",0), failnpr_err("failnpr_err","",0);
   if (bin == anabin(0,1.6,6.5,30,0,200) || bin == anabin(1.6,2.4,3,30,0,200)) { // Min.Bias bin
      hnumpr = integrateHist(hnumpr); numpr.setVal(hnumpr->GetBinContent(1)); numpr_err.setVal(hnumpr->GetBinError(1));
      hnumnpr = integrateHist(hnumnpr); numnpr.setVal(hnumnpr->GetBinContent(1)); numnpr_err.setVal(hnumnpr->GetBinError(1));
      hdenpr = integrateHist(hdenpr); failpr.setVal(hdenpr->GetBinContent(1)-numpr.getVal()); failpr_err.setVal(sqrt(pow(hdenpr->GetBinError(1),2)-pow(numpr_err.getVal(),2)));
      hdennpr = integrateHist(hdennpr); failnpr.setVal(hdennpr->GetBinContent(1)-numnpr.getVal()); failnpr_err.setVal(sqrt(pow(hdennpr->GetBinError(1),2)-pow(numnpr_err.getVal(),2)));
   } else {
      for (int i=1; i<=hnumpr->GetNbinsX(); i++) { // loop over the histogram bins to find the one we want
         if ((bin.centbin()==binI(0,200) && bin.ptbin()==binF(hnumpr->GetXaxis()->GetBinLowEdge(i),hnumpr->GetXaxis()->GetBinUpEdge(i))) // this is the bin we're looking for (pt)
               || bin.centbin()==binI(2*hnumpr->GetXaxis()->GetBinLowEdge(i),2*hnumpr->GetXaxis()->GetBinUpEdge(i))) { // this is the bin we're looking for (centrality)
            numpr.setVal(hnumpr->GetBinContent(i));
            numpr_err.setVal(hnumpr->GetBinError(i));
            numnpr.setVal(hnumnpr->GetBinContent(i));
            numnpr_err.setVal(hnumnpr->GetBinError(i));
            failpr.setVal(hdenpr->GetBinContent(i)-numpr.getVal());
            failnpr.setVal(hdennpr->GetBinContent(i)-numnpr.getVal());
            failpr_err.setVal(sqrt(pow(hdenpr->GetBinError(i),2)-pow(numpr_err.getVal(),2)));
            failnpr_err.setVal(sqrt(pow(hdennpr->GetBinError(i),2)-pow(numnpr_err.getVal(),2)));
            break;
         }
      }
   }

   delete fpr; fpr=0;
   delete fnpr; fnpr=0;
   delete tfeffpr; tfeffpr=0;
   delete tfeffnpr; tfeffnpr=0;

   // build the RooRealVar for each ingredient
   RooRealVar Npr_var("Npr_var","Npr_var",Npr.getVal(),Npr.getVal()-100.*Npr_err.getVal(),Npr.getVal()+100.*Npr_err.getVal());
   RooRealVar Nnpr_var("Nnpr_var","Nnpr_var",Nnpr.getVal(),Nnpr.getVal()-100.*Nnpr_err.getVal(),Nnpr.getVal()+100.*Nnpr_err.getVal());
   RooRealVar numpr_var("numpr_var","numpr_var",numpr.getVal(),0,numpr.getVal()+100.*numpr_err.getVal());
   RooRealVar numnpr_var("numnpr_var","numnpr_var",numnpr.getVal(),0,numnpr.getVal()+100.*numnpr_err.getVal());
   RooRealVar failpr_var("failpr_var","failpr_var",failpr.getVal(),0,failpr.getVal()+100.*failpr_err.getVal());
   RooRealVar failnpr_var("failnpr_var","failnpr_var",failnpr.getVal(),0,failnpr.getVal()+100.*failnpr_err.getVal());

   // build the PDFs: Gaussian for numbers of events, Gaussian (PbPb) or Poissonian (pp) for MC numbers of events
   RooGaussian Npr_pdf("Npr_pdf","Npr_pdf",Npr_var,Npr,Npr_err);
   RooGaussian Nnpr_pdf("Nnpr_pdf","Nnpr_pdf",Nnpr_var,Nnpr,Nnpr_err);
   RooAbsPdf *numpr_pdf=NULL; 
   numpr_pdf = isPbPb ? 
      (RooAbsPdf*) new RooGaussian("numpr_var","numpr_var",numpr_var,numpr,numpr_err) : 
      (RooAbsPdf*) new RooPoisson("numpr_var","numpr_var",numpr_var,numpr);
   RooAbsPdf *numnpr_pdf=NULL; 
   numnpr_pdf = isPbPb ? 
      (RooAbsPdf*) new RooGaussian("numnpr_var","numnpr_var",numnpr_var,numnpr,numnpr_err) : 
      (RooAbsPdf*) new RooPoisson("numnpr_var","numnpr_var",numnpr_var,numnpr);
   RooAbsPdf *failpr_pdf=NULL; 
   failpr_pdf = isPbPb ? 
      (RooAbsPdf*) new RooGaussian("failpr_var","failpr_var",failpr_var,failpr,failpr_err) : 
      (RooAbsPdf*) new RooPoisson("failpr_var","failpr_var",failpr_var,failpr);
   RooAbsPdf *failnpr_pdf=NULL; 
   failnpr_pdf = isPbPb ? 
      (RooAbsPdf*) new RooGaussian("failnpr_var","failnpr_var",failnpr_var,failnpr,failnpr_err) : 
      (RooAbsPdf*) new RooPoisson("failnpr_var","failnpr_var",failnpr_var,failnpr);

   // cout << Npr.getVal() << " " << Nnpr.getVal() << " " << numpr.getVal() << " " << numnpr.getVal() << " " << failpr.getVal() << " " << failnpr.getVal() << endl;
   // cout << Npr_err.getVal() << " " << Nnpr_err.getVal() << " " << numpr_err.getVal() << " " << numnpr_err.getVal() << " " << failpr_err.getVal() << " " << failnpr_err.getVal() << endl;
   RooArgList lvar(Npr_var,Nnpr_var,numpr_var,numnpr_var,failpr_var,failnpr_var);
   TString formulastr;
   if (!doNprompt) formulastr = "((@2/(@2+@4)) - (@0/(@0+@1))) / ((@2/(@2+@4)) - (@3/(@3+@5)))"; // B-fraction
   else  formulastr = "(@0+@1) * (((@2/(@2+@4)) - (@0/(@0+@1))) / ((@2/(@2+@4)) - (@3/(@3+@5))))"; // Nb of prompt onia
   RooFormulaVar formula("formula","formula",formulastr.Data(),lvar);
   RooArgSet pdfs(Npr_pdf,Nnpr_pdf,*numpr_pdf,*numnpr_pdf,*failpr_pdf,*failnpr_pdf);
   SamplingDistribution s = combDist(formula, pdfs, 1e4);

   // bfrac.setVal(s.InverseCDF(0.5)); // median
   bfrac.setVal(mean(&s)); // mean
   bfrac.setAsymError(s.InverseCDF(ROOT::Math::normal_cdf(-1))-bfrac.getVal(),s.InverseCDF(ROOT::Math::normal_cdf(1))-bfrac.getVal());
   bfrac.setError((fabs(bfrac.getErrorLo())+fabs(bfrac.getErrorHi()))/2.);
   // cout << s.InverseCDF(0.5) << " " << mean(&s) << ", " << s.InverseCDF(ROOT::Math::normal_cdf(-1)) << " " << s.InverseCDF(ROOT::Math::normal_cdf(1)) << endl;

   delete numpr_pdf; numpr_pdf=0;
   delete numnpr_pdf; numnpr_pdf=0;
   delete failpr_pdf; failpr_pdf=0;
   delete failnpr_pdf; failnpr_pdf=0;

   return bfrac;
}

SamplingDistribution combDist(RooFormulaVar formula, RooArgSet pdfs, int nevts) {
   RooRealVar formulaRRV("formulaRRV","formulaRRV",-1e99,1e99);
   RooDataSet data("sampledist","sampledist",RooArgSet(formulaRRV));
   double themin=1e99, themax=-1e99;
   for (int i=0; i<nevts; i++) {
      TIterator* it = pdfs.createIterator();
      RooAbsPdf *thePdf = (RooAbsPdf*) it->Next();
      while (thePdf) {
         thePdf->generateEvent(1);
         thePdf = (RooAbsPdf*) it->Next();
      }
      // cout << formula->getVal() << endl;
      double theval = formula.getVal();
      if (theval<themin) themin = theval;
      if (theval>themax) themax = theval;
      formulaRRV.setVal(theval);
      data.add(RooArgSet(formulaRRV));
   }

   // // plot the distribution
   // RooPlot *frame = formulaRRV.frame(Bins(100),Range(themin, themax));
   // TCanvas *c1 = new TCanvas();
   // data.plotOn(frame);
   // frame->Draw();
   // c1->SaveAs("test.pdf");
   // delete c1; c1=0;
   // delete frame; frame=0;

   return SamplingDistribution("s","s",data);
}

void testCombDist() {
   RooRealVar x("x","x",-5,5);
   RooRealVar y("y","y",-5,5);
   RooFormulaVar f("f","f","@0+@1",RooArgList(x,y));
   RooUniform ux("ux","ux",RooArgSet(x));
   RooUniform uy("uy","uy",RooArgSet(y));
   RooArgSet s(ux,uy);
   combDist(f,s,100000);
}

double mean(SamplingDistribution *s) {
   vector<double> v = s->GetSamplingDistribution();
   if (v.size()==0) return 0;
   double ans=0;
   for (vector<double>::const_iterator it=v.begin(); it!=v.end(); it++) ans+=*it;

   return ans / v.size();
}

#endif // ifndef bfrac_h
