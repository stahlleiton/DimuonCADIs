#!/bin/sh

root -l makeRaa_pt.C+ -q -b 
root -l makeRaa_y.C+ -q -b 
root -l makeRaa_cent.C+ -q -b 

root -l compare_pt.C+ -q -b 
root -l compare_y.C+ -q -b 
root -l compare_cent.C+ -q -b 
