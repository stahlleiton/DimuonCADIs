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
double npartSystX_alice[nNpartBins_alice]   = {7,7,7, 6,6,6};
  
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
double npartSystX5_alice[nNpartBins5_alice]   = {7,7,6,6,6};
  
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
  
 

//=================================== ALICE J/psi Raa
// http://arxiv.org/pdf/1506.08804v2.pdf 
// http://hepdata.cedar.ac.uk/view/ins1380192 : table A.10 & Fig. 9 (0-90%)
  double raaJpsiAliceCent090_xval[] = { 0.5, 1.5, 2.5, 3.5, 4.5, 5.5, 7.0 };
  double raaJpsiAliceCent090_xerrminus[] = { 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 1.0 };
  double raaJpsiAliceCent090_xerrplus[] = { 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 1.0 };
  double raaJpsiAliceCent090_yval[] = { 0.779, 0.677, 0.519, 0.425, 0.405, 0.322, 0.364 };
  double raaJpsiAliceCent090_yerrminus[] = { 0.13043005788544296, 0.09538343671728337, 0.07443117626371358, 0.06420280367709809, 0.07158910531638177, 0.06324555320336758, 0.08900561780022652 };
  double raaJpsiAliceCent090_yerrplus[] = { 0.13043005788544296, 0.09538343671728337, 0.07443117626371358, 0.06420280367709809, 0.07158910531638177, 0.06324555320336758, 0.08900561780022652 };
  double raaJpsiAliceCent090_ystatminus[] = { 0.076, 0.047, 0.038, 0.039, 0.054, 0.052, 0.079 };
  double raaJpsiAliceCent090_ystatplus[] = { 0.076, 0.047, 0.038, 0.039, 0.054, 0.052, 0.079 };
  int raaJpsiAliceCent090_numpoints = 7;


// #################### THEORY ##################
// ================== D, B, non-prompt J/psi v2 vs pT
// Pol-Bernard,  http://arxiv.org/abs/1409.0900
// 20%-60% centrality, mid rapidity,
// MC@HQV508 + EPOS2; June 15 2014
double v2PB_xval[] = {0.25, 0.75, 1.25, 1.75, 2.25, 2.75, 3.25, 3.75, 4.25, 4.75, 5.25, 5.75, 6.25, 6.75, 7.25, 7.75, 9., 11., 13., 15., 17.};
int v2PB_numpoints= 21;

// collisional energy loss K=0.8
double v2PB_K08_D[] = {0.00519167, 0.0229781, 0.0566016, 0.0916477, 0.115801, 0.128514, 0.13322, 0.134285, 0.135355, 0.132314, 0.130277, 0.12691, 0.123735, 0.124555, 0.119838, 0.116127, 0.110277, 0.0909381, 0.0911278, 0.0858003, 0.0690734};
double v2PB_K08_B[] = {0.00377077, 0.00259729, 0.00808071, 0.0121422, 0.0185802, 0.0269947, 0.0335512, 0.0343668, 0.0367998, 0.042304, 0.0413705, 0.0440683, 0.0440307, 0.0448333, 0.0423954, 0.0442806, 0.0478011, 0.0506068, 0.0454098, 0.0454485, 0.0442413};
double v2PB_K08_NPJpsi[] = {0.00112473, 0.00326366, 0.00530827, 0.00786583, 0.0109681, 0.0139433, 0.0174829, 0.0170122, 0.0192354, 0.0195477, 0.0174653, 0.022819, 0.0190821, 0.0193311, 0.0172817, 0.0186743, 0.0190898, 0.0178753, 0.0174636, 0.0103488, 0.0160539};

// collisional energy loss K=0.8
double v2PB_K15_D[] = {0.00435925, 0.0246732, 0.063743, 0.107271, 0.139617, 0.156508, 0.162872, 0.161692, 0.154873, 0.148355, 0.13999, 0.132768, 0.124511, 0.122534, 0.114152, 0.111393, 0.096739, 0.0833733, 0.0757386, 0.0611825, 0.0492327};
double v2PB_K15_B[] = {-0.00243433, 0.0041036, 0.0117114, 0.022222, 0.034077, 0.0441863, 0.0533224, 0.0601718, 0.0667693, 0.0702851, 0.0697248, 0.0723221, 0.0704822, 0.0713655, 0.0672338, 0.0696872, 0.06401, 0.0611464, 0.055591, 0.0530136, 0.0515218};
double v2PB_K15_NPJpsi[] = {0.000230445, 0.00330077, 0.00791534, 0.0136335, 0.0189053, 0.0237825, 0.0262673, 0.0294653, 0.0287633, 0.0304797, 0.0278732, 0.0272297, 0.0269976, 0.0229378, 0.0236902, 0.02393, 0.020116, 0.0172719, 0.0182439, 0.0114318, 0.0154325};


#endif
