
datevalue=$(date +%F)
echo $datevalue
mv TMVAClassification TMVAClassification_$datevalue
mkdir -p TMVAClassification/plots
root -l tmvagui.C
