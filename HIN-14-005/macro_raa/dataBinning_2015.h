#ifndef DATABINNING_2015
#define DATABINNING_2015

double nMbEvents = 1161.4982598;
double ppLumi    = 5400;
int npartShift=2;

/// ------------------------------ binning 
// 50-100, 50-40, 40-30,30-20, 20-10, 10-0
const unsigned int nBinsNpart6    = 6;
double binsNpart6[nBinsNpart6]    = {22.0709, 86.2622, 129.9835, 187.1470, 261.4178, 355.3528};
double binsNpart6Err[nBinsNpart6] = {0,0,0,0,0,0};
double binsNpart6X[nBinsNpart6]   = {7,7,7,7,8,8};

double binsNpart6_shiftMinus[nBinsNpart6] = {binsNpart6[0]-npartShift,binsNpart6[1]-npartShift,binsNpart6[2]-npartShift,
					     binsNpart6[3]-npartShift,binsNpart6[4]-npartShift,binsNpart6[5]-npartShift};
double binsNpart6_shiftPlus[nBinsNpart6]  = {binsNpart6[0]+npartShift,binsNpart6[1]+npartShift,binsNpart6[2]+npartShift,
					     binsNpart6[3]+npartShift,binsNpart6[4]+npartShift,binsNpart6[5]+npartShift};

double adTaa6[nBinsNpart6]       = {0.47, 2.75, 5.09, 8.8, 14.48, 23.2};
double adTaa6Err[nBinsNpart6]    = {0.07, 0.3, 0.43, 0.58, 0.76,  0.99};
double adDeltaCent6[nBinsNpart6] = {0.5,  0.1, 0.1,  0.1,  0.1,   0.1};

// 0-5,5-10,10-15,15-20,20-25,25-30,30-35,35-40,40-45,45-50,50-60,60-100 (in reversed: first is 60-100%)
const unsigned int nBinsNpart12  = 12;
double binsNpart12[nBinsNpart12] = {14.19, 53.53, 76.68, 95.78, 117.54, 142.58, 171.08, 203.62, 240.31, 282.67, 329.48,381.41};
double binsNpart12Err[nBinsNpart12] = {0,0,0,0,0,0,0,0,0,0,0,0};
//double binsNpart12X[nBinsNpart12]   = {10,10,10,10,10,10,10,10,10,10,10,10};
double binsNpart12X[nBinsNpart12]   = {7,7,7,7,7,7,7,7,7,8,8,8};


double adTaa12[nBinsNpart12]  = {0.25, 1.36, 2.29, 3.2, 4.36, 5.83, 7.66, 9.94, 12.73, 16.23, 20.48, 25.91};
double adTaa12Err[nBinsNpart12] = {0.04, 0.19, 0.26, 0.33, 0.39, 0.47, 0.53, 0.64, 0.72, 0.8, 0.94, 1.06};
double adDeltaCent12[nBinsNpart12]= {0.4, 0.1, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05};

// 0-100 bin
const unsigned int nBinsMB         = 1;
const unsigned int nBinsMB3        = 3;
double binsNpartMB[nBinsMB]        = {113.0518};
double binsNpartMB3[nBinsMB3]      = {113.0518, 113.0518,113.0518};
double binsNpartMBErr[nBinsMB]     = {0};
double binsNpartMB3Err[nBinsMB3]   = {0,0,0};

double binsPtMB[nBinsMB]     = {10};
double binsPtMBErr[nBinsMB]  = {0};
double binsPtMBX[nBinsMB]    = {0.05};

double binsYMB[nBinsMB]     = {1.2};
double binsYMBErr[nBinsMB]  = {0};
double binsYMBX[nBinsMB]    = {0.1};

double adTaaMB[nBinsMB]       = {5.67};
double adTaaMBErr[nBinsMB]    = {0.32};
double adDeltaCentMB[nBinsMB] = {1};

