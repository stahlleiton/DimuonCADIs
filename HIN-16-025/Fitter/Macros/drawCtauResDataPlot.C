#ifndef drawCtauResDataPlot_C
#define drawCtauResDataPlot_C

#include "Utilities/initClasses.h"

void setCtauResDataRange        ( RooPlot& frame, const RooWorkspace& myws, const string& dsName, const string& varName, const bool& setLogScale, const vector<double>& rangeErr, const double excEvts=0.0 );
void printCtauResDataParameters ( TPad& pad, const RooWorkspace& myws, const bool& isPbPb, const string& pdfName, const bool& isWeighted );

bool drawCtauResDataPlot(RooWorkspace& myws,    // Local workspace
                         const string& outputDir,     // Output directory
                         const struct InputOpt& opt,  // Variable with run information (kept for legacy purpose)
                         const struct KinCuts& cut,   // Variable with current kinematic cuts
                         const map<string, string>& parIni,   // Variable containing all initial parameters
                         const string& plotLabel,     // The label used to define the output file name
                         // Select the type of datasets to fit
                               string DSTAG,          // Specifies the type of datasets: i.e, DATA, MCJPSINP, ...
                         const string& dsName,        // Specifies dataset name
                         const bool& isPbPb,          // Define if it is PbPb (True) or PP (False)
                         // Select the type of object to fit
                         // Select the fitting options
                         // Select the drawing options
                         const bool& setLogScale,     // Draw plot with log scale
                         const bool& incSS,           // Include Same Sign data
                         const double& binWidth       // Bin width
                         ) 
{

  if (DSTAG.find("_")!=std::string::npos) DSTAG.erase(DSTAG.find("_"));
  
  const string dsNameCut = dsName+"_CTAUNCUT"; // This is the dataset used to fit

  const bool isWeighted = myws.data(dsName.c_str())->isWeighted();
  const bool isMC = (DSTAG.find("MC")!=std::string::npos);
  const bool incJpsi = (dsName.find("JPSI")!=std::string::npos);
  const bool incPsi2S = (dsName.find("PSI2S")!=std::string::npos);
  const bool incNonPrompt = (DSTAG.find("NOPR")!=std::string::npos);
  vector<double> range; range.push_back(cut.dMuon.ctauN.Min); range.push_back(cut.dMuon.ctauN.Max);

  const string pdfType = "pdfCTAUNRES";
  const string varName = "ctauN";
  const string pdfTotName = Form("%s_Tot_%s", pdfType.c_str(), (isPbPb?"PbPb":"PP"));
  string obj = "Bkg";
  if (incJpsi) obj = "Jpsi";
  if (incPsi2S) obj = "Psi2S";
 
  const double minRange = -10.0;
  const double maxRange = 10.0;
  const double outTot = myws.data(dsName.c_str())->numEntries();
  const double outErr = outTot - (myws.data(dsNameCut.c_str())->numEntries());
  if (outErr<0) { cout << "[ERROR] Number of events is smaller after ctau cut: Total " << outTot << " and cutted " << (myws.data(dsNameCut.c_str())->numEntries()) << endl; return false; }
  const int nBins = min(int( round((maxRange - minRange)/binWidth) ), 1000);
  const int COLOR[] = { kGreen+3, kRed+2, kBlue+2, kViolet-5};

  // Create the main plot of the fit
  auto frame = std::unique_ptr<RooPlot>(myws.var(varName.c_str())->frame(Bins(nBins), Range(minRange, maxRange)));
  frame->updateNormVars(RooArgSet(*myws.var(varName.c_str())));
  myws.data(dsName.c_str())->plotOn(frame.get(), Name("dOS"), DataError(RooAbsData::SumW2), XErrorSize(0), MarkerColor(kBlack), LineColor(kBlack), MarkerSize(1.2));

  int nGauss = 1;
  for (int i=1; i<5; i++) {
    if (myws.pdf(Form("%s%d_%s_%s", pdfType.c_str(), i, obj.c_str(),(isPbPb?"PbPb":"PP")))){
      myws.pdf(pdfTotName.c_str())->plotOn(frame.get(),Name(Form("PDF%d", i)),Components(RooArgSet(*myws.pdf(Form("%s%d_%s_%s", pdfType.c_str(), i, obj.c_str(), (isPbPb?"PbPb":"PP"))))),
                                           Normalization(myws.data(dsNameCut.c_str())->sumEntries(), RooAbsReal::NumEvent),
                                           LineColor(COLOR[i-1]), Precision(1e-5), NormRange("CtauNWindow")
                                           );
      nGauss++;
    }
  }
  
  myws.pdf(pdfTotName.c_str())->plotOn(frame.get(),Name("PDF"), Normalization(myws.data(dsNameCut.c_str())->sumEntries(), RooAbsReal::NumEvent),
                                       LineColor(kBlack), Precision(1e-5), NormRange("CtauNWindow")
                                       );

  myws.data(dsName.c_str())->plotOn(frame.get(), Name("dOS"), DataError(RooAbsData::SumW2), XErrorSize(0), MarkerColor(kBlack), LineColor(kBlack), MarkerSize(1.2));

  // set the CMS style
  setTDRStyle();
  
  // Create the pull distribution of the fit
  RooHist *hpull = frame->pullHist(0, "PDF", true);
  hpull->SetName("hpull");
  auto frame2 = std::unique_ptr<RooPlot>(myws.var(varName.c_str())->frame(Title("Pull Distribution"), Bins(nBins), Range(minRange, maxRange)));
  frame2->addPlotable(hpull, "PX"); 
  // Create the main canvas
  auto cFig  = std::unique_ptr<TCanvas>(new TCanvas(Form("cCtauResDataFig_%s", (isPbPb?"PbPb":"PP")), "cCtauResDataFig",800,800));
  cFig->cd();
  auto pad1  = new TPad(Form("pad1_%s", (isPbPb?"PbPb":"PP")),"",0,0.23,1,1);
  auto pad2  = new TPad(Form("pad2_%s", (isPbPb?"PbPb":"PP")),"",0,0,1,.228);
  auto pline = std::unique_ptr<TLine>(new TLine(minRange, 0.0, maxRange, 0.0));
  
  auto pad4 = new TPad("pad4","This is pad4",0.55,0.46,0.97,0.87);
  pad4->SetFillStyle(0);
  pad4->SetLeftMargin(0.28);
  pad4->SetRightMargin(0.10);
  pad4->SetBottomMargin(0.21);
  pad4->SetTopMargin(0.072);

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
  setCtauResDataRange(*frame, myws, dsName, varName, setLogScale, range, outErr);

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

  printCtauResDataParameters(*pad1, myws, isPbPb, pdfTotName, (isWeighted&&isMC));
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
  t.DrawLatex(0.21, 0.86-dy, Form("%.1f #leq p_{T}^{#mu#mu} < %.1f GeV/c",cut.dMuon.Pt.Min,cut.dMuon.Pt.Max)); dy+=0.045;
  t.DrawLatex(0.21, 0.86-dy, Form("%.1f #leq |y^{#mu#mu}| < %.1f",cut.dMuon.AbsRap.Min,cut.dMuon.AbsRap.Max)); dy+=0.045;
  if (isPbPb) {t.DrawLatex(0.21, 0.86-dy, Form("Cent. %d-%d%%", (int)(cut.Centrality.Start/2), (int)(cut.Centrality.End/2))); dy+=0.045;}
  if (outErr>0.0) {
    if (isPbPb) {
      t.DrawLatex(0.21, 0.86-dy, Form("Excl: (%.4f%%) %.2f evts", (outErr*100.0/outTot), outErr)); dy+=1.5*0.045;
    } else {
      t.DrawLatex(0.21, 0.86-dy, Form("Excl: (%.4f%%) %.0f evts", (outErr*100.0/outTot), outErr)); dy+=1.5*0.045;
    }
  }

  // Drawing the Legend
  double ymin = 0.7202;
  if (incSS)  { ymin = 0.7202; } else { ymin = 0.7452; }
  TLegend leg(0.5175, ymin, 0.7180, 0.8809); leg.SetTextSize(0.03);
  if (frame->findObject("dOS")) { leg.AddEntry(frame->findObject("dOS"), (incSS?"Opposite Charge":"Data"),"pe"); }
  if (incSS && frame->findObject("dSS")) { leg.AddEntry(frame->findObject("dSS"),"Same Charge","pe"); }
  if(frame->findObject("PDF")) { leg.AddEntry(frame->findObject("PDF"),"Total fit","l"); }
  for (int i=1; i<nGauss; i++) {
    if(frame->findObject(Form("PDF%d",i))) { leg.AddEntry(frame->findObject(Form("PDF%d",i)),Form("PR Gauss %d", i),"l"); }
  }
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
  frame2->GetXaxis()->SetTitleOffset(1.1);
  frame2->GetXaxis()->SetTitleSize(0.12);
  frame2->GetXaxis()->SetLabelSize(0.1);
  frame2->GetXaxis()->SetTitle("#frac{#font[12]{l}_{J/#psi}}{#font[12]{#sigma}_{J/#psi}}");
  frame2->GetYaxis()->SetRangeUser(-7.0, 7.0);

  frame2->Draw(); 
  
  // *** Print chi2/ndof 
  printChi2(myws, *pad2, *frame, varName, dsName, pdfTotName, nBins);
  
  pline->Draw("same");
  pad2->Update();
  
  // Save the plot in different formats
  gSystem->mkdir(Form("%sctauRes/%s/plot/root/", outputDir.c_str(), DSTAG.c_str()), kTRUE); 
  cFig->SaveAs(Form("%sctauRes/%s/plot/root/PLOT_%s_%s_%s%s_pt%.0f%.0f_rap%.0f%.0f_cent%d%d.root", outputDir.c_str(), DSTAG.c_str(), "CTAURES", DSTAG.c_str(), (isPbPb?"PbPb":"PP"), plotLabel.c_str(), (cut.dMuon.Pt.Min*10.0), (cut.dMuon.Pt.Max*10.0), (cut.dMuon.AbsRap.Min*10.0), (cut.dMuon.AbsRap.Max*10.0), cut.Centrality.Start, cut.Centrality.End));
  gSystem->mkdir(Form("%sctauRes/%s/plot/png/", outputDir.c_str(), DSTAG.c_str()), kTRUE);
  cFig->SaveAs(Form("%sctauRes/%s/plot/png/PLOT_%s_%s_%s%s_pt%.0f%.0f_rap%.0f%.0f_cent%d%d.png", outputDir.c_str(), DSTAG.c_str(), "CTAURES", DSTAG.c_str(), (isPbPb?"PbPb":"PP"), plotLabel.c_str(), (cut.dMuon.Pt.Min*10.0), (cut.dMuon.Pt.Max*10.0), (cut.dMuon.AbsRap.Min*10.0), (cut.dMuon.AbsRap.Max*10.0), cut.Centrality.Start, cut.Centrality.End));
  gSystem->mkdir(Form("%sctauRes/%s/plot/pdf/", outputDir.c_str(), DSTAG.c_str()), kTRUE);
  cFig->SaveAs(Form("%sctauRes/%s/plot/pdf/PLOT_%s_%s_%s%s_pt%.0f%.0f_rap%.0f%.0f_cent%d%d.pdf", outputDir.c_str(), DSTAG.c_str(), "CTAURES", DSTAG.c_str(), (isPbPb?"PbPb":"PP"), plotLabel.c_str(), (cut.dMuon.Pt.Min*10.0), (cut.dMuon.Pt.Max*10.0), (cut.dMuon.AbsRap.Min*10.0), (cut.dMuon.AbsRap.Max*10.0), cut.Centrality.Start, cut.Centrality.End));
  
  cFig->Clear();
  cFig->Close();

  return true;
}

