//Plotting 3S Centrality Data for HIN-16-008  
//Chad Flores  
//cflores@ucdavis.edu  
//xadflores@gmail.com  
//Forgive the few comments, contact for help


#include "Style/tdrstyle.C"
#include "Style/CMS_lumi.C"

static const int nCentBin = 4;
static const int nFile = 3;


void plotUL(){

setTDRStyle();
writeExtraText = false;


float cent_bins[nCentBin]={21.9,109.17,226.75,358.8};
float cent_bins_pts[2]={109.17,358.8};

float UpperLim[nCentBin]={0.56,0.56,0.17,0.66};
float LowerLim[nCentBin]={0.0,0.0,0.0,0.0};
float UpperLim65[nCentBin]={0.36,0.40,0.057,0.47};
float LowerLim65[nCentBin]={0.0,0.087,0.0,0.079};

float IntUpLim = 0.26;
float IntLowLim = 0.0;
float IntUpLim65 = 0.16;
float IntLowLim65 = 0.0;

double xfrac = 0.85;


TCanvas *c1 = new TCanvas("c1","",600/xfrac,600);

TArrow *arrCent1 = new TArrow(cent_bins[0],UpperLim[0],cent_bins[0],LowerLim[0],0.04,"|->");
TArrow *arrCent2 = new TArrow(cent_bins[1],UpperLim[1],cent_bins[1],LowerLim[1],0.04,"|->");
TArrow *arrCent3 = new TArrow(cent_bins[2],UpperLim[2],cent_bins[2],LowerLim[2],0.04,"|->");
TArrow *arrCent4 = new TArrow(cent_bins[3],UpperLim[3],cent_bins[3],LowerLim[3],0.04,"|->");

TBox *boxCent1 = new TBox(cent_bins[0]-5,UpperLim65[0],cent_bins[0]+5,LowerLim65[0]);
TBox *boxCent2 = new TBox(cent_bins[1]-5,UpperLim65[1],cent_bins[1]+5,LowerLim65[1]);
TBox *boxCent3 = new TBox(cent_bins[2]-5,UpperLim65[2],cent_bins[2]+5,LowerLim65[2]);
TBox *boxCent4 = new TBox(cent_bins[3]-5,UpperLim65[3],cent_bins[3]+5,LowerLim65[3]);
 
TPad *p1 = new TPad("p1","p1",0.0,0.0,xfrac,1.0);
p1->SetLeftMargin(gStyle->GetPadLeftMargin()/xfrac);
p1->SetRightMargin(0);
p1->SetFrameBorderMode(0);
p1->SetBottomMargin(0.15);
p1->SetBorderMode(0);
p1->SetBorderSize(0);
p1->Draw();

TPad *p2 = new TPad("p2","p2",xfrac,0,1,1);
p2->SetRightMargin(0.02);
p2->SetRightMargin(gStyle->GetPadRightMargin()/(1.-xfrac));
p2->SetLeftMargin(0);
p2->SetFrameBorderMode(0);
p2->SetBottomMargin(0.15);
p2->SetBorderMode(0);
p2->SetBorderSize(0);
p2->Draw();

p1->cd();

TF1 *f1 = new TF1("f1","1",0,420);

f1->SetFillColor(19);
f1->SetFillStyle( 0 );
f1->SetMarkerStyle(20);
f1->SetMarkerSize(0.8);
f1->SetLineWidth(1);
f1->SetLineColor(1);
f1->SetLineStyle(2);
f1->GetYaxis()->SetRangeUser(0.0,1.6);
f1->GetXaxis()->SetRangeUser(0.0,420);
f1->GetXaxis()->SetTitle("N_{part}");
f1->GetXaxis()->SetTitleOffset(1.1);
f1->GetXaxis()->CenterTitle(true);
f1->GetYaxis()->SetTitle("(#varUpsilon(3S)/#varUpsilon(1S))_{PbPb}/(#varUpsilon(3S)/#varUpsilon(1S))_{pp}");
f1->Draw();

arrCent1-> SetLineWidth(2);
arrCent1->SetLineColor(kAzure-4);
arrCent2-> SetLineWidth(2);
arrCent2->SetLineColor(kAzure-4);
arrCent3-> SetLineWidth(2);
arrCent3->SetLineColor(kAzure-4);
arrCent4-> SetLineWidth(2);
arrCent4->SetLineColor(kAzure-4);


boxCent1->SetFillColorAlpha(kRed,0.3);
boxCent2->SetFillColorAlpha(kRed,0.3);
boxCent3->SetFillColorAlpha(kRed,0.3);
boxCent4->SetFillColorAlpha(kRed,0.3);


boxCent1 -> Draw("5");
boxCent2 -> Draw();
boxCent3 -> Draw();
boxCent4 -> Draw();

arrCent1 -> Draw();
arrCent2 -> Draw();
arrCent3 -> Draw();
arrCent4 -> Draw();

TBox *boxLeg = new TBox(310,1.1,320,1.03);
TArrow *arrLeg = new TArrow(315,1.2,315,1.13,0.03,"|->");

arrLeg-> SetLineWidth(2);
arrLeg->SetLineColor(kAzure-4);
arrLeg-> SetLineWidth(2);

boxLeg->SetFillColorAlpha(kRed,0.3);
boxLeg -> Draw();
arrLeg->Draw();

int   iPos =33;
int   iPeriod = 106;
TString label = "";
CMS_lumi(p1, iPeriod,iPos, label );


TLatex *t = new TLatex(); 
t->SetTextSize(0.038);
t->SetTextFont(42);
t->DrawLatex(20,1.45,"p_{T}^{#mu#mu} < 30 GeV/c");
t->DrawLatex(20,1.33,"|y^{#mu#mu}| < 2.4");
t->DrawLatex(20,1.21,"p_{T}^{#mu} > 4 GeV/c");
t->SetTextSize(0.035);
t->DrawLatex(12,.65,"50-100%");
t->DrawLatex(100,.65,"30-50%");
t->DrawLatex(210,.45,"10-30%");
t->DrawLatex(350,.70,"0-10%");


t->SetTextSize(0.038);
t->DrawLatex(335,1.05,"68% CL");
t->DrawLatex(335,1.15,"95% CL");

p1->Update();
p2->cd();

float dumX=0.5;
  

TBox *boxInt = new TBox(dumX-0.08,IntUpLim65,dumX+.08,IntLowLim65);

gPad->SetTicks(1,1);

TF1 *f2 = new TF1("f1","1",0,1);
f2->Draw("axis");
f2->GetXaxis()->SetTickLength(0);
f2->GetYaxis()->SetTickLength(0.13);
f2->GetYaxis()->SetRangeUser(0,1.6);
f2->GetXaxis()->SetLabelSize(0);

TArrow *arrInt = new TArrow(dumX,IntUpLim,dumX,IntLowLim,0.04,"|->");
arrInt-> SetLineWidth(2);
arrInt-> SetLineColor(kAzure-4);

boxInt->SetFillColorAlpha(kRed,0.3);
boxInt -> Draw();

arrInt-> Draw();

t->SetTextSize(.2);
t->SetTextAngle(90);
t->DrawLatex(0.55,.5,"0-100%");

p2->Update();
c1->Update();

c1->SaveAs("DR31VsCent.pdf");

}