// pt:
const unsigned int nBinsPt    = 6;
const unsigned int nBinsPt3   = 3;
// <pT> values are written here
//double binsPt[nBinsPt]        = {7.34, 8.96, 10.16, 11.88, 14.23, 19.74}; // pt>6.5 first bin, 0--2.4
//double binsPt3[nBinsPt3]      = {3.73, 4.96, 5.95}; // pt 3-6.5 in fwd region
//double binsPt3_mb[nBinsPt3]   = {4.5, 10.0, 15}; // mb in the fwd region
// middle pT of each given bin are written here
double binsPt[nBinsPt]        = {7.5, 9.0, 10.25, 12.0, 14.5, 23.0}; // pt>6.5 first bin, 0--2.4
double binsPt3[nBinsPt3]      = {3.75, 5.0, 6.0}; // pt 3-6.5 in fwd region
double binsPt3_mb[nBinsPt3]   = {4.5, 10.0, 15}; // mb in the fwd region
double binsPtErr[nBinsPt]     = {1.0, 0.5, 0.75, 1.0, 1.5, 7};
double binsPt3Err[nBinsPt3]   = {0.75, 0.5, 0.5}; 
double binsPt3Err_mb[nBinsPt3]= {0,0,0};
double binsPtX[nBinsPt]     = {0.5,0.5,0.5,0.5,0.5,0.5};
double binsPt3X[nBinsPt3]   = {0.4,0.4,0.4}; 
double binsPt3X_mb[nBinsPt3]= {0.4,0.4,0.4};

// rapidity:
const unsigned int nBinsY       = 6;
const unsigned int nBinsY3      = 3;
// <|y|> values are written here
//double binsY[nBinsY]   = {0.20, 0.61, 1.02, 1.41, 1.78, 2.13}; //  6.5--30
//double binsY3[nBinsY3] = {0.71, 1.41, 1.86}; //  6.5--30
// middle |y| of each given bin are written here
double binsY[nBinsY]   = {0.2, 0.6, 1.0, 1.4, 1.8, 2.2}; //  6.5--30
double binsY3[nBinsY3] = {0.6, 1.4, 2.0}; //  6.5--30
double binsYErr[nBinsY]   = {0.2,0.2,0.2,0.2,0.2,0.2}; //  6.5--30
double binsY3Err[nBinsY3] = {0.6,0.2,0.4}; //  6.5--30
double binsYX[nBinsY]   = {0.05, 0.05, 0.05, 0.05, 0.05, 0.05}; //  6.5--30
double binsY3X[nBinsY3] = {0.05, 0.05, 0.05}; //  6.5--30


//------------------------------------------------------------------------ graph bins, will be filled in macro
// pr RAA
// mb
double prJpsi_mb[nBinsMB]        = {0};
double prJpsiErr_mb[nBinsMB]     = {0};
double prJpsiErrSyst_mb[nBinsMB] = {0};

// 1D
double prJpsi_cent[nBinsNpart12]        = {0};
double prJpsiErr_cent[nBinsNpart12]     = {0};
double prJpsiErrSyst_cent[nBinsNpart12] = {0};

double prJpsi_pt[nBinsPt]        = {0};
double prJpsiErr_pt[nBinsPt]     = {0};
double prJpsiErrSyst_pt[nBinsPt] = {0};

double prJpsi_y[nBinsY]        = {0};
double prJpsiErr_y[nBinsY]     = {0};
double prJpsiErrSyst_y[nBinsY] = {0};

// 3 y-bins
double prJpsi_y_y[nBinsY3]        = {0};
double prJpsiErr_y_y[nBinsY3]     = {0};
double prJpsiErrSyst_y_y[nBinsY3] = {0};

// 2D rapidity dependence of high-pT
double prJpsi_pt6530y012_cent[nBinsNpart6]        = {0};
double prJpsiErr_pt6530y012_cent[nBinsNpart6]     = {0};
double prJpsiErrSyst_pt6530y012_cent[nBinsNpart6] = {0};

double prJpsi_pt6530y1216_cent[nBinsNpart6]         = {0};
double prJpsiErr_pt6530y1216_cent[nBinsNpart6]      = {0};
double prJpsiErrSyst_pt6530y1216_cent[nBinsNpart6]  = {0};

double prJpsi_pt6530y1624_cent[nBinsNpart6]         = {0};
double prJpsiErr_pt6530y1624_cent[nBinsNpart6]      = {0};
double prJpsiErrSyst_pt6530y1624_cent[nBinsNpart6]  = {0};

