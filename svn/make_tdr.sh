#!/bin/bash
eval `./tdr runtime -sh`
#tdr  --style=an b AN-13-023
#tdr --draft --style=an b SMP-12-021
tdr --draft --style=an b AN-16-182
#tdr --draft --style=an b AN-13-023
#open tmp/SMP-12-021_temp.pdf&
open tmp/AN-16-182_temp.pdf&
