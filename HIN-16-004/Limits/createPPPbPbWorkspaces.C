#ifndef createPPPbPbWorkspaces_C
#define createPPPbPbWorkspaces_C

#include "TString.h"
#include "TSystem.h"

#include <vector>
#include <map>
#include <cstdlib>
#include <iostream>
#include <sstream>

#include "../Fitter/Macros/Utilities/resultUtils.h"
#include "../Fitter/Systematics/syst.h"
#include "combinedWorkspace.C"

using namespace std;

// use the LLR batch system?
const bool usebatch=true;

void createPPPbPbWorkspaces(
                   const char* workDirName, // workDirName: usual tag where to look for files in Output
                   bool doSyst=false,
                   const char* DSTag="DATA", // Data Set tag can be: "DATA","MCPSI2SP", "MCJPSIP" ...
                   const char* ACTag="Nominal", // Analysis Case tag (e.g. Nominal fits = "Nominal")
                   int nCPU=2
)
{
  // list of files
  vector<TString> theFiles_PbPb = fileList(workDirName,"PbPb",DSTag,"../Fitter");
  vector<TString> theFiles_PP = fileList(workDirName,"PP",DSTag,"../Fitter");
  if ( (theFiles_PbPb.size() < 1) || (theFiles_PP.size() < 1) )
  {
    cout << "#[Error]: No files found in " << workDirName << endl;
    return;
  }
  
  map<anabin, syst> syst_PbPb;
  map<anabin, syst> syst_PP;
  map<anabin, syst> syst_PbPb_add;
  if ( doSyst )
  {
    syst_PbPb = readSyst_all("PbPb","../Fitter");
    if ( syst_PbPb.empty() )
    {
      cout << "#[Error]: No PbPb systematics files found" << endl;
      return;
    }
    syst_PbPb_add = readSyst("../Fitter/Systematics/csv/syst_PbPb_bhad_add.csv");
    if ( syst_PbPb_add.empty() )
    {
      cout << "#[Error]: No additive PbPb systematics files found" << endl;
      return;
    }
    
    syst_PP = readSyst_all("PP","../Fitter");
    if ( syst_PP.empty() )
    {
      cout << "#[Error]: No PP systematics files found" << endl;
      return;
    }
  }
  
  int cnt=1;
  for (vector<TString>::const_iterator it_PbPb=theFiles_PbPb.begin(); it_PbPb!=theFiles_PbPb.end(); it_PbPb++)
  {
    cout << endl;
    cout << "############ Merging workspaces for analysis bin " << cnt << " ..." << endl;
    cout << "PbPb workspace " << cnt << " / " << theFiles_PbPb.size() << ": " << *it_PbPb << endl;
    
    double systVal(0.);
    double systValAdd(0.);
    if ( doSyst )
    {
      anabin thebinPbPb = binFromFile(*it_PbPb);
      systVal = syst_PbPb[thebinPbPb].value;
      systValAdd = syst_PbPb_add[thebinPbPb].value;
    }
    
    bool foundPPws = false;
    for (vector<TString>::const_iterator it_PP=theFiles_PP.begin(); it_PP!=theFiles_PP.end(); it_PP++)
    {
      if ( !foundPPws && isSameBinPPPbPb(*it_PbPb, *it_PP) )
      {
        foundPPws = true;
        
        cout << "PP workspace " << cnt << " / " << theFiles_PP.size() << ": " << *it_PP << endl;
        cout << endl;
        
        if ( doSyst )
        {
          anabin thebinPP = binFromFile(*it_PP);
          systVal = sqrt( pow(systVal,2.) + pow(syst_PP[thebinPP].value,2.) );
        }
        
        TString binName(*it_PbPb);
        binName.Remove(0,binName.Last('/')+1);
        binName.Remove(0,binName.Index("_pt")+1);
        binName.Remove(binName.First('.'),binName.Length());
        if ( doSyst ) binName.Prepend("wSyst_");
        else  binName.Prepend("woSyst_");
        
        if (!usebatch) {
           combinedWorkspace(*it_PbPb, *it_PP, Form("combined_PbPbPP_workspace_%s.root",binName.Data()), systVal, systValAdd, ACTag, nCPU);
        } else {
           TString exports;
           exports += Form("export it_PbPb=%s; ",it_PbPb->Data());
           exports += Form("export it_PP=%s; ",it_PP->Data());
           exports += Form("export binName=%s; ", binName.Data());
           exports += Form("export systVal=%f; ", systVal);
           exports += Form("export systValAdd=%f; ", systValAdd);
           exports += Form("export ACTag=%s; ", ACTag);
           exports += Form("export nCPU=%i; ", nCPU);
           exports += Form("export pwd_=%s; ", gSystem->pwd());
           TString command("qsub -k oe -q cms@llrt3 ");
           command += "-N ${binName} ";
           command += "-V ";
           command += Form("-o %s ", gSystem->pwd());
           command += Form("-v it_PbPb,it_PP,binName,systVal,systValAdd,ACTag,nCPU,pwd_ ");
           command += "runbatch_workspace.sh";
           TString command_full = exports + command;
           cout << command_full.Data() << endl;
           system(command_full.Data());
        }
        
        cout << ">>>>>>>> Combined workspace created for bin " << binName.Data() << endl;
      }
      else continue;
    }
    
    if ( !foundPPws ) cout << "# [Error]: No PP workspace found for " << *it_PbPb << endl;
    
    cnt++;
  } // loop on the files
}

#endif // #ifndef createPPPbPbWorkspaces_C