// low-pt fwd region
// minbias in 3 pt bins: 3-6.5, 6.5-30, 3-30
double prJpsi_y1624MB_pt[nBinsMB3]       = {0};
double prJpsiErr_y1624MB_pt[nBinsMB3]    = {0};
double prJpsiErrSyst_y1624MB_pt[nBinsMB3]= {0};

double prJpsi_pt365y1624_pt[nBinsPt3]        = {0};
double prJpsiErr_pt365y1624_pt[nBinsPt3]     = {0};
double prJpsiErrSyst_pt365y1624_pt[nBinsPt3] = {0};

double prJpsi_pt365y1624_cent[nBinsNpart6]        = {0};
double prJpsiErr_pt365y1624_cent[nBinsNpart6]     = {0};
double prJpsiErrSyst_pt365y1624_cent[nBinsNpart6] = {0};


// ----------- non-pr 
// mb
double nonPrJpsi_mb[nBinsMB]        = {0};
double nonPrJpsiErr_mb[nBinsMB]     = {0};
double nonPrJpsiErrSyst_mb[nBinsMB] = {0};

// 1D 
double nonPrJpsi_cent[nBinsNpart12]        = {0};
double nonPrJpsiErr_cent[nBinsNpart12]     = {0};
double nonPrJpsiErrSyst_cent[nBinsNpart12] = {0};

double nonPrJpsi_pt[nBinsPt]        = {0};
double nonPrJpsiErr_pt[nBinsPt]     = {0};
double nonPrJpsiErrSyst_pt[nBinsPt] = {0};

double nonPrJpsi_y[nBinsY]        = {0};
double nonPrJpsiErr_y[nBinsY]     = {0};
double nonPrJpsiErrSyst_y[nBinsY] = {0};

// 3 y-bins
double nonPrJpsi_y_y[nBinsY3]        = {0};
double nonPrJpsiErr_y_y[nBinsY3]     = {0};
double nonPrJpsiErrSyst_y_y[nBinsY3] = {0};

// 2D rapidity dependence of high-pT
double nonPrJpsi_pt6530y012_cent[nBinsNpart6]        = {0};
double nonPrJpsiErr_pt6530y012_cent[nBinsNpart6]     = {0};
double nonPrJpsiErrSyst_pt6530y012_cent[nBinsNpart6] = {0};

double nonPrJpsi_pt6530y1216_cent[nBinsNpart6]        = {0};
double nonPrJpsiErr_pt6530y1216_cent[nBinsNpart6]     = {0};
double nonPrJpsiErrSyst_pt6530y1216_cent[nBinsNpart6] = {0};

double nonPrJpsi_pt6530y1624_cent[nBinsNpart6]        = {0};
double nonPrJpsiErr_pt6530y1624_cent[nBinsNpart6]     = {0};
double nonPrJpsiErrSyst_pt6530y1624_cent[nBinsNpart6] = {0};

// low-pt fwd region
// minbias in 3 pt bins: 3-6.5, 6.5-30, 3-30
double nonPrJpsi_y1624MB_pt[nBinsMB3]       = {0};
double nonPrJpsiErr_y1624MB_pt[nBinsMB3]    = {0};
double nonPrJpsiErrSyst_y1624MB_pt[nBinsMB3]= {0};

double nonPrJpsi_pt365y1624_pt[nBinsPt3]        = {0};
double nonPrJpsiErr_pt365y1624_pt[nBinsPt3]     = {0};
double nonPrJpsiErrSyst_pt365y1624_pt[nBinsPt3] = {0};

double nonPrJpsi_pt365y1624_cent[nBinsNpart6]       = {0};
double nonPrJpsiErr_pt365y1624_cent[nBinsNpart6]    = {0};
double nonPrJpsiErrSyst_pt365y1624_cent[nBinsNpart6]= {0};

//-------------------------- for testing purposes, the traditionally corrected yields & raa
// pr RAA
// mb
double prJpsiTrad_mb[nBinsMB]        = {0};
double prJpsiTradErr_mb[nBinsMB]     = {0};

// 1D
double prJpsiTrad_cent[nBinsNpart12]        = {0};
double prJpsiTradErr_cent[nBinsNpart12]     = {0};

