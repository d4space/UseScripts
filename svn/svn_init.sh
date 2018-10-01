#!/bin/bash
#myDir="HIG14032"
#myDir="HIG-15-003"
#myDir="AN2012067"
myDir="AN-18-227"
#myDir="AN-16-182"


#echo $myDir
svn co -N svn+ssh://salee@svn.cern.ch/reps/tdr2 $myDir
cd $myDir
svn update utils
svn update -N notes
#svn update -N papers
#svn update notes/AN-16-182
svn update notes/$myDir
#svn update notes/HIG-15-003
#svn update notes/HIG-14-032
#svn update notes/AN-15-300
