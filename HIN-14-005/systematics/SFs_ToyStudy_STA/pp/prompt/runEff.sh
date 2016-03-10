#!/bin/sh 
#cat test.txt >& tlist
#cat test2.txt >& tlist2
ls -al | grep Eff_ >& dirlist

exec < dirlist
export i=1
while read line
do
  cd Eff_00$i
  root -l ppEffJpsiSysSFsSTA_$i.C+ -q -b
  cd ../
  i=$(expr $i + 1);

done

