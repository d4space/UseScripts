#scp -r kistiUi20:Latino/CMSSW10215pch2/src/SNuAnalytics/Configurations/HWWSemiLepHighMass/nAODv5v6_MVA/out_root_2017_Resggf400_ggf800vsEW0p1 .
#scp -r kistiUi20:Latino/CMSSW10215pch2/src/SNuAnalytics/Configurations/HWWSemiLepHighMass/nAODv5v6_MVA/out_root_2017_Resggf400_ggf800vsVbf800 .
#scp -r kistiUi20:Latino/CMSSW10215pch2/src/SNuAnalytics/Configurations/HWWSemiLepHighMass/nAODv5v6_MVA/out_root_2017_Bstggf400_ggf800vsVbf800 .
#scp -r kistiUi20:Latino/CMSSW10215pch2/src/SNuAnalytics/Configurations/HWWSemiLepHighMass/nAODv5v6_MVA/out_root_2017_Bstggf400_ggf800vsEW0p1 .
#scp -r kistiUi20:Latino/CMSSW10215pch2/src/SNuAnalytics/Configurations/HWWSemiLepHighMass/nAODv5v6_MVA/out_root_2017_Resggf400_ggf200vsEW0p1 .
#scp -r kistiUi20:Latino/CMSSW10215pch2/src/SNuAnalytics/Configurations/HWWSemiLepHighMass/nAODv5v6_MVA/out_root_2017_Resggf400_ggf200vsVbf200 .
#scp -r kistiUi20:Latino/CMSSW10215pch2/src/SNuAnalytics/Configurations/HWWSemiLepHighMass/nAODv5v6_MVA/out_root_2017_Bstggf400_ggf200vsVbf200 .
#scp -r kistiUi20:Latino/CMSSW10215pch2/src/SNuAnalytics/Configurations/HWWSemiLepHighMass/nAODv5v6_MVA/out_root_2017_Bstggf400_ggf200vsEW0p1 .
#scp -r kistiUi20:Latino/CMSSW10215pch2/src/SNuAnalytics/Configurations/HWWSemiLepHighMass/nAODv5v6_MVA/out_root_2017_ggf400_EW_Res .
#scp -r kistiUi20:Latino/CMSSW10215pch2/src/SNuAnalytics/Configurations/HWWSemiLepHighMass/nAODv5v6_MVA/out_root_2017_ggf400_vbfEW .


datevalue=$(date +%F)
echo $datevalue

mv TMVAClassification TMVAClassification_$datevalue
mkdir -p TMVAClassification/plots
root -l 'mvaeffscxxMod.C("","out_root_2017_Resggf400_ggf800vsEW0p1/out_train_2017.root",50,0.01)'
#root -l 'mvaeffscxxMod.C("","out_root_2017_Resggf400_ggf800vsVbf800/out_train_2017.root",50,0.01)'
#root -l 'mvaeffscxxMod.C("","out_root_2017_Bstggf400_ggf800vsVbf800/out_train_2017.root",50,0.01)'
#root -l 'mvaeffscxxMod.C("","out_root_2017_Bstggf400_ggf800vsEW0p1/out_train_2017.root",50,0.01)'
#root -l 'mvaeffscxxMod.C("","out_root_2017_Resggf400_ggf200vsEW0p1/out_train_2017.root",50,0.01)'
#root -l 'mvaeffscxxMod.C("","out_root_2017_Resggf400_ggf200vsVbf200/out_train_2017.root",50,0.01)'
#root -l 'mvaeffscxxMod.C("","out_root_2017_Bstggf400_ggf200vsVbf200/out_train_2017.root",50,0.01)'
#root -l 'mvaeffscxxMod.C("","out_root_2017_Bstggf400_ggf200vsEW0p1/out_train_2017.root",50,0.01)'
#root -l 'mvaeffscxxMod.C("","out_root_2017_ggf400_EW_Res/out_train_2017.root",50,0.01)'
#root -l 'mvaeffscxxMod.C("","out_root_2017_ggf400_vbf400_Res/out_train_2017.root",50,0.001)'
#root -l 'mvaeffscxxMod.C("","out_root_2017_ggf400_vbfEW/out_train_2017.root",50,0.01)'
#root -l 'mvaeffscxxMod.C("","out_root_2017_ggf400_EW/out_train_2017.root",50,0.01)'
#root -l 'mvaeffscxxMod.C("","out_root_2017_ggf400_vbf400/out_train_2017.root",50,0.01)'


#export TMVASYS=$ROOTSYS
#root -l tmvagui.C


#root -l -e 'TMVA::TMVAGUI("out_root_2017/out_train_2017.root")'
