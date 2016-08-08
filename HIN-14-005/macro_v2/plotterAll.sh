#!/bin/sh

## this does all the v2 fits, for all nominal and systm. variations

#nominal
root -l v2_y_plotter.C+ -q -b 
root -l v2_pt_plotter.C"(1,2)" -q -b 
root -l v2_pt_plotter.C"(2,1)" -q -b 
root -l v2_cent_plotter.C+ -q -b 

