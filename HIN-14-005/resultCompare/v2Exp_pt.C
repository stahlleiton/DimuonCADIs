#if !defined(__CINT__) || defined(__MAKECINT__)
#include <iostream>
#include <fstream>

#include <sstream>
#include <string>
#include <vector>

#include <Riostream.h>
#include "TROOT.h"

#include "TCanvas.h"
#include "TColor.h"
#include "TF1.h"
#include "TFile.h"
#include "TH1.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TMath.h"
#include "TSystem.h"
#include "TStyle.h"

#include "dats/data_cms2016.h"
#include "dats/data_others2016.h"

#include "../CMS_lumi.C"
#include "../tdrstyle.C"

#endif

// data sample
const int nData   = 1;

void v2Exp_pt(bool bSavePlots     = true, 
	      float rangeYAxis    = 0.45,
	      float rangeXAxis    = 20,
	      const char* figNamePrefix="v2qn")
{
  gSystem->mkdir(Form("./figs/png"), kTRUE);
  gSystem->mkdir(Form("./figs/pdf"), kTRUE);
  setTDRStyle();
  // gStyle->SetEndErrorSize(1.2);

  float mPion = 0.14;
  float mJpsi = 3.1;
  float mD    = 1.8;

  // low-pt:
  double xBin[]         = {4.5, 8.7};
  double vTwoLowPt[]    = {0.051, 0.065};
  double vTwoLowPtSt[]  = {0.022, 0.024};
  double vTwoLowPtSt2[] = {0.0, 0.0};
  double vTwoLowPtSys[] = {0.023, 0.008};
  double lxerr0[] = {0.0, 0.0};
  double lxerr[] = {.3, .3};
  // for scaling case
  double xBin_nq[]         = {0,0};
  double vTwoLowPt_nq[]    = {0,0};
  double vTwoLowPtSt_nq[]  = {0,0};
  double vTwoLowPtSt2_nq[] = {0.0, 0.0};
  double vTwoLowPtSys_nq[] = {0,0};
  double lxerr0_nq[]       = {0.0, 0.0};
  double lxerr_nq[]        = {0,0};

  TGraphErrors *gV2LowPtPrpCor    = new TGraphErrors(2, xBin, vTwoLowPt, lxerr0, vTwoLowPtSt);
  TGraphErrors *gV2LowPtPrpCor2   = new TGraphErrors(2, xBin, vTwoLowPt, lxerr0, vTwoLowPtSt2);
  TGraphErrors *gV2LowPtPrpCorSys = new TGraphErrors(2, xBin, vTwoLowPt, lxerr, vTwoLowPtSys);

 
  gV2LowPtPrpCor->SetMarkerColor(kViolet+2);
  gV2LowPtPrpCorSys->SetFillColor(kViolet-9);
  gV2LowPtPrpCor->SetMarkerSize(1.1);
  gV2LowPtPrpCor2->SetMarkerSize(1.1);

  gV2LowPtPrpCor->SetMarkerStyle(20);
  gV2LowPtPrpCor2->SetMarkerStyle(24);
  gV2LowPtPrpCor->SetMarkerSize(1.2);
  gV2LowPtPrpCor2->SetMarkerSize(1.2);

  //---- high-pT
  double vTwo[]     = {0.045, 0.081, 0.030};
  double vTwoErr[]  = {0.023, 0.022, 0.021};
  double vTwoErr2[] = {0.0, 0.0, 0.0};
  double vTwoSys[]  = {0.010, 0.009, 0.007};
  double xpTBin2[]  = {7.2, 8.9, 13.2};
  double xerr[]     = {0.0,0.0,0.0};
  double xerrl[]    = {0.7,0.9,3.2};
  double xerrh[]    = {0.8,1.1,16.8};
  double xerrSys[]  = {0.3,0.3,0.3};

  double vTwo_nq[]     = {0.0, 0.0, 0.0};
  double vTwoErr_nq[]  = {0.0, 0.0, 0.0};
  double vTwoErr2_nq[] = {0.0, 0.0, 0.0};
  double vTwoSys_nq[]  = {0.0, 0.0, 0.0};
  double xpTBin2_nq[]  = {0.0, 0.0, 0.0};
  double xerr_nq[]     = {0.0,0.0,0.0};
  double xerrSys_nq[]  = {0.0, 0.0, 0.0};

  TGraphErrors *gV2DefCor    = new TGraphErrors(3, xpTBin2, vTwo, xerr, vTwoErr);
  TGraphErrors *gV2DefCor2   = new TGraphErrors(3, xpTBin2, vTwo, xerr, vTwoErr2);
  TGraphErrors *gV2DefCorSys = new TGraphErrors(3, xpTBin2, vTwo, xerrSys, vTwoSys);

  // -----------------------------------------------------------------------------------------
  // ----- charged hadrons
 double pTChar[] = {0.35155,0.450224,0.549549,0.696161,0.894857, 1.09425,1.38282,1.7545,
		    2.20994,2.71137, 3.21374,3.71682,4.38975,5.41389,6.7541,8.8191,10.853,
		    13.5449,18.2327};
  double v2Char[] = { 0.0372676,0.0497789, 0.0614912,0.0769464,0.0959001,0.112707,0.132255,0.156064,
		      0.177723,0.192246,0.196678, 0.190676,0.172406,0.139361,0.110389, 0.0838311,
		      0.0729596,0.0583707,0.0505057};
  double v2CharSt[] = {3.57E-05,3.59E-05,3.78E-05,2.98E-05,3.64E-05,4.38E-05,4.12E-05,
		       6.33E-05, 9.71E-05,0.000155514,0.000243019,0.000370948,0.000439321,
		       0.00080026,0.00105231,0.00202344,0.00332819,0.00409849,0.00682223};
  double v2CharSt2[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
  double v2CharSys[] = {0.000209266,0.000280117,0.000346661,0.00043495,0.000542674, 0.000640005,
			0.000754443,0.000892962,0.00102104,0.00110518,0.00112907,0.00108984,0.000974753,
			0.000779083,0.000606727,0.000461356,0.000404182,0.000336519,0.000292843};
 double chxerr[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
 double chxerr2[] = {0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2};

 double pTChar_nq[19]    = {0};
 double v2Char_nq[19]    = {0};
 double v2CharSt_nq[19]  = {0};
 double v2CharSt2_nq[19] = {0.0};
 double v2CharSys_nq[19] = {0.};
 double chxerr_nq[19]    = {0.0};
 double chxerr2_nq[19]   = {0.2};

 TGraphErrors *gChar = new TGraphErrors(19, pTChar, v2Char, chxerr, v2CharSt);
 TGraphErrors *gChar2 = new TGraphErrors(19, pTChar, v2Char, chxerr, v2CharSt2);
 TGraphErrors *gCharSys = new TGraphErrors(19, pTChar, v2Char, chxerr2, v2CharSys);

 gChar->SetMarkerStyle(20);
 gChar->SetMarkerColor(kAzure+1);
 gChar->SetLineColor(kAzure+1);
 gChar->SetMarkerSize(1.3);
 gChar2->SetMarkerStyle(24);
 gChar2->SetMarkerColor(kBlue+2);
 gChar2->SetLineColor(kBlue+2);
 gChar2->SetMarkerSize(1.3);

 gCharSys->SetFillColor(kAzure+9);

  //----------- D from ALICE
 

  TGraphErrors *gALdMeson     = new TGraphErrors(6, ALdMesonX, ALdMesonY, ALdMesonXl, ALdMesonYl);
  TGraphErrors *gALdMeson2    = new TGraphErrors(6, ALdMesonX, ALdMesonY, ALdMesonXl, ALdMesonYl);
  TGraphErrors *gALdMeson3    = new TGraphErrors(6, ALdMesonX, ALdMesonY, ALdMesonXerr, ALdMesonXerr);
  TGraphErrors *gALdMesonSys  = new TGraphErrors(6, ALdMesonX, ALdMesonY, ALdMesonXl, ALdMesonSysl);
  TGraphAsymmErrors *gALdMesonSysB = new TGraphAsymmErrors(6, ALdMesonX, ALdMesonY, ALdMesonXl2, ALdMesonXl2, ALdMesonSysBdl, ALdMesonSysBdh);

 if(doQscale)
    {
      for (int ib=0; ib<6;ib++)
	{
	  double mt             = TMath::Sqrt(ALdMesonX[ib]*ALdMesonX[ib]+mD*mD);
	  ALdMesonX_nq[ib]      = (mt-mD)/2;
	  ALdMesonY_nq[ib]      = ALdMesonY[ib]/2;
	  ALdMesonYl_nq[ib]     = ALdMesonYl[ib]/2; 
	  ALdMesonSysl_nq[ib]   = ALdMesonSysl[ib]/2;
	  ALdMesonSysBdl_nq[ib] = ALdMesonSysBdl[ib]/2;
	  ALdMesonSysBdh_nq[ib] = ALdMesonSysBdh[ib]/2;
	  ALdMesonXl_nq[ib]     = ALdMesonXl[ib]/3;
	  ALdMesonXl2_nq[ib]    = ALdMesonXl2[ib]/4;
	  cout<<"high-uncert = "<<ALdMesonSysBdh_nq[ib]<<"\t"<<ALdMesonY_nq[ib]<<endl;
	}
      gALdMeson     = new TGraphErrors(6, ALdMesonX_nq, ALdMesonY_nq, ALdMesonXl_nq, ALdMesonYl_nq);
      gALdMeson2    = new TGraphErrors(6, ALdMesonX_nq, ALdMesonY_nq, ALdMesonXl_nq, ALdMesonYl_nq);
      gALdMeson3    = new TGraphErrors(6, ALdMesonX_nq, ALdMesonY_nq, ALdMesonXerr_nq, ALdMesonXerr_nq);
      gALdMesonSys  = new TGraphErrors(6, ALdMesonX_nq, ALdMesonY_nq, ALdMesonXl_nq, ALdMesonSysl_nq);
      gALdMesonSysB = new TGraphAsymmErrors(6, ALdMesonX_nq, ALdMesonY_nq, ALdMesonXl2_nq, ALdMesonXl2_nq, ALdMesonSysBdl_nq, ALdMesonSysBdh_nq);
    }


  gALdMeson->SetMarkerStyle(27);
  gALdMeson->SetMarkerColor(kGray+2);
  gALdMeson->SetLineColor(kGray+2);
  gALdMeson->SetMarkerSize(1.8);

  gALdMeson3->SetMarkerStyle(33);
  gALdMeson3->SetMarkerColor(kWhite);
  gALdMeson3->SetLineColor(kGray+2);
  gALdMeson3->SetMarkerSize(1.8);
  
  gALdMesonSys->SetFillStyle(0);
  gALdMesonSys->SetMarkerStyle(27);
  gALdMesonSys->SetMarkerColor(kGray+2);
  gALdMesonSys->SetLineColor(kGray+2);
  gALdMesonSys->SetMarkerSize(1.7);

  gALdMesonSysB->SetFillColor(kGray);

  //------------------------------------- inclusive Jpsi from alice
  // ALICE 
  double alc[4]     = {0.04, 0.09, 0.04, 0.01};
  double alcX[4]    = {1.2, 3.0, 4.9, 7.2};
  double alcErr[4]  = {0.04, 0.04, 0.06, 0.095};
  double alcErr2[4] = {0.0, 0.0, 0.0, 0.0};
  double alcSys[4]  = {0.02, 0.02, 0.03, 0.15};
  double xerr3[4]  = {0,0,0,0};
  double xerr4[4]  = {0.25,0.25,0.25,0.25};

  double alc_nq[4]     = {0.};
  double alcX_nq[4]    = {0};
  double alcErr_nq[4]  = {0.};
  double alcErr2_nq[4] = {0.0};
  double alcSys_nq[4]  = {0};
  double xerr3_nq[4]   = {0.0};
  double xerr4_nq[4]   = {0};

  TGraphErrors *gAL    = new TGraphErrors(4, alcX, alc, xerr3, alcErr);
  TGraphErrors *gAL2   = new TGraphErrors(4, alcX, alc, xerr3, alcErr2);
  TGraphErrors *gALSys = new TGraphErrors(4, alcX, alc, xerr4, alcSys);

 if(doQscale)
    {
      for (int ib=0; ib<4;ib++)
	{
	  double mt      = TMath::Sqrt(alcX[ib]*alcX[ib]+mJpsi*mJpsi);
	  alcX_nq[ib]    = (mt-mJpsi)/2;
	  alc_nq[ib]     = alc[ib]/2;
	  alcErr_nq[ib]  = alcErr[ib]/2; 
	  alcErr2_nq[ib] = alcErr2[ib]/2;
	  alcSys_nq[ib]  = alcSys[ib]/2;
	  xerr3_nq[ib]   = xerr3[ib]/2;
	  xerr4_nq[ib]   = xerr4[ib]/4;
	  //  cout<<"Kt = "<<xpTBin2_nq[ib]<<endl;
	}
      gAL    = new TGraphErrors(4, alcX_nq, alc_nq, xerr3_nq, alcErr_nq);
      gAL2   = new TGraphErrors(4, alcX_nq, alc_nq, xerr3_nq, alcErr2_nq);
      gALSys = new TGraphErrors(4, alcX_nq, alc_nq, xerr4_nq, alcSys_nq);
    }

  gAL->SetMarkerStyle(21);
  gAL->SetMarkerColor(kBlack);
  gAL->SetLineColor(kBlack);
  gAL->SetMarkerSize(1.1);
  gAL2->SetMarkerStyle(21);
  gAL2->SetMarkerColor(kBlack);
  gAL2->SetLineColor(kBlack);
  gAL2->SetMarkerSize(1.1);

  gALSys->SetFillColor(1);
  gALSys->SetFillStyle(0);
  gALSys->SetMarkerStyle(21);
  gALSys->SetMarkerColor(kBlack);
  gALSys->SetLineColor(kBlack);
  gALSys->SetMarkerSize(1.1);



  // drawing
 TLatex lx; 
 lx.SetNDC();
 lx.SetTextFont(42);
 lx.SetTextAlign(11);
  //------------------------------------------------------------------------
  // put everything on one plot
 TH1D *phAxis_v2 = new TH1D("phAxis_v2",";p_{T} GeV/c;v_{2}",1,0,rangeXAxis);  
 phAxis_v2->SetDirectory(0);
 phAxis_v2->GetXaxis()->CenterTitle(true);
 phAxis_v2->GetXaxis()->LabelsOption("h");
 phAxis_v2->GetYaxis()->SetRangeUser(-0.05,rangeYAxis); 
 phAxis_v2->GetYaxis()->SetTitleOffset(1.25);
 
 TH1D *phAxis_v2nq = new TH1D("phAxis_v2nq",";(m_{T}-m_{0})/n_{q} [GeV/c^{2}];v_{2}/n_{q}",1,0,6);  
 phAxis_v2nq->SetDirectory(0);
 phAxis_v2nq->GetXaxis()->CenterTitle(true);
 phAxis_v2nq->GetXaxis()->LabelsOption("h");
 phAxis_v2nq->GetYaxis()->SetRangeUser(0.,0.25); 
 phAxis_v2nq->GetYaxis()->SetTitleOffset(1.25);

 TCanvas *pcCombi = new TCanvas("pcCombi","pcCombi");
 gPad->SetRightMargin(0.01);

 if(!doQscale) phAxis_v2->Draw();
 else phAxis_v2nq->Draw();

 
 gALdMesonSysB->Draw("2");
 gALdMesonSys->Draw("2");
 gALdMeson->Draw("pz");

 gCharSys->Draw("2");
 gChar->Draw("pz");
 gChar2->Draw("p");

 gALSys->Draw("2");
  gAL->Draw("PZ");
  gAL2->Draw("P");

 gV2LowPtPrpCorSys->RemovePoint(1);
 gV2LowPtPrpCor->RemovePoint(1);
 gV2LowPtPrpCor2->RemovePoint(1);

 gV2LowPtPrpCorSys->Draw("2");
 gV2LowPtPrpCor->Draw("PZ");
 gV2LowPtPrpCor2->Draw("P");
 
 gV2DefCorSys->Draw("2");
 gV2DefCor->Draw("pz");
 gV2DefCor2->Draw("p");

 // legends ----
 TLegend *legdMJp = new TLegend(0.57, 0.6, 0.71, 0.95);
 legdMJp->SetTextFont(42);
 legdMJp->SetBorderSize(0);
 legdMJp->SetTextSize(0.03);
 legdMJp->SetLineColor(1);
 legdMJp->SetLineStyle(1);
 legdMJp->SetLineWidth(1);
 legdMJp->SetFillColor(19);
 legdMJp->SetFillStyle(0);

  legdMJp->SetTextSize(0.03);
  legdMJp->AddEntry(gV2LowPtPrpCor,"Prompt J/#psi","PL");
  legdMJp->AddEntry(gV2LowPtPrpCor,"   1.6<|y|<2.4, 3<p_{T}<6.5 GeV/c","");
  legdMJp->AddEntry(gV2DefCor,"Prompt J/#psi","PL");
  legdMJp->AddEntry(gV2DefCor,"   |y|<2.4, 6.5<p_{T}<30 GeV/c","");
  legdMJp->AddEntry(gChar,"h^{#pm}, |#eta|<0.8","PL");
  // legdMJp->AddEntry(gChar,"   |#eta|<0.8","");
  legdMJp->AddEntry(gALdMeson,"ALICE Prompt D, 30-50%","P");
  legdMJp->AddEntry(gALdMeson,"   |y|<0.8, 2<p_{T}<16 GeV/c","");
  legdMJp->AddEntry(gAL,"ALICE Inclusive J/#psi, 20-60%","P");
  legdMJp->AddEntry(gAL,"   2.5<y<4.0, p_{T}<10 GeV/c","");
  legdMJp->Draw("same");

  TLatex *lt1 = new TLatex();
  lt1->SetNDC();
  lt1->DrawLatex(0.15,0.9,"CMS Preliminary");
  lt1->DrawLatex(0.15,0.85,"PbPb  #sqrt{s_{NN}} = 2.76 TeV");
  lt1->DrawLatex(0.15,0.8,"Cent. 10-60%");

 gPad->RedrawAxis();
      
 if(bSavePlots)
   {
     pcCombi->SaveAs(Form("%s_nqScale%d.pdf",figNamePrefix,doQscale));
     pcCombi->SaveAs(Form("%s_nqScale%d.png",figNamePrefix,doQscale));  
   }

  
  return;
}

