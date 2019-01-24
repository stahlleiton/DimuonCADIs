#ifndef drawCtauErrorPlot_C
#define drawCtauErrorPlot_C

#include "Utilities/initClasses.h"
#include "TGaxis.h"

void setCtauErrorRange ( RooPlot& frame, const RooWorkspace& myws, const string& dsName, const bool& setLogScale, const vector<double>& rangeErr );
void setCCBinning      ( TH1& hist, const int& nMaxBins, const RooBinning& bins, const vector<double>& rangeErr);

void drawCtauErrorPlot(RooWorkspace& myws,          // Local workspace
                       const string& outputDir,     // Output directory
                       const struct InputOpt& opt,  // Variable with run information (kept for legacy purpose)
                       const struct KinCuts& cut,   // Variable with current kinematic cuts
                       const map<string, string>&  parIni,   // Variable containing all initial parameters
                       const string& plotLabel,     // The label used to define the output file name
                       // Select the type of datasets to fit
                             string DSTAG,          // Specifies the type of datasets: i.e, DATA, MCJPSINP, ...
                       const bool& isPbPb,          // Define if it is PbPb (True) or PP (False)
                       // Select the type of object to fit
                       const bool& incJpsi,         // Includes Jpsi model
                       const bool& incPsi2S,        // Includes Psi(2S) model
                       const bool& incBkg,          // Includes Background model        
                       // Select the fitting options
                       const bool& plotPureSMC,     // Flag to indicate if we want to fit pure signal MC
                       // Select the drawing options
                       const bool& setLogScale,     // Draw plot with log scale
                       const bool& incSS,           // Include Same Sign data
                       const double&  binWidth      // Bin width
                       ) 
{

  RooMsgService::instance().getStream(0).removeTopic(RooFit::Caching);
  RooMsgService::instance().getStream(0).removeTopic(RooFit::Plotting);
  RooMsgService::instance().getStream(0).removeTopic(RooFit::Integration);
  RooMsgService::instance().getStream(0).removeTopic(RooFit::NumIntegration);
  RooMsgService::instance().getStream(0).removeTopic(RooFit::Minimization);
  RooMsgService::instance().getStream(1).removeTopic(RooFit::Caching);
  RooMsgService::instance().getStream(1).removeTopic(RooFit::Plotting);
  RooMsgService::instance().getStream(1).removeTopic(RooFit::Integration);
  RooMsgService::instance().getStream(1).removeTopic(RooFit::NumIntegration);
  RooMsgService::instance().getStream(1).removeTopic(RooFit::Minimization);
  RooMsgService::instance().setGlobalKillBelow(RooFit::WARNING);


  const string hOSName = Form("dhCTAUERRTot_Tot_%s", (isPbPb?"PbPb":"PP"));
  string dsOSName = Form("dOS_%s_%s", DSTAG.c_str(), (isPbPb?"PbPb":"PP"));
  const string dsSSName = Form("dSS_%s_%s", DSTAG.c_str(), (isPbPb?"PbPb":"PP"));
  if (plotPureSMC) dsOSName = Form("dOS_%s_%s_NoBkg", DSTAG.c_str(), (isPbPb?"PbPb":"PP"));

  vector<double> rangeErr; rangeErr.push_back(cut.dMuon.ctauErr.Min); rangeErr.push_back(cut.dMuon.ctauErr.Max);

  // Create the main plot of the fitq
  const double minRange = (double)(floor(rangeErr[0]*100.)/100.);
  const double maxRange = (double)(ceil(rangeErr[1]*100.)/100.);
  const int nBins = min(int( round((cut.dMuon.ctauErr.Max - cut.dMuon.ctauErr.Min)/binWidth) ), 1000);
  auto frame = std::unique_ptr<RooPlot>(myws.var("ctauErr")->frame(Range(minRange, maxRange)));
  const RooBinning bins(nBins, cut.dMuon.ctauErr.Min, cut.dMuon.ctauErr.Max);
  const double norm = myws.data(hOSName.c_str())->sumEntries();
  const double outTot = myws.data(dsOSName.c_str())->numEntries();
  auto dsRed = std::unique_ptr<RooAbsData>(myws.data(dsOSName.c_str())->reduce(Form("(ctauErr>=%.6f || ctauErr<=%.6f)", rangeErr[1], rangeErr[0])));
  const double outErr = dsRed->numEntries();
 
  myws.data(hOSName.c_str())->plotOn(frame.get(), Name("dOS"), MarkerColor(kBlack), LineColor(kBlack), MarkerSize(1.2), DataError(RooAbsData::SumW2), Binning(bins));
  if ((incJpsi&&incBkg) || (incPsi2S&&incBkg)) { myws.pdf(Form("pdfCTAUERRTot_Tot_%s", (isPbPb?"PbPb":"PP")))->plotOn(frame.get(),Name("PDF"), LineStyle(1), LineColor(kGreen+1), Precision(1e-6), Range("CtauErrFullWindow") ); }
  if (incBkg) {
    const string pdfName = Form("pdfCTAUERR_Bkg_%s", (isPbPb?"PbPb":"PP"));
    const string dataName = Form("dhCTAUERR_Bkg_%s", (isPbPb?"PbPb":"PP"));
    myws.data(dataName.c_str())->plotOn(frame.get(), Name("BKGDATA"), DataError(RooAbsData::SumW2), MarkerColor(kBlue-4), MarkerSize(0.8), Binning(bins));
    myws.pdf(pdfName.c_str())->plotOn(frame.get(),Name("BKG"), LineStyle(1), LineColor(kBlue+1), Precision(1e-6), Range("CtauErrFullWindow") );
  }
  if (incPsi2S) {
    const string pdfName = Form("pdfCTAUERR_Psi2S_%s", (isPbPb?"PbPb":"PP")); 
    const string dataName = Form("dhCTAUERR_Psi2S_%s", (isPbPb?"PbPb":"PP"));
    myws.data(dataName.c_str())->plotOn(frame.get(), Name("PSI2SDATA"), DataError(RooAbsData::SumW2), MarkerColor(kViolet-2), MarkerSize(0.8), Binning(bins));
    myws.pdf(pdfName.c_str())->plotOn(frame.get(),Name("PSI2S"), LineStyle(1), LineColor(kViolet+2), Precision(1e-6), Range("CtauErrFullWindow") );
  }
  if (incJpsi) {
    const string pdfName = Form("pdfCTAUERR_Jpsi_%s", (isPbPb?"PbPb":"PP")); 
    const string dataName = Form("dhCTAUERR_Jpsi_%s", (isPbPb?"PbPb":"PP"));
    myws.data(dataName.c_str())->plotOn(frame.get(), Name("JPSIDATA"), DataError(RooAbsData::SumW2), MarkerColor(kRed-4), MarkerSize(0.8), Binning(bins));
    myws.pdf(pdfName.c_str())->plotOn(frame.get(),Name("JPSI"), LineStyle(1), LineColor(kRed+3), Precision(1e-6), Range("CtauErrFullWindow") );
  }
 
  if (incSS) { 
    myws.data(dsSSName.c_str())->plotOn(frame.get(), Name("dSS"), MarkerColor(kRed), LineColor(kRed), MarkerSize(1.2), Binning(bins)); 
  }

  // Create the pull distribution of the fit
  RooHist *hpull = frame->pullHist(0, 0, true);
  hpull->SetName("hpull");
  auto frame2 = std::unique_ptr<RooPlot>(myws.var("ctauErr")->frame(Title("Pull Distribution"), Range(minRange, maxRange)));
  frame2->addPlotable(hpull, "PX");

  // set the CMS style
  setTDRStyle();
  
  // Create the main canvas
  auto cFig  = std::unique_ptr<TCanvas>(new TCanvas(Form("cCtauErrFig_%s", (isPbPb?"PbPb":"PP")), "cCtauErrFig",800,800)); cFig->cd();
  auto pad1  = new TPad(Form("pad1_%s", (isPbPb?"PbPb":"PP")),"",0,0.23,1,1);
  auto pad2  = new TPad(Form("pad2_%s", (isPbPb?"PbPb":"PP")),"",0,0,1,.228);
  auto pline = std::unique_ptr<TLine>(new TLine(cut.dMuon.ctauErr.Min, 0.0, cut.dMuon.ctauErr.Max, 0.0));

  frame->SetTitle("");
  frame->GetXaxis()->SetTitle("");
  frame->GetXaxis()->CenterTitle(kTRUE);
  frame->GetXaxis()->SetTitleSize(0.045);
  frame->GetXaxis()->SetTitleFont(42);
  frame->GetXaxis()->SetTitleOffset(3);
  frame->GetXaxis()->SetLabelOffset(3);
  frame->GetYaxis()->SetLabelSize(0.04);
  frame->GetYaxis()->SetTitleSize(0.04);
  frame->GetYaxis()->SetTitleOffset(1.7);
  frame->GetYaxis()->SetTitleFont(42);
  setCtauErrorRange(*frame, myws, dsOSName, setLogScale, rangeErr);

  cFig->cd();
  pad2->SetTopMargin(0.02);
  pad2->SetBottomMargin(0.4);
  pad2->SetFillStyle(4000); 
  pad2->SetFrameFillStyle(4000); 
  pad1->SetBottomMargin(0.015); 
  //plot fit
  pad1->Draw();
  pad1->cd(); 
  frame->Draw();

  pad1->SetLogy(setLogScale);

  // Drawing the text in the plot
  TLatex t = TLatex(); t.SetNDC(); t.SetTextSize(0.032);
  float dy = 0; 
  
  t.SetTextSize(0.03);
  t.DrawLatex(0.21, 0.86-dy, "2015 HI Soft Muon ID"); dy+=0.045;
  if (isPbPb) {
    t.DrawLatex(0.21, 0.86-dy, "HLT_HIL1DoubleMu0_v1"); dy+=0.045;
  } else {
    t.DrawLatex(0.21, 0.86-dy, "HLT_HIL1DoubleMu0_v1"); dy+=0.045;
  } 
  if (isPbPb) {t.DrawLatex(0.21, 0.86-dy, Form("Cent. %d-%d%%", (int)(cut.Centrality.Start/2), (int)(cut.Centrality.End/2))); dy+=0.045;}
  t.DrawLatex(0.21, 0.86-dy, Form("%.1f #leq p_{T}^{#mu#mu} < %.1f GeV/c",cut.dMuon.Pt.Min,cut.dMuon.Pt.Max)); dy+=0.045;
  t.DrawLatex(0.70, 0.86-dy, Form("Loss: (%.4f%%) %.0f evts", (outErr*100.0/outTot), outErr));
  t.DrawLatex(0.21, 0.86-dy, Form("%.1f #leq |y^{#mu#mu}| < %.1f",cut.dMuon.AbsRap.Min,cut.dMuon.AbsRap.Max)); dy+=1.5*0.045;

  // Drawing the Legend
  double ymin = 0.7802;
  if (incPsi2S && incJpsi && incSS)  { ymin = 0.7202; } 
  if (incPsi2S && incJpsi && !incSS) { ymin = 0.7452; }
  TLegend leg(0.5175, ymin, 0.7180, 0.8809); leg.SetTextSize(0.03);
  if (frame->findObject("dOS")) { leg.AddEntry(frame->findObject("dOS"), (incSS?"Opposite Charge":"Data"),"pe"); }
  if (incSS && frame->findObject("dSS")) { leg.AddEntry(frame->findObject("dSS"),"Same Charge","pe"); }
  if(((incJpsi&&incBkg) || (incPsi2S&&incBkg))&&frame->findObject("PDF")) { leg.AddEntry(frame->findObject("PDF"),"Total PDF","l"); }
  if(incJpsi && frame->findObject("JPSI")) { leg.AddEntry(frame->findObject("JPSI"),"J/#psi PDF","l"); }
  if(incPsi2S && frame->findObject("PSI2S")) { leg.AddEntry(frame->findObject("PSI2S"),"#psi(2S) PDF","l"); }
  if(incBkg && frame->findObject("BKG")) { leg.AddEntry(frame->findObject("BKG"),"Background","l"); }
  leg.Draw("same");

  //Drawing the title
  string label;
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
  
  CMS_lumi(pad1, isPbPb ? 108 : 107, 33, "");
  gStyle->SetTitleFontSize(0.05);
  
  pad1->Update();
  cFig->cd(); 

  //---plot pull
  pad2->Draw();
  pad2->cd();
    
  frame2->SetTitle("");
  frame2->GetYaxis()->CenterTitle(kTRUE);
  frame2->GetYaxis()->SetTitleOffset(0.4);
  frame2->GetYaxis()->SetTitleSize(0.1);
  frame2->GetYaxis()->SetLabelSize(0.1);
  frame2->GetYaxis()->SetTitle("Pull");
  frame2->GetXaxis()->CenterTitle(kTRUE);
  frame2->GetXaxis()->SetTitleOffset(1);
  frame2->GetXaxis()->SetTitleSize(0.12);
  frame2->GetXaxis()->SetLabelSize(0.1);
  frame2->GetXaxis()->SetTitle("#font[12]{l}_{J/#psi} Error (mm)");
  frame2->GetYaxis()->SetRangeUser(-7.0, 7.0);

  frame2->Draw(); 
  
  // *** Print chi2/ndof 
  //printChi2(myws, *pad2, *frame, "ctauErr", dsOSName, pdfTotName, nBins);

  myws.var("ctauErr")->setMin(rangeErr[0]);
  myws.var("ctauErr")->setMax(rangeErr[1]);

  pline->Draw("same");
  pad2->Update();
 
  const bool SB = (incBkg&&(!incPsi2S&&!incJpsi));
  // Save the plot in different formats
  gSystem->mkdir(Form("%sctauErr%s/%s/plot/root/", outputDir.c_str(), (SB?"SB":""), DSTAG.c_str()), kTRUE); 
  cFig->SaveAs(Form("%sctauErr%s/%s/plot/root/PLOT_%s_%s_%s%s_pt%.0f%.0f_rap%.0f%.0f_cent%d%d.root", outputDir.c_str(), (SB?"SB":""), DSTAG.c_str(), "CTAUERR", DSTAG.c_str(), (isPbPb?"PbPb":"PP"), plotLabel.c_str(), (cut.dMuon.Pt.Min*10.0), (cut.dMuon.Pt.Max*10.0), (cut.dMuon.AbsRap.Min*10.0), (cut.dMuon.AbsRap.Max*10.0), cut.Centrality.Start, cut.Centrality.End));
  gSystem->mkdir(Form("%sctauErr%s/%s/plot/png/", outputDir.c_str(), (SB?"SB":""), DSTAG.c_str()), kTRUE);
  cFig->SaveAs(Form("%sctauErr%s/%s/plot/png/PLOT_%s_%s_%s%s_pt%.0f%.0f_rap%.0f%.0f_cent%d%d.png", outputDir.c_str(), (SB?"SB":""), DSTAG.c_str(), "CTAUERR", DSTAG.c_str(), (isPbPb?"PbPb":"PP"), plotLabel.c_str(), (cut.dMuon.Pt.Min*10.0), (cut.dMuon.Pt.Max*10.0), (cut.dMuon.AbsRap.Min*10.0), (cut.dMuon.AbsRap.Max*10.0), cut.Centrality.Start, cut.Centrality.End));
  gSystem->mkdir(Form("%sctauErr%s/%s/plot/pdf/", outputDir.c_str(), (SB?"SB":""), DSTAG.c_str()), kTRUE);
  cFig->SaveAs(Form("%sctauErr%s/%s/plot/pdf/PLOT_%s_%s_%s%s_pt%.0f%.0f_rap%.0f%.0f_cent%d%d.pdf", outputDir.c_str(), (SB?"SB":""), DSTAG.c_str(), "CTAUERR", DSTAG.c_str(), (isPbPb?"PbPb":"PP"), plotLabel.c_str(), (cut.dMuon.Pt.Min*10.0), (cut.dMuon.Pt.Max*10.0), (cut.dMuon.AbsRap.Min*10.0), (cut.dMuon.AbsRap.Max*10.0), cut.Centrality.Start, cut.Centrality.End));
  
  cFig->Clear();
  cFig->Close();
}

