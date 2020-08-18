#scp -r kistiUi20:Latino/CMSSW10215pch2/src/SNuAnalytics/Configurations/HWWSemiLepHighMass/nAODv5v6_MVA/out_root_2017_Bstggf400_ggf800vsVbf800 .



#export TMVASYS=$ROOTSYS
#root -l tmvagui.C


#root -l -e 'TMVA::TMVAGUI("out_train_2017_BDTsimple_hwwGgfvsEW0p1_backUp.root")'
root -l -e 'TMVA::TMVAGUI("out_train_2017_Bst_Pggfh1500_hwwGgfvsEW0p1.root")'
