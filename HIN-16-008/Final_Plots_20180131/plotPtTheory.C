//Plotting PT Data for HIN-16-008 
//Chad Flores 
//cflores@ucdavis.edu 
//xadflores@gmail.com 
//Forgive the few comments, contact for help


#include "Style/tdrstyle.C"
#include "Style/CMS_lumi.C"


float RError(float A, float eA, float B, float eB);
float sumSqEr(float a_er, float b_er,float c_er,float d_er);


//ratio error
float RError(float A, float eA, float B, float eB){
  float f=A/B;
  float fA=eA/A;
  float fB=eB/B;
  float eR=  f*sqrt( (fA*fA + fB*fB )) ;
  return eR;
  }

float sumSqEr(float a_er, float b_er, float c_er, float d_er){
float asq = (a_er*a_er);
float bsq = (b_er*b_er);
float csq = (c_er*c_er);
float dsq = (d_er*d_er);
float sq_er = TMath::Sqrt(asq + bsq + csq + dsq);
return sq_er;
}


static const int nFile = 6;
static const int nPtBin = 3;

void plotPtTheory(){

setTDRStyle();
writeExtraText=false;

float pt_bins[nPtBin]={2.5,8.5,21};
float pt_bins_er[nPtBin]={2.5,3.5,9};

char fileName[nFile][500];
float n1s[nFile];
float f21[nFile];
float f31[nFile];
float n1s_er[nFile];
float f21_er[nFile];
float f31_er[nFile];

float nBkg[nFile];
float doubleRat[nPtBin];
float doubleRat_er[nPtBin];

float sysSig[nPtBin]={.034,.0119,.01};
float sysBkg[nPtBin]={0.3848,0.1304,.0261};
float sysEff[nPtBin]={0.014,0.014,0.014};
float sysSigPdf[nPtBin]={0.0236,0.0193,0.0060};
float sysTot[nPtBin];

sprintf(fileName[0],"PT/FIT_PbPb_DATA_sig_2_Bkg_ErfExp1_262620_263757_0cent200_0p0pt5p0_0p0y2p4.root");
sprintf(fileName[1],"PT/FIT_PbPb_DATA_sig_2_Bkg_ErfExp6_262620_263757_0cent200_5p0pt12p0_0p0y2p4.root");
sprintf(fileName[2],"PT/FIT_PbPb_DATA_sig_2_Bkg_ErfExp6_262620_263757_0cent200_12p0pt30p0_0p0y2p4.root");

sprintf(fileName[3],"PT/FIT_PP_DATA_sig_2_Bkg_ErfExp1_262157_262328_-1cent1_0p0pt5p0_0p0y2p4.root");
sprintf(fileName[4],"PT/FIT_PP_DATA_sig_2_Bkg_ErfExp6_262157_262328_-1cent1_5p0pt12p0_0p0y2p4.root");
sprintf(fileName[5],"PT/FIT_PP_DATA_sig_2_Bkg_ErfExp6_262157_262328_-1cent1_12p0pt30p0_0p0y2p4.root");

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
	//cout<<"Bin: "<< i <<" N1S: "<<n1s[i]<<" N1S_er:"<<n1s_er[i]<<" f21: "<<f21[i]<<" f21_er: "<<f21_er[i]<<" f31: "<<f31[i]<<" f31_er: "<<f31_er[i]<<" nbkg: "<<nBkg[i]<<endl;

}


for(Int_t i=0;i<nPtBin;i++){
        
	doubleRat[i]= f21[i]/f21[i+3];
        doubleRat_er[i]= RError(f21[i],f21_er[i],f21[i+3],f21_er[i+3]);
	sysSig[i] = sysSig[i]*doubleRat[i];
	sysBkg[i] = sysBkg[i]*doubleRat[i];
	sysEff[i] = sysEff[i]*doubleRat[i];
	sysSigPdf[i] = sysSigPdf[i]*doubleRat[i];
	sysTot[i] = sumSqEr(sysSig[i],sysBkg[i],sysEff[i],sysSigPdf[i]); 
	cout<<"double Ratio: "<<doubleRat[i]<<" doubleRatio_Err: "<<doubleRat_er[i] <<" doubleRatio_Sys: "<<sysTot[i]<<endl;
}

TCanvas *c1 = new TCanvas("c1","Ups(2S) double ratio vs pt",600,600);

c1->SetBottomMargin(0.15);

TF1 *f1 = new TF1("f1","1",0,30);
f1->SetFillColor(19);
f1->SetFillStyle( 0 );
f1->SetMarkerStyle(20);
f1->SetMarkerSize(0.8);
f1->SetLineWidth(1);
f1->SetLineColor(1);
f1->SetLineStyle(2);
f1->GetYaxis()->SetTitle("(#varUpsilon(2S)/#varUpsilon(1S))_{PbPb}/(#varUpsilon(2S)/#varUpsilon(1S))_{pp}");
f1->GetXaxis()->SetTitle("p^{#mu#mu}_{T} (GeV/c)");
f1->GetXaxis()->SetTitleOffset(1.1);
f1->GetXaxis()->CenterTitle();
f1->GetYaxis()->SetRangeUser(0,1.6);
f1->GetXaxis()->SetRangeUser(0,30);
f1->Draw();

