#include "TFile.h"
#include "TH1.h"
#include "TArrow.h"
#include "TLine.h"
#include "TLatex.h"
#include "TCanvas.h"
#include "TLegend.h"
#include <iostream>
#include "../Fitter/Macros/Utilities/EVENTUTILS.h"

using namespace std;
using namespace HI;

float getmean(TH1F* hist, double xmin, double xmax);
float getmeanB(TH1F* hist, int imin, int imax);
TH1F* flatten(TH1F* input);
void  diff(TH1F* input);

void plotCent(const char* filename="histos.root") {
   TFile *f = TFile::Open(filename);
   if (!f || !f->IsOpen()) return;

   TH1F *hCentrality_mid = (TH1F*) f->Get("hCentrality_mid");
   const int nrebin = 5;
   TH1F *hCentrality_fwd = (TH1F*) f->Get("hCentrality_fwd");
   TH1F *hNpart_mid = (TH1F*) f->Get("hNpart_mid"); hNpart_mid->Rebin(nrebin);
   TH1F *hNpart_fwd = (TH1F*) f->Get("hNpart_fwd"); hNpart_fwd->Rebin(nrebin);

   TH1F *hCentralityMB_mid = flatten(hCentrality_mid);
   TH1F *hCentralityMB_fwd = flatten(hCentrality_fwd);
   TH1F *hNpartMB_mid = flatten(hNpart_mid);
   TH1F *hNpartMB_fwd = flatten(hNpart_fwd);

   diff(hNpart_mid);
   diff(hNpart_fwd);
   diff(hNpartMB_mid);
   diff(hNpartMB_fwd);

   TCanvas *c1 = new TCanvas();
   hCentrality_mid->GetXaxis()->SetTitle("Centrality percentile (%)");
   hCentrality_mid->GetYaxis()->SetTitle("Entries");
   hCentrality_mid->Draw();
   hCentralityMB_mid->Draw("same");
   double ymin = hCentrality_mid->GetMinimum();
   double ymax = 1.*hCentrality_mid->GetMaximum();
   cout << ymin << " " << ymax << endl;
   // bin limits
   TLine *tl = new TLine(); 
   tl->SetLineStyle(2); tl->SetLineWidth(2);
   tl->DrawLine(10,ymin,10,ymax);
   tl->DrawLine(20,ymin,20,ymax);
   tl->DrawLine(30,ymin,30,ymax);
   tl->DrawLine(40,ymin,40,ymax);
   tl->DrawLine(50,ymin,50,ymax);
   // average centrality in each bin
   TArrow *ta = new TArrow(); ta->SetLineStyle(1); ta->SetLineWidth(2);
   ta->SetLineColor(kBlack);
   float x = getmean(hCentralityMB_mid,0,10);
   ta->DrawArrow(x,0.3*ymax,x,ymin+0.0*(ymax-ymin));
   x = getmean(hCentralityMB_mid,10,20);
   ta->DrawArrow(x,0.3*ymax,x,ymin+0.0*(ymax-ymin));
   x = getmean(hCentralityMB_mid,20,30);
   ta->DrawArrow(x,0.3*ymax,x,ymin+0.0*(ymax-ymin));
   x = getmean(hCentralityMB_mid,30,40);
   ta->DrawArrow(x,0.3*ymax,x,ymin+0.0*(ymax-ymin));
   x = getmean(hCentralityMB_mid,40,50);
   ta->DrawArrow(x,0.3*ymax,x,ymin+0.0*(ymax-ymin));
   x = getmean(hCentralityMB_mid,50,100);
   ta->DrawArrow(x,0.3*ymax,x,ymin+0.0*(ymax-ymin));
   ta->SetLineColor(kRed);
   x = getmean(hCentrality_mid,0,10);
   ta->DrawArrow(x,0.3*ymax,x,ymin+0.0*(ymax-ymin));
   x = getmean(hCentrality_mid,10,20);
   ta->DrawArrow(x,0.3*ymax,x,ymin+0.0*(ymax-ymin));
   x = getmean(hCentrality_mid,20,30);
   ta->DrawArrow(x,0.3*ymax,x,ymin+0.0*(ymax-ymin));
   x = getmean(hCentrality_mid,30,40);
   ta->DrawArrow(x,0.3*ymax,x,ymin+0.0*(ymax-ymin));
   x = getmean(hCentrality_mid,40,50);
   ta->DrawArrow(x,0.3*ymax,x,ymin+0.0*(ymax-ymin));
   x = getmean(hCentrality_mid,50,100);
   ta->DrawArrow(x,0.3*ymax,x,ymin+0.0*(ymax-ymin));

   TLegend *tleg = new TLegend(0.59,0.52,0.89,0.67);
   tleg->SetBorderSize(0);
   tleg->AddEntry(hCentrality_mid,"Dimuon data","lp");
   tleg->AddEntry(hCentralityMB_mid,"Flat centrality (\"MB\")","lp");
   tleg->Draw();

   TLatex *tlx = new TLatex();
   tlx->DrawLatex(60,0.8*ymax,"PbPb data, |y|<1.6");
   c1->SaveAs("centrality_mid.pdf");

   // FWD
   hCentrality_fwd->GetXaxis()->SetTitle("Centrality percentile (%)");
   hCentrality_fwd->GetYaxis()->SetTitle("Entries");
   hCentrality_fwd->Draw();
   hCentralityMB_fwd->Draw("same");
   ymin = hCentrality_fwd->GetMinimum();
   ymax = 1.*hCentrality_fwd->GetMaximum();
   cout << ymin << " " << ymax << endl;
   // bin limits
   tl->SetLineStyle(2); tl->SetLineWidth(2);
   tl->DrawLine(20,ymin,20,ymax);
   tl->DrawLine(40,ymin,40,ymax);
   // average centrality in each bin
   ta->SetLineColor(kBlack);
   x = getmean(hCentralityMB_fwd,0,20);
   ta->DrawArrow(x,0.3*ymax,x,ymin+0.0*(ymax-ymin));
   x = getmean(hCentralityMB_fwd,20,40);
   ta->DrawArrow(x,0.3*ymax,x,ymin+0.0*(ymax-ymin));
   x = getmean(hCentralityMB_fwd,40,100);
   ta->DrawArrow(x,0.3*ymax,x,ymin+0.0*(ymax-ymin));
   ta->SetLineColor(kRed);
   x = getmean(hCentrality_fwd,0,20);
   ta->DrawArrow(x,0.3*ymax,x,ymin+0.0*(ymax-ymin));
   x = getmean(hCentrality_fwd,20,40);
   ta->DrawArrow(x,0.3*ymax,x,ymin+0.0*(ymax-ymin));
   x = getmean(hCentrality_fwd,40,100);
   ta->DrawArrow(x,0.3*ymax,x,ymin+0.0*(ymax-ymin));

   tlx->DrawLatex(50,0.8*ymax,"PbPb data, 1.6<|y|<2.4");
   tleg->Draw();
   c1->SaveAs("centrality_fwd.pdf");

   // NPART
   // MID
   hNpart_mid->GetXaxis()->SetTitle("N_{part}");
   hNpart_mid->GetYaxis()->SetTitle("dN/dN_{part}");
   hNpart_mid->Draw();
   hNpartMB_mid->Draw("same");
   ymin = hNpart_mid->GetMinimum();
   ymax = 1.*hNpart_mid->GetMaximum();
   cout << ymin << " " << ymax << endl;
   // bin limits
   tl->SetLineStyle(2); tl->SetLineWidth(2);
   tl->DrawLine(findNpart(20),ymin,findNpart(20),ymax);
   tl->DrawLine(findNpart(40),ymin,findNpart(40),ymax);
   tl->DrawLine(findNpart(60),ymin,findNpart(60),ymax);
   tl->DrawLine(findNpart(80),ymin,findNpart(80),ymax);
   tl->DrawLine(findNpart(100),ymin,findNpart(100),ymax);
   // average centrality in each bin
   ta->SetLineColor(kBlack);
   x = getmeanB(hNpartMB_mid,180/nrebin+1,200/nrebin+1);
   ta->DrawArrow(x,0.3*ymax,x,ymin+0.0*(ymax-ymin));
   x = getmeanB(hNpartMB_mid,160/nrebin+1,180/nrebin+1);
   ta->DrawArrow(x,0.3*ymax,x,ymin+0.0*(ymax-ymin));
   x = getmeanB(hNpartMB_mid,140/nrebin+1,160/nrebin+1);
   ta->DrawArrow(x,0.3*ymax,x,ymin+0.0*(ymax-ymin));
   x = getmeanB(hNpartMB_mid,120/nrebin+1,140/nrebin+1);
   ta->DrawArrow(x,0.3*ymax,x,ymin+0.0*(ymax-ymin));
   x = getmeanB(hNpartMB_mid,100/nrebin+1,120/nrebin+1);
   ta->DrawArrow(x,0.3*ymax,x,ymin+0.0*(ymax-ymin));
   x = getmeanB(hNpartMB_mid,1,100/nrebin+1);
   ta->DrawArrow(x,0.3*ymax,x,ymin+0.0*(ymax-ymin));
   ta->SetLineColor(kRed);
   x = getmeanB(hNpart_mid,180/nrebin+1,200/nrebin+1);
   ta->DrawArrow(x,0.3*ymax,x,ymin+0.0*(ymax-ymin));
   x = getmeanB(hNpart_mid,160/nrebin+1,180/nrebin+1);
   ta->DrawArrow(x,0.3*ymax,x,ymin+0.0*(ymax-ymin));
   x = getmeanB(hNpart_mid,140/nrebin+1,160/nrebin+1);
   ta->DrawArrow(x,0.3*ymax,x,ymin+0.0*(ymax-ymin));
   x = getmeanB(hNpart_mid,120/nrebin+1,140/nrebin+1);
   ta->DrawArrow(x,0.3*ymax,x,ymin+0.0*(ymax-ymin));
   x = getmeanB(hNpart_mid,100/nrebin+1,120/nrebin+1);
   ta->DrawArrow(x,0.3*ymax,x,ymin+0.0*(ymax-ymin));
   x = getmeanB(hNpart_mid,1,100/nrebin+1);
   ta->DrawArrow(x,0.3*ymax,x,ymin+0.0*(ymax-ymin));

   tlx->DrawLatex(50,0.8*ymax,"PbPb data, |y|<1.6");
   tleg->SetY1NDC(0.42);
   tleg->SetY2NDC(0.57);
   tleg->Draw();
   c1->SaveAs("Npart_mid.root");

   // FWD
   hNpart_fwd->GetXaxis()->SetTitle("N_{part}");
   hNpart_fwd->GetYaxis()->SetTitle("dN/dN_{part}");
   hNpart_fwd->Draw();
   hNpartMB_fwd->Draw("same");
   ymin = hNpart_fwd->GetMinimum();
   ymax = 1.*hNpart_fwd->GetMaximum();
   cout << ymin << " " << ymax << endl;
   // bin limits
   tl->SetLineStyle(2); tl->SetLineWidth(2);
   tl->DrawLine(findNpart(40),ymin,findNpart(40),ymax);
   tl->DrawLine(findNpart(80),ymin,findNpart(80),ymax);
   // average centrality in each bin
   ta->SetLineColor(kBlack);
   x = getmeanB(hNpartMB_fwd,160/nrebin+1,200/nrebin+1);
   ta->DrawArrow(x,0.3*ymax,x,ymin+0.0*(ymax-ymin));
   x = getmeanB(hNpartMB_fwd,120/nrebin+1,160/nrebin+1);
   ta->DrawArrow(x,0.3*ymax,x,ymin+0.0*(ymax-ymin));
   x = getmeanB(hNpartMB_fwd,1,120/nrebin+1);
   ta->DrawArrow(x,0.3*ymax,x,ymin+0.0*(ymax-ymin));
   ta->SetLineColor(kRed);
   x = getmeanB(hNpart_fwd,160/nrebin+1,200/nrebin+1);
   ta->DrawArrow(x,0.3*ymax,x,ymin+0.0*(ymax-ymin));
   x = getmeanB(hNpart_fwd,120/nrebin+1,160/nrebin+1);
   ta->DrawArrow(x,0.3*ymax,x,ymin+0.0*(ymax-ymin));
   x = getmeanB(hNpart_fwd,1,120/nrebin+1);
   ta->DrawArrow(x,0.3*ymax,x,ymin+0.0*(ymax-ymin));

   tlx->DrawLatex(50,0.8*ymax,"PbPb data, 1.6<|y|<2.4");
   tleg->Draw();
   c1->SaveAs("Npart_fwd.root");
}

