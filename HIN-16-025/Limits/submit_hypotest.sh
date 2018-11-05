#!/bin/bash

export pwd_=$PWD
export njobs=600
export sbmodel="SbHypo"
export bmodel="BHypo"
export infile="CombinedWorkspaces/combo_12007_16004_20160808.root"
export calculatorType=0
export testStatType=2
export ntoys=50

for i in `seq 0 $njobs`; do
   echo qsub -k oe -q cms@llrt3 -l nodes=1:ppn=1 -N hypotest_rndseed${rndseed} -V -o $PWD -v pwd_,njobs,sbmodel,bmodel,infile,calculatorType,testSTatType,ntoys,rndseed runbatch_hypotest.sh
   export rndseed=$i
   qsub -k oe -q cms@llrt3 -l nodes=1:ppn=1 -N hypotest_rndseed${rndseed} -V -o $PWD -v pwd_,njobs,sbmodel,bmodel,infile,calculatorType,testSTatType,ntoys,rndseed runbatch_hypotest.sh
done
