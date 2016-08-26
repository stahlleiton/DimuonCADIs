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
#include <TLegendEntry.h>
#include <TStyle.h>
#include <TLatex.h>
#include <TInterpreter.h>
#include <TGraphAsymmErrors.h>
#include <TGraphErrors.h>
#include "../CMS_lumi.C"
#include "../tdrstyle.C"
#include "../textPosition.h"
#include "v2_dataNumbers_2015.h"
#endif

void v2_pt_plotter(
       int jpsiCategory         = 2, // -1: All; 1 : Prompt, 2 : Non-Prompt, 3: Bkg
       int npNumBinsHighPt      = 1, // possible number of bins for non-prompt high-pt: 1 or 2
       string nDphiBins         = "4",
       const char* outputDir    = "output", 
       const char* outputRootDir= "outRoot",
       const char* inputDir     = "outputNumbers",// where phi and v2 numbers are (root, and txt format)
       const char* inputDirSyst = "../calcSyst_v2/histSyst",// where phi and v2 numbers are (root, and txt format)
       bool lineAt65            = false,
       bool bDoDebug            = false,
       bool bSavePlots          = 1,
       bool bSaveRoot           = 1
        ) {
  using namespace std;

  gSystem->mkdir(Form("./%s/png",outputDir), kTRUE);
  gSystem->mkdir(Form("./%s/pdf",outputDir), kTRUE);

  gSystem->mkdir(Form("./%s/npr2ptbin/png",outputDir), kTRUE);
  gSystem->mkdir(Form("./%s/npr2ptbin/pdf",outputDir), kTRUE);
  
  // set the style
  setTDRStyle();

  // input files: prompt and non-prompt ones
  const char* v2InFileDirs[1] = {"histsV2Yields_20160304_v2W_dPhiBins4"};
  const char* signal[4]       = {"", "Prp","NPrp","Bkg"};

  int categStart        = 1;
  int categEnd          = 4;
  if (jpsiCategory==1) categEnd=2;
  if (jpsiCategory==2) {categStart=2; categEnd=3;}
  if (jpsiCategory==3) {categStart=3;}

  TGraphAsymmErrors *pgV2;
  TGraphErrors *pgV2_sys;
  TGraphErrors *pgV2_cont;
  
  // low-pt
  TGraphAsymmErrors *pgV2_low;
  TGraphErrors *pgV2_low_sys;
  TGraphErrors *pgV2_low_cont;
      
  int nBins                   =  nPtBins_pr-1; // remove the low-pt bin, which is treated/read separatelly
  for(int iCateg=categStart; iCateg<categEnd; iCateg++)
    {
      cout<<"====================== Doing "<< signal[iCateg] << "==========================="<<endl;
      if(iCateg==2) 
        {
          if(npNumBinsHighPt==2) nBins   = nPtBins_np-1;
          if(npNumBinsHighPt==1) nBins   = 1;
        }
      cout<<" !!!!! Number of pT bins: "<< nBins<<endl;
  
      double *adXaxis  = new double[nBins];//   location on x-axis  
      double *adXaxis_l = new double[nBins];// bin width to the left
      double *adXaxis_h = new double[nBins];// bin width to the right

      double *adV2 = new double[nBins]      ; // v2 values
      double *adV2_stat = new double[nBins] ;// stat uncert
      double *adV2_syst = new double[nBins] ;// stat uncert
      double *adV2_err0 = new double[nBins] ;// error  0
      double *adV2_low = new double[1]; //low-pt, 1 bin, 3-6.5
      double *adV2_low_stat = new double[1];
      double *adV2_low_syst = new double[1];
      double *adV2_low_err0 = new double[1];

      for(int ib=0; ib<nBins; ib++) {
        adWidth_systBox[ib] = 0.5;
        adXaxis[ib]      = adXaxisPt_pr[ib];
        adXaxis_l[ib]    = adXaxisPt_pr_l[ib];
        adXaxis_h[ib]    = adXaxisPt_pr_h[ib];
        
        if(jpsiCategory==2) {
          adXaxis[ib]      = adXaxisPt_np[ib];
          adXaxis_l[ib]    = adXaxisPt_np_l[ib];
          adXaxis_h[ib]    = adXaxisPt_np_h[ib];
          
          if(npNumBinsHighPt==1)
            {
              adXaxis[ib]      = adXaxisPt_np1[ib];
              adXaxis_l[ib]    = adXaxisPt_np1_l[ib];
              adXaxis_h[ib]    = adXaxisPt_np1_h[ib];
            }
        }
      }//number of bins, low and high
      
      //--------------------------------------------------------
      // read from input file
  
      ifstream in;
      std::string nameVar   = outFilePlot[1];
      std::string nameSig   = signal[iCateg];
      std::string nameDir   = v2InFileDirs[0];
      string inputFile      = nameVar + "_"+ nameSig + "_nphibin" + nDphiBins + ".dat";
      string inputFile_mb   = "mb_"+ nameSig + "_nphibin" + nDphiBins + ".dat";

      // ****** the v2 and v2_stat
      cout << "!!!!!! Input file name: "<< inputFile <<endl;
      in.open(Form("%s/%s/data/%s",inputDir,nameDir.c_str(),inputFile.c_str()));
      if (!in.good()) {cout << "######### Fail to open input.txt file.##################" << endl;}
      string whatBin[3];
      double x[4] = {0};
      double fitprob = 0;
      int iline   = 0;
      string tmpstring;
      getline(in,tmpstring);
      while ( in.good() && iline<nBins+1 ) {
        in >> whatBin[0] >> whatBin[1] >> whatBin[2] >> x[0] >> x[1] >> x[2] >> x[3] >> fitprob;
        if(iline==0) {
          adV2_low[iline]      = x[2];      
          adV2_low_stat[iline] = x[3];
        } else {
          if( !(npNumBinsHighPt==1 && iCateg==2) || iCateg!=2 )
            {
              adV2[iline-1]      = x[2];
              adV2_stat[iline-1] = x[3];
            }
        }
        cout<< "Bin " << whatBin[0] << "\t"<< whatBin[1] << "\t" << whatBin[2]<<"\t";
        cout <<"v2= "<< x[2] << "\t stat_error= "<< x[3]<<endl;
        iline++;
      }
      in.close();
      fitprob = 0;
      if(npNumBinsHighPt==1 && iCateg==2)
        {
          cout << "!!!!!! Input mb file name: "<< inputFile_mb <<endl;
          in.open(Form("%s/%s/data/%s",inputDir,nameDir.c_str(),inputFile_mb.c_str()));
          iline=0;
          getline(in,tmpstring);
          while ( in.good() && iline<nBins ){
            in >> whatBin[0] >> whatBin[1] >> whatBin[2] >> x[0] >> x[1] >> x[2] >> x[3] >> fitprob;
            adV2[iline]      = x[2];
            adV2_stat[iline] = x[3];
            
            cout<< "Bin " << whatBin[0] << "\t"<< whatBin[1] << "\t" << whatBin[2]<<"\t";
            cout <<"v2= "<<adV2[iline] << "\t stat_error= "<< adV2_stat[iline]<<endl;
            
            iline++;
          }
          in.close();
        }
      //--------------------------------------------------------
      // read the systematic numbers 
      string inputFile_syst      = "syst_" + inputFile;
      string inputFile_mb_syst   = "syst_" + inputFile_mb;
      
      
      cout << "!!!!!! Input syst. file name: "<< inputFile_syst <<endl;
      in.open(Form("%s/data/%s",inputDirSyst,inputFile_syst.c_str()));
      
      if (!in.good()) {cout << "######### Fail to open syst_input.txt file.##################" << endl;}
      
      double y[5] = {0};
      fitprob = 0;
      iline       = 0;
      getline(in,tmpstring);
      while ( in.good() && iline<nBins+1) {
        in >> whatBin[0] >> whatBin[1] >> whatBin[2] >> y[0] >> y[1] >> y[2] >> y[3] >> y[4] >> fitprob;
        
        if(iline==0 ) { adV2_low_syst[iline] = y[1];
        } else {
          if(!(npNumBinsHighPt==1 && iCateg==2) || iCateg!=2 )
            adV2_syst[iline-1]      = y[1];
        }
        
        cout<< "Bin " << whatBin[0] << "\t"<< whatBin[1] << "\t" << whatBin[2]<<"\t";
        cout <<"v2= "<< y[0] << "\t syst_error= "<< y[1] <<endl;
        iline++;
      }
      in.close();
      
      // minbias point if the case
      if(npNumBinsHighPt==1 && iCateg==2){
        cout << "!!!!!! Input mb syst. file name: "<< inputFile_mb <<endl;
        in.open(Form("%s/data/%s",inputDirSyst,inputFile_mb_syst.c_str()));

        if (!in.good()) {cout << "######### Fail to open syst_input.txt file.##################" << endl;}
  
        iline=0;
        fitprob = 0;
        getline(in,tmpstring);
        while ( in.good() && iline<nBins ){
          in >> whatBin[0] >> whatBin[1] >> whatBin[2] >> y[0] >> y[1] >> y[2] >> y[3] >> y[4] >> fitprob;
          adV2_syst[iline]      = y[1];
          
          cout<< "Bin " << whatBin[0] << "\t"<< whatBin[1] << "\t" << whatBin[2]<<"\t";
          cout <<"v2= "<< y[0] << "\t syst_error= "<< y[1]<<endl;
          
          iline++;
        }
        in.close();
      }
      
      //--------------------------------------------------------

      if(bDoDebug) {
        for(int ib=0; ib<nBins; ib++) {
          cout<<"Bin "<<ib<<"\t stat. uncert.: "<<adV2_stat[ib]<< "\t syst. uncer.: "<< adV2_syst[ib]<< endl;
          // cout << "adXaxis: "<< adXaxis[ib]<<"\t adXaxis_l: "<<adXaxis_l[ib]<<"\t adXaxis_h: "<<adXaxis_h[ib]<<endl;
        }
        cout<<"Bin 00 "<<"\t stat. uncert.: "<<adV2_low_stat[0]<< "\t syst. uncer.: "<< adV2_low_syst[0]<< endl;
      }
      // high-pt
      pgV2     = new TGraphAsymmErrors(nBins, adXaxis, adV2, adXaxis_l, adXaxis_h, adV2_stat, adV2_stat);
      pgV2_sys = new TGraphErrors(nBins, adXaxis, adV2, adWidth_systBox,adV2_syst);
      pgV2_cont= new TGraphErrors(nBins, adXaxis, adV2, adV2_err0, adV2_err0);
      
      // low-pt
      pgV2_low     = new TGraphAsymmErrors(1, adXaxis_low, adV2_low, adXaxis_low_l, adXaxis_low_h, adV2_low_stat, adV2_low_stat);
      pgV2_low_sys = new TGraphErrors(1, adXaxis_low, adV2_low, adWidth_systBox,adV2_low_syst);
      pgV2_low_cont= new TGraphErrors(1, adXaxis_low, adV2_low, adV2_low_err0, adV2_low_err0);
      
      // //-------------------------------------------------- Drawing stuff
      // colors and symbols
      // high-pt
      pgV2->SetMarkerColor(kRed+1);
      pgV2->SetMarkerStyle(21);
      pgV2->SetMarkerSize(1.1);

      pgV2_sys->SetFillColorAlpha(kRed-9,0.5);      
      pgV2_sys->SetMarkerStyle(21);
      pgV2_sys->SetMarkerSize(1.1);

      pgV2_cont->SetMarkerStyle(25);      
      pgV2_cont->SetMarkerSize(1.1);
      
      if(jpsiCategory==2)
        {
          pgV2->SetMarkerColor(kOrange+2);
          pgV2->SetMarkerStyle(29);
          pgV2->SetMarkerSize(2);

          pgV2_sys->SetFillColorAlpha(kOrange-9,0.5);
          pgV2_sys->SetMarkerStyle(30);
          pgV2_sys->SetMarkerSize(2);
          
          pgV2_cont->SetMarkerStyle(30); 
          pgV2_cont->SetMarkerSize(2);
        }
      if(jpsiCategory==3)// bkg
        {
          pgV2->SetMarkerColor(1);
          pgV2_sys->SetFillColorAlpha(19,0.5);
        }
      // low-pt
      pgV2_low->SetMarkerColor(kViolet+2);
      pgV2_low->SetMarkerStyle(34);
      pgV2_low->SetMarkerSize(1.7);

      pgV2_low_sys->SetFillColorAlpha(kViolet-9,0.5);
      pgV2_low_sys->SetMarkerStyle(28);
      pgV2_low_sys->SetMarkerSize(1.7);

      pgV2_low_cont->SetMarkerStyle(28); 
      pgV2_low_cont->SetMarkerSize(1.7);
      //-------------------------------------------------------
      // general labels 
      TLatex *lt1  = new TLatex();
      lt1->SetNDC();
      lt1->SetTextFont(42);
      lt1->SetTextSize(ltxSetTextSize2);
      
      TH1F *phAxis = new TH1F("phAxis",";p_{T} (GeV/c);v_{2}",10,0,30);
      phAxis->GetYaxis()->SetRangeUser(0,0.25);
      if(jpsiCategory==1){phAxis->GetYaxis()->SetRangeUser(0,0.2);phAxis->GetYaxis()->SetNdivisions(505);}
      phAxis->GetXaxis()->CenterTitle();
      phAxis->GetYaxis()->CenterTitle();
      
      TLine *line    = new TLine();
      line->SetLineWidth(1);
          
      //-------------- Drawing 
      TCanvas *pc = new TCanvas("pc","pc");
      phAxis->Draw();
      CMS_lumi(pc,12001000,0);
      lt1->SetTextSize(ltxSetTextSize1);
      lt1->SetTextFont(132);
      lt1->DrawLatex(ltxText_xStart,ltxText_yStart,Form("%s",legend[iCateg]));

      lt1->SetTextSize(ltxSetTextSize2);
      lt1->SetTextFont(42);
      lt1->DrawLatex(ltxCentOnly_x,ltxCentOnly_y,Form("%s",centBinsLegend[0]));

      
      pgV2_sys->Draw("2");
      pgV2_low_sys->Draw("2");
      
      pgV2_low->Draw("PZ");
      pgV2->Draw("PZ");
      
      pgV2_low_cont->Draw("P");
      pgV2_cont->Draw("P");
      gPad->RedrawAxis();
      
      line->DrawLine(0,0,30,0);
      if (lineAt65) {
        line->SetLineStyle(7);
        if (iCateg==1) line->DrawLine(6.5,0.005,6.5,0.12);
        else if (iCateg==2) line->DrawLine(6.5,0.0,6.5,0.17);
      }

      TLegend *leg1 = new TLegend(legRaaPt_xLowStart,legRaaPt_y,legRaaPt_xLowEnd,legRaaPt_y+0.05,NULL,"brNDC");
      leg1->SetBorderSize(0);
      leg1->SetTextFont(42);
      leg1->SetTextSize(ltxSetTextSize2);
      leg1->SetTextColor(kViolet+2);
      leg1->SetLineColor(1);
      leg1->SetLineStyle(1);
      leg1->SetLineWidth(1);
      leg1->SetFillColor(19);
      leg1->SetFillStyle(0);
      TLegendEntry *entry1 = leg1->AddEntry("pgV2_low","1.6 < |y| < 2.4","p");
      entry1->SetMarkerStyle(34);
      entry1->SetMarkerColor(kViolet+2);
      entry1->SetMarkerSize(1.7);
      entry1->SetFillStyle(1001);
      entry1->SetFillColor(kViolet-9);
      
      TLegend *leg2 = new TLegend(legRaaPt_xHighStart,legRaaPt_y,legRaaPt_xHighEnd,legRaaPt_y+0.05,NULL,"brNDC");
      leg2->SetBorderSize(0);
      leg2->SetTextFont(42);
      leg2->SetTextSize(ltxSetTextSize2);
      if (iCateg==1) leg2->SetTextColor(kRed+1);
      else if(iCateg==2) leg2->SetTextColor(kOrange+2);
      else if(iCateg==3) leg2->SetTextColor(1);
      
      leg2->SetLineColor(1);
      leg2->SetLineStyle(1);
      leg2->SetLineWidth(1);
      leg2->SetFillColor(19);
      leg2->SetFillStyle(0);
      TLegendEntry *entry2 = leg2->AddEntry("pgV2","|y| < 2.4","p");
      entry2->SetFillStyle(1001);
      if (iCateg==1)
      {
        entry2->SetMarkerStyle(21);
        entry2->SetMarkerColor(kRed+1);
        entry2->SetMarkerSize(1.2);
        entry2->SetFillColor(kRed-9);
      }
      else if(iCateg==2)
      {
        entry2->SetMarkerStyle(29);
        entry2->SetMarkerColor(kOrange+2);
        entry2->SetMarkerSize(1.7);
        entry2->SetFillColor(kOrange-9);
      }
      else if(iCateg==3) leg2->SetTextColor(1);
      leg1->Draw();
      leg2->Draw();
      pc->Update();
      
      if(bSavePlots)
      {
        if(npNumBinsHighPt==1 && iCateg==2) 
        {
          pc->SaveAs(Form("%s/npr2ptbin/png/v2_%s_%s_nphi%s.png",outputDir,nameVar.c_str(),nameSig.c_str(),nDphiBins.c_str()));
          pc->SaveAs(Form("%s/npr2ptbin/pdf/v2_%s_%s_nphi%s.pdf",outputDir,nameVar.c_str(),nameSig.c_str(),nDphiBins.c_str()));
        }
        else
        {
          pc->SaveAs(Form("%s/png/v2_%s_%s_nphi%s.png",outputDir,nameVar.c_str(),nameSig.c_str(),nDphiBins.c_str()));
          pc->SaveAs(Form("%s/pdf/v2_%s_%s_nphi%s.pdf",outputDir,nameVar.c_str(),nameSig.c_str(),nDphiBins.c_str()));
        }
      }

      if(bSaveRoot)
      {
        TFile *pfOutput = new TFile(Form("%s/%s_v2_pt_plotter.root",outputRootDir,signal[iCateg]),"RECREATE");
        pgV2->Write("pgV2");
        pgV2_sys->Write("pgV2_sys");
        pgV2_low_sys->Write("pgV2_low_sys");
        pgV2_low->Write("pgV2_low");
        pgV2_cont->Write("pgV2_cont");
        pgV2_low_cont->Write("pgV2_low_cont");

        pfOutput->Write(); 
      }

      delete[] adXaxis;
      delete[] adXaxis_l;
      delete[] adXaxis_h;

      delete[] adV2;
      delete[] adV2_stat;
      delete[] adV2_syst;
      delete[] adV2_err0;
      delete[] adV2_low;
      delete[] adV2_low_stat;
      delete[] adV2_low_syst;
      delete[] adV2_low_err0;
    }//jpsi categ

}