void setCtauErrorRange(RooPlot& frame, const RooWorkspace& myws, const string& dsName, const bool& setLogScale, const vector<double>& rangeErr)
{ 
  // Find maximum and minimum points of Plot to rescale Y axis
  auto h = std::unique_ptr<TH1>(myws.data(dsName.c_str())->createHistogram("hist", *myws.var("ctauErr"), Binning(frame.GetNbinsX(),frame.GetXaxis()->GetXmin(),frame.GetXaxis()->GetXmax())));
  const double YMax = h->GetBinContent(h->GetMaximumBin());
  double YMin = 1e99;
  for (int i=1; i<=h->GetNbinsX(); i++) if (h->GetBinContent(i)>0) YMin = min(YMin, h->GetBinContent(i));
  
  bool isMC = false;
  if (dsName.find("MC")!=std::string::npos) isMC = true;
    
  double Yup(0.),Ydown(0.);
  if(setLogScale)
  {
    Yup = YMax*TMath::Power((YMax/YMin), (0.4/(1.0-0.4-0.3)));
    Ydown = YMin/(TMath::Power((YMax/YMin), (0.3/(1.0-0.4-0.3))));
  }
  else
  {
    Ydown = max(YMin-(YMax-YMin)*0.2,0.0);
    Yup = YMax+(YMax-YMin)*0.5;
  }
  frame.GetYaxis()->SetRangeUser(Ydown,Yup);

  auto minline = new TLine(rangeErr[0], 0.0, rangeErr[0], (setLogScale?(Ydown*TMath::Power((Yup/Ydown),0.5)):(Ydown + (Yup-Ydown)*0.5)));
  minline->SetLineStyle(2);
  minline->SetLineColor(1);
  minline->SetLineWidth(3);
  frame.addObject(minline);
  auto maxline = new TLine(rangeErr[1], 0.0, rangeErr[1], (setLogScale?(Ydown*TMath::Power((Yup/Ydown),0.5)):(Ydown + (Yup-Ydown)*0.5)));
  maxline->SetLineStyle(2);
  maxline->SetLineColor(1);
  maxline->SetLineWidth(3);
  frame.addObject(maxline);
};


#endif // #ifndef drawCtauErrorPlot_C
