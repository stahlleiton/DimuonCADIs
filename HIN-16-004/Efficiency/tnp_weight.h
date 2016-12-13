#ifndef tnp_weight_h
#define tnp_weight_h

#include "TMath.h"

// IN THIS FILE YOU WILL FIND:
// ++++++++++++++
//
// - Trigger:
//   * idx = 0:  nominal
//   * idx = 1..100: toy variations, stat. only
//   * idx = -1: syst variation, "new_MAX", +1 sigma
//   * idx = -2: syst variation, "new_MAX", -1 sigma
//   * idx = -10: binned
// - MuID, STA: 
//   * only one SF (for systematic uncertainty only)

double tnp_weight_trg_pbpb(double x, double eta, int idx);
double tnp_weight_trg_pp(double x, double eta, int idx);
double tnp_weight_muid_pbpb(double x, double eta);
double tnp_weight_muid_pp(double x, double eta);
double tnp_weight_sta_pp(double x, double eta);

///////////////////////////////////////////////////
//               T R G    P b P b                //
///////////////////////////////////////////////////
double tnp_weight_trg_pbpb(double x, double eta, int idx)
{
   // binned
   if (idx == -10) {
      if (fabs(eta) < 1.2) {
         // 0 < |eta| < 1.2
         if (x<4) return 1.05083;
         else if (x<4.5) return 1.00482;
         else if (x<5) return 0.991942;
         else if (x<5.5) return 0.999019;
         else if (x<6.5) return 0.998047;
         else if (x<8) return 0.982955;
         else if (x<10.5) return 0.994642;
         else if (x<14) return 0.982937;
         else if (x<18) return 0.980605;
         else return 1.00328;
      } else if (fabs(eta) < 1.8) {
         // 1.2 < |eta| < 1.8
         if (x<3) return 1.141;
         else if (x<3.5) return 1.08315;
         else if (x<4) return 1.03487;
         else if (x<4.5) return 1.01835;
         else if (x<5) return 0.997828;
         else if (x<6) return 0.987733;
         else if (x<7.5) return 1.00724;
         else if (x<10) return 0.994324;
         else if (x<15) return 1.01714;
         else return 1.01007;
      } else if (fabs(eta) < 2.1) {
         // 1.8 < |eta| < 2.1
         if (x<2) return 1.45635;
         else if (x<2.5) return 1.23666;
         else if (x<3) return 1.09206;
         else if (x<3.5) return 1.06978;
         else if (x<4) return 1.02001;
         else if (x<4.5) return 1.03328;
         else if (x<5.5) return 1.02344;
         else if (x<6.5) return 1.03221;
         else if (x<8) return 0.979755;
         else if (x<9.5) return 0.9832;
         else if (x<13) return 1.0359;
         else return 0.97688;
      } else {
         // 2.1 < |eta| < 2.4
         if (x<2.2) return 1.27031;
         else if (x<2.7) return 1.15418;
         else if (x<3.2) return 1.06914;
         else if (x<3.7) return 1.04694;
         else if (x<4.7) return 1.06675;
         else if (x<6.5) return 1.04215;
         else if (x<8.5) return 1.02019;
         else if (x<11) return 1.03374;
         else return 1.07708;
      }
   }

   // denominator (from MC)
   double den=1;
   if (fabs(eta)<1.2) den = 0.97701*TMath::Erf((x-2.38691)/1.31574);
   else if (fabs(eta)<1.8) den = 0.94675*TMath::Erf((x-2.29271)/1.19047);
   else if (fabs(eta)<2.1) den = 0.89781*TMath::Erf((x-0.80641)/2.31842);
   else den = 0.87208*TMath::Erf((x-0.63360)/1.99416);

   // numerator (from data)
   double num=1;

   if (fabs(eta)<1.2)
   {
      if (idx==0) num = 0.96859*TMath::Erf((x-1.86137)/1.57333);
      else if (idx == -1   ) num = 0.96934*TMath::Erf((x-1.86313)/1.55954);
      else if (idx == -2   ) num = 0.96786*TMath::Erf((x-1.86663)/1.57798);
      else if (idx == 1   ) num = 0.97220*TMath::Erf((x-0.68242)/2.39776);
      else if (idx == 2   ) num = 0.96885*TMath::Erf((x-2.06374)/1.46946);
      else if (idx == 3   ) num = 0.96628*TMath::Erf((x-1.99454)/1.48976);
      else if (idx == 4   ) num = 0.96903*TMath::Erf((x-2.05862)/1.44175);
      else if (idx == 5   ) num = 0.96828*TMath::Erf((x-1.69241)/1.68223);
      else if (idx == 6   ) num = 0.96664*TMath::Erf((x-1.84564)/1.56131);
      else if (idx == 7   ) num = 0.97196*TMath::Erf((x-1.89639)/1.59681);
      else if (idx == 8   ) num = 0.96775*TMath::Erf((x-1.90964)/1.56506);
      else if (idx == 9   ) num = 0.96701*TMath::Erf((x-2.07249)/1.42218);
      else if (idx == 10  ) num = 0.96758*TMath::Erf((x-2.24322)/1.24312);
      else if (idx == 11  ) num = 0.96434*TMath::Erf((x-1.76470)/1.66288);
      else if (idx == 12  ) num = 0.97217*TMath::Erf((x-1.50141)/1.86413);
      else if (idx == 13  ) num = 0.96988*TMath::Erf((x-2.05018)/1.45612);
      else if (idx == 14  ) num = 0.96816*TMath::Erf((x-1.55670)/1.82452);
      else if (idx == 15  ) num = 0.96473*TMath::Erf((x-2.15182)/1.34965);
      else if (idx == 16  ) num = 0.97441*TMath::Erf((x-0.87068)/2.42192);
      else if (idx == 17  ) num = 0.96730*TMath::Erf((x-1.86952)/1.58975);
      else if (idx == 18  ) num = 0.96938*TMath::Erf((x-2.33178)/1.24227);
      else if (idx == 19  ) num = 0.97152*TMath::Erf((x-1.01588)/2.19496);
      else if (idx == 20  ) num = 0.96818*TMath::Erf((x-2.03238)/1.47533);
      else if (idx == 21  ) num = 0.97003*TMath::Erf((x-1.31970)/2.00786);
      else if (idx == 22  ) num = 0.96844*TMath::Erf((x-2.14262)/1.36264);
      else if (idx == 23  ) num = 0.97174*TMath::Erf((x-1.66324)/1.76821);
      else if (idx == 24  ) num = 0.96475*TMath::Erf((x-2.61641)/0.96294);
      else if (idx == 25  ) num = 0.96945*TMath::Erf((x-1.96163)/1.51640);
      else if (idx == 26  ) num = 0.96596*TMath::Erf((x-2.51843)/1.01968);
      else if (idx == 27  ) num = 0.97182*TMath::Erf((x-1.34291)/2.04292);
      else if (idx == 28  ) num = 0.96863*TMath::Erf((x-1.80088)/1.59445);
      else if (idx == 29  ) num = 0.96562*TMath::Erf((x-1.85628)/1.57174);
      else if (idx == 30  ) num = 0.97149*TMath::Erf((x-1.25275)/2.02267);
      else if (idx == 31  ) num = 0.96800*TMath::Erf((x-1.16419)/2.03537);
      else if (idx == 32  ) num = 0.96567*TMath::Erf((x-1.92222)/1.50817);
      else if (idx == 33  ) num = 0.96846*TMath::Erf((x-2.05640)/1.42841);
      else if (idx == 34  ) num = 0.96960*TMath::Erf((x-1.02677)/2.21714);
      else if (idx == 35  ) num = 0.96744*TMath::Erf((x-1.84682)/1.56258);
      else if (idx == 36  ) num = 0.96905*TMath::Erf((x-1.27446)/2.03285);
      else if (idx == 37  ) num = 0.96981*TMath::Erf((x-1.21539)/2.10635);
      else if (idx == 38  ) num = 0.97227*TMath::Erf((x-1.43724)/1.90706);
      else if (idx == 39  ) num = 0.96527*TMath::Erf((x-2.26238)/1.26417);
      else if (idx == 40  ) num = 0.96588*TMath::Erf((x-1.74047)/1.62838);
      else if (idx == 41  ) num = 0.97303*TMath::Erf((x-2.05165)/1.44574);
      else if (idx == 42  ) num = 0.96872*TMath::Erf((x-1.49195)/1.83047);
      else if (idx == 43  ) num = 0.97314*TMath::Erf((x-1.08069)/2.21919);
      else if (idx == 44  ) num = 0.96384*TMath::Erf((x-2.00338)/1.40617);
      else if (idx == 45  ) num = 0.96519*TMath::Erf((x-2.52126)/1.00854);
      else if (idx == 46  ) num = 0.97054*TMath::Erf((x-1.66028)/1.74479);
      else if (idx == 47  ) num = 0.97420*TMath::Erf((x-1.02555)/2.20491);
      else if (idx == 48  ) num = 0.96606*TMath::Erf((x-1.61424)/1.78328);
      else if (idx == 49  ) num = 0.96764*TMath::Erf((x-1.51521)/1.82161);
      else if (idx == 50  ) num = 0.97195*TMath::Erf((x-1.06257)/2.21002);
      else if (idx == 51  ) num = 0.97055*TMath::Erf((x-1.61494)/1.78469);
      else if (idx == 52  ) num = 0.96670*TMath::Erf((x-2.36340)/1.19428);
      else if (idx == 53  ) num = 0.96992*TMath::Erf((x-1.54537)/1.77335);
      else if (idx == 54  ) num = 0.96967*TMath::Erf((x-1.88416)/1.58906);
      else if (idx == 55  ) num = 0.96974*TMath::Erf((x-2.11529)/1.40003);
      else if (idx == 56  ) num = 0.96406*TMath::Erf((x-2.62453)/0.92872);
      else if (idx == 57  ) num = 0.96689*TMath::Erf((x-1.53479)/1.80108);
      else if (idx == 58  ) num = 0.96813*TMath::Erf((x-2.21323)/1.35774);
      else if (idx == 59  ) num = 0.97106*TMath::Erf((x-1.54930)/1.76601);
      else if (idx == 60  ) num = 0.96423*TMath::Erf((x-1.95907)/1.50410);
      else if (idx == 61  ) num = 0.96930*TMath::Erf((x-1.33853)/1.92124);
      else if (idx == 62  ) num = 0.96498*TMath::Erf((x-1.42071)/1.91352);
      else if (idx == 63  ) num = 0.96474*TMath::Erf((x-2.08557)/1.40885);
      else if (idx == 64  ) num = 0.96872*TMath::Erf((x-1.15552)/2.00911);
      else if (idx == 65  ) num = 0.96577*TMath::Erf((x-2.27128)/1.25865);
      else if (idx == 66  ) num = 0.97023*TMath::Erf((x-2.13485)/1.40311);
      else if (idx == 67  ) num = 0.96968*TMath::Erf((x-1.89762)/1.59839);
      else if (idx == 68  ) num = 0.97091*TMath::Erf((x-1.35985)/2.00688);
      else if (idx == 69  ) num = 0.96638*TMath::Erf((x-1.49899)/1.84060);
      else if (idx == 70  ) num = 0.96831*TMath::Erf((x-2.16703)/1.31999);
      else if (idx == 71  ) num = 0.96879*TMath::Erf((x-2.19541)/1.35264);
      else if (idx == 72  ) num = 0.96393*TMath::Erf((x-1.81291)/1.53774);
      else if (idx == 73  ) num = 0.96796*TMath::Erf((x-1.84917)/1.56368);
      else if (idx == 74  ) num = 0.96939*TMath::Erf((x-1.68146)/1.68299);
      else if (idx == 75  ) num = 0.96803*TMath::Erf((x-1.72499)/1.65927);
      else if (idx == 76  ) num = 0.96870*TMath::Erf((x-1.85842)/1.57117);
      else if (idx == 77  ) num = 0.96828*TMath::Erf((x-1.74760)/1.67636);
      else if (idx == 78  ) num = 0.97064*TMath::Erf((x-1.11443)/2.17887);
      else if (idx == 79  ) num = 0.96288*TMath::Erf((x-2.04989)/1.39129);
      else if (idx == 80  ) num = 0.96998*TMath::Erf((x-1.70891)/1.68305);
      else if (idx == 81  ) num = 0.97054*TMath::Erf((x-2.21671)/1.34494);
      else if (idx == 82  ) num = 0.96795*TMath::Erf((x-2.52571)/1.07716);
      else if (idx == 83  ) num = 0.97430*TMath::Erf((x-1.08618)/2.16423);
      else if (idx == 84  ) num = 0.97414*TMath::Erf((x-1.08249)/2.20265);
      else if (idx == 85  ) num = 0.96836*TMath::Erf((x-1.82504)/1.54366);
      else if (idx == 86  ) num = 0.96901*TMath::Erf((x-1.86514)/1.57647);
      else if (idx == 87  ) num = 0.96142*TMath::Erf((x-2.13986)/1.34664);
      else if (idx == 88  ) num = 0.96499*TMath::Erf((x-2.23478)/1.26678);
      else if (idx == 89  ) num = 0.96774*TMath::Erf((x-1.83814)/1.55635);
      else if (idx == 90  ) num = 0.96772*TMath::Erf((x-2.16838)/1.34425);
      else if (idx == 91  ) num = 0.97023*TMath::Erf((x-1.88892)/1.59164);
      else if (idx == 92  ) num = 0.96593*TMath::Erf((x-1.83388)/1.55283);
      else if (idx == 93  ) num = 0.96747*TMath::Erf((x-1.66498)/1.70099);
      else if (idx == 94  ) num = 0.96667*TMath::Erf((x-1.85033)/1.56688);
      else if (idx == 95  ) num = 0.97162*TMath::Erf((x-1.94503)/1.52424);
      else if (idx == 96  ) num = 0.96896*TMath::Erf((x-1.97393)/1.49615);
      else if (idx == 97  ) num = 0.96648*TMath::Erf((x-2.08178)/1.36789);
      else if (idx == 98  ) num = 0.96547*TMath::Erf((x-1.82265)/1.54430);
      else if (idx == 99  ) num = 0.96643*TMath::Erf((x-2.04637)/1.41923);
      else if (idx == 100 ) num = 0.97157*TMath::Erf((x-1.52176)/1.82833);
   }
   else if (fabs(eta)<1.8)
   {
      if (idx==0) num = 0.94753*TMath::Erf((x-2.29787)/1.01635);
      else if (idx == -1   ) num = 0.94854*TMath::Erf((x-2.28198)/1.02715);
      else if (idx == -2   ) num = 0.94645*TMath::Erf((x-2.31364)/1.00454);
      else if (idx == 1   ) num = 0.93891*TMath::Erf((x-2.23717)/1.06326);
      else if (idx == 2   ) num = 0.95035*TMath::Erf((x-2.26085)/1.03494);
      else if (idx == 3   ) num = 0.94591*TMath::Erf((x-2.27134)/1.03744);
      else if (idx == 4   ) num = 0.94767*TMath::Erf((x-2.31942)/1.01378);
      else if (idx == 5   ) num = 0.94674*TMath::Erf((x-2.36126)/0.91640);
      else if (idx == 6   ) num = 0.94910*TMath::Erf((x-2.33385)/0.94013);
      else if (idx == 7   ) num = 0.94639*TMath::Erf((x-2.36999)/0.88805);
      else if (idx == 8   ) num = 0.95082*TMath::Erf((x-2.23857)/1.07721);
      else if (idx == 9   ) num = 0.94217*TMath::Erf((x-2.14948)/1.16471);
      else if (idx == 10  ) num = 0.94969*TMath::Erf((x-2.12712)/1.24420);
      else if (idx == 11  ) num = 0.95102*TMath::Erf((x-2.20301)/1.21113);
      else if (idx == 12  ) num = 0.94243*TMath::Erf((x-2.31937)/0.97791);
      else if (idx == 13  ) num = 0.94950*TMath::Erf((x-2.29777)/1.01542);
      else if (idx == 14  ) num = 0.94172*TMath::Erf((x-2.33558)/0.95436);
      else if (idx == 15  ) num = 0.94669*TMath::Erf((x-2.31585)/0.98138);
      else if (idx == 16  ) num = 0.94140*TMath::Erf((x-2.34193)/0.95721);
      else if (idx == 17  ) num = 0.94066*TMath::Erf((x-2.27864)/0.98999);
      else if (idx == 18  ) num = 0.94452*TMath::Erf((x-2.42255)/0.85359);
      else if (idx == 19  ) num = 0.94454*TMath::Erf((x-2.29662)/0.97185);
      else if (idx == 20  ) num = 0.95021*TMath::Erf((x-2.31958)/0.98094);
      else if (idx == 21  ) num = 0.93799*TMath::Erf((x-2.35816)/0.96618);
      else if (idx == 22  ) num = 0.94021*TMath::Erf((x-2.38063)/0.86438);
      else if (idx == 23  ) num = 0.94562*TMath::Erf((x-2.30220)/1.02183);
      else if (idx == 24  ) num = 0.94959*TMath::Erf((x-2.18641)/1.18186);
      else if (idx == 25  ) num = 0.94133*TMath::Erf((x-2.34208)/0.91977);
      else if (idx == 26  ) num = 0.94345*TMath::Erf((x-2.33624)/0.95720);
      else if (idx == 27  ) num = 0.94955*TMath::Erf((x-2.32529)/1.01382);
      else if (idx == 28  ) num = 0.94769*TMath::Erf((x-2.40304)/0.94482);
      else if (idx == 29  ) num = 0.94500*TMath::Erf((x-2.23991)/1.09392);
      else if (idx == 30  ) num = 0.94590*TMath::Erf((x-2.24537)/1.07922);
      else if (idx == 31  ) num = 0.94729*TMath::Erf((x-2.34982)/0.92482);
      else if (idx == 32  ) num = 0.94876*TMath::Erf((x-2.32311)/1.08668);
      else if (idx == 33  ) num = 0.94686*TMath::Erf((x-2.35707)/0.93423);
      else if (idx == 34  ) num = 0.95105*TMath::Erf((x-2.27679)/1.07726);
      else if (idx == 35  ) num = 0.94809*TMath::Erf((x-2.30707)/1.03075);
      else if (idx == 36  ) num = 0.94118*TMath::Erf((x-2.40481)/0.83860);
      else if (idx == 37  ) num = 0.94852*TMath::Erf((x-2.22760)/1.14815);
      else if (idx == 38  ) num = 0.94586*TMath::Erf((x-2.17168)/1.15843);
      else if (idx == 39  ) num = 0.94267*TMath::Erf((x-2.24319)/1.06628);
      else if (idx == 40  ) num = 0.94839*TMath::Erf((x-2.14492)/1.18519);
      else if (idx == 41  ) num = 0.94786*TMath::Erf((x-2.28279)/1.02619);
      else if (idx == 42  ) num = 0.95361*TMath::Erf((x-2.21456)/1.12783);
      else if (idx == 43  ) num = 0.95182*TMath::Erf((x-2.41061)/0.88103);
      else if (idx == 44  ) num = 0.94294*TMath::Erf((x-2.38684)/0.87276);
      else if (idx == 45  ) num = 0.94664*TMath::Erf((x-2.35609)/0.94301);
      else if (idx == 46  ) num = 0.95070*TMath::Erf((x-2.34107)/0.94133);
      else if (idx == 47  ) num = 0.94784*TMath::Erf((x-2.25026)/1.08360);
      else if (idx == 48  ) num = 0.94703*TMath::Erf((x-2.17728)/1.20770);
      else if (idx == 49  ) num = 0.94515*TMath::Erf((x-2.41059)/0.88878);
      else if (idx == 50  ) num = 0.94555*TMath::Erf((x-2.29110)/1.00743);
      else if (idx == 51  ) num = 0.95038*TMath::Erf((x-2.28148)/1.05756);
      else if (idx == 52  ) num = 0.94894*TMath::Erf((x-2.28407)/1.06279);
      else if (idx == 53  ) num = 0.94710*TMath::Erf((x-2.28041)/0.98982);
      else if (idx == 54  ) num = 0.95541*TMath::Erf((x-2.21251)/1.08614);
      else if (idx == 55  ) num = 0.94509*TMath::Erf((x-2.40371)/0.93810);
      else if (idx == 56  ) num = 0.94875*TMath::Erf((x-2.29816)/1.01736);
      else if (idx == 57  ) num = 0.94759*TMath::Erf((x-2.22405)/1.12716);
      else if (idx == 58  ) num = 0.93943*TMath::Erf((x-2.28038)/0.99497);
      else if (idx == 59  ) num = 0.94327*TMath::Erf((x-2.32332)/0.94012);
      else if (idx == 60  ) num = 0.95543*TMath::Erf((x-2.23935)/1.16646);
      else if (idx == 61  ) num = 0.94589*TMath::Erf((x-2.34510)/0.93029);
      else if (idx == 62  ) num = 0.94766*TMath::Erf((x-2.25065)/1.06147);
      else if (idx == 63  ) num = 0.94184*TMath::Erf((x-2.28080)/0.99395);
      else if (idx == 64  ) num = 0.94917*TMath::Erf((x-2.26009)/1.08544);
      else if (idx == 65  ) num = 0.94972*TMath::Erf((x-2.41995)/0.85267);
      else if (idx == 66  ) num = 0.94466*TMath::Erf((x-2.48403)/0.71985);
      else if (idx == 67  ) num = 0.94739*TMath::Erf((x-2.32931)/0.94870);
      else if (idx == 68  ) num = 0.94794*TMath::Erf((x-2.32716)/0.95947);
      else if (idx == 69  ) num = 0.94985*TMath::Erf((x-2.39698)/0.89027);
      else if (idx == 70  ) num = 0.94932*TMath::Erf((x-2.43105)/0.84032);
      else if (idx == 71  ) num = 0.94999*TMath::Erf((x-2.35592)/0.93848);
      else if (idx == 72  ) num = 0.94787*TMath::Erf((x-2.31019)/0.99908);
      else if (idx == 73  ) num = 0.94926*TMath::Erf((x-2.27031)/1.03409);
      else if (idx == 74  ) num = 0.94586*TMath::Erf((x-2.29391)/1.01078);
      else if (idx == 75  ) num = 0.94758*TMath::Erf((x-2.24904)/1.08838);
      else if (idx == 76  ) num = 0.94769*TMath::Erf((x-2.33403)/0.94723);
      else if (idx == 77  ) num = 0.94380*TMath::Erf((x-2.27129)/0.97183);
      else if (idx == 78  ) num = 0.95347*TMath::Erf((x-2.30362)/1.02225);
      else if (idx == 79  ) num = 0.94351*TMath::Erf((x-2.32622)/0.95336);
      else if (idx == 80  ) num = 0.94761*TMath::Erf((x-2.31138)/1.03476);
      else if (idx == 81  ) num = 0.95175*TMath::Erf((x-2.15296)/1.16244);
      else if (idx == 82  ) num = 0.94968*TMath::Erf((x-2.22026)/1.12878);
      else if (idx == 83  ) num = 0.94696*TMath::Erf((x-2.15635)/1.15505);
      else if (idx == 84  ) num = 0.94943*TMath::Erf((x-2.31939)/1.00900);
      else if (idx == 85  ) num = 0.95889*TMath::Erf((x-2.18729)/1.15953);
      else if (idx == 86  ) num = 0.95373*TMath::Erf((x-2.16682)/1.17205);
      else if (idx == 87  ) num = 0.94640*TMath::Erf((x-2.25505)/1.09624);
      else if (idx == 88  ) num = 0.94846*TMath::Erf((x-2.28179)/1.07669);
      else if (idx == 89  ) num = 0.94782*TMath::Erf((x-2.33945)/0.97821);
      else if (idx == 90  ) num = 0.95920*TMath::Erf((x-2.14912)/1.25437);
      else if (idx == 91  ) num = 0.94757*TMath::Erf((x-2.26889)/1.09399);
      else if (idx == 92  ) num = 0.95188*TMath::Erf((x-2.19279)/1.17627);
      else if (idx == 93  ) num = 0.94202*TMath::Erf((x-2.29387)/1.01334);
      else if (idx == 94  ) num = 0.94066*TMath::Erf((x-2.34927)/0.85274);
      else if (idx == 95  ) num = 0.94680*TMath::Erf((x-2.42209)/0.83081);
      else if (idx == 96  ) num = 0.94319*TMath::Erf((x-2.32340)/0.92327);
      else if (idx == 97  ) num = 0.94924*TMath::Erf((x-2.17215)/1.18594);
      else if (idx == 98  ) num = 0.95353*TMath::Erf((x-2.25046)/1.09236);
      else if (idx == 99  ) num = 0.94240*TMath::Erf((x-2.37859)/0.88922);
      else if (idx == 100 ) num = 0.94808*TMath::Erf((x-2.20120)/1.15939);
   }
   else if (fabs(eta)<2.1)
   {
      if (idx==0) num = 0.90308*TMath::Erf((x+0.38340)/3.01756);
      else if (idx == -1   ) num = 0.91411*TMath::Erf((x+0.88702)/3.47188);
      else if (idx == -2   ) num = 0.89193*TMath::Erf((x-0.03453)/2.62900);
      else if (idx == 1   ) num = 0.90133*TMath::Erf((x+2.51524)/4.71657);
      else if (idx == 2   ) num = 0.89938*TMath::Erf((x-0.06893)/2.63399);
      else if (idx == 3   ) num = 0.90375*TMath::Erf((x+0.70347)/3.45947);
      else if (idx == 4   ) num = 0.90401*TMath::Erf((x-0.39313)/2.41467);
      else if (idx == 5   ) num = 0.88480*TMath::Erf((x+2.29709)/4.56739);
      else if (idx == 6   ) num = 0.89109*TMath::Erf((x+1.06979)/3.42303);
      else if (idx == 7   ) num = 0.89845*TMath::Erf((x+0.46623)/3.22404);
      else if (idx == 8   ) num = 0.92622*TMath::Erf((x+1.27287)/3.96030);
      else if (idx == 9   ) num = 0.90232*TMath::Erf((x+0.89344)/3.45003);
      else if (idx == 10  ) num = 0.90578*TMath::Erf((x-0.09903)/2.53014);
      else if (idx == 11  ) num = 0.89629*TMath::Erf((x+1.99642)/4.39586);
      else if (idx == 12  ) num = 0.87757*TMath::Erf((x+0.22838)/2.55574);
      else if (idx == 13  ) num = 0.90430*TMath::Erf((x-0.21894)/2.59751);
      else if (idx == 14  ) num = 0.92484*TMath::Erf((x-0.38317)/2.49856);
      else if (idx == 15  ) num = 0.90022*TMath::Erf((x-0.62171)/2.13680);
      else if (idx == 16  ) num = 0.90568*TMath::Erf((x-0.31125)/2.43715);
      else if (idx == 17  ) num = 0.90842*TMath::Erf((x+0.46539)/3.03519);
      else if (idx == 18  ) num = 0.91753*TMath::Erf((x+0.32573)/3.14621);
      else if (idx == 19  ) num = 0.89349*TMath::Erf((x-0.46061)/2.26023);
      else if (idx == 20  ) num = 0.91794*TMath::Erf((x+5.49976)/7.03158);
      else if (idx == 21  ) num = 0.88517*TMath::Erf((x+0.84423)/3.21242);
      else if (idx == 22  ) num = 0.90298*TMath::Erf((x+0.68959)/3.38150);
      else if (idx == 23  ) num = 0.92797*TMath::Erf((x+0.08437)/3.08065);
      else if (idx == 24  ) num = 0.89026*TMath::Erf((x-0.16617)/2.65288);
      else if (idx == 25  ) num = 0.92337*TMath::Erf((x+4.44746)/6.85401);
      else if (idx == 26  ) num = 0.88888*TMath::Erf((x+0.35533)/2.85771);
      else if (idx == 27  ) num = 0.90306*TMath::Erf((x+0.05722)/2.83532);
      else if (idx == 28  ) num = 0.90449*TMath::Erf((x+0.43834)/3.08061);
      else if (idx == 29  ) num = 0.92440*TMath::Erf((x+1.49333)/4.26229);
      else if (idx == 30  ) num = 0.90958*TMath::Erf((x+1.55308)/4.19987);
      else if (idx == 31  ) num = 0.87435*TMath::Erf((x-1.21415)/1.28728);
      else if (idx == 32  ) num = 0.90720*TMath::Erf((x+0.86987)/3.25010);
      else if (idx == 33  ) num = 0.89337*TMath::Erf((x+0.20992)/2.73142);
      else if (idx == 34  ) num = 0.89829*TMath::Erf((x+3.37266)/5.40874);
      else if (idx == 35  ) num = 0.89568*TMath::Erf((x+2.42446)/4.55309);
      else if (idx == 36  ) num = 0.89148*TMath::Erf((x-0.55512)/1.96536);
      else if (idx == 37  ) num = 0.90946*TMath::Erf((x+0.67907)/3.28619);
      else if (idx == 38  ) num = 0.89490*TMath::Erf((x+1.06397)/3.65065);
      else if (idx == 39  ) num = 0.90462*TMath::Erf((x-0.38578)/2.37546);
      else if (idx == 40  ) num = 0.87301*TMath::Erf((x-0.47416)/1.91654);
      else if (idx == 41  ) num = 0.87731*TMath::Erf((x-0.20504)/2.27713);
      else if (idx == 42  ) num = 0.88756*TMath::Erf((x-0.78447)/1.81204);
      else if (idx == 43  ) num = 0.91966*TMath::Erf((x-0.14912)/2.79753);
      else if (idx == 44  ) num = 0.90146*TMath::Erf((x-0.65164)/2.00435);
      else if (idx == 45  ) num = 0.91837*TMath::Erf((x+0.04792)/2.94349);
      else if (idx == 46  ) num = 0.89783*TMath::Erf((x+0.52600)/3.31095);
      else if (idx == 47  ) num = 0.89940*TMath::Erf((x+0.34263)/3.16886);
      else if (idx == 48  ) num = 0.88998*TMath::Erf((x-0.10339)/2.62082);
      else if (idx == 49  ) num = 0.91888*TMath::Erf((x-0.28315)/2.59053);
      else if (idx == 50  ) num = 0.91343*TMath::Erf((x+0.11011)/2.78660);
      else if (idx == 51  ) num = 0.88749*TMath::Erf((x+0.82065)/3.24149);
      else if (idx == 52  ) num = 0.89758*TMath::Erf((x-0.71825)/1.98357);
      else if (idx == 53  ) num = 0.90252*TMath::Erf((x-0.16748)/2.49708);
      else if (idx == 54  ) num = 0.91812*TMath::Erf((x+1.31527)/3.98975);
      else if (idx == 55  ) num = 0.91096*TMath::Erf((x-0.39460)/2.42503);
      else if (idx == 56  ) num = 0.91327*TMath::Erf((x+1.76644)/4.49519);
      else if (idx == 57  ) num = 0.93348*TMath::Erf((x-0.72815)/2.09701);
      else if (idx == 58  ) num = 0.89835*TMath::Erf((x+0.73167)/3.06207);
      else if (idx == 59  ) num = 0.89840*TMath::Erf((x+0.00238)/2.51083);
      else if (idx == 60  ) num = 0.89302*TMath::Erf((x+0.80239)/3.24793);
      else if (idx == 61  ) num = 0.87143*TMath::Erf((x+1.12502)/3.22869);
      else if (idx == 62  ) num = 0.92543*TMath::Erf((x+0.35322)/3.14925);
      else if (idx == 63  ) num = 0.89613*TMath::Erf((x+0.54113)/2.89346);
      else if (idx == 64  ) num = 0.88552*TMath::Erf((x+0.86215)/3.20295);
      else if (idx == 65  ) num = 0.89764*TMath::Erf((x-0.18359)/2.28236);
      else if (idx == 66  ) num = 0.91130*TMath::Erf((x+0.79615)/3.39557);
      else if (idx == 67  ) num = 0.90162*TMath::Erf((x-0.16368)/2.57634);
      else if (idx == 68  ) num = 0.89933*TMath::Erf((x-0.50153)/2.17003);
      else if (idx == 69  ) num = 0.91062*TMath::Erf((x+1.48648)/4.17404);
      else if (idx == 70  ) num = 0.90863*TMath::Erf((x+0.14089)/2.77845);
      else if (idx == 71  ) num = 0.90796*TMath::Erf((x+0.25725)/2.95999);
      else if (idx == 72  ) num = 0.88684*TMath::Erf((x-0.30889)/2.24101);
      else if (idx == 73  ) num = 0.89895*TMath::Erf((x-0.35268)/2.35784);
      else if (idx == 74  ) num = 0.90060*TMath::Erf((x+1.31374)/3.69622);
      else if (idx == 75  ) num = 0.90401*TMath::Erf((x+0.67193)/3.19159);
      else if (idx == 76  ) num = 0.89913*TMath::Erf((x+4.89065)/5.91700);
      else if (idx == 77  ) num = 0.88745*TMath::Erf((x+0.00411)/2.68113);
      else if (idx == 78  ) num = 0.90692*TMath::Erf((x+0.57875)/3.35768);
      else if (idx == 79  ) num = 0.86222*TMath::Erf((x+0.36570)/2.68251);
      else if (idx == 80  ) num = 0.89631*TMath::Erf((x-0.20282)/2.45861);
      else if (idx == 81  ) num = 0.89445*TMath::Erf((x-0.68104)/2.16591);
      else if (idx == 82  ) num = 0.91009*TMath::Erf((x+1.65957)/4.13236);
      else if (idx == 83  ) num = 0.89310*TMath::Erf((x+0.63138)/2.93873);
      else if (idx == 84  ) num = 0.90464*TMath::Erf((x+2.03629)/4.12120);
      else if (idx == 85  ) num = 0.90484*TMath::Erf((x+1.94694)/4.34561);
      else if (idx == 86  ) num = 0.91820*TMath::Erf((x+1.99734)/4.55747);
      else if (idx == 87  ) num = 0.89881*TMath::Erf((x+1.13199)/3.60696);
      else if (idx == 88  ) num = 0.89174*TMath::Erf((x+0.10509)/2.66778);
      else if (idx == 89  ) num = 0.90317*TMath::Erf((x+0.75288)/3.46098);
      else if (idx == 90  ) num = 0.90123*TMath::Erf((x+4.28682)/6.39578);
      else if (idx == 91  ) num = 0.88905*TMath::Erf((x+0.27105)/2.65697);
      else if (idx == 92  ) num = 0.92952*TMath::Erf((x+0.04262)/2.75286);
      else if (idx == 93  ) num = 0.92110*TMath::Erf((x+2.64333)/5.14776);
      else if (idx == 94  ) num = 0.91314*TMath::Erf((x+1.93417)/4.45411);
      else if (idx == 95  ) num = 0.91004*TMath::Erf((x+0.02527)/2.80648);
      else if (idx == 96  ) num = 0.90279*TMath::Erf((x+0.25425)/3.03008);
      else if (idx == 97  ) num = 0.91834*TMath::Erf((x+2.26470)/5.01178);
      else if (idx == 98  ) num = 0.88781*TMath::Erf((x+0.64929)/3.11468);
      else if (idx == 99  ) num = 0.89985*TMath::Erf((x+2.55673)/4.90839);
      else if (idx == 100 ) num = 0.90635*TMath::Erf((x+0.29160)/2.98695);
   }
   else
   {
      if (idx==0) num = 0.91174*TMath::Erf((x+0.42843)/2.61737);
      else if (idx == -1   ) num = 0.91165*TMath::Erf((x+0.74551)/2.77943);
      else if (idx == -2   ) num = 0.91169*TMath::Erf((x+0.17925)/2.49352);
      else if (idx == 1   ) num = 0.88556*TMath::Erf((x-0.05093)/2.19732);
      else if (idx == 2   ) num = 0.93512*TMath::Erf((x+1.62238)/3.79510);
      else if (idx == 3   ) num = 0.92535*TMath::Erf((x+0.36939)/2.65491);
      else if (idx == 4   ) num = 0.92326*TMath::Erf((x+1.45818)/3.71173);
      else if (idx == 5   ) num = 0.88710*TMath::Erf((x-0.08378)/1.92964);
      else if (idx == 6   ) num = 0.90841*TMath::Erf((x+1.14399)/3.10913);
      else if (idx == 7   ) num = 0.92312*TMath::Erf((x+1.51513)/3.62567);
      else if (idx == 8   ) num = 0.89696*TMath::Erf((x+0.28327)/2.33805);
      else if (idx == 9   ) num = 0.91713*TMath::Erf((x+2.61254)/4.38020);
      else if (idx == 10  ) num = 0.87039*TMath::Erf((x+0.27459)/2.17674);
      else if (idx == 11  ) num = 0.94197*TMath::Erf((x+0.41717)/3.08282);
      else if (idx == 12  ) num = 0.91011*TMath::Erf((x+1.26963)/3.21006);
      else if (idx == 13  ) num = 0.89205*TMath::Erf((x+0.32470)/2.42502);
      else if (idx == 14  ) num = 0.88254*TMath::Erf((x-0.16029)/1.99310);
      else if (idx == 15  ) num = 0.91733*TMath::Erf((x+0.56530)/2.66756);
      else if (idx == 16  ) num = 0.92879*TMath::Erf((x+0.56715)/3.02109);
      else if (idx == 17  ) num = 0.90091*TMath::Erf((x+0.32390)/2.44721);
      else if (idx == 18  ) num = 0.90047*TMath::Erf((x-0.24963)/1.96296);
      else if (idx == 19  ) num = 0.95174*TMath::Erf((x+0.32860)/2.67137);
      else if (idx == 20  ) num = 0.92468*TMath::Erf((x-0.07924)/2.29450);
      else if (idx == 21  ) num = 0.89604*TMath::Erf((x-0.05031)/2.02791);
      else if (idx == 22  ) num = 0.90072*TMath::Erf((x-0.13398)/2.05239);
      else if (idx == 23  ) num = 0.90001*TMath::Erf((x-0.37983)/1.58726);
      else if (idx == 24  ) num = 0.94335*TMath::Erf((x+0.48368)/3.01309);
      else if (idx == 25  ) num = 0.87845*TMath::Erf((x-1.15032)/0.86968);
      else if (idx == 26  ) num = 0.87663*TMath::Erf((x-0.20549)/1.89327);
      else if (idx == 27  ) num = 0.89593*TMath::Erf((x+0.07299)/2.21400);
      else if (idx == 28  ) num = 0.89823*TMath::Erf((x+0.75857)/2.80539);
      else if (idx == 29  ) num = 0.90028*TMath::Erf((x+0.03872)/1.94030);
      else if (idx == 30  ) num = 0.94501*TMath::Erf((x+0.32269)/2.77848);
      else if (idx == 31  ) num = 0.88987*TMath::Erf((x+0.15127)/2.15727);
      else if (idx == 32  ) num = 0.89597*TMath::Erf((x+0.65894)/2.77349);
      else if (idx == 33  ) num = 0.88357*TMath::Erf((x+0.32272)/2.31358);
      else if (idx == 34  ) num = 0.91055*TMath::Erf((x+0.53697)/2.51001);
      else if (idx == 35  ) num = 0.89333*TMath::Erf((x-0.67775)/1.46018);
      else if (idx == 36  ) num = 0.90734*TMath::Erf((x-0.22058)/1.98696);
      else if (idx == 37  ) num = 0.91007*TMath::Erf((x+0.10334)/2.44059);
      else if (idx == 38  ) num = 0.89061*TMath::Erf((x-0.09604)/2.15249);
      else if (idx == 39  ) num = 0.90725*TMath::Erf((x+0.35874)/2.79728);
      else if (idx == 40  ) num = 0.87054*TMath::Erf((x-1.71684)/0.25814);
      else if (idx == 41  ) num = 0.91293*TMath::Erf((x+0.13197)/2.36298);
      else if (idx == 42  ) num = 0.87263*TMath::Erf((x-0.38698)/1.77750);
      else if (idx == 43  ) num = 0.91071*TMath::Erf((x+1.67319)/3.63544);
      else if (idx == 44  ) num = 0.89188*TMath::Erf((x-0.56284)/1.59353);
      else if (idx == 45  ) num = 0.91615*TMath::Erf((x+0.82864)/3.20319);
      else if (idx == 46  ) num = 0.91038*TMath::Erf((x-0.45720)/1.82855);
      else if (idx == 47  ) num = 0.92414*TMath::Erf((x-0.22087)/2.21285);
      else if (idx == 48  ) num = 0.88432*TMath::Erf((x+0.57883)/2.49281);
      else if (idx == 49  ) num = 0.90028*TMath::Erf((x-0.04745)/2.08324);
      else if (idx == 50  ) num = 0.87886*TMath::Erf((x+0.61502)/2.46084);
      else if (idx == 51  ) num = 0.87955*TMath::Erf((x+0.22107)/2.23334);
      else if (idx == 52  ) num = 0.89412*TMath::Erf((x+0.15168)/2.22112);
      else if (idx == 53  ) num = 0.91646*TMath::Erf((x+0.29646)/2.71776);
      else if (idx == 54  ) num = 0.88986*TMath::Erf((x+0.65144)/2.60518);
      else if (idx == 55  ) num = 0.88582*TMath::Erf((x+0.80599)/2.59040);
      else if (idx == 56  ) num = 0.89141*TMath::Erf((x+1.27088)/3.05764);
      else if (idx == 57  ) num = 0.86332*TMath::Erf((x+0.96815)/2.67610);
      else if (idx == 58  ) num = 0.91871*TMath::Erf((x+0.62789)/2.78795);
      else if (idx == 59  ) num = 0.92253*TMath::Erf((x+0.90289)/3.19798);
      else if (idx == 60  ) num = 0.93460*TMath::Erf((x+0.64892)/3.02476);
      else if (idx == 61  ) num = 0.90345*TMath::Erf((x+0.48103)/2.34694);
      else if (idx == 62  ) num = 0.94197*TMath::Erf((x+0.85863)/3.44957);
      else if (idx == 63  ) num = 0.90177*TMath::Erf((x-0.45037)/1.82742);
      else if (idx == 64  ) num = 0.91758*TMath::Erf((x+0.30191)/2.48286);
      else if (idx == 65  ) num = 0.91413*TMath::Erf((x-0.39183)/1.87413);
      else if (idx == 66  ) num = 0.91855*TMath::Erf((x-0.08918)/2.29942);
      else if (idx == 67  ) num = 0.91094*TMath::Erf((x+0.08127)/2.41802);
      else if (idx == 68  ) num = 0.90990*TMath::Erf((x+0.31698)/2.51846);
      else if (idx == 69  ) num = 0.87164*TMath::Erf((x+0.21257)/2.11652);
      else if (idx == 70  ) num = 0.91246*TMath::Erf((x+1.74237)/3.77912);
      else if (idx == 71  ) num = 0.87421*TMath::Erf((x-0.79314)/1.23412);
      else if (idx == 72  ) num = 0.87039*TMath::Erf((x-0.30021)/1.70053);
      else if (idx == 73  ) num = 0.85786*TMath::Erf((x-0.83068)/1.09659);
      else if (idx == 74  ) num = 0.89392*TMath::Erf((x-0.22577)/1.95690);
      else if (idx == 75  ) num = 0.89327*TMath::Erf((x-0.24873)/1.84525);
      else if (idx == 76  ) num = 0.90773*TMath::Erf((x+0.10269)/2.45636);
      else if (idx == 77  ) num = 0.92194*TMath::Erf((x-0.01758)/2.18684);
      else if (idx == 78  ) num = 0.91402*TMath::Erf((x+0.92906)/3.25360);
      else if (idx == 79  ) num = 0.89912*TMath::Erf((x+0.70545)/2.70498);
      else if (idx == 80  ) num = 0.89525*TMath::Erf((x+0.25565)/2.26352);
      else if (idx == 81  ) num = 0.91832*TMath::Erf((x+1.39577)/3.90724);
      else if (idx == 82  ) num = 0.91601*TMath::Erf((x-0.07235)/2.15516);
      else if (idx == 83  ) num = 0.89444*TMath::Erf((x-0.37968)/1.96507);
      else if (idx == 84  ) num = 0.89884*TMath::Erf((x+0.98593)/2.95295);
      else if (idx == 85  ) num = 0.90200*TMath::Erf((x+0.97603)/2.80845);
      else if (idx == 86  ) num = 0.90262*TMath::Erf((x-0.86479)/1.39761);
      else if (idx == 87  ) num = 0.87359*TMath::Erf((x+1.73489)/3.19910);
      else if (idx == 88  ) num = 0.88471*TMath::Erf((x+0.14941)/2.29428);
      else if (idx == 89  ) num = 0.91374*TMath::Erf((x+0.26769)/2.35604);
      else if (idx == 90  ) num = 0.90205*TMath::Erf((x+0.39789)/2.53676);
      else if (idx == 91  ) num = 0.91755*TMath::Erf((x+0.58025)/2.91726);
      else if (idx == 92  ) num = 0.91852*TMath::Erf((x+2.12959)/4.28679);
      else if (idx == 93  ) num = 0.90339*TMath::Erf((x-0.52417)/1.74583);
      else if (idx == 94  ) num = 0.90871*TMath::Erf((x+0.62459)/2.74819);
      else if (idx == 95  ) num = 0.89396*TMath::Erf((x+0.93206)/2.77567);
      else if (idx == 96  ) num = 0.88428*TMath::Erf((x+0.34629)/2.25652);
      else if (idx == 97  ) num = 0.93935*TMath::Erf((x+0.90253)/3.21417);
      else if (idx == 98  ) num = 0.92371*TMath::Erf((x-0.34233)/1.98688);
      else if (idx == 99  ) num = 0.90600*TMath::Erf((x+0.06380)/2.15114);
      else if (idx == 100 ) num = 0.94401*TMath::Erf((x+3.42680)/5.66206);
   }

   // return
   return num/den;
}

