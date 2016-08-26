#ifndef V2_DATANUMBERS_2015
#define V2_DATANUMBERS_2015

// Binning: bin0= minbias; then have promptBins, then non-prompt bins; (all together stored in 1 vector)
// ------------------------------------- event plane resolution corrections
double dEvPlResCorr[]    = { 0.8168,                         // MB: 10-60%
			     0.6311, 0.8166, 0.8418, 0.7812, // prompt: 0-10%, 10-20%, 20-30%, 30-60%;
			     0.6311, 0.8270, 0.7812};        // non-prompt: 0-10%, 10-30%, 30-60%
double dEvPlResCorrErr[] = { 0.0013,                         // MB: 10-60%
			     0.0023, 0.0018, 0.0025, 0.0029, // prompt: 0-10%, 10-20%, 20-30%, 30-60%;
			     0.0023, 0.0020, 0.0029};        // non-prompt: 0-10%, 10-30%, 30-60%

//------------------------------------------ legends and names
const char* legend[4]      = {"","Prompt J/#psi","Nonprompt J/#psi","Background"};
const char* outFilePlot[4] = {"mb","pt","rap","cent"};

const int nPtBins   = 8;
const int nYBins    = 6;
const int nCentBins = 8;

const char* ptBinsName[nPtBins]     = {"65300","3065","6580","80100","100300","3065","65100","100300"};
const char* yBinsName[nYBins]       = {"0024","0012","1216","1624","0012","1224"};
const char* centBinsName[nCentBins] = {"1060","010", "1020","2030","3060", "010","1030","3060"};


const char* ptBinsLegend[nPtBins]     = {"6.5 < p_{T} < 30 GeV/c",// MB
           "3 < p_{T} < 6.5","6.5 < p_{T} < 8","8 < p_{T} < 10","10 < p_{T} < 30",//prompt
           "3 < p_{T} < 6.5","6.5 < p_{T} < 10","10 < p_{T} < 30"}; // non-prompt
const char* yBinsLegend[nYBins]       = {"|y| < 2.4",// MB
           "|y| < 1.2","1.2 < |y| < 1.6","1.6 < |y| < 2.4",//prompt
           "|y| < 1.2","1.2 < |y| < 2.4"}; // non-prompt
const char* centBinsLegend[nCentBins] = {"Cent. 10-60\%",// MB
           "10-0\%","20-10\%","30-20\%","60-30\%",//prompt
           "10-0\%","30-10\%","60-30\%"}; // non-prompt

//------------------------------------- BINNING and limits
// prompt bins
double ptBins_pr[]    = {3.0, 6.5, 8.0, 10, 30.0};
double yBins_pr[]     = {0.0, 1.2, 1.6, 2.4};
double centBins_pr[]  = {0.0, 10.0, 20.0, 30.0, 60.0};

// non-prompt bins
double ptBins_np[]   = {3.0, 6.5, 10.0, 30.0};
double yBins_np[]    = {0.0, 1.2, 2.4};
double centBins_np[] = {0.0, 10.0, 30.0, 60.0};
// integrated bin
double centBins_int[] = {10.0, 60.0};

const int nPtBins_pr   = sizeof(ptBins_pr)/sizeof(double) -1;
const int nYBins_pr    = sizeof(yBins_pr)/sizeof(double) -1;
const unsigned int nCentBins_pr = sizeof(centBins_pr)/sizeof(double) -1;
const int nPtBins_np   = sizeof(ptBins_np)/sizeof(double) -1;
const int nYBins_np    = sizeof(yBins_np)/sizeof(double) -1;
const unsigned int nCentBins_np = sizeof(centBins_np)/sizeof(double) -1;


//--------------------------------------- plotting location
// bogus bin for minbias value
double adXaxis_mb[] = {200};
// pt axis
// 2 <pt> bins for high-pt non-prompt
double adXaxisPt_np[2]   = {7.8,13.1};
double adXaxisPt_np_l[2] = {1.3,3.1};
double adXaxisPt_np_h[2] = {2.2,16.9};
// 1 <pt> bins for high-pt non-prompt
double adXaxisPt_np1[1]   = {8.9};// location on x-axis
double adXaxisPt_np1_l[1] = {2.4};// bin width to the left
double adXaxisPt_np1_h[1] = {21.1}; // bin width to the right
// 3 <pt> bins for high-pt prompt
double adXaxisPt_pr[3]    = {7.2, 8.8, 13.1};// location on x-axis  
double adXaxisPt_pr_l[3]  = {0.7, 0.8,  3.1}; // bin width to the left
double adXaxisPt_pr_h[3]  = {0.8, 1.2, 16.9};// bin width to the right

// 1 <pt> bins for low-pt
double adXaxis_low[1]         = {4.4}; // bin width to the left
double adXaxis_low_l[1]       = {1.4}; // bin width to the left
double adXaxis_low_h[1]       = {2.1};// bin width to the right
double adWidth_low_systBox[1] = {0.25}; // width of the systm. uncert.

// rapidity axis
// 2 <|y|> bins for high-pt non-prompt
double adXaxisY_np[2]   = {0.7,1.7};
double adXaxisY_np_l[2] = {0.7,0.5};
double adXaxisY_np_h[2] = {0.6,0.7};

// 3 <|y|> bins for high-pt prompt
double adXaxisY_pr[3]    = {0.7, 1.4, 1.9};// location on x-axis  
double adXaxisY_pr_l[3]  = {0.7, 0.2, 0.3}; // bin width to the left
double adXaxisY_pr_h[3]  = {0.5, 0.2, 0.5};// bin width to the right

// cent axis
double adXaxisCent_pr[4]   = {90,187,261,355}; // Npart (60-30, 30-20, 20-10, 10-0)
double adXaxisCent_np[3]   = {90,224,355}; // Npart (60-30, 30-10, 10-0)

double flip_adXaxisCent_pr[4]   = {355, 261, 187, 90}; // Npart (10-0, 20-10, 30-20, 60-30)
double flip_adXaxisCent_np[3]   = {355, 224, 90}; // Npart (10-0, 30-10, 60-30)

double adWidth_systBox[3] = {0.25, 0.25, 0.25}; // width of the systm. uncert.

#endif
