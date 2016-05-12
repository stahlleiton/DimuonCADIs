#ifndef computeLimits_C
#define computeLimits_C

#include "TString.h"
#include "TH1D.h"
#include "TLine.h"

#include <vector>

#include "../Fitter/Macros/Utilities/resultUtils.h"
#include "combinedWorkspace.C"
#include "runLimit_RaaNS_Workspace.C"

using namespace std;

void computeLimits(
                   const char* workDirName, // workDirName: usual tag where to look for files in Output
                   double CL=0.683, // Confidence Level for limits computation
                   const char* DSTag="DATA", // Data Set tag can be: "DATA","MCPSI2SP", "MCJPSIP" ...
                   const char* lFileName="cLimits.csv" // file name to save limits results
)
{
  // list of files
  vector<TString> theFiles_PbPb = fileList(workDirName,"PbPb",DSTag,"../Fitter");
  vector<TString> theFiles_PP = fileList(workDirName,"PP",DSTag,"../Fitter");
  
  // bin edges
  float ptmin, ptmax, ymin, ymax, centmin, centmax;
  
  // histo for 1sigma limits checks
  TH1* hCL(0x0);
  if ( CL == 0.683 )
  {
    hCL = new TH1D("hOneSigmaCLComparison","",theFiles_PbPb.size(),0,theFiles_PbPb.size());
    hCL->GetYaxis()->SetTitle("CL_{1#sigma}/#sigma");
    hCL->GetYaxis()->SetTitleOffset(1.15);
    hCL->SetStats(0);
    hCL->SetDirectory(0);
    hCL->SetMarkerColor(1);
    hCL->SetMarkerStyle(20);
    hCL->SetMarkerSize(1);
    hCL->SetLineColor(1);
    
    TLine* l1 = new TLine(0.,1.,hCL->GetXaxis()->GetXmax(),1.);
    l1->SetLineWidth(3);
    
    hCL->GetListOfFunctions()->Add(l1);
  }
  
  // Confidence interval computarion
  string limitsFileName = string("csv/") + lFileName;
  ofstream file(limitsFileName.c_str());
  file << CL*100 << "% " << " confidence limits" << endl;
  
  int cnt=1;
  for (vector<TString>::const_iterator it_PbPb=theFiles_PbPb.begin(); it_PbPb!=theFiles_PbPb.end(); it_PbPb++)
  {
    cout << "Computing limits for analysis bin " << cnt << endl;
    cout << "PbPb workspace " << cnt << " / " << theFiles_PbPb.size() << ": " << *it_PbPb << endl;
    
    for (vector<TString>::const_iterator it_PP=theFiles_PP.begin(); it_PP!=theFiles_PP.end(); it_PP++)
    {
      if ( isSameBinPPPbPb(*it_PbPb, *it_PP) )
      {
        cout << "PP workspace " << cnt << " / " << theFiles_PP.size() << ": " << *it_PP << endl;
        
        double sigmaDoubleR = combinedWorkspace(*it_PbPb, *it_PP,"combined_PbPbPP_workspace.root", -1.);
        
        pair<double,double> limits = runLimit_RaaNS_Workspace("combined_PbPbPP_workspace.root", "RFrac2Svs1S_PbPbvsPP", "simPdf", "wcombo", "dOS_DATA", CL);
        
        anabin thebin = binFromFile(*it_PP);
        ptmin = thebin.ptbin().low();
        ptmax = thebin.ptbin().high();
        ymin = thebin.rapbin().low();
        ymax = thebin.rapbin().high();
        centmin = thebin.centbin().low();
        centmax = thebin.centbin().high();
        
        file << thebin.rapbin().low() << ", " << thebin.rapbin().high() << ", "
        << thebin.ptbin().low() << ", " << thebin.ptbin().high() << ", "
        << thebin.centbin().low() << ", " << thebin.centbin().high() << ", "
        << limits.first << ", " << limits.second << endl;
        
        if ( hCL )
        {
          TString binName(Form("Pt[%.1f,%.1f]-Y[%.1f,%.1f]-C[%.1f,%.1f]",ptmin,ptmax,ymin,ymax,centmin,centmax));
          
          double comp = -1.;
          if ( sigmaDoubleR != 0 ) comp = (limits.second-limits.first)/(2.*sigmaDoubleR);
          hCL->SetBinContent(cnt,comp);
          hCL->GetXaxis()->SetBinLabel(cnt,binName.Data());
        }
      }
      else continue;
    }
    
    cnt++;
  } // loop on the files
  file.close();
  cout << "Closed " << limitsFileName << endl << endl;
  
  if ( hCL )
  {
    TFile* fSave = new TFile("oneSigmaCLComparison.root","RECREATE");
    
    TCanvas* c = new TCanvas("cOneSigmaCLComparison","",90,116,1265,535);
    c->Range(-3.690909,-0.01066472,33.30606,0.01252061);
    c->SetFillColor(0);
    c->SetBorderMode(0);
    c->SetBorderSize(2);
    c->SetRightMargin(0.1163896);
    c->SetTopMargin(0.03732809);
    c->SetBottomMargin(0.1630648);
    c->SetFrameBorderMode(0);
    c->SetFrameBorderMode(0);
    hCL->Draw("p");
    
    c->Write("cOneSigmaCLComparison", TObject::kOverwrite | TObject::kSingleKey);
    fSave->Close(); delete fSave;
  }
}

#endif // #ifndef computeLimits_C