///////////////////////////////////////////////////
//               T R G    P P                    //
///////////////////////////////////////////////////
double tnp_weight_trg_pp(double x, double eta, int idx)
{
   // binned
   if (idx == -10) {
      if (fabs(eta) < 1.2) {
         // 0 < |eta| < 1.2
         if (x<4) return 1.05133;
         else if (x<4.5) return 1.01493;
         else if (x<5) return 1.00637;
         else if (x<5.5) return 1.00866;
         else if (x<6) return 1.00403;
         else if (x<7) return 0.999526;
         else if (x<8) return 0.993882;
         else if (x<10.5) return 0.999195;
         else if (x<14) return 0.994448;
         else if (x<18) return 0.997814;
         else return 0.99535;
      } else if (fabs(eta)<1.8) {
         // 1.2 < |eta| < 1.8
         if (x<3) return 1.20399;
         else if (x<3.5) return 1.08496;
         else if (x<4) return 1.0579;
         else if (x<4.5) return 1.02943;
         else if (x<5) return 1.02136;
         else if (x<6) return 1.01855;
         else if (x<7) return 1.01709;
         else if (x<9) return 1.00907;
         else if (x<14) return 1.00748;
         else if (x<18) return 1.02484;
         else return 1.02539;
      } else if (fabs(eta)<2.1) {
         // 1.8 < |eta| < 2.1
         if (x<2.5) return 1.23506;
         else if (x<3) return 1.07046;
         else if (x<3.5) return 1.05211;
         else if (x<4) return 1.04089;
         else if (x<4.5) return 1.04115;
         else if (x<5) return 1.03067;
         else if (x<6) return 1.02442;
         else if (x<7) return 1.025;
         else if (x<9) return 1.02442;
         else if (x<13) return 0.997836;
         else return 0.944194;
      } else {
         // 2.1 < |eta| < 2.4
         if (x<2.5) return 1.21185;
         else if (x<3) return 1.10746;
         else if (x<3.5) return 1.07255;
         else if (x<4) return 1.03562;
         else if (x<4.5) return 1.03939;
         else if (x<5) return 1.02434;
         else if (x<6.5) return 1.02967;
         else if (x<8.5) return 1.02523;
         else if (x<12) return 1.03297;
         else return 0.990749;
      }
   }

   // denominator (from MC)
   double den=1;
   if (fabs(eta)<1.2) den = (92.04760*(TMath::Erf((x-5.07863)/8.30156)*TMath::Exp(-1.28287*x)))+0.97482;
   else if (fabs(eta)<1.8) den = (45.24737*(TMath::Erf((x-6.26296)/4.09850)*TMath::Exp(-1.49794*x)))+0.94428;
   else if (fabs(eta)<2.1) den = (7.66192*(TMath::Erf((x-5.08376)/11.97845)*TMath::Exp(-0.74457*x)))+0.89233;
   else den = (15.88800*(TMath::Erf((x-8.31955)/48.83503)*TMath::Exp(-1.02550*x)))+0.88037;

   // numerator (from data)
   double num=1;
   if (fabs(eta)<1.2)
   {
      if (idx==0) num = (94.98743*(TMath::Erf((x-4.65627)/12.08735)*TMath::Exp(-1.19988*x)))+0.97140;
      else if (idx == -1   ) num = (94.99970*(TMath::Erf((x-4.65559)/12.07261)*TMath::Exp(-1.19965*x)))+0.97161;
      else if (idx == -2   ) num = (95.00000*(TMath::Erf((x-4.65716)/12.11383)*TMath::Exp(-1.20079*x)))+0.97105;
      else if (idx == 1   ) num = (78.00179*(TMath::Erf((x-4.82324)/12.71748)*TMath::Exp(-1.20089*x)))+0.97443;
      else if (idx == 2   ) num = (94.99772*(TMath::Erf((x-4.44777)/10.84693)*TMath::Exp(-1.16536*x)))+0.96628;
      else if (idx == 3   ) num = (94.99287*(TMath::Erf((x-4.67699)/11.92449)*TMath::Exp(-1.19691*x)))+0.97066;
      else if (idx == 4   ) num = (80.18794*(TMath::Erf((x-4.65625)/4.95961)*TMath::Exp(-1.38798*x)))+0.97215;
      else if (idx == 5   ) num = (94.99987*(TMath::Erf((x-4.59425)/11.72910)*TMath::Exp(-1.19890*x)))+0.96889;
      else if (idx == 6   ) num = (94.99963*(TMath::Erf((x-4.72298)/11.58317)*TMath::Exp(-1.21774*x)))+0.97163;
      else if (idx == 7   ) num = (94.99934*(TMath::Erf((x-4.55699)/11.48440)*TMath::Exp(-1.18791*x)))+0.96784;
      else if (idx == 8   ) num = (94.99977*(TMath::Erf((x-4.64503)/11.93530)*TMath::Exp(-1.19637*x)))+0.97244;
      else if (idx == 9   ) num = (94.99546*(TMath::Erf((x-4.66963)/11.83573)*TMath::Exp(-1.19315*x)))+0.97224;
      else if (idx == 10  ) num = (93.12520*(TMath::Erf((x-4.69811)/8.50164)*TMath::Exp(-1.30113*x)))+0.97038;
      else if (idx == 11  ) num = (82.96439*(TMath::Erf((x-4.82347)/5.80421)*TMath::Exp(-1.39560*x)))+0.97421;
      else if (idx == 12  ) num = (92.64096*(TMath::Erf((x-4.65390)/12.04974)*TMath::Exp(-1.19780*x)))+0.97240;
      else if (idx == 13  ) num = (94.94442*(TMath::Erf((x-4.72587)/9.14025)*TMath::Exp(-1.29164*x)))+0.97301;
      else if (idx == 14  ) num = (93.88566*(TMath::Erf((x-4.66077)/12.11902)*TMath::Exp(-1.20080*x)))+0.97115;
      else if (idx == 15  ) num = (94.90661*(TMath::Erf((x-4.62331)/11.76726)*TMath::Exp(-1.19879*x)))+0.97117;
      else if (idx == 16  ) num = (94.89744*(TMath::Erf((x-4.45645)/10.86062)*TMath::Exp(-1.17134*x)))+0.96651;
      else if (idx == 17  ) num = (87.50820*(TMath::Erf((x-4.65958)/6.36055)*TMath::Exp(-1.34730*x)))+0.97157;
      else if (idx == 18  ) num = (66.10450*(TMath::Erf((x-4.71580)/3.64308)*TMath::Exp(-1.40799*x)))+0.97343;
      else if (idx == 19  ) num = (94.83409*(TMath::Erf((x-4.65729)/9.72353)*TMath::Exp(-1.26519*x)))+0.97197;
      else if (idx == 20  ) num = (78.20383*(TMath::Erf((x-4.64870)/17.83662)*TMath::Exp(-1.05419*x)))+0.97044;
      else if (idx == 21  ) num = (94.99855*(TMath::Erf((x-4.66201)/11.91428)*TMath::Exp(-1.19460*x)))+0.97266;
      else if (idx == 22  ) num = (94.96904*(TMath::Erf((x-4.52421)/11.67776)*TMath::Exp(-1.18223*x)))+0.96691;
      else if (idx == 23  ) num = (94.99995*(TMath::Erf((x-4.65300)/12.04411)*TMath::Exp(-1.19896*x)))+0.97177;
      else if (idx == 24  ) num = (94.99682*(TMath::Erf((x-4.84775)/12.85355)*TMath::Exp(-1.22262*x)))+0.97400;
      else if (idx == 25  ) num = (94.99661*(TMath::Erf((x-4.50107)/10.98875)*TMath::Exp(-1.18075*x)))+0.96796;
      else if (idx == 26  ) num = (94.99983*(TMath::Erf((x-4.61115)/11.45850)*TMath::Exp(-1.18780*x)))+0.97029;
      else if (idx == 27  ) num = (94.99305*(TMath::Erf((x-4.70487)/12.13197)*TMath::Exp(-1.20791*x)))+0.97283;
      else if (idx == 28  ) num = (44.29690*(TMath::Erf((x-4.69910)/26.06779)*TMath::Exp(-0.83284*x)))+0.96828;
      else if (idx == 29  ) num = (94.77408*(TMath::Erf((x-4.60268)/11.96296)*TMath::Exp(-1.19120*x)))+0.96901;
      else if (idx == 30  ) num = (94.77141*(TMath::Erf((x-4.79946)/13.00626)*TMath::Exp(-1.21447*x)))+0.97382;
      else if (idx == 31  ) num = (87.55387*(TMath::Erf((x-4.94007)/13.14558)*TMath::Exp(-1.22848*x)))+0.97418;
      else if (idx == 32  ) num = (94.44417*(TMath::Erf((x-4.86863)/9.94007)*TMath::Exp(-1.29416*x)))+0.97433;
      else if (idx == 33  ) num = (94.97443*(TMath::Erf((x-4.58699)/11.56103)*TMath::Exp(-1.19616*x)))+0.97084;
      else if (idx == 34  ) num = (94.81010*(TMath::Erf((x-4.69612)/12.39694)*TMath::Exp(-1.20082*x)))+0.97333;
      else if (idx == 35  ) num = (90.25277*(TMath::Erf((x-4.58478)/6.26267)*TMath::Exp(-1.34213*x)))+0.97110;
      else if (idx == 36  ) num = (91.52795*(TMath::Erf((x-4.65180)/12.03214)*TMath::Exp(-1.19813*x)))+0.97232;
      else if (idx == 37  ) num = (94.97915*(TMath::Erf((x-4.65568)/12.00704)*TMath::Exp(-1.19954*x)))+0.97213;
      else if (idx == 38  ) num = (94.99572*(TMath::Erf((x-4.64937)/12.02524)*TMath::Exp(-1.19780*x)))+0.97183;
      else if (idx == 39  ) num = (73.31845*(TMath::Erf((x-4.88777)/4.55038)*TMath::Exp(-1.44690*x)))+0.97474;
      else if (idx == 40  ) num = (94.99933*(TMath::Erf((x-4.53351)/11.54946)*TMath::Exp(-1.17644*x)))+0.96800;
      else if (idx == 41  ) num = (89.78883*(TMath::Erf((x-4.52652)/7.32823)*TMath::Exp(-1.28358*x)))+0.96967;
      else if (idx == 42  ) num = (94.99618*(TMath::Erf((x-4.73141)/12.22818)*TMath::Exp(-1.20517*x)))+0.97331;
      else if (idx == 43  ) num = (94.66276*(TMath::Erf((x-4.51781)/11.16381)*TMath::Exp(-1.18168*x)))+0.96943;
      else if (idx == 44  ) num = (94.99132*(TMath::Erf((x-4.71500)/12.27223)*TMath::Exp(-1.20019*x)))+0.97157;
      else if (idx == 45  ) num = (94.66379*(TMath::Erf((x-4.60445)/11.83360)*TMath::Exp(-1.19224*x)))+0.97076;
      else if (idx == 46  ) num = (93.83842*(TMath::Erf((x-4.58433)/11.88134)*TMath::Exp(-1.19382*x)))+0.96967;
      else if (idx == 47  ) num = (94.21065*(TMath::Erf((x-4.82162)/9.34454)*TMath::Exp(-1.30205*x)))+0.97475;
      else if (idx == 48  ) num = (65.99751*(TMath::Erf((x-4.81962)/4.08317)*TMath::Exp(-1.41042*x)))+0.97629;
      else if (idx == 49  ) num = (72.83815*(TMath::Erf((x-4.99256)/4.47154)*TMath::Exp(-1.46970*x)))+0.97603;
      else if (idx == 50  ) num = (94.99765*(TMath::Erf((x-4.65620)/11.83947)*TMath::Exp(-1.19555*x)))+0.97139;
      else if (idx == 51  ) num = (70.51143*(TMath::Erf((x-4.63435)/3.95561)*TMath::Exp(-1.40652*x)))+0.97192;
      else if (idx == 52  ) num = (69.66583*(TMath::Erf((x-4.66480)/3.91681)*TMath::Exp(-1.39171*x)))+0.97297;
      else if (idx == 53  ) num = (94.88359*(TMath::Erf((x-4.65220)/12.05880)*TMath::Exp(-1.19975*x)))+0.97167;
      else if (idx == 54  ) num = (94.99851*(TMath::Erf((x-4.65762)/12.02483)*TMath::Exp(-1.19920*x)))+0.97080;
      else if (idx == 55  ) num = (94.99486*(TMath::Erf((x-4.63045)/17.05371)*TMath::Exp(-1.10085*x)))+0.96977;
      else if (idx == 56  ) num = (87.86886*(TMath::Erf((x-4.74790)/12.56930)*TMath::Exp(-1.20192*x)))+0.97249;
      else if (idx == 57  ) num = (90.35521*(TMath::Erf((x-4.73021)/12.41028)*TMath::Exp(-1.20913*x)))+0.97084;
      else if (idx == 58  ) num = (94.06603*(TMath::Erf((x-4.60092)/12.51100)*TMath::Exp(-1.17370*x)))+0.97032;
      else if (idx == 59  ) num = (94.99525*(TMath::Erf((x-4.66555)/11.82863)*TMath::Exp(-1.19454*x)))+0.97174;
      else if (idx == 60  ) num = (86.69217*(TMath::Erf((x-4.70928)/6.19983)*TMath::Exp(-1.37023*x)))+0.97359;
      else if (idx == 61  ) num = (94.99850*(TMath::Erf((x-4.71949)/12.05635)*TMath::Exp(-1.20390*x)))+0.97226;
      else if (idx == 62  ) num = (72.69413*(TMath::Erf((x-4.77436)/4.94196)*TMath::Exp(-1.37164*x)))+0.97457;
      else if (idx == 63  ) num = (89.83443*(TMath::Erf((x-4.85795)/13.04397)*TMath::Exp(-1.21481*x)))+0.97410;
      else if (idx == 64  ) num = (94.70542*(TMath::Erf((x-4.69658)/8.43625)*TMath::Exp(-1.30074*x)))+0.97166;
      else if (idx == 65  ) num = (62.63474*(TMath::Erf((x-4.73898)/19.57202)*TMath::Exp(-0.99859*x)))+0.97067;
      else if (idx == 66  ) num = (64.74040*(TMath::Erf((x-4.69748)/3.58377)*TMath::Exp(-1.42086*x)))+0.97286;
      else if (idx == 67  ) num = (94.94048*(TMath::Erf((x-4.60896)/11.58186)*TMath::Exp(-1.19609*x)))+0.97076;
      else if (idx == 68  ) num = (69.78515*(TMath::Erf((x-4.71815)/4.01176)*TMath::Exp(-1.39718*x)))+0.97361;
      else if (idx == 69  ) num = (94.62570*(TMath::Erf((x-4.65257)/9.57704)*TMath::Exp(-1.25720*x)))+0.97130;
      else if (idx == 70  ) num = (94.99380*(TMath::Erf((x-4.67146)/12.05913)*TMath::Exp(-1.19932*x)))+0.97091;
      else if (idx == 71  ) num = (57.13329*(TMath::Erf((x-4.72206)/28.13261)*TMath::Exp(-0.87566*x)))+0.96851;
      else if (idx == 72  ) num = (93.46428*(TMath::Erf((x-4.63378)/15.81824)*TMath::Exp(-1.12577*x)))+0.96861;
      else if (idx == 73  ) num = (90.34133*(TMath::Erf((x-4.66296)/12.17710)*TMath::Exp(-1.20174*x)))+0.97097;
      else if (idx == 74  ) num = (88.47162*(TMath::Erf((x-4.65067)/6.25061)*TMath::Exp(-1.35797*x)))+0.97166;
      else if (idx == 75  ) num = (94.04090*(TMath::Erf((x-4.66223)/12.13374)*TMath::Exp(-1.20077*x)))+0.97106;
      else if (idx == 76  ) num = (90.03620*(TMath::Erf((x-4.82395)/12.85090)*TMath::Exp(-1.21110*x)))+0.97477;
      else if (idx == 77  ) num = (61.24387*(TMath::Erf((x-4.71433)/3.14999)*TMath::Exp(-1.44521*x)))+0.97373;
      else if (idx == 78  ) num = (81.47735*(TMath::Erf((x-4.82441)/6.37854)*TMath::Exp(-1.36257*x)))+0.97494;
      else if (idx == 79  ) num = (87.30748*(TMath::Erf((x-4.73379)/5.84634)*TMath::Exp(-1.39963*x)))+0.97206;
      else if (idx == 80  ) num = (94.71240*(TMath::Erf((x-4.65836)/14.37652)*TMath::Exp(-1.15380*x)))+0.97094;
      else if (idx == 81  ) num = (92.00053*(TMath::Erf((x-4.65638)/12.10971)*TMath::Exp(-1.20058*x)))+0.97120;
      else if (idx == 82  ) num = (94.99782*(TMath::Erf((x-4.60157)/11.63455)*TMath::Exp(-1.18850*x)))+0.96984;
      else if (idx == 83  ) num = (93.43307*(TMath::Erf((x-4.65505)/15.17349)*TMath::Exp(-1.13324*x)))+0.97094;
      else if (idx == 84  ) num = (94.86510*(TMath::Erf((x-4.72373)/10.48185)*TMath::Exp(-1.25025*x)))+0.97195;
      else if (idx == 85  ) num = (94.97560*(TMath::Erf((x-4.73223)/12.47543)*TMath::Exp(-1.21116*x)))+0.97159;
      else if (idx == 86  ) num = (94.99801*(TMath::Erf((x-4.73898)/12.34281)*TMath::Exp(-1.20516*x)))+0.97376;
      else if (idx == 87  ) num = (74.67261*(TMath::Erf((x-4.69765)/4.70693)*TMath::Exp(-1.39152*x)))+0.97407;
      else if (idx == 88  ) num = (87.21237*(TMath::Erf((x-4.90188)/13.09709)*TMath::Exp(-1.21576*x)))+0.97720;
      else if (idx == 89  ) num = (94.88654*(TMath::Erf((x-4.55950)/12.18522)*TMath::Exp(-1.17588*x)))+0.96823;
      else if (idx == 90  ) num = (92.23613*(TMath::Erf((x-4.61961)/12.04057)*TMath::Exp(-1.19374*x)))+0.97083;
      else if (idx == 91  ) num = (92.86242*(TMath::Erf((x-4.65513)/12.05149)*TMath::Exp(-1.19845*x)))+0.97235;
      else if (idx == 92  ) num = (82.40165*(TMath::Erf((x-4.54869)/5.57244)*TMath::Exp(-1.33748*x)))+0.96874;
      else if (idx == 93  ) num = (70.21317*(TMath::Erf((x-4.61956)/21.19427)*TMath::Exp(-0.98357*x)))+0.96735;
      else if (idx == 94  ) num = (91.49496*(TMath::Erf((x-4.65551)/12.09586)*TMath::Exp(-1.19970*x)))+0.97148;
      else if (idx == 95  ) num = (94.99999*(TMath::Erf((x-4.64991)/11.93991)*TMath::Exp(-1.19620*x)))+0.97271;
      else if (idx == 96  ) num = (71.88374*(TMath::Erf((x-4.62231)/17.12067)*TMath::Exp(-1.04647*x)))+0.96925;
      else if (idx == 97  ) num = (62.67252*(TMath::Erf((x-4.72860)/3.54308)*TMath::Exp(-1.41956*x)))+0.97409;
      else if (idx == 98  ) num = (94.97456*(TMath::Erf((x-4.72508)/10.70812)*TMath::Exp(-1.24305*x)))+0.97361;
      else if (idx == 99  ) num = (50.77249*(TMath::Erf((x-4.52660)/25.02548)*TMath::Exp(-0.84883*x)))+0.96302;
      else if (idx == 100 ) num = (88.08939*(TMath::Erf((x-4.66162)/3.30229)*TMath::Exp(-1.49917*x)))+0.97405;
   }
   else if (fabs(eta)<1.8)
   {
      if (idx==0) num = (89.05209*(TMath::Erf((x-5.52785)/8.71512)*TMath::Exp(-1.49971*x)))+0.95930;
      else if (idx == -1   ) num = (89.12551*(TMath::Erf((x-5.52881)/8.70812)*TMath::Exp(-1.49976*x)))+0.96031;
      else if (idx == -2   ) num = (88.97073*(TMath::Erf((x-5.52656)/8.72354)*TMath::Exp(-1.49915*x)))+0.95829;
      else if (idx == 1   ) num = (83.43315*(TMath::Erf((x-5.67185)/8.99081)*TMath::Exp(-1.48388*x)))+0.95666;
      else if (idx == 2   ) num = (89.04742*(TMath::Erf((x-5.52749)/8.71655)*TMath::Exp(-1.49994*x)))+0.95832;
      else if (idx == 3   ) num = (88.88984*(TMath::Erf((x-5.52333)/8.73106)*TMath::Exp(-1.49983*x)))+0.95948;
      else if (idx == 4   ) num = (89.07040*(TMath::Erf((x-5.53019)/8.71287)*TMath::Exp(-1.49510*x)))+0.95993;
      else if (idx == 5   ) num = (94.66695*(TMath::Erf((x-5.01455)/7.98241)*TMath::Exp(-1.48812*x)))+0.95401;
      else if (idx == 6   ) num = (92.91591*(TMath::Erf((x-5.30325)/8.23101)*TMath::Exp(-1.50000*x)))+0.95895;
      else if (idx == 7   ) num = (89.02993*(TMath::Erf((x-5.52879)/8.71723)*TMath::Exp(-1.49619*x)))+0.95907;
      else if (idx == 8   ) num = (88.96090*(TMath::Erf((x-5.52692)/8.72489)*TMath::Exp(-1.49584*x)))+0.95802;
      else if (idx == 9   ) num = (89.17976*(TMath::Erf((x-5.53078)/8.70198)*TMath::Exp(-1.49761*x)))+0.96093;
      else if (idx == 10  ) num = (88.13217*(TMath::Erf((x-5.50850)/8.79784)*TMath::Exp(-1.49996*x)))+0.95536;
      else if (idx == 11  ) num = (94.73034*(TMath::Erf((x-4.97844)/7.94415)*TMath::Exp(-1.47906*x)))+0.95407;
      else if (idx == 12  ) num = (88.79622*(TMath::Erf((x-5.52534)/8.73933)*TMath::Exp(-1.49991*x)))+0.95829;
      else if (idx == 13  ) num = (88.74006*(TMath::Erf((x-5.52063)/8.74449)*TMath::Exp(-1.49986*x)))+0.95917;
      else if (idx == 14  ) num = (88.93277*(TMath::Erf((x-5.53019)/8.72642)*TMath::Exp(-1.49571*x)))+0.95875;
      else if (idx == 15  ) num = (89.11438*(TMath::Erf((x-5.52757)/8.70888)*TMath::Exp(-1.49899*x)))+0.95981;
      else if (idx == 16  ) num = (94.61605*(TMath::Erf((x-5.03637)/8.07439)*TMath::Exp(-1.48508*x)))+0.95188;
      else if (idx == 17  ) num = (88.54205*(TMath::Erf((x-5.52085)/8.76460)*TMath::Exp(-1.49999*x)))+0.95649;
      else if (idx == 18  ) num = (88.98490*(TMath::Erf((x-5.52368)/8.72234)*TMath::Exp(-1.49800*x)))+0.95773;
      else if (idx == 19  ) num = (80.33534*(TMath::Erf((x-6.03362)/9.34806)*TMath::Exp(-1.49780*x)))+0.96418;
      else if (idx == 20  ) num = (88.80467*(TMath::Erf((x-5.52388)/8.74075)*TMath::Exp(-1.49745*x)))+0.95607;
      else if (idx == 21  ) num = (91.85418*(TMath::Erf((x-5.38995)/8.47945)*TMath::Exp(-1.49918*x)))+0.95887;
      else if (idx == 22  ) num = (88.71131*(TMath::Erf((x-5.51835)/8.74677)*TMath::Exp(-1.49984*x)))+0.95959;
      else if (idx == 23  ) num = (94.97609*(TMath::Erf((x-4.95768)/7.85311)*TMath::Exp(-1.47888*x)))+0.95380;
      else if (idx == 24  ) num = (92.62486*(TMath::Erf((x-5.46128)/8.60541)*TMath::Exp(-1.49988*x)))+0.96231;
      else if (idx == 25  ) num = (81.87588*(TMath::Erf((x-5.76508)/9.18714)*TMath::Exp(-1.47641*x)))+0.96186;
      else if (idx == 26  ) num = (89.37899*(TMath::Erf((x-5.53504)/8.68084)*TMath::Exp(-1.49403*x)))+0.96331;
      else if (idx == 27  ) num = (89.04842*(TMath::Erf((x-5.52670)/8.71621)*TMath::Exp(-1.48833*x)))+0.95868;
      else if (idx == 28  ) num = (94.74298*(TMath::Erf((x-5.01642)/10.13190)*TMath::Exp(-1.40143*x)))+0.95521;
      else if (idx == 29  ) num = (94.70847*(TMath::Erf((x-5.14729)/7.96790)*TMath::Exp(-1.49608*x)))+0.95739;
      else if (idx == 30  ) num = (79.43257*(TMath::Erf((x-6.09146)/9.39226)*TMath::Exp(-1.49899*x)))+0.96613;
      else if (idx == 31  ) num = (92.24172*(TMath::Erf((x-5.35760)/8.40077)*TMath::Exp(-1.49971*x)))+0.95900;
      else if (idx == 32  ) num = (81.51035*(TMath::Erf((x-5.94492)/9.28026)*TMath::Exp(-1.49271*x)))+0.96423;
      else if (idx == 33  ) num = (88.51196*(TMath::Erf((x-5.51039)/8.76484)*TMath::Exp(-1.49990*x)))+0.95790;
      else if (idx == 34  ) num = (92.66244*(TMath::Erf((x-5.35736)/8.38971)*TMath::Exp(-1.49925*x)))+0.95877;
      else if (idx == 35  ) num = (82.55719*(TMath::Erf((x-5.86779)/9.23469)*TMath::Exp(-1.48609*x)))+0.96274;
      else if (idx == 36  ) num = (89.03122*(TMath::Erf((x-5.52435)/8.71803)*TMath::Exp(-1.49848*x)))+0.95835;
      else if (idx == 37  ) num = (93.70002*(TMath::Erf((x-5.20472)/8.09468)*TMath::Exp(-1.49997*x)))+0.95788;
      else if (idx == 38  ) num = (90.75095*(TMath::Erf((x-5.44876)/8.63745)*TMath::Exp(-1.49982*x)))+0.96033;
      else if (idx == 39  ) num = (89.16938*(TMath::Erf((x-5.52897)/8.70337)*TMath::Exp(-1.49667*x)))+0.96031;
      else if (idx == 40  ) num = (89.28051*(TMath::Erf((x-5.52757)/8.69235)*TMath::Exp(-1.49802*x)))+0.96111;
      else if (idx == 41  ) num = (94.09275*(TMath::Erf((x-5.23046)/8.01126)*TMath::Exp(-1.49699*x)))+0.95608;
      else if (idx == 42  ) num = (89.24151*(TMath::Erf((x-5.53018)/8.69564)*TMath::Exp(-1.49660*x)))+0.96138;
      else if (idx == 43  ) num = (89.02932*(TMath::Erf((x-5.52671)/8.71788)*TMath::Exp(-1.49988*x)))+0.95880;
      else if (idx == 44  ) num = (77.67482*(TMath::Erf((x-5.92911)/9.48610)*TMath::Exp(-1.46497*x)))+0.96241;
      else if (idx == 45  ) num = (80.21228*(TMath::Erf((x-5.96984)/9.28765)*TMath::Exp(-1.49807*x)))+0.95974;
      else if (idx == 46  ) num = (92.07915*(TMath::Erf((x-5.27187)/8.11449)*TMath::Exp(-1.49976*x)))+0.95890;
      else if (idx == 47  ) num = (79.25122*(TMath::Erf((x-5.94135)/9.31784)*TMath::Exp(-1.48466*x)))+0.96159;
      else if (idx == 48  ) num = (88.39300*(TMath::Erf((x-5.51368)/8.77340)*TMath::Exp(-1.49989*x)))+0.95778;
      else if (idx == 49  ) num = (89.06522*(TMath::Erf((x-5.52655)/8.71462)*TMath::Exp(-1.49986*x)))+0.95964;
      else if (idx == 50  ) num = (89.11520*(TMath::Erf((x-5.53114)/8.70812)*TMath::Exp(-1.49764*x)))+0.96050;
      else if (idx == 51  ) num = (93.73062*(TMath::Erf((x-4.96049)/7.09414)*TMath::Exp(-1.49952*x)))+0.95837;
      else if (idx == 52  ) num = (88.81010*(TMath::Erf((x-5.52157)/8.73991)*TMath::Exp(-1.49995*x)))+0.95798;
      else if (idx == 53  ) num = (89.11843*(TMath::Erf((x-5.53250)/8.70757)*TMath::Exp(-1.49670*x)))+0.96072;
      else if (idx == 54  ) num = (88.70819*(TMath::Erf((x-5.52190)/8.74674)*TMath::Exp(-1.49987*x)))+0.95896;
      else if (idx == 55  ) num = (88.35970*(TMath::Erf((x-5.51113)/8.77206)*TMath::Exp(-1.49984*x)))+0.96087;
      else if (idx == 56  ) num = (89.23891*(TMath::Erf((x-5.53118)/8.69657)*TMath::Exp(-1.49241*x)))+0.96183;
      else if (idx == 57  ) num = (89.17267*(TMath::Erf((x-5.52907)/8.70335)*TMath::Exp(-1.49697*x)))+0.96046;
      else if (idx == 58  ) num = (83.61700*(TMath::Erf((x-5.78547)/9.14316)*TMath::Exp(-1.48631*x)))+0.96046;
      else if (idx == 59  ) num = (94.99357*(TMath::Erf((x-5.06924)/7.70099)*TMath::Exp(-1.49537*x)))+0.95532;
      else if (idx == 60  ) num = (88.91357*(TMath::Erf((x-5.52349)/8.72886)*TMath::Exp(-1.49982*x)))+0.95947;
      else if (idx == 61  ) num = (94.33278*(TMath::Erf((x-5.10723)/8.17567)*TMath::Exp(-1.49728*x)))+0.95493;
      else if (idx == 62  ) num = (89.29068*(TMath::Erf((x-5.53490)/8.69037)*TMath::Exp(-1.49907*x)))+0.96255;
      else if (idx == 63  ) num = (79.28993*(TMath::Erf((x-6.02026)/9.33466)*TMath::Exp(-1.49830*x)))+0.95871;
      else if (idx == 64  ) num = (89.14854*(TMath::Erf((x-5.52664)/8.70793)*TMath::Exp(-1.49988*x)))+0.96055;
      else if (idx == 65  ) num = (91.59700*(TMath::Erf((x-5.30335)/8.33099)*TMath::Exp(-1.49886*x)))+0.95999;
      else if (idx == 66  ) num = (79.38170*(TMath::Erf((x-5.99373)/9.43124)*TMath::Exp(-1.48120*x)))+0.96539;
      else if (idx == 67  ) num = (88.94457*(TMath::Erf((x-5.52447)/8.72709)*TMath::Exp(-1.49996*x)))+0.95843;
      else if (idx == 68  ) num = (86.22894*(TMath::Erf((x-5.85913)/9.19955)*TMath::Exp(-1.49837*x)))+0.96341;
      else if (idx == 69  ) num = (79.61784*(TMath::Erf((x-6.00011)/9.25900)*TMath::Exp(-1.49992*x)))+0.96062;
      else if (idx == 70  ) num = (84.91294*(TMath::Erf((x-5.88501)/9.14199)*TMath::Exp(-1.50000*x)))+0.96250;
      else if (idx == 71  ) num = (89.00087*(TMath::Erf((x-5.52810)/8.71976)*TMath::Exp(-1.49635*x)))+0.95872;
      else if (idx == 72  ) num = (82.20937*(TMath::Erf((x-5.87057)/9.22971)*TMath::Exp(-1.48520*x)))+0.96444;
      else if (idx == 73  ) num = (88.42288*(TMath::Erf((x-5.50878)/8.76941)*TMath::Exp(-1.49986*x)))+0.95886;
      else if (idx == 74  ) num = (89.08328*(TMath::Erf((x-5.52870)/8.71181)*TMath::Exp(-1.49651*x)))+0.95927;
      else if (idx == 75  ) num = (89.15377*(TMath::Erf((x-5.52631)/8.70520)*TMath::Exp(-1.49834*x)))+0.95986;
      else if (idx == 76  ) num = (89.71352*(TMath::Erf((x-5.52758)/8.75689)*TMath::Exp(-1.50000*x)))+0.95881;
      else if (idx == 77  ) num = (81.22932*(TMath::Erf((x-5.83316)/9.18570)*TMath::Exp(-1.48508*x)))+0.96172;
      else if (idx == 78  ) num = (94.65939*(TMath::Erf((x-5.13522)/7.99269)*TMath::Exp(-1.49423*x)))+0.95677;
      else if (idx == 79  ) num = (88.59085*(TMath::Erf((x-5.51353)/8.75763)*TMath::Exp(-1.49987*x)))+0.95876;
      else if (idx == 80  ) num = (89.77408*(TMath::Erf((x-5.63202)/8.89499)*TMath::Exp(-1.49955*x)))+0.96562;
      else if (idx == 81  ) num = (92.62569*(TMath::Erf((x-5.34545)/8.43095)*TMath::Exp(-1.49820*x)))+0.95863;
      else if (idx == 82  ) num = (88.91802*(TMath::Erf((x-5.52443)/8.72953)*TMath::Exp(-1.49988*x)))+0.95728;
      else if (idx == 83  ) num = (85.21890*(TMath::Erf((x-5.85159)/9.02441)*TMath::Exp(-1.49984*x)))+0.96318;
      else if (idx == 84  ) num = (92.77635*(TMath::Erf((x-5.34063)/8.35243)*TMath::Exp(-1.49984*x)))+0.95979;
      else if (idx == 85  ) num = (88.99136*(TMath::Erf((x-5.52956)/8.72116)*TMath::Exp(-1.49895*x)))+0.95890;
      else if (idx == 86  ) num = (88.75413*(TMath::Erf((x-5.52550)/8.74387)*TMath::Exp(-1.49993*x)))+0.95808;
      else if (idx == 87  ) num = (88.30803*(TMath::Erf((x-5.51157)/8.78547)*TMath::Exp(-1.50000*x)))+0.95359;
      else if (idx == 88  ) num = (88.90641*(TMath::Erf((x-5.52313)/8.72972)*TMath::Exp(-1.49999*x)))+0.95945;
      else if (idx == 89  ) num = (88.86574*(TMath::Erf((x-5.52324)/8.73462)*TMath::Exp(-1.49842*x)))+0.95635;
      else if (idx == 90  ) num = (89.18839*(TMath::Erf((x-5.53040)/8.70352)*TMath::Exp(-1.49979*x)))+0.96246;
      else if (idx == 91  ) num = (88.71181*(TMath::Erf((x-5.51831)/8.75003)*TMath::Exp(-1.49881*x)))+0.95469;
      else if (idx == 92  ) num = (89.08875*(TMath::Erf((x-5.53207)/8.71085)*TMath::Exp(-1.49949*x)))+0.96029;
      else if (idx == 93  ) num = (89.16084*(TMath::Erf((x-5.52794)/8.70453)*TMath::Exp(-1.49642*x)))+0.96017;
      else if (idx == 94  ) num = (88.82146*(TMath::Erf((x-5.52651)/8.73675)*TMath::Exp(-1.49987*x)))+0.95908;
      else if (idx == 95  ) num = (89.04055*(TMath::Erf((x-5.52557)/8.71760)*TMath::Exp(-1.49983*x)))+0.96025;
      else if (idx == 96  ) num = (94.55319*(TMath::Erf((x-5.19560)/7.96830)*TMath::Exp(-1.49894*x)))+0.95829;
      else if (idx == 97  ) num = (86.32300*(TMath::Erf((x-5.85578)/9.08769)*TMath::Exp(-1.49977*x)))+0.96585;
      else if (idx == 98  ) num = (92.72874*(TMath::Erf((x-5.13193)/8.13091)*TMath::Exp(-1.49232*x)))+0.95128;
      else if (idx == 99  ) num = (90.91086*(TMath::Erf((x-5.52293)/8.73307)*TMath::Exp(-1.49943*x)))+0.96242;
      else if (idx == 100 ) num = (89.17352*(TMath::Erf((x-5.53027)/8.70326)*TMath::Exp(-1.49552*x)))+0.96060;
   }
   else if (fabs(eta)<2.1)
   {
      if (idx==0) num = (17.20847*(TMath::Erf((x-3.27157)/49.82961)*TMath::Exp(-0.35612*x)))+0.81176;
      else if (idx == -1   ) num = (16.86904*(TMath::Erf((x-3.23643)/49.99575)*TMath::Exp(-0.34727*x)))+0.80853;
      else if (idx == -2   ) num = (17.70207*(TMath::Erf((x-3.30571)/49.83999)*TMath::Exp(-0.36590*x)))+0.81509;
      else if (idx == 1   ) num = (15.49000*(TMath::Erf((x-2.43386)/49.82813)*TMath::Exp(-0.25840*x)))+0.67731;
      else if (idx == 2   ) num = (15.40804*(TMath::Erf((x-2.75900)/49.28762)*TMath::Exp(-0.29172*x)))+0.74155;
      else if (idx == 3   ) num = (17.45000*(TMath::Erf((x-3.15532)/50.00000)*TMath::Exp(-0.34546*x)))+0.79670;
      else if (idx == 4   ) num = (16.39787*(TMath::Erf((x-3.21805)/49.69005)*TMath::Exp(-0.33527*x)))+0.80678;
      else if (idx == 5   ) num = (16.32908*(TMath::Erf((x-3.53504)/45.13954)*TMath::Exp(-0.39712*x)))+0.84142;
      else if (idx == 6   ) num = (17.01456*(TMath::Erf((x-3.86788)/44.29352)*TMath::Exp(-0.46746*x)))+0.87344;
      else if (idx == 7   ) num = (16.53103*(TMath::Erf((x-3.23881)/49.90544)*TMath::Exp(-0.34266*x)))+0.80927;
      else if (idx == 8   ) num = (17.86591*(TMath::Erf((x-3.90151)/44.40302)*TMath::Exp(-0.47824*x)))+0.87447;
      else if (idx == 9   ) num = (15.55758*(TMath::Erf((x-3.17321)/49.86156)*TMath::Exp(-0.33053*x)))+0.79906;
      else if (idx == 10  ) num = (16.89975*(TMath::Erf((x-3.48719)/47.40364)*TMath::Exp(-0.38923*x)))+0.83771;
      else if (idx == 11  ) num = (16.04244*(TMath::Erf((x-3.49432)/48.30884)*TMath::Exp(-0.38115*x)))+0.83672;
      else if (idx == 12  ) num = (14.21628*(TMath::Erf((x-3.76490)/40.91119)*TMath::Exp(-0.43326*x)))+0.86392;
      else if (idx == 13  ) num = (17.20816*(TMath::Erf((x-3.19855)/49.80352)*TMath::Exp(-0.34570*x)))+0.80282;
      else if (idx == 14  ) num = (17.31204*(TMath::Erf((x-3.45456)/47.17680)*TMath::Exp(-0.38370*x)))+0.83755;
      else if (idx == 15  ) num = (15.93246*(TMath::Erf((x-3.30666)/49.79542)*TMath::Exp(-0.34297*x)))+0.81425;
      else if (idx == 16  ) num = (18.65453*(TMath::Erf((x-2.87390)/49.98133)*TMath::Exp(-0.32610*x)))+0.75220;
      else if (idx == 17  ) num = (18.70397*(TMath::Erf((x-3.30603)/47.62654)*TMath::Exp(-0.39279*x)))+0.82079;
      else if (idx == 18  ) num = (16.78795*(TMath::Erf((x-2.86425)/48.86033)*TMath::Exp(-0.31414*x)))+0.75301;
      else if (idx == 19  ) num = (16.08935*(TMath::Erf((x-2.71049)/48.36827)*TMath::Exp(-0.28874*x)))+0.72414;
      else if (idx == 20  ) num = (17.89248*(TMath::Erf((x-3.35446)/47.77778)*TMath::Exp(-0.38217*x)))+0.82275;
      else if (idx == 21  ) num = (17.15665*(TMath::Erf((x-3.10874)/49.37506)*TMath::Exp(-0.33474*x)))+0.79165;
      else if (idx == 22  ) num = (17.77752*(TMath::Erf((x-3.13938)/48.83655)*TMath::Exp(-0.35086*x)))+0.79567;
      else if (idx == 23  ) num = (17.05667*(TMath::Erf((x-3.26890)/49.93147)*TMath::Exp(-0.35740*x)))+0.81176;
      else if (idx == 24  ) num = (16.11124*(TMath::Erf((x-3.35380)/49.98440)*TMath::Exp(-0.35251*x)))+0.81711;
      else if (idx == 25  ) num = (18.11222*(TMath::Erf((x-3.50973)/49.51672)*TMath::Exp(-0.39102*x)))+0.84067;
      else if (idx == 26  ) num = (16.92788*(TMath::Erf((x-3.21563)/49.60192)*TMath::Exp(-0.34308*x)))+0.80574;
      else if (idx == 27  ) num = (15.55957*(TMath::Erf((x-2.42131)/49.29775)*TMath::Exp(-0.25463*x)))+0.67133;
      else if (idx == 28  ) num = (16.98332*(TMath::Erf((x-3.10389)/49.91584)*TMath::Exp(-0.32919*x)))+0.78669;
      else if (idx == 29  ) num = (17.79041*(TMath::Erf((x-3.22234)/48.86945)*TMath::Exp(-0.36394*x)))+0.80865;
      else if (idx == 30  ) num = (15.87498*(TMath::Erf((x-2.54635)/49.94816)*TMath::Exp(-0.26577*x)))+0.69542;
      else if (idx == 31  ) num = (17.91060*(TMath::Erf((x-3.75388)/48.35793)*TMath::Exp(-0.43073*x)))+0.86416;
      else if (idx == 32  ) num = (17.35261*(TMath::Erf((x-3.10080)/49.99997)*TMath::Exp(-0.33443*x)))+0.78473;
      else if (idx == 33  ) num = (17.39706*(TMath::Erf((x-3.27944)/49.39748)*TMath::Exp(-0.35846*x)))+0.80883;
      else if (idx == 34  ) num = (11.45759*(TMath::Erf((x-3.75912)/34.33284)*TMath::Exp(-0.41303*x)))+0.85921;
      else if (idx == 35  ) num = (14.56090*(TMath::Erf((x-3.59141)/41.17572)*TMath::Exp(-0.40193*x)))+0.84658;
      else if (idx == 36  ) num = (17.80473*(TMath::Erf((x-3.31146)/48.64894)*TMath::Exp(-0.37565*x)))+0.82207;
      else if (idx == 37  ) num = (17.20782*(TMath::Erf((x-3.34685)/49.76225)*TMath::Exp(-0.36071*x)))+0.82106;
      else if (idx == 38  ) num = (16.80046*(TMath::Erf((x-2.71903)/49.94536)*TMath::Exp(-0.29184*x)))+0.72990;
      else if (idx == 39  ) num = (16.20979*(TMath::Erf((x-3.11792)/49.52147)*TMath::Exp(-0.32572*x)))+0.78830;
      else if (idx == 40  ) num = (16.22471*(TMath::Erf((x-3.67165)/45.99908)*TMath::Exp(-0.41007*x)))+0.85480;
      else if (idx == 41  ) num = (18.49138*(TMath::Erf((x-3.45116)/48.49419)*TMath::Exp(-0.40902*x)))+0.83784;
      else if (idx == 42  ) num = (16.33662*(TMath::Erf((x-3.45084)/49.97022)*TMath::Exp(-0.36277*x)))+0.83432;
      else if (idx == 43  ) num = (16.05979*(TMath::Erf((x-3.83305)/48.38801)*TMath::Exp(-0.40995*x)))+0.86511;
      else if (idx == 44  ) num = (15.69696*(TMath::Erf((x-3.33713)/49.84071)*TMath::Exp(-0.33503*x)))+0.82011;
      else if (idx == 45  ) num = (17.08901*(TMath::Erf((x-3.33523)/49.69294)*TMath::Exp(-0.36818*x)))+0.82388;
      else if (idx == 46  ) num = (18.04747*(TMath::Erf((x-3.07534)/48.60903)*TMath::Exp(-0.34662*x)))+0.78738;
      else if (idx == 47  ) num = (15.05988*(TMath::Erf((x-3.43087)/43.83204)*TMath::Exp(-0.37572*x)))+0.83094;
      else if (idx == 48  ) num = (16.44511*(TMath::Erf((x-2.97892)/49.99778)*TMath::Exp(-0.31094*x)))+0.77038;
      else if (idx == 49  ) num = (19.57597*(TMath::Erf((x-3.62722)/49.65168)*TMath::Exp(-0.43720*x)))+0.85854;
      else if (idx == 50  ) num = (17.64652*(TMath::Erf((x-3.44324)/48.52962)*TMath::Exp(-0.38873*x)))+0.83379;
      else if (idx == 51  ) num = (17.49548*(TMath::Erf((x-3.16634)/49.54011)*TMath::Exp(-0.35419*x)))+0.80168;
      else if (idx == 52  ) num = (17.54496*(TMath::Erf((x-2.96236)/49.78095)*TMath::Exp(-0.32802*x)))+0.76927;
      else if (idx == 53  ) num = (17.52139*(TMath::Erf((x-3.32189)/46.88959)*TMath::Exp(-0.37084*x)))+0.81984;
      else if (idx == 54  ) num = (16.70289*(TMath::Erf((x-3.65103)/42.59072)*TMath::Exp(-0.44469*x)))+0.85617;
      else if (idx == 55  ) num = (18.55955*(TMath::Erf((x-3.29575)/49.64591)*TMath::Exp(-0.38348*x)))+0.81950;
      else if (idx == 56  ) num = (12.47068*(TMath::Erf((x-3.51389)/39.18165)*TMath::Exp(-0.36912*x)))+0.83705;
      else if (idx == 57  ) num = (15.35232*(TMath::Erf((x-2.94479)/49.95243)*TMath::Exp(-0.29794*x)))+0.76297;
      else if (idx == 58  ) num = (15.56832*(TMath::Erf((x-2.59032)/49.95249)*TMath::Exp(-0.26641*x)))+0.70647;
      else if (idx == 59  ) num = (17.21509*(TMath::Erf((x-3.27661)/49.33643)*TMath::Exp(-0.35663*x)))+0.81116;
      else if (idx == 60  ) num = (16.77060*(TMath::Erf((x-3.22443)/48.85805)*TMath::Exp(-0.34593*x)))+0.80628;
      else if (idx == 61  ) num = (17.27043*(TMath::Erf((x-3.20564)/49.99979)*TMath::Exp(-0.34706*x)))+0.80087;
      else if (idx == 62  ) num = (15.30225*(TMath::Erf((x-3.60539)/45.63103)*TMath::Exp(-0.39004*x)))+0.84902;
      else if (idx == 63  ) num = (17.78319*(TMath::Erf((x-3.39558)/49.43444)*TMath::Exp(-0.37634*x)))+0.82514;
      else if (idx == 64  ) num = (15.23687*(TMath::Erf((x-3.19769)/47.76584)*TMath::Exp(-0.32859*x)))+0.79984;
      else if (idx == 65  ) num = (17.88445*(TMath::Erf((x-3.68338)/49.94078)*TMath::Exp(-0.41969*x)))+0.85791;
      else if (idx == 66  ) num = (15.32704*(TMath::Erf((x-3.38703)/49.60343)*TMath::Exp(-0.34236*x)))+0.81841;
      else if (idx == 67  ) num = (14.97311*(TMath::Erf((x-2.80064)/49.97354)*TMath::Exp(-0.28042*x)))+0.74217;
      else if (idx == 68  ) num = (18.64050*(TMath::Erf((x-3.03595)/49.98810)*TMath::Exp(-0.35226*x)))+0.77984;
      else if (idx == 69  ) num = (16.96940*(TMath::Erf((x-3.09220)/49.74069)*TMath::Exp(-0.32710*x)))+0.78576;
      else if (idx == 70  ) num = (14.79553*(TMath::Erf((x-3.00009)/49.35959)*TMath::Exp(-0.29923*x)))+0.76882;
      else if (idx == 71  ) num = (17.59650*(TMath::Erf((x-3.08860)/49.47741)*TMath::Exp(-0.33640*x)))+0.78567;
      else if (idx == 72  ) num = (18.15273*(TMath::Erf((x-3.57642)/49.99510)*TMath::Exp(-0.40146*x)))+0.84965;
      else if (idx == 73  ) num = (13.65324*(TMath::Erf((x-3.59600)/42.33486)*TMath::Exp(-0.37873*x)))+0.84473;
      else if (idx == 74  ) num = (17.08157*(TMath::Erf((x-3.53333)/47.46148)*TMath::Exp(-0.39680*x)))+0.84242;
      else if (idx == 75  ) num = (15.07804*(TMath::Erf((x-3.11234)/48.67282)*TMath::Exp(-0.31227*x)))+0.78953;
      else if (idx == 76  ) num = (11.71675*(TMath::Erf((x-3.81370)/31.16422)*TMath::Exp(-0.45610*x)))+0.86802;
      else if (idx == 77  ) num = (17.45363*(TMath::Erf((x-3.13379)/49.75502)*TMath::Exp(-0.34217*x)))+0.79419;
      else if (idx == 78  ) num = (13.77627*(TMath::Erf((x-3.07877)/49.94990)*TMath::Exp(-0.29293*x)))+0.78303;
      else if (idx == 79  ) num = (17.21858*(TMath::Erf((x-3.41371)/49.62852)*TMath::Exp(-0.36636*x)))+0.82720;
      else if (idx == 80  ) num = (15.15995*(TMath::Erf((x-3.11605)/49.32707)*TMath::Exp(-0.31383*x)))+0.78447;
      else if (idx == 81  ) num = (17.94542*(TMath::Erf((x-3.52676)/46.21184)*TMath::Exp(-0.42463*x)))+0.84722;
      else if (idx == 82  ) num = (14.76073*(TMath::Erf((x-3.03402)/49.67875)*TMath::Exp(-0.29324*x)))+0.77916;
      else if (idx == 83  ) num = (16.38323*(TMath::Erf((x-2.89567)/49.98785)*TMath::Exp(-0.30276*x)))+0.75533;
      else if (idx == 84  ) num = (18.34124*(TMath::Erf((x-3.24551)/47.27025)*TMath::Exp(-0.37799*x)))+0.81216;
      else if (idx == 85  ) num = (17.57581*(TMath::Erf((x-3.26467)/49.76030)*TMath::Exp(-0.36370*x)))+0.80891;
      else if (idx == 86  ) num = (17.76680*(TMath::Erf((x-3.15229)/47.99360)*TMath::Exp(-0.35482*x)))+0.79837;
      else if (idx == 87  ) num = (15.96803*(TMath::Erf((x-3.11460)/49.69405)*TMath::Exp(-0.32714*x)))+0.79296;
      else if (idx == 88  ) num = (17.21121*(TMath::Erf((x-3.46355)/49.29280)*TMath::Exp(-0.38066*x)))+0.83557;
      else if (idx == 89  ) num = (17.42608*(TMath::Erf((x-3.45364)/49.99961)*TMath::Exp(-0.38555*x)))+0.83046;
      else if (idx == 90  ) num = (17.88767*(TMath::Erf((x-3.23148)/49.62954)*TMath::Exp(-0.36620*x)))+0.80709;
      else if (idx == 91  ) num = (16.89160*(TMath::Erf((x-3.77601)/42.84846)*TMath::Exp(-0.46118*x)))+0.86726;
      else if (idx == 92  ) num = (15.97761*(TMath::Erf((x-3.35677)/49.88746)*TMath::Exp(-0.34778*x)))+0.81591;
      else if (idx == 93  ) num = (17.14356*(TMath::Erf((x-3.25442)/49.27423)*TMath::Exp(-0.35588*x)))+0.81317;
      else if (idx == 94  ) num = (17.04177*(TMath::Erf((x-2.88372)/49.95281)*TMath::Exp(-0.30939*x)))+0.75571;
      else if (idx == 95  ) num = (15.53721*(TMath::Erf((x-3.13959)/49.41065)*TMath::Exp(-0.31517*x)))+0.79198;
      else if (idx == 96  ) num = (16.94166*(TMath::Erf((x-3.12771)/49.85655)*TMath::Exp(-0.34196*x)))+0.79431;
      else if (idx == 97  ) num = (18.70747*(TMath::Erf((x-3.51824)/49.98628)*TMath::Exp(-0.40383*x)))+0.84385;
      else if (idx == 98  ) num = (8.72823*(TMath::Erf((x-4.08310)/20.75239)*TMath::Exp(-0.53899*x)))+0.88793;
      else if (idx == 99  ) num = (17.99307*(TMath::Erf((x-3.42732)/46.99686)*TMath::Exp(-0.39412*x)))+0.83119;
      else if (idx == 100 ) num = (16.67104*(TMath::Erf((x-3.98905)/49.23097)*TMath::Exp(-0.43031*x)))+0.87599;
   }
   else
   {
      if (idx==0) num = (8.19515*(TMath::Erf((x-3.58636)/45.28699)*TMath::Exp(-0.45804*x)))+0.87662;
      else if (idx == -1   ) num = (8.91820*(TMath::Erf((x-3.59311)/46.52315)*TMath::Exp(-0.48034*x)))+0.87851;
      else if (idx == -2   ) num = (7.72635*(TMath::Erf((x-3.57536)/44.70329)*TMath::Exp(-0.43994*x)))+0.87484;
      else if (idx == 1   ) num = (9.25587*(TMath::Erf((x-3.59868)/38.85721)*TMath::Exp(-0.55576*x)))+0.87965;
      else if (idx == 2   ) num = (6.80501*(TMath::Erf((x-3.68895)/49.60116)*TMath::Exp(-0.36212*x)))+0.87724;
      else if (idx == 3   ) num = (5.84808*(TMath::Erf((x-4.11020)/49.95589)*TMath::Exp(-0.37114*x)))+0.88543;
      else if (idx == 4   ) num = (10.74458*(TMath::Erf((x-5.55299)/36.69071)*TMath::Exp(-1.01858*x)))+0.89286;
      else if (idx == 5   ) num = (9.07108*(TMath::Erf((x-3.64432)/43.77138)*TMath::Exp(-0.52678*x)))+0.88227;
      else if (idx == 6   ) num = (8.13620*(TMath::Erf((x-9.39293)/49.80365)*TMath::Exp(-1.05652*x)))+0.89647;
      else if (idx == 7   ) num = (8.71262*(TMath::Erf((x-5.62696)/46.92015)*TMath::Exp(-0.76335*x)))+0.90969;
      else if (idx == 8   ) num = (7.52530*(TMath::Erf((x-3.61559)/47.60661)*TMath::Exp(-0.40331*x)))+0.87626;
      else if (idx == 9   ) num = (7.76505*(TMath::Erf((x-2.83283)/49.30411)*TMath::Exp(-0.32784*x)))+0.83260;
      else if (idx == 10  ) num = (11.84957*(TMath::Erf((x-5.11024)/27.95719)*TMath::Exp(-1.09312*x)))+0.89766;
      else if (idx == 11  ) num = (6.45294*(TMath::Erf((x-9.85109)/42.32747)*TMath::Exp(-1.03318*x)))+0.90516;
      else if (idx == 12  ) num = (7.83246*(TMath::Erf((x-2.69152)/49.41544)*TMath::Exp(-0.32479*x)))+0.82733;
      else if (idx == 13  ) num = (7.48641*(TMath::Erf((x-3.70230)/48.26296)*TMath::Exp(-0.42438*x)))+0.88030;
      else if (idx == 14  ) num = (8.80349*(TMath::Erf((x-3.46850)/42.07521)*TMath::Exp(-0.49472*x)))+0.86913;
      else if (idx == 15  ) num = (7.08586*(TMath::Erf((x-2.28330)/49.82176)*TMath::Exp(-0.25417*x)))+0.78468;
      else if (idx == 16  ) num = (8.52116*(TMath::Erf((x-3.91246)/45.92853)*TMath::Exp(-0.51337*x)))+0.89277;
      else if (idx == 17  ) num = (2.98097*(TMath::Erf((x-8.02092)/17.41609)*TMath::Exp(-0.90277*x)))+0.91085;
      else if (idx == 18  ) num = (7.71891*(TMath::Erf((x-2.46999)/49.67795)*TMath::Exp(-0.28907*x)))+0.80394;
      else if (idx == 19  ) num = (5.73978*(TMath::Erf((x-9.76581)/49.16845)*TMath::Exp(-0.88581*x)))+0.90994;
      else if (idx == 20  ) num = (7.08376*(TMath::Erf((x-2.84823)/49.18980)*TMath::Exp(-0.29741*x)))+0.82818;
      else if (idx == 21  ) num = (8.39589*(TMath::Erf((x-2.65368)/49.10530)*TMath::Exp(-0.33166*x)))+0.81829;
      else if (idx == 22  ) num = (6.43200*(TMath::Erf((x-1.08433)/46.58591)*TMath::Exp(-0.18091*x)))+0.66619;
      else if (idx == 23  ) num = (8.04789*(TMath::Erf((x-3.30498)/40.20464)*TMath::Exp(-0.45743*x)))+0.86268;
      else if (idx == 24  ) num = (10.83928*(TMath::Erf((x-3.89583)/31.49023)*TMath::Exp(-0.76826*x)))+0.89135;
      else if (idx == 25  ) num = (11.38822*(TMath::Erf((x-6.49520)/29.81270)*TMath::Exp(-1.21799*x)))+0.89255;
      else if (idx == 26  ) num = (9.82539*(TMath::Erf((x-2.51404)/44.67914)*TMath::Exp(-0.35306*x)))+0.80966;
      else if (idx == 27  ) num = (8.33199*(TMath::Erf((x-3.55714)/44.24989)*TMath::Exp(-0.44986*x)))+0.87839;
      else if (idx == 28  ) num = (10.71677*(TMath::Erf((x-4.02059)/49.98520)*TMath::Exp(-0.60409*x)))+0.88511;
      else if (idx == 29  ) num = (7.55121*(TMath::Erf((x-3.35119)/43.65577)*TMath::Exp(-0.41670*x)))+0.86592;
      else if (idx == 30  ) num = (7.79744*(TMath::Erf((x-3.44505)/47.11438)*TMath::Exp(-0.41895*x)))+0.86813;
      else if (idx == 31  ) num = (7.59674*(TMath::Erf((x-2.81042)/49.64493)*TMath::Exp(-0.32454*x)))+0.83665;
      else if (idx == 32  ) num = (8.38556*(TMath::Erf((x-2.40841)/49.01099)*TMath::Exp(-0.29598*x)))+0.79666;
      else if (idx == 33  ) num = (6.42246*(TMath::Erf((x-2.54238)/49.99773)*TMath::Exp(-0.25789*x)))+0.80815;
      else if (idx == 34  ) num = (7.30560*(TMath::Erf((x-3.40224)/48.28033)*TMath::Exp(-0.39199*x)))+0.86364;
      else if (idx == 35  ) num = (5.92499*(TMath::Erf((x-2.28186)/48.97706)*TMath::Exp(-0.22186*x)))+0.78612;
      else if (idx == 36  ) num = (7.60336*(TMath::Erf((x-3.05125)/48.83816)*TMath::Exp(-0.35518*x)))+0.84431;
      else if (idx == 37  ) num = (6.37812*(TMath::Erf((x-2.62853)/49.56626)*TMath::Exp(-0.27342*x)))+0.81563;
      else if (idx == 38  ) num = (10.13127*(TMath::Erf((x-3.47920)/31.62345)*TMath::Exp(-0.63998*x)))+0.87927;
      else if (idx == 39  ) num = (6.63372*(TMath::Erf((x-2.82061)/49.35474)*TMath::Exp(-0.29171*x)))+0.82690;
      else if (idx == 40  ) num = (10.41482*(TMath::Erf((x-6.64691)/44.97512)*TMath::Exp(-0.97708*x)))+0.90484;
      else if (idx == 41  ) num = (5.73215*(TMath::Erf((x-3.25493)/49.93071)*TMath::Exp(-0.28733*x)))+0.85254;
      else if (idx == 42  ) num = (7.18082*(TMath::Erf((x-9.54614)/40.09172)*TMath::Exp(-1.06718*x)))+0.90378;
      else if (idx == 43  ) num = (10.05383*(TMath::Erf((x-6.22899)/49.36351)*TMath::Exp(-0.88871*x)))+0.90191;
      else if (idx == 44  ) num = (8.77560*(TMath::Erf((x-6.25682)/41.86075)*TMath::Exp(-0.90014*x)))+0.90671;
      else if (idx == 45  ) num = (7.98635*(TMath::Erf((x-3.19319)/47.73068)*TMath::Exp(-0.39311*x)))+0.86043;
      else if (idx == 46  ) num = (7.93241*(TMath::Erf((x-2.21728)/49.88983)*TMath::Exp(-0.25507*x)))+0.77778;
      else if (idx == 47  ) num = (11.40170*(TMath::Erf((x-4.99617)/49.97132)*TMath::Exp(-0.78845*x)))+0.89766;
      else if (idx == 48  ) num = (8.23055*(TMath::Erf((x-8.12849)/42.97252)*TMath::Exp(-1.00643*x)))+0.91211;
      else if (idx == 49  ) num = (5.82129*(TMath::Erf((x-2.99056)/49.99999)*TMath::Exp(-0.29472*x)))+0.84139;
      else if (idx == 50  ) num = (6.25466*(TMath::Erf((x-1.92794)/49.92131)*TMath::Exp(-0.22409*x)))+0.76068;
      else if (idx == 51  ) num = (7.74147*(TMath::Erf((x-3.72566)/47.65885)*TMath::Exp(-0.44091*x)))+0.87922;
      else if (idx == 52  ) num = (6.69727*(TMath::Erf((x-2.69418)/49.30915)*TMath::Exp(-0.28828*x)))+0.82299;
      else if (idx == 53  ) num = (14.16680*(TMath::Erf((x-6.24986)/42.99335)*TMath::Exp(-1.12085*x)))+0.89979;
      else if (idx == 54  ) num = (11.19309*(TMath::Erf((x-3.41956)/32.18238)*TMath::Exp(-0.66688*x)))+0.87951;
      else if (idx == 55  ) num = (7.67847*(TMath::Erf((x-3.72362)/47.28336)*TMath::Exp(-0.46257*x)))+0.87625;
      else if (idx == 56  ) num = (11.61903*(TMath::Erf((x-5.48530)/49.99573)*TMath::Exp(-0.89060*x)))+0.89293;
      else if (idx == 57  ) num = (2.49360*(TMath::Erf((x-7.87166)/11.52390)*TMath::Exp(-1.03157*x)))+0.90104;
      else if (idx == 58  ) num = (10.64428*(TMath::Erf((x-3.37880)/41.61782)*TMath::Exp(-0.54836*x)))+0.87476;
      else if (idx == 59  ) num = (11.60871*(TMath::Erf((x-5.05682)/31.06368)*TMath::Exp(-1.05078*x)))+0.89075;
      else if (idx == 60  ) num = (6.77779*(TMath::Erf((x-2.25963)/49.79902)*TMath::Exp(-0.24944*x)))+0.78746;
      else if (idx == 61  ) num = (11.44075*(TMath::Erf((x-5.04201)/28.43437)*TMath::Exp(-1.04042*x)))+0.89824;
      else if (idx == 62  ) num = (7.19113*(TMath::Erf((x-2.37630)/49.92957)*TMath::Exp(-0.25940*x)))+0.79368;
      else if (idx == 63  ) num = (6.26462*(TMath::Erf((x-3.83702)/49.89960)*TMath::Exp(-0.36134*x)))+0.87587;
      else if (idx == 64  ) num = (8.89611*(TMath::Erf((x-3.40351)/41.55932)*TMath::Exp(-0.48836*x)))+0.87152;
      else if (idx == 65  ) num = (7.95881*(TMath::Erf((x-4.25636)/49.99784)*TMath::Exp(-0.49983*x)))+0.89574;
      else if (idx == 66  ) num = (2.27222*(TMath::Erf((x-9.90265)/3.01506)*TMath::Exp(-1.30761*x)))+0.89758;
      else if (idx == 67  ) num = (2.26119*(TMath::Erf((x-7.68548)/11.84603)*TMath::Exp(-0.98413*x)))+0.89676;
      else if (idx == 68  ) num = (5.38355*(TMath::Erf((x-9.95454)/48.55903)*TMath::Exp(-0.88329*x)))+0.91071;
      else if (idx == 69  ) num = (11.36174*(TMath::Erf((x-3.73256)/29.69095)*TMath::Exp(-0.77455*x)))+0.88518;
      else if (idx == 70  ) num = (7.37589*(TMath::Erf((x-8.85926)/39.73260)*TMath::Exp(-1.03408*x)))+0.90968;
      else if (idx == 71  ) num = (7.52599*(TMath::Erf((x-2.94849)/49.07746)*TMath::Exp(-0.32633*x)))+0.83896;
      else if (idx == 72  ) num = (8.40524*(TMath::Erf((x-3.81728)/48.49688)*TMath::Exp(-0.46778*x)))+0.88352;
      else if (idx == 73  ) num = (8.05468*(TMath::Erf((x-3.67971)/46.56320)*TMath::Exp(-0.43434*x)))+0.87876;
      else if (idx == 74  ) num = (5.88949*(TMath::Erf((x-1.06492)/48.96789)*TMath::Exp(-0.16490*x)))+0.67804;
      else if (idx == 75  ) num = (8.06103*(TMath::Erf((x-3.34831)/47.09397)*TMath::Exp(-0.39988*x)))+0.86582;
      else if (idx == 76  ) num = (1.63661*(TMath::Erf((x-8.33317)/12.81321)*TMath::Exp(-0.79265*x)))+0.91173;
      else if (idx == 77  ) num = (10.64391*(TMath::Erf((x-5.09173)/30.23230)*TMath::Exp(-0.96506*x)))+0.90297;
      else if (idx == 78  ) num = (4.88246*(TMath::Erf((x-7.04578)/30.78166)*TMath::Exp(-0.85017*x)))+0.90419;
      else if (idx == 79  ) num = (6.24164*(TMath::Erf((x-1.60936)/49.37811)*TMath::Exp(-0.19022*x)))+0.72593;
      else if (idx == 80  ) num = (17.16900*(TMath::Erf((x-6.16526)/46.04847)*TMath::Exp(-1.17308*x)))+0.89828;
      else if (idx == 81  ) num = (5.82765*(TMath::Erf((x-6.03678)/36.33856)*TMath::Exp(-0.76174*x)))+0.90714;
      else if (idx == 82  ) num = (12.81356*(TMath::Erf((x-6.51440)/48.94249)*TMath::Exp(-1.04334*x)))+0.89964;
      else if (idx == 83  ) num = (7.50207*(TMath::Erf((x-3.55439)/47.16326)*TMath::Exp(-0.40322*x)))+0.87493;
      else if (idx == 84  ) num = (11.35271*(TMath::Erf((x-3.18483)/30.60789)*TMath::Exp(-0.63500*x)))+0.87068;
      else if (idx == 85  ) num = (11.35526*(TMath::Erf((x-4.57817)/28.50857)*TMath::Exp(-0.94508*x)))+0.89366;
      else if (idx == 86  ) num = (13.61197*(TMath::Erf((x-6.07873)/34.63359)*TMath::Exp(-1.21320*x)))+0.89492;
      else if (idx == 87  ) num = (14.95121*(TMath::Erf((x-6.73457)/28.05634)*TMath::Exp(-1.39485*x)))+0.89493;
      else if (idx == 88  ) num = (7.61764*(TMath::Erf((x-3.50315)/44.58843)*TMath::Exp(-0.42164*x)))+0.87159;
      else if (idx == 89  ) num = (14.30992*(TMath::Erf((x-5.91908)/42.25371)*TMath::Exp(-1.12505*x)))+0.89743;
      else if (idx == 90  ) num = (7.03157*(TMath::Erf((x-3.65617)/48.73098)*TMath::Exp(-0.39661*x)))+0.87628;
      else if (idx == 91  ) num = (10.73140*(TMath::Erf((x-5.52522)/48.68644)*TMath::Exp(-0.82474*x)))+0.90802;
      else if (idx == 92  ) num = (6.90978*(TMath::Erf((x-2.32296)/49.33344)*TMath::Exp(-0.24763*x)))+0.79088;
      else if (idx == 93  ) num = (11.87783*(TMath::Erf((x-5.24665)/42.71153)*TMath::Exp(-0.89918*x)))+0.89926;
      else if (idx == 94  ) num = (6.29350*(TMath::Erf((x-2.09687)/49.45320)*TMath::Exp(-0.22394*x)))+0.77103;
      else if (idx == 95  ) num = (10.23545*(TMath::Erf((x-5.61200)/36.04968)*TMath::Exp(-0.96617*x)))+0.90007;
      else if (idx == 96  ) num = (8.80014*(TMath::Erf((x-3.70344)/46.81679)*TMath::Exp(-0.50530*x)))+0.87701;
      else if (idx == 97  ) num = (9.14823*(TMath::Erf((x-3.98638)/40.56376)*TMath::Exp(-0.60071*x)))+0.89146;
      else if (idx == 98  ) num = (1.28278*(TMath::Erf((x-7.98443)/10.24242)*TMath::Exp(-0.73453*x)))+0.91878;
      else if (idx == 99  ) num = (10.50762*(TMath::Erf((x-5.62759)/36.59487)*TMath::Exp(-0.95351*x)))+0.90256;
      else if (idx == 100 ) num = (6.36097*(TMath::Erf((x-1.80626)/49.67144)*TMath::Exp(-0.21334*x)))+0.74592;
   }

   // return
   return num/den;
}

