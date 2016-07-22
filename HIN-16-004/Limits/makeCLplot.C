#include "StandardHypoTestInvDemo.C"

void makeCLplot(const char* filename, int calcType=2) {
   TFile *f = new TFile(filename);
   if (!f) return;

   RooStats::HypoTestInvTool h;
   h.SetParameter("ResultFileName",filename);
   RooStats::HypoTestInverterResult *r = (RooStats::HypoTestInverterResult*) f->Get("result_RFrac2Svs1S_PbPbvsPP_P");
   if (!r) return;
   h.AnalyzeResult(r,calcType,2,false,1000,"toto");
   delete f;
}