float xErr[nPtBin]={2.5,3.5,9};
TGraphErrors *gSys = new TGraphErrors(nPtBin,pt_bins , doubleRat,xErr ,sysTot);
gSys->SetLineColor(kAzure-2);
gSys->SetLineWidth(1);
gSys->SetFillColorAlpha(kAzure-4,0.3);


TGraphErrors *g1 = new TGraphErrors(nPtBin,pt_bins , doubleRat,pt_bins_er ,doubleRat_er);
g1->SetTitle("");
g1->SetMarkerStyle(20);
g1->SetMarkerColor(kAzure-2);
g1->SetLineColor(kAzure-2);
g1->SetLineWidth(1);
g1->SetMarkerSize(1);

const int npt = 3;

float x[npt] = {2.5,3.5,9};

//----Rapp Model
float ymax[npt] = {0.283415903,0.305316539,0.391296719};
float ymin[npt] = {0.26283084,0.287840102,0.360306655};

float ypoint[npt];
float ylow[npt];
float yhigh[npt];

for(int p =0;p<npt;p++){
    ypoint[p]=(ymax[p]+ymin[p])*0.5;
    yhigh[p]=TMath::Abs(ymax[p]-ypoint[p]);
    ylow[p]=TMath::Abs(ymin[p]-ypoint[p]);
    }

gRat = new TGraphAsymmErrors(npt,pt_bins,ypoint,x,x,ylow,yhigh);
gRat->SetFillColor(TColor::GetColor("#48BF91"));
gRat->SetLineColor(TColor::GetColor("#48BF91"));
gRat->SetFillStyle(3004);
gRat->SetLineWidth(2);
gRat->Draw("2SAME");

gRatH = new TGraphErrors(npt,pt_bins,ymax,pt_bins_er);
gRatH->SetLineWidth(2);
gRatH->SetLineColor(TColor::GetColor("#48BF91"));
gRatH->SetMarkerSize(0);
gRatL = new TGraphErrors(npt,pt_bins,ymin,pt_bins_er);
gRatL->SetLineWidth(2);
gRatL->SetMarkerSize(0);
gRatL->SetLineColor(TColor::GetColor("#48BF91"));

gRatH->Draw("psame");
gRatL->Draw("psame");


//----Strickland Model
TGraphErrors* g1S = new TGraphErrors("Theory_Data/CMSratioRunTwopT.tsv","%lg %lg %lg %lg","\t");

int nPts = g1S->GetN();
//cout<<"Number of Points: "<<nPts<<endl;
float r2S1S_1[npt];
float r2S1S_2[npt];
float r2S1S_3[npt];

for(int i=0;i< nPts; i++){
r2S1S_1[i]=g1S->GetY()[i];
r2S1S_2[i]=g1S->GetErrorX(i);
r2S1S_3[i]=g1S->GetErrorY(i);
}


TGraphErrors* g2t = new TGraphErrors(npt,pt_bins,r2S1S_1,x,0);
TGraphErrors* g3t = new TGraphErrors(npt,pt_bins,r2S1S_2,x,0);
TGraphErrors* g4t = new TGraphErrors(npt,pt_bins,r2S1S_3,x,0);


g2t->Draw("samep");
g2t->SetLineWidth(2);
g2t->SetLineStyle(3);
g2t->SetLineColor(TColor::GetColor("#F37A48"));
g2t->SetMarkerSize(0);
g3t->Draw("SAMEp");
g3t->SetLineWidth(2);
g3t->SetLineStyle(1);
g3t->SetLineColor(TColor::GetColor("#F37A48"));
g3t->SetMarkerSize(0);
g4t->Draw("SAMEp");
g4t->SetLineWidth(2);
g4t->SetLineColor(TColor::GetColor("#F37A48"));
g4t->SetLineStyle(8);

g4t->SetMarkerSize(0);

gSys->Draw("same5");
g1->Draw("samep");



leg = new TLegend(0.55,0.64,.9,0.82);
leg->SetTextFont(42);
leg->SetBorderSize(0);
leg->SetFillStyle(0);
leg->SetHeader("Krouppa and Strickland");
leg ->SetTextSize(0.038);
leg->AddEntry(g2t,"4#pi #eta/s = 1","l");
leg->AddEntry(g3t,"4#pi #eta/s = 2","l");
leg->AddEntry(g4t,"4#pi #eta/s = 3","l");
leg -> Draw();

leg2 = new TLegend(0.55,0.57,.9,0.62);
leg2->SetTextFont(42);
leg2->SetBorderSize(0);
leg2->SetFillStyle(0);
leg2 ->SetTextSize(0.038);
leg2->AddEntry(gRat,"Du et al.","F");
leg2 -> Draw();



int   iPos =33;
int   iPeriod = 107;
TString label = "";
CMS_lumi(c1, iPeriod,iPos, label );

TLatex *t = new TLatex(); 
t->SetTextSize(0.038);
t->SetTextFont(42);
t->DrawLatex(2,1.45,"|y^{#mu#mu}| < 2.4");
t->DrawLatex(2,1.33,"p_{T}^{#mu} > 4 GeV/c");
t->DrawLatex(2,1.21,"Centrality 0-100%");

c1->Update();

c1->SaveAs("DR21VsPt_Theory.pdf");


}