void setCtauResDataRange(RooPlot& frame, const RooWorkspace& myws, const string& dsName, const string& varName, const bool& setLogScale, const vector<double>& rangeErr, const double excEvts)
{ 
  // Find maximum and minimum points of Plot to rescale Y axis
  auto h = std::unique_ptr<TH1>(myws.data(dsName.c_str())->createHistogram("hist", *myws.var(varName.c_str()), Binning(frame.GetNbinsX(),frame.GetXaxis()->GetXmin(),frame.GetXaxis()->GetXmax())));
  const double YMax = h->GetBinContent(h->GetMaximumBin());
  double YMin = 1e99;
  for (int i=1; i<=h->GetNbinsX(); i++) if (h->GetBinContent(i)>0) YMin = min(YMin, h->GetBinContent(i));

  double Yup(0.),Ydown(0.);

  if(setLogScale)
  {
    Yup = YMax*TMath::Power((YMax/YMin), (0.5/(1.0-0.5-0.2)));
    Ydown = YMin/(TMath::Power((YMax/YMin), (0.2/(1.0-0.5-0.2))));
  }
  else
  {
    Yup = YMax+(YMax-0.0)*0.5;
    Ydown = 0.0;
  }
  frame.GetYaxis()->SetRangeUser(Ydown,Yup);

  if (excEvts>0.0) {
    auto minline = new TLine(rangeErr[0], 0.0, rangeErr[0], (setLogScale?(Ydown*TMath::Power((Yup/Ydown),0.4)):(Ydown + (Yup-Ydown)*0.4)));
    minline->SetLineStyle(2);
    minline->SetLineColor(1);
    minline->SetLineWidth(3);
    frame.addObject(minline);
    auto maxline = new TLine(rangeErr[1], 0.0, rangeErr[1], (setLogScale?(Ydown*TMath::Power((Yup/Ydown),0.4)):(Ydown + (Yup-Ydown)*0.4)));
    maxline->SetLineStyle(2);
    maxline->SetLineColor(1);
    maxline->SetLineWidth(3);
    frame.addObject(maxline);
  }
};


