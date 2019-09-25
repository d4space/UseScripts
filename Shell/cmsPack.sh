#!/bin/bash
export VO_CMS_SW_DIR=/cvmfs/cms.cern.ch
source ${VO_CMS_SW_DIR}/cmsset_default.sh
if [[ $HOSTNAME == *"ui"*".sdfarm.kr"* ]]; then
  echo "Configuration for "$HOSTNAME
  export SCRAM_ARCH=slc7_amd64_gcc700 # for 940 10_1_0 , 10_2_X
  source /cvmfs/cms.cern.ch/crab3/crab.sh
fi
#export SCRAM_ARCH=slc6_amd64_gcc530 # for 80X
#export SCRAM_ARCH=slc6_amd64_gcc493
#export SCRAM_ARCH=slc5_amd64_gcc472 # for  611
