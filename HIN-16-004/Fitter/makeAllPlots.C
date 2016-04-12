#include "results2syst.C"
#include "plotVars.C"
#include "plotResults.C"
#include "Systematics/syst.h"

// flags
const bool doSysts = false;        // compute the systematics
const bool printSysts = true;     // print the systematics summary table
const bool plotMassPlots = false; // not implemented yet
const bool plotAllVars = true;     // plot the dependance of all vars with pt, centrality, y
const bool plotAllResults = true; // and plot the results!
const bool plotSystResults = true;// plot results taking syst fits as nominal too

void makeAllPlots(const char* nominalDir, const char* systDirs="") {
   string allDirs = string(nominalDir) + "," + string(systDirs);

   // first, systematics
   if (doSysts) {
      results2syst(allDirs.c_str(), "syst_PP_fit.csv", "Systematic uncertainty from fitting (PP)", 1, "PP");
      results2syst(allDirs.c_str(), "syst_PbPb_fit.csv", "Systematic uncertainty from fitting (PbPb)", 1, "PbPb");
   }

   // print the systematics table
   if (printSysts) {
      readSyst_all("PP", true, "Systematics/tex/syst_PP_all.tex");
      readSyst_all("PbPb", true, "Systematics/tex/syst_PbPb_all.tex");
   }

   // draw mass plots
   if (plotMassPlots) {
      cout << "Not yet implemented!" << endl;
   }

   // draw plots for all variables
   if (plotAllVars) {
      plotPt(nominalDir,"RFrac2Svs1S",""); 
      plotCent(nominalDir,"RFrac2Svs1S",""); 
      plotRap(nominalDir,"RFrac2Svs1S",""); 
      plotPt(nominalDir,"sigma1_Jpsi",""); 
      plotCent(nominalDir,"sigma1_Jpsi",""); 
      plotRap(nominalDir,"sigma1_Jpsi",""); 
      plotPt(nominalDir,"f_Jpsi",""); 
      plotCent(nominalDir,"f_Jpsi",""); 
      plotRap(nominalDir,"f_Jpsi",""); 
      plotPt(nominalDir,"rSigma21_Jpsi",""); 
      plotCent(nominalDir,"rSigma21_Jpsi",""); 
      plotRap(nominalDir,"rSigma21_Jpsi",""); 
      plotPt(nominalDir,"nparbkg","",false); 
      plotCent(nominalDir,"nparbkg","",false); 
      plotRap(nominalDir,"nparbkg","",false); 

      if (printSysts) {
         plotFiles(allDirs.c_str(), "RFrac2Svs1S", "pt", 0, 1.6, 6.5, 30, 0, 200, "PP");
         plotFiles(allDirs.c_str(), "RFrac2Svs1S", "pt", 1.6, 2.4, 3, 30, 0, 200, "PP");
         plotFiles(allDirs.c_str(), "RFrac2Svs1S", "pt", 0, 1.6, 6.5, 30, 0, 200, "PbPb");
         plotFiles(allDirs.c_str(), "RFrac2Svs1S", "pt", 1.6, 2.4, 3, 30, 0, 200, "PbPb");
         plotFiles(allDirs.c_str(), "RFrac2Svs1S", "cent", 0, 1.6, 6.5, 30, 0, 200, "PbPb");
         plotFiles(allDirs.c_str(), "RFrac2Svs1S", "cent", 1.6, 2.4, 3, 30, 0, 200, "PbPb");
         plotFiles(allDirs.c_str(), "normchi2", "pt", 0, 1.6, 6.5, 30, 0, 200, "PP",false);
         plotFiles(allDirs.c_str(), "normchi2", "pt", 1.6, 2.4, 3, 30, 0, 200, "PP",false);
         plotFiles(allDirs.c_str(), "normchi2", "pt", 0, 1.6, 6.5, 30, 0, 200, "PbPb",false);
         plotFiles(allDirs.c_str(), "normchi2", "pt", 1.6, 2.4, 3, 30, 0, 200, "PbPb",false);
         plotFiles(allDirs.c_str(), "normchi2", "cent", 0, 1.6, 6.5, 30, 0, 200, "PbPb",false);
         plotFiles(allDirs.c_str(), "normchi2", "cent", 1.6, 2.4, 3, 30, 0, 200, "PbPb",false);
      }
   }

   // plot results
   if (plotAllResults) {
      plotPt(nominalDir);
      plotCent(nominalDir);
   }
   if (plotSystResults) {
      TString workDirNamesStr(systDirs);
      TString workDirName; Int_t from = 0;
      while (workDirNamesStr.Tokenize(workDirName, from , ",")) {
         plotPt(workDirName.Data());
         plotCent(workDirName.Data());
      }
   }

   // we're done! and now, a script so that everything is in the right place
   cout << "We're done! Let's copy everything to the note now." << endl << endl << endl;
   cout << "### Copy the lines below to a script ###" << endl << endl;
   cout << "#!/bin/bash" << endl << endl;
   cout << "# settings" << endl;
   cout << "basedir=" << gSystem->pwd() << "/" << endl;
   cout << "notedir=/path/to/the/notes/notes/AN-16-067/trunk/" << endl << endl;

   // copy stuff...
   cout << "cp $basedir/Systematics/tex/*tex $notedir/tables" << endl;
   cout << "cp $basedir/Output/" << nominalDir << "/plot/RESULT/pdf/*pdf $notedir/figures/Results" << endl;
   cout << "cp $basedir/Output/" << nominalDir << "/tex/*tex $notedir/tables/Results" << endl;
   TString workDirNamesStr(systDirs);
   TString workDirName; Int_t from = 0;
   while (workDirNamesStr.Tokenize(workDirName, from , ",")) {
      cout << "mkdir -p $notedir/figures/Results/" << workDirName.Data() << " $notedir/tables/Results/" << workDirName.Data() << endl;
      cout << "cp $basedir/Output/" << workDirName.Data() << "/plot/RESULT/pdf/*pdf $notedir/figures/Results/" << workDirName.Data() << endl;
      cout << "cp $basedir/Output/" << workDirName.Data() << "/tex/*tex $notedir/tables/Results/" << workDirName.Data() << endl;
   }

   cout << endl;
   cout << "echo \"Done copying files. Now do:\"" << endl;
   cout << "echo \"cd  $notedir\"" << endl;
   cout << "echo \"cd ../../..\"" << endl;
   cout << "echo \"eval `notes/tdr runtime -sh`\"" << endl;
   cout << "echo \"cd -\"" << endl;
   cout << "echo \"tdr --style=an b AN-16-067\"" << endl;
   cout << "echo \"cp ../../tmp/AN-16-067_temp.pdf AN-16-067.pdf\"" << endl;
   cout << "echo \"svn commit\"" << endl;
   cout << "echo \"# freeze note; get approved; publish\"" << endl << endl;
   cout << "### Copy the lines above to a script ###" << endl;
}
