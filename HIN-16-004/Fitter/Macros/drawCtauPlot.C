#ifndef drawCtauPlot_C
#define drawCtauPlot_C

#include "Utilities/initClasses.h"
#include "Utilities/EVENTUTILS.h"
#include "Utilities/resultUtils.h"
#include "TGaxis.h"

void setRange(RooWorkspace& myws, RooPlot* frame, string dsName, bool setLogScale, double dMuonYmin = -1.);
void makeAllCtauPlots(const char* workDirName, const char* DSTag, bool paperStyle, bool incSS);


void drawCtauPlot(RooWorkspace& myws,   // Local workspace
                  string outputDir,     // Output directory
                  struct InputOpt opt,  // Variable with run information (kept for legacy purpose)
                  struct KinCuts cut,   // Variable with current kinematic cuts
                  string plotLabel,     // The label used to define the output file name
                  // Select the type of datasets to fit
                  string DSTAG,         // Specifies the type of datasets: i.e, DATA, MCJPSINP, ...
                  bool isPbPb,          // Define if it is PbPb (True) or PP (False)
                  // Select the drawing options
                  bool setLogScale,     // Draw plot with log scale
                  bool incSS,           // Include Same Sign data
                  int  nBins,           // Number of bins used for plotting
                  bool paperStyle=false,// if true, print less info
                  bool saveWS=true      // save the workspace into a file
                  ) 
{
  if (DSTAG.find("_")!=std::string::npos) DSTAG.erase(DSTAG.find("_"));
  
  string dsOSName = Form("dOS_%s_%s", DSTAG.c_str(), (isPbPb?"PbPb":"PP"));
  string dsSSName = Form("dSS_%s_%s", DSTAG.c_str(), (isPbPb?"PbPb":"PP"));

  
  // Create the main plot of the fit
  // RooPlot*   frame     = myws.var("invMass")->frame(Bins(nBins), Range(cut.dMuon.M.Min, cut.dMuon.M.Max));
  RooPlot*   frame     = myws.var("ctau")->frame(Bins(nBins), Range(-1,3));
  myws.data(dsOSName.c_str())->plotOn(frame, Name("dOS"), DataError(RooAbsData::SumW2), XErrorSize(0), MarkerColor(kBlack), LineColor(kBlack), MarkerSize(1.2));
  
  if (paperStyle) TGaxis::SetMaxDigits(3); // to display powers of 10
    
  if (incSS) { 
    myws.data(dsSSName.c_str())->plotOn(frame, Name("dSS"), MarkerColor(kRed), LineColor(kRed), MarkerSize(1.2)); 
  }
  myws.data(dsOSName.c_str())->plotOn(frame, Name("dOS"), DataError(RooAbsData::SumW2), XErrorSize(0), MarkerColor(kBlack), LineColor(kBlack), MarkerSize(1.2));

  
  // set the CMS style
  setTDRStyle();
  
  // Create the main canvas
  TCanvas *cFig  = new TCanvas(Form("cMassFig_%s", (isPbPb?"PbPb":"PP")), "cMassFig",800,800);
  TPad    *pad1  = new TPad(Form("pad1_%s", (isPbPb?"PbPb":"PP")),"",0,paperStyle ? 0 : 0.23,1,1);
  TPad    *pad2  = new TPad(Form("pad2_%s", (isPbPb?"PbPb":"PP")),"",0,0,1,.228);
  TLine   *pline = new TLine(cut.dMuon.M.Min, 0.0, cut.dMuon.M.Max, 0.0);
  
  // TPad *pad4 = new TPad("pad4","This is pad4",0.55,0.46,0.97,0.87);
  TPad *pad4 = new TPad("pad4","This is pad4",0.55,paperStyle ? 0.29 : 0.36,0.97,paperStyle ? 0.70 : 0.77);
  pad4->SetFillStyle(0);
  pad4->SetLeftMargin(0.28);
  pad4->SetRightMargin(0.10);
  pad4->SetBottomMargin(0.21);
  pad4->SetTopMargin(0.072);

  frame->SetTitle("");
  frame->GetXaxis()->CenterTitle(kTRUE);
  if (!paperStyle) {
     frame->GetXaxis()->SetTitle("");
     frame->GetXaxis()->SetTitleSize(0.045);
     frame->GetXaxis()->SetTitleFont(42);
     frame->GetXaxis()->SetTitleOffset(3);
     frame->GetXaxis()->SetLabelOffset(3);
     frame->GetYaxis()->SetLabelSize(0.04);
     frame->GetYaxis()->SetTitleSize(0.04);
     frame->GetYaxis()->SetTitleOffset(1.7);
     frame->GetYaxis()->SetTitleFont(42);
  } else {
     frame->GetXaxis()->SetTitle("m_{#mu^{+}#mu^{-}} (GeV/c^{2})");
     frame->GetXaxis()->SetTitleOffset(1.1);
     frame->GetYaxis()->SetTitleOffset(1.45);
     frame->GetXaxis()->SetTitleSize(0.05);
     frame->GetYaxis()->SetTitleSize(0.05);
  }
  setRange(myws, frame, dsOSName, setLogScale, cut.dMuon.AbsRap.Min);
  if (paperStyle) {
     double Ydown = 0.1;//frame->GetMinimum();
     double Yup = 0.9*frame->GetMaximum();
     frame->GetYaxis()->SetRangeUser(Ydown,Yup);
  }
 
  cFig->cd();
  pad2->SetTopMargin(0.02);
  pad2->SetBottomMargin(0.4);
  pad2->SetFillStyle(4000); 
  pad2->SetFrameFillStyle(4000); 
  if (!paperStyle) pad1->SetBottomMargin(0.015); 
  //plot fit
  pad1->Draw();
  pad1->cd(); 
  frame->Draw();

  pad1->SetLogy(setLogScale);

  // Drawing the text in the plot
  TLatex *t = new TLatex(); t->SetNDC(); t->SetTextSize(0.032);
  float dy = 0; 
  
  t->SetTextSize(0.03);
  if (!paperStyle) { // do not print selection details for paper style
     t->DrawLatex(0.21, 0.86-dy, "2015 HI Soft Muon ID"); dy+=0.045;
     if (isPbPb) {
        t->DrawLatex(0.21, 0.86-dy, "HLT_HIL1DoubleMu0_v1"); dy+=0.045;
     } else {
        t->DrawLatex(0.21, 0.86-dy, "HLT_HIL1DoubleMu0_v1"); dy+=0.045;
     } 
  }
  if (cut.dMuon.AbsRap.Min>0.1) {t->DrawLatex(0.20, 0.86-dy, Form("%.1f < |y^{#mu#mu}| < %.1f",cut.dMuon.AbsRap.Min,cut.dMuon.AbsRap.Max)); dy+=1.5*0.045;}
  else {t->DrawLatex(0.20, 0.86-dy, Form("|y^{#mu#mu}| < %.1f",cut.dMuon.AbsRap.Max)); dy+=1.5*0.045;}
  t->DrawLatex(0.20, 0.86-dy, Form("%g < p_{T}^{#mu#mu} < %g GeV/c",cut.dMuon.Pt.Min,cut.dMuon.Pt.Max)); dy+=0.045;
  t->DrawLatex(0.20, 0.86-dy, Form("%g < M^{#mu#mu} < %g GeV/c^{2}",cut.dMuon.M.Min,cut.dMuon.M.Max)); dy+=0.045;
  if (isPbPb) {t->DrawLatex(0.20, 0.86-dy, Form("Cent. %d-%d%%", (int)(cut.Centrality.Start/2), (int)(cut.Centrality.End/2))); dy+=0.045;}
  dy+=0.5*0.045; t->DrawLatex(0.20, 0.86-dy, "#mu in acceptance"); dy+=0.045;

  // Drawing the Legend
  double ymin = 0.7802;
  if (paperStyle) { ymin = 0.72; }
  TLegend* leg = new TLegend(0.5175, ymin, 0.7180, 0.8809); leg->SetTextSize(0.03);
  leg->AddEntry(frame->findObject("dOS"), (incSS?"Opposite Charge":"Data"),"pe");
  if (incSS) { leg->AddEntry(frame->findObject("dSS"),"Same Charge","pe"); }
  leg->Draw("same");

  //Drawing the title
  TString label;
  if (isPbPb) {
    if (opt.PbPb.RunNb.Start==opt.PbPb.RunNb.End){
      label = Form("PbPb Run %d", opt.PbPb.RunNb.Start);
    } else {
      label = Form("%s [%s %d-%d]", "PbPb", "HIOniaL1DoubleMu0", opt.PbPb.RunNb.Start, opt.PbPb.RunNb.End);
    }
  } else {
    if (opt.pp.RunNb.Start==opt.pp.RunNb.End){
      label = Form("PP Run %d", opt.pp.RunNb.Start);
    } else {
      label = Form("%s [%s %d-%d]", "PP", "DoubleMu0", opt.pp.RunNb.Start, opt.pp.RunNb.End);
    }
  }
  
  // CMS_lumi(pad1, isPbPb ? 105 : 104, 33, label);
  CMS_lumi(pad1, isPbPb ? 108 : 107, 33, "");
  if (!paperStyle) gStyle->SetTitleFontSize(0.05);
  
  pad1->Update();
  cFig->cd(); 


  // Save the plot in different formats
  gSystem->mkdir(Form("%splot/%s/root/", outputDir.c_str(), DSTAG.c_str()), kTRUE); 
  cFig->SaveAs(Form("%splot/%s/root/%s_%s_%s%s_pt%.0f%.0f_rap%.0f%.0f_cent%d%d.root", outputDir.c_str(), DSTAG.c_str(), DSTAG.c_str(),  "Psi2SJpsi", (isPbPb?"PbPb":"PP"), plotLabel.c_str(), (cut.dMuon.Pt.Min*10.0), (cut.dMuon.Pt.Max*10.0), (cut.dMuon.AbsRap.Min*10.0), (cut.dMuon.AbsRap.Max*10.0), cut.Centrality.Start, cut.Centrality.End));
  gSystem->mkdir(Form("%splot/%s/png/", outputDir.c_str(), DSTAG.c_str()), kTRUE);
  cFig->SaveAs(Form("%splot/%s/png/%s_%s_%s%s_pt%.0f%.0f_rap%.0f%.0f_cent%d%d.png", outputDir.c_str(), DSTAG.c_str(), DSTAG.c_str(), "Psi2SJpsi", (isPbPb?"PbPb":"PP"), plotLabel.c_str(), (cut.dMuon.Pt.Min*10.0), (cut.dMuon.Pt.Max*10.0), (cut.dMuon.AbsRap.Min*10.0), (cut.dMuon.AbsRap.Max*10.0), cut.Centrality.Start, cut.Centrality.End));
  gSystem->mkdir(Form("%splot/%s/pdf/", outputDir.c_str(), DSTAG.c_str()), kTRUE);
  cFig->SaveAs(Form("%splot/%s/pdf/%s_%s_%s%s_pt%.0f%.0f_rap%.0f%.0f_cent%d%d.pdf", outputDir.c_str(), DSTAG.c_str(), DSTAG.c_str(), "Psi2SJpsi", (isPbPb?"PbPb":"PP"), plotLabel.c_str(), (cut.dMuon.Pt.Min*10.0), (cut.dMuon.Pt.Max*10.0), (cut.dMuon.AbsRap.Min*10.0), (cut.dMuon.AbsRap.Max*10.0), cut.Centrality.Start, cut.Centrality.End));
  
  cFig->Clear();
  cFig->Close();
  
  // Save the workspace
  if (saveWS) {
     gSystem->mkdir(Form("%sresult/%s/", outputDir.c_str(), DSTAG.c_str()), kTRUE); 
     TFile *file = NULL;
     file = new TFile(Form("%sresult/%s/FIT_%s_%s_%s%s_pt%.0f%.0f_rap%.0f%.0f_cent%d%d.root", outputDir.c_str(), DSTAG.c_str(), DSTAG.c_str(), "Psi2SJpsi", (isPbPb?"PbPb":"PP"), plotLabel.c_str(), (cut.dMuon.Pt.Min*10.0), (cut.dMuon.Pt.Max*10.0), (cut.dMuon.AbsRap.Min*10.0), (cut.dMuon.AbsRap.Max*10.0), cut.Centrality.Start, cut.Centrality.End), "RECREATE");  
     if (!file) { 
        cout << "[ERROR] Output root file with fit results could not be created!" << endl; 
     } else {
        file->cd();    
        myws.Write("workspace"); 
        file->Write(); file->Close(); delete file;
     }
  }
}


