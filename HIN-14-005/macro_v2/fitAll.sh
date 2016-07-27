#!/bin/sh

## this does all the v2 fits, for all nominal and systm. variations

#nominal
root -l v2_fitter.C\(1,-1,0,4,\"../readFitTable\",\"outputNumbers\",1,0\)+ -q -b 
root -l v2_fitter.C\(2,-1,0,4,\"../readFitTable\",\"outputNumbers\",1,0\)+ -q -b 
root -l v2_fitter.C\(3,-1,0,4,\"../readFitTable\",\"outputNumbers\",1,0\)+ -q -b 

#nominal with straight line fit
root -l v2_fitter.C\(1,-1,0,4,\"../readFitTable\",\"outputNumbers\",1,1\)+ -q -b 
root -l v2_fitter.C\(2,-1,0,4,\"../readFitTable\",\"outputNumbers\",1,1\)+ -q -b

#syst. variations
# prompt
root -l v2_fitter.C\(1,-1,1,4,\"../readFitTable\",\"outputNumbers\",1,0\)+ -q -b 
# non-pr
root -l v2_fitter.C\(2,-1,1,4,\"../readFitTable\",\"outputNumbers\",1,0\)+ -q -b 
#bkgd
root -l v2_fitter.C\(3,-1,1,4,\"../readFitTable\",\"outputNumbers\",1,0\)+ -q -b 
