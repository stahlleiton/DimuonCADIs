// merge the contents of different files containing HypoTestResult for the same bin
// Compile with g++  `root-config --cflags --libs --ldflags` -lRooStats -o hadd_hypotestresult hadd_hypotestresult.C
//
// Usage: ./hadd_hypotestinvresult output.root input1.root input2.root ...


#include "TFile.h"
#include "TString.h"
#include "RooStats/HypoTestResult.h"
#include <iostream>

using namespace RooStats;
using namespace std;

int main(int argc, const char** argv) {
   if (argc<3) {
      cout << "Usage: " << argv[0] << " output.root input1.root input2.root ..." << endl;
      return 1;
   }

   TFile *output = new TFile(argv[1],"RECREATE");
   HypoTestResult *res = NULL;
   TString name;

   for (int i=0; i<argc-2; i++) {
      TFile *f = new TFile(argv[i+2]);
      if (!f) {
         cout << argv[i+2] << " does not exist." << endl;
         continue;
      }
      RooStats::HypoTestResult* r = static_cast<RooStats::HypoTestResult*>(f->FindObjectAny("HypoTestCalculator_result"));
      if (!r) {
         cout << "[ERROR] Could not find HypoTestResult in file "<< argv[i+2] << endl;
         continue;
      }
      if (!res) {
         name = r->GetName();
         output->cd();
         res = new HypoTestResult(*r);
      } else {
         res->Append(r);
      }
      f->Close();
      delete f;
   }

   res->SetName(name);
   output->cd();
   res->Write();
   output->Write();
   output->Close();
}