///////////////////////////////////////////////////
//             M U I D    P b P b                //
///////////////////////////////////////////////////

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// !!  ONLY FOR SYSTEMATICS! DO NOT APPLY FOR THE NOMINAL CORRECTION!!! !!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

double tnp_weight_muid_pbpb(double x, double eta) {
   // denominator (from MC)
   double den=1;
   if (fabs(eta)<1.2) den = 0.99*TMath::Erf((x+1.16)/3.49);
   else if (fabs(eta)<1.8) den = 0.99*TMath::Erf((x+3.63)/4.27);
   else if (fabs(eta)<2.1) den = 1.00*TMath::Erf((x+4.64)/6.08);
   else den = 0.99*TMath::Erf((x+3.17)/4.98);

   // numerator (from data)
   double num=1;
   if (fabs(eta)<1.2) num = 0.98*TMath::Erf((x+0.39)/2.94);
   else if (fabs(eta)<1.8) num = 0.99*TMath::Erf((x+4.38)/5.03);
   else if (fabs(eta)<2.1) num = 1.00*TMath::Erf((x+5.26)/6.31);
   else num = 1.00*TMath::Erf((x+3.34)/5.38);

   return num/den;
}

///////////////////////////////////////////////////
//                 M U I D    P P                //
///////////////////////////////////////////////////

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// !!  ONLY FOR SYSTEMATICS! DO NOT APPLY FOR THE NOMINAL CORRECTION!!! !!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

