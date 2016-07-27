/*
  Macro that makes histograms of:
1) yields from the fit_table file that contains the fit results, that sits in a path of the form data/v2/20150817_PbPb/pp_../noWeight-weighted_.../summary/

  Before running the macro:
a) chose adjut the name of the directories (2015...) in the 'whichSample[1]' vector
b) adjust the name of the weighted-no weighted directory, in the vectors whichWeight[2], and whichWeight_pp[2]

The output root files of this macro, wich contains the histograms with the yields, are the input root file that are needed to make the v2 plots! 

 */
#include <Riostream.h>
#include <TSystem.h>
#include <TBrowser.h>
#include <TROOT.h>
#include <TGraph.h>
#include <TNtuple.h>
#include <TString.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TF1.h>
#include <TProfile.h>
#include <TMath.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TStyle.h>
#include <TLatex.h>
#include <TInterpreter.h>
#include <TGraphAsymmErrors.h>
#include <TGraphErrors.h>

using namespace std;

// remember to replace, in the case of the 'weighted' case, the rel. stat uncertianties with the one from the 'unweighted' yields

void makeHistos_v2(int nDphiBins   = 4, 
                   int sampleStart = 0,// the variations to read; all listed in 'whichSample' array
                   int sampleEnd   = -1,
                   int setting     = 0, 
                   const char* inputFitDataFileLocation = "../data/v2/"
                  ) 
{
  const char* whichSetting[1]       = {"20160304"};
  string whichSample[]              = {"v2noW","v2W","v2W_noTnPSF","v2W_muIDTrig","v2W_const","v2W_maxVar","v2W_minVar","v2W_MLAR","v2W_polFunct","v2W_ctau2mm","v2W_resOpt2","v2W_sigG1G2","v2W_4DEffOnly"};
  const char* outputHistDataFile[1] = {"histsV2Yields"};

  double PI = TMath::Pi();

  // Check if sampleEnd is small or equal to the array size of sample list 
  int sampleSize = sizeof(whichSample)/sizeof(string);
  if ((sampleSize < sampleEnd) || (sampleEnd==-1)) sampleEnd = sampleSize;
  for(int is=sampleStart; is<sampleEnd; is++)
  {
    ifstream in;
    string filename = Form("%s/%s/%s/summary/fit_table",inputFitDataFileLocation,whichSetting[setting],whichSample[is].c_str());
    in.open(filename.c_str());
    if (!in.is_open()) {
      cout << "input file " << filename << " cannot be open" << endl;
      continue;
    }

    TFile *pfOutput;
    pfOutput = new TFile(Form("%s_%s_%s_dPhiBins%d.root",outputHistDataFile[0],whichSetting[setting],whichSample[is].c_str(),nDphiBins),"RECREATE");

    TH1F *hBkg[300];
    TH1F *hPrp[300];
    TH1F *hNPrp[300];
    TH1::SetDefaultSumw2();

    double x[300];
    double prpt[300], prptErr[300], nprpt[300], nprptErr[300];
    double bkg[300], bkgErr[300];
    double rap1[300], rap2[300], pT1[300], pT2[300];//, cent1[300], cent2[300];
    int cent1[300], cent2[300];
    char tmp[512]={' '};
    int nline = 0;

    // Read 2 header lines and throw them away to keep only numbers
    string tmpstring;
    getline(in,tmpstring);
    cout << tmpstring << " is being processed..." << endl;
    getline(in,tmpstring);
    
    while (in.good()) 
    {
      in >> x[0] >> x[1] >> x[2] >> x[3] >> x[4] >> x[5] >> x[6] >> x[7] >> x[8] >> x[9] >> x[10] >> x[11] >> x[12] >> x[13] >> x[14] >> x[15] >> x[16] >> x[17];
      rap1[nline]  = x[0];  rap2[nline]     = fabs(x[1]); // rapidity (second value comes with '-')
      pT1[nline]   = x[2];  pT2[nline]      = fabs(x[3]); // pt (second value comes with '-')
      cent1[nline] = x[4];  cent2[nline]    = fabs(x[5]);// centrlaity (second value comes with '-')
      // [6]&[7] is the phi interval
      bkg[nline]   = x[10];  bkgErr[nline]   = fabs(x[11]);  // bkg yield and error
      //[7]&[8] is the inclusive and inclusive error
      prpt[nline]  = x[12]; prptErr[nline]  = fabs(x[13]); // prompt yield and error
      nprpt[nline] = x[14]; nprptErr[nline] = fabs(x[15]); // non-prompt yield and error
      //[16]&[17] is the b-fraction
      
      if(nline == 11)
      {
        cout<<tmp<<endl;
        cout<<"TEST: Stored values in line : "<<nline<<endl;
        cout<<rap1[nline]<<" "<<rap2[nline]<<" ";
        cout<<pT1[nline]<<" "<<pT2[nline]<<" ";
        cout<<cent1[nline]<<" "<<cent2[nline]<<" ";
        cout<<bkg[nline]<<" "<<prpt[nline] <<" "<<prptErr[nline]<<" "<<nprpt[nline] <<" "<<nprptErr[nline]<<endl;
      }
      nline++;
    }
    printf(" found %d points\n",nline);
    
    // Make histograms
    pfOutput->cd();
    
    const int nDB_p1 = nDphiBins+1;
    for(int j = 0; j < nline/nDB_p1; j++)
    {
      // bkg histograms
      TString histBkg(Form("Rap_%0.1f%0.1f_pT_%0.1f%0.1f_Cent_%d%d_Bkg",rap1[nDB_p1*j],rap2[nDB_p1*j],pT1[nDB_p1*j],pT2[nDB_p1*j],cent1[nDB_p1*j],cent2[nDB_p1*j]));
      histBkg.ReplaceAll(".","");
      hBkg[j] = new TH1F(histBkg,";#Delta #phi;",nDphiBins,0,PI/2);
      //prompt histograms
      TString histPrompt(Form("Rap_%0.1f%0.1f_pT_%0.1f%0.1f_Cent_%d%d_Prp",rap1[nDB_p1*j],rap2[nDB_p1*j],pT1[nDB_p1*j],pT2[nDB_p1*j],cent1[nDB_p1*j],cent2[nDB_p1*j]));
      histPrompt.ReplaceAll(".","");
      hPrp[j] = new TH1F(histPrompt,";#Delta #phi;",nDphiBins,0,PI/2);
      //non-prompt histograms
      TString histNonPrompt(Form("Rap_%0.1f%0.1f_pT_%0.1f%0.1f_Cent_%d%d_NPrp",rap1[nDB_p1*j],rap2[nDB_p1*j],pT1[nDB_p1*j],pT2[nDB_p1*j],cent1[nDB_p1*j],cent2[nDB_p1*j]));
      histNonPrompt.ReplaceAll(".","");
      hNPrp[j] = new TH1F(histNonPrompt,";#Delta #phi;",nDphiBins,0,PI/2);
      for (int k=0; k<nDphiBins; k++) {
        //0-1.571 bin needs to be skipped! (2nd in the array)
        int kidx = k;
        if (k>=1) kidx = k+1;
        hBkg[j]->SetBinContent(k+1, bkg[nDB_p1*j+kidx]);
        hBkg[j]->SetBinError(k+1, bkgErr[nDB_p1*j+kidx]);
        hPrp[j]->SetBinContent(k+1, prpt[nDB_p1*j+kidx]);
        hPrp[j]->SetBinError(k+1, prptErr[nDB_p1*j+kidx]);
        hNPrp[j]->SetBinContent(k+1, nprpt[nDB_p1*j+kidx]);
        hNPrp[j]->SetBinError(k+1, nprptErr[nDB_p1*j+kidx]);
      }
//      cout<< "j: " << j << " " << histBkg << endl;
    }
    
    pfOutput->Write();
    in.close();
  }//sample end
}