double prJpsiTrad_pt[nBinsPt]        = {0};
double prJpsiTradErr_pt[nBinsPt]     = {0};

double prJpsiTrad_y[nBinsY]        = {0};
double prJpsiTradErr_y[nBinsY]     = {0};

// 3 y-bins
double prJpsiTrad_y_y[nBinsY3]        = {0};
double prJpsiTradErr_y_y[nBinsY3]     = {0};

// 2D rapidity dependence of high-pT
double prJpsiTrad_pt6530y012_cent[nBinsNpart6]        = {0};
double prJpsiTradErr_pt6530y012_cent[nBinsNpart6]     = {0};

double prJpsiTrad_pt6530y1216_cent[nBinsNpart6]         = {0};
double prJpsiTradErr_pt6530y1216_cent[nBinsNpart6]      = {0};

double prJpsiTrad_pt6530y1624_cent[nBinsNpart6]         = {0};
double prJpsiTradErr_pt6530y1624_cent[nBinsNpart6]      = {0};

// low-pt fwd region
// minbias in 3 pt bins: 3-6.5, 6.5-30, 3-30
double prJpsiTrad_y1624MB_pt[nBinsMB3]       = {0};
double prJpsiTradErr_y1624MB_pt[nBinsMB3]    = {0};

double prJpsiTrad_pt365y1624_pt[nBinsPt3]        = {0};
double prJpsiTradErr_pt365y1624_pt[nBinsPt3]     = {0};

double prJpsiTrad_pt365y1624_cent[nBinsNpart6]        = {0};
double prJpsiTradErr_pt365y1624_cent[nBinsNpart6]     = {0};

// ----------- non-pr 
// mb
double nonPrJpsiTrad_mb[nBinsMB]        = {0};
double nonPrJpsiTradErr_mb[nBinsMB]     = {0};


// 1D 
double nonPrJpsiTrad_cent[nBinsNpart12]        = {0};
double nonPrJpsiTradErr_cent[nBinsNpart12]     = {0};

double nonPrJpsiTrad_pt[nBinsPt]        = {0};
double nonPrJpsiTradErr_pt[nBinsPt]     = {0};

double nonPrJpsiTrad_y[nBinsY]        = {0};
double nonPrJpsiTradErr_y[nBinsY]     = {0};

// 3 y-bins
double nonPrJpsiTrad_y_y[nBinsY3]        = {0};
double nonPrJpsiTradErr_y_y[nBinsY3]     = {0};

// 2D rapidity dependence of high-pT
double nonPrJpsiTrad_pt6530y012_cent[nBinsNpart6]        = {0};
double nonPrJpsiTradErr_pt6530y012_cent[nBinsNpart6]     = {0};

double nonPrJpsiTrad_pt6530y1216_cent[nBinsNpart6]        = {0};
double nonPrJpsiTradErr_pt6530y1216_cent[nBinsNpart6]     = {0};

double nonPrJpsiTrad_pt6530y1624_cent[nBinsNpart6]        = {0};
double nonPrJpsiTradErr_pt6530y1624_cent[nBinsNpart6]     = {0};

// low-pt fwd region
// minbias in 3 pt bins: 3-6.5, 6.5-30, 3-30
double nonPrJpsiTrad_y1624MB_pt[nBinsMB3]           = {0};
double nonPrJpsiTradErr_y1624MB_pt[nBinsMB3]        = {0};

double nonPrJpsiTrad_pt365y1624_pt[nBinsPt3]        = {0};
double nonPrJpsiTradErr_pt365y1624_pt[nBinsPt3]     = {0};

double nonPrJpsiTrad_pt365y1624_cent[nBinsNpart6]   = {0};
double nonPrJpsiTradErr_pt365y1624_cent[nBinsNpart6]= {0};

//--------------------------------------------------------------------------------------------
// bins for histograms
double bins1[2] = {0,1};
double bins3[4] = {0,1, 2, 3};
double bins5[6] = {0,1, 2, 3, 4, 5};
double bins6[7] = {0,1, 2, 3, 4, 5, 6};
double bins7[8] = {0,1, 2, 3, 4, 5, 6,7};
double bins12[13] = {0,1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
// boxes width

#endif



