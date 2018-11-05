#include "results2syst.C"
#include "plotVars.C"
#include "plotResults.C"
#include "Systematics/syst.h"
#include "Macros/drawMassPlot.C"

#include "TFile.h"
#include "TString.h"
#include "RooWorkspace.h"
#include "RooRealVar.h"

using namespace RooFit;

void makeProjectionPlots(const char* workDirName, const char* DSTag="DATA", bool paperStyle=true, bool incSS=false);

void makeProjectionPlots(const char* workDirName, const char* DSTag, bool paperStyle, bool incSS) {
   // list of files
   vector<TString> theFiles = fileList(workDirName,"",DSTag);

   for (vector<TString>::const_iterator it=theFiles.begin(); it!=theFiles.end(); it++) {
      TFile *f = TFile::Open(*it);
      TString t; Int_t from = 0;
      bool catchjpsi=false, catchpsip=false, catchbkg=false;
      Char_t jpsiName[128], psipName[128], bkgName[128];
      while (it->Tokenize(t, from, "_")) {
         if (catchjpsi) {strcpy(jpsiName, t.Data()); catchjpsi=false;}
         if (catchpsip) {strcpy(psipName, t.Data()); catchpsip=false;}
         if (catchbkg) {strcpy(bkgName, t.Data()); catchbkg=false;}
         if (t=="Jpsi") catchjpsi=true;
         if (t=="Psi2S") catchpsip=true;
         if (t=="Bkg") catchbkg=true;
      }
      RooWorkspace *myws = (RooWorkspace*) f->Get("workspace");
      string outputDir = string("Output/") + string(workDirName) + string("/");
      struct InputOpt opt;
      opt.pp.RunNb.Start   = 262157; opt.PbPb.RunNb.Start = 262620;
      opt.pp.RunNb.End     = 262328; opt.PbPb.RunNb.End   = 263757;
      opt.pp.TriggerBit    = (int) PP::HLT_HIL1DoubleMu0_v1; 
      opt.PbPb.TriggerBit  = (int) HI::HLT_HIL1DoubleMu0_v1; 
      struct KinCuts cut;
      RooRealVar *invMass = myws->var("invMass");
      cut.dMuon.M.Min = invMass->getMin();
      cut.dMuon.M.Max = invMass->getMax();
      
      // if this is not a MB bin, skip it
      anabin thebin = binFromFile(*it);
      // if (!(thebin==anabin(0,1.6,6.5,30,0,200) || thebin==anabin(1.6,2.4,3,30,0,200))) continue;
      if (!(thebin==anabin(0,1.6,9,12,0,200) || 
               thebin==anabin(1.6,2.4,20,30,0,200) ||
               thebin==anabin(1.6,2.4,3,30,0,40) ||
               thebin==anabin(0,1.6,6.5,30,0,200) || thebin==anabin(1.6,2.4,3,30,0,200)
           )) continue;

      cut.Centrality.Start = thebin.centbin().low();
      cut.Centrality.End = thebin.centbin().high();
      cut.dMuon.Pt.Min = thebin.ptbin().low();
      cut.dMuon.Pt.Max = thebin.ptbin().high();
      cut.dMuon.AbsRap.Min = thebin.rapbin().low();
      cut.dMuon.AbsRap.Max = thebin.rapbin().high();
      bool isPbPb = (it->Index("PbPb")>0);
      if (!isPbPb) continue; // do not do an extrapolation for pp
      string plotLabel = "";
      bool incJpsi = true;//!paperStyle;
      bool incPsi2S = true;//!paperStyle;
      if (incJpsi)  { plotLabel = plotLabel + Form("_Jpsi_%s", jpsiName);  } 
      if (incPsi2S) { plotLabel = plotLabel + Form("_Psi2S_%s", psipName); }
      bool incBkg = true;
      bool cutCtau = true;
      bool doSimulFit = false;
      cout << *it << " " << it->Index("PbPb") << endl;
      bool setLogScale = !paperStyle;
      bool zoomPsi = paperStyle;
      int nBins = 46;
      bool getMeanPT = false;

      // here we replace the actual data with projection data
      double lumi_orig = 464e-3;
      double lumi_proj = 10.;
      RooAbsPdf *thepdf = myws->pdf(Form("pdfMASS_Tot_%s", isPbPb ? "PbPb" : "PP"));
      int nevents = myws->data(Form("dOS_DATA_%s", isPbPb ? "PbPb" : "PP"))->numEntries();
      nevents = (nevents*lumi_proj)/lumi_orig;
      RooDataSet *dataproj = thepdf->generate(RooArgSet(*invMass),
            nevents,
            Extended(),
            NumEvents(nevents),
            Name(Form("dOS_DATA2_%s", isPbPb ? "PbPb" : "PP")),
            ProtoData(*((RooDataSet*) myws->data(Form("dOS_DATA_%s",isPbPb?"PbPb":"PP"))))
            );
      // dataproj->SetName(Form("dOS_DATA2_%s", isPbPb ? "PbPb" : "PP"));
      cout << dataproj->GetName() << " " << dataproj->sumEntries() << endl;
      cout << myws->import(*dataproj) << endl;
      Double_t norm = myws->data(dataproj->GetName())->sumEntries();
      cout << norm << endl;

      drawMassPlot(*myws, outputDir, opt, cut, plotLabel, "DATA2", isPbPb, incJpsi, incPsi2S, incBkg, cutCtau, doSimulFit, false, setLogScale, incSS, zoomPsi, nBins, getMeanPT, paperStyle, false);

      delete myws;
      delete f;
   }
}