void setRange(RooWorkspace& myws, RooPlot* frame, string dsName, bool setLogScale, double dMuonYmin)
{ 
  // Find maximum and minimum points of Plot to rescale Y axis
  TH1* h = myws.data(dsName.c_str())->createHistogram("hist", *myws.var("invMass"), Binning(frame->GetNbinsX(),frame->GetXaxis()->GetXmin(),frame->GetXaxis()->GetXmax()));
  Double_t YMax = h->GetBinContent(h->GetMaximumBin());
  // Double_t YMin = min( h->GetBinContent(h->FindFirstBinAbove(0.0)), h->GetBinContent(h->FindLastBinAbove(0.0)) );
  Double_t YMin = 1e99;
  for (int i=1; i<=h->GetNbinsX(); i++) if (h->GetBinContent(i)>0) YMin = min(YMin, h->GetBinContent(i));
  
  bool isMC = false;
  if (dsName.find("MC")!=std::string::npos) isMC = true;
    
  Double_t Yup(0.),Ydown(0.);
  if(setLogScale)
  {
    if (isMC) Ydown = YMin*0.3;
    else Ydown = YMin/(TMath::Power((YMax/YMin), (0.1/(1.0-0.1-0.4))));
    Yup = YMax*TMath::Power((YMax/YMin), (0.4/(1.0-0.1-0.4)));
  }
  else
  {
    Ydown = max(YMin-(YMax-YMin)*(0.1/(1.0-0.1-0.4)),0.0);
    Yup = YMax+(YMax-YMin)*(0.4/(1.0-0.1-0.4));
  }
  frame->GetYaxis()->SetRangeUser(Ydown,Yup);
  delete h;
  
  // Create line to indicate upper fitting range for MC
  if (isMC)
  {
    if (dsName.find("JPSIP")!=std::string::npos)
    {
      TLine* line(0x0);
      if (dMuonYmin >= 1.6) line = new TLine(3.32,Ydown,3.32,Yup);
      else line = new TLine(3.26,Ydown,3.26,Yup);
      line->SetLineStyle(2);
      line->SetLineColor(1);
      line->SetLineWidth(3);
      
      frame->addObject(line);
    }
    else if (dsName.find("PSI2S")!=std::string::npos)
    {
      TLine* line(0x0);
      if (dMuonYmin >= 1.6) line = new TLine(3.95,Ydown,3.95,Yup);
      else line = new TLine(3.85,Ydown,3.85,Yup);
      line->SetLineStyle(2);
      line->SetLineColor(1);
      line->SetLineWidth(3);
      
      frame->addObject(line);
    }

  }
 
}

