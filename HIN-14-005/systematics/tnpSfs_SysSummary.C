// This is the macro to draw T&P systematics summary for HIN-14-005
// Editor : Dong Ho Moon
//

void tnpSfs_SysSummary(){
  gROOT->Macro("/Users/dmoon/rootlogon.C");
  gStyle->SetPadLeftMargin(0.13);
  gStyle->SetTitleYOffset(1.2);

  double xnpart[] = {2.5, 7.5, 12.5, 17.5, 22.5, 27.5, 32.5, 37.5, 42.5, 47.5, 52.5, 57.5, 80.0};
  double xnpart_err[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
  double xnpart_err2[] = {2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0};
  double xpt[] = {7.0, 8.0, 9.0, 10.25, 12.0, 14.5, 23.0};
  double xpt_err[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
  double xpt_err2[] = {0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5};
  double xrap[] = {0.2, 0.6, 1.0, 1.4, 1.8, 2.2};
  double xrap_err[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
  double xrap_err2[] = {0.05, 0.05, 0.05, 0.05, 0.05, 0.05};
  double eff_npart_prp_pbpb[] = {0.430, 0.464, 0.468, 0.495, 0.490, 0.497, 0.513, 0.511, 0.517, 0.512, 0.529, 0.539, 0.531};
  double eff_npart_nprp_pbpb[] = {0.351, 0.370, 0.375, 0.375, 0.381, 0.376, 0.381, 0.392, 0.399, 0.391, 0.394, 0.394, 0.404};
  double eff_pt_prp_pbpb[] = {0.351, 0.418, 0.497, 0.573, 0.627, 0.675, 0.695};
  double eff_pt_nprp_pbpb[] = {0.283, 0.308, 0.357, 0.403, 0.441, 0.464, 0.466};
  double eff_rap_prp_pbpb[] = {0.494, 0.517, 0.490, 0.468, 0.457, 0.436};
  double eff_rap_nprp_pbpb[] = {0.379, 0.385, 0.390, 0.350, 0.335, 0.312};
  double eff_pt_prp_pp[] = {0.325, 0.417, 0.487, 0.549, 0.589, 0.632, 0.652};
  double eff_pt_nprp_pp[] = {0.320, 0.396, 0.470, 0.526, 0.578, 0.614, 0.629};
  double eff_rap_prp_pp[] = {0.501, 0.534, 0.490, 0.432, 0.420, 0.362};
  double eff_rap_nprp_pp[] = {0.526, 0.549, 0.514, 0.439, 0.417, 0.354};
  double sys_npart_prp_pbpb[] = {0.057, 0.059, 0.059, 0.059, 0.060, 0.062, 0.061, 0.061, 0.062, 0.061, 0.061, 0.062, 0.062};
  double sys_npart_nprp_pbpb[] = {0.021, 0.022, 0.040, 0.046, 0.055, 0.046, 0.055, 0.056, 0.058, 0.057, 0.057, 0.057, 0.057};
  double sys_pt_prp_pbpb[] = {0.083, 0.066, 0.058, 0.052, 0.046, 0.044, 0.044};
  double sys_pt_nprp_pbpb[] = {0.032, 0.065, 0.057, 0.050, 0.045, 0.044, 0.041};
  double sys_rap_prp_pbpb[] = {0.024, 0.026, 0.063, 0.092, 0.082, 0.087};
  double sys_rap_nprp_pbpb[] = {0.022, 0.024, 0.063, 0.092, 0.078, 0.084};
  double sys_pt_prp_pp[] = {0.009, 0.007, 0.006, 0.006, 0.006, 0.006, 0.007};
  double sys_pt_nprp_pp[] = {0.009, 0.007, 0.006, 0.006, 0.006, 0.007, 0.007};
  double sys_rap_prp_pp[] = {0.009, 0.008, 0.007, 0.008, 0.011, 0.022};
  double sys_rap_nprp_pp[] = {0.009, 0.008, 0.007, 0.009, 0.011, 0.021};

  double xlpt[] = {2.75, 5.0, 6.0};
  double xlpt_err[] = {0.0, 0.0, 0.0};
  double xlpt_err2[] = {0.5, 0.5, 0.5};
  double eff_lpt_prp_pbpb[] = {0.139, 0.246, 0.347};
  double eff_lpt_nprp_pbpb[] = {0.107, 0.179, 0.237};
  double sys_lpt_prp_pbpb[] = {0.207, 0.146, 0.112};
  double sys_lpt_nprp_pbpb[] = {0.204, 0.151, 0.116};
  double eff_lpt_prp_pp[] = {0.185, 0.263, 0.326};
  double eff_lpt_nprp_pp[] = {0.178, 0.257, 0.313};
  double sys_lpt_prp_pp[] = {0.063, 0.032, 0.048};
  double sys_lpt_nprp_pp[] = {0.056, 0.031, 0.033};



  TGraphErrors *gEffCent_Prp_PbPb = new TGraphErrors(13, xnpart, eff_npart_prp_pbpb, xnpart_err, xnpart_err);
  TGraphErrors *gEffCent_NPrp_PbPb = new TGraphErrors(13, xnpart, eff_npart_nprp_pbpb, xnpart_err, xnpart_err);
  
  TGraphErrors *gEffCent_Prp_PbPbsys = new TGraphErrors(13, xnpart, eff_npart_prp_pbpb, xnpart_err2, sys_npart_prp_pbpb);
  TGraphErrors *gEffCent_NPrp_PbPbsys = new TGraphErrors(13, xnpart, eff_npart_nprp_pbpb, xnpart_err2, sys_npart_nprp_pbpb);

  TGraphErrors *gEffPt_Prp_PbPb = new TGraphErrors(7, xpt, eff_pt_prp_pbpb, xpt_err, xpt_err);
  TGraphErrors *gEffPt_NPrp_PbPb = new TGraphErrors(7, xpt, eff_pt_nprp_pbpb, xpt_err, xpt_err);

  TGraphErrors *gEffPt_Prp_PbPbsys = new TGraphErrors(7, xpt, eff_pt_prp_pbpb, xpt_err2, sys_pt_prp_pbpb);
  TGraphErrors *gEffPt_NPrp_PbPbsys = new TGraphErrors(7, xpt, eff_pt_nprp_pbpb, xpt_err2, sys_pt_nprp_pbpb);

  TGraphErrors *gEfflPt_Prp_PbPb = new TGraphErrors(3, xlpt, eff_lpt_prp_pbpb, xlpt_err, xlpt_err);
  TGraphErrors *gEfflPt_NPrp_PbPb = new TGraphErrors(3, xlpt, eff_lpt_nprp_pbpb, xlpt_err, xlpt_err);

  TGraphErrors *gEfflPt_Prp_PbPbsys = new TGraphErrors(3, xlpt, eff_lpt_prp_pbpb, xlpt_err2, sys_lpt_prp_pbpb);
  TGraphErrors *gEfflPt_NPrp_PbPbsys = new TGraphErrors(3, xlpt, eff_lpt_nprp_pbpb, xlpt_err2, sys_lpt_nprp_pbpb);


  TGraphErrors *gEffRap_Prp_PbPb = new TGraphErrors(6, xrap, eff_rap_prp_pbpb, xrap_err, xrap_err);
  TGraphErrors *gEffRap_NPrp_PbPb = new TGraphErrors(6, xrap, eff_rap_nprp_pbpb, xrap_err, xrap_err);

  TGraphErrors *gEffRap_Prp_PbPbsys = new TGraphErrors(6, xrap, eff_rap_prp_pbpb, xrap_err2, sys_rap_prp_pbpb);
  TGraphErrors *gEffRap_NPrp_PbPbsys = new TGraphErrors(6, xrap, eff_rap_nprp_pbpb, xrap_err2, sys_rap_nprp_pbpb);

  TGraphErrors *gEffPt_Prp_pp = new TGraphErrors(7, xpt, eff_pt_prp_pp, xpt_err, xpt_err);
  TGraphErrors *gEffPt_NPrp_pp = new TGraphErrors(7, xpt, eff_pt_nprp_pp, xpt_err, xpt_err);

  TGraphErrors *gEffPt_Prp_ppsys = new TGraphErrors(7, xpt, eff_pt_prp_pp, xpt_err2, sys_pt_prp_pp);
  TGraphErrors *gEffPt_NPrp_ppsys = new TGraphErrors(7, xpt, eff_pt_nprp_pp, xpt_err2, sys_pt_nprp_pp);

  TGraphErrors *gEfflPt_Prp_pp = new TGraphErrors(3, xlpt, eff_lpt_prp_pp, xlpt_err, xlpt_err);
  TGraphErrors *gEfflPt_NPrp_pp = new TGraphErrors(3, xlpt, eff_lpt_nprp_pp, xlpt_err, xlpt_err);

  TGraphErrors *gEfflPt_Prp_ppsys = new TGraphErrors(3, xlpt, eff_lpt_prp_pp, xlpt_err2, sys_lpt_prp_pp);
  TGraphErrors *gEfflPt_NPrp_ppsys = new TGraphErrors(3, xlpt, eff_lpt_nprp_pp, xlpt_err2, sys_lpt_nprp_pp);

  TGraphErrors *gEffRap_Prp_pp = new TGraphErrors(6, xrap, eff_rap_prp_pp, xrap_err, xrap_err);
  TGraphErrors *gEffRap_NPrp_pp = new TGraphErrors(6, xrap, eff_rap_nprp_pp, xrap_err, xrap_err);

  TGraphErrors *gEffRap_Prp_ppsys = new TGraphErrors(6, xrap, eff_rap_prp_pp, xrap_err2, sys_rap_prp_pp);
  TGraphErrors *gEffRap_NPrp_ppsys = new TGraphErrors(6, xrap, eff_rap_nprp_pp, xrap_err2, sys_rap_nprp_pp);

  gEffCent_Prp_PbPbsys->SetFillColorAlpha(kAzure-9,0.6);
  gEffCent_NPrp_PbPbsys->SetFillColorAlpha(kRed-9,0.6);

  gEffPt_Prp_PbPbsys->SetFillColorAlpha(kAzure-9,0.6);
  gEffPt_NPrp_PbPbsys->SetFillColorAlpha(kRed-9,0.6);
  gEffPt_Prp_ppsys->SetFillColorAlpha(kPink+1,0.6);
  gEffPt_NPrp_ppsys->SetFillColorAlpha(kGreen-7,0.6);

  gEfflPt_Prp_PbPbsys->SetFillColorAlpha(kBlue-10,0.6);
  gEfflPt_NPrp_PbPbsys->SetFillColorAlpha(kRed-10,0.6);
  gEfflPt_Prp_ppsys->SetFillColorAlpha(kMagenta-10,0.6);
  gEfflPt_NPrp_ppsys->SetFillColorAlpha(kTeal-9,0.6);

  gEffRap_Prp_PbPbsys->SetFillColorAlpha(kAzure-9,0.6);
  gEffRap_NPrp_PbPbsys->SetFillColorAlpha(kRed-9,0.6);
  gEffRap_Prp_ppsys->SetFillColorAlpha(kPink+1,0.6);
  gEffRap_NPrp_ppsys->SetFillColorAlpha(kGreen-7,0.6);

  gEffCent_Prp_PbPb->SetMarkerStyle(20);
  gEffPt_Prp_PbPb->SetMarkerStyle(20);
  gEfflPt_Prp_PbPb->SetMarkerStyle(24);
  gEffRap_Prp_PbPb->SetMarkerStyle(20);

  gEffCent_NPrp_PbPb->SetMarkerStyle(34);
  gEffPt_NPrp_PbPb->SetMarkerStyle(34);
  gEfflPt_NPrp_PbPb->SetMarkerStyle(28);
  gEffRap_NPrp_PbPb->SetMarkerStyle(34);

  gEffCent_NPrp_PbPb->SetMarkerSize(1.4);
  gEffPt_NPrp_PbPb->SetMarkerSize(1.4);
  gEfflPt_NPrp_PbPb->SetMarkerSize(1.4);
  gEffRap_NPrp_PbPb->SetMarkerSize(1.4);
  gEffPt_Prp_pp->SetMarkerSize(1.4);
  gEfflPt_Prp_pp->SetMarkerSize(1.4);
  gEffRap_Prp_pp->SetMarkerSize(1.4);
  gEffPt_NPrp_pp->SetMarkerSize(1.4);
  gEfflPt_NPrp_pp->SetMarkerSize(1.4);
  gEffRap_NPrp_pp->SetMarkerSize(1.4);

  gEffPt_Prp_pp->SetMarkerStyle(33);
  gEfflPt_Prp_pp->SetMarkerStyle(27);
  gEffRap_Prp_pp->SetMarkerStyle(33);

  gEffPt_NPrp_pp->SetMarkerStyle(29);
  gEfflPt_NPrp_pp->SetMarkerStyle(30);
  gEffRap_NPrp_pp->SetMarkerStyle(29);

  gEffCent_Prp_PbPb->SetMarkerColor(kBlue+2);
  gEffCent_NPrp_PbPb->SetMarkerColor(kRed+1);
  gEffPt_Prp_PbPb->SetMarkerColor(kBlue+2);
  gEffPt_NPrp_PbPb->SetMarkerColor(kRed+1);
  gEffPt_Prp_pp->SetMarkerColor(kPink-7);
  gEffPt_NPrp_pp->SetMarkerColor(kTeal+4);
  gEfflPt_Prp_PbPb->SetMarkerColor(kBlue+2);
  gEfflPt_NPrp_PbPb->SetMarkerColor(kRed+1);
  gEfflPt_Prp_pp->SetMarkerColor(kPink-7);
  gEfflPt_NPrp_pp->SetMarkerColor(kTeal+4);
  gEffRap_Prp_PbPb->SetMarkerColor(kBlue+2);
  gEffRap_NPrp_PbPb->SetMarkerColor(kRed+1);
  gEffRap_Prp_pp->SetMarkerColor(kPink-7);
  gEffRap_NPrp_pp->SetMarkerColor(kTeal+4);

  gEffCent_Prp_PbPb->SetLineColor(kBlue+2);
  gEffCent_NPrp_PbPb->SetLineColor(kRed+1);
  gEffPt_Prp_PbPb->SetLineColor(kBlue+2);
  gEffPt_NPrp_PbPb->SetLineColor(kRed+1);
  gEffPt_Prp_pp->SetLineColor(kRed+1);
  gEffPt_NPrp_pp->SetLineColor(kTeal+4);
  gEfflPt_Prp_PbPb->SetLineColor(kBlue+2);
  gEfflPt_NPrp_PbPb->SetLineColor(kRed+1);
  gEfflPt_Prp_pp->SetLineColor(kRed+1);
  gEfflPt_NPrp_pp->SetLineColor(kTeal+4);
  gEffRap_Prp_PbPb->SetLineColor(kBlue+2);
  gEffRap_NPrp_PbPb->SetLineColor(kRed+1);
  gEffRap_Prp_pp->SetLineColor(kRed+1);
  gEffRap_NPrp_pp->SetLineColor(kTeal+4);


  TH1F *hPadCent = new TH1F("hPadCent",";Centrality (%);Efficiency",10,0,100);
  hPadCent->GetXaxis()->SetLabelSize(20);
  hPadCent->GetXaxis()->SetLabelFont(43);
  hPadCent->GetXaxis()->SetTitleSize(24);
  hPadCent->GetXaxis()->SetTitleFont(43);
  hPadCent->GetXaxis()->SetTitleOffset(1.0);
  hPadCent->GetXaxis()->CenterTitle();
  hPadCent->GetYaxis()->CenterTitle();

  TH1F *hPadPt = new TH1F("hPadPt",";p_{T} (GeV/c);Efficiency",10,0,30);
  hPadPt->GetXaxis()->SetLabelSize(20);
  hPadPt->GetXaxis()->SetLabelFont(43);
  hPadPt->GetXaxis()->SetTitleSize(24);
  hPadPt->GetXaxis()->SetTitleFont(43);
  hPadPt->GetXaxis()->SetTitleOffset(1.0);
  hPadPt->GetXaxis()->CenterTitle();
  hPadPt->GetYaxis()->CenterTitle();

  TH1F *hPadRap = new TH1F("hPadRap",";|y|;Efficiency",10,0,2.4);
  hPadRap->GetXaxis()->SetLabelSize(20);
  hPadRap->GetXaxis()->SetLabelFont(43);
  hPadRap->GetXaxis()->SetTitleSize(24);
  hPadRap->GetXaxis()->SetTitleFont(43);
  hPadRap->GetXaxis()->SetTitleOffset(1.0);
  hPadRap->GetXaxis()->CenterTitle();
  hPadRap->GetYaxis()->CenterTitle();

  TLatex *lt1 = new TLatex();
  lt1->SetNDC();

  TLegend *leg1 = new TLegend(0.16,0.75,0.36,0.85);
  leg1->SetFillStyle(0);
  leg1->SetFillColor(0);
  leg1->SetBorderSize(0);
  leg1->SetTextSize(0.035);
  leg1->AddEntry(gEffCent_Prp_PbPb,"PbPb Prompt J/#psi","PL");
  leg1->AddEntry(gEffCent_NPrp_PbPb,"PbPb Non-prompt J/#psi","PL");

  TLegend *leg2 = new TLegend(0.16,0.66,0.36,0.85);
  //TLegend *leg2 = new TLegend(0.40,0.26,0.55,0.45);
  leg2->SetFillStyle(0);
  leg2->SetFillColor(0);
  leg2->SetBorderSize(0);
  leg2->SetTextSize(0.035);
  leg2->AddEntry(gEffPt_Prp_PbPb,"PbPb Prompt J/#psi","PL");
  leg2->AddEntry(gEffPt_NPrp_PbPb,"PbPb Non-prompt J/#psi","PL");
  leg2->AddEntry(gEffPt_Prp_pp,"pp Prompt J/#psi","PL");
  leg2->AddEntry(gEffPt_NPrp_pp,"pp Non-prompt J/#psi","PL");

  TLegend *leg3 = new TLegend(0.16,0.76,0.36,0.85);
  //TLegend *leg3 = new TLegend(0.40,0.26,0.55,0.45);
  leg3->SetFillStyle(0);
  leg3->SetFillColor(0);
  leg3->SetBorderSize(0);
  leg3->SetTextSize(0.035);
  leg3->AddEntry(gEffPt_Prp_PbPb,"PbPb Prompt J/#psi","PL");
  leg3->AddEntry(gEffPt_NPrp_PbPb,"PbPb Non-prompt J/#psi","PL");

  TLegend *leg4 = new TLegend(0.16,0.76,0.36,0.85);
  //TLegend *leg4 = new TLegend(0.40,0.26,0.55,0.45);
  leg4->SetFillStyle(0);
  leg4->SetFillColor(0);
  leg4->SetBorderSize(0);
  leg4->SetTextSize(0.035);
  leg4->AddEntry(gEffPt_Prp_pp,"pp Prompt J/#psi","PL");
  leg4->AddEntry(gEffPt_NPrp_pp,"pp Non-prompt J/#psi","PL");

  TLegend *leg5 = new TLegend(0.56,0.26,0.71,0.45);
  leg5->SetFillStyle(0);
  leg5->SetFillColor(0);
  leg5->SetBorderSize(0);
  leg5->SetTextSize(0.035);
  leg5->SetHeader("1.6<|y|<2.4");
  leg5->AddEntry(gEfflPt_Prp_pp,"pp Prompt J/#psi","PL");
  leg5->AddEntry(gEfflPt_NPrp_pp,"pp Non-prompt J/#psi","PL");

  TLegend *leg6 = new TLegend(0.56,0.26,0.71,0.45);
  leg6->SetFillStyle(0);
  leg6->SetFillColor(0);
  leg6->SetBorderSize(0);
  leg6->SetTextSize(0.035);
  leg6->SetHeader("1.6<|y|<2.4");
  leg6->AddEntry(gEfflPt_Prp_PbPb,"PbPb Prompt J/#psi","PL");
  leg6->AddEntry(gEfflPt_NPrp_PbPb,"PbPb Non-prompt J/#psi","PL");

  TCanvas *c1 = new TCanvas("c1","",550,500);
  hPadCent->Draw();
  gEffCent_Prp_PbPbsys->Draw("2");
  gEffCent_NPrp_PbPbsys->Draw("2");
  gEffCent_Prp_PbPb->Draw("pz");
  gEffCent_NPrp_PbPb->Draw("pz");

  lt1->SetTextSize(0.045);
  lt1->DrawLatex(0.16,0.89,"CMS Simulation");
  //lt1->DrawLatex(0.2,0.82,"PbPb  #sqrt{s_{NN}} = 2.76 TeV");
  lt1->SetTextSize(0.04);
  lt1->DrawLatex(0.62, 0.89, "6.5 < p_{T} < 30 (GeV/c)");
  //lt1->DrawLatex(0.62, 0.88, "Cent. 0 - 100 %");
  lt1->DrawLatex(0.16,0.16,"PYTHIA+EvtGen+HYDJET");
  lt1->DrawLatex(0.62, 0.82, "|y| < 2.4");

  leg1->Draw();

  c1->SaveAs("eff_sfs_sys_comp_cent.png");
  c1->SaveAs("eff_sfs_sys_comp_cent.pdf");

  hPadPt->Draw();
  gEffPt_Prp_PbPbsys->SetFillColorAlpha(kAzure-9,0.6);
  gEffPt_NPrp_PbPbsys->SetFillColorAlpha(kRed-9,0.6);
  gEffPt_Prp_PbPbsys->Draw("2");
  gEffPt_NPrp_PbPbsys->Draw("2");
  gEffPt_Prp_PbPb->Draw("pz");
  gEffPt_NPrp_PbPb->Draw("pz");

  gEfflPt_Prp_PbPbsys->SetFillColorAlpha(kBlue-10,0.6);
  gEfflPt_NPrp_PbPbsys->SetFillColorAlpha(kRed-10,0.6);
  gEfflPt_Prp_PbPbsys->Draw("2");
  gEfflPt_NPrp_PbPbsys->Draw("2");
  gEfflPt_Prp_PbPb->Draw("pz");
  gEfflPt_NPrp_PbPb->Draw("pz");


  lt1->SetTextSize(0.045);
  lt1->DrawLatex(0.16,0.89,"CMS Simulation");
  //lt1->DrawLatex(0.2,0.82,"PbPb  #sqrt{s_{NN}} = 2.76 TeV");
  lt1->SetTextSize(0.04);
  //lt1->DrawLatex(0.62, 0.88, "6.5 < p_{T} < 30 (GeV/c)");
  lt1->DrawLatex(0.62, 0.89, "Cent. 0 - 100 %");
  lt1->DrawLatex(0.57,0.16,"PYTHIA+EvtGen+HYDJET");
  lt1->DrawLatex(0.62, 0.82, "|y| < 2.4");

  leg3->Draw();
  leg6->Draw();

  c1->SaveAs("eff_sfs_sys_comp_pT_pbpb.png");
  c1->SaveAs("eff_sfs_sys_comp_pT_pbpb.pdf");

  hPadPt->Draw();
  gEffPt_Prp_PbPbsys->Draw("2");
  gEffPt_Prp_PbPb->Draw("pz");

  gEfflPt_Prp_PbPbsys->Draw("2");
  gEfflPt_Prp_PbPb->Draw("pz");


  lt1->SetTextSize(0.045);
  lt1->DrawLatex(0.16,0.89,"CMS Simulation");
  lt1->DrawLatex(0.16,0.82,"Prompt J/#psi");
  //lt1->DrawLatex(0.2,0.82,"PbPb  #sqrt{s_{NN}} = 2.76 TeV");
  lt1->SetTextSize(0.04);
  //lt1->DrawLatex(0.62, 0.88, "6.5 < p_{T} < 30 (GeV/c)");
  lt1->DrawLatex(0.62, 0.89, "Cent. 0 - 100 %");
  lt1->DrawLatex(0.57,0.16,"PYTHIA+EvtGen+HYDJET");
  lt1->DrawLatex(0.62, 0.82, "|y| < 2.4");

  //leg3->Draw();

  c1->SaveAs("eff_sfs_sys_comp_pT_pbpb_only_prp.png");
  c1->SaveAs("eff_sfs_sys_comp_pT_pbpb_only_prp.pdf");

  hPadPt->Draw();
  gEffPt_NPrp_PbPbsys->Draw("2");
  gEffPt_NPrp_PbPb->Draw("pz");

  gEfflPt_NPrp_PbPbsys->Draw("2");
  gEfflPt_NPrp_PbPb->Draw("pz");


  lt1->SetTextSize(0.045);
  lt1->DrawLatex(0.16,0.89,"CMS Simulation");
  lt1->DrawLatex(0.16,0.82,"Non-Prompt J/#psi");
  //lt1->DrawLatex(0.2,0.82,"PbPb  #sqrt{s_{NN}} = 2.76 TeV");
  lt1->SetTextSize(0.04);
  //lt1->DrawLatex(0.62, 0.88, "6.5 < p_{T} < 30 (GeV/c)");
  lt1->DrawLatex(0.62, 0.89, "Cent. 0 - 100 %");
  lt1->DrawLatex(0.57,0.16,"PYTHIA+EvtGen+HYDJET");
  lt1->DrawLatex(0.62, 0.82, "|y| < 2.4");


  c1->SaveAs("eff_sfs_sys_comp_pT_pbpb_only_nprp.png");
  c1->SaveAs("eff_sfs_sys_comp_pT_pbpb_only_nprp.pdf");


  hPadPt->Draw();
  gEffPt_Prp_ppsys->Draw("2");
  gEffPt_NPrp_ppsys->Draw("2");
  gEffPt_Prp_pp->Draw("pz");
  gEffPt_NPrp_pp->Draw("pz");
  gEfflPt_Prp_ppsys->Draw("2");
  gEfflPt_NPrp_ppsys->Draw("2");
  gEfflPt_Prp_pp->Draw("pz");
  gEfflPt_NPrp_pp->Draw("pz");

  lt1->SetTextSize(0.045);
  lt1->DrawLatex(0.16,0.89,"CMS Simulation");
  //lt1->DrawLatex(0.2,0.82,"PbPb  #sqrt{s_{NN}} = 2.76 TeV");
  lt1->SetTextSize(0.04);
  //lt1->DrawLatex(0.62, 0.88, "6.5 < p_{T} < 30 (GeV/c)");
  lt1->DrawLatex(0.62, 0.89, "Cent. 0 - 100 %");
  lt1->DrawLatex(0.16,0.16,"PYTHIA+EvtGen");
  lt1->DrawLatex(0.62, 0.82, "|y| < 2.4");

  leg4->Draw();
  leg5->Draw();

  c1->SaveAs("eff_sfs_sys_comp_pT_pp.png");
  c1->SaveAs("eff_sfs_sys_comp_pT_pp.pdf");

  
  hPadRap->Draw();
  gEffRap_Prp_PbPbsys->SetFillColorAlpha(kAzure-9,0.6);
  gEffRap_NPrp_PbPbsys->SetFillColorAlpha(kRed-9,0.6);
  gEffRap_Prp_PbPbsys->Draw("2");
  gEffRap_NPrp_PbPbsys->Draw("2");
  gEffRap_Prp_PbPb->Draw("pz");
  gEffRap_NPrp_PbPb->Draw("pz");

  lt1->SetTextSize(0.045);
  lt1->DrawLatex(0.16,0.89,"CMS Simulation");
  //lt1->DrawLatex(0.2,0.82,"PbPb  #sqrt{s_{NN}} = 2.76 TeV");
  lt1->SetTextSize(0.04);
  lt1->DrawLatex(0.62, 0.89, "Cent. 0 - 100 %");
  lt1->DrawLatex(0.62, 0.82, "6.5 < p_{T} < 30 (GeV/c)");
  lt1->DrawLatex(0.16,0.16,"PYTHIA+EvtGen+HYDJET");
  //lt1->DrawLatex(0.62, 0.82, "|y| < 2.4");

  leg3->Draw();

  c1->SaveAs("eff_sfs_sys_comp_rap_pbpb.png");
  c1->SaveAs("eff_sfs_sys_comp_rap_pbpb.pdf");

  hPadRap->Draw();
  gEffRap_Prp_PbPbsys->Draw("2");
  gEffRap_Prp_PbPb->Draw("pz");

  lt1->SetTextSize(0.045);
  lt1->DrawLatex(0.16,0.89,"CMS Simulation");
  lt1->DrawLatex(0.16,0.82,"Prompt J/#psi");
  //lt1->DrawLatex(0.2,0.82,"PbPb  #sqrt{s_{NN}} = 2.76 TeV");
  lt1->SetTextSize(0.04);
  lt1->DrawLatex(0.62, 0.89, "Cent. 0 - 100 %");
  lt1->DrawLatex(0.62, 0.82, "6.5 < p_{T} < 30 (GeV/c)");
  lt1->DrawLatex(0.16,0.16,"PYTHIA+EvtGen+HYDJET");
  //lt1->DrawLatex(0.62, 0.82, "|y| < 2.4");


  c1->SaveAs("eff_sfs_sys_comp_rap_pbpb_prp.png");
  c1->SaveAs("eff_sfs_sys_comp_rap_pbpb_prp.pdf");

  hPadRap->Draw();
  gEffRap_NPrp_PbPbsys->Draw("2");
  gEffRap_NPrp_PbPb->Draw("pz");

  lt1->SetTextSize(0.045);
  lt1->DrawLatex(0.16,0.89,"CMS Simulation");
  lt1->DrawLatex(0.16,0.82,"Non-Prompt J/#psi");
  //lt1->DrawLatex(0.2,0.82,"PbPb  #sqrt{s_{NN}} = 2.76 TeV");
  lt1->SetTextSize(0.04);
  lt1->DrawLatex(0.62, 0.89, "Cent. 0 - 100 %");
  lt1->DrawLatex(0.62, 0.82, "6.5 < p_{T} < 30 (GeV/c)");
  lt1->DrawLatex(0.16,0.16,"PYTHIA+EvtGen+HYDJET");
  //lt1->DrawLatex(0.62, 0.82, "|y| < 2.4");


  c1->SaveAs("eff_sfs_sys_comp_rap_pbpb_nprp.png");
  c1->SaveAs("eff_sfs_sys_comp_rap_pbpb_nprp.pdf");


  
  hPadRap->Draw();
  gEffRap_Prp_ppsys->Draw("2");
  gEffRap_NPrp_ppsys->Draw("2");
  gEffRap_Prp_pp->Draw("pz");
  gEffRap_NPrp_pp->Draw("pz");

  lt1->SetTextSize(0.045);
  lt1->DrawLatex(0.16,0.89,"CMS Simulation");
  //lt1->DrawLatex(0.2,0.82,"PbPb  #sqrt{s_{NN}} = 2.76 TeV");
  lt1->SetTextSize(0.04);
  lt1->DrawLatex(0.62, 0.89, "Cent. 0 - 100 %");
  lt1->DrawLatex(0.62, 0.82, "6.5 < p_{T} < 30 (GeV/c)");
  lt1->DrawLatex(0.16,0.16,"PYTHIA+EvtGen");
  //lt1->DrawLatex(0.62, 0.82, "|y| < 2.4");

  leg4->Draw();

  c1->SaveAs("eff_sfs_sys_comp_rap_pp.png");
  c1->SaveAs("eff_sfs_sys_comp_rap_pp.pdf");


}
