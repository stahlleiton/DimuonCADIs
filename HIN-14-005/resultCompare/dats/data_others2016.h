#ifndef DATA_OTHERS
#define DATA_OTHERS


/* // ALICE D: pT */ //they only have 0-10% and 30-50%
/* const int nPtBins_alice_d = 7;  */
/* double ptBins_alice_d[nPtBins_alice_d]    = {2.5, 3.5, 4.5, 5.5, 7, 10, 14}; // 0--2.4 */
/* double ptBinsErr_alice_d[nPtBins_alice_d] = {0.0,0.0,0.0,0.0}; */
/* double ptBinsLow_alice_d[nPtBins_alice_d] = {0.5,0.5,0.5,0.5,1.,2.,2.}; // 0--2.4 */
/* double ptBinsUp_alice_d[nPtBins_alice_d]  = {0.5,0.5,0.5,0.5,1.,2.,2.}; // 0--2.4 */
  
/* // D alice RAA _pt */
/* double raaPt_alice_d[nPtBins_alice_d]             = {0.51,0.37,0.33,0.27,0.28, 0.26, 0.35};  */
/* double raaPtErr_alice_d[nPtBins_alice_d]          = {0.1,0.06,0.05,0.07,0.04,0.03,0.06}; */
/* double raaPtErrSystLow_alice_d[nPtBins_alice_d]   = {0.18, 0.11, 0.1, 0.08, 0.07, 0.06, 0.1}; */
/* double raaPtErrSystHigh_alice_d[nPtBins_alice_d]  = {0.22, 0.13, 0.11,0.09, 0.08, 0.07, 0.12}; */


// ALICE D: <Npart>
//http://hepdata.cedar.ac.uk/view/ins1377363
//http://arxiv.org/pdf/1506.06604v2.pdf
// Average D meson RAA (average of D0, D+ and D∗+) in 8<pT<16 GeV/c, |y|<0.8
const int nNpartBins_alice = 6; 
double npartBins_alice[nNpartBins_alice]    = {356.0, 260.1,185.8, 128.5, 84.7, 32.2}; 
double npartBinsErr_alice[nNpartBins_alice] = {.0,.0, .0,.0,.0,0};
double npartSystX_alice[nNpartBins_alice]   = {10,10,10, 10,10,10};
  
// D alice RAA _Npart
double raaAlice_cent_dpt816[nNpartBins_alice]         = {0.15, 0.20, 0.27, 0.32, 0.37, 0.66}; 
double raaAliceStat_cent_dpt816[nNpartBins_alice]     = {0.01, 0.02, 0.03, 0.04, 0.05, 0.1};// hep data of 5th point seems wrong: 0.01

double raaAliceSystHigh_cent_dpt816[nNpartBins_alice] = {0.02, 0.03, 0.04, 0.04, 0.05, 0.10};//
double raaAliceSystLow_cent_dpt816[nNpartBins_alice]  = {0.02, 0.03, 0.04, 0.04, 0.05, 0.11};// hep data 5th and 6th point seem wrong 0.005 and 0.011

double raaAliceSystCorrHigh_cent_dpt816[nNpartBins_alice]   = {0.03, 0.04, 0.05, 0.06, 0.07, 0.12};
double raaAliceSystCorrLow_cent_dpt816[nNpartBins_alice]    = {0.03, 0.04, 0.05, 0.06, 0.07, 0.13};

//--------------------------
// D closed to the prompt Jpsi bin
// ALICE D: <Npart>
//http://arxiv.org/pdf/1203.2160v4.pdf table 6
// Average D meson RAA (average of D0, D+ and D∗+) in 6<pT<12 GeV/c, |y|<0.8
const int nNpartBins5_alice = 5; 
double npartBins5_alice[nNpartBins5_alice]    = {357, 261,157, 69, 23}; 
double npartBinsErr5_alice[nNpartBins5_alice] = {.0,.0,.0,.0,0};
double npartSystX5_alice[nNpartBins5_alice]   = {10,10,10,10,10};
  
// D alice RAA _Npart
double raaAlice_cent_dpt612[nNpartBins5_alice]         = {0.23, 0.28, 0.42, 0.54, 0.81}; 
double raaAliceStat_cent_dpt612[nNpartBins5_alice]     = {0.03, 0.04, 0.04, 0.05, 0.1};// hep data of 5th point seems wrong: 0.01

double raaAliceSystHigh_cent_dpt612[nNpartBins5_alice] = {0.05, 0.06, 0.08, 0.1, 0.16};//
double raaAliceSystLow_cent_dpt612[nNpartBins_alice]   = {0.06, 0.07, 0.11, 0.13,0.21};// hep data 5th and 6th point seem wrong 0.005 and 0.011

double raaAliceSystCorrHigh_cent_dpt612[nNpartBins5_alice]   = {0.03, 0.03, 0.05, 0.07, 0.11};
double raaAliceSystCorrLow_cent_dpt612[nNpartBins5_alice]    = {0.03, 0.04, 0.06, 0.08, 0.12};

/// low-pt bin
// http://hepdata.cedar.ac.uk/view/ins1093488/next
double raaAlice_cent_dpt25[nNpartBins5_alice]         = {0.31, 0.40, 0.49, 0.53, 0.62}; 
double raaAliceStat_cent_dpt25[nNpartBins5_alice]     = {0.05, 0.05, 0.05, 0.06, 0.09};// hep data of 5th point seems wrong: 0.01

double raaAliceSystHigh_cent_dpt25[nNpartBins5_alice] = {0.10, 0.13, 0.14, 0.13, 0.14};//
double raaAliceSystLow_cent_dpt25[nNpartBins_alice]   = {0.09, 0.12, 0.14, 0.14,0.16};// hep data 5th and 6th point seem wrong 0.005 and 0.011

double raaAliceSystCorrHigh_cent_dpt25[nNpartBins5_alice]   = {0.05, 0.06, 0.07, 0.08, 0.09};
double raaAliceSystCorrLow_cent_dpt25[nNpartBins5_alice]    = {0.08, 0.1, 0.12, 0.13, 0.16};


//================================== ALICE v2
//http://arxiv.org/pdf/1305.2707v3.pdf
//http://hepdata.cedar.ac.uk/view/ins1233087 Fig. 3
  double v2AliceX_pt[]    = {2.499, 3.482, 4.897, 6.963, 9.717, 13.475};
  double v2AliceXErr_pt[] = {0.15,   0.15,   0.15,   0.15,   0.15,  0.15};
  double v2AliceXl[]      = {0.2,   0.25,   0.25,   0.25,   0.25,  0.25};
  double v2AliceXl2[]     = {0.25,  0.25,   0.25,   0.25,   0.25,  0.25};

  double v2Alice_pt[]          = {0.164, 0.192, 0.23, 0.148, 0.265, 0.015};
  double v2AliceStat_pt[]      = {0.065, 0.052, 0.044, 0.056, 0.061, 0.099};
  double v2AliceSyst_pt[]      = {0.058, 0.028, 0.024, 0.034, 0.042, 0.105};
  double v2AliceSystBHigh_pt[] = {0.094, 0.094, 0.09, 0.058, 0.14, 0.0};
  double v2AliceSystBLow_pt[]  = {0.0, 0.0, 0.0, 0.0, 0.0, 0.01};
  
 

#endif

