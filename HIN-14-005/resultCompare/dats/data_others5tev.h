#ifndef DATA_OTHERS5TEV
#define DATA_OTHERS5TEV

// ATLAS data thieving with xyScan from HP2016 plenary talk, Sept2016 : https://indico.cern.ch/event/502239/contributions/2279342/attachments/1341949/2021303/1-2-HP16-AtlasOverviewV1-23092016.pdf, Cent 0-80%, |y|<2

const int nPtBins_atlas = 8;
Double_t atlasRaa_ptErr[nPtBins_atlas]={0};
Double_t atlasRaa_ptErrSyst[nPtBins_atlas]={0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5};

Double_t atlasRaa_pt[nPtBins_atlas]   = { 9.43686, 10.4916, 11.4387, 12.824, 14.8662, 17.6223, 23.2241, 33.9327};
Double_t atlasRaa[nPtBins_atlas]      = { 0.334239, 0.320652, 0.315217, 0.336957, 0.320652, 0.32337, 0.350543, 0.355978};

Double_t atlasRaa_stat[nPtBins_atlas] = { 0.0176311, 0.0189873, 0.0149186, 0.0149186, 0.0162749, 0.0162749, 0.0189873, 0.0447559};
Double_t atlasRaa_syst[nPtBins_atlas] = { 0.0923913, 0.0706522, 0.0611413, 0.0557065, 0.0516304, 0.048913,  0.0516304, 0.048913};
Double_t atlasRaa_glb[nPtBins_atlas]  = { 0.0379747, 0.0352622, 0.0325497, 0.0366184, 0.0352622, 0.0379747, 0.0420434, 0.0406872};

// CMS B RAA, 0-100%, |y|<2.4




#endif
