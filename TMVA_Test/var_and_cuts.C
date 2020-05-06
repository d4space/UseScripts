{
  TFile f("out_root_2017/out_train_2017.root");
  f.cd("TMVAClassification");
  TrainTree->Draw("P_SovB>>hSig", "classID==0 && P_SovB < 1.5");
  TH1F* hSig = (TH1F*)gDirectory->Get("hSig");
  TrainTree->Draw("P_SovB>>hBkg", "classID==1 && P_SovB < 1.5");
  //TrainTree->Draw("P_SovB>>hBkg", "classID==1 && P_SovB < 1.5 && RecCuts> 0.1");
  //TrainTree->Draw("P_SovB>>h2", "classID==1 && P_SovB < 1.5","same");
  TH1F* hBkg = (TH1F*)gDirectory->Get("hBkg");
  hSig->SetLineColor(kBlue);
  hBkg->SetLineColor(kRed);
  hSig->SetLineWidth(2);
  hBkg->SetLineWidth(2);
  hSig->Draw();
  hBkg->Draw("same");
}
