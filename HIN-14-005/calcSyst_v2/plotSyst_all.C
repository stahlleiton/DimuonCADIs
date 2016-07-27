/*
Macro to calculate the systm. uncertainty for v2
Input:
 *.dat produced by v2_fitter.C macro
Output:
 *.dat files, containing the total uncert, as well as individual contributions

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
#include <TH1D.h>
#include <TFile.h>
#include <TH1D.h>
#include <TF1.h>
#include <TMath.h>
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
#include "../macro_v2/v2_dataNumbers_2015.h"
#endif

void plotSyst_all(
    int jpsiCategory      = -1, // -1: all, 1 : Prompt, 2 : Non-Prompt, 3: Bkg
    int nChoseVariable    = -1, // -1 = all; 0 integrated; 1 pt; 2: rapidity; 3: centrality;
    string nDphiBins      = "4",
    const char* inputDir  = "../macro_v2/outputNumbers", // the place where the input root files, with the histograms are
    const char* outputDir = "histSyst",// where the output figures will be
    bool bDoDebug         = false,
    bool bSavePlots       = true
    ) {
  gSystem->mkdir(Form("./%s/png",outputDir), kTRUE);
  gSystem->mkdir(Form("./%s/pdf",outputDir), kTRUE);
  // set the style
  setTDRStyle();
 
  // input files: prompt and non-prompt ones
  const int nFiles = 10;
  // the position of the variations in the array is used later; make sure it corresponds to wht's in array
  int iVar_4d  = 1;
  int iVar_3d  = 2;
  int iVar_tnp = 4;
  int iVar_fit = 5;
  const char* v2InFileDirs[nFiles] = {"histsV2Yields_20160304_v2W_dPhiBins4",  //0
              "histsV2Yields_20160304_v2W_ctau2mm_dPhiBins4",//1
              "histsV2Yields_20160304_v2W_minVar_dPhiBins4",//2
              "histsV2Yields_20160304_v2W_maxVar_dPhiBins4", //3
              "histsV2Yields_20160304_v2W_noTnPSF_dPhiBins4",//4
              "histsV2Yields_20160304_v2W_sigG1G2_dPhiBins4",//5
              "histsV2Yields_20160304_v2W_polFunct_dPhiBins4",//6
              "histsV2Yields_20160304_v2W_resOpt2_dPhiBins4",//7
              "histsV2Yields_20160304_v2W_MLAR_dPhiBins4",//8
              "histsV2Yields_20160304_v2W_const_dPhiBins4"//9
  };
// legend or systm variation
const char* legendSyst[] = {"",
          "4DEff", 
          "3DEff_min",
          "3DEff_max",
          "no_TnP", 
          "signal_2Gaus",
          "bkg_polFunct",
          "signal_resOpt2", 
          "bFrac_MLAR",
          "constrained"};

 const char* variable[4] = {"0-100\%", "p_{T} (GeV/c)", "|y|", "N_{part}"};
 const char* signal[4]   = {"", "Prp","NPrp","Bkg"};

  // Check if categEnd is small or equal to the array size of sample list 
  int categStart        = 1;
  int categEnd          = 4;
  if (jpsiCategory==1) categEnd=2;
  if (jpsiCategory==2) {categStart=2; categEnd=3;}
  if (jpsiCategory==3) {categStart=3;}

  int varStart = 0; 
  int varEnd   = 4;
  if(nChoseVariable==0) varEnd=1;//mb only
  if(nChoseVariable==1) {varStart=1; varEnd=2;}//pt only
  if(nChoseVariable==2) {varStart=2; varEnd=3;}//y only
  if(nChoseVariable==3) {varStart=3;}//centrality only

  //------------ read the numbers from input file
  for(int iCateg=categStart; iCateg<categEnd; iCateg++)
  {
    cout<<"====================== Doing "<< signal[iCateg] << "==========================="<<endl;

    for ( int iVar=varStart; iVar<varEnd; iVar++)
    {  

      int nBins           =  1; // mb, 1 bin, is the default
      if(iCateg==1 || iCateg==3)
      {
        if (iVar == 1)  nBins =  nPtBins_pr;
        if (iVar == 2)  nBins =  nYBins_pr;
        if (iVar == 3)  nBins =  nCentBins_pr;
      }
      if(iCateg ==2)
      {
        if (iVar == 1)  nBins =  nPtBins_np; // for non-prompt Jpsi, take as default 2 pt bin
        if (iVar == 2)  nBins =  nYBins_np;
        if (iVar == 3)  nBins =  nCentBins_np;
      }
      cout<<" !!!!! Number of bins: "<< nBins<<endl;


      cout<<"######## Variable: "<< outFilePlot[iVar] << "  ########" << endl;
      double adV2[nFiles][nBins]; // v2 values
    
      std::vector< std::vector<std::string> > str_kin;// pt, rapidity, centrlaity, to store for output dat files
      str_kin.resize( nBins );
      for( auto &i : str_kin )
        i.resize( 3 );

      // open the files with v2, and store them in arrays
      for(int iFile = 0; iFile<nFiles; iFile++)
      {
        ifstream in;
        std::string nameVar   = outFilePlot[iVar]; //rap
        std::string nameSig   = signal[iCateg];// pr, npr, bkg
        std::string nameDir   = v2InFileDirs[iFile];// 
        string inputFile      = nameVar + "_"+ nameSig + "_nphibin" + nDphiBins + ".dat";
        cout << "!!!!!! Input file name: "<< nameDir <<"/\t"<<inputFile <<endl;
        // read the v2 nominal value from input file
        
        string whatBin[3];
        double x[4] = {0};
        int   iline = 0;
        string tmpstring;
        in.open(Form("%s/%s/data/%s",inputDir,nameDir.c_str(),inputFile.c_str()));
        
        getline(in,tmpstring);
        
        while ( in.good() && iline<nBins ) {
          in >> whatBin[0] >> whatBin[1] >> whatBin[2] >> x[0] >> x[1] >> x[2] >> x[3];
          str_kin[iline][0] = whatBin[0];
          str_kin[iline][1] = whatBin[1];
          str_kin[iline][2] = whatBin[2];
            
          adV2[iFile][iline]  = x[2];
            
          cout<< "Bin " << whatBin[0] << "\t"<< whatBin[1] << "\t" << whatBin[2]<<"\t";
          cout <<"v2= "<< x[2] <<endl;
          iline++;
        }
        in.close();
      }// iFile

      // ################ For centrality, it's in decreasing order of centrality: 010 first, 30-60 last      
      if(bDoDebug) {
        for(int ib=0; ib<nBins; ib++) {
          cout <<"!!!! Looking at: pt = "<< str_kin[ib][0]<<"\t y: "<<str_kin[ib][1]<<"\t cent: "<<str_kin[ib][2]<<endl;
        }
      }
      // at this point have in the 2D arrays all variations; nominal value is stored in [0][0] 
      //-------------------------------------------
      // calculate the systm. uncertainty: 
      // A) for TnP: full difference between with and without TnP -- var #4
      // B) for 4D efficiency: -- variation # 1
      // C) for 3D efficiency: full difference between max and min variation -- #3 and #2
      // D) for fit(sgn+bkg) systm: RMS of all variations -- #5--#9
      
      double v2[nBins];
      double contrib_tnp[nBins];
      double contrib_4d[nBins];
      double contrib_3d_min[nBins];
      double contrib_3d_max[nBins];
      double contrib_fit_0[nBins];
      double contrib_fit_1[nBins];
      double contrib_fit_2[nBins];
      double contrib_fit_3[nBins];
      double contrib_fit_4[nBins];
      
      double v2Syst[nBins];
      
      for(int ib=0; ib<nBins; ib++) {
        double v2 = adV2[0][ib];// nominal value for bin ib
    
        // calculate individual contributions
        double relContrib_tnp = (v2-adV2[iVar_tnp][ib])/v2;
        contrib_tnp[ib]       = 100 * relContrib_tnp; 
        
        double relContrib_4d = (v2-adV2[iVar_4d][ib])/v2;
        contrib_4d[ib]       = 100 * relContrib_4d;
    
        double relContrib_3d_max = (v2-adV2[iVar_3d][ib])/v2;
        double relContrib_3d_min = (v2-adV2[iVar_3d+1][ib])/v2;
        contrib_3d_min[ib]       = 100 * relContrib_3d_min;
        contrib_3d_max[ib]       = 100 * relContrib_3d_max;
  
        for (int iFit=iVar_fit; iFit<nFiles; iFit++)
        {
          double iFitContrib = (v2-adV2[iFit][ib])/v2;
          if(iFit==iVar_fit) contrib_fit_0[ib]   = 100 * iFitContrib;
          if(iFit==iVar_fit+1) contrib_fit_1[ib] = 100 * iFitContrib;
          if(iFit==iVar_fit+2) contrib_fit_2[ib] = 100 * iFitContrib;
          if(iFit==iVar_fit+3) contrib_fit_3[ib] = 100 * iFitContrib;
          if(iFit==iVar_fit+4) contrib_fit_4[ib] = 100 * iFitContrib;
          
          if(bDoDebug)
          {
            cout<<"Fit contribution "<<iFit<<"\t = "<< iFitContrib<<endl;
          }
        }

      }//for each bin

      //-----------------------------------------------------------------------
      // set the TGraphs
      TGraph *gSyst_fit0  = new TGraph(nBins, adXaxis_mb, contrib_fit_0);
      TGraph *gSyst_fit1  = new TGraph(nBins, adXaxis_mb, contrib_fit_1);
      TGraph *gSyst_fit2  = new TGraph(nBins, adXaxis_mb, contrib_fit_2);
      TGraph *gSyst_fit3  = new TGraph(nBins, adXaxis_mb, contrib_fit_3);
      TGraph *gSyst_fit4  = new TGraph(nBins, adXaxis_mb, contrib_fit_4);
 

      TGraph *gSyst_4d    = new TGraph(nBins, adXaxis_mb, contrib_4d);
      TGraph *gSyst_3dmin = new TGraph(nBins, adXaxis_mb, contrib_3d_min);
      TGraph *gSyst_3dmax = new TGraph(nBins, adXaxis_mb, contrib_3d_max);
      TGraph *gSyst_tnp   = new TGraph(nBins, adXaxis_mb, contrib_tnp);

      if(iVar==1) //pt
      {
        if(iCateg!=2)
        {
          gSyst_fit0  = new TGraph(nBins, ptBins_pr, contrib_fit_0);
          gSyst_fit1  = new TGraph(nBins, ptBins_pr, contrib_fit_1);
          gSyst_fit2  = new TGraph(nBins, ptBins_pr, contrib_fit_2);
          gSyst_fit3  = new TGraph(nBins, ptBins_pr, contrib_fit_3);
          gSyst_fit4  = new TGraph(nBins, ptBins_pr, contrib_fit_4);
   
          gSyst_4d    = new TGraph(nBins, ptBins_pr, contrib_4d);
          gSyst_3dmin = new TGraph(nBins, ptBins_pr, contrib_3d_min);
          gSyst_3dmax = new TGraph(nBins, ptBins_pr, contrib_3d_max);
          gSyst_tnp   = new TGraph(nBins, ptBins_pr, contrib_tnp);
        }
        if(iCateg==2)
        {
          gSyst_fit0  = new TGraph(nBins, ptBins_np, contrib_fit_0);
          gSyst_fit1  = new TGraph(nBins, ptBins_np, contrib_fit_1);
          gSyst_fit2  = new TGraph(nBins, ptBins_np, contrib_fit_2);
          gSyst_fit3  = new TGraph(nBins, ptBins_np, contrib_fit_3);
          gSyst_fit4  = new TGraph(nBins, ptBins_np, contrib_fit_4);
   
          gSyst_4d    = new TGraph(nBins, ptBins_np, contrib_4d);
          gSyst_3dmin = new TGraph(nBins, ptBins_np, contrib_3d_min);
          gSyst_3dmax = new TGraph(nBins, ptBins_np, contrib_3d_max);
          gSyst_tnp   = new TGraph(nBins, ptBins_np, contrib_tnp);
        }
      }

      if(iVar==2) //rapidiy
      {
        if(iCateg!=2)
        {
          gSyst_fit0  = new TGraph(nBins, adXaxisY_pr, contrib_fit_0);
          gSyst_fit1  = new TGraph(nBins, adXaxisY_pr, contrib_fit_1);
          gSyst_fit2  = new TGraph(nBins, adXaxisY_pr, contrib_fit_2);
          gSyst_fit3  = new TGraph(nBins, adXaxisY_pr, contrib_fit_3);
          gSyst_fit4  = new TGraph(nBins, adXaxisY_pr, contrib_fit_4);
   
          gSyst_4d    = new TGraph(nBins, adXaxisY_pr, contrib_4d);
          gSyst_3dmin = new TGraph(nBins, adXaxisY_pr, contrib_3d_min);
          gSyst_3dmax = new TGraph(nBins, adXaxisY_pr, contrib_3d_max);
          gSyst_tnp   = new TGraph(nBins, adXaxisY_pr, contrib_tnp);
        }
        if(iCateg==2)
        {
          gSyst_fit0  = new TGraph(nBins, adXaxisY_np, contrib_fit_0);
          gSyst_fit1  = new TGraph(nBins, adXaxisY_np, contrib_fit_1);
          gSyst_fit2  = new TGraph(nBins, adXaxisY_np, contrib_fit_2);
          gSyst_fit3  = new TGraph(nBins, adXaxisY_np, contrib_fit_3);
          gSyst_fit4  = new TGraph(nBins, adXaxisY_np, contrib_fit_4);
   
          gSyst_4d    = new TGraph(nBins, adXaxisY_np, contrib_4d);
          gSyst_3dmin = new TGraph(nBins, adXaxisY_np, contrib_3d_min);
          gSyst_3dmax = new TGraph(nBins, adXaxisY_np, contrib_3d_max);
          gSyst_tnp   = new TGraph(nBins, adXaxisY_np, contrib_tnp);
        }
      }
      if(iVar==3) //centrality
      {
        if(iCateg!=2)
        {
          gSyst_fit0  = new TGraph(nBins, flip_adXaxisCent_pr, contrib_fit_0);
          gSyst_fit1  = new TGraph(nBins, flip_adXaxisCent_pr, contrib_fit_1);
          gSyst_fit2  = new TGraph(nBins, flip_adXaxisCent_pr, contrib_fit_2);
          gSyst_fit3  = new TGraph(nBins, flip_adXaxisCent_pr, contrib_fit_3);
          gSyst_fit4  = new TGraph(nBins, flip_adXaxisCent_pr, contrib_fit_4);
   
          gSyst_4d    = new TGraph(nBins, flip_adXaxisCent_pr, contrib_4d);
          gSyst_3dmin = new TGraph(nBins, flip_adXaxisCent_pr, contrib_3d_min);
          gSyst_3dmax = new TGraph(nBins, flip_adXaxisCent_pr, contrib_3d_max);
          gSyst_tnp   = new TGraph(nBins, flip_adXaxisCent_pr, contrib_tnp);
        }
        if(iCateg==2)
        {
          gSyst_fit0  = new TGraph(nBins, flip_adXaxisCent_np, contrib_fit_0);
          gSyst_fit1  = new TGraph(nBins, flip_adXaxisCent_np, contrib_fit_1);
          gSyst_fit2  = new TGraph(nBins, flip_adXaxisCent_np, contrib_fit_2);
          gSyst_fit3  = new TGraph(nBins, flip_adXaxisCent_np, contrib_fit_3);
          gSyst_fit4  = new TGraph(nBins, flip_adXaxisCent_np, contrib_fit_4);
   
          gSyst_4d    = new TGraph(nBins, flip_adXaxisCent_np, contrib_4d);
          gSyst_3dmin = new TGraph(nBins, flip_adXaxisCent_np, contrib_3d_min);
          gSyst_3dmax = new TGraph(nBins, flip_adXaxisCent_np, contrib_3d_max);
          gSyst_tnp   = new TGraph(nBins, flip_adXaxisCent_np, contrib_tnp);
        }
        // remove 1st point which is 0-10%
        gSyst_fit0->RemovePoint(0); 
        gSyst_fit1->RemovePoint(0); 
        gSyst_fit2->RemovePoint(0); 
        gSyst_fit3->RemovePoint(0); 
        gSyst_fit4->RemovePoint(0); 
     
        gSyst_4d->RemovePoint(0); 
        gSyst_3dmin->RemovePoint(0); 
        gSyst_3dmax->RemovePoint(0); 
        gSyst_tnp->RemovePoint(0); 
      }

      // marker styles
      gSyst_fit0->SetMarkerStyle(20);
      gSyst_fit1->SetMarkerStyle(21);
      gSyst_fit2->SetMarkerStyle(22);
      gSyst_fit3->SetMarkerStyle(23);
      gSyst_fit4->SetMarkerStyle(24);
 

      gSyst_4d->SetMarkerStyle(25);
      gSyst_3dmin->SetMarkerStyle(26);
      gSyst_3dmax->SetMarkerStyle(27);
      gSyst_tnp->SetMarkerStyle(28);  

      gSyst_fit0->SetMarkerSize(1.2);
      gSyst_fit1->SetMarkerSize(1.2);
      gSyst_fit2->SetMarkerSize(1.2);
      gSyst_fit3->SetMarkerSize(1.2);
      gSyst_fit4->SetMarkerSize(1.2);
 

      gSyst_4d->SetMarkerSize(1.2);  
      gSyst_3dmin->SetMarkerSize(1.2);
      gSyst_3dmax->SetMarkerSize(1.2);
      gSyst_tnp->SetMarkerSize(1.2);

      // --------- draw in each canvas
      //----------------------- DRAWING settings ---------------------------------------
      //---axes
      TF1 *f4 = new TF1("f4","0",0,400);
      if(iVar==1)f4 = new TF1("f4","0",0,30);
      if(iVar==2)f4 = new TF1("f4","0",0,2.4);
      f4->GetYaxis()->SetRangeUser(-100,100);
      if(iCateg==2 && iVar==3)f4->GetYaxis()->SetRangeUser(-300,300);
      f4->GetXaxis()->CenterTitle(kTRUE);
      f4->SetLineWidth(1); 
      f4->GetYaxis()->SetTitle("(nominal-variation)/nominal [%]");
      f4->GetXaxis()->SetTitle(Form("%s",variable[iVar]));
   
      //---- canvas
      // legend for fit variation  
      TLegend *leg1 = new TLegend(0.65,0.7,0.8,0.9);
      leg1->SetFillStyle(0);
      leg1->SetFillColor(0);
      leg1->SetBorderSize(0);
      leg1->SetMargin(0.2);
      leg1->SetTextSize(0.04);
      leg1->AddEntry(gSyst_fit0,Form("%s",legendSyst[5]),"P");
      leg1->AddEntry(gSyst_fit1,Form("%s",legendSyst[6]),"P");
      leg1->AddEntry(gSyst_fit2,Form("%s",legendSyst[7]),"P");
      leg1->AddEntry(gSyst_fit3,Form("%s",legendSyst[8]),"P");
      leg1->AddEntry(gSyst_fit4,Form("%s",legendSyst[9]),"P");
      
      // legend for rest of variations
      TLegend *leg2 = new TLegend(0.65,0.15,0.8,0.35);
      leg2->SetFillStyle(0);
      leg2->SetFillColor(0);
      leg2->SetBorderSize(0);
      leg2->SetMargin(0.2);
      leg2->SetTextSize(0.04);
      leg2->AddEntry(gSyst_4d,Form("%s",legendSyst[4]),"P");
      leg2->AddEntry(gSyst_3dmin,Form("%s",legendSyst[3]),"P");
      leg2->AddEntry(gSyst_3dmax,Form("%s",legendSyst[2]),"P");
      leg2->AddEntry(gSyst_tnp,Form("%s",legendSyst[1]),"P");



      //----------------------------------------
      TLatex *lcent = new TLatex(20,-80,"Cent. 10-60%");
      lcent->SetTextFont(42);
      lcent->SetTextSize(0.05);

      TLatex *ly = new TLatex(20.0,-70,"|y| < 2.4");
      ly->SetTextFont(42);
      ly->SetTextSize(0.05);

      TLatex *lpt = new TLatex(20.0,-60,"6.5 < p_{T} < 30 GeV/c");
      lpt->SetTextFont(42);
      lpt->SetTextSize(0.05);
      //---------------------------------------
      TCanvas *pc = new TCanvas("pc","pc", 1000,500);
      pc->Divide(2,1);
      pc->cd(1); f4->Draw();gPad->SetGridy();
      pc->cd(2); f4->Draw();gPad->SetGridy();
      
      pc->cd(1);
      gSyst_fit0->Draw("P");
      gSyst_fit1->Draw("P");
      gSyst_fit2->Draw("P");
      gSyst_fit3->Draw("P");
      gSyst_fit4->Draw("P");
     
      pc->cd(2);
      gSyst_4d->Draw("P");
      gSyst_3dmin->Draw("P");
      gSyst_3dmax->Draw("P");
      gSyst_tnp->Draw("P");

      // //---------------------------------- legends and labels
      TLatex *lCateg = new TLatex(20,85,Form("%s",legend[iCateg]));
      pc->cd(1);
      leg1->Draw();

      if(iVar==0) //mb
      {
        pc->cd(1); lpt->Draw(); ly->Draw(); lcent->Draw(); pc->Update();
      }
      if(iVar==1) //pt
      {
        lCateg = new TLatex(2,85,Form("%s",legend[iCateg]));
        pc->cd(1); ly->Draw(); lcent->Draw(); pc->Update();
      }
      if(iVar==2) //rapidty
      {
        lCateg = new TLatex(0.15,85,Form("%s",legend[iCateg]));
        pc->cd(1); lpt->Draw(); lcent->Draw(); pc->Update();
      }
      if(iVar==3) //centrality
      {
        pc->cd(1); lpt->Draw(); ly->Draw(); pc->Update();
      }

      lCateg->SetTextFont(42);
      lCateg->SetTextSize(0.05);
      lCateg->Draw();

      pc->cd(2);
      leg2->Draw();
      pc->Update();



      //-------------- 

      // -----------------------------------------------------------------------------
      // write out the output
      std::string nameVar   = outFilePlot[iVar]; //pt
      std::string nameSig   = signal[iCateg];// pr, npr, bkg
      string outputFileName     = "syst_"+nameVar + "_"+ nameSig;
            
      if(bSavePlots)
      {
        pc->SaveAs(Form("%s/png/v2_%s.png",outputDir,outputFileName.c_str()));
        pc->SaveAs(Form("%s/pdf/v2_%s.pdf",outputDir,outputFileName.c_str()));
      }
    }// for iVar= ... each variable, pt, y, cent
  }// for iCateg.. each category, pr, non-pr, bkg
}






