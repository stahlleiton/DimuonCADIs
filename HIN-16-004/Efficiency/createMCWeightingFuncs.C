//
//  createMCWeightingFuncs.C
//  
//
//  Created by Javier Martin Blanco on 28/06/16.
//
//

#include <stdio.h>
#include <iostream>

#include "TFile.h"
#include "TH1.h"
#include "TString.h"
#include "TObjArray.h"
#include "TF1.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLatex.h"

#include "FitFunctions.h"


using namespace std;

void customiseLegend(TLegend& legend)
{
  legend.SetBorderSize(0);
  legend.SetTextSize(0.04);
  legend.SetLineColor(1);
  legend.SetLineStyle(1);
  legend.SetLineWidth(1);
  legend.SetFillColor(0);
  legend.SetFillStyle(1001);
}

Bool_t initialChecks(const char* partNameCheck, const char* collNameCheck, const char* rapNameCheck, Int_t polOrderCheck)
{
  if (strcmp(partNameCheck,"JPsi") && strcmp(partNameCheck,"Psi2S"))
  {
    cout << "[ERROR]: partName must be Jpsi or Psi2S" << endl;
    return kFALSE;
  }
  
  if (strcmp(collNameCheck,"PP") && strcmp(collNameCheck,"PbPb"))
  {
    cout << "[ERROR]: colName must be PP or PbPb" << endl;
    return kFALSE;
  }
  
  if (strcmp(rapNameCheck,"Mid") && strcmp(rapNameCheck,"Fwd"))
  {
    cout << "[ERROR]: rapName must be Mid or Fwd" << endl;
    return kFALSE;
  }
  
  if ( (polOrderCheck<1) || (polOrderCheck>2))
  {
    cout << "[ERROR]: Only polOrder 1 and 2 are supported" << endl;
    return kFALSE;
  }
  
  return kTRUE;
}