void printCtauResDataParameters(TPad& pad, const RooWorkspace& myws, const bool& isPbPb, const string& pdfName, const bool& isWeighted)
{
  pad.cd();
  TLatex t = TLatex(); t.SetNDC(); t.SetTextSize(0.026); float dy = 0.045;
  auto par = std::unique_ptr<RooArgSet>(myws.pdf(pdfName.c_str())->getParameters(RooArgSet(*myws.var("invMass"), *myws.var("ctau"), *myws.var("ctauErr"))));
  auto params = std::unique_ptr<RooArgSet>((RooArgSet*)par->selectByAttrib("Constant",kFALSE));
  std::unique_ptr<TIterator> parIt = std::unique_ptr<TIterator>(params->createIterator());
  for (RooRealVar* it = (RooRealVar*)parIt->Next(); it!=NULL; it = (RooRealVar*)parIt->Next() ) {
    stringstream ss(it->GetName()); string s1, s2, s3, label;
    getline(ss, s1, '_'); getline(ss, s2, '_'); getline(ss, s3, '_');
    // Parse the parameter's labels
    if(s1=="invMass"){continue;} else if(s1=="ctau"){continue;}
    else if(s1=="ctauN"){continue;} else if(s1=="ctauErr"){continue;} else if(s1=="MassRatio"){continue;}
    else if(s1=="One"){continue;} else if(s1=="mMin"){continue;} else if(s1=="mMax"){continue;}
    else if(s1.find("sigma")!=std::string::npos || s1.find("lambda")!=std::string::npos || s1.find("alpha")!=std::string::npos){
      s1=Form("#%s",s1.c_str());
    }
    else if(s1=="rS21"){ s1="(s_{2}/s_{1})"; }
    else if(s1=="rS32"){ s1="(s_{3}/s_{2})"; }
    else if(s1=="rS43"){ s1="(s_{4}/s_{3})"; }
    //
    if(s2=="CtauRes")  { s2="Res";   }
    else if(s2=="Jpsi" && (s1=="N" || s1=="b"))  { s2="J/#psi";   }
    else if(s2=="Psi2S" && (s1=="N" || s1=="b"))  { s2="#psi(2S)";   }
    else if(s2=="Bkg" && (s1=="N" || s1=="b"))  { s2="Bkg";   }
    else {continue;}
    if(s3!=""){
      label=Form("%s_{%s}^{%s}", s1.c_str(), s2.c_str(), s3.c_str());
    }
    // Print the parameter's results
    if(s1=="N"){ 
      t.DrawLatex(0.69, 0.75-dy, Form((isWeighted?"%s = %.6f#pm%.6f ":"%s = %.0f#pm%.0f "), label.c_str(), it->getValV(), it->getError())); dy+=0.045; 
    }
    else if(s1.find("ctau")!=std::string::npos){ 
      t.DrawLatex(0.69, 0.75-dy, Form("%s = %.4f#pm%.4f mm", (label.insert(1, string("#"))).c_str(), it->getValV(), it->getError())); dy+=0.045; 
    }
    else { 
      t.DrawLatex(0.69, 0.75-dy, Form("%s = %.4f#pm%.4f", label.c_str(), it->getValV(), it->getError())); dy+=0.045; 
    }
  }
};


#endif // #ifndef drawCtauResDataPlot_C
