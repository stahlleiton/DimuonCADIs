//Plotting Centrality Data for HIN-16-008
//Chad Flores
//cflores@ucdavis.edu
//xadflores@gmail.com
//Forgive the few comments, contact for help

#include "Style/tdrstyle.C"
#include "Style/CMS_lumi.C"


float RError(float A, float eA, float B, float eB);
float sumSqEr(float a_er, float b_er, float c_er,float d_er,float e_er);

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
static const int n = 1;

void plotCentTheory(){

setTDRStyle();
writeExtraText = false;

float cent_bins[nCentBin]={8.3,30.59,53.85,86.95,131.4,189.2,264.3,333.4,384.4};

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
doubleRatMB_er[0]= RError(f21[10],f21_er[10],f21[9],f21_er[9]);

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


//Global box error
float ppErrTot =  TMath::Sqrt(sysPP[0]*sysPP[0]+(f21_er[9]/f21[9])*(f21_er[9]/f21[9]));
//cout<<"pp stat "<< f21_er[9]/f21[9]<<"total" << ppErrTot<<endl;

TBox *ppBox = new TBox(411,1+ppErrTot,420,1-ppErrTot);
ppBox->SetLineWidth(1);
ppBox->SetLineColor(kAzure-2);
ppBox->SetFillColorAlpha(kAzure-4,0.3);

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


f1->Draw();

int   iPos =33;
int   iPeriod = 106;
TString label = "";
CMS_lumi(p1, iPeriod,iPos, label );


//-------------Rapp Model
const int pn = 29;
double rappBins[pn]={7.3,11.7,17.8,25.8,35.5,47.0,60.1,74.8,90.8,108.0,126.3,145.5,165.5,186.0,207.0,228.1,249.2,270.2,290.6,310.4,329.1,346.5,362.2,375.8,387.1,395.8,401.9,405.4,406.6};
double ymax[pn] = {0.9587425,0.9187990,0.8565655,0.7644562,0.6723351,0.5902134,0.5266496,0.4792459,0.4405638,0.4096609,0.3840947,0.3651010,0.3487012,0.3352589,0.3238887,0.3159977,0.3078883,0.3021869,0.2982383,0.2942326,0.2915874,0.2889154,0.2873280,0.2868202,0.2860695,0.2868717,0.2880443,0.2887858,0.2893011};
double ymin[pn] = {0.9576127,0.9155766,0.8501958,0.7559712,0.6647700,0.5870179,0.5245276,0.4720668,0.4285442,0.3933171,0.3635568,0.3413902,0.3220806,0.3061444,0.2932733,0.2831388,0.2735915,0.2661652,0.2600746,0.2548182,0.2508956,0.2467867,0.2439757,0.2421033,0.2413969,0.2411573,0.2412219,0.2413616,0.2413116};
double ypoint[pn];
double ylow[pn];
double yhigh[pn];

for(int p =0;p<pn;p++){
    ypoint[p]=(ymax[p]+ymin[p])*0.5; 
    yhigh[p]=TMath::Abs(ymax[p]-ypoint[p]);
    ylow[p]=TMath::Abs(ymin[p]-ypoint[p]);
    }

gRat = new TGraphAsymmErrors(pn,rappBins,ypoint,0,0,ylow,yhigh);
gRat->SetFillColor(TColor::GetColor("#48BF91"));
gRat->SetLineColor(TColor::GetColor("#48BF91"));
gRat->SetFillStyle(3004);
gRat->SetLineWidth(2);
gRat->Draw("3SAME");

gRatH = new TGraphErrors(pn,rappBins,ymax);
gRatH->SetLineWidth(2);
gRatH->SetLineColor(TColor::GetColor("#48BF91"));
gRatL = new TGraphErrors(pn,rappBins,ymin);
gRatL->SetLineWidth(2);
gRatL->SetLineColor(TColor::GetColor("#48BF91"));

gRatH->Draw("lsame");
gRatL->Draw("lsame");


//--------------Strickland Model
const int npt = 407;

TGraphErrors* g1S = new TGraphErrors("Theory_Data/Y1SNpart5023Xi0.tsv","%lg %lg %lg %lg","\t");
TGraphErrors* g2S = new TGraphErrors("Theory_Data/Y2SNpart5023Xi0.tsv","%lg %lg %lg %lg","\t");

int nPts = g1S->GetN();
//cout<<"Number of Points: "<<nPts<<endl;
double y1S_1[npt];
double y2S_1[npt];
double r2S1S_1[npt];

double y1S_2[npt];
double y2S_2[npt];
double r2S1S_2[npt];

double y1S_3[npt];
double y2S_3[npt];
double r2S1S_3[npt];


for(int i=0;i< nPts; i++){
y1S_1[i]=g1S->GetY()[i];
y2S_1[i]=g2S->GetY()[i];
r2S1S_1[i]=y2S_1[i]/y1S_1[i];

y1S_2[i]=g1S->GetErrorX(i);
y2S_2[i]=g2S->GetErrorX(i);
r2S1S_2[i]=y2S_2[i]/y1S_2[i];

y1S_3[i]=g1S->GetErrorY(i);
y2S_3[i]=g2S->GetErrorY(i);
r2S1S_3[i]=y2S_3[i]/y1S_3[i];

}


TGraph* g2t = new TGraph(g1S->GetN(),g1S->GetX(),r2S1S_1);
TGraph* g3t = new TGraph(g1S->GetN(),g1S->GetX(),r2S1S_2);
TGraph* g4t = new TGraph(g1S->GetN(),g1S->GetX(),r2S1S_3);
g2t->Draw("lsame");
g2t->SetLineWidth(2);
g2t->SetLineStyle(3);
//g2t->SetLineColor(kBlue);
g2t->SetLineColor(TColor::GetColor("#F37A48"));
g3t->Draw("SAMEl");
g3t->SetLineWidth(2);
g3t->SetLineStyle(1);
//g3t->SetLineColor(kBlue);
g3t->SetLineColor(TColor::GetColor("#F37A48"));
g4t->Draw("SAMEl");
g4t->SetLineWidth(2);
//g4t->SetLineColor(kBlue);
g4t->SetLineColor(TColor::GetColor("#F37A48"));
g4t->SetLineStyle(8);

TLatex *t = new TLatex();// t->SetTextSize(0.042);
t->SetTextSize(0.038);
t->SetTextFont(42);
t->DrawLatex(20,1.45,"p_{T}^{#mu#mu} < 30 GeV/c");
t->DrawLatex(20,1.33,"|y^{#mu#mu}| < 2.4");
t->DrawLatex(20,1.21,"p_{T}^{#mu} > 4 GeV/c");

gSys->Draw("same5");
g1->Draw("psamez");
ppBox->Draw("samel");
arrCent1-> SetLineWidth(1);
arrCent1->SetLineColor(kAzure-2);
arrCent1 -> Draw();

TArrow *arrLeg = new TArrow(233,.83,233,0.76,0.03,"|->");
arrLeg-> SetLineWidth(2);
arrLeg->SetLineColor(kAzure-2);
arrLeg-> SetLineWidth(1);
arrLeg->Draw();
t->SetTextSize(0.038);
t->DrawLatex(253,0.78,"95% CL");


leg = new TLegend(0.6,0.64,.9,0.82);
leg->SetTextFont(42);
leg->SetBorderSize(0);
leg->SetFillStyle(0);
leg->SetHeader("Krouppa and Strickland");
leg ->SetTextSize(0.038);
leg->AddEntry(g2t,"4#pi #eta/s = 1","l");
leg->AddEntry(g3t,"4#pi #eta/s = 2","l");
leg->AddEntry(g4t,"4#pi #eta/s = 3","l");
leg -> Draw();

leg2 = new TLegend(0.6,0.57,.9,0.62);
leg2->SetTextFont(42);
leg2->SetBorderSize(0);
leg2->SetFillStyle(0);
leg2 ->SetTextSize(0.038);
leg2->AddEntry(gRat,"Du et al.","F");
leg2 -> Draw();

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


//--------Graphing the MB Systematics
TGraphErrors *gSysMB = new TGraphErrors(1,dumX ,doubleRatMB,dumXMB ,sysPbPbPP);

gSysMB->SetFillColorAlpha(kAzure-4,0.3);
gSysMB->SetLineColor(kAzure-2);
gSysMB->SetLineWidth(1);
gSysMB->Draw("same5");

//--------Graphing the MB point and Stat error
TGraphErrors *g2 = new TGraphErrors(1,dumX , doubleRatMB,0 ,doubleRatMB_er);
g2->SetTitle("");
g2->SetMarkerStyle(20);
g2->SetMarkerColor(kAzure-2);
g2->SetLineColor(kAzure-2);
g2->SetMarkerSize(1);
g2->Draw("psame");

TLatex *t2 = new TLatex(); t2->SetTextSize(0.2);
t2->SetTextFont(42);
t2->SetTextAngle(90);
t2->DrawLatex(.55,0.45,"0-100%");

p2->Update();
c1->Update();

c1->SaveAs("DR21VsCent_Theory.pdf");

}
