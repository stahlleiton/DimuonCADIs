#!/bin/sh

root -l makeSyst_pt.C+ -q -b 
root -l makeSyst_y.C+ -q -b 
root -l makeSyst_cent.C+ -q -b 

root -l plotSyst_pt.C+ -q -b 
root -l plotSyst_y.C+ -q -b 
root -l plotSyst_cent.C+ -q -b 
