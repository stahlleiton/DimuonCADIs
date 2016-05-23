// -*- C++ -*-
//
// Package:    Fitter
// 
/*
 Description: TTree to RooDataSet converter.
 Implementation:
     This program creates two RooDataSets (opposite-sign and same-sign dimuons) from an Onia Tree.
*/
// Original Author:  Andre Stahl,
//         Created:  Feb 26 19:08 CET 2016
//
//

#include "Utilities/initOniaTree.C"
#include "Utilities/EVENTUTILS.h"
#include "Utilities/initClasses.h"

map<int, double>   fCentMap; // map for centrality-Ncoll mapping
double             fCentBinning[200];
int                fCentBins;

string  findMyTree(string FileName);
bool    getTChain(TChain* fChain, vector<string> FileNames);
void    iniBranch(TChain* fChain,bool isMC=false);
double  deltaR(TLorentzVector* GenMuon, TLorentzVector* RecoMuon);
bool    isMatchedRecoDiMuon(int iRecoDiMuon, double maxDeltaR=0.03);
double  getNColl(int centr, bool isPP);
double  getAccEff(Double_t rapidity,Double_t pt);
void    setCentralityMap(const char* file);
TH2F *AccEffHist = NULL;

bool tree2DataSet(RooWorkspace& Workspace, vector<string> InputFileNames, string DSName, string OutputFileName, bool UpdateDS=false)
{
  RooDataSet* dataOS = NULL; RooDataSet* dataSS = NULL; RooDataSet* dataOSNoBkg = NULL;
  
  bool isMC = false;
  if (DSName.find("MC")!=std::string::npos) isMC =true;
  
  bool isPP = false;
  if (DSName.find("PP")!=std::string::npos) isPP =true;
  int triggerIndex_PP   = 0;
  int triggerIndex_PbPb = 0;
  int CentFactor = 1;

  bool applyWeight = false;
  if (isMC && !isPP) applyWeight = true;
  
  bool isPureSDataset = false;
  if (OutputFileName.find("_PureS")!=std::string::npos) isPureSDataset = true;

  bool applyWeight_AccEff = false;
  if (OutputFileName.find("_AccEff")!=std::string::npos) applyWeight_AccEff = true;
  if(applyWeight == true) applyWeight_AccEff = false;
  
  if (gSystem->AccessPathName(OutputFileName.c_str()) || UpdateDS) {
    cout << "[INFO] Creating " << (isPureSDataset ? "pure signal " : "") << "RooDataSet for " << DSName << endl;
    TreeName = findMyTree(InputFileNames[0]); if(TreeName==""){return false;}

    TChain* theTree = new TChain(TreeName.c_str(),"");
    if(!getTChain(theTree, InputFileNames)){ return false; }     // Import files to TChain
    initOniaTree(theTree);                                       // Initialize the Onia Tree
    iniBranch(theTree,isMC);                                     // Initialize the Branches

    RooRealVar* mass    = new RooRealVar("invMass","#mu#mu mass", 2.0, 5.0, "GeV/c^{2}");
    RooRealVar* ctau    = new RooRealVar("ctau","c_{#tau}", -10.0, 10.0, "cm");
    RooRealVar* ctauErr = new RooRealVar("ctauErr","#sigma_{c#tau}", -1.0, 1.0, "cm");	
    RooRealVar* ptQQ    = new RooRealVar("pt","#mu#mu p_{T}", 0.0, 50.0, "GeV/c");
    RooRealVar* rapQQ   = new RooRealVar("rap","#mu#mu y", -2.4, 2.4, "");
    RooRealVar* cent    = new RooRealVar("cent","centrality", 0.0, 200.0, "");
    RooRealVar* weight  = new RooRealVar("weight","MC weight", 0.0, 10000.0, "");
    RooRealVar* weightAE   = new RooRealVar("weightAE","Data weight with AccEff", 0.0, 10000.0, "");
    RooArgSet*  cols    = NULL;
    
    if (applyWeight || applyWeight_AccEff)
      {
      if(isMC) setCentralityMap(Form("%s/Input/CentralityMap_PbPb2015.txt",gSystem->ExpandPathName(gSystem->pwd())));
      cols   = new RooArgSet(*mass, *ctau, *ctauErr, *ptQQ, *rapQQ, *cent, *weight);
      dataOS = new RooDataSet(Form("dOS_%s", DSName.c_str()), "dOS", *cols, WeightVar(*weight), StoreAsymError(*mass));
      dataSS = new RooDataSet(Form("dSS_%s", DSName.c_str()), "dSS", *cols, WeightVar(*weight), StoreAsymError(*mass));
      if (isPureSDataset) dataOSNoBkg = new RooDataSet(Form("dOS_%s_NoBkg", DSName.c_str()), "dOSNoBkg", *cols, WeightVar(*weight), StoreAsymError(*mass));
      if(applyWeight_AccEff)cout<<"---one applyWeight_AccEff applied---"<<endl;
    }
    else
    {
      cols = new RooArgSet(*mass, *ctau, *ctauErr, *ptQQ, *rapQQ, *cent);                   
      dataOS = new RooDataSet(Form("dOS_%s", DSName.c_str()), "dOS", *cols, StoreAsymError(*mass));
      dataSS = new RooDataSet(Form("dSS_%s", DSName.c_str()), "dSS", *cols, StoreAsymError(*mass));
      if (isMC && isPureSDataset) dataOSNoBkg = new RooDataSet(Form("dOS_%s_NoBkg", DSName.c_str()), "dOSNoBkg", *cols, StoreAsymError(*mass));
    }
    
    TFile *froot = new TFile("Input/f2dTot.root"); 
    
    Long64_t nentries = theTree->GetEntries();
    //nentries = 50000;
    cout << "[INFO] Starting to process " << nentries << " nentries" << endl;
    for (Long64_t jentry=0; jentry<nentries;jentry++) {

      if (jentry%1000000==0) cout << "[INFO] " << jentry << "/" << nentries << endl;
       
      if (theTree->LoadTree(jentry)<0) break;
      if (theTree->GetTreeNumber()!=fCurrent) {
	fCurrent = theTree->GetTreeNumber();
	cout << "[INFO] Processing Root File: " << InputFileNames[fCurrent] << endl;
      }
      Reco_QQ_4mom->Clear();
      Reco_QQ_mumi_4mom->Clear();
      Reco_QQ_mupl_4mom->Clear();
      if (isMC) {
        Gen_QQ_mumi_4mom->Clear();
        Gen_QQ_mupl_4mom->Clear();
      }
      theTree->GetEntry(jentry);  
      
      for (int iQQ=0; iQQ<Reco_QQ_size; iQQ++) {
        TLorentzVector *RecoQQ4mom = (TLorentzVector*) Reco_QQ_4mom->At(iQQ);
        mass->setVal(RecoQQ4mom->M());
        if (theTree->GetBranch("Reco_QQ_ctau3D")) { ctau->setVal(Reco_QQ_ctau3D[iQQ]); }
        else if (theTree->GetBranch("Reco_QQ_ctau")) { ctau->setVal(Reco_QQ_ctau[iQQ]); }
        else { cout << "[ERROR] No ctau information found in the Onia Tree" << endl; }
        if (theTree->GetBranch("Reco_QQ_ctauErr3D")) { ctauErr->setVal(Reco_QQ_ctauErr3D[iQQ]); }
        else if (theTree->GetBranch("Reco_QQ_ctauErr")) { ctauErr->setVal(Reco_QQ_ctauErr[iQQ]); }
        else { cout << "[ERROR] No ctauErr information found in the Onia Tree" << endl; }
        
        ptQQ->setVal(RecoQQ4mom->Pt());
        rapQQ->setVal(RecoQQ4mom->Rapidity());
        cent->setVal(Centrality*CentFactor);
        
        if (applyWeight){
          double w = theTree->GetWeight();//*getNColl(Centrality,isPP);
          weight->setVal(w);
        }
	if (applyWeight_AccEff){
	  if(isPP && fabs(RecoQQ4mom->Rapidity())<1.6){                                     
	    if(RecoQQ4mom->M()<=3.4) AccEffHist = (TH2F*)froot->Get("hjpsitotal2d_midpp"); 
	    if(RecoQQ4mom->M()>3.4) AccEffHist = (TH2F*)froot->Get("hjpsi2total2d_midpp"); 
	  }
	  else if(isPP){ 
	    if(RecoQQ4mom->M()<=3.4) AccEffHist = (TH2F*)froot->Get("hjpsitotal2d_fwdpp");           
	    if(RecoQQ4mom->M()>3.4)  AccEffHist = (TH2F*)froot->Get("hjpsi2total2d_fwdpp");                     
	  }                                                                                                                   
	  if(!isPP && fabs(RecoQQ4mom->Rapidity())<1.6){
	    if(RecoQQ4mom->M()<=3.4) AccEffHist = (TH2F*)froot->Get("hjpsitotal2d_midpbpb");
	    if(RecoQQ4mom->M()>3.4)  AccEffHist = (TH2F*)froot->Get("hjpsi2total2d_midpbpb");                      
	  }                                                                                     
	  else if(!isPP){                                                                                                
	    if(RecoQQ4mom->M()<=3.4) AccEffHist = (TH2F*)froot->Get("hjpsitotal2d_fwdpbpb");              	      
	    if(RecoQQ4mom->M()>3.4)  AccEffHist = (TH2F*)froot->Get("hjpsi2total2d_fwdpbpb");                  
	  }                      
	  
	  
	  double AccEff = getAccEff(RecoQQ4mom->Rapidity(),RecoQQ4mom->Pt());
	  double wAE = 1/AccEff;
	  weight->setVal(wAE);
	}
      
	if (
            ( RecoQQ::areMuonsInAcceptance2015(iQQ) ) &&  // 2015 Global Muon Acceptance Cuts
            ( RecoQQ::passQualityCuts2015(iQQ)      ) &&  // 2015 Soft Global Muon Quality Cuts
	    ( RecoQQ::isTriggerMatch(iQQ, (isPP ? triggerIndex_PP : triggerIndex_PbPb))        )     // HLT_HIL1DoubleMu0_v1
	    )
	  {
	    if (Reco_QQ_sign[iQQ]==0) { // Opposite-Sign dimuons
	      if (isMC && isPureSDataset && isMatchedRecoDiMuon(iQQ)) 
		dataOSNoBkg->add(*cols, (applyWeight ? weight->getVal() : 1.0)); // Signal-only dimuons
	      else
	        dataOS->add(*cols, ( (applyWeight || applyWeight_AccEff) ? weight->getVal() : 1.0)); //Signal and background dimuons
	      
	    }
            else { // Like-Sign dimuons
	      if (!isPureSDataset) dataSS->add(*cols, ( (applyWeight || applyWeight_AccEff) ? weight->getVal() : 1.0));
	    }
	  }
      }
    }
    // Close the TChain and all its pointers
    delete Reco_QQ_4mom; delete Reco_QQ_mumi_4mom; delete Reco_QQ_mupl_4mom; delete Gen_QQ_mumi_4mom; delete Gen_QQ_mupl_4mom;
    theTree->Reset(); delete theTree;
    froot->Close();
    
    // Save all the datasets
    TFile *DBFile = TFile::Open(OutputFileName.c_str(),"RECREATE");
    DBFile->cd();
    if (isMC && isPureSDataset) dataOSNoBkg->Write(Form("dOS_%s_NoBkg", DSName.c_str()));
    else
    {
      dataOS->Write(Form("dOS_%s", DSName.c_str()));
      dataSS->Write(Form("dSS_%s", DSName.c_str()));
    }
    DBFile->Write(); DBFile->Close(); delete DBFile;
  }
  else {

    cout << "[INFO] Loading RooDataSet from " << OutputFileName << endl;
    
    TFile *DBFile = TFile::Open(OutputFileName.c_str(),"READ");
    if (isMC && isPureSDataset) dataOSNoBkg = (RooDataSet*)DBFile->Get(Form("dOS_%s_NoBkg", DSName.c_str()));
    else
    {
      dataOS = (RooDataSet*)DBFile->Get(Form("dOS_%s", DSName.c_str()));
      dataSS = (RooDataSet*)DBFile->Get(Form("dSS_%s", DSName.c_str()));
    }
    DBFile->Close(); delete DBFile;
  }

  // Import datasets to workspace
  if (isMC && isPureSDataset)
  {
    if (!dataOSNoBkg) { cout << "[ERROR] " << DSName << " was not found" << endl; return false; }
    Workspace.import(*dataOSNoBkg);
  }
  else
  {
    if(!dataOS || !dataSS) { cout << "[ERROR] " << DSName << " was not found" << endl; return false; }
    Workspace.import(*dataOS);
    Workspace.import(*dataSS);
  }

  // delete the local datasets
  delete dataSS; delete dataOS; delete dataOSNoBkg;
						   
  return true;
};

