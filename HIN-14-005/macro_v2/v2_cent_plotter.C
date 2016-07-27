/*
Macro to plot the
a)  v2 vs pT for prompt, non-prompt and bkg, 
b)  the phi fits for each bin
Input:
a) *.dat and *.root produced by v2_fitter.C macro in the same directory
b) the systematic uncertainties, which are calculated in excel, and hard-coded in v2_dataBinning_2015.h

 */
#if !defined(__CINT__) || defined(__MAKECINT__)
#include <iostream>
#include <fstream>
#include <string>

#include <Riostream.h>
#include <TSystem.h>
#include <TProfile.h>
#include <TBrowser.h>
#include <TROOT.h>
#include <TGraph.h>
#include <TNtuple.h>
#include <TString.h>
#include <TFile.h>
#include <TF1.h>
#include <TMath.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TStyle.h>
#include <TLatex.h>
#include <TInterpreter.h>
#include <TGraphAsymmErrors.h>
#include <TGraphErrors.h>
#include "../CMS_lumi.C"
#include "../tdrstyle.C"
#include "v2_dataNumbers_2015.h"
#endif


void v2_cent_plotter(
		     int jpsiCategory      = 1, // -1: all; 1 : Prompt, 2 : Non-Prompt, 3: Bkg
		     string nDphiBins      = "4",
		     const char* outputDir = "output", 
		     const char* outputRootDir= "outRoot",
		     const char* inputDir  = "outputNumbers",// where phi and v2 numbers are (root, and txt format)
		     const char* inputDirSyst = "../calcSyst_v2/histSyst",// where phi and v2 numbers are (root, and txt format)
		     bool bDoDebug         = true,
		     bool bAddCent010      = false, 
		     bool bSavePlots       = true,
		     bool bSaveRoot        = true
		     ) {
  using namespace std;
  gSystem->mkdir(Form("./%s",outputRootDir), kTRUE);
  gSystem->mkdir(Form("./%s/png",outputDir), kTRUE);
  gSystem->mkdir(Form("./%s/pdf",outputDir), kTRUE);
  
  // set the style
  setTDRStyle();
  gStyle->SetOptFit(0);
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(kFALSE);

  // input files: prompt and non-prompt ones
  const char* v2InFileDirs[1] = {"histsV2Yields_20160304_v2W_dPhiBins4"};
  const char* signal[4]       = {"", "Prp","NPrp","Bkg"};
 
  int categStart        = 1;
  int categEnd          = 4;
  if (jpsiCategory==1) categEnd=2;
  if (jpsiCategory==2) {categStart=2; categEnd=3;}
  if (jpsiCategory==3) {categStart=3;}

  // Reminder for TGraphAssymError: gr = new TGraphAsymmErrors(n,x,y,exl,exh,eyl,eyh);// n,x,y,err_x, err_y
  unsigned int nBins          =  nCentBins_pr;
  for(int iCateg=categStart; iCateg<categEnd; iCateg++)
    {
      if(iCateg==2) nBins   =  nCentBins_np;
      if(!bAddCent010) nBins = nBins-1; // if you want to skip 0-10% bin
  
      cout<<" !!!!! Number of cent bins: "<< nBins<<endl;
      
      double *adXaxis = new double[nBins];   //   location on x-axis  
      double *adV2      = new double[nBins]; // v2 values
      double *adV2_stat = new double[nBins] ;// stat uncert
      double *adV2_syst = new double[nBins] ;// stat uncert
      double *adV2_err0 = new double[nBins] ;// error  0
      
      for (unsigned int ib=0; ib<nBins; ib++)
	{
	  adWidth_systBox[ib] = 10;
	  
	  if(iCateg==2) adXaxis[ib] = adXaxisCent_np[ib];
	  else adXaxis[ib] = adXaxisCent_pr[ib];
	  
	  if(bDoDebug) cout<<"Bin "<<ib<<"\t adAxis= "<<adXaxis[ib]<<" " << nBins<<endl;
	}
      
      //--------------------------------------------------------
      // read from input file
      
      ifstream in;
      std::string nameVar   = outFilePlot[3]; // cent
      std::string nameSig   = signal[iCateg]; // prompt, non-pro or bkg
      std::string nameDir   = v2InFileDirs[0];
      string inputFile      = nameVar + "_"+ nameSig + "_nphibin" + nDphiBins + ".dat";
      
      cout << "!!!!!! Input file name: "<< inputFile <<endl;
      in.open(Form("%s/%s/data/%s",inputDir,nameDir.c_str(),inputFile.c_str()));
      
      // *************** read the v2 and v2_stat 
      string whatBin[3];
      double x[4] = {0};
      double fitprob = 0;
      unsigned int iline   = 0;
      string tmpstring;
      getline(in,tmpstring);
      if(!bAddCent010) getline(in,tmpstring); // pr J/psi v2 doesn't include 0-10% bin
      while ( in.good() && iline<nBins)
	{
	  in >> whatBin[0] >> whatBin[1] >> whatBin[2] >> x[0] >> x[1] >> x[2] >> x[3] >> fitprob;
	  
	  adV2[nBins-iline-1]      = x[2];
	  adV2_stat[nBins-iline-1] = x[3];
	  
	  cout<< "Bin " << whatBin[0] << "\t"<< whatBin[1] << "\t" << whatBin[2]<<"\t";
	  cout <<"v2= "<< adV2[nBins-iline-1] << "\t stat_error= "<< adV2_stat[nBins-iline-1] <<endl;
	  iline++;
	}
      in.close();
      //--------------------------------------------------------
      // ******* read the systematic numbers 
      string inputFile_syst      = "syst_" + inputFile;
      cout << "!!!!!! Input syst. file name: "<< inputFile_syst <<endl;
      in.open(Form("%s/data/%s",inputDirSyst,inputFile_syst.c_str()));
      
      if (!in.good()) {cout << "######### Fail to open syst_input.txt file.##################" << endl;}
      
      double y[5] = {0};
      fitprob     = 0;
      iline       = 0;
      getline(in,tmpstring);
      if(!bAddCent010) getline(in,tmpstring); // pr J/psi v2 doesn't include 0-10% bin
      while ( in.good() && iline<nBins) {
	in >> whatBin[0] >> whatBin[1] >> whatBin[2] >> y[0] >> y[1] >> y[2] >> y[3] >> y[4] >> fitprob;
	adV2_syst[nBins-iline-1]      = y[1];

	cout<< "Bin " << whatBin[0] << "\t"<< whatBin[1] << "\t" << whatBin[2]<<"\t";
	cout <<"v2= "<< adV2[nBins-iline-1] << "\t syst_error= "<< adV2[nBins-iline-1] <<endl;
	iline++;
      }
      in.close();

      //--------------------------------------------------------

      if(bDoDebug)
	{
	  cout << "debugging " << nBins << endl;
	  for(unsigned int ib=0; ib<nBins; ib++) cout<<"Bin "<<ib<<"\t adXaxis: "<< adXaxis[ib]<<"\t v2= "<<adV2[ib]<<endl;
	}
      // high-pt
      TGraphErrors *pgV2          = new TGraphErrors(nBins, adXaxis, adV2, adV2_stat, adV2_stat);
      TGraphErrors *pgV2_sys      = new TGraphErrors(nBins, adXaxis, adV2, adWidth_systBox,adV2_syst);
      TGraphErrors *pgV2_cont     = new TGraphErrors(nBins, adXaxis, adV2, adV2_err0, adV2_err0);
      
      //-------------------------------------------------- Drawing stuff
      // colors and symbols
      // high-pt
      pgV2->SetMarkerColor(kRed+1);
      pgV2_sys->SetFillColor(kRed-9);
      
      pgV2->SetMarkerStyle(21);
      pgV2_sys->SetMarkerStyle(21);
      pgV2_cont->SetMarkerStyle(25); 
      
      pgV2->SetMarkerSize(1.1);
      pgV2_cont->SetMarkerSize(1.1);
      
      if(iCateg==2)
	{
	  pgV2->SetMarkerColor(kOrange+2);
	  pgV2_sys->SetFillColor(kOrange-9);
	}
      if(iCateg==3)// bkg
	{
	  pgV2->SetMarkerColor(1);
	  pgV2_sys->SetFillColor(19);
	}
 
      //-------------------------------------------------------
      // general labels 
      TLatex *lt1  = new TLatex();
      lt1->SetNDC();
      lt1->SetTextFont(42);
      lt1->SetTextSize(0.04);
      
      TH1F *phAxis = new TH1F("phAxis",";N_{part};v_{2}",10,0,400);
      if(iCateg==2) {
	phAxis->GetYaxis()->SetRangeUser(-0.07,0.25);
      } else {
	phAxis->GetYaxis()->SetRangeUser(0,0.25);
      }
      phAxis->GetXaxis()->CenterTitle();
      phAxis->GetYaxis()->CenterTitle();
      
      TF1 *line    = new TF1();//"line","0",0,400);
      line->SetLineWidth(1);

      //-------------- Drawing 
      TCanvas *pc = new TCanvas("pc","pc");
      phAxis->Draw();
      CMS_lumi(pc,101,33);
      lt1->SetTextSize(0.05);
      lt1->DrawLatex(0.2,0.85,Form("%s",legend[iCateg]));
      lt1->SetTextSize(0.04);
      lt1->DrawLatex(0.2,0.80,Form("%s",ptBinsLegend[0]));
      lt1->SetTextSize(0.04);
      lt1->DrawLatex(0.2,0.75,Form("%s",yBinsLegend[0]));
      lt1->SetTextSize(0.04);
      lt1->DrawLatex(0.2,0.20,Form("Cent."));
  
      if(iCateg==2) {
	lt1->DrawLatex((adXaxis[0]+30)/400.0,0.20,Form("%s",centBinsLegend[nCentBins-1]));
	lt1->DrawLatex((adXaxis[1]+5)/400.0,0.20,Form("%s",centBinsLegend[nCentBins-1-1]));
	lt1->DrawLatex((adXaxis[2]-5)/400.0,0.20,Form("%s",centBinsLegend[nCentBins-1-2]));
      } else {
	lt1->DrawLatex((adXaxis[0]+30)/400.0,0.20,Form("%s",centBinsLegend[nCentBins-nBins-1]));
	lt1->DrawLatex((adXaxis[1]+5)/400.0,0.20,Form("%s",centBinsLegend[nCentBins-nBins-2]));
	lt1->DrawLatex((adXaxis[2]-5)/400.0,0.20,Form("%s",centBinsLegend[nCentBins-nBins-3]));
      }
  
      pgV2_sys->Draw("2");
      pgV2->Draw("PZ");
      pgV2_cont->Draw("P");
      gPad->RedrawAxis();
 
      if(bSavePlots)
	{
	  pc->SaveAs(Form("%s/png/v2_%s_%s_nphi%s.png",outputDir,nameVar.c_str(),nameSig.c_str(),nDphiBins.c_str()));
	  pc->SaveAs(Form("%s/pdf/v2_%s_%s_nphi%s.pdf",outputDir,nameVar.c_str(),nameSig.c_str(),nDphiBins.c_str()));
	}

      if(bSaveRoot)
	{
	  TFile *pfOutput = new TFile(Form("%s/%s_v2_cent_plotter.root",outputRootDir,signal[iCateg]),"RECREATE");
	  pgV2->Write("pgV2");
	  pgV2_sys->Write("pgV2_sys");
	  pgV2_cont->Draw("pgV2_cont");
	  pfOutput->Write(); 
  
	}
       
      delete[] adXaxis;
      delete[] adV2;
      delete[] adV2_stat;
      delete[] adV2_syst;
      delete[] adV2_err0;
    }
  
}