void makeAllCtauPlots(const char* workDirName, const char* DSTag, bool paperStyle, bool incSS) {
   // list of files
   vector<TString> theFiles = fileList(workDirName,"",DSTag);

   for (vector<TString>::const_iterator it=theFiles.begin(); it!=theFiles.end(); it++) {
      // if this is not a MB bin, skip it
      anabin thebin = binFromFile(*it);
      if (!(thebin==anabin(0,1.6,6.5,30,0,200) || thebin==anabin(1.6,2.4,3,30,0,200))) continue;

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

      bool isPbPb = (it->Index("PbPb")>0);
      struct KinCuts cut;
      cut.dMuon.M.Min = 3;
      cut.dMuon.M.Max = 3.2;
      
      cut.Centrality.Start = thebin.centbin().low();
      cut.Centrality.End = thebin.centbin().high();
      cut.dMuon.Pt.Min = thebin.ptbin().low();
      cut.dMuon.Pt.Max = thebin.ptbin().high();
      cut.dMuon.AbsRap.Min = thebin.rapbin().low();
      cut.dMuon.AbsRap.Max = thebin.rapbin().high();

      string indMuonMass    = Form("(%.6f < invMass && invMass < %.6f)",       cut.dMuon.M.Min,       cut.dMuon.M.Max);
      // if (fitSB) indMuonMass = indMuonMass + "&&" + "((2.0 < invMass && invMass < 2.7) || (3.3 < invMass && invMass < 3.45) || (3.9 < invMass && invMass < 5.0))";
      string indMuonRap     = Form("(%.6f <= abs(rap) && abs(rap) < %.6f)",    cut.dMuon.AbsRap.Min,  cut.dMuon.AbsRap.Max);
      string indMuonPt      = Form("(%.6f <= pt && pt < %.6f)",                cut.dMuon.Pt.Min,      cut.dMuon.Pt.Max);
      // string indMuonCtau    = Form("(%.6f < ctau && ctau < %.6f)",             cut.dMuon.ctau.Min,    cut.dMuon.ctau.Max); 
      // if(cut.dMuon.ctauCut!=""){ indMuonCtau = cut.dMuon.ctauCut; }
      // string indMuonCtauErr = Form("(%.6f < ctauErr && ctauErr < %.6f)",       cut.dMuon.ctauErr.Min, cut.dMuon.ctauErr.Max);
      string inCentrality   = Form("(%d <= cent && cent < %d)",                cut.Centrality.Start,  cut.Centrality.End);

      string strCut         = indMuonMass +"&&"+ indMuonRap +"&&"+ indMuonPt;// +"&&"+ indMuonCtau +"&&"+ indMuonCtauErr;
      if (isPbPb){ strCut = strCut +"&&"+ inCentrality; } 

      // RooWorkspace *myws = (RooWorkspace*) f->Get("workspace");
      TFile *fmaster = NULL;
      if (isPbPb) fmaster = TFile::Open("DataSet/DATASET_DATA_PbPb.root");
      else fmaster = TFile::Open("DataSet/DATASET_DATA_PP.root");
      if (!fmaster || !fmaster->IsOpen()) continue;
      RooDataSet *dOS = (RooDataSet*) fmaster->Get(Form("dOS_DATA_%s",isPbPb ? "PbPb" : "PP"));
      RooDataSet *dSS = (RooDataSet*) fmaster->Get(Form("dSS_DATA_%s",isPbPb ? "PbPb" : "PP"));
      RooWorkspace *myws = new RooWorkspace("workspace");
      cout << strCut << endl;
      if (dOS) {
         RooDataSet *dOS2 = (RooDataSet*) dOS->reduce(strCut.c_str());
         myws->import(*dOS2);
         cout << dOS2->sumEntries() << endl;
      }
      if (dSS) {
         RooDataSet *dSS2 = (RooDataSet*) dSS->reduce(strCut.c_str());
         myws->import(*dSS2);
         cout << dSS2->sumEntries() << endl;
      }


      string outputDir = string("Output/") + string(workDirName) + string("/");
      struct InputOpt opt;
      opt.pp.RunNb.Start   = 262157; opt.PbPb.RunNb.Start = 262620;
      opt.pp.RunNb.End     = 262328; opt.PbPb.RunNb.End   = 263757;
      opt.pp.TriggerBit    = (int) PP::HLT_HIL1DoubleMu0_v1; 
      opt.PbPb.TriggerBit  = (int) HI::HLT_HIL1DoubleMu0_v1; 
      string plotLabel = "";
      bool cutCtau = true;
      bool doSimulFit = false;
      cout << *it << " " << it->Index("PbPb") << endl;
      bool setLogScale = true; //!paperStyle;
      bool zoomPsi = paperStyle;
      int nBins = 80;

      drawCtauPlot(*myws, outputDir, opt, cut, plotLabel, DSTag, isPbPb, setLogScale, incSS, nBins, paperStyle, false);

      delete myws;
      delete f;
      delete fmaster;
   }
}

#endif // #ifndef drawMassPlot_C