string findMyTree(string FileName)
{
  TFile *f = TFile::Open(FileName.c_str(), "READ");
  string name = "";
  if(f->GetListOfKeys()->Contains("hionia")){ name = "hionia/myTree"; }
  else if(f->GetListOfKeys()->Contains("myTree")){ name = "myTree"; }
  else { cout << "[ERROR] myTree was not found in: " << FileName << endl; }
  f->Close(); delete f;
  return name; 
};
  
bool getTChain(TChain *fChain, vector<string> FileNames) 
{
  cout << "[INFO] Extrating TTree " << TreeName.c_str() << endl;
  for (vector<string>::iterator FileName = FileNames.begin() ; FileName != FileNames.end(); ++FileName){
    cout << "[INFO] Adding TFile " << FileName->c_str() << endl;
    fChain->Add(Form("%s/%s", FileName->c_str(),  TreeName.c_str()));
  } 
  if (!fChain) { cout << "[ERROR] fChain was not created, some input files are missing" << endl; return false; } 
  return true;
};

void iniBranch(TChain* fChain, bool isMC)
{
  cout << "[INFO] Initializing Branches of " << TreeName.c_str() << endl;
  if (fChain->GetBranch("Reco_QQ_4mom"))      { fChain->GetBranch("Reco_QQ_4mom")->SetAutoDelete(false);      }
  if (fChain->GetBranch("Reco_QQ_mupl_4mom")) { fChain->GetBranch("Reco_QQ_mupl_4mom")->SetAutoDelete(false); }
  if (fChain->GetBranch("Reco_QQ_mumi_4mom")) { fChain->GetBranch("Reco_QQ_mumi_4mom")->SetAutoDelete(false); } 
  if (isMC) {
    if (fChain->GetBranch("Gen_QQ_mupl_4mom")) { fChain->GetBranch("Gen_QQ_mupl_4mom")->SetAutoDelete(false); }
    if (fChain->GetBranch("Gen_QQ_mumi_4mom")) { fChain->GetBranch("Gen_QQ_mumi_4mom")->SetAutoDelete(false); }
  }
  fChain->SetBranchStatus("*",0);
  RecoQQ::iniBranches(fChain); 
  if (fChain->GetBranch("Centrality"))        { fChain->SetBranchStatus("Centrality",1);        }
  if (fChain->GetBranch("Reco_QQ_size"))      { fChain->SetBranchStatus("Reco_QQ_size",1);      }
  if (fChain->GetBranch("Reco_QQ_sign"))      { fChain->SetBranchStatus("Reco_QQ_sign",1);      }
  if (fChain->GetBranch("Reco_QQ_4mom"))      { fChain->SetBranchStatus("Reco_QQ_4mom",1);      }
  if (fChain->GetBranch("Reco_QQ_mupl_4mom")) { fChain->SetBranchStatus("Reco_QQ_mupl_4mom",1); }
  if (fChain->GetBranch("Reco_QQ_mumi_4mom")) { fChain->SetBranchStatus("Reco_QQ_mumi_4mom",1); }
  if (fChain->GetBranch("Reco_QQ_ctau3D"))    { fChain->SetBranchStatus("Reco_QQ_ctau3D",1);    }
  if (fChain->GetBranch("Reco_QQ_ctauErr3D")) { fChain->SetBranchStatus("Reco_QQ_ctauErr3D",1); }
  if (fChain->GetBranch("Reco_QQ_ctau"))      { fChain->SetBranchStatus("Reco_QQ_ctau",1);      }
  if (fChain->GetBranch("Reco_QQ_ctauErr"))   { fChain->SetBranchStatus("Reco_QQ_ctauErr",1);   }
  if (isMC)
  {
    if (fChain->GetBranch("Gen_QQ_size"))      { fChain->SetBranchStatus("Gen_QQ_size",1);      }
    if (fChain->GetBranch("Gen_QQ_mupl_4mom")) { fChain->SetBranchStatus("Gen_QQ_mupl_4mom",1); }
    if (fChain->GetBranch("Gen_QQ_mumi_4mom")) { fChain->SetBranchStatus("Gen_QQ_mumi_4mom",1); }
  }
};


