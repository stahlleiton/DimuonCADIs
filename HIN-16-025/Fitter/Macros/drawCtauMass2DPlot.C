#ifndef drawCtauMass2DPlot_C
#define drawCtauMass2DPlot_C

#include "Utilities/initClasses.h"


void drawCtauMass2DPlot(RooWorkspace& myws,          // Local workspace
                        const string& outputDir,     // Output directory
                        const struct KinCuts& cut,   // Variable with current kinematic cuts
                        const string& plotLabel,     // The label used to define the output file name
                        // Select the type of datasets to fit
                              string DSTAG,          // Specifies the type of datasets: i.e, DATA, MCJPSINP, ...
                        const bool& isPbPb,          // Define if it is PbPb (True) or PP (False)\
                        // Select the drawing options
                        const map<string, double>& binWidth={} // User-defined Location of the fit results
                        )
{

  gStyle->SetOptStat(0);

  if (DSTAG.find("_")!=std::string::npos) DSTAG.erase(DSTAG.find("_"));

  const double minRangeCtau = -0.5;
  const double maxRangeCtau = 2.0;
  const int nBinsCtau = min(int( round((maxRangeCtau - minRangeCtau)/binWidth.at("CTAU")*2) ), 1000);

  const double minRangeMass = cut.dMuon.M.Min;
  const double maxRangeMass = cut.dMuon.M.Max;
  const int nBinsMass = min(int( round((maxRangeMass - minRangeMass)/binWidth.at("MASS")) ), 1000);
  const string pdfTotName  = Form("pdfCTAUMASS_Tot_%s", (isPbPb?"PbPb":"PP"));
  auto hPDF = std::unique_ptr<TH1>(((RooAbsReal*)myws.pdf(pdfTotName.c_str()))->createHistogram("PDF 2D",*myws.var("ctau"), Extended(kTRUE), Binning(nBinsCtau, minRangeCtau, maxRangeCtau), YVar(*myws.var("invMass"), Binning(nBinsMass, minRangeMass, maxRangeMass))));

  const string dsOSName = Form("dOS_%s_%s", DSTAG.c_str(), (isPbPb?"PbPb":"PP"));
  auto hDATA = std::unique_ptr<TH1>(((RooDataSet*)myws.data(dsOSName.c_str()))->createHistogram("DATA 2D",*myws.var("ctau"), Binning(nBinsCtau, minRangeCtau, maxRangeCtau), YVar(*myws.var("invMass"), Binning(nBinsMass, minRangeMass, maxRangeMass))));
  
  // Create the main canvas
  auto cFigPDF = std::unique_ptr<TCanvas>(new TCanvas(Form("cCtauMassPDF_%s", (isPbPb?"PbPb":"PP")), "cCtauMassPDF",2000,2000));
  cFigPDF->cd();

  hPDF->GetYaxis()->CenterTitle(kTRUE);
  hPDF->GetYaxis()->SetTitleOffset(2.1);
  hPDF->GetYaxis()->SetTitleSize(0.035);
  hPDF->GetYaxis()->SetLabelSize(0.025);
  hPDF->GetYaxis()->SetTitle("Mass [GeV/c]");
  hPDF->GetXaxis()->CenterTitle(kTRUE);
  hPDF->GetXaxis()->SetTitleOffset(2.1);
  hPDF->GetXaxis()->SetTitleSize(0.035);
  hPDF->GetXaxis()->SetLabelSize(0.025);
  hPDF->GetXaxis()->SetTitle("#font[12]{l}_{J/#psi} (mm)");
  hPDF->GetZaxis()->SetTitleOffset(2.0);
  hPDF->GetZaxis()->SetTitleSize(0.035);
  hPDF->GetZaxis()->SetLabelSize(0.025);
  hPDF->GetYaxis()->SetRangeUser(minRangeMass, maxRangeMass);
  hPDF->GetXaxis()->SetRangeUser(minRangeCtau, maxRangeCtau);
  cFigPDF->SetLogz(kTRUE);

  hPDF->Draw("LEGO2");

  gSystem->mkdir(Form("%sctauMass/%s/plot/pdf2D/", outputDir.c_str(), DSTAG.c_str()), kTRUE); 
  cFigPDF->SaveAs(Form("%sctauMass/%s/plot/pdf2D/PLOT_%s_%s_%s%s_pt%.0f%.0f_rap%.0f%.0f_cent%d%d.pdf", outputDir.c_str(), DSTAG.c_str(), "CTAUMASSPDF", DSTAG.c_str(), (isPbPb?"PbPb":"PP"), plotLabel.c_str(), (cut.dMuon.Pt.Min*10.0), (cut.dMuon.Pt.Max*10.0), (cut.dMuon.AbsRap.Min*10.0), (cut.dMuon.AbsRap.Max*10.0), cut.Centrality.Start, cut.Centrality.End));

  cFigPDF->Clear();
  cFigPDF->Close();

  // Create the main canvas
  auto cFigDATA = std::unique_ptr<TCanvas>(new TCanvas(Form("cCtauMassPDF_%s", (isPbPb?"PbPb":"PP")), "cCtauMassPDF",2000,2000));
  cFigDATA->cd();

  hDATA->GetYaxis()->CenterTitle(kTRUE);
  hDATA->GetYaxis()->SetTitleOffset(2.1);
  hDATA->GetYaxis()->SetTitleSize(0.035);
  hDATA->GetYaxis()->SetLabelSize(0.025);
  hDATA->GetYaxis()->SetTitle("Mass [GeV/c]");
  hDATA->GetXaxis()->CenterTitle(kTRUE);
  hDATA->GetXaxis()->SetTitleOffset(2.1);
  hDATA->GetXaxis()->SetTitleSize(0.035);
  hDATA->GetXaxis()->SetLabelSize(0.025);
  hDATA->GetXaxis()->SetTitle("#font[12]{l}_{J/#psi} (mm)");
  hDATA->GetZaxis()->SetTitleOffset(2.0);
  hDATA->GetZaxis()->SetTitleSize(0.035);
  hDATA->GetZaxis()->SetLabelSize(0.025);
  hDATA->GetYaxis()->SetRangeUser(minRangeMass, maxRangeMass);
  hDATA->GetXaxis()->SetRangeUser(minRangeCtau, maxRangeCtau);
  cFigDATA->SetLogz(kTRUE);

  hDATA->Draw("LEGO2");

  gSystem->mkdir(Form("%sctauMass/%s/plot/pdf2D/", outputDir.c_str(), DSTAG.c_str()), kTRUE); 
  cFigDATA->SaveAs(Form("%sctauMass/%s/plot/pdf2D/PLOT_%s_%s_%s%s_pt%.0f%.0f_rap%.0f%.0f_cent%d%d.pdf", outputDir.c_str(), DSTAG.c_str(), "CTAUMASSDATA", DSTAG.c_str(), (isPbPb?"PbPb":"PP"), plotLabel.c_str(), (cut.dMuon.Pt.Min*10.0), (cut.dMuon.Pt.Max*10.0), (cut.dMuon.AbsRap.Min*10.0), (cut.dMuon.AbsRap.Max*10.0), cut.Centrality.Start, cut.Centrality.End));

  cFigDATA->Clear();
  cFigDATA->Close();
};


#endif // #ifndef drawCtauMass2DPlot_C
