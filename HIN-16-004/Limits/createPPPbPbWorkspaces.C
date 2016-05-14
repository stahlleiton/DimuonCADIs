#ifndef createPPPbPbWorkspaces_C
#define createPPPbPbWorkspaces_C

#include "TString.h"
#include "TSystem.h"

#include <vector>

#include "../Fitter/Macros/Utilities/resultUtils.h"
#include "combinedWorkspace.C"

using namespace std;

void createPPPbPbWorkspaces(
                   const char* workDirName, // workDirName: usual tag where to look for files in Output
                   bool doSyst=false,
                   const char* DSTag="DATA", // Data Set tag can be: "DATA","MCPSI2SP", "MCJPSIP" ...
                   const char* ACTag="Nominal" // Analysis Case tag (e.g. Nominal fits = "Nominal")
)
{
  // list of files
  vector<TString> theFiles_PbPb = fileList(workDirName,"PbPb",DSTag,"../Fitter");
  vector<TString> theFiles_PP = fileList(workDirName,"PP",DSTag,"../Fitter");
  
  // bin edges
  float ptmin, ptmax, ymin, ymax, centmin, centmax;
  
  int cnt=1;
  for (vector<TString>::const_iterator it_PbPb=theFiles_PbPb.begin(); it_PbPb!=theFiles_PbPb.end(); it_PbPb++)
  {
    cout << endl;
    cout << "############ Merging workspaces for analysis bin " << cnt << " ..." << endl;
    cout << "PbPb workspace " << cnt << " / " << theFiles_PbPb.size() << ": " << *it_PbPb << endl;
    
    for (vector<TString>::const_iterator it_PP=theFiles_PP.begin(); it_PP!=theFiles_PP.end(); it_PP++)
    {
      if ( isSameBinPPPbPb(*it_PbPb, *it_PP) )
      {
        cout << "PP workspace " << cnt << " / " << theFiles_PP.size() << ": " << *it_PP << endl;
        cout << endl;
        
        TString binName(*it_PbPb);
        binName.Remove(0,binName.Last('/')+1);
        binName.Remove(0,binName.Index("_pt")+1);
        binName.Remove(binName.First('.'),binName.Length());
        
        combinedWorkspace(*it_PbPb, *it_PP, Form("combined_PbPbPP_workspace_%s.root",binName.Data()), doSyst, ACTag);
        
        cout << ">>>>>>>> Combined workspace created for bin " << binName.Data() << endl;
      }
      else continue;
    }
    
    cnt++;
  } // loop on the files
}

#endif // #ifndef createPPPbPbWorkspaces_C
