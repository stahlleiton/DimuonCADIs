//Plotting Centrality Data for HIN-16-008 and for HIN-11-011
////Chad Flores
////cflores@ucdavis.edu
////xadflores@gmail.com
////Forgive the few comments, contact for help

#include "Style/tdrstyle.C"
#include "Style/CMS_lumi.C"



float RError(float A, float eA, float B, float eB);
float sumSqEr(float a_er, float b_er, float c_er,float d_er,float e_er);

//ratio error
float RError(float A, float eA, float B, float eB){
  float f=A/B;
  float fA=eA/A;
  float fB=eB/B;
  float eR=  f*sqrt( (fA*fA + fB*fB )) ;
  return eR;
  }

float sumSqEr(float a_er, float b_er,float c_er,float d_er,float e_er){
float asq = (a_er*a_er);
float bsq = (b_er*b_er);
float csq = (c_er*c_er);
float dsq = (d_er*d_er);
float esq = (e_er*e_er);
float sq_er = TMath::Sqrt(asq + bsq + csq +dsq + esq);
return sq_er;
}



static const int nFile = 11;
static const int nCentBin = 9;
static const int nCentOld = 7;
static const int n = 1;

void plotCentComp(){

setTDRStyle();
writeExtraText = false;

float cent_bins[nCentBin]={8.3,30.59,53.85,86.95,131.4,189.2,264.3,333.4,384.4};
float cent_oldBins[nCentOld]={22,86,130,187,261,329,381};

float doubleRat11[nCentOld] = {0.304,0.420,0.344,0.428,0.138,0.102,0.270};
float doubleRat11_er[nCentOld] = {0.154,0.220,0.138,0.124,0.108,0.189,0.146};
float doubleRat11_sys[nCentOld] = {0.040,0.048,0.027,0.039,0.041,0.042,0.031};

float doubleRat11MB[1]={0.21};
float doubleRat11MB_er[1]={0.07};
float doubleRat11MB_sys[1]={0.02};




char fileName[nFile][500];
float n1s[nFile];
float f21[nFile];
float f31[nFile];
float n1s_er[nFile];
float f21_er[nFile];
float f31_er[nFile];

float nBkg[nFile];

float sysSig[nCentBin] = {.0278,.01,.0118,.0267,.0323,.0121,.0382,.0405,.0694};
float sysBkg[nCentBin] = {.1493,.0445,.0828,.0554,.0524,.04045,.0565,.0509,.2653};
float sysSigPdf[nCentBin] = {.0214,.0187,.0205,.0158,.0171,.0179,.0223,.0128,.0448};
float sysBkgVar[nCentBin] = {.0844,.1117,.0085,.0133,.0244,.0099,.0192,.0047,.0040};

float sysTot[nCentBin];

float sysSigPbPb[n]={0.0222};
float sysSigPP[n]={0.0038};
float sysSigPdfPP[n]={0.0097};
float sysBkgVarPP[n]={0.0001};
float sysBkgPP[n] = {0.0205};
float sysBkgPbPb[n] = {0.0465};
float sysEffPbPb[n] = {0.0};
float sysSigPdfPbPb[n] = {0.0184};
float sysBkgVarPbPb[n] = {0.0012};
float sysEffPP[n] = {0.014};
float sysPbPb[n];
float sysPbPbPP[n];
float sysPP[n];

float doubleRat[nCentBin];
float doubleRatMB[n];
float doubleRat_er[nCentBin];
float doubleRatMB_er[n];

sprintf(fileName[8],"Cent/FIT_PbPb_DATA_sig_2_Bkg_ErfExp1_262620_263757_0cent10_0p0pt30p0_0p0y2p4.root");
sprintf(fileName[7],"Cent/FIT_PbPb_DATA_sig_2_Bkg_ErfExp1_262620_263757_10cent20_0p0pt30p0_0p0y2p4.root");
sprintf(fileName[6],"Cent/FIT_PbPb_DATA_sig_2_Bkg_ErfExp1_262620_263757_20cent40_0p0pt30p0_0p0y2p4.root");
sprintf(fileName[5],"Cent/FIT_PbPb_DATA_sig_2_Bkg_ErfExp1_262620_263757_40cent60_0p0pt30p0_0p0y2p4.root");
sprintf(fileName[4],"Cent/FIT_PbPb_DATA_sig_2_Bkg_ErfExp1_262620_263757_60cent80_0p0pt30p0_0p0y2p4.root");
sprintf(fileName[3],"Cent/FIT_PbPb_DATA_sig_2_Bkg_ErfExp1_262620_263757_80cent100_0p0pt30p0_0p0y2p4.root");
sprintf(fileName[2],"Cent/FIT_PbPb_DATA_sig_2_Bkg_ErfExp1_262620_263757_100cent120_0p0pt30p0_0p0y2p4.root");
sprintf(fileName[1],"Cent/FIT_PbPb_DATA_sig_2_Bkg_ErfExp1_262620_263757_120cent140_0p0pt30p0_0p0y2p4.root");
sprintf(fileName[0],"Cent/FIT_PbPb_DATA_sig_2_Bkg_ErfExp1_262620_263757_140cent200_0p0pt30p0_0p0y2p4.root");

sprintf(fileName[9],"Integrated/FIT_PP_DATA_sig_2_Bkg_ErfExp1_262157_262328_-1cent1_0p0pt30p0_0p0y2p4.root");
sprintf(fileName[10],"Integrated/FIT_PbPb_DATA_sig_2_Bkg_ErfExp1_262620_263757_0cent200_0p0pt30p0_0p0y2p4.root");

RooWorkspace *myws;
TFile *inFile;

for(int i=0;i<nFile;i++){

	inFile = new TFile(fileName[i],"R");	
	myws = (RooWorkspace*)inFile->Get("myws");
	
	RooRealVar *nsig1f = (RooRealVar*)myws->var("N_{#varUpsilon(1S)}");
	RooRealVar *f2Sv1S = (RooRealVar*)myws->var("R_{#frac{2S}{1S}}");
	RooRealVar *f3Sv1S = (RooRealVar*)myws->var("R_{#frac{3S}{1S}}");
	RooRealVar *nbkg = (RooRealVar*)myws->var("n_{Bkgd}");
	
	n1s[i]= nsig1f->getVal();
	f21[i]= f2Sv1S->getVal();
	f31[i]= f3Sv1S->getVal();
	n1s_er[i]= nsig1f->getError();
	f21_er[i]= f2Sv1S->getError();
	f31_er[i]= f3Sv1S->getError();
	
	nBkg[i]= nbkg->getVal();
	
	//cout<<"Cent Bin: "<< i <<" N1S: "<<n1s[i]<<" N1S_er:"<<n1s_er[i]<<" f21: "<<f21[i]<<" f21_er:"<<f21_er[i]<<" f31: "<<f31[i]<<" f31_er:"<<f31_er[i]<<" nbkg:"<<nBkg[i]<<endl;
}

for(Int_t i=0;i<nCentBin;i++){

        doubleRat[i] = f21[i]/f21[9];
        doubleRat_er[i]= RError(f21[i],f21_er[i],f21[9],0);
        sysSig[i] = sysSig[i]*doubleRat[i];
        sysBkg[i] = sysBkg[i]*doubleRat[i];
        sysSigPdf[i] = sysSigPdf[i]*doubleRat[i];
        sysBkgVar[i] = sysBkgVar[i]*doubleRat[i];
        sysTot[i] = sumSqEr(sysSig[i],sysBkg[i],sysSigPdf[i],sysBkgVar[i],0);
     
	cout<<"double Ratio: "<<doubleRat[i]<<" doubleRatio_Err: "<<doubleRat_er[i] <<" doubleRatio_Sys: "<<sysTot[i]<<endl;
}

doubleRatMB[0]= f21[10]/f21[9];
doubleRatMB_er[0]= RError(f21[10],f21_er[10],f21[9],0);

sysSigPbPb[0] = doubleRatMB[0]*sysSigPbPb[0];
sysBkgPbPb[0]= doubleRatMB[0]*sysBkgPbPb[0];
sysEffPbPb[0]= doubleRatMB[0]*sysEffPbPb[0];
sysSigPdfPbPb[0]= doubleRatMB[0]*sysSigPdfPbPb[0];
sysBkgVarPbPb[0]= doubleRatMB[0]*sysBkgVarPbPb[0];

sysPbPb[0] = sumSqEr(sysSigPbPb[0],sysBkgPbPb[0],sysEffPbPb[0],sysSigPdfPbPb[0],sysBkgVarPbPb[0]);
sysPP[0] = sumSqEr(sysSigPP[0],sysBkgPP[0],sysEffPP[0],sysSigPdfPP[0],sysBkgVarPP[0]);

//Total MB Systematic
sysPbPbPP[0] = sumSqEr(doubleRatMB[0]*sysPP[0],sysPbPb[0],0,0,0);

cout<<"doubleRat MB: "<<doubleRatMB[0]<<" doubleRat MB Err: "<<doubleRatMB_er[0]<<" doubleRat MB Sys (PP and PbPb): "<< sysPbPbPP[0] <<endl;



//Plotting
double xfrac = 0.85;
TCanvas *c1 = new TCanvas("c1","Upsilon double ratio vs Npart",600/xfrac,600);
  



TPad *p1 = new TPad("p1","p1",0.0,0.0,xfrac,1.0);
p1->SetLeftMargin(gStyle->GetPadLeftMargin()/xfrac);
p1->SetRightMargin(0);
p1->SetFrameBorderMode(0);
p1->SetBorderMode(0);
p1->SetBottomMargin(0.15);
p1->SetBorderSize(0);
p1->Draw();


TPad *p2 = new TPad("p2","p2",xfrac,0,1,1);
p2->SetRightMargin(gStyle->GetPadRightMargin()/(1.-xfrac));
p2->SetLeftMargin(0);
p2->SetFrameBorderMode(0);
p2->SetBorderMode(0);
p2->SetBottomMargin(0.15);
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
f1->GetYaxis()->SetTitle("(#varUpsilon(2S)/#varUpsilon(1S))_{PbPb}/(#varUpsilon(2S)/#varUpsilon(1S))_{pp}");
f1->Draw();


TArrow *arrCent1 = new TArrow(cent_bins[8],0.36,cent_bins[8],0,0.04,"|->");

TBox *ulBox = new TBox(cent_bins[8]-5,0.23,cent_bins[8]+5,0.0);
ulBox->SetFillColor(kRed);
ulBox->SetFillStyle(3001);

cent_bins[8]=1000;
float ppErrTot =  TMath::Sqrt(sysPP[0]*sysPP[0]+(f21_er[9]/f21[9])*(f21_er[9]/f21[9]));
//cout<<"pp stat "<< f21_er[9]/f21[9]<<"total" << ppErrTot<<endl;

TBox *ppBox = new TBox(403,1+ppErrTot,411,1-ppErrTot);
ppBox->SetLineWidth(1);
ppBox->SetLineColor(kAzure-2);
ppBox->SetFillColorAlpha(kAzure-4,0.3);

TBox *ppBox11 = new TBox(411,1+0.12,420,1-0.12);
ppBox11->SetLineWidth(1);
ppBox11->SetLineColor(kRed-2);
ppBox11->SetFillColorAlpha(kRed-4,0.3);



TGraphErrors *g1 = new TGraphErrors(nCentBin,cent_bins , doubleRat,0 ,doubleRat_er);
g1->SetTitle("");
g1->SetMarkerStyle(20);
g1->SetLineColor(kAzure-2);
g1->SetMarkerColor(kAzure-2);
g1->SetMarkerSize(1);

float dumXer[nCentBin]={4,4,4,4,4,4,4,4,4};
TGraphErrors *gSys = new TGraphErrors(nCentBin,cent_bins ,doubleRat,dumXer ,sysTot);
gSys->SetLineColor(kAzure-2);
gSys->SetFillColorAlpha(kAzure-4,0.3);
gSys->SetLineWidth(1);


TGraphErrors *g1_11 = new TGraphErrors(7,cent_oldBins , doubleRat11,0 ,doubleRat11_er);
g1_11->SetTitle("");
g1_11->SetMarkerStyle(20);
g1_11->SetLineColor(kRed-2);
g1_11->SetMarkerColor(kRed-2);
g1_11->SetMarkerSize(1);

float dumXer_11[nCentBin]={4,4,4,4,4,4,4};
TGraphErrors *gSys_11 = new TGraphErrors(7,cent_oldBins ,doubleRat11,dumXer_11 ,doubleRat11_sys);
gSys_11->SetLineColor(kRed-2);
gSys_11->SetFillColorAlpha(kRed-4,0.3);
gSys_11->SetLineWidth(1);



f1->Draw();
gSys->Draw("same5");
g1->Draw("psamez");
gSys_11->Draw("same5");
g1_11->Draw("psamez");

ppBox->Draw("samel");
ppBox11->Draw("samel");
arrCent1-> SetLineWidth(1);
arrCent1->SetLineColor(kAzure-2);
//ulBox->Draw();
arrCent1 -> Draw();
int   iPos =33;
int   iPeriod = 106;
TString label = "";
CMS_lumi(p1, iPeriod,iPos, label );


TLatex *t = new TLatex();// t->SetTextSize(0.042);
t->SetTextSize(0.038);
t->SetTextFont(42);
t->DrawLatex(20,1.45,"p_{T}^{#mu#mu} < 30 GeV/c");
t->DrawLatex(20,1.33,"|y^{#mu#mu}| < 2.4");
t->DrawLatex(20,1.21,"p_{T}^{#mu} > 4 GeV/c");

TArrow *arrLeg = new TArrow(255,.95,255,0.88,0.03,"|->");
arrLeg-> SetLineWidth(2);
arrLeg->SetLineColor(kAzure-2);
arrLeg-> SetLineWidth(1);
arrLeg->Draw();
t->SetTextSize(0.038);
t->DrawLatex(275,0.9,"95% CL");



leg2 = new TLegend(0.65,0.77,.9,0.82);
leg2->SetTextFont(42);
leg2->SetBorderSize(0);
leg2->SetFillStyle(0);
leg2 ->SetTextSize(0.038);
leg2->AddEntry(g1_11,"#sqrt{s_{NN}} = 2.76 TeV","pe");
leg2 -> Draw();


t->DrawLatex(213,1.21,"PRL 109 (2012) 222301");


legc = new TLegend(0.65,0.65,.9,0.7);
legc->SetTextFont(42);
legc->SetBorderSize(0);
legc->SetFillStyle(0);
legc ->SetTextSize(0.038);
legc->AddEntry(g1,"#sqrt{s_{NN}} = 5.02 TeV","pe");
legc -> Draw();


p1->Update();
c1->Update();

p2->cd();
gPad->SetTicks(1,1);
float dumX[1]={.5};
TF1 *f2 = new TF1("f1","1",0,1);
f2->Draw("axis");
f2->GetXaxis()->SetTickLength(0);
f2->GetYaxis()->SetTickLength(0.15);
f2 -> GetYaxis()->SetRangeUser(0,1.6);
f2 -> GetXaxis()->SetRangeUser(0,1);
f2->GetXaxis()->SetLabelSize(0);

float dumXMB[n]={0.05};

TGraphErrors *gSysMB = new TGraphErrors(1,dumX ,doubleRatMB,dumXMB ,sysPbPbPP);

gSysMB->SetFillColorAlpha(kAzure-4,0.3);
gSysMB->SetLineColor(kAzure-2);
gSysMB->SetLineWidth(1);
gSysMB->Draw("same5");

TGraphErrors *g2 = new TGraphErrors(1,dumX , doubleRatMB,0 ,doubleRatMB_er);
g2->SetTitle("");
g2->SetMarkerStyle(20);
g2->SetMarkerColor(kAzure-2);
g2->SetLineColor(kAzure-2);
g2->SetMarkerSize(1);
g2->Draw("psame");

TGraphErrors *gSysMB11 = new TGraphErrors(1,dumX ,doubleRat11MB,dumXMB ,doubleRat11MB_sys);

gSysMB11->SetFillColorAlpha(kRed-4,0.3);
gSysMB11->SetLineColor(kRed-2);
gSysMB11->SetLineWidth(1);
gSysMB11->Draw("same5");

TGraphErrors *g211 = new TGraphErrors(1,dumX , doubleRat11MB,0 ,doubleRat11MB_er);
g211->SetTitle("");
g211->SetMarkerStyle(20);
g211->SetMarkerColor(kRed-2);
g211->SetLineColor(kRed-2);
g211->SetMarkerSize(1);
g211->Draw("psame");


TLatex *t2 = new TLatex(); t2->SetTextSize(0.2);
t2->SetTextFont(42);
t2->SetTextAngle(90);
t2->DrawLatex(.55,0.45,"0-100%");

p2->Update();
c1->Update();

c1->SaveAs("DR21VsCent_Comp11011.pdf");


}
