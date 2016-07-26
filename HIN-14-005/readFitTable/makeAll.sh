#!/bin/sh

## this does the readFile for all, pp, pbpb, prompt, non-prompt, efficiency, etc, Raa and v2

### *************************************************** RAA ***************************
###################### PbPb ######################

#  weighted
root -l makeHistos_raa.C\(0,1,0\)+ -q -b 

# non-weighted
# make efficiency histogram
root -l makeHistos_raa.C\(0,0,1\)+ -q -b
# non-weighted 
root -l makeHistos_raa.C\(0,0,0\)+ -q -b

#### ------------- systematic uncertainties
### 3D eff
root -l makeHistos_raa.C\(0,1,2\)+ -q -b
### TnP
#-----  stat
root -l makeHistos_raa.C\(0,1,3\)+ -q -b
#----- syst
root -l makeHistos_raa.C\(0,1,4\)+ -q -b
# TnP-sta 
# ----- stat
root -l makeHistos_raa.C\(0,1,5\)+ -q -b
# ----- syst
root -l makeHistos_raa.C\(0,1,6\)+ -q -b

###################### pp ######################

#  weighted
root -l makeHistos_raa.C\(1,1,0\)+ -q -b 

# non-weighted
# make efficiency histogram
root -l makeHistos_raa.C\(1,0,1\)+ -q -b
# non-weighted 
root -l makeHistos_raa.C\(1,0,0\)+ -q -b

#### ------------- systematic uncertainties
### 3D eff
root -l makeHistos_raa.C\(1,1,2\)+ -q -b
### TnP
#-----  stat
root -l makeHistos_raa.C\(1,1,3\)+ -q -b
#----- syst
root -l makeHistos_raa.C\(1,1,4\)+ -q -b
# TnP-sta 
# ----- stat
root -l makeHistos_raa.C\(1,1,5\)+ -q -b
# ----- syst
root -l makeHistos_raa.C\(1,1,6\)+ -q -b

############## rest of the systematic variations ##################
###################### PbPb ######################
root -l makeHistos_raa_syst.C\(0,0\)+ -q -b

###################### pp ######################
root -l makeHistos_raa_syst.C\(1,0\)+ -q -b

### *************************************************** v2 ***************************
root -l makeHistos_v2.C+ -q -b