double deltaR(TLorentzVector* GenMuon, TLorentzVector* RecoMuon)
{
  double dEta = RecoMuon->Eta() - GenMuon->Eta();
  double dPhi = RecoMuon->Phi() - GenMuon->Phi();
  return ((double) TMath::Sqrt( (dEta*dEta) + (dPhi*dPhi) ) );
}


bool isMatchedRecoDiMuon(int iRecoDiMuon, double maxDeltaR)
{
  TLorentzVector* RecoMuonpl = (TLorentzVector*) Reco_QQ_mupl_4mom->At(iRecoDiMuon);
  TLorentzVector* RecoMuonmi = (TLorentzVector*) Reco_QQ_mumi_4mom->At(iRecoDiMuon);
  
  bool isMatched(false);
  int iGenMuon(0);
  while ( !isMatched && (iGenMuon < Gen_QQ_size) )
  {
    TLorentzVector *GenMuonpl = (TLorentzVector*)Gen_QQ_mupl_4mom->At(iGenMuon);
    TLorentzVector *GenMuonmi = (TLorentzVector*)Gen_QQ_mumi_4mom->At(iGenMuon);
    double dRpl = deltaR(GenMuonpl,RecoMuonpl);
    double dRmi = deltaR(GenMuonmi,RecoMuonmi);
    if ( (dRpl < maxDeltaR) && (dRmi < maxDeltaR)  ) isMatched = true;
    iGenMuon++;
  }
  
  return isMatched;
}