double tnp_weight_muid_pp(double x, double eta) {
   // denominator (from MC)
   double den=1;
   if (fabs(eta)<1.2) den = 0.21136*TMath::Erf((x-1.68695)/2.17395)+0.78062;
   else if (fabs(eta)<1.8) den = 0.01167*TMath::Erf((x+7.79151)/23.51093)+0.98813;
   else if (fabs(eta)<2.1) den = 0.00089*TMath::Erf((x+1.63869)/15.87452)+0.99843;
   else den = 0.00267*TMath::Erf((x+1.24027)/20.11707)+0.99573;

   // numerator (from data)
   double num=1;
   if (fabs(eta)<1.2) num = 0.66604*TMath::Erf((x-0.15272)/2.64610)+0.32362;
   else if (fabs(eta)<1.8) num = 0.00057*TMath::Erf((x-5.90893)/1.15286)+0.99199;
   else if (fabs(eta)<2.1) num = 0.00001*TMath::Erf((x-9.73000)/49.99969)+0.99755;
   else num = 0.00260*TMath::Erf((x-2.77358)/0.15943)+0.99398;

   return num/den;
}

///////////////////////////////////////////////////
//                   S T A    P P                //
///////////////////////////////////////////////////

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// !!  ONLY FOR SYSTEMATICS! DO NOT APPLY FOR THE NOMINAL CORRECTION!!! !!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

double tnp_weight_sta_pp(double x, double eta) {
   // denominator (from MC)
   double den=1;
   if (fabs(eta)<1.2) den = 1.16795*TMath::Erf((x-2.70934)/1.14920)-0.18306;
   else if (fabs(eta)<1.8) den = 0.06821*TMath::Erf((x-2.40814)/1.06603)+0.90716;
   else if (fabs(eta)<2.1) den = 0.02081*TMath::Erf((x-2.07575)/0.86473)+0.96405;
   else den = 0.03392*TMath::Erf((x-2.61705)/0.01196)+0.94859;

   // numerator (from data)
   double num=1;
   if (fabs(eta)<1.2) num = 1.16758*TMath::Erf((x-2.58133)/1.23203)-0.17965;
   else if (fabs(eta)<1.8) num = 0.23761*TMath::Erf((x+9.72168)/10.29208)+0.75652;
   else if (fabs(eta)<2.1) num = 0.02287*TMath::Erf((x-1.50087)/0.12003)+0.96844;
   else num = 0.02083*TMath::Erf((x-4.83631)/0.17496)+0.97713;

   return num/den;
}

#endif //#ifndef tnp_weight_h