float getmean(TH1F* hist, double xmin, double xmax) {
   int imin = hist->FindBin(xmin);
   int imax = hist->FindBin(xmax);
   // cout << imin << " " << imax << endl;
   return getmeanB(hist, imin, imax);
}

float getmeanB(TH1F* hist, int imin, int imax) {
   double tot1=0, tot2=0;
   for (int i=imin; i<imax; i++) {
      tot1 += hist->GetBinContent(i)*hist->GetBinCenter(i)*hist->GetBinWidth(i);
      tot2 += hist->GetBinContent(i)*hist->GetBinWidth(i);
   }
   float ans = (tot2!=0) ? tot1/tot2 : 0;
   cout << imin << " " << imax << " " << ans << endl;;
   return ans;
}

TH1F* flatten(TH1F* input) {
   TH1F *ans = (TH1F*) input->Clone(Form("%s_flat",input->GetName()));
   double binc = ans->Integral()/ans->GetNbinsX();
   for (int i=1; i<ans->GetNbinsX()+1; i++) {
      ans->SetBinContent(i,binc);
      ans->SetBinError(i,sqrt(binc));
   }
   input->SetMarkerColor(kRed);
   input->SetLineColor(kRed);
   return ans;
}

void  diff(TH1F* input) {
   for (int i=1; i<input->GetNbinsX()+1; i++) {
      double binc = input->GetBinContent(i);
      double err = input->GetBinError(i);
      double width = input->GetBinWidth(i);;
      input->SetBinContent(i,binc/width);
      input->SetBinError(i,err/width);
   }
}
