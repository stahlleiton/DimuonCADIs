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
         else if (x<12.5) return 0.981742;
         else if (x<30) return 0.98747;
      } else if (fabs(eta) < 1.8) {
         // 1.2 < |eta| < 1.8
         if (x<3) return 1.141;
         else if (x<3.5) return 1.08315;
         else if (x<4) return 1.03487;
         else if (x<4.5) return 1.01835;
         else if (x<5) return 0.997828;
         else if (x<6) return 0.987733;
         else if (x<7.5) return 1.00724;
         else if (x<12.5) return 0.998316;
         else if (x<30) return 1.02257;
      } else if (fabs(eta) < 2.1) {
         // 1.8 < |eta| < 2.1
         if (x<2) return 1.45635;
         else if (x<2.5) return 1.23666;
         else if (x<3) return 1.09206;
         else if (x<3.5) return 1.06978;
         else if (x<4) return 1.02001;
         else if (x<4.5) return 1.03328;
         else if (x<5.5) return 1.02344;
         else if (x<7) return 1.02623;
         else if (x<12.5) return 0.993525;
         else if (x<30) return 1.00672;
      } else {
         // 2.1 < |eta| < 2.4
         if (x<2.2) return 1.27031;
         else if (x<2.7) return 1.15418;
         else if (x<3.2) return 1.06914;
         else if (x<3.7) return 1.04694;
         else if (x<4.7) return 1.06675;
         else if (x<8) return 1.04164;
         else if (x<30) return 1.00671;
      }
   }

   // denominator (from MC)
   double den=1;
   if (fabs(eta)<1.2) den = 82.77278*(TMath::Erf((x-5.70040)/5.45156)*TMath::Exp(-1.45014*x))+0.97951;
   else if (fabs(eta)<1.8) den = 56.13109*(TMath::Erf((x-5.96515)/5.28006)*TMath::Exp(-1.49061*x))+0.95064;
   else if (fabs(eta)<2.1) den = 7.52181*(TMath::Erf((x-4.70591)/18.65633)*TMath::Exp(-0.55159*x))+0.88412;
   else den = 5.82563*(TMath::Erf((x-3.75613)/12.81456)*TMath::Exp(-0.59941*x))+0.85287;

   // numerator (from data)
   double num=1;

   if (fabs(eta)<1.2)
   {
      if (idx==0) num = 16.68771*(TMath::Erf((x-5.10333)/6.16292)*TMath::Exp(-1.03492*x))+0.96698;
      else if (idx == -1   ) num =   16.62038*(TMath::Erf((x-5.08923)/6.19250)*TMath::Exp(-1.03604*x))+0.96803;
      else if (idx == -2   ) num =   16.76962*(TMath::Erf((x-5.11407)/6.13436)*TMath::Exp(-1.03389*x))+0.96620;
      else if (idx == 1   ) num = 4.33415*(TMath::Erf((x-5.26356)/14.55606)*TMath::Exp(-0.56701*x))+0.96652;
      else if (idx == 2   ) num = 14.25393*(TMath::Erf((x-6.00430)/7.52566)*TMath::Exp(-1.04308*x))+0.97217;
      else if (idx == 3   ) num = 7.44938*(TMath::Erf((x-4.91056)/9.29746)*TMath::Exp(-0.71714*x))+0.95899;
      else if (idx == 4   ) num = 14.66661*(TMath::Erf((x-5.56925)/6.99217)*TMath::Exp(-1.05281*x))+0.96782;
      else if (idx == 5   ) num = 58.11221*(TMath::Erf((x-5.84546)/6.87686)*TMath::Exp(-1.40565*x))+0.97199;
      else if (idx == 6   ) num = 49.97410*(TMath::Erf((x-5.59848)/5.26489)*TMath::Exp(-1.44993*x))+0.96559;
      else if (idx == 7   ) num = 72.69628*(TMath::Erf((x-5.26590)/4.62771)*TMath::Exp(-1.49554*x))+0.96634;
      else if (idx == 8   ) num = 17.90023*(TMath::Erf((x-4.85188)/5.80652)*TMath::Exp(-1.02643*x))+0.96775;
      else if (idx == 9   ) num = 5.37593*(TMath::Erf((x-4.94963)/12.56860)*TMath::Exp(-0.57455*x))+0.96035;
      else if (idx == 10  ) num = 16.10144*(TMath::Erf((x-5.23272)/6.43776)*TMath::Exp(-1.04501*x))+0.96842;
      else if (idx == 11  ) num = 66.88425*(TMath::Erf((x-5.62173)/5.82864)*TMath::Exp(-1.49347*x))+0.97179;
      else if (idx == 12  ) num = 35.66310*(TMath::Erf((x-5.49392)/5.14118)*TMath::Exp(-1.35532*x))+0.96483;
      else if (idx == 13  ) num = 17.53728*(TMath::Erf((x-5.00008)/5.87450)*TMath::Exp(-1.02176*x))+0.96495;
      else if (idx == 14  ) num = 16.55790*(TMath::Erf((x-5.06501)/6.21822)*TMath::Exp(-1.03694*x))+0.96927;
      else if (idx == 15  ) num = 8.50259*(TMath::Erf((x-4.73459)/8.64029)*TMath::Exp(-0.73940*x))+0.95612;
      else if (idx == 16  ) num = 8.31359*(TMath::Erf((x-5.00417)/10.64011)*TMath::Exp(-0.69998*x))+0.96422;
      else if (idx == 17  ) num = 7.66481*(TMath::Erf((x-4.96743)/10.10355)*TMath::Exp(-0.68305*x))+0.96288;
      else if (idx == 18  ) num = 4.30501*(TMath::Erf((x-4.93872)/12.44221)*TMath::Exp(-0.59175*x))+0.96348;
      else if (idx == 19  ) num = 12.75195*(TMath::Erf((x-6.04676)/7.72014)*TMath::Exp(-1.04143*x))+0.97156;
      else if (idx == 20  ) num = 16.38049*(TMath::Erf((x-5.07976)/6.27598)*TMath::Exp(-1.03956*x))+0.96803;
      else if (idx == 21  ) num = 13.43774*(TMath::Erf((x-5.63580)/7.33245)*TMath::Exp(-1.05349*x))+0.96852;
      else if (idx == 22  ) num = 80.19891*(TMath::Erf((x-5.77807)/6.31368)*TMath::Exp(-1.49994*x))+0.97032;
      else if (idx == 23  ) num = 11.02516*(TMath::Erf((x-4.88573)/7.23331)*TMath::Exp(-0.88348*x))+0.96370;
      else if (idx == 24  ) num = 14.77169*(TMath::Erf((x-5.37682)/6.84894)*TMath::Exp(-1.04235*x))+0.96981;
      else if (idx == 25  ) num = 8.80191*(TMath::Erf((x-4.85075)/8.45331)*TMath::Exp(-0.74800*x))+0.96487;
      else if (idx == 26  ) num = 32.10100*(TMath::Erf((x-5.40455)/5.00538)*TMath::Exp(-1.30666*x))+0.97097;
      else if (idx == 27  ) num = 12.50585*(TMath::Erf((x-6.31429)/8.11001)*TMath::Exp(-1.04440*x))+0.97241;
      else if (idx == 28  ) num = 58.60120*(TMath::Erf((x-5.06643)/4.14456)*TMath::Exp(-1.44940*x))+0.96782;
      else if (idx == 29  ) num = 15.04658*(TMath::Erf((x-5.32994)/6.79373)*TMath::Exp(-1.05135*x))+0.96597;
      else if (idx == 30  ) num = 26.98896*(TMath::Erf((x-5.77487)/5.12482)*TMath::Exp(-1.25946*x))+0.97374;
      else if (idx == 31  ) num = 10.36944*(TMath::Erf((x-4.94578)/7.62008)*TMath::Exp(-0.84685*x))+0.96489;
      else if (idx == 32  ) num = 6.01082*(TMath::Erf((x-4.84860)/10.82831)*TMath::Exp(-0.63103*x))+0.95611;
      else if (idx == 33  ) num = 18.83747*(TMath::Erf((x-4.75404)/5.30187)*TMath::Exp(-1.00761*x))+0.96383;
      else if (idx == 34  ) num = 3.60992*(TMath::Erf((x-4.74278)/16.18483)*TMath::Exp(-0.41022*x))+0.95123;
      else if (idx == 35  ) num = 43.97659*(TMath::Erf((x-4.80017)/3.96670)*TMath::Exp(-1.34284*x))+0.96578;
      else if (idx == 36  ) num = 60.34622*(TMath::Erf((x-6.15896)/6.56974)*TMath::Exp(-1.49210*x))+0.97094;
      else if (idx == 37  ) num = 10.83250*(TMath::Erf((x-4.94772)/7.23631)*TMath::Exp(-0.83746*x))+0.96531;
      else if (idx == 38  ) num = 8.84540*(TMath::Erf((x-4.88424)/8.37047)*TMath::Exp(-0.77758*x))+0.96402;
      else if (idx == 39  ) num = 15.79421*(TMath::Erf((x-5.45475)/6.64278)*TMath::Exp(-1.04462*x))+0.96986;
      else if (idx == 40  ) num = 9.02456*(TMath::Erf((x-4.90215)/8.06279)*TMath::Exp(-0.76717*x))+0.96323;
      else if (idx == 41  ) num = 13.60512*(TMath::Erf((x-5.77044)/7.46999)*TMath::Exp(-1.02899*x))+0.96893;
      else if (idx == 42  ) num = 16.78215*(TMath::Erf((x-5.11500)/6.12614)*TMath::Exp(-1.03289*x))+0.96655;
      else if (idx == 43  ) num = 57.79060*(TMath::Erf((x-4.89321)/3.49401)*TMath::Exp(-1.42489*x))+0.96590;
      else if (idx == 44  ) num = 17.38028*(TMath::Erf((x-4.92880)/5.98339)*TMath::Exp(-1.03235*x))+0.96540;
      else if (idx == 45  ) num = 18.67582*(TMath::Erf((x-4.73800)/5.44188)*TMath::Exp(-1.02320*x))+0.96366;
      else if (idx == 46  ) num = 15.29172*(TMath::Erf((x-5.48809)/6.75619)*TMath::Exp(-1.05475*x))+0.97056;
      else if (idx == 47  ) num = 12.24115*(TMath::Erf((x-6.02499)/8.05120)*TMath::Exp(-1.03912*x))+0.96925;
      else if (idx == 48  ) num = 17.32640*(TMath::Erf((x-4.86427)/6.00499)*TMath::Exp(-1.03248*x))+0.96647;
      else if (idx == 49  ) num = 14.48147*(TMath::Erf((x-5.87034)/7.29336)*TMath::Exp(-1.06412*x))+0.96904;
      else if (idx == 50  ) num = 7.83867*(TMath::Erf((x-4.92789)/8.99324)*TMath::Exp(-0.73829*x))+0.96149;
      else if (idx == 51  ) num = 16.01584*(TMath::Erf((x-5.29582)/6.49484)*TMath::Exp(-1.04352*x))+0.96842;
      else if (idx == 52  ) num = 52.04589*(TMath::Erf((x-5.80042)/5.72567)*TMath::Exp(-1.45706*x))+0.97335;
      else if (idx == 53  ) num = 31.52026*(TMath::Erf((x-5.04529)/2.45404)*TMath::Exp(-1.37107*x))+0.96616;
      else if (idx == 54  ) num = 12.89421*(TMath::Erf((x-4.79644)/6.70643)*TMath::Exp(-0.87537*x))+0.96358;
      else if (idx == 55  ) num = 14.15169*(TMath::Erf((x-5.82087)/7.23320)*TMath::Exp(-1.06262*x))+0.97096;
      else if (idx == 56  ) num = 10.36400*(TMath::Erf((x-4.88003)/7.69321)*TMath::Exp(-0.84250*x))+0.96294;
      else if (idx == 57  ) num = 7.98547*(TMath::Erf((x-4.79592)/10.39923)*TMath::Exp(-0.66772*x))+0.95845;
      else if (idx == 58  ) num = 15.80513*(TMath::Erf((x-5.00363)/6.28756)*TMath::Exp(-1.00185*x))+0.96392;
      else if (idx == 59  ) num = 3.05593*(TMath::Erf((x-4.97697)/7.05841)*TMath::Exp(-0.58947*x))+0.96069;
      else if (idx == 60  ) num = 18.50548*(TMath::Erf((x-4.78431)/5.53673)*TMath::Exp(-1.01574*x))+0.96454;
      else if (idx == 61  ) num = 18.10743*(TMath::Erf((x-4.94868)/5.70601)*TMath::Exp(-1.02668*x))+0.96660;
      else if (idx == 62  ) num = 18.44459*(TMath::Erf((x-4.81729)/5.52591)*TMath::Exp(-1.02706*x))+0.96426;
      else if (idx == 63  ) num = 18.31059*(TMath::Erf((x-4.88877)/5.51524)*TMath::Exp(-1.01952*x))+0.96508;
      else if (idx == 64  ) num = 18.31284*(TMath::Erf((x-4.76049)/5.65696)*TMath::Exp(-1.02818*x))+0.96273;
      else if (idx == 65  ) num = 39.10193*(TMath::Erf((x-5.04602)/3.97388)*TMath::Exp(-1.31858*x))+0.97086;
      else if (idx == 66  ) num = 29.71907*(TMath::Erf((x-4.91618)/4.43134)*TMath::Exp(-1.19181*x))+0.96584;
      else if (idx == 67  ) num = 10.61926*(TMath::Erf((x-4.92881)/7.28987)*TMath::Exp(-0.83638*x))+0.96405;
      else if (idx == 68  ) num = 17.51092*(TMath::Erf((x-4.96729)/5.81693)*TMath::Exp(-1.02129*x))+0.96711;
      else if (idx == 69  ) num = 34.68630*(TMath::Erf((x-4.86870)/2.14235)*TMath::Exp(-1.39497*x))+0.96531;
      else if (idx == 70  ) num = 7.15824*(TMath::Erf((x-5.05279)/9.06137)*TMath::Exp(-0.75537*x))+0.96166;
      else if (idx == 71  ) num = 6.50871*(TMath::Erf((x-5.16543)/9.96200)*TMath::Exp(-0.75190*x))+0.96587;
      else if (idx == 72  ) num = 55.09927*(TMath::Erf((x-5.24102)/4.37952)*TMath::Exp(-1.44833*x))+0.96892;
      else if (idx == 73  ) num = 17.08243*(TMath::Erf((x-5.09676)/6.01110)*TMath::Exp(-1.02906*x))+0.96881;
      else if (idx == 74  ) num = 9.28521*(TMath::Erf((x-5.06988)/7.86521)*TMath::Exp(-0.81648*x))+0.96414;
      else if (idx == 75  ) num = 7.71340*(TMath::Erf((x-4.81000)/9.27440)*TMath::Exp(-0.70259*x))+0.96073;
      else if (idx == 76  ) num = 15.15197*(TMath::Erf((x-5.42248)/6.76930)*TMath::Exp(-1.05676*x))+0.97018;
      else if (idx == 77  ) num = 19.73682*(TMath::Erf((x-4.57780)/5.05772)*TMath::Exp(-1.00508*x))+0.95954;
      else if (idx == 78  ) num = 16.61917*(TMath::Erf((x-5.11183)/6.19458)*TMath::Exp(-1.03670*x))+0.96498;
      else if (idx == 79  ) num = 8.34898*(TMath::Erf((x-4.89329)/8.26690)*TMath::Exp(-0.77096*x))+0.96184;
      else if (idx == 80  ) num = 11.05449*(TMath::Erf((x-4.80527)/7.32961)*TMath::Exp(-0.84207*x))+0.95939;
      else if (idx == 81  ) num = 17.10789*(TMath::Erf((x-4.97483)/6.24877)*TMath::Exp(-1.02778*x))+0.96534;
      else if (idx == 82  ) num = 8.63547*(TMath::Erf((x-4.74854)/8.86936)*TMath::Exp(-0.74849*x))+0.96046;
      else if (idx == 83  ) num = 13.38736*(TMath::Erf((x-5.78896)/7.47432)*TMath::Exp(-1.05078*x))+0.96945;
      else if (idx == 84  ) num = 17.48550*(TMath::Erf((x-5.25117)/6.23247)*TMath::Exp(-1.08529*x))+0.96767;
      else if (idx == 85  ) num = 65.57951*(TMath::Erf((x-4.65085)/3.42984)*TMath::Exp(-1.45228*x))+0.96477;
      else if (idx == 86  ) num = 11.04192*(TMath::Erf((x-4.66059)/7.38692)*TMath::Exp(-0.80899*x))+0.95746;
      else if (idx == 87  ) num = 81.86716*(TMath::Erf((x-5.28048)/5.23625)*TMath::Exp(-1.49989*x))+0.96702;
      else if (idx == 88  ) num = 49.26329*(TMath::Erf((x-6.44016)/6.26431)*TMath::Exp(-1.46400*x))+0.97077;
      else if (idx == 89  ) num = 11.70323*(TMath::Erf((x-6.10059)/7.95421)*TMath::Exp(-1.06382*x))+0.97089;
      else if (idx == 90  ) num = 6.81146*(TMath::Erf((x-4.77518)/10.31342)*TMath::Exp(-0.64634*x))+0.95895;
      else if (idx == 91  ) num = 71.75971*(TMath::Erf((x-4.86717)/3.34777)*TMath::Exp(-1.47374*x))+0.96820;
      else if (idx == 92  ) num = 37.85643*(TMath::Erf((x-5.63839)/4.88194)*TMath::Exp(-1.37541*x))+0.97229;
      else if (idx == 93  ) num = 19.07994*(TMath::Erf((x-4.70318)/5.31913)*TMath::Exp(-1.00881*x))+0.96262;
      else if (idx == 94  ) num = 14.01180*(TMath::Erf((x-6.31032)/7.74752)*TMath::Exp(-1.07596*x))+0.97118;
      else if (idx == 95  ) num = 81.01483*(TMath::Erf((x-5.02260)/9.26967)*TMath::Exp(-1.31022*x))+0.96574;
      else if (idx == 96  ) num = 54.63292*(TMath::Erf((x-5.20097)/4.31567)*TMath::Exp(-1.44115*x))+0.96769;
      else if (idx == 97  ) num = 7.48957*(TMath::Erf((x-5.26338)/10.90215)*TMath::Exp(-0.69283*x))+0.96616;
      else if (idx == 98  ) num = 18.19611*(TMath::Erf((x-4.79203)/5.74094)*TMath::Exp(-1.02351*x))+0.95979;
      else if (idx == 99  ) num = 25.86558*(TMath::Erf((x-5.64586)/5.36576)*TMath::Exp(-1.26484*x))+0.97272;
      else if (idx == 100 ) num = 17.15400*(TMath::Erf((x-5.09337)/5.98519)*TMath::Exp(-1.02885*x))+0.96776;
   }
   else if (fabs(eta)<1.8)
   {
      if (idx==0) num = 94.25172*(TMath::Erf((x-4.82340)/6.86893)*TMath::Exp(-1.49990*x))+0.95010;
      else if (idx == -1   ) num =   92.60864*(TMath::Erf((x-4.82417)/6.86299)*TMath::Exp(-1.50000*x))+0.95115;
      else if (idx == -2   ) num =   94.99952*(TMath::Erf((x-4.82486)/6.80899)*TMath::Exp(-1.50000*x))+0.94909;
      else if (idx == 1   ) num = 93.61657*(TMath::Erf((x-4.75950)/6.24736)*TMath::Exp(-1.49999*x))+0.95125;
      else if (idx == 2   ) num = 91.71335*(TMath::Erf((x-4.97988)/7.58226)*TMath::Exp(-1.49999*x))+0.94919;
      else if (idx == 3   ) num = 86.85890*(TMath::Erf((x-5.31012)/8.48064)*TMath::Exp(-1.49981*x))+0.94992;
      else if (idx == 4   ) num = 81.09598*(TMath::Erf((x-5.71563)/8.66698)*TMath::Exp(-1.49878*x))+0.95541;
      else if (idx == 5   ) num = 93.42656*(TMath::Erf((x-4.82737)/6.82912)*TMath::Exp(-1.49997*x))+0.95571;
      else if (idx == 6   ) num = 94.79913*(TMath::Erf((x-4.63487)/6.28754)*TMath::Exp(-1.49999*x))+0.95218;
      else if (idx == 7   ) num = 89.87939*(TMath::Erf((x-4.82644)/6.84329)*TMath::Exp(-1.49996*x))+0.95302;
      else if (idx == 8   ) num = 82.47365*(TMath::Erf((x-5.91002)/10.08817)*TMath::Exp(-1.49900*x))+0.94866;
      else if (idx == 9   ) num = 87.48036*(TMath::Erf((x-5.98639)/10.39991)*TMath::Exp(-1.49940*x))+0.95341;
      else if (idx == 10  ) num = 81.49499*(TMath::Erf((x-5.23574)/7.73630)*TMath::Exp(-1.50000*x))+0.95085;
      else if (idx == 11  ) num = 92.15477*(TMath::Erf((x-4.92497)/7.40597)*TMath::Exp(-1.49997*x))+0.95085;
      else if (idx == 12  ) num = 94.98548*(TMath::Erf((x-4.49819)/5.76047)*TMath::Exp(-1.50000*x))+0.94611;
      else if (idx == 13  ) num = 94.41285*(TMath::Erf((x-4.72740)/6.19228)*TMath::Exp(-1.49999*x))+0.95533;
      else if (idx == 14  ) num = 94.76472*(TMath::Erf((x-4.67195)/6.37421)*TMath::Exp(-1.50000*x))+0.94885;
      else if (idx == 15  ) num = 94.97166*(TMath::Erf((x-4.49728)/6.01967)*TMath::Exp(-1.49998*x))+0.94468;
      else if (idx == 16  ) num = 94.89024*(TMath::Erf((x-4.57083)/5.93030)*TMath::Exp(-1.50000*x))+0.95220;
      else if (idx == 17  ) num = 94.16549*(TMath::Erf((x-4.36792)/5.13373)*TMath::Exp(-1.49990*x))+0.94659;
      else if (idx == 18  ) num = 94.91201*(TMath::Erf((x-4.24665)/4.78414)*TMath::Exp(-1.49998*x))+0.94568;
      else if (idx == 19  ) num = 94.65233*(TMath::Erf((x-4.82675)/6.83337)*TMath::Exp(-1.50000*x))+0.95244;
      else if (idx == 20  ) num = 94.20794*(TMath::Erf((x-5.00925)/7.55090)*TMath::Exp(-1.49966*x))+0.95576;
      else if (idx == 21  ) num = 79.07162*(TMath::Erf((x-6.06349)/10.36345)*TMath::Exp(-1.49893*x))+0.95069;
      else if (idx == 22  ) num = 94.72021*(TMath::Erf((x-4.34670)/5.02586)*TMath::Exp(-1.49995*x))+0.94455;
      else if (idx == 23  ) num = 94.91923*(TMath::Erf((x-4.59698)/6.08311)*TMath::Exp(-1.49999*x))+0.94498;
      else if (idx == 24  ) num = 94.87610*(TMath::Erf((x-4.66170)/6.21751)*TMath::Exp(-1.49999*x))+0.95081;
      else if (idx == 25  ) num = 93.73590*(TMath::Erf((x-4.33493)/5.16180)*TMath::Exp(-1.49977*x))+0.94480;
      else if (idx == 26  ) num = 94.95048*(TMath::Erf((x-4.83768)/6.77248)*TMath::Exp(-1.49999*x))+0.95152;
      else if (idx == 27  ) num = 94.73046*(TMath::Erf((x-4.68769)/6.18611)*TMath::Exp(-1.49999*x))+0.95159;
      else if (idx == 28  ) num = 89.58622*(TMath::Erf((x-5.05709)/7.65024)*TMath::Exp(-1.49977*x))+0.95501;
      else if (idx == 29  ) num = 92.28078*(TMath::Erf((x-4.60922)/5.49678)*TMath::Exp(-1.49939*x))+0.94715;
      else if (idx == 30  ) num = 90.79895*(TMath::Erf((x-5.48926)/8.56395)*TMath::Exp(-1.49963*x))+0.95410;
      else if (idx == 31  ) num = 94.96700*(TMath::Erf((x-4.37567)/4.90569)*TMath::Exp(-1.49986*x))+0.94968;
      else if (idx == 32  ) num = 93.28861*(TMath::Erf((x-4.66564)/6.10113)*TMath::Exp(-1.48102*x))+0.94683;
      else if (idx == 33  ) num = 94.99934*(TMath::Erf((x-4.61930)/6.17122)*TMath::Exp(-1.49994*x))+0.94671;
      else if (idx == 34  ) num = 91.84844*(TMath::Erf((x-4.49732)/5.51728)*TMath::Exp(-1.49986*x))+0.94449;
      else if (idx == 35  ) num = 94.24081*(TMath::Erf((x-4.83163)/6.88860)*TMath::Exp(-1.49971*x))+0.94678;
      else if (idx == 36  ) num = 94.79182*(TMath::Erf((x-4.93026)/7.18160)*TMath::Exp(-1.50000*x))+0.95084;
      else if (idx == 37  ) num = 92.51666*(TMath::Erf((x-4.76610)/6.83054)*TMath::Exp(-1.49997*x))+0.94527;
      else if (idx == 38  ) num = 94.99470*(TMath::Erf((x-4.62696)/6.19962)*TMath::Exp(-1.50000*x))+0.94742;
      else if (idx == 39  ) num = 86.35167*(TMath::Erf((x-6.12526)/10.90641)*TMath::Exp(-1.49996*x))+0.95095;
      else if (idx == 40  ) num = 81.35021*(TMath::Erf((x-5.53001)/8.72138)*TMath::Exp(-1.49906*x))+0.94868;
      else if (idx == 41  ) num = 94.80007*(TMath::Erf((x-4.85771)/6.69031)*TMath::Exp(-1.49997*x))+0.95048;
      else if (idx == 42  ) num = 91.79418*(TMath::Erf((x-5.34915)/8.34733)*TMath::Exp(-1.49990*x))+0.95180;
      else if (idx == 43  ) num = 91.78697*(TMath::Erf((x-4.47827)/5.10846)*TMath::Exp(-1.49961*x))+0.94717;
      else if (idx == 44  ) num = 94.99280*(TMath::Erf((x-4.86610)/6.87443)*TMath::Exp(-1.49979*x))+0.95357;
      else if (idx == 45  ) num = 94.87460*(TMath::Erf((x-4.47402)/5.60675)*TMath::Exp(-1.50000*x))+0.94766;
      else if (idx == 46  ) num = 91.74051*(TMath::Erf((x-4.74327)/6.73971)*TMath::Exp(-1.49997*x))+0.95104;
      else if (idx == 47  ) num = 94.91671*(TMath::Erf((x-4.16911)/4.35847)*TMath::Exp(-1.49994*x))+0.94583;
      else if (idx == 48  ) num = 85.36531*(TMath::Erf((x-5.06875)/7.45304)*TMath::Exp(-1.49989*x))+0.94892;
      else if (idx == 49  ) num = 94.61995*(TMath::Erf((x-4.86429)/6.90287)*TMath::Exp(-1.49999*x))+0.95069;
      else if (idx == 50  ) num = 93.04836*(TMath::Erf((x-4.95253)/7.40959)*TMath::Exp(-1.49999*x))+0.95095;
      else if (idx == 51  ) num = 94.81109*(TMath::Erf((x-4.26944)/4.91964)*TMath::Exp(-1.49993*x))+0.94148;
      else if (idx == 52  ) num = 88.64768*(TMath::Erf((x-5.85751)/9.65458)*TMath::Exp(-1.49990*x))+0.95180;
      else if (idx == 53  ) num = 94.84106*(TMath::Erf((x-4.69451)/6.53556)*TMath::Exp(-1.50000*x))+0.95008;
      else if (idx == 54  ) num = 94.89174*(TMath::Erf((x-4.58049)/6.31195)*TMath::Exp(-1.49966*x))+0.95142;
      else if (idx == 55  ) num = 94.37639*(TMath::Erf((x-5.15788)/7.06742)*TMath::Exp(-1.49993*x))+0.95677;
      else if (idx == 56  ) num = 87.44906*(TMath::Erf((x-6.29544)/11.41360)*TMath::Exp(-1.50000*x))+0.95327;
      else if (idx == 57  ) num = 93.90302*(TMath::Erf((x-4.55669)/5.79909)*TMath::Exp(-1.49993*x))+0.94731;
      else if (idx == 58  ) num = 94.99370*(TMath::Erf((x-4.33642)/5.18539)*TMath::Exp(-1.49998*x))+0.94861;
      else if (idx == 59  ) num = 94.43803*(TMath::Erf((x-4.53218)/5.76037)*TMath::Exp(-1.49980*x))+0.95430;
      else if (idx == 60  ) num = 87.25272*(TMath::Erf((x-5.34858)/8.11765)*TMath::Exp(-1.50000*x))+0.95111;
      else if (idx == 61  ) num = 94.05032*(TMath::Erf((x-4.61442)/5.90546)*TMath::Exp(-1.50000*x))+0.94917;
      else if (idx == 62  ) num = 87.07823*(TMath::Erf((x-5.23101)/7.61005)*TMath::Exp(-1.50000*x))+0.95824;
      else if (idx == 63  ) num = 74.02462*(TMath::Erf((x-5.94265)/8.53645)*TMath::Exp(-1.49973*x))+0.95268;
      else if (idx == 64  ) num = 89.06128*(TMath::Erf((x-5.23831)/8.33578)*TMath::Exp(-1.50000*x))+0.94793;
      else if (idx == 65  ) num = 94.57390*(TMath::Erf((x-4.34330)/4.98453)*TMath::Exp(-1.49990*x))+0.94489;
      else if (idx == 66  ) num = 94.99238*(TMath::Erf((x-4.85443)/6.83129)*TMath::Exp(-1.49999*x))+0.95480;
      else if (idx == 67  ) num = 89.09012*(TMath::Erf((x-5.52198)/9.25298)*TMath::Exp(-1.49989*x))+0.95863;
      else if (idx == 68  ) num = 93.57314*(TMath::Erf((x-4.68736)/6.06433)*TMath::Exp(-1.49997*x))+0.95121;
      else if (idx == 69  ) num = 94.35749*(TMath::Erf((x-4.59776)/6.51755)*TMath::Exp(-1.49999*x))+0.94599;
      else if (idx == 70  ) num = 94.99665*(TMath::Erf((x-4.60646)/6.34059)*TMath::Exp(-1.50000*x))+0.94886;
      else if (idx == 71  ) num = 88.99178*(TMath::Erf((x-4.74835)/6.79139)*TMath::Exp(-1.49997*x))+0.95446;
      else if (idx == 72  ) num = 82.48938*(TMath::Erf((x-5.44225)/7.50889)*TMath::Exp(-1.49983*x))+0.94819;
      else if (idx == 73  ) num = 94.99760*(TMath::Erf((x-4.95618)/6.87817)*TMath::Exp(-1.50000*x))+0.95093;
      else if (idx == 74  ) num = 94.95289*(TMath::Erf((x-4.58449)/6.36183)*TMath::Exp(-1.50000*x))+0.94722;
      else if (idx == 75  ) num = 94.82428*(TMath::Erf((x-4.97762)/7.31559)*TMath::Exp(-1.50000*x))+0.95366;
      else if (idx == 76  ) num = 94.99472*(TMath::Erf((x-4.36532)/5.53351)*TMath::Exp(-1.49998*x))+0.94485;
      else if (idx == 77  ) num = 82.60913*(TMath::Erf((x-5.80678)/9.61463)*TMath::Exp(-1.49865*x))+0.95055;
      else if (idx == 78  ) num = 82.58123*(TMath::Erf((x-5.51125)/8.14651)*TMath::Exp(-1.49919*x))+0.95809;
      else if (idx == 79  ) num = 94.99446*(TMath::Erf((x-4.50998)/5.69243)*TMath::Exp(-1.49996*x))+0.95160;
      else if (idx == 80  ) num = 94.94623*(TMath::Erf((x-4.32704)/5.46652)*TMath::Exp(-1.49999*x))+0.94007;
      else if (idx == 81  ) num = 93.96295*(TMath::Erf((x-4.92255)/7.58780)*TMath::Exp(-1.49999*x))+0.94872;
      else if (idx == 82  ) num = 89.32906*(TMath::Erf((x-5.02813)/7.32291)*TMath::Exp(-1.49996*x))+0.95082;
      else if (idx == 83  ) num = 94.47113*(TMath::Erf((x-4.91263)/6.51430)*TMath::Exp(-1.49987*x))+0.95290;
      else if (idx == 84  ) num = 91.41446*(TMath::Erf((x-5.18269)/7.90146)*TMath::Exp(-1.49994*x))+0.95416;
      else if (idx == 85  ) num = 94.97810*(TMath::Erf((x-4.85376)/6.80329)*TMath::Exp(-1.49963*x))+0.95212;
      else if (idx == 86  ) num = 94.51536*(TMath::Erf((x-4.57186)/5.59814)*TMath::Exp(-1.49994*x))+0.94856;
      else if (idx == 87  ) num = 92.97989*(TMath::Erf((x-4.73176)/6.63358)*TMath::Exp(-1.49998*x))+0.95255;
      else if (idx == 88  ) num = 85.95634*(TMath::Erf((x-5.12706)/7.44943)*TMath::Exp(-1.49927*x))+0.95181;
      else if (idx == 89  ) num = 89.19166*(TMath::Erf((x-6.13335)/11.16525)*TMath::Exp(-1.49999*x))+0.95530;
      else if (idx == 90  ) num = 94.84575*(TMath::Erf((x-4.40669)/5.44604)*TMath::Exp(-1.49995*x))+0.94597;
      else if (idx == 91  ) num = 94.52521*(TMath::Erf((x-4.83289)/6.85275)*TMath::Exp(-1.49998*x))+0.95112;
      else if (idx == 92  ) num = 94.99951*(TMath::Erf((x-4.38423)/5.39317)*TMath::Exp(-1.50000*x))+0.94530;
      else if (idx == 93  ) num = 93.42213*(TMath::Erf((x-4.69595)/5.82646)*TMath::Exp(-1.49989*x))+0.95048;
      else if (idx == 94  ) num = 92.44821*(TMath::Erf((x-4.82342)/6.88786)*TMath::Exp(-1.49995*x))+0.94763;
      else if (idx == 95  ) num = 87.00837*(TMath::Erf((x-5.06860)/7.32795)*TMath::Exp(-1.50000*x))+0.95117;
      else if (idx == 96  ) num = 94.99777*(TMath::Erf((x-4.57105)/5.86455)*TMath::Exp(-1.49999*x))+0.94437;
      else if (idx == 97  ) num = 84.79898*(TMath::Erf((x-5.37253)/8.07389)*TMath::Exp(-1.49988*x))+0.94528;
      else if (idx == 98  ) num = 91.78802*(TMath::Erf((x-4.34420)/5.08925)*TMath::Exp(-1.50000*x))+0.94833;
      else if (idx == 99  ) num = 83.78606*(TMath::Erf((x-6.51352)/12.07844)*TMath::Exp(-1.49992*x))+0.94917;
      else if (idx == 100 ) num = 92.91111*(TMath::Erf((x-4.54362)/5.64101)*TMath::Exp(-1.49998*x))+0.94959;
   }
   else if (fabs(eta)<2.1)
   {
      if (idx==0) num = 0.58006*(TMath::Erf((x-3.98576)/1.14408)*TMath::Exp(-0.49759*x))+0.88544;
      else if (idx == -1   ) num =   0.36154*(TMath::Erf((x-3.81556)/1.04814)*TMath::Exp(-0.36171*x))+0.87868;
      else if (idx == -2   ) num =   0.95807*(TMath::Erf((x-4.18120)/1.13216)*TMath::Exp(-0.66829*x))+0.89045;
      else if (idx == 1   ) num = 1.25161*(TMath::Erf((x-8.67697)/40.57401)*TMath::Exp(-0.16061*x))+0.93824;
      else if (idx == 2   ) num = 0.47012*(TMath::Erf((x-3.80085)/1.03240)*TMath::Exp(-0.47425*x))+0.87829;
      else if (idx == 3   ) num = 0.06713*(TMath::Erf((x-3.37784)/0.40157)*TMath::Exp(-0.00000*x))+0.82895;
      else if (idx == 4   ) num = 0.96064*(TMath::Erf((x-4.59590)/1.70164)*TMath::Exp(-0.62987*x))+0.90692;
      else if (idx == 5   ) num = 0.30605*(TMath::Erf((x-3.95722)/0.96082)*TMath::Exp(-0.31731*x))+0.89853;
      else if (idx == 6   ) num = 0.09904*(TMath::Erf((x-3.50163)/0.84522)*TMath::Exp(-0.03277*x))+0.84070;
      else if (idx == 7   ) num = 0.20338*(TMath::Erf((x-3.43873)/0.99164)*TMath::Exp(-0.11978*x))+0.81928;
      else if (idx == 8   ) num = 0.45513*(TMath::Erf((x-3.57881)/1.64742)*TMath::Exp(-0.29895*x))+0.85097;
      else if (idx == 9   ) num = 1.03726*(TMath::Erf((x-4.49475)/1.32284)*TMath::Exp(-0.65783*x))+0.90775;
      else if (idx == 10  ) num = 0.48229*(TMath::Erf((x-4.33119)/1.17074)*TMath::Exp(-0.40665*x))+0.91443;
      else if (idx == 11  ) num = 0.74371*(TMath::Erf((x-3.98657)/1.05305)*TMath::Exp(-0.55775*x))+0.88356;
      else if (idx == 12  ) num = 0.10617*(TMath::Erf((x-3.10032)/0.20612)*TMath::Exp(-0.01981*x))+0.80550;
      else if (idx == 13  ) num = 0.57426*(TMath::Erf((x-4.30909)/1.14994)*TMath::Exp(-0.49629*x))+0.88463;
      else if (idx == 14  ) num = 0.19661*(TMath::Erf((x-3.73325)/0.48744)*TMath::Exp(-0.27972*x))+0.86362;
      else if (idx == 15  ) num = 2.92352*(TMath::Erf((x-3.91299)/6.94454)*TMath::Exp(-0.58454*x))+0.88810;
      else if (idx == 16  ) num = 0.51122*(TMath::Erf((x-4.24862)/0.10394)*TMath::Exp(-0.55829*x))+0.89117;
      else if (idx == 17  ) num = 0.30769*(TMath::Erf((x-3.95823)/0.77008)*TMath::Exp(-0.31111*x))+0.87886;
      else if (idx == 18  ) num = 0.15442*(TMath::Erf((x-3.24865)/1.02046)*TMath::Exp(-0.06128*x))+0.81927;
      else if (idx == 19  ) num = 1.22186*(TMath::Erf((x-3.74924)/0.63124)*TMath::Exp(-0.79882*x))+0.88207;
      else if (idx == 20  ) num = 0.35948*(TMath::Erf((x-4.03441)/1.24416)*TMath::Exp(-0.34445*x))+0.87511;
      else if (idx == 21  ) num = 0.29150*(TMath::Erf((x-4.29994)/1.15704)*TMath::Exp(-0.36921*x))+0.88601;
      else if (idx == 22  ) num = 0.69880*(TMath::Erf((x-4.46239)/1.79526)*TMath::Exp(-0.51083*x))+0.89926;
      else if (idx == 23  ) num = 1.00727*(TMath::Erf((x-3.95369)/0.94413)*TMath::Exp(-0.71537*x))+0.88555;
      else if (idx == 24  ) num = 0.32616*(TMath::Erf((x-3.86543)/0.90002)*TMath::Exp(-0.36109*x))+0.88711;
      else if (idx == 25  ) num = 1.01826*(TMath::Erf((x-4.18649)/0.91961)*TMath::Exp(-0.81599*x))+0.86988;
      else if (idx == 26  ) num = 0.55704*(TMath::Erf((x-4.16560)/0.86539)*TMath::Exp(-0.45343*x))+0.88833;
      else if (idx == 27  ) num = 0.13505*(TMath::Erf((x-3.26987)/0.54463)*TMath::Exp(-0.09346*x))+0.82504;
      else if (idx == 28  ) num = 0.13854*(TMath::Erf((x-3.18128)/0.65015)*TMath::Exp(-0.04886*x))+0.78462;
      else if (idx == 29  ) num = 0.90826*(TMath::Erf((x-3.71263)/0.83905)*TMath::Exp(-0.65542*x))+0.89665;
      else if (idx == 30  ) num = 0.61089*(TMath::Erf((x-4.43678)/1.88320)*TMath::Exp(-0.45036*x))+0.89237;
      else if (idx == 31  ) num = 0.52629*(TMath::Erf((x-4.15978)/1.14083)*TMath::Exp(-0.48186*x))+0.89084;
      else if (idx == 32  ) num = 0.42839*(TMath::Erf((x-4.14522)/1.23848)*TMath::Exp(-0.40807*x))+0.89195;
      else if (idx == 33  ) num = 0.10002*(TMath::Erf((x-3.31649)/0.39911)*TMath::Exp(-0.04015*x))+0.81570;
      else if (idx == 34  ) num = 0.33224*(TMath::Erf((x-4.12685)/1.33636)*TMath::Exp(-0.31554*x))+0.88940;
      else if (idx == 35  ) num = 0.11063*(TMath::Erf((x-3.17662)/0.17646)*TMath::Exp(-0.04852*x))+0.80740;
      else if (idx == 36  ) num = 0.96192*(TMath::Erf((x-5.20229)/2.16128)*TMath::Exp(-0.68933*x))+0.90719;
      else if (idx == 37  ) num = 7.59957*(TMath::Erf((x-3.33049)/25.93489)*TMath::Exp(-0.41608*x))+0.84709;
      else if (idx == 38  ) num = 1.25947*(TMath::Erf((x-3.82581)/4.36434)*TMath::Exp(-0.37260*x))+0.86277;
      else if (idx == 39  ) num = 1.68598*(TMath::Erf((x-4.33249)/0.26176)*TMath::Exp(-0.85428*x))+0.91785;
      else if (idx == 40  ) num = 0.40026*(TMath::Erf((x-3.62439)/1.36146)*TMath::Exp(-0.28438*x))+0.85531;
      else if (idx == 41  ) num = 0.54457*(TMath::Erf((x-4.10413)/1.21437)*TMath::Exp(-0.47052*x))+0.88066;
      else if (idx == 42  ) num = 0.62792*(TMath::Erf((x-3.67121)/1.93650)*TMath::Exp(-0.43438*x))+0.87185;
      else if (idx == 43  ) num = 1.65006*(TMath::Erf((x-4.21660)/1.21525)*TMath::Exp(-0.81333*x))+0.90210;
      else if (idx == 44  ) num = 0.55767*(TMath::Erf((x-4.62867)/1.04470)*TMath::Exp(-0.51895*x))+0.90348;
      else if (idx == 45  ) num = 0.11976*(TMath::Erf((x-3.17904)/0.17400)*TMath::Exp(-0.07555*x))+0.81261;
      else if (idx == 46  ) num = 0.74376*(TMath::Erf((x-4.67633)/1.53283)*TMath::Exp(-0.56480*x))+0.89985;
      else if (idx == 47  ) num = 0.09663*(TMath::Erf((x-3.19317)/0.16879)*TMath::Exp(-0.09225*x))+0.82733;
      else if (idx == 48  ) num = 1.30187*(TMath::Erf((x-4.36926)/1.30722)*TMath::Exp(-0.76526*x))+0.92207;
      else if (idx == 49  ) num = 1.20031*(TMath::Erf((x-3.64585)/0.30599)*TMath::Exp(-0.82393*x))+0.88755;
      else if (idx == 50  ) num = 1.90898*(TMath::Erf((x-3.51839)/6.47370)*TMath::Exp(-0.39830*x))+0.85037;
      else if (idx == 51  ) num = 0.17216*(TMath::Erf((x-3.18488)/1.25456)*TMath::Exp(-0.07341*x))+0.80489;
      else if (idx == 52  ) num = 0.07610*(TMath::Erf((x-3.16623)/0.10645)*TMath::Exp(-0.00009*x))+0.80873;
      else if (idx == 53  ) num = 3.42594*(TMath::Erf((x-3.96898)/10.29937)*TMath::Exp(-0.58307*x))+0.88298;
      else if (idx == 54  ) num = 0.52828*(TMath::Erf((x-3.99131)/0.86962)*TMath::Exp(-0.45913*x))+0.88443;
      else if (idx == 55  ) num = 6.06271*(TMath::Erf((x-9.99949)/35.68823)*TMath::Exp(-0.82892*x))+0.92738;
      else if (idx == 56  ) num = 0.82867*(TMath::Erf((x-4.35027)/1.36740)*TMath::Exp(-0.55686*x))+0.90923;
      else if (idx == 57  ) num = 21.98481*(TMath::Erf((x-8.14569)/20.69032)*TMath::Exp(-1.49958*x))+0.89171;
      else if (idx == 58  ) num = 1.80779*(TMath::Erf((x-4.25083)/4.53601)*TMath::Exp(-0.54160*x))+0.90549;
      else if (idx == 59  ) num = 0.99311*(TMath::Erf((x-3.51556)/1.16315)*TMath::Exp(-0.60631*x))+0.88041;
      else if (idx == 60  ) num = 0.39143*(TMath::Erf((x-4.11444)/1.00792)*TMath::Exp(-0.39596*x))+0.87222;
      else if (idx == 61  ) num = 0.11552*(TMath::Erf((x-3.14117)/0.65733)*TMath::Exp(-0.04080*x))+0.80106;
      else if (idx == 62  ) num = 0.26072*(TMath::Erf((x-3.87852)/0.97671)*TMath::Exp(-0.25893*x))+0.86442;
      else if (idx == 63  ) num = 2.38848*(TMath::Erf((x-4.94527)/6.28842)*TMath::Exp(-0.70697*x))+0.91307;
      else if (idx == 64  ) num = 0.69544*(TMath::Erf((x-3.67439)/1.29792)*TMath::Exp(-0.46616*x))+0.86984;
      else if (idx == 65  ) num = 6.42594*(TMath::Erf((x-3.98326)/15.58046)*TMath::Exp(-0.56673*x))+0.88577;
      else if (idx == 66  ) num = 1.33890*(TMath::Erf((x-3.77711)/0.80193)*TMath::Exp(-0.76240*x))+0.87850;
      else if (idx == 67  ) num = 1.34653*(TMath::Erf((x-3.50206)/1.59514)*TMath::Exp(-0.64704*x))+0.87649;
      else if (idx == 68  ) num = 0.64115*(TMath::Erf((x-3.54134)/1.35062)*TMath::Exp(-0.55019*x))+0.85521;
      else if (idx == 69  ) num = 0.54391*(TMath::Erf((x-3.49512)/1.93031)*TMath::Exp(-0.35562*x))+0.84593;
      else if (idx == 70  ) num = 0.14142*(TMath::Erf((x-3.52042)/0.72584)*TMath::Exp(-0.05668*x))+0.81110;
      else if (idx == 71  ) num = 0.09558*(TMath::Erf((x-3.13182)/0.15259)*TMath::Exp(-0.02338*x))+0.80314;
      else if (idx == 72  ) num = 1.81544*(TMath::Erf((x-4.64910)/3.63955)*TMath::Exp(-0.73270*x))+0.89652;
      else if (idx == 73  ) num = 0.58164*(TMath::Erf((x-3.88425)/1.04468)*TMath::Exp(-0.50427*x))+0.88108;
      else if (idx == 74  ) num = 0.39757*(TMath::Erf((x-3.83383)/1.28394)*TMath::Exp(-0.30168*x))+0.86898;
      else if (idx == 75  ) num = 0.11811*(TMath::Erf((x-3.09593)/0.36937)*TMath::Exp(-0.01179*x))+0.77891;
      else if (idx == 76  ) num = 1.44105*(TMath::Erf((x-4.16994)/1.35851)*TMath::Exp(-0.83816*x))+0.88443;
      else if (idx == 77  ) num = 0.62936*(TMath::Erf((x-1.98111)/3.97866)*TMath::Exp(-0.10610*x))+0.64494;
      else if (idx == 78  ) num = 2.23145*(TMath::Erf((x-3.62395)/4.73337)*TMath::Exp(-0.56671*x))+0.87049;
      else if (idx == 79  ) num = 0.58205*(TMath::Erf((x-4.36322)/2.04669)*TMath::Exp(-0.45905*x))+0.89930;
      else if (idx == 80  ) num = 0.60167*(TMath::Erf((x-3.75106)/1.78863)*TMath::Exp(-0.50122*x))+0.87974;
      else if (idx == 81  ) num = 0.22412*(TMath::Erf((x-3.54297)/0.97127)*TMath::Exp(-0.24226*x))+0.84114;
      else if (idx == 82  ) num = 0.08257*(TMath::Erf((x-3.72477)/0.78439)*TMath::Exp(-0.00000*x))+0.83405;
      else if (idx == 83  ) num = 1.74557*(TMath::Erf((x-4.03491)/1.06515)*TMath::Exp(-0.86749*x))+0.89555;
      else if (idx == 84  ) num = 0.09836*(TMath::Erf((x-3.37926)/0.63931)*TMath::Exp(-0.00072*x))+0.81930;
      else if (idx == 85  ) num = 0.17924*(TMath::Erf((x-3.34378)/1.02970)*TMath::Exp(-0.13856*x))+0.82659;
      else if (idx == 86  ) num = 0.95665*(TMath::Erf((x-4.17553)/1.14465)*TMath::Exp(-0.67010*x))+0.90564;
      else if (idx == 87  ) num = 0.36265*(TMath::Erf((x-3.71783)/0.77082)*TMath::Exp(-0.40988*x))+0.86226;
      else if (idx == 88  ) num = 0.31757*(TMath::Erf((x-3.57668)/1.92487)*TMath::Exp(-0.17133*x))+0.83440;
      else if (idx == 89  ) num = 3.83257*(TMath::Erf((x-9.99905)/25.76176)*TMath::Exp(-0.71999*x))+0.92337;
      else if (idx == 90  ) num = 1.23559*(TMath::Erf((x-4.05804)/0.41091)*TMath::Exp(-0.77693*x))+0.89327;
      else if (idx == 91  ) num = 0.71027*(TMath::Erf((x-3.98790)/1.37875)*TMath::Exp(-0.49879*x))+0.89132;
      else if (idx == 92  ) num = 1.02516*(TMath::Erf((x-4.44693)/1.07395)*TMath::Exp(-0.72695*x))+0.90343;
      else if (idx == 93  ) num = 0.29306*(TMath::Erf((x-3.89289)/0.97429)*TMath::Exp(-0.34084*x))+0.87524;
      else if (idx == 94  ) num = 0.07258*(TMath::Erf((x-3.48928)/0.72417)*TMath::Exp(-0.00738*x))+0.84062;
      else if (idx == 95  ) num = 0.84661*(TMath::Erf((x-4.33826)/1.32621)*TMath::Exp(-0.61549*x))+0.90520;
      else if (idx == 96  ) num = 0.09690*(TMath::Erf((x-3.22088)/0.00441)*TMath::Exp(-0.02114*x))+0.81310;
      else if (idx == 97  ) num = 0.35057*(TMath::Erf((x-3.49492)/0.62119)*TMath::Exp(-0.35377*x))+0.85977;
      else if (idx == 98  ) num = 0.23629*(TMath::Erf((x-4.19407)/1.19414)*TMath::Exp(-0.25635*x))+0.87927;
      else if (idx == 99  ) num = 0.33720*(TMath::Erf((x-3.76604)/1.04984)*TMath::Exp(-0.33580*x))+0.87034;
      else if (idx == 100 ) num = 0.68458*(TMath::Erf((x-4.11843)/1.48773)*TMath::Exp(-0.49505*x))+0.87800;
   }
   else
   {
      if (idx==0) num = 0.27895*(TMath::Erf((x-2.97052)/1.78136)*TMath::Exp(-0.21840*x))+0.84463;
      else if (idx == -1   ) num =   0.23809*(TMath::Erf((x-2.80014)/1.89823)*TMath::Exp(-0.18284*x))+0.83996;
      else if (idx == -2   ) num =   0.39208*(TMath::Erf((x-3.10618)/1.91259)*TMath::Exp(-0.28409*x))+0.85036;
      else if (idx == 1   ) num = 0.14027*(TMath::Erf((x-3.33082)/0.38099)*TMath::Exp(-0.20509*x))+0.85982;
      else if (idx == 2   ) num = 1.06113*(TMath::Erf((x-3.45501)/3.26986)*TMath::Exp(-0.51897*x))+0.89071;
      else if (idx == 3   ) num = 0.37691*(TMath::Erf((x-3.43082)/0.00362)*TMath::Exp(-0.49070*x))+0.88952;
      else if (idx == 4   ) num = 5.36257*(TMath::Erf((x-3.05702)/13.96309)*TMath::Exp(-0.59764*x))+0.86107;
      else if (idx == 5   ) num = 4.39782*(TMath::Erf((x-3.24503)/17.69668)*TMath::Exp(-0.46456*x))+0.87887;
      else if (idx == 6   ) num = 0.19045*(TMath::Erf((x-3.28775)/0.42565)*TMath::Exp(-0.26105*x))+0.88077;
      else if (idx == 7   ) num = 2.28583*(TMath::Erf((x-0.73518)/11.46147)*TMath::Exp(-0.16997*x))+0.53561;
      else if (idx == 8   ) num = 0.54277*(TMath::Erf((x-2.50977)/2.71606)*TMath::Exp(-0.22738*x))+0.80347;
      else if (idx == 9   ) num = 0.70336*(TMath::Erf((x-3.91350)/2.82314)*TMath::Exp(-0.50477*x))+0.89919;
      else if (idx == 10  ) num = 0.46717*(TMath::Erf((x-3.46458)/0.22939)*TMath::Exp(-0.59767*x))+0.88980;
      else if (idx == 11  ) num = 0.80434*(TMath::Erf((x-1.55917)/5.43695)*TMath::Exp(-0.18502*x))+0.72858;
      else if (idx == 12  ) num = 0.20416*(TMath::Erf((x-3.06151)/0.95140)*TMath::Exp(-0.22755*x))+0.86149;
      else if (idx == 13  ) num = 0.24883*(TMath::Erf((x-3.30108)/0.89359)*TMath::Exp(-0.32383*x))+0.87509;
      else if (idx == 14  ) num = 0.98218*(TMath::Erf((x-1.97451)/5.47738)*TMath::Exp(-0.20041*x))+0.72774;
      else if (idx == 15  ) num = 0.55091*(TMath::Erf((x-3.38059)/0.22538)*TMath::Exp(-0.58540*x))+0.91532;
      else if (idx == 16  ) num = 0.27221*(TMath::Erf((x-2.85192)/1.71948)*TMath::Exp(-0.20889*x))+0.83747;
      else if (idx == 17  ) num = 1.65286*(TMath::Erf((x+1.02679)/9.59422)*TMath::Exp(-0.07792*x))+0.24012;
      else if (idx == 18  ) num = 0.57014*(TMath::Erf((x-3.19046)/2.00847)*TMath::Exp(-0.46932*x))+0.88393;
      else if (idx == 19  ) num = 0.23197*(TMath::Erf((x-4.14399)/0.00000)*TMath::Exp(-0.38936*x))+0.88938;
      else if (idx == 20  ) num = 0.19376*(TMath::Erf((x-3.19880)/0.85616)*TMath::Exp(-0.20760*x))+0.86722;
      else if (idx == 21  ) num = 0.29983*(TMath::Erf((x-4.09113)/0.25126)*TMath::Exp(-0.35171*x))+0.90165;
      else if (idx == 22  ) num = 0.46874*(TMath::Erf((x-2.37722)/2.70317)*TMath::Exp(-0.14824*x))+0.75156;
      else if (idx == 23  ) num = 0.46261*(TMath::Erf((x-3.18350)/1.73017)*TMath::Exp(-0.43307*x))+0.88715;
      else if (idx == 24  ) num = 1.24485*(TMath::Erf((x-2.52103)/5.48468)*TMath::Exp(-0.27684*x))+0.78293;
      else if (idx == 25  ) num = 0.16516*(TMath::Erf((x-3.19862)/0.78668)*TMath::Exp(-0.25947*x))+0.85487;
      else if (idx == 26  ) num = 2.98891*(TMath::Erf((x-3.86522)/12.14687)*TMath::Exp(-0.54215*x))+0.90606;
      else if (idx == 27  ) num = 0.42280*(TMath::Erf((x-2.29134)/3.10617)*TMath::Exp(-0.14084*x))+0.78370;
      else if (idx == 28  ) num = 0.63032*(TMath::Erf((x-3.69296)/0.51097)*TMath::Exp(-0.62054*x))+0.90611;
      else if (idx == 29  ) num = 0.47976*(TMath::Erf((x-3.40021)/0.25661)*TMath::Exp(-0.70432*x))+0.91078;
      else if (idx == 30  ) num = 0.19672*(TMath::Erf((x-3.24323)/0.65127)*TMath::Exp(-0.25100*x))+0.86415;
      else if (idx == 31  ) num = 2.89523*(TMath::Erf((x-2.37560)/9.50823)*TMath::Exp(-0.36468*x))+0.78214;
      else if (idx == 32  ) num = 0.18563*(TMath::Erf((x-2.13759)/1.17245)*TMath::Exp(-0.00645*x))+0.75105;
      else if (idx == 33  ) num = 0.26613*(TMath::Erf((x-2.72030)/1.88811)*TMath::Exp(-0.19674*x))+0.83144;
      else if (idx == 34  ) num = 0.40646*(TMath::Erf((x-2.57022)/3.26019)*TMath::Exp(-0.20086*x))+0.82946;
      else if (idx == 35  ) num = 0.42451*(TMath::Erf((x-2.55084)/2.74906)*TMath::Exp(-0.21760*x))+0.81260;
      else if (idx == 36  ) num = 0.80890*(TMath::Erf((x-2.91264)/2.43388)*TMath::Exp(-0.41246*x))+0.84991;
      else if (idx == 37  ) num = 0.44029*(TMath::Erf((x-3.27587)/1.44944)*TMath::Exp(-0.42017*x))+0.88470;
      else if (idx == 38  ) num = 0.50011*(TMath::Erf((x-3.47489)/0.45556)*TMath::Exp(-0.57878*x))+0.90234;
      else if (idx == 39  ) num = 0.21456*(TMath::Erf((x-3.45723)/0.20557)*TMath::Exp(-0.29418*x))+0.88319;
      else if (idx == 40  ) num = 0.13201*(TMath::Erf((x-3.38602)/0.89386)*TMath::Exp(-0.13041*x))+0.87208;
      else if (idx == 41  ) num = 0.68751*(TMath::Erf((x-2.99258)/2.16702)*TMath::Exp(-0.49359*x))+0.87388;
      else if (idx == 42  ) num = 0.84975*(TMath::Erf((x-0.88525)/6.43384)*TMath::Exp(-0.11936*x))+0.62185;
      else if (idx == 43  ) num = 0.25078*(TMath::Erf((x-2.39786)/2.54039)*TMath::Exp(-0.15012*x))+0.84190;
      else if (idx == 44  ) num = 0.66303*(TMath::Erf((x-4.06893)/0.95654)*TMath::Exp(-0.71945*x))+0.89575;
      else if (idx == 45  ) num = 0.69628*(TMath::Erf((x-3.21584)/1.65685)*TMath::Exp(-0.58241*x))+0.89789;
      else if (idx == 46  ) num = 0.32489*(TMath::Erf((x-2.55636)/3.07991)*TMath::Exp(-0.19165*x))+0.82313;
      else if (idx == 47  ) num = 0.50999*(TMath::Erf((x-2.28730)/5.05003)*TMath::Exp(-0.18319*x))+0.80460;
      else if (idx == 48  ) num = 0.26750*(TMath::Erf((x-3.32211)/1.46668)*TMath::Exp(-0.28783*x))+0.86864;
      else if (idx == 49  ) num = 0.94718*(TMath::Erf((x-3.16424)/0.53665)*TMath::Exp(-0.88445*x))+0.90022;
      else if (idx == 50  ) num = 0.18973*(TMath::Erf((x-2.13506)/1.86637)*TMath::Exp(-0.01410*x))+0.75617;
      else if (idx == 51  ) num = 0.64542*(TMath::Erf((x-2.23696)/3.16324)*TMath::Exp(-0.22942*x))+0.76446;
      else if (idx == 52  ) num = 4.20331*(TMath::Erf((x-2.63244)/16.31619)*TMath::Exp(-0.38527*x))+0.82282;
      else if (idx == 53  ) num = 0.38290*(TMath::Erf((x-3.37294)/1.76670)*TMath::Exp(-0.40991*x))+0.87698;
      else if (idx == 54  ) num = 1.49028*(TMath::Erf((x-2.86269)/8.38928)*TMath::Exp(-0.25680*x))+0.82822;
      else if (idx == 55  ) num = 0.53762*(TMath::Erf((x-2.76185)/2.12408)*TMath::Exp(-0.27265*x))+0.81087;
      else if (idx == 56  ) num = 2.75463*(TMath::Erf((x-1.99539)/11.49034)*TMath::Exp(-0.23727*x))+0.71823;
      else if (idx == 57  ) num = 1.17394*(TMath::Erf((x-6.93172)/10.16046)*TMath::Exp(-0.63756*x))+0.92340;
      else if (idx == 58  ) num = 0.08157*(TMath::Erf((x-3.29999)/0.61147)*TMath::Exp(-0.01408*x))+0.83405;
      else if (idx == 59  ) num = 0.11869*(TMath::Erf((x-3.37230)/0.17742)*TMath::Exp(-0.12075*x))+0.85757;
      else if (idx == 60  ) num = 0.21414*(TMath::Erf((x-2.74635)/1.83496)*TMath::Exp(-0.17306*x))+0.84262;
      else if (idx == 61  ) num = 0.38103*(TMath::Erf((x-2.41690)/2.11560)*TMath::Exp(-0.18587*x))+0.79291;
      else if (idx == 62  ) num = 0.08462*(TMath::Erf((x-3.34499)/0.41768)*TMath::Exp(-0.04916*x))+0.85507;
      else if (idx == 63  ) num = 0.08942*(TMath::Erf((x-2.92783)/0.82405)*TMath::Exp(-0.00270*x))+0.83298;
      else if (idx == 64  ) num = 0.18140*(TMath::Erf((x-3.19537)/2.67613)*TMath::Exp(-0.13280*x))+0.85671;
      else if (idx == 65  ) num = 0.63467*(TMath::Erf((x-3.57618)/0.03226)*TMath::Exp(-0.74801*x))+0.89680;
      else if (idx == 66  ) num = 12.66512*(TMath::Erf((x-3.01391)/36.43229)*TMath::Exp(-0.50713*x))+0.86560;
      else if (idx == 67  ) num = 0.80977*(TMath::Erf((x-3.04625)/3.61007)*TMath::Exp(-0.35674*x))+0.85526;
      else if (idx == 68  ) num = 1.32428*(TMath::Erf((x-8.84971)/12.16741)*TMath::Exp(-0.59122*x))+0.94425;
      else if (idx == 69  ) num = 5.02230*(TMath::Erf((x-2.06190)/17.19680)*TMath::Exp(-0.38771*x))+0.77382;
      else if (idx == 70  ) num = 0.43263*(TMath::Erf((x-2.51597)/3.86836)*TMath::Exp(-0.20436*x))+0.80495;
      else if (idx == 71  ) num = 0.20941*(TMath::Erf((x-2.75014)/1.54217)*TMath::Exp(-0.06198*x))+0.79230;
      else if (idx == 72  ) num = 0.49125*(TMath::Erf((x-2.25259)/2.46397)*TMath::Exp(-0.16315*x))+0.74898;
      else if (idx == 73  ) num = 5.28083*(TMath::Erf((x-3.02938)/12.05809)*TMath::Exp(-0.55855*x))+0.86368;
      else if (idx == 74  ) num = 0.63578*(TMath::Erf((x-3.39073)/1.32547)*TMath::Exp(-0.49719*x))+0.89724;
      else if (idx == 75  ) num = 7.97896*(TMath::Erf((x-2.59441)/17.99644)*TMath::Exp(-0.48268*x))+0.81010;
      else if (idx == 76  ) num = 1.22728*(TMath::Erf((x-3.58571)/0.25880)*TMath::Exp(-0.93700*x))+0.91287;
      else if (idx == 77  ) num = 0.77442*(TMath::Erf((x-1.03758)/5.21651)*TMath::Exp(-0.09985*x))+0.58635;
      else if (idx == 78  ) num = 0.13989*(TMath::Erf((x-3.04320)/0.54297)*TMath::Exp(-0.21263*x))+0.86373;
      else if (idx == 79  ) num = 1.26519*(TMath::Erf((x-3.30511)/3.62565)*TMath::Exp(-0.57772*x))+0.87829;
      else if (idx == 80  ) num = 2.82235*(TMath::Erf((x-2.51950)/12.44394)*TMath::Exp(-0.28637*x))+0.79644;
      else if (idx == 81  ) num = 0.06354*(TMath::Erf((x-3.35744)/0.14266)*TMath::Exp(-0.00002*x))+0.86957;
      else if (idx == 82  ) num = 0.49921*(TMath::Erf((x-2.70806)/2.15818)*TMath::Exp(-0.24483*x))+0.81692;
      else if (idx == 83  ) num = 0.50329*(TMath::Erf((x-3.65870)/2.61130)*TMath::Exp(-0.40870*x))+0.88938;
      else if (idx == 84  ) num = 0.70123*(TMath::Erf((x-0.93084)/6.29848)*TMath::Exp(-0.12186*x))+0.67424;
      else if (idx == 85  ) num = 0.09363*(TMath::Erf((x-3.06615)/1.01246)*TMath::Exp(-0.00012*x))+0.83786;
      else if (idx == 86  ) num = 0.44050*(TMath::Erf((x-3.16184)/1.22545)*TMath::Exp(-0.45021*x))+0.89057;
      else if (idx == 87  ) num = 3.41376*(TMath::Erf((x-2.36615)/10.95138)*TMath::Exp(-0.33455*x))+0.77252;
      else if (idx == 88  ) num = 0.99986*(TMath::Erf((x-2.67002)/2.75751)*TMath::Exp(-0.39441*x))+0.83645;
      else if (idx == 89  ) num = 6.56079*(TMath::Erf((x-2.14609)/17.48667)*TMath::Exp(-0.42094*x))+0.76971;
      else if (idx == 90  ) num = 0.22053*(TMath::Erf((x-2.93202)/1.94333)*TMath::Exp(-0.18856*x))+0.84753;
      else if (idx == 91  ) num = 1.43926*(TMath::Erf((x-1.46360)/6.69721)*TMath::Exp(-0.18967*x))+0.62366;
      else if (idx == 92  ) num = 1.67508*(TMath::Erf((x-1.19590)/9.94281)*TMath::Exp(-0.16444*x))+0.61990;
      else if (idx == 93  ) num = 0.95206*(TMath::Erf((x-3.65221)/2.82985)*TMath::Exp(-0.59265*x))+0.88709;
      else if (idx == 94  ) num = 0.33870*(TMath::Erf((x-2.67566)/1.72653)*TMath::Exp(-0.22745*x))+0.83871;
      else if (idx == 95  ) num = 0.30642*(TMath::Erf((x-2.78696)/2.01806)*TMath::Exp(-0.24681*x))+0.82579;
      else if (idx == 96  ) num = 0.12437*(TMath::Erf((x-3.32324)/0.54161)*TMath::Exp(-0.20831*x))+0.86775;
      else if (idx == 97  ) num = 1.46635*(TMath::Erf((x-3.45632)/0.20727)*TMath::Exp(-1.07016*x))+0.89859;
      else if (idx == 98  ) num = 0.40670*(TMath::Erf((x-2.24245)/3.08692)*TMath::Exp(-0.15190*x))+0.76694;
      else if (idx == 99  ) num = 0.16716*(TMath::Erf((x-3.41317)/0.25949)*TMath::Exp(-0.35316*x))+0.88894;
      else if (idx == 100 ) num = 0.19576*(TMath::Erf((x-3.29978)/1.44751)*TMath::Exp(-0.16775*x))+0.85901;
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
         if (x<4) return 1.05166;
         else if (x<4.5) return 1.01489;
         else if (x<5) return 1.00625;
         else if (x<5.5) return 1.00858;
         else if (x<6) return 1.00396;
         else if (x<7) return 0.999445;
         else if (x<8) return 0.993827;
         else if (x<10.5) return 0.99905;
         else if (x<14) return 0.994489;
         else if (x<18) return 0.997698;
         else if (x<30) return 0.995153;
      } else if (fabs(eta)<1.8) {
         // 1.2 < |eta| < 1.8
         if (x<3) return 1.20421;
         else if (x<3.5) return 1.08567;
         else if (x<4) return 1.05784;
         else if (x<4.5) return 1.02947;
         else if (x<5) return 1.02139;
         else if (x<6) return 1.0185;
         else if (x<7) return 1.0172;
         else if (x<9) return 1.00903;
         else if (x<14) return 1.00746;
         else if (x<18) return 1.02468;
         else if (x<30) return 1.0251;
      } else if (fabs(eta)<2.1) {
         // 1.8 < |eta| < 2.1
         if (x<2.5) return 1.23547;
         else if (x<3) return 1.07077;
         else if (x<3.5) return 1.05213;
         else if (x<4) return 1.0408;
         else if (x<4.5) return 1.04117;
         else if (x<5) return 1.03069;
         else if (x<6) return 1.02427;
         else if (x<7) return 1.02459;
         else if (x<9) return 1.02441;
         else if (x<12) return 0.990256;
         else if (x<16) return 0.984322;
         else if (x<30) return 0.92996;
      } else {
         // 2.1 < |eta| < 2.4
         if (x<2.5) return 1.21176;
         else if (x<3) return 1.10765;
         else if (x<3.5) return 1.07239;
         else if (x<4) return 1.03555;
         else if (x<4.5) return 1.0393;
         else if (x<5) return 1.02399;
         else if (x<6.5) return 1.02925;
         else if (x<8.5) return 1.02507;
         else if (x<11) return 1.03665;
         else if (x<14) return 1.00561;
         else if (x<30) return 0.972387;
      }
   }

   // denominator (from MC)
   double den=1;
   if (fabs(eta)<1.2) den = 9.48433*(TMath::Erf((x-4.98004)/2.29550)*TMath::Exp(-0.99183*x))+0.97295;
   else if (fabs(eta)<1.8) den = 1.39020*(TMath::Erf((x-1.90863)/1.42330)*TMath::Exp(-0.00013*x))-0.44676;
   else if (fabs(eta)<2.1) den = 7.66100*(TMath::Erf((x-4.26619)/13.41197)*TMath::Exp(-0.57522*x))+0.87257;
   else den = 7.71316*(TMath::Erf((x-4.42009)/19.31524)*TMath::Exp(-0.68050*x))+0.86940;

   // numerator (from data)
   double num=1;
   if (fabs(eta)<1.2)
   {
      if (idx==0) num = 9.35664*(TMath::Erf((x-4.68281)/2.24520)*TMath::Exp(-1.03376*x))+0.97082;
      else if (idx == -1   ) num =   9.41877*(TMath::Erf((x-4.67913)/2.24273)*TMath::Exp(-1.03374*x))+0.97103;
      else if (idx == -2   ) num =   9.40074*(TMath::Erf((x-4.68381)/2.24921)*TMath::Exp(-1.03457*x))+0.97050;
      else if (idx == 1   ) num = 8.26889*(TMath::Erf((x-4.81463)/2.35557)*TMath::Exp(-1.03954*x))+0.97387;
      else if (idx == 2   ) num = 9.47902*(TMath::Erf((x-4.50259)/3.14210)*TMath::Exp(-0.91297*x))+0.96442;
      else if (idx == 3   ) num = 9.40987*(TMath::Erf((x-4.70459)/2.21219)*TMath::Exp(-1.03080*x))+0.97012;
      else if (idx == 4   ) num = 9.48744*(TMath::Erf((x-4.65609)/1.53309)*TMath::Exp(-1.11907*x))+0.97093;
      else if (idx == 5   ) num = 9.49800*(TMath::Erf((x-4.61762)/1.79685)*TMath::Exp(-1.07753*x))+0.96874;
      else if (idx == 6   ) num = 9.46721*(TMath::Erf((x-4.73935)/2.27137)*TMath::Exp(-1.03594*x))+0.97090;
      else if (idx == 7   ) num = 9.49184*(TMath::Erf((x-4.59428)/2.17796)*TMath::Exp(-1.02538*x))+0.96732;
      else if (idx == 8   ) num = 9.49953*(TMath::Erf((x-4.63210)/2.16206)*TMath::Exp(-1.03037*x))+0.97135;
      else if (idx == 9   ) num = 9.45027*(TMath::Erf((x-4.72408)/2.24819)*TMath::Exp(-1.02839*x))+0.97224;
      else if (idx == 10  ) num = 9.48048*(TMath::Erf((x-4.70542)/1.98208)*TMath::Exp(-1.07703*x))+0.96947;
      else if (idx == 11  ) num = 9.49113*(TMath::Erf((x-4.78330)/1.65180)*TMath::Exp(-1.12468*x))+0.97327;
      else if (idx == 12  ) num = 9.38803*(TMath::Erf((x-4.67300)/2.25105)*TMath::Exp(-1.03384*x))+0.97176;
      else if (idx == 13  ) num = 9.49853*(TMath::Erf((x-4.73838)/1.91386)*TMath::Exp(-1.08516*x))+0.97245;
      else if (idx == 14  ) num = 9.48811*(TMath::Erf((x-4.69461)/2.72848)*TMath::Exp(-0.99686*x))+0.97029;
      else if (idx == 15  ) num = 9.49707*(TMath::Erf((x-4.63198)/1.79226)*TMath::Exp(-1.07521*x))+0.97080;
      else if (idx == 16  ) num = 9.49978*(TMath::Erf((x-4.49534)/2.06481)*TMath::Exp(-1.01208*x))+0.96593;
      else if (idx == 17  ) num = 9.46608*(TMath::Erf((x-4.67867)/2.27406)*TMath::Exp(-1.03890*x))+0.97003;
      else if (idx == 18  ) num = 9.49398*(TMath::Erf((x-4.71679)/1.60696)*TMath::Exp(-1.10003*x))+0.97207;
      else if (idx == 19  ) num = 9.48406*(TMath::Erf((x-4.66314)/1.70947)*TMath::Exp(-1.10153*x))+0.97142;
      else if (idx == 20  ) num = 9.48278*(TMath::Erf((x-4.68293)/3.18479)*TMath::Exp(-0.95983*x))+0.97014;
      else if (idx == 21  ) num = 9.46140*(TMath::Erf((x-4.72129)/2.25797)*TMath::Exp(-1.03144*x))+0.97259;
      else if (idx == 22  ) num = 9.25812*(TMath::Erf((x-4.57552)/3.61342)*TMath::Exp(-0.90402*x))+0.96492;
      else if (idx == 23  ) num = 9.42490*(TMath::Erf((x-4.67552)/2.23138)*TMath::Exp(-1.03265*x))+0.97122;
      else if (idx == 24  ) num = 9.43115*(TMath::Erf((x-4.83613)/2.33996)*TMath::Exp(-1.05084*x))+0.97322;
      else if (idx == 25  ) num = 9.49635*(TMath::Erf((x-4.54908)/2.12301)*TMath::Exp(-1.01864*x))+0.96779;
      else if (idx == 26  ) num = 9.49618*(TMath::Erf((x-4.63475)/2.14973)*TMath::Exp(-1.02159*x))+0.96963;
      else if (idx == 27  ) num = 9.49979*(TMath::Erf((x-4.70557)/1.73117)*TMath::Exp(-1.09391*x))+0.97249;
      else if (idx == 28  ) num = 7.59348*(TMath::Erf((x-4.71967)/5.13832)*TMath::Exp(-0.80107*x))+0.96837;
      else if (idx == 29  ) num = 9.49720*(TMath::Erf((x-4.63858)/3.20362)*TMath::Exp(-0.94441*x))+0.96747;
      else if (idx == 30  ) num = 9.39854*(TMath::Erf((x-4.79661)/2.35908)*TMath::Exp(-1.04324*x))+0.97313;
      else if (idx == 31  ) num = 9.48275*(TMath::Erf((x-4.91384)/2.49525)*TMath::Exp(-1.06392*x))+0.97348;
      else if (idx == 32  ) num = 9.49242*(TMath::Erf((x-4.81307)/1.90797)*TMath::Exp(-1.09300*x))+0.97317;
      else if (idx == 33  ) num = 9.49247*(TMath::Erf((x-4.60646)/1.83904)*TMath::Exp(-1.06824*x))+0.97038;
      else if (idx == 34  ) num = 9.49705*(TMath::Erf((x-4.71735)/3.00160)*TMath::Exp(-0.97291*x))+0.97218;
      else if (idx == 35  ) num = 9.49832*(TMath::Erf((x-4.62222)/1.76368)*TMath::Exp(-1.08303*x))+0.97019;
      else if (idx == 36  ) num = 9.14193*(TMath::Erf((x-4.67424)/2.24439)*TMath::Exp(-1.03262*x))+0.97173;
      else if (idx == 37  ) num = 9.48932*(TMath::Erf((x-4.64641)/1.27093)*TMath::Exp(-1.14597*x))+0.97226;
      else if (idx == 38  ) num = 9.49668*(TMath::Erf((x-4.64950)/2.22962)*TMath::Exp(-1.03011*x))+0.97092;
      else if (idx == 39  ) num = 9.41508*(TMath::Erf((x-4.84837)/1.92880)*TMath::Exp(-1.11237*x))+0.97361;
      else if (idx == 40  ) num = 9.35145*(TMath::Erf((x-4.57677)/3.30083)*TMath::Exp(-0.91717*x))+0.96607;
      else if (idx == 41  ) num = 9.49841*(TMath::Erf((x-4.56766)/2.15783)*TMath::Exp(-1.02784*x))+0.96851;
      else if (idx == 42  ) num = 9.46921*(TMath::Erf((x-4.74570)/2.25827)*TMath::Exp(-1.03663*x))+0.97264;
      else if (idx == 43  ) num = 9.48912*(TMath::Erf((x-4.55453)/2.11600)*TMath::Exp(-1.02107*x))+0.96894;
      else if (idx == 44  ) num = 9.45922*(TMath::Erf((x-4.73705)/2.27904)*TMath::Exp(-1.03242*x))+0.97101;
      else if (idx == 45  ) num = 9.49821*(TMath::Erf((x-4.62537)/2.17859)*TMath::Exp(-1.03083*x))+0.97018;
      else if (idx == 46  ) num = 9.49875*(TMath::Erf((x-4.61054)/2.24513)*TMath::Exp(-1.03030*x))+0.96900;
      else if (idx == 47  ) num = 9.49905*(TMath::Erf((x-4.76333)/2.32249)*TMath::Exp(-1.04638*x))+0.97308;
      else if (idx == 48  ) num = 9.50000*(TMath::Erf((x-4.75982)/1.58180)*TMath::Exp(-1.11490*x))+0.97484;
      else if (idx == 49  ) num = 9.48742*(TMath::Erf((x-4.89548)/1.83636)*TMath::Exp(-1.13026*x))+0.97487;
      else if (idx == 50  ) num = 9.49575*(TMath::Erf((x-4.65606)/2.19387)*TMath::Exp(-1.02887*x))+0.97030;
      else if (idx == 51  ) num = 9.49558*(TMath::Erf((x-4.67525)/1.88985)*TMath::Exp(-1.07950*x))+0.97074;
      else if (idx == 52  ) num = 9.49678*(TMath::Erf((x-4.68229)/1.70602)*TMath::Exp(-1.08182*x))+0.97163;
      else if (idx == 53  ) num = 9.42904*(TMath::Erf((x-4.67411)/2.24075)*TMath::Exp(-1.03393*x))+0.97113;
      else if (idx == 54  ) num = 9.49547*(TMath::Erf((x-4.64687)/2.17683)*TMath::Exp(-1.03269*x))+0.96977;
      else if (idx == 55  ) num = 9.47858*(TMath::Erf((x-4.64369)/2.19859)*TMath::Exp(-1.02746*x))+0.96982;
      else if (idx == 56  ) num = 9.07901*(TMath::Erf((x-4.75915)/2.34889)*TMath::Exp(-1.03895*x))+0.97188;
      else if (idx == 57  ) num = 9.45708*(TMath::Erf((x-4.74692)/2.35357)*TMath::Exp(-1.04708*x))+0.97034;
      else if (idx == 58  ) num = 9.49888*(TMath::Erf((x-4.63924)/2.97021)*TMath::Exp(-0.95924*x))+0.96925;
      else if (idx == 59  ) num = 9.42588*(TMath::Erf((x-4.68951)/2.19391)*TMath::Exp(-1.02857*x))+0.97115;
      else if (idx == 60  ) num = 9.33889*(TMath::Erf((x-4.72470)/2.33144)*TMath::Exp(-1.04412*x))+0.97229;
      else if (idx == 61  ) num = 9.48075*(TMath::Erf((x-4.74381)/2.25962)*TMath::Exp(-1.03574*x))+0.97161;
      else if (idx == 62  ) num = 9.47793*(TMath::Erf((x-4.77093)/2.23905)*TMath::Exp(-1.04155*x))+0.97311;
      else if (idx == 63  ) num = 9.47799*(TMath::Erf((x-4.86152)/3.17647)*TMath::Exp(-0.99381*x))+0.97295;
      else if (idx == 64  ) num = 9.38213*(TMath::Erf((x-4.69330)/2.25278)*TMath::Exp(-1.03587*x))+0.97020;
      else if (idx == 65  ) num = 9.04267*(TMath::Erf((x-4.76065)/3.32382)*TMath::Exp(-0.95482*x))+0.97086;
      else if (idx == 66  ) num = 9.48742*(TMath::Erf((x-4.71985)/1.72933)*TMath::Exp(-1.10592*x))+0.97175;
      else if (idx == 67  ) num = 9.49973*(TMath::Erf((x-4.63535)/1.77186)*TMath::Exp(-1.07494*x))+0.97066;
      else if (idx == 68  ) num = 9.47287*(TMath::Erf((x-4.69948)/1.51502)*TMath::Exp(-1.10616*x))+0.97216;
      else if (idx == 69  ) num = 9.40173*(TMath::Erf((x-4.68525)/2.25336)*TMath::Exp(-1.03489*x))+0.97038;
      else if (idx == 70  ) num = 9.40115*(TMath::Erf((x-4.69988)/2.24073)*TMath::Exp(-1.03315*x))+0.97036;
      else if (idx == 71  ) num = 7.76722*(TMath::Erf((x-4.74509)/4.53374)*TMath::Exp(-0.83566*x))+0.96870;
      else if (idx == 72  ) num = 9.49554*(TMath::Erf((x-4.65958)/2.64961)*TMath::Exp(-0.99592*x))+0.96820;
      else if (idx == 73  ) num = 8.95797*(TMath::Erf((x-4.69024)/2.26146)*TMath::Exp(-1.03570*x))+0.97047;
      else if (idx == 74  ) num = 9.49928*(TMath::Erf((x-4.65888)/1.63802)*TMath::Exp(-1.10992*x))+0.97073;
      else if (idx == 75  ) num = 9.35917*(TMath::Erf((x-4.68782)/2.25502)*TMath::Exp(-1.03514*x))+0.97052;
      else if (idx == 76  ) num = 9.34461*(TMath::Erf((x-4.80559)/2.34632)*TMath::Exp(-1.04502*x))+0.97412;
      else if (idx == 77  ) num = 9.48508*(TMath::Erf((x-4.70278)/1.54774)*TMath::Exp(-1.12625*x))+0.97238;
      else if (idx == 78  ) num = 9.48579*(TMath::Erf((x-4.80731)/1.73114)*TMath::Exp(-1.11299*x))+0.97416;
      else if (idx == 79  ) num = 9.48363*(TMath::Erf((x-4.73332)/1.87048)*TMath::Exp(-1.10414*x))+0.97115;
      else if (idx == 80  ) num = 9.37409*(TMath::Erf((x-4.68477)/2.24258)*TMath::Exp(-1.03336*x))+0.97077;
      else if (idx == 81  ) num = 9.49009*(TMath::Erf((x-4.67362)/2.31974)*TMath::Exp(-1.03786*x))+0.97039;
      else if (idx == 82  ) num = 9.49232*(TMath::Erf((x-4.62816)/2.17554)*TMath::Exp(-1.02616*x))+0.96921;
      else if (idx == 83  ) num = 9.39153*(TMath::Erf((x-4.68660)/2.23142)*TMath::Exp(-1.03218*x))+0.97079;
      else if (idx == 84  ) num = 9.49545*(TMath::Erf((x-4.73020)/1.91278)*TMath::Exp(-1.07943*x))+0.97134;
      else if (idx == 85  ) num = 9.47770*(TMath::Erf((x-4.74630)/2.31442)*TMath::Exp(-1.04339*x))+0.97098;
      else if (idx == 86  ) num = 9.47642*(TMath::Erf((x-4.74980)/2.28514)*TMath::Exp(-1.03698*x))+0.97303;
      else if (idx == 87  ) num = 9.49977*(TMath::Erf((x-4.70788)/2.28785)*TMath::Exp(-1.04272*x))+0.97220;
      else if (idx == 88  ) num = 9.29504*(TMath::Erf((x-4.86600)/2.40170)*TMath::Exp(-1.05114*x))+0.97625;
      else if (idx == 89  ) num = 6.83830*(TMath::Erf((x-4.61074)/4.72403)*TMath::Exp(-0.77158*x))+0.96427;
      else if (idx == 90  ) num = 9.49943*(TMath::Erf((x-4.66104)/2.29172)*TMath::Exp(-1.03674*x))+0.97050;
      else if (idx == 91  ) num = 9.48821*(TMath::Erf((x-4.71137)/2.32988)*TMath::Exp(-1.03878*x))+0.97214;
      else if (idx == 92  ) num = 9.48607*(TMath::Erf((x-4.59027)/1.68832)*TMath::Exp(-1.08438*x))+0.96791;
      else if (idx == 93  ) num = 9.48456*(TMath::Erf((x-4.63852)/3.52222)*TMath::Exp(-0.93260*x))+0.96719;
      else if (idx == 94  ) num = 9.46652*(TMath::Erf((x-4.67609)/3.00548)*TMath::Exp(-0.97376*x))+0.97022;
      else if (idx == 95  ) num = 9.44895*(TMath::Erf((x-4.67320)/2.22704)*TMath::Exp(-1.02996*x))+0.97207;
      else if (idx == 96  ) num = 9.19651*(TMath::Erf((x-4.64772)/2.27420)*TMath::Exp(-1.03080*x))+0.96975;
      else if (idx == 97  ) num = 9.42914*(TMath::Erf((x-4.69527)/1.32988)*TMath::Exp(-1.14386*x))+0.97288;
      else if (idx == 98  ) num = 9.48955*(TMath::Erf((x-4.73191)/2.27044)*TMath::Exp(-1.04114*x))+0.97267;
      else if (idx == 99  ) num = 9.23348*(TMath::Erf((x-4.53733)/5.21635)*TMath::Exp(-0.81939*x))+0.96296;
      else if (idx == 100 ) num = 9.49978*(TMath::Erf((x-4.65434)/1.28258)*TMath::Exp(-1.13972*x))+0.97239;
   }
   else if (fabs(eta)<1.8)
   {
      if (idx==0) num = 1.39725*(TMath::Erf((x-1.86831)/1.34219)*TMath::Exp(-0.00000*x))-0.43928;
      else if (idx == -1   ) num =   1.39778*(TMath::Erf((x-1.86930)/1.34251)*TMath::Exp(-0.00000*x))-0.43884;
      else if (idx == -2   ) num =   1.39662*(TMath::Erf((x-1.86783)/1.33998)*TMath::Exp(-0.00000*x))-0.43987;
      else if (idx == 1   ) num = 1.39880*(TMath::Erf((x-1.83311)/1.40168)*TMath::Exp(-0.00052*x))-0.43588;
      else if (idx == 2   ) num = 1.39742*(TMath::Erf((x-1.88398)/1.31977)*TMath::Exp(-0.00000*x))-0.44097;
      else if (idx == 3   ) num = 1.39729*(TMath::Erf((x-1.86650)/1.34022)*TMath::Exp(-0.00000*x))-0.43920;
      else if (idx == 4   ) num = 1.39769*(TMath::Erf((x-1.87294)/1.34747)*TMath::Exp(-0.00000*x))-0.43901;
      else if (idx == 5   ) num = 1.39870*(TMath::Erf((x-1.86782)/1.33444)*TMath::Exp(-0.00045*x))-0.43805;
      else if (idx == 6   ) num = 1.39843*(TMath::Erf((x-1.91279)/1.29133)*TMath::Exp(-0.00000*x))-0.44097;
      else if (idx == 7   ) num = 1.39719*(TMath::Erf((x-1.87148)/1.34551)*TMath::Exp(-0.00000*x))-0.43944;
      else if (idx == 8   ) num = 1.39699*(TMath::Erf((x-1.89175)/1.31672)*TMath::Exp(-0.00000*x))-0.44100;
      else if (idx == 9   ) num = 1.39817*(TMath::Erf((x-1.87150)/1.34497)*TMath::Exp(-0.00000*x))-0.43855;
      else if (idx == 10  ) num = 1.39599*(TMath::Erf((x-1.85926)/1.33639)*TMath::Exp(-0.00017*x))-0.44015;
      else if (idx == 11  ) num = 1.39692*(TMath::Erf((x-1.92418)/1.26125)*TMath::Exp(-0.00005*x))-0.44301;
      else if (idx == 12  ) num = 1.39669*(TMath::Erf((x-1.85327)/1.35830)*TMath::Exp(-0.00000*x))-0.43904;
      else if (idx == 13  ) num = 1.39706*(TMath::Erf((x-1.86503)/1.33859)*TMath::Exp(-0.00000*x))-0.43936;
      else if (idx == 14  ) num = 1.39707*(TMath::Erf((x-1.87140)/1.34644)*TMath::Exp(-0.00000*x))-0.43953;
      else if (idx == 15  ) num = 1.39852*(TMath::Erf((x-1.87198)/1.34196)*TMath::Exp(-0.00014*x))-0.43830;
      else if (idx == 16  ) num = 1.40043*(TMath::Erf((x-1.87408)/1.33900)*TMath::Exp(-0.00084*x))-0.43675;
      else if (idx == 17  ) num = 1.39561*(TMath::Erf((x-1.86256)/1.33947)*TMath::Exp(-0.00000*x))-0.44061;
      else if (idx == 18  ) num = 1.39820*(TMath::Erf((x-1.87346)/1.34178)*TMath::Exp(-0.00025*x))-0.43864;
      else if (idx == 19  ) num = 1.39852*(TMath::Erf((x-1.83863)/1.37791)*TMath::Exp(-0.00000*x))-0.43683;
      else if (idx == 20  ) num = 1.39825*(TMath::Erf((x-1.88177)/1.32548)*TMath::Exp(-0.00016*x))-0.44196;
      else if (idx == 21  ) num = 1.39792*(TMath::Erf((x-1.90232)/1.30150)*TMath::Exp(-0.00000*x))-0.44087;
      else if (idx == 22  ) num = 1.39718*(TMath::Erf((x-1.86442)/1.33767)*TMath::Exp(-0.00000*x))-0.43924;
      else if (idx == 23  ) num = 1.39835*(TMath::Erf((x-1.92256)/1.27159)*TMath::Exp(-0.00025*x))-0.44164;
      else if (idx == 24  ) num = 1.39992*(TMath::Erf((x-1.91520)/1.29708)*TMath::Exp(-0.00000*x))-0.43975;
      else if (idx == 25  ) num = 1.39895*(TMath::Erf((x-1.85290)/1.37170)*TMath::Exp(-0.00000*x))-0.43904;
      else if (idx == 26  ) num = 1.39954*(TMath::Erf((x-1.87667)/1.35013)*TMath::Exp(-0.00000*x))-0.43745;
      else if (idx == 27  ) num = 1.40074*(TMath::Erf((x-1.88716)/1.35162)*TMath::Exp(-0.00049*x))-0.43683;
      else if (idx == 28  ) num = 1.40246*(TMath::Erf((x-1.85702)/1.37351)*TMath::Exp(-0.00102*x))-0.43399;
      else if (idx == 29  ) num = 1.39873*(TMath::Erf((x-1.92197)/1.28122)*TMath::Exp(-0.00009*x))-0.44113;
      else if (idx == 30  ) num = 1.39936*(TMath::Erf((x-1.83300)/1.38325)*TMath::Exp(-0.00000*x))-0.43580;
      else if (idx == 31  ) num = 1.39826*(TMath::Erf((x-1.91435)/1.28473)*TMath::Exp(-0.00000*x))-0.44124;
      else if (idx == 32  ) num = 1.39889*(TMath::Erf((x-1.84959)/1.37156)*TMath::Exp(-0.00000*x))-0.43698;
      else if (idx == 33  ) num = 1.39796*(TMath::Erf((x-1.86808)/1.33200)*TMath::Exp(-0.00022*x))-0.43877;
      else if (idx == 34  ) num = 1.39815*(TMath::Erf((x-1.91580)/1.28853)*TMath::Exp(-0.00000*x))-0.44128;
      else if (idx == 35  ) num = 1.39982*(TMath::Erf((x-1.86163)/1.36631)*TMath::Exp(-0.00000*x))-0.43918;
      else if (idx == 36  ) num = 1.39736*(TMath::Erf((x-1.89063)/1.31368)*TMath::Exp(-0.00004*x))-0.44064;
      else if (idx == 37  ) num = 1.39996*(TMath::Erf((x-1.89539)/1.31166)*TMath::Exp(-0.00034*x))-0.43846;
      else if (idx == 38  ) num = 1.39843*(TMath::Erf((x-1.89395)/1.30428)*TMath::Exp(-0.00000*x))-0.44009;
      else if (idx == 39  ) num = 1.39891*(TMath::Erf((x-1.88682)/1.32520)*TMath::Exp(-0.00000*x))-0.44040;
      else if (idx == 40  ) num = 1.39985*(TMath::Erf((x-1.89674)/1.31361)*TMath::Exp(-0.00017*x))-0.43857;
      else if (idx == 41  ) num = 1.39764*(TMath::Erf((x-1.92705)/1.28929)*TMath::Exp(-0.00004*x))-0.44219;
      else if (idx == 42  ) num = 1.39908*(TMath::Erf((x-1.88481)/1.33030)*TMath::Exp(-0.00000*x))-0.43930;
      else if (idx == 43  ) num = 1.39875*(TMath::Erf((x-1.87103)/1.34200)*TMath::Exp(-0.00026*x))-0.43807;
      else if (idx == 44  ) num = 1.39761*(TMath::Erf((x-1.83559)/1.39665)*TMath::Exp(-0.00000*x))-0.43722;
      else if (idx == 45  ) num = 1.39622*(TMath::Erf((x-1.82709)/1.38393)*TMath::Exp(-0.00000*x))-0.43828;
      else if (idx == 46  ) num = 1.39824*(TMath::Erf((x-1.90641)/1.29484)*TMath::Exp(-0.00000*x))-0.44065;
      else if (idx == 47  ) num = 1.39701*(TMath::Erf((x-1.82627)/1.39587)*TMath::Exp(-0.00000*x))-0.43735;
      else if (idx == 48  ) num = 1.39616*(TMath::Erf((x-1.86146)/1.33595)*TMath::Exp(-0.00001*x))-0.44007;
      else if (idx == 49  ) num = 1.39768*(TMath::Erf((x-1.88428)/1.31897)*TMath::Exp(-0.00000*x))-0.44002;
      else if (idx == 50  ) num = 1.39797*(TMath::Erf((x-1.87086)/1.34559)*TMath::Exp(-0.00000*x))-0.43869;
      else if (idx == 51  ) num = 1.39999*(TMath::Erf((x-1.96809)/1.22183)*TMath::Exp(-0.00000*x))-0.44271;
      else if (idx == 52  ) num = 1.39790*(TMath::Erf((x-1.87834)/1.32044)*TMath::Exp(-0.00000*x))-0.44190;
      else if (idx == 53  ) num = 1.39811*(TMath::Erf((x-1.87189)/1.34699)*TMath::Exp(-0.00000*x))-0.43859;
      else if (idx == 54  ) num = 1.39729*(TMath::Erf((x-1.85214)/1.35639)*TMath::Exp(-0.00000*x))-0.43932;
      else if (idx == 55  ) num = 1.39780*(TMath::Erf((x-1.86143)/1.33420)*TMath::Exp(-0.00000*x))-0.43857;
      else if (idx == 56  ) num = 1.40033*(TMath::Erf((x-1.88933)/1.33264)*TMath::Exp(-0.00000*x))-0.44014;
      else if (idx == 57  ) num = 1.39831*(TMath::Erf((x-1.89270)/1.31747)*TMath::Exp(-0.00000*x))-0.43980;
      else if (idx == 58  ) num = 1.39743*(TMath::Erf((x-1.87317)/1.34872)*TMath::Exp(-0.00000*x))-0.43926;
      else if (idx == 59  ) num = 1.39886*(TMath::Erf((x-1.93703)/1.26677)*TMath::Exp(-0.00019*x))-0.44186;
      else if (idx == 60  ) num = 1.39731*(TMath::Erf((x-1.86686)/1.34030)*TMath::Exp(-0.00000*x))-0.43919;
      else if (idx == 61  ) num = 1.39741*(TMath::Erf((x-1.87852)/1.30470)*TMath::Exp(-0.00018*x))-0.44071;
      else if (idx == 62  ) num = 1.39905*(TMath::Erf((x-1.87107)/1.34510)*TMath::Exp(-0.00000*x))-0.43773;
      else if (idx == 63  ) num = 1.39530*(TMath::Erf((x-1.81690)/1.39551)*TMath::Exp(-0.00000*x))-0.43855;
      else if (idx == 64  ) num = 1.39856*(TMath::Erf((x-1.90945)/1.28438)*TMath::Exp(-0.00000*x))-0.44089;
      else if (idx == 65  ) num = 1.39854*(TMath::Erf((x-1.89788)/1.29373)*TMath::Exp(-0.00000*x))-0.44017;
      else if (idx == 66  ) num = 1.39910*(TMath::Erf((x-1.83959)/1.39008)*TMath::Exp(-0.00000*x))-0.43601;
      else if (idx == 67  ) num = 1.39712*(TMath::Erf((x-1.89129)/1.30667)*TMath::Exp(-0.00000*x))-0.44096;
      else if (idx == 68  ) num = 1.39900*(TMath::Erf((x-1.87743)/1.35221)*TMath::Exp(-0.00000*x))-0.43796;
      else if (idx == 69  ) num = 1.39625*(TMath::Erf((x-1.81212)/1.40310)*TMath::Exp(-0.00000*x))-0.43731;
      else if (idx == 70  ) num = 1.39842*(TMath::Erf((x-1.87552)/1.34976)*TMath::Exp(-0.00000*x))-0.43843;
      else if (idx == 71  ) num = 1.39707*(TMath::Erf((x-1.87148)/1.34600)*TMath::Exp(-0.00000*x))-0.43953;
      else if (idx == 72  ) num = 1.40053*(TMath::Erf((x-1.85794)/1.37030)*TMath::Exp(-0.00000*x))-0.43831;
      else if (idx == 73  ) num = 1.39875*(TMath::Erf((x-1.86416)/1.33726)*TMath::Exp(-0.00028*x))-0.43780;
      else if (idx == 74  ) num = 1.39762*(TMath::Erf((x-1.88301)/1.33062)*TMath::Exp(-0.00000*x))-0.43996;
      else if (idx == 75  ) num = 1.39919*(TMath::Erf((x-1.88702)/1.32368)*TMath::Exp(-0.00015*x))-0.43921;
      else if (idx == 76  ) num = 1.39748*(TMath::Erf((x-1.89232)/1.31050)*TMath::Exp(-0.00000*x))-0.44071;
      else if (idx == 77  ) num = 1.39756*(TMath::Erf((x-1.83955)/1.38193)*TMath::Exp(-0.00000*x))-0.43745;
      else if (idx == 78  ) num = 1.40077*(TMath::Erf((x-1.91176)/1.29918)*TMath::Exp(-0.00046*x))-0.43869;
      else if (idx == 79  ) num = 1.39756*(TMath::Erf((x-1.87534)/1.31970)*TMath::Exp(-0.00000*x))-0.44072;
      else if (idx == 80  ) num = 1.40110*(TMath::Erf((x-1.90631)/1.30695)*TMath::Exp(-0.00000*x))-0.43823;
      else if (idx == 81  ) num = 1.39794*(TMath::Erf((x-1.90804)/1.29338)*TMath::Exp(-0.00000*x))-0.44122;
      else if (idx == 82  ) num = 1.39659*(TMath::Erf((x-1.88304)/1.31824)*TMath::Exp(-0.00002*x))-0.44112;
      else if (idx == 83  ) num = 1.39890*(TMath::Erf((x-1.87719)/1.35173)*TMath::Exp(-0.00000*x))-0.43804;
      else if (idx == 84  ) num = 1.39870*(TMath::Erf((x-1.90997)/1.29456)*TMath::Exp(-0.00000*x))-0.44049;
      else if (idx == 85  ) num = 1.39704*(TMath::Erf((x-1.86868)/1.34268)*TMath::Exp(-0.00000*x))-0.43949;
      else if (idx == 86  ) num = 1.39648*(TMath::Erf((x-1.85286)/1.35657)*TMath::Exp(-0.00000*x))-0.43947;
      else if (idx == 87  ) num = 1.40038*(TMath::Erf((x-1.82898)/1.39889)*TMath::Exp(-0.00095*x))-0.43442;
      else if (idx == 88  ) num = 1.39758*(TMath::Erf((x-1.88464)/1.31456)*TMath::Exp(-0.00000*x))-0.44013;
      else if (idx == 89  ) num = 1.39600*(TMath::Erf((x-1.88538)/1.31765)*TMath::Exp(-0.00000*x))-0.44159;
      else if (idx == 90  ) num = 1.39887*(TMath::Erf((x-1.86895)/1.34220)*TMath::Exp(-0.00000*x))-0.43784;
      else if (idx == 91  ) num = 1.40082*(TMath::Erf((x-1.87502)/1.34442)*TMath::Exp(-0.00086*x))-0.43634;
      else if (idx == 92  ) num = 1.39778*(TMath::Erf((x-1.86898)/1.34345)*TMath::Exp(-0.00000*x))-0.43882;
      else if (idx == 93  ) num = 1.39817*(TMath::Erf((x-1.89441)/1.31620)*TMath::Exp(-0.00000*x))-0.43996;
      else if (idx == 94  ) num = 1.39799*(TMath::Erf((x-1.85336)/1.35722)*TMath::Exp(-0.00000*x))-0.43985;
      else if (idx == 95  ) num = 1.39806*(TMath::Erf((x-1.88800)/1.31294)*TMath::Exp(-0.00000*x))-0.43992;
      else if (idx == 96  ) num = 1.39852*(TMath::Erf((x-1.92800)/1.27805)*TMath::Exp(-0.00000*x))-0.44157;
      else if (idx == 97  ) num = 1.40189*(TMath::Erf((x-1.89321)/1.33500)*TMath::Exp(-0.00000*x))-0.43876;
      else if (idx == 98  ) num = 1.40139*(TMath::Erf((x-1.84653)/1.37785)*TMath::Exp(-0.00110*x))-0.43455;
      else if (idx == 99  ) num = 1.39964*(TMath::Erf((x-1.90630)/1.30224)*TMath::Exp(-0.00000*x))-0.43950;
      else if (idx == 100 ) num = 1.39841*(TMath::Erf((x-1.89176)/1.32208)*TMath::Exp(-0.00000*x))-0.43970;
   }
   else if (fabs(eta)<2.1)
   {
      if (idx==0) num = 9.14386*(TMath::Erf((x-3.22375)/28.33917)*TMath::Exp(-0.33445*x))+0.80359;
      else if (idx == -1   ) num =   9.41574*(TMath::Erf((x-3.43162)/28.33819)*TMath::Exp(-0.36600*x))+0.83128;
      else if (idx == -2   ) num =   9.48460*(TMath::Erf((x-3.01751)/30.11716)*TMath::Exp(-0.30684*x))+0.77269;
      else if (idx == 1   ) num = 9.36718*(TMath::Erf((x-3.26053)/25.06239)*TMath::Exp(-0.37800*x))+0.81172;
      else if (idx == 2   ) num = 9.40281*(TMath::Erf((x-3.16632)/27.53197)*TMath::Exp(-0.34585*x))+0.79865;
      else if (idx == 3   ) num = 9.02226*(TMath::Erf((x-3.27350)/29.08147)*TMath::Exp(-0.32598*x))+0.80760;
      else if (idx == 4   ) num = 9.10748*(TMath::Erf((x-3.18064)/28.42763)*TMath::Exp(-0.33952*x))+0.80260;
      else if (idx == 5   ) num = 9.36396*(TMath::Erf((x-2.81812)/30.23423)*TMath::Exp(-0.29062*x))+0.74429;
      else if (idx == 6   ) num = 9.49565*(TMath::Erf((x-3.02383)/27.44712)*TMath::Exp(-0.33830*x))+0.78220;
      else if (idx == 7   ) num = 9.19458*(TMath::Erf((x-3.21081)/22.86517)*TMath::Exp(-0.39801*x))+0.81431;
      else if (idx == 8   ) num = 9.32429*(TMath::Erf((x-2.74377)/30.36648)*TMath::Exp(-0.27665*x))+0.73677;
      else if (idx == 9   ) num = 9.42038*(TMath::Erf((x-3.41376)/28.64772)*TMath::Exp(-0.36539*x))+0.82869;
      else if (idx == 10  ) num = 9.48981*(TMath::Erf((x-3.18594)/26.03207)*TMath::Exp(-0.35679*x))+0.80033;
      else if (idx == 11  ) num = 9.42117*(TMath::Erf((x-3.14911)/27.20810)*TMath::Exp(-0.34595*x))+0.79861;
      else if (idx == 12  ) num = 9.44183*(TMath::Erf((x-3.37304)/27.10067)*TMath::Exp(-0.36832*x))+0.82503;
      else if (idx == 13  ) num = 9.29073*(TMath::Erf((x-3.14229)/28.10905)*TMath::Exp(-0.33004*x))+0.79791;
      else if (idx == 14  ) num = 9.49260*(TMath::Erf((x-3.40839)/26.95500)*TMath::Exp(-0.37807*x))+0.82898;
      else if (idx == 15  ) num = 9.40926*(TMath::Erf((x-3.77709)/29.44050)*TMath::Exp(-0.39735*x))+0.85805;
      else if (idx == 16  ) num = 9.45973*(TMath::Erf((x-3.56910)/22.93281)*TMath::Exp(-0.43344*x))+0.85261;
      else if (idx == 17  ) num = 9.13171*(TMath::Erf((x-3.02760)/29.85326)*TMath::Exp(-0.29309*x))+0.77024;
      else if (idx == 18  ) num = 9.40090*(TMath::Erf((x-3.26739)/26.58871)*TMath::Exp(-0.35142*x))+0.81057;
      else if (idx == 19  ) num = 9.43681*(TMath::Erf((x-3.78436)/26.30091)*TMath::Exp(-0.43854*x))+0.86333;
      else if (idx == 20  ) num = 9.39485*(TMath::Erf((x-2.87492)/36.05098)*TMath::Exp(-0.25343*x))+0.74319;
      else if (idx == 21  ) num = 9.35714*(TMath::Erf((x-3.34796)/23.70115)*TMath::Exp(-0.39334*x))+0.82427;
      else if (idx == 22  ) num = 9.28988*(TMath::Erf((x-3.26001)/28.41914)*TMath::Exp(-0.34963*x))+0.80872;
      else if (idx == 23  ) num = 9.49616*(TMath::Erf((x-3.61151)/26.46329)*TMath::Exp(-0.39761*x))+0.85035;
      else if (idx == 24  ) num = 8.73516*(TMath::Erf((x-3.19853)/30.85972)*TMath::Exp(-0.30811*x))+0.80199;
      else if (idx == 25  ) num = 8.93466*(TMath::Erf((x-3.16359)/30.32379)*TMath::Exp(-0.31401*x))+0.79465;
      else if (idx == 26  ) num = 9.47652*(TMath::Erf((x-2.95553)/30.80145)*TMath::Exp(-0.30256*x))+0.76735;
      else if (idx == 27  ) num = 9.20737*(TMath::Erf((x-3.32010)/28.84618)*TMath::Exp(-0.33734*x))+0.81461;
      else if (idx == 28  ) num = 9.44749*(TMath::Erf((x-3.22246)/26.88648)*TMath::Exp(-0.34969*x))+0.80226;
      else if (idx == 29  ) num = 9.43329*(TMath::Erf((x-3.00338)/26.37556)*TMath::Exp(-0.33142*x))+0.77114;
      else if (idx == 30  ) num = 9.20112*(TMath::Erf((x-2.60884)/29.54767)*TMath::Exp(-0.26878*x))+0.70744;
      else if (idx == 31  ) num = 9.49614*(TMath::Erf((x-3.25004)/26.86745)*TMath::Exp(-0.35994*x))+0.80646;
      else if (idx == 32  ) num = 9.45433*(TMath::Erf((x-2.99045)/30.76114)*TMath::Exp(-0.30406*x))+0.77133;
      else if (idx == 33  ) num = 8.84606*(TMath::Erf((x-2.92391)/30.86065)*TMath::Exp(-0.28022*x))+0.76315;
      else if (idx == 34  ) num = 9.49986*(TMath::Erf((x-2.73704)/34.22162)*TMath::Exp(-0.25740*x))+0.73032;
      else if (idx == 35  ) num = 9.37126*(TMath::Erf((x-2.89226)/29.75153)*TMath::Exp(-0.29250*x))+0.75494;
      else if (idx == 36  ) num = 9.18704*(TMath::Erf((x-3.11683)/29.57554)*TMath::Exp(-0.30933*x))+0.78455;
      else if (idx == 37  ) num = 9.49115*(TMath::Erf((x-2.85804)/31.37716)*TMath::Exp(-0.28332*x))+0.75234;
      else if (idx == 38  ) num = 9.49397*(TMath::Erf((x-3.45785)/26.02793)*TMath::Exp(-0.40155*x))+0.83285;
      else if (idx == 39  ) num = 9.34725*(TMath::Erf((x-3.02848)/28.83873)*TMath::Exp(-0.31409*x))+0.77958;
      else if (idx == 40  ) num = 8.95329*(TMath::Erf((x-3.80165)/22.23097)*TMath::Exp(-0.46492*x))+0.86613;
      else if (idx == 41  ) num = 9.45021*(TMath::Erf((x-4.01657)/25.23998)*TMath::Exp(-0.47659*x))+0.88053;
      else if (idx == 42  ) num = 9.41589*(TMath::Erf((x-3.88139)/24.00295)*TMath::Exp(-0.47385*x))+0.87442;
      else if (idx == 43  ) num = 9.09787*(TMath::Erf((x-3.11417)/29.41854)*TMath::Exp(-0.31428*x))+0.78908;
      else if (idx == 44  ) num = 9.48727*(TMath::Erf((x-3.48964)/27.19948)*TMath::Exp(-0.38362*x))+0.83951;
      else if (idx == 45  ) num = 8.97547*(TMath::Erf((x-3.02367)/30.58496)*TMath::Exp(-0.29260*x))+0.77733;
      else if (idx == 46  ) num = 9.48685*(TMath::Erf((x-3.59614)/28.32399)*TMath::Exp(-0.38337*x))+0.84711;
      else if (idx == 47  ) num = 8.44764*(TMath::Erf((x-3.05390)/30.82100)*TMath::Exp(-0.28713*x))+0.77919;
      else if (idx == 48  ) num = 9.49882*(TMath::Erf((x-3.73626)/28.04272)*TMath::Exp(-0.40508*x))+0.85594;
      else if (idx == 49  ) num = 9.02045*(TMath::Erf((x-3.48684)/30.35758)*TMath::Exp(-0.34576*x))+0.83023;
      else if (idx == 50  ) num = 9.05986*(TMath::Erf((x-2.89842)/31.65276)*TMath::Exp(-0.28639*x))+0.75818;
      else if (idx == 51  ) num = 9.49988*(TMath::Erf((x-3.61110)/28.25784)*TMath::Exp(-0.39514*x))+0.84621;
      else if (idx == 52  ) num = 9.48611*(TMath::Erf((x-2.88420)/33.09562)*TMath::Exp(-0.27804*x))+0.75269;
      else if (idx == 53  ) num = 9.02346*(TMath::Erf((x-3.27768)/31.36757)*TMath::Exp(-0.31075*x))+0.80432;
      else if (idx == 54  ) num = 9.32337*(TMath::Erf((x-3.27621)/28.37878)*TMath::Exp(-0.34710*x))+0.81298;
      else if (idx == 55  ) num = 9.48308*(TMath::Erf((x-3.51708)/27.93829)*TMath::Exp(-0.37687*x))+0.83926;
      else if (idx == 56  ) num = 9.29089*(TMath::Erf((x-3.11044)/29.12374)*TMath::Exp(-0.31857*x))+0.78568;
      else if (idx == 57  ) num = 9.34751*(TMath::Erf((x-2.95652)/31.02137)*TMath::Exp(-0.28916*x))+0.76464;
      else if (idx == 58  ) num = 9.20642*(TMath::Erf((x-3.32677)/28.81496)*TMath::Exp(-0.33757*x))+0.81642;
      else if (idx == 59  ) num = 9.31283*(TMath::Erf((x-3.55952)/30.22770)*TMath::Exp(-0.35646*x))+0.84355;
      else if (idx == 60  ) num = 9.48076*(TMath::Erf((x-3.05689)/27.22920)*TMath::Exp(-0.34289*x))+0.78400;
      else if (idx == 61  ) num = 8.25963*(TMath::Erf((x-2.23384)/31.89446)*TMath::Exp(-0.22322*x))+0.65397;
      else if (idx == 62  ) num = 9.43237*(TMath::Erf((x-3.36970)/26.95483)*TMath::Exp(-0.37522*x))+0.82768;
      else if (idx == 63  ) num = 9.45708*(TMath::Erf((x-3.46692)/25.26293)*TMath::Exp(-0.41309*x))+0.83878;
      else if (idx == 64  ) num = 9.49911*(TMath::Erf((x-2.99727)/26.78912)*TMath::Exp(-0.33828*x))+0.77822;
      else if (idx == 65  ) num = 9.35314*(TMath::Erf((x-3.46174)/28.82988)*TMath::Exp(-0.36499*x))+0.83087;
      else if (idx == 66  ) num = 9.23065*(TMath::Erf((x-3.37645)/29.35017)*TMath::Exp(-0.34164*x))+0.82254;
      else if (idx == 67  ) num = 9.47128*(TMath::Erf((x-3.85611)/28.67424)*TMath::Exp(-0.41854*x))+0.86759;
      else if (idx == 68  ) num = 9.06852*(TMath::Erf((x-2.83888)/32.51830)*TMath::Exp(-0.26590*x))+0.74661;
      else if (idx == 69  ) num = 9.45908*(TMath::Erf((x-3.23718)/26.59610)*TMath::Exp(-0.35847*x))+0.81125;
      else if (idx == 70  ) num = 9.44333*(TMath::Erf((x-3.40970)/25.83914)*TMath::Exp(-0.37776*x))+0.83039;
      else if (idx == 71  ) num = 9.26457*(TMath::Erf((x-3.34733)/28.41248)*TMath::Exp(-0.35327*x))+0.82009;
      else if (idx == 72  ) num = 9.38646*(TMath::Erf((x-3.16861)/26.81533)*TMath::Exp(-0.34202*x))+0.79985;
      else if (idx == 73  ) num = 9.00928*(TMath::Erf((x-3.06664)/29.96444)*TMath::Exp(-0.30083*x))+0.77965;
      else if (idx == 74  ) num = 8.98317*(TMath::Erf((x-3.50501)/31.17963)*TMath::Exp(-0.33997*x))+0.83324;
      else if (idx == 75  ) num = 9.49994*(TMath::Erf((x-3.48412)/26.85154)*TMath::Exp(-0.38233*x))+0.83476;
      else if (idx == 76  ) num = 8.49835*(TMath::Erf((x-3.23206)/30.84036)*TMath::Exp(-0.30278*x))+0.79919;
      else if (idx == 77  ) num = 9.43682*(TMath::Erf((x-3.06091)/28.53988)*TMath::Exp(-0.31829*x))+0.78135;
      else if (idx == 78  ) num = 9.49997*(TMath::Erf((x-3.46785)/26.10872)*TMath::Exp(-0.39701*x))+0.83359;
      else if (idx == 79  ) num = 9.41747*(TMath::Erf((x-3.25534)/24.48340)*TMath::Exp(-0.38296*x))+0.81505;
      else if (idx == 80  ) num = 9.16910*(TMath::Erf((x-2.56404)/30.42459)*TMath::Exp(-0.26292*x))+0.70363;
      else if (idx == 81  ) num = 9.49400*(TMath::Erf((x-3.47320)/27.33384)*TMath::Exp(-0.38603*x))+0.83521;
      else if (idx == 82  ) num = 9.32440*(TMath::Erf((x-3.03642)/29.61903)*TMath::Exp(-0.30527*x))+0.77877;
      else if (idx == 83  ) num = 9.11403*(TMath::Erf((x-3.13898)/29.15527)*TMath::Exp(-0.31164*x))+0.79099;
      else if (idx == 84  ) num = 9.43927*(TMath::Erf((x-2.78657)/30.05444)*TMath::Exp(-0.29031*x))+0.73983;
      else if (idx == 85  ) num = 9.14192*(TMath::Erf((x-3.26923)/31.86809)*TMath::Exp(-0.30890*x))+0.80668;
      else if (idx == 86  ) num = 9.40642*(TMath::Erf((x-3.38872)/28.45821)*TMath::Exp(-0.35716*x))+0.82516;
      else if (idx == 87  ) num = 9.23227*(TMath::Erf((x-3.36274)/29.42242)*TMath::Exp(-0.34685*x))+0.82273;
      else if (idx == 88  ) num = 9.17897*(TMath::Erf((x-2.71762)/34.15720)*TMath::Exp(-0.25549*x))+0.72988;
      else if (idx == 89  ) num = 9.29748*(TMath::Erf((x-3.20998)/29.59044)*TMath::Exp(-0.32303*x))+0.80162;
      else if (idx == 90  ) num = 8.68578*(TMath::Erf((x-3.07819)/29.34459)*TMath::Exp(-0.30691*x))+0.78431;
      else if (idx == 91  ) num = 9.39910*(TMath::Erf((x-2.82955)/29.05495)*TMath::Exp(-0.30094*x))+0.74969;
      else if (idx == 92  ) num = 9.26603*(TMath::Erf((x-2.59977)/31.68656)*TMath::Exp(-0.25607*x))+0.71112;
      else if (idx == 93  ) num = 9.45173*(TMath::Erf((x-3.16505)/26.21037)*TMath::Exp(-0.34699*x))+0.79773;
      else if (idx == 94  ) num = 9.48938*(TMath::Erf((x-3.03523)/28.02506)*TMath::Exp(-0.32345*x))+0.77652;
      else if (idx == 95  ) num = 8.71025*(TMath::Erf((x-2.67087)/27.89291)*TMath::Exp(-0.27825*x))+0.72138;
      else if (idx == 96  ) num = 9.43208*(TMath::Erf((x-2.81997)/35.22060)*TMath::Exp(-0.26393*x))+0.74727;
      else if (idx == 97  ) num = 9.05173*(TMath::Erf((x-2.78534)/28.73578)*TMath::Exp(-0.27647*x))+0.73134;
      else if (idx == 98  ) num = 9.48026*(TMath::Erf((x-3.51194)/25.50675)*TMath::Exp(-0.40593*x))+0.84585;
      else if (idx == 99  ) num = 9.48390*(TMath::Erf((x-3.62371)/23.77826)*TMath::Exp(-0.43562*x))+0.85925;
      else if (idx == 100 ) num = 9.46595*(TMath::Erf((x-3.30562)/26.70727)*TMath::Exp(-0.36402*x))+0.81686;
   }
   else
   {
      if (idx==0) num = 4.34828*(TMath::Erf((x-2.41381)/36.34555)*TMath::Exp(-0.23268*x))+0.79944;
      else if (idx == -1   ) num =   4.34585*(TMath::Erf((x-2.41120)/36.38078)*TMath::Exp(-0.23270*x))+0.79952;
      else if (idx == -2   ) num =   4.34572*(TMath::Erf((x-2.41508)/36.36983)*TMath::Exp(-0.23272*x))+0.79939;
      else if (idx == 1   ) num = 3.15922*(TMath::Erf((x+5.76771)/32.44915)*TMath::Exp(-0.06322*x))+0.07074;
      else if (idx == 2   ) num = 4.21898*(TMath::Erf((x-1.69468)/39.16831)*TMath::Exp(-0.19825*x))+0.75696;
      else if (idx == 3   ) num = 5.58800*(TMath::Erf((x-2.13154)/37.86746)*TMath::Exp(-0.24570*x))+0.77166;
      else if (idx == 4   ) num = 4.73632*(TMath::Erf((x-2.08400)/34.80976)*TMath::Exp(-0.21939*x))+0.76121;
      else if (idx == 5   ) num = 8.16882*(TMath::Erf((x-4.10776)/22.93126)*TMath::Exp(-0.83639*x))+0.88847;
      else if (idx == 6   ) num = 5.08117*(TMath::Erf((x+0.42871)/41.10323)*TMath::Exp(-0.12874*x))+0.51247;
      else if (idx == 7   ) num = 4.55250*(TMath::Erf((x-2.77385)/36.59813)*TMath::Exp(-0.26855*x))+0.82602;
      else if (idx == 8   ) num = 4.48978*(TMath::Erf((x-1.64653)/37.67338)*TMath::Exp(-0.19498*x))+0.73535;
      else if (idx == 9   ) num = 4.34970*(TMath::Erf((x-1.71672)/45.80666)*TMath::Exp(-0.17576*x))+0.75385;
      else if (idx == 10  ) num = 4.99669*(TMath::Erf((x+5.23604)/36.88843)*TMath::Exp(-0.07090*x))-0.16833;
      else if (idx == 11  ) num = 4.86683*(TMath::Erf((x-1.77572)/33.50936)*TMath::Exp(-0.22672*x))+0.74440;
      else if (idx == 12  ) num = 4.20743*(TMath::Erf((x-1.65769)/38.63235)*TMath::Exp(-0.18965*x))+0.74308;
      else if (idx == 13  ) num = 2.08448*(TMath::Erf((x+8.65858)/20.85291)*TMath::Exp(-0.04063*x))-0.18770;
      else if (idx == 14  ) num = 6.01799*(TMath::Erf((x-3.96186)/36.51814)*TMath::Exp(-0.46209*x))+0.88840;
      else if (idx == 15  ) num = 4.50197*(TMath::Erf((x-2.50471)/34.89381)*TMath::Exp(-0.23020*x))+0.79722;
      else if (idx == 16  ) num = 4.24779*(TMath::Erf((x-0.39458)/43.01301)*TMath::Exp(-0.17425*x))+0.70073;
      else if (idx == 17  ) num = 3.94749*(TMath::Erf((x-0.10306)/40.50779)*TMath::Exp(-0.13046*x))+0.62436;
      else if (idx == 18  ) num = 4.61398*(TMath::Erf((x-2.01427)/37.08798)*TMath::Exp(-0.21354*x))+0.76654;
      else if (idx == 19  ) num = 4.40451*(TMath::Erf((x-2.38870)/35.83864)*TMath::Exp(-0.22778*x))+0.80206;
      else if (idx == 20  ) num = 4.50826*(TMath::Erf((x+0.49867)/40.72323)*TMath::Exp(-0.12926*x))+0.55208;
      else if (idx == 21  ) num = 4.68063*(TMath::Erf((x-2.90900)/35.46767)*TMath::Exp(-0.29199*x))+0.83391;
      else if (idx == 22  ) num = 4.96548*(TMath::Erf((x-2.35765)/40.80267)*TMath::Exp(-0.22843*x))+0.79089;
      else if (idx == 23  ) num = 4.32709*(TMath::Erf((x-3.51034)/40.11802)*TMath::Exp(-0.30024*x))+0.86465;
      else if (idx == 24  ) num = 4.41659*(TMath::Erf((x-2.17488)/36.79007)*TMath::Exp(-0.21484*x))+0.77745;
      else if (idx == 25  ) num = 4.88984*(TMath::Erf((x-2.30719)/30.53819)*TMath::Exp(-0.26070*x))+0.78608;
      else if (idx == 26  ) num = 5.09131*(TMath::Erf((x-2.77046)/31.64614)*TMath::Exp(-0.31439*x))+0.82341;
      else if (idx == 27  ) num = 4.60618*(TMath::Erf((x-2.67158)/36.01695)*TMath::Exp(-0.27708*x))+0.82110;
      else if (idx == 28  ) num = 4.10186*(TMath::Erf((x-1.03815)/41.42019)*TMath::Exp(-0.18461*x))+0.73355;
      else if (idx == 29  ) num = 4.89119*(TMath::Erf((x-2.83959)/33.45651)*TMath::Exp(-0.33063*x))+0.83500;
      else if (idx == 30  ) num = 4.50588*(TMath::Erf((x-1.60403)/38.70297)*TMath::Exp(-0.18667*x))+0.72965;
      else if (idx == 31  ) num = 2.80728*(TMath::Erf((x-2.12557)/0.00016)*TMath::Exp(-1.45391*x))+0.89137;
      else if (idx == 32  ) num = 1.86237*(TMath::Erf((x+6.25107)/26.70861)*TMath::Exp(-0.05618*x))+0.27568;
      else if (idx == 33  ) num = 2.94625*(TMath::Erf((x-2.39421)/0.10788)*TMath::Exp(-1.49951*x))+0.88253;
      else if (idx == 34  ) num = 4.22403*(TMath::Erf((x-1.51808)/39.85369)*TMath::Exp(-0.17218*x))+0.73196;
      else if (idx == 35  ) num = 3.82888*(TMath::Erf((x+2.45401)/38.82153)*TMath::Exp(-0.10241*x))+0.41923;
      else if (idx == 36  ) num = 5.38616*(TMath::Erf((x-2.95103)/43.41690)*TMath::Exp(-0.26222*x))+0.83469;
      else if (idx == 37  ) num = 5.37240*(TMath::Erf((x-2.45629)/38.99577)*TMath::Exp(-0.27025*x))+0.80197;
      else if (idx == 38  ) num = 3.15215*(TMath::Erf((x-2.33853)/0.01036)*TMath::Exp(-1.49526*x))+0.88450;
      else if (idx == 39  ) num = 5.15458*(TMath::Erf((x-2.70014)/32.27207)*TMath::Exp(-0.34688*x))+0.83817;
      else if (idx == 40  ) num = 5.25552*(TMath::Erf((x-1.16855)/36.37978)*TMath::Exp(-0.18637*x))+0.66828;
      else if (idx == 41  ) num = 4.37383*(TMath::Erf((x-3.19540)/38.55373)*TMath::Exp(-0.29106*x))+0.84774;
      else if (idx == 42  ) num = 4.32605*(TMath::Erf((x-2.77449)/42.24910)*TMath::Exp(-0.21841*x))+0.81615;
      else if (idx == 43  ) num = 5.48099*(TMath::Erf((x-2.67596)/41.16328)*TMath::Exp(-0.25295*x))+0.81525;
      else if (idx == 44  ) num = 3.78751*(TMath::Erf((x-1.35797)/41.48186)*TMath::Exp(-0.15429*x))+0.72638;
      else if (idx == 45  ) num = 5.04222*(TMath::Erf((x-2.79747)/31.72236)*TMath::Exp(-0.31136*x))+0.83218;
      else if (idx == 46  ) num = 3.95311*(TMath::Erf((x+0.37909)/37.24879)*TMath::Exp(-0.11930*x))+0.55068;
      else if (idx == 47  ) num = 5.16209*(TMath::Erf((x-2.73921)/30.30643)*TMath::Exp(-0.31776*x))+0.82259;
      else if (idx == 48  ) num = 5.43504*(TMath::Erf((x-2.30643)/29.14589)*TMath::Exp(-0.33320*x))+0.80081;
      else if (idx == 49  ) num = 5.33393*(TMath::Erf((x-3.47330)/32.43075)*TMath::Exp(-0.40519*x))+0.87160;
      else if (idx == 50  ) num = 4.39145*(TMath::Erf((x-3.00622)/38.49178)*TMath::Exp(-0.25902*x))+0.83738;
      else if (idx == 51  ) num = 3.84852*(TMath::Erf((x-0.63165)/41.01750)*TMath::Exp(-0.13869*x))+0.66807;
      else if (idx == 52  ) num = 4.83673*(TMath::Erf((x-3.02872)/34.86702)*TMath::Exp(-0.31011*x))+0.83779;
      else if (idx == 53  ) num = 4.75184*(TMath::Erf((x+2.25686)/40.75815)*TMath::Exp(-0.09796*x))+0.32462;
      else if (idx == 54  ) num = 4.85188*(TMath::Erf((x-2.56140)/33.10304)*TMath::Exp(-0.27873*x))+0.80676;
      else if (idx == 55  ) num = 2.97080*(TMath::Erf((x+5.98781)/31.45450)*TMath::Exp(-0.06676*x))+0.10251;
      else if (idx == 56  ) num = 4.44884*(TMath::Erf((x-2.01689)/36.27813)*TMath::Exp(-0.21309*x))+0.76431;
      else if (idx == 57  ) num = 4.54864*(TMath::Erf((x+0.75780)/43.78553)*TMath::Exp(-0.13650*x))+0.56840;
      else if (idx == 58  ) num = 5.63399*(TMath::Erf((x-3.39735)/32.19006)*TMath::Exp(-0.42513*x))+0.86723;
      else if (idx == 59  ) num = 4.22223*(TMath::Erf((x-2.33830)/34.26258)*TMath::Exp(-0.22745*x))+0.79102;
      else if (idx == 60  ) num = 3.95777*(TMath::Erf((x-3.08857)/43.83355)*TMath::Exp(-0.22818*x))+0.83842;
      else if (idx == 61  ) num = 7.87399*(TMath::Erf((x-4.37128)/23.16501)*TMath::Exp(-0.83271*x))+0.89985;
      else if (idx == 62  ) num = 4.30359*(TMath::Erf((x-2.29617)/42.18127)*TMath::Exp(-0.18967*x))+0.78741;
      else if (idx == 63  ) num = 6.85410*(TMath::Erf((x-3.65190)/27.73604)*TMath::Exp(-0.58987*x))+0.88411;
      else if (idx == 64  ) num = 4.60042*(TMath::Erf((x-1.34921)/39.40192)*TMath::Exp(-0.17285*x))+0.70905;
      else if (idx == 65  ) num = 4.43018*(TMath::Erf((x-1.73514)/36.17353)*TMath::Exp(-0.24275*x))+0.78286;
      else if (idx == 66  ) num = 4.40500*(TMath::Erf((x-2.13929)/37.18349)*TMath::Exp(-0.21546*x))+0.77513;
      else if (idx == 67  ) num = 5.39667*(TMath::Erf((x-3.38777)/33.96817)*TMath::Exp(-0.38430*x))+0.86233;
      else if (idx == 68  ) num = 4.15846*(TMath::Erf((x-1.45373)/40.15558)*TMath::Exp(-0.17268*x))+0.72698;
      else if (idx == 69  ) num = 4.63592*(TMath::Erf((x-2.23472)/30.95344)*TMath::Exp(-0.25367*x))+0.78077;
      else if (idx == 70  ) num = 5.18421*(TMath::Erf((x-2.60771)/31.90493)*TMath::Exp(-0.32187*x))+0.81973;
      else if (idx == 71  ) num = 6.58134*(TMath::Erf((x-4.42374)/30.32943)*TMath::Exp(-0.65752*x))+0.89697;
      else if (idx == 72  ) num = 4.05025*(TMath::Erf((x+0.25526)/41.94452)*TMath::Exp(-0.13473*x))+0.61448;
      else if (idx == 73  ) num = 4.31739*(TMath::Erf((x-1.59291)/38.75904)*TMath::Exp(-0.18412*x))+0.73693;
      else if (idx == 74  ) num = 9.49670*(TMath::Erf((x-4.53003)/8.91216)*TMath::Exp(-1.49970*x))+0.89222;
      else if (idx == 75  ) num = 5.25896*(TMath::Erf((x-3.15667)/32.08849)*TMath::Exp(-0.36372*x))+0.85560;
      else if (idx == 76  ) num = 5.03033*(TMath::Erf((x-3.08572)/33.13889)*TMath::Exp(-0.32561*x))+0.84655;
      else if (idx == 77  ) num = 3.84170*(TMath::Erf((x-1.49910)/42.56224)*TMath::Exp(-0.20220*x))+0.77885;
      else if (idx == 78  ) num = 4.01239*(TMath::Erf((x+0.92254)/44.19696)*TMath::Exp(-0.12830*x))+0.58445;
      else if (idx == 79  ) num = 4.73645*(TMath::Erf((x-1.36131)/45.58013)*TMath::Exp(-0.16612*x))+0.71911;
      else if (idx == 80  ) num = 2.43520*(TMath::Erf((x-2.21197)/49.50462)*TMath::Exp(-0.16443*x))+0.83999;
      else if (idx == 81  ) num = 4.02190*(TMath::Erf((x-0.48798)/38.96526)*TMath::Exp(-0.13684*x))+0.64075;
      else if (idx == 82  ) num = 4.93154*(TMath::Erf((x-3.04987)/34.57616)*TMath::Exp(-0.32426*x))+0.84174;
      else if (idx == 83  ) num = 5.02678*(TMath::Erf((x-3.11836)/34.75810)*TMath::Exp(-0.31463*x))+0.84878;
      else if (idx == 84  ) num = 5.07329*(TMath::Erf((x-2.40858)/30.30220)*TMath::Exp(-0.32512*x))+0.82187;
      else if (idx == 85  ) num = 5.15996*(TMath::Erf((x-2.21572)/0.15299)*TMath::Exp(-1.49997*x))+0.88176;
      else if (idx == 86  ) num = 4.37232*(TMath::Erf((x-2.38482)/36.16544)*TMath::Exp(-0.23845*x))+0.79772;
      else if (idx == 87  ) num = 4.77867*(TMath::Erf((x-0.72766)/39.66357)*TMath::Exp(-0.15187*x))+0.64245;
      else if (idx == 88  ) num = 5.49192*(TMath::Erf((x-2.21446)/39.31160)*TMath::Exp(-0.31218*x))+0.82779;
      else if (idx == 89  ) num = 5.51817*(TMath::Erf((x-3.23217)/31.15848)*TMath::Exp(-0.38716*x))+0.85974;
      else if (idx == 90  ) num = 3.25148*(TMath::Erf((x+0.12939)/36.03396)*TMath::Exp(-0.11907*x))+0.62463;
      else if (idx == 91  ) num = 4.05289*(TMath::Erf((x-0.74911)/39.53196)*TMath::Exp(-0.15114*x))+0.67048;
      else if (idx == 92  ) num = 3.14132*(TMath::Erf((x-1.44581)/44.25597)*TMath::Exp(-0.16531*x))+0.78169;
      else if (idx == 93  ) num = 8.96535*(TMath::Erf((x-4.39994)/19.53412)*TMath::Exp(-1.01421*x))+0.89398;
      else if (idx == 94  ) num = 4.22744*(TMath::Erf((x-0.49391)/42.92362)*TMath::Exp(-0.13423*x))+0.64590;
      else if (idx == 95  ) num = 3.99282*(TMath::Erf((x-0.45379)/40.59125)*TMath::Exp(-0.13708*x))+0.64891;
      else if (idx == 96  ) num = 9.45216*(TMath::Erf((x-3.81486)/5.10960)*TMath::Exp(-1.50000*x))+0.89607;
      else if (idx == 97  ) num = 5.58147*(TMath::Erf((x-3.39500)/33.91675)*TMath::Exp(-0.40489*x))+0.86484;
      else if (idx == 98  ) num = 9.41463*(TMath::Erf((x-2.90433)/2.78354)*TMath::Exp(-1.49999*x))+0.88652;
      else if (idx == 99  ) num = 2.88357*(TMath::Erf((x+2.45164)/35.56828)*TMath::Exp(-0.09732*x))+0.49869;
      else if (idx == 100 ) num = 5.21472*(TMath::Erf((x-2.59997)/42.38026)*TMath::Exp(-0.25674*x))+0.81454;
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
