//Plotting Y Data for HIN-16-008  
//Chad Flores  
//cflores@ucdavis.edu  
//xadflores@gmail.com  
//Forgive the few comments, contact for help

#include "Style/tdrstyle.C"
#include "Style/CMS_lumi.C"



float RError(float A, float eA, float B, float eB);
float sumSqEr(float a_er, float b_er, float c_er,float d_er);

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


static const int nFile = 4;
static const int nRapBin = 2;

void plotRapTheory(){

setTDRStyle();
writeExtraText = false;

float pt_bins[nRapBin]={0.6,1.8};
float pt_bins_er[nRapBin]={0.6,0.6};

char fileName[nFile][500];
float n1s[nFile];
float f21[nFile];
float f31[nFile];
float n1s_er[nFile];
float f21_er[nFile];
float f31_er[nFile];

float sysSig[nRapBin]={.0241,.0344};
float sysBkg[nRapBin]={.0289,.0337};
float sysEff[nRapBin]={0.014,0.014};
float sysSigPdf[nRapBin]={0.0022,0.0448};
float sysTot[nRapBin];

float nBkg[nFile];

float doubleRat[nRapBin];
float doubleRat_er[nRapBin];
sprintf(fileName[0],"Rap/FIT_PbPb_DATA_sig_2_Bkg_ErfExp1_262620_263757_0cent200_0p0pt30p0_0p0y1p2.root");
sprintf(fileName[1],"Rap/FIT_PbPb_DATA_sig_2_Bkg_ErfExp1_262620_263757_0cent200_0p0pt30p0_1p2y2p4.root");
sprintf(fileName[2],"Rap/FIT_PP_DATA_sig_2_Bkg_ErfExp1_262157_262328_-1cent1_0p0pt30p0_0p0y1p2.root");
sprintf(fileName[3],"Rap/FIT_PP_DATA_sig_2_Bkg_ErfExp1_262157_262328_-1cent1_0p0pt30p0_1p2y2p4.root");

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
	
	//cout<<"Bin: "<< i <<" N1S: "<<n1s[i]<<" N1S_er:"<<n1s_er[i]<<" f21: "<<f21[i]<<" f21_er:"<<f21_er[i]<<" f31: "<<f31[i]<<" f31_er:"<<f31_er[i]<<" nbkg: "<<nBkg[i]<<endl;

}


for(Int_t i=0;i<nRapBin;i++){
  
        doubleRat[i]= f21[i]/f21[i+2];
        doubleRat_er[i]= RError(f21[i],f21_er[i],f21[i+2],f21_er[i+2]);
        sysSig[i] = sysSig[i]*doubleRat[i];
        sysBkg[i] = sysBkg[i]*doubleRat[i];
        sysEff[i] = sysEff[i]*doubleRat[i];
        sysSigPdf[i] = sysSigPdf[i]*doubleRat[i];
	sysTot[i] = sumSqEr(sysSig[i],sysBkg[i],sysEff[i],sysSigPdf[i]);
	cout<<"double Ratio: "<<doubleRat[i]<<" doubleRatio_Err: "<<doubleRat_er[i] <<" doubleRatio_Sys: "<<sysTot[i]<<endl;
}

TCanvas *c1 = new TCanvas("c1","A Simple Graph with error bars",600,600);
c1->SetBottomMargin(0.15);
TF1 *f1 = new TF1("f1","1",0,2.40);
f1->SetFillColor(19);
f1->SetFillStyle( 0 );
f1->SetMarkerStyle(20);
f1->SetMarkerSize(0.8);
f1->SetLineWidth(1);
f1->SetLineColor(1);
f1->SetLineStyle(2);
f1->GetXaxis()->SetTitle("|y^{#mu#mu}|");
f1->GetYaxis()->SetTitle("(#varUpsilon(2S)/#varUpsilon(1S))_{PbPb}/(#varUpsilon(2S)/#varUpsilon(1S))_{pp}");
f1->GetXaxis()->SetTitleOffset(1.1);
f1->GetXaxis()->CenterTitle();
f1->GetYaxis()->SetRangeUser(0,1.6);
f1->GetXaxis()->SetRangeUser(0,2.40);
f1->GetXaxis()->SetNdivisions(511);
f1->Draw();

float xErr[nRapBin]={.6,.6};

TGraphErrors *gSys = new TGraphErrors(nRapBin,pt_bins , doubleRat,xErr ,sysTot);
gSys->SetFillColorAlpha(kAzure-4,0.3);
gSys->SetLineColor(kAzure-2);

TGraphErrors *g1 = new TGraphErrors(nRapBin,pt_bins , doubleRat,0 ,doubleRat_er);
g1->SetTitle("");
g1->SetMarkerStyle(20);
g1->SetMarkerColor(kAzure-2);
g1->SetLineColor(kAzure-2);
g1->SetMarkerSize(1);


const int npt = 2;

//--Strickland Model
TGraphErrors* g1S = new TGraphErrors("Theory_Data/CMSratioRunTwoRap.tsv","%lg %lg %lg %lg","\t");

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


TGraphErrors* g2t = new TGraphErrors(npt,pt_bins,r2S1S_1,pt_bins_er,0);
TGraphErrors* g3t = new TGraphErrors(npt,pt_bins,r2S1S_2,pt_bins_er,0);
TGraphErrors* g4t = new TGraphErrors(npt,pt_bins,r2S1S_3,pt_bins_er,0);




g2t->Draw("psame");
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


int   iPos =33;
int   iPeriod = 107;
TString label = "";
CMS_lumi(c1, iPeriod,iPos, label );

TLatex *t = new TLatex(); 
t->SetTextSize(0.038);
t->SetTextFont(42);
t->DrawLatex(0.2,1.45,"p_{T}^{#mu#mu} < 30 GeV/c");
t->DrawLatex(0.2,1.33,"p_{T}^{#mu} > 4 GeV/c");
t->DrawLatex(0.2,1.21,"Centrality 0-100%");

c1->Update();

c1->SaveAs("DR21VsRap_Theory.pdf");

}
