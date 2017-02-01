{
  bool bSavePlots = true;
  bool bCMSHeavy = false; // NPJpsi + D     @2.76TeV
  bool bHeavy    = false;// NPJpsi + D +D(ALICE)  @2.76TeV
  bool bCMS      = true;// NPJpsi + D + h @2.76TeV
 
//=========Macro generated from canvas: pc/pc
//=========  (Mon Jan 30 17:54:07 2017) by ROOT version5.34/36
   TCanvas *pc = new TCanvas("pc", "pc",0,44,600,600);
   gStyle->SetOptFit(1);
   gStyle->SetOptStat(0);
   gStyle->SetOptTitle(0);
   pc->Range(-80,-0.225,420,1.65);
   pc->SetFillColor(0);
   pc->SetBorderMode(0);
   pc->SetBorderSize(2);
   pc->SetTickx(1);
   pc->SetTicky(1);
   pc->SetLeftMargin(0.16);
   pc->SetRightMargin(0.04);
   pc->SetTopMargin(0.08);
   pc->SetBottomMargin(0.12);
   pc->SetFrameFillStyle(0);
   pc->SetFrameBorderMode(0);
   pc->SetFrameFillStyle(0);
   pc->SetFrameBorderMode(0);
   
   TH1F *phAxis = new TH1F("phAxis","",10,0,400);
   phAxis->SetMinimum(0);
   phAxis->SetMaximum(1.5);
   phAxis->SetLineStyle(0);
   phAxis->SetMarkerStyle(20);
   phAxis->GetXaxis()->SetTitle("<N_{part}>");
   phAxis->GetXaxis()->CenterTitle(true);
   phAxis->GetXaxis()->SetLabelFont(42);
   phAxis->GetXaxis()->SetLabelOffset(0.007);
   phAxis->GetXaxis()->SetLabelSize(0.05);
   phAxis->GetXaxis()->SetTitleSize(0.06);
   phAxis->GetXaxis()->SetTitleOffset(0.9);
   phAxis->GetXaxis()->SetTitleFont(42);
   phAxis->GetYaxis()->SetTitle("R_{AA}");
   phAxis->GetYaxis()->CenterTitle(true);
   phAxis->GetYaxis()->SetLabelFont(42);
   phAxis->GetYaxis()->SetLabelOffset(0.007);
   phAxis->GetYaxis()->SetLabelSize(0.05);
   phAxis->GetYaxis()->SetTitleSize(0.06);
   phAxis->GetYaxis()->SetTitleOffset(1.25);
   phAxis->GetYaxis()->SetTitleFont(42);
   phAxis->GetZaxis()->SetLabelFont(42);
   phAxis->GetZaxis()->SetLabelOffset(0.007);
   phAxis->GetZaxis()->SetLabelSize(0.05);
   phAxis->GetZaxis()->SetTitleSize(0.06);
   phAxis->GetZaxis()->SetTitleFont(42);
   phAxis->Draw("");

   //========================================================================================
   //========================================================================================
   //========================================================================================
   // ***************** CMS nonprompt Jpsi ********************************
   //global uncertianty for the NP_Jpsi
   TBox *box_npJpsi = new TBox(390,0.9505126,400,1.049487);
   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#99ccff");
   box_npJpsi->SetFillColor(kViolet-9);
   box_npJpsi->SetLineWidth(2);
  

   TGraphErrors *gre_npSyst = new TGraphErrors(6);
   gre_npSyst->SetFillColorAlpha(kViolet-9,0.5);
   gre_npSyst->SetMarkerStyle(29);
   gre_npSyst->SetPoint(0,20.0709,0.773602);
   gre_npSyst->SetPointError(0,7,0.1514108);
   gre_npSyst->SetPoint(1,84.2622,0.7406624);
   gre_npSyst->SetPointError(1,7,0.1123886);
   gre_npSyst->SetPoint(2,127.9835,0.5523034);
   gre_npSyst->SetPointError(2,7,0.06598914);
   gre_npSyst->SetPoint(3,185.147,0.5611979);
   gre_npSyst->SetPointError(3,7,0.0806565);
   gre_npSyst->SetPoint(4,259.4178,0.5305178);
   gre_npSyst->SetPointError(4,8,0.07242686);
   gre_npSyst->SetPoint(5,353.3528,0.3627322);
   gre_npSyst->SetPointError(5,8,0.0369654);
   
   TGraphErrors *gre_np = new TGraphErrors(6);
   gre_np->SetMarkerColor(kViolet+3);
   gre_np->SetMarkerStyle(29);
   gre_np->SetMarkerSize(1.8);
   gre_np->SetPoint(0,20.0709,0.773602);
   gre_np->SetPointError(0,0,0.09407875);
   gre_np->SetPoint(1,84.2622,0.7406624);
   gre_np->SetPointError(1,0,0.08984647);
   gre_np->SetPoint(2,127.9835,0.5523034);
   gre_np->SetPointError(2,0,0.05538717);
   gre_np->SetPoint(3,185.147,0.5611979);
   gre_np->SetPointError(3,0,0.04433045);
   gre_np->SetPoint(4,259.4178,0.5305178);
   gre_np->SetPointError(4,0,0.03497956);
   gre_np->SetPoint(5,353.3528,0.3627322);
   gre_np->SetPointError(5,0,0.02196061);


   //========================================================================================
   //========================================================================================
   //========================================================================================
   // *********** alice ALICE D: <Npart> *********** *********** ***********
   //http://hepdata.cedar.ac.uk/view/ins1377363
   //http://arxiv.org/pdf/1506.06604v2.pdf
   // Average D meson RAA (average of D0, D+ and D∗+) in 8<pT<16 GeV/c, |y|<0.8
   const int nNpartBins_alice = 6; 
   double npartBins_alice[nNpartBins_alice]    = {356.0, 260.1,185.8, 128.5, 84.7, 32.2}; 
   double npartBinsErr_alice[nNpartBins_alice] = {.0,.0, .0,.0,.0,0};
   double npartSystX_alice[nNpartBins_alice]   = {7,7,7, 6,6,6};
  
   // D alice RAA _Npart
   double raaAlice_cent_dpt816[nNpartBins_alice]         = {0.15, 0.20, 0.27, 0.32, 0.37, 0.66}; 
   double raaAliceStat_cent_dpt816[nNpartBins_alice]     = {0.01, 0.02, 0.03, 0.04, 0.05, 0.1};// hep data of 5th point seems wrong: 0.01

   double raaAliceSystHigh_cent_dpt816[nNpartBins_alice] = {0.02, 0.03, 0.04, 0.04, 0.05, 0.10};//
   double raaAliceSystLow_cent_dpt816[nNpartBins_alice]  = {0.02, 0.03, 0.04, 0.04, 0.05, 0.11};// hep data 5th and 6th point seem wrong 0.005 and 0.011

   double raaAliceSystCorrHigh_cent_dpt816[nNpartBins_alice]   = {0.03, 0.04, 0.05, 0.06, 0.07, 0.12};
   double raaAliceSystCorrLow_cent_dpt816[nNpartBins_alice]    = {0.03, 0.04, 0.05, 0.06, 0.07, 0.13};

 
  //  TGraphAsymmErrors(n,x,y,exl,exh,eyl,eyh);
  TGraphAsymmErrors *pgAlice_dpt816         = new TGraphAsymmErrors(nNpartBins_alice,
                                                                    npartBins_alice, raaAlice_cent_dpt816,
                                                                    npartBinsErr_alice, npartBinsErr_alice, 
                                                                    raaAliceStat_cent_dpt816,raaAliceStat_cent_dpt816);
  TGraphAsymmErrors *pgAliceSyst_dpt816     = new TGraphAsymmErrors(nNpartBins_alice,
                                                                    npartBins_alice, raaAlice_cent_dpt816, 
                                                                    npartSystX_alice, npartSystX_alice,
                                                                    raaAliceSystLow_cent_dpt816,raaAliceSystHigh_cent_dpt816);
  TGraphAsymmErrors *pgAliceSystCorr_dpt816 = new TGraphAsymmErrors(nNpartBins_alice,
                                                                    npartBins_alice, raaAlice_cent_dpt816,
                                                                    npartSystX_alice, npartSystX_alice,
                                                                    raaAliceSystCorrLow_cent_dpt816,raaAliceSystCorrHigh_cent_dpt816);
  pgAlice_dpt816->SetName("pgAlice_dpt816");
  
  pgAlice_dpt816->SetMarkerStyle(25);
  pgAlice_dpt816->SetMarkerSize(1.2);
  // systmatic boxes
  // uncorrelated, open
  pgAliceSyst_dpt816->SetLineColor(1);
  pgAliceSyst_dpt816->SetFillColor(0);
  pgAliceSyst_dpt816->SetFillStyle(0);

  // correlated, filled
  pgAliceSystCorr_dpt816->SetLineColor(1);
  pgAliceSystCorr_dpt816->SetFillColor(kGray);
 
  //========================================================================================
  //========================================================================================
  //========================================================================================
  // ************* CMS D@2.76Tev vs Npart (PRELIMINARY: HIN-15-005)
  TBox *boxUp_d276 = new TBox(380,1,390,1.244);
  boxUp_d276->SetFillColor(kGreen-9);
  boxUp_d276->SetLineWidth(2);
  TBox *boxDown_d276 = new TBox(380,0.756,390,1);
  boxDown_d276->SetFillColor(kGreen-9);
  boxDown_d276->SetLineWidth(2);
  
  const int nNpartBins_d276 = 6; 
  double npartBins_d276[nNpartBins_d276]    = {22.06,86.23,130.06,187.35,261.49,355.45}; 
  double npartBinsErr_d276[nNpartBins_d276] = {.0,.0, .0,.0,.0,0};
  double npartSystX_d276[nNpartBins_d276]   = {6,6,6, 7,7,7};

  double raaD276_dpt816[nNpartBins_d276]     = {0.687388,0.592072,0.430673,0.400014,0.292332,0.230041};
  double raaD276Stat_dpt816[nNpartBins_d276] = {0.0572567,0.0521664,0.0344595,0.0278675,0.0207364,0.0164228};
  double raaD276Syst_dpt816[nNpartBins_d276] = {0.174531,0.134221,0.105083,0.102842,0.0815442,0.0691289};

  TGraphErrors *pgD276     = new TGraphErrors(nNpartBins_d276,npartBins_d276,raaD276_dpt816, npartBinsErr_d276,raaD276Stat_dpt816);
  TGraphErrors *pgD276Syst = new TGraphErrors(nNpartBins_d276,npartBins_d276,raaD276_dpt816, npartSystX_d276,raaD276Syst_dpt816);

  pgD276->SetMarkerStyle(21);
  pgD276->SetMarkerSize(1.2);
  pgD276->SetMarkerColor(kGreen+3);

  pgD276Syst->SetLineColor(1);
  pgD276Syst->SetFillColorAlpha(kGreen-9,0.5);
   
   //========================================================================================
   //========================================================================================
   //========================================================================================
  // ************* CMS charged hadrons: 1 point, 7.2 < pT < 9.6 GeV, cent [0,5],[5,10],[10,30],[30,50],[50-70],[70,90]]
  TBox *box_h276 = new TBox(370,0.94,380,1.06);
  ci = TColor::GetColor("#ffcc00");
  box_h276->SetFillColor(ci);
  box_h276->SetLineWidth(2);

  const int nNpartBins_h276 = 6; 
  double npartBins_h276[nNpartBins_h276]    = {11.4,42,108,224,329,381}; 
  double npartBinsErr_h276[nNpartBins_h276] = {.0,.0, .0,.0,.0,0};
  double npartSystX_h276[nNpartBins_h276]   = {6,6,6, 7,7,7};

  double raaH276_hpt7296[nNpartBins_h276]     = {0.6418,0.5412,0.3613,0.2358,0.1791,0.1519};
  double raaH276Stat_hpt7296[nNpartBins_h276] = {0.0181,0.0081,0.0041,0.0024,0.0024,0.002};
  double raaH276Syst_hpt7296[nNpartBins_h276] = {0.121926,0.0865283,0.0426506,0.0217406,0.015645,0.0128151};

  TGraphErrors *pgH276     = new TGraphErrors(nNpartBins_h276, npartBins_h276,raaH276_hpt7296, npartBinsErr_h276,raaH276Stat_hpt7296);
  TGraphErrors *pgH276Syst = new TGraphErrors(nNpartBins_h276, npartBins_h276,raaH276_hpt7296, npartSystX_h276,raaH276Syst_hpt7296);

  pgH276->SetMarkerStyle(20);
  pgH276->SetMarkerSize(1.2);
 
  pgH276Syst->SetLineColor(1);
  pgH276Syst->SetFillColor(ci);
  
   //========================================================================================
   //========================================================================================
   //========================================================================================
  // ************ DRAWING ****************
       TLatex *   tex = new TLatex(0.96,0.9424," pp, PbPb #sqrt{s_{NN}} = 2.76 TeV");
     tex->SetNDC();
     tex->SetTextAlign(31);
     tex->SetTextFont(42);
     tex->SetTextSize(0.04);
     tex->SetLineWidth(2);
     tex->Draw();
     tex = new TLatex(0.16,0.9424,"");
     tex->SetNDC();
     tex->SetTextFont(42);
     tex->SetTextSize(0.04);
     tex->SetLineWidth(2);
     tex->Draw();
     tex = new TLatex(0.16,0.9424,"CMS");
     tex->SetNDC();
     tex->SetTextFont(61);
     tex->SetTextSize(0.06);
     tex->SetLineWidth(2);
     tex->Draw();

     //--- beauty legend 
   
     TLegend *leg_beauty = new TLegend(0.25,0.82,0.45,0.89,NULL,"brNDC");
     leg_beauty->SetBorderSize(0);
     leg_beauty->SetTextFont(132);
     leg_beauty->SetTextSize(0.03);
     leg_beauty->SetLineColor(1);
     leg_beauty->SetLineStyle(1);
     leg_beauty->SetLineWidth(1);
     leg_beauty->SetFillColor(19);
     leg_beauty->SetFillStyle(0);
     TLegendEntry *entry=leg_beauty->AddEntry("pgCms","Beauty: nonprompt J/#psi, p_{T} #in [6.5,30] GeV/c","");
     entry->SetLineColor(1);
     entry->SetLineStyle(1);
     entry->SetLineWidth(1);
     entry->SetMarkerColor(1);
     entry->SetMarkerStyle(29);
     entry->SetMarkerSize(1);
     entry->SetTextFont(132);
     entry->SetTextSize(0.04);
     entry=leg_beauty->AddEntry("pgCms","arXiv:1610.00613, |y| < 1.2","p");
     entry->SetLineColor(1);
     entry->SetLineStyle(1);
     entry->SetLineWidth(1);
   
     entry->SetMarkerColor(kViolet+3);
     entry->SetMarkerStyle(29);
     entry->SetMarkerSize(1.8);
     entry->SetTextFont(42);
     entry->SetTextSize(0.035);
    

     //------ charm legend
     TLegend *leg_charm = new TLegend(0.25,0.7,0.45,0.8,NULL,"brNDC");
     leg_charm->SetBorderSize(0);
     leg_charm->SetTextFont(132);
     leg_charm->SetTextSize(0.03);
     leg_charm->SetLineColor(1);
     leg_charm->SetLineStyle(1);
     leg_charm->SetLineWidth(1);
     leg_charm->SetFillColor(19);
     leg_charm->SetFillStyle(0);
     entry=leg_charm->AddEntry("pgAlice_dpt816","Charm: prompt D, p_{T} #in [8,16] GeV/c","");
     entry->SetLineColor(1);
     entry->SetLineStyle(1);
     entry->SetLineWidth(1);
     entry->SetMarkerColor(1);
     entry->SetMarkerStyle(21);
     entry->SetMarkerSize(1);
     entry->SetTextFont(132);
     entry->SetTextSize(0.04);
     entry=leg_charm->AddEntry("pgD276","CMS PAS-HIN-15-005: |y| < 1","p");
     entry->SetLineColor(1);
     entry->SetLineStyle(1);
     entry->SetLineWidth(1);
     entry->SetMarkerColor(kGreen+3);
     entry->SetMarkerStyle(21);
     entry->SetMarkerSize(1.2);
     entry->SetTextFont(42);
     entry->SetTextSize(0.035);

     if(bHeavy)
       {
	 entry=leg_charm->AddEntry("pgAlice_dpt816","ALICE JHEP 11 (2015): |y| < 0.5","p");
	 entry->SetLineColor(1);
	 entry->SetLineStyle(1);
	 entry->SetLineWidth(1);
	 entry->SetMarkerColor(1);
	 entry->SetMarkerStyle(25);
	 entry->SetMarkerSize(1.2);
	 entry->SetTextFont(42);
	 entry->SetTextSize(0.035);
       }
     
   

     //-------- charged hadrons legend
     TLegend *leg_h = new TLegend(0.25,0.61,0.45,0.68,NULL,"brNDC");
     leg_h->SetBorderSize(0);
     leg_h->SetTextFont(132);
     leg_h->SetTextSize(0.03);
     leg_h->SetLineColor(1);
     leg_h->SetLineStyle(1);
     leg_h->SetLineWidth(1);
     leg_h->SetFillColor(19);
     leg_h->SetFillStyle(0);
     TLegendEntry *entry=leg_h->AddEntry("pgCms","Charged hadrons, p_{T} #in [7.2,9.6] GeV/c","");
     entry->SetLineColor(1);
     entry->SetLineStyle(1);
     entry->SetLineWidth(1);
     entry->SetMarkerColor(1);
     entry->SetMarkerStyle(22);
     entry->SetMarkerSize(1.2);
     entry->SetTextFont(132);
     entry->SetTextSize(0.04);
     entry=leg_h->AddEntry("pgCms","EPJC 72 (2012): |y| < 1","p");
     entry->SetLineColor(1);
     entry->SetLineStyle(1);
     entry->SetLineWidth(1);
   
     entry->SetMarkerStyle(20);
     entry->SetMarkerSize(1.2);
     entry->SetTextFont(42);
     entry->SetTextSize(0.035);
    

     
   //===========================================
     leg_beauty->Draw();
     leg_charm->Draw();
     if(bCMSHeavy) // NPJpsi + D     @2.76TeV
       {
	 box_npJpsi->Draw();
	 boxUp_d276->Draw();
	 boxDown_d276->Draw();
	 
	 gre_npSyst->Draw("2");
	 pgD276Syst->Draw("2");
	 
	 gre_np->Draw("p");
	 pgD276->Draw("P");
	 
 
       }
  if(bCMS)// NPJpsi + D + h @2.76TeV
    {
      box_h276->Draw();
      box_npJpsi->Draw();
      boxUp_d276->Draw();
      boxDown_d276->Draw();
      
      pgD276Syst->Draw("2");
      gre_npSyst->Draw("2");
      pgH276Syst->Draw("2");

      gre_np->Draw("p");
      pgD276->Draw("P");
      pgH276->Draw("P");
      
      leg_h->Draw();

    }
  if(bHeavy)// NPJpsi + D +D(ALICE)  @2.76TeV
    {

      box_npJpsi->Draw();
      boxUp_d276->Draw();
      boxDown_d276->Draw();
      
      pgD276Syst->Draw("2");
      gre_npSyst->Draw("2");
  
      gre_np->Draw("p");
      pgD276->Draw("P");
  
      
      pgAliceSystCorr_dpt816->Draw("2");
      pgAliceSyst_dpt816->Draw("2");
      pgAlice_dpt816->Draw("P");

    }
  TLine *line = new TLine(0,1,400,1);
  line->Draw();
  pc->Update();
  
  if(bSavePlots)
  {
    pc->SaveAs(Form("figs/pdf/raaCent_cmsHeavy%d_heavy%d_cms%d.pdf",bCMSHeavy,bHeavy,bCMS));
    pc->SaveAs(Form("figs/png/raaCent_cmsHeavy%d_heavy%d_cms%d.png",bCMSHeavy,bHeavy,bCMS));
  }
  

     
}
