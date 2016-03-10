#!/bin/sh 
#cat test.txt >& tlist
#cat test2.txt >& tlist2
cat ../fnc_pp_eta1.txt >& tlist1
cat ../fnc_pp_eta2.txt >& tlist2

exec < tlist1
export i=1
while read line1
do
  echo $i
  awk -v p=$line1 '{gsub("_fnc1_",p); print}' ppEffJpsiSysSFsSTA.C > del_eff_$i.C;
  i=$(expr $i + 1);

done

exec < tlist2
export i=1
while read line4
do
  echo $i
  mkdir Eff_00$i
  cat del_eff_$i.C | sed s/_idx_/$i/g > del_eff4_$i.C
  awk -v p=$line4 '{gsub("_fnc2_",p); print}' del_eff4_$i.C > Eff_00$i/ppEffJpsiSysSFsSTA_$i.C
  #root -l Eff_00$i/pbpbEffJpsi_$i.C+ -q -b
  #cat del_eff2_$i.C | sed s/_fnc2_/$line2/g > Eff_00$i/pbpbEffJpsi_$i.C
  i=$(expr $i + 1);

done

rm del_*