double getNColl(int centr, bool isPP)
{
  // Returns the corresponding Ncoll value to the "centr" centrality bin
  
  if ( isPP ) return 1.;
  
  int normCent = TMath::Nint(centr/2.);
  
  int lcent = 0;
  int ucent = 0;
  for ( int i = 0 ; i < fCentBins ; i++ )
  {
    ucent = fCentBinning[i];
    if ( (normCent >= lcent) && (normCent < ucent) ) return fCentMap[ucent];
    else lcent = ucent;
  }
  return 1.;
}


void setCentralityMap(const char* file)
{
  // Creates a mapping between centrality and Ncoll, based on a text file (taken from: https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideHeavyIonCentrality)
  
  if ( strlen(file) > 0 )
  {
    char line[1024];
    ifstream in(file);
    float lcent;
    float ucent;
    float Ncoll;
    
    fCentBins = 0;
    while ( in.getline(line,1024,'\n'))
    {
      sscanf(line,"%f %f %f",&lcent,&ucent,&Ncoll);
      
      fCentMap[ucent] = Ncoll;
      fCentBinning[fCentBins++] = ucent;
    }
    if ( fCentBins == 0 ) std::cout << "[INFO] No centrality map could be defined: The file provided is empty" << std::endl;
    else std::cout << "[INFO] Defining centrality map" << std::endl;
  }
  else
  {
    fCentBins = 0;
    std::cout << "[INFO] No centrality map could be defined: No file provided" << std::endl;
  }
}

double getAccEff(Double_t rapidity, Double_t pt)
{
  
  if (!AccEffHist)
    {
      std::cout << "[INFO] No histogram provided for AccEff weight" << std::endl;
      return 0;
    }
  
  Int_t bin = AccEffHist->FindBin(rapidity, pt);
  Double_t accXeff = AccEffHist->GetBinContent(bin);
  if(accXeff<0.00001) accXeff=1.0;
  
  delete AccEffHist;
  return accXeff;
}