void createMCWeightingFuncs(const char* fileData, const char* fileMC, const char* partName="JPsi", const char* collName="PP", const char* rapName="Mid", Int_t polOrder = 2, Int_t nRand=100)
// partName = "JPsi" or "Psi2S", collName = "PP" or "PbPb"
// rapName = "Mid" or "Fwd"
// nRand is the number of random data distributions that we want to generate from the original one
// polOrder is the order of the polinomial to be used to fit the DATA/MC distribution (supported 1 or 2)
{
  // Input parameters checks
  if (!initialChecks(partName,collName,rapName,polOrder)) return;

  // Read the input files and extract the corresponding distributions
  TFile* fData = new TFile(fileData,"READ");
  TFile* fMC = new TFile(fileMC,"READ");
  if (!fData || !fMC)
  {
    cout << "[ERROR]: One of the two input files was not found" << endl;
    return;
  }
  
  TString hName(Form("hdistr_%s_%s_%s",partName,collName,rapName));
  
  TH1* histoData = static_cast<TH1*>(fData->FindObjectAny(hName.Data()));
  if (!histoData)
  {
    cout << "Data histo " << hName.Data() << " not found in file " << fileData << endl;
    return;
  }
  histoData->Sumw2(kTRUE);
  
  TH1* histoMC = static_cast<TH1*>(fMC->FindObjectAny(hName.Data()));
  if (!histoMC)
  {
    cout << "MC histo " << hName.Data() << " not found in file " << fileMC << endl;
    return;
  }
  histoMC->Sumw2(kTRUE);
  
  // Create output objects
  TObjArray* aFunctions = new TObjArray();
  aFunctions->SetOwner(kTRUE);
  TObjArray* aRatios = new TObjArray();
  aRatios->SetOwner(kTRUE);
  TObjArray* aDataHistos = new TObjArray();
  aDataHistos->SetOwner(kTRUE);
  
  // Normalise the initial DATA and MC distributions
  TH1* histoDataClone = static_cast<TH1*>(histoData->Clone(Form("hdistr_%s_%s_nominal",partName,collName)));
  Double_t norm = 1./histoDataClone->Integral();
  histoDataClone->Scale(norm); // Normalise histo to 1
  histoDataClone->SetLineColor(1);
  histoDataClone->SetMarkerColor(1);
  histoDataClone->SetStats(0);
  histoDataClone->GetXaxis()->SetTitle("p_{T}");
  histoDataClone->GetYaxis()->SetTitle("dN/dp_{T}");
  aDataHistos->Add(histoDataClone); // First object in the arrays will correspond to the nominal results
  
  TH1* histoMCClone = static_cast<TH1*>(histoMC->Clone(Form("hdistr_%s_%s_MC",partName,collName)));
  norm = 1./histoMCClone->Integral();
  histoMCClone->Scale(norm); // Normalise histo to 1
  histoMCClone->SetLineColor(4);
  histoMCClone->SetMarkerColor(4);
  histoMCClone->SetStats(0);
  histoMCClone->GetXaxis()->SetTitle("p_{T}");
  histoMCClone->GetYaxis()->SetTitle("dN/dp_{T}");
  
  // Loop on nRand to get the randomised data distributions
  TH1* histoDataClone2(0x0);
  for (Int_t n=0 ; n<nRand ; n++)
  {
    histoDataClone2 = static_cast<TH1*>(histoDataClone->Clone(Form("hdistr_%s_%s_n%d",partName,collName,n)));
    histoDataClone2->Reset();
    Int_t nBins = histoDataClone2->GetNbinsX();
    for (Int_t i=1; i<=nBins ; i++)
    {
      Double_t newYield = histoDataClone->GetBinContent(i);
      Double_t newError = histoDataClone->GetBinError(i);
      
      TF1* fRand = new TF1("fRand","TMath::Gaus(x,[0],[1],1)",newYield-6.*newError,newYield+6.*newError); // Function to randomise yields in each bin
      fRand->SetNpx(10000);
      fRand->SetParameter(0,newYield);
      fRand->SetParameter(1,newError);

      newYield = fRand->GetRandom();
      if (newYield<0.) newYield = 0.;
      
      histoDataClone2->SetBinContent(i,newYield);
      histoDataClone2->SetBinError(i,newError);
      histoDataClone2->SetLineColor(2);
      histoDataClone2->SetMarkerColor(2);
    }
    
    norm = 1./histoDataClone->Integral();
    histoDataClone->Scale(norm); // Normalise randomised histo to 1
    aDataHistos->Add(histoDataClone2);
  }
  
  // Loop on randomised histos, ratio DATA/MC and fitting function
  TIter nextHisto(aDataHistos);
  histoDataClone = 0x0;
  histoDataClone2 = 0x0;
  Int_t nh(0);
  while ( (histoDataClone = static_cast<TH1*>(nextHisto())) )
  {
    histoDataClone2 = static_cast<TH1*>(histoDataClone->Clone(nh == 0 ? Form("hratio_%s_%s_nominal",partName,collName) : Form("hratio_%s_%s_n%d",partName,collName,nh-1)));
    histoDataClone2->Divide(histoMCClone);
    histoDataClone2->SetLineColor(nh==0 ? 1 : 2);
    histoDataClone2->SetMarkerColor(nh==0 ? 1 : 2);
    histoDataClone2->GetXaxis()->SetTitle("p_{T}");
    histoDataClone2->GetYaxis()->SetTitle("dN/dp_{T}(DATA)/dN/dp_{T}(MC)");
    
    TString fName(nh==0 ? "wFunction_nominal" : Form("wFunction_n%d",nh-1));
    TF1* fWeight(0x0);
    if (polOrder==1) fWeight= new TF1(fName.Data(),Pol1,0.,30.,2);
    else if (polOrder==2) fWeight= new TF1(fName.Data(),Pol2,0.,30.,3);
    fWeight->SetLineColor(nh==0 ? 1 : 2);
    histoDataClone2->Fit(fWeight,"N");
    
    aFunctions->Add(fWeight);
    aRatios->Add(histoDataClone2);
    
    nh++;
  }
  
  // Create canvases with the randomised data and weight functions
  TCanvas* cData = new TCanvas();
  TLegend* lData = new TLegend(0.6002865,0.6919831,0.7679083,0.8565401);
  customiseLegend(*lData);
  nextHisto.Reset();
  nh = 0;
  while ( (histoDataClone = static_cast<TH1*>(nextHisto())) )
  {
    if (nh == 0)
    {
      lData->AddEntry(histoDataClone,"Nominal data","l");
      lData->AddEntry(histoMCClone,"Nominal MC","l");
    }
    histoDataClone->DrawClone("same");
    nh++;
  }
  lData->Draw("same");
  nextHisto.Reset(); //Following lines are just to plot the nominal histo on top of the others
  histoDataClone = static_cast<TH1*>(nextHisto());
  histoDataClone->DrawClone("same");
  histoMCClone->DrawClone("same"); // Draw also the MC distribution
  
  TCanvas* cFuncs = new TCanvas();
  TLegend* lFuncs = new TLegend(0.6002865,0.6919831,0.7679083,0.8565401);
  customiseLegend(*lFuncs);
  TIter nextFunc(aFunctions);
  TF1* fDummy(0x0);
  nh = 0;
  while ( (fDummy = static_cast<TF1*>(nextFunc())) )
  {
    if (nh == 0)
    {
      lFuncs->AddEntry(fDummy,"Nominal","l");
      TH1* hDummy = static_cast<TH1*>((aRatios->At(0))->Clone()); // Get ratio histo for the axes label
      Double_t ymax = hDummy->GetMaximum();
      Double_t ymin = hDummy->GetMinimum();
      hDummy->Reset("ICES");
      hDummy->GetYaxis()->SetRangeUser(ymin-0.3*ymin,ymax+0.3*ymax);
      hDummy->Draw();
    }
    else fDummy->DrawClone("same");
    nh++;
  }
  lFuncs->Draw("same");
  nextFunc.Reset(); //Following lines are just to plot the nominal function on top of the others
  fDummy = static_cast<TF1*>(nextFunc());
  fDummy->DrawClone("same");
  
  TCanvas* cRatios = new TCanvas();
  TLegend* lRatios = new TLegend(0.6002865,0.6919831,0.7679083,0.8565401);
  customiseLegend(*lRatios);
  TIter nextRatio(aRatios);
  nh = 0;
  while ( (histoDataClone = static_cast<TH1*>(nextRatio())) )
  {
    if (nh == 0) lRatios->AddEntry(histoDataClone,"Nominal","l");
    histoDataClone->DrawClone("same");
    nh++;
  }
  lRatios->Draw("same");
  nextRatio.Reset(); //Following lines are just to plot the nominal histo on top of the others
  histoDataClone = static_cast<TH1*>(nextRatio());
  histoDataClone->DrawClone("same");
  
  
  // Save arrays with results to file
  TFile* fSave = new TFile(Form("weights_%s_%s_%s.root",partName,collName,rapName),"RECREATE");
  cData->Write("dataDistributions",TObject::kOverwrite | TObject::kSingleKey);
  cFuncs->Write("weightingFuncs",TObject::kOverwrite | TObject::kSingleKey);
  cRatios->Write("ratiosDataMC",TObject::kOverwrite | TObject::kSingleKey);
  histoMCClone->Write(histoMCClone->GetName(), TObject::kOverwrite | TObject::kSingleKey);
  aDataHistos->Write("hRandData", TObject::kOverwrite | TObject::kSingleKey);
  aRatios->Write("DataOverMC", TObject::kOverwrite | TObject::kSingleKey);
  aFunctions->Write("wFunctions", TObject::kOverwrite | TObject::kSingleKey);
  
  fSave->Close();
  
  // Close files and delete pointers
  fData->Close(); delete fData;
  fMC->Close(); delete fMC;
}