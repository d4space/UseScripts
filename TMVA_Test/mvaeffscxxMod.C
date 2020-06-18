// From root-6.20.04/tmva/tmvagui/src

#include "TApplication.h"
#include "TCanvas.h"
#include "TClass.h"
#include "TFile.h"
#include "TFormula.h"
#include "TGButton.h"
#include "TGLabel.h"
#include "TGNumberEntry.h"
#include "TGWindow.h"
#include "TGaxis.h"
#include "TH1.h"
#include "TH2.h"
#include "TIterator.h"
#include "TKey.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TLine.h"
#include "TList.h"
#include "TMVA/mvaeffs.h"
#include "TMVA/tmvaglob.h"
#include "TPad.h"
#include "TROOT.h"
#include "TStyle.h"

#include <iomanip>
#include <iostream>

using std::cout;
using std::endl;
using std::setfill;
using std::setw;

namespace MineTMVA{

   void mvaeffs(TString dataset, TString fin = "TMVA.root", 
                Bool_t useTMVAStyle = kTRUE, TString formula="S/sqrt(S+B)" );

   // this macro plots the signal and background efficiencies
   // as a function of the MVA cut.


   class MethodInfo : public TNamed {
   public:
   MethodInfo() :
      methodName(""),
         varName(""),
         methodTitle(""),
         sig(0),
         bgd(0),
         origSigE(0),
         origBgdE(0),
         sigE(0),
         bgdE(0),
         purS(0),
         sSig(0),
         effpurS(0),
         canvas(0),
         line1(0),
         line2(0),
         rightAxis(0),
         maxSignificance(0),
         maxSignificanceErr(0)
            {cout<<"my MethodInfo"<<endl;}
      virtual ~MethodInfo();

      TString  methodName;
      TString  methodTitle;
      TString  varName;
      TH1*     sig;
      TH1*     bgd;
      TH1*     testSig;
      TH1*     testBkg;
      TH1*     origSigE;
      TH1*     origBgdE;
      TH1*     sigE;
      TH1*     bgdE;
      TH1*     purS;
      TH1*     sSig;    
      TH1*     effpurS;
      TCanvas* canvas;
      TLatex*  line1;
      TLatex*  line2;
      TGaxis*  rightAxis;
      Double_t maxSignificance;
      Double_t maxSignificanceErr;

      void SetResultHists(); 
      void SetVariableHists(); 

      ClassDef(MethodInfo,0);
   };

   class StatDialogMVAEffs {  

      RQ_OBJECT("StatDialogMVAEffs")
      
         public:

      StatDialogMVAEffs(TString ds,const TGWindow* p, Float_t ns, Float_t nb);
      virtual ~StatDialogMVAEffs();
   
      void SetFormula(const TString& f) { fFormula = f; }
      TString GetFormula();
      TString GetFormulaString() { return fFormula; }
      TString GetLatexFormula();
   
      void ReadHistograms(TFile* file);
      void ReadVariableToHisto(TFile* file, Int_t Nbins, Float_t maxX);
      void UpdateSignificanceHists();
      void MyVarSignificanceHists();
      void DrawHistograms();

      void RaiseDialog() { if (fMain) { fMain->RaiseWindow(); fMain->Layout(); fMain->MapWindow(); } }

   private:

      TGMainFrame *fMain;
      Float_t fNSignal;
      Float_t fNBackground;  
      TString fFormula;
      TString dataset;
      TList * fInfoList;

      TGNumberEntry* fSigInput;
      TGNumberEntry* fBkgInput;

      TGHorizontalFrame* fButtons;
      TGTextButton* fDrawButton;
      TGTextButton* fCloseButton;

      Int_t maxLenTitle;

      void UpdateCanvases();

   public:

      // slots
      void SetNSignal(); //*SIGNAL*
      void SetNBackground(); //*SIGNAL*
      void Redraw(); //*SIGNAL*
      void Close(); //*SIGNAL*

      // result printing
      void PrintResults( const MethodInfo* info );
   };

}

MineTMVA::MethodInfo::~MethodInfo() 
{
   delete sigE;
   delete bgdE;
   delete purS;
   delete sSig;
   delete effpurS;
   if(gROOT->GetListOfCanvases()->FindObject(canvas))
      delete canvas;
}

void MineTMVA::MethodInfo::SetResultHists() 
{
   TString pname    = "purS_"         + methodTitle;
   TString epname   = "effpurS_"      + methodTitle;
   TString ssigname = "significance_" + methodTitle;
   
   sigE = (TH1*)origSigE->Clone("sigEffi");
   bgdE = (TH1*)origBgdE->Clone("bgdEffi");
   
   Int_t nbins = sigE->GetNbinsX();
   Double_t low = sigE->GetBinLowEdge(1);
   Double_t high = sigE->GetBinLowEdge(nbins+1);
   purS    = new TH1F(pname, pname, nbins, low, high);
   sSig    = new TH1F(ssigname, ssigname, nbins, low, high);
   effpurS = new TH1F(epname, epname, nbins, low, high);        
   
   // chop off useless stuff
   sigE->SetTitle( Form("Cut efficiencies for %s classifier", methodTitle.Data()) );
   
   // set the histogram style
   TMVA::TMVAGlob::SetSignalAndBackgroundStyle( sigE, bgdE );
   TMVA::TMVAGlob::SetSignalAndBackgroundStyle( purS, bgdE );
   TMVA::TMVAGlob::SetSignalAndBackgroundStyle( effpurS, bgdE );
   sigE->SetFillStyle( 0 );
   bgdE->SetFillStyle( 0 );
   sSig->SetFillStyle( 0 );
   sigE->SetLineWidth( 3 );
   bgdE->SetLineWidth( 3 );
   sSig->SetLineWidth( 3 );
   
   // the purity and quality
   purS->SetFillStyle( 0 );
   purS->SetLineWidth( 2 );
   purS->SetLineStyle( 5 );
   effpurS->SetFillStyle( 0 );
   effpurS->SetLineWidth( 2 );
   effpurS->SetLineStyle( 6 );
}
void MineTMVA::MethodInfo::SetVariableHists() 
{
   TString pname    = "purS_"         + methodTitle;
   TString epname   = "effpurS_"      + methodTitle;
   TString ssigname = "significance_" + methodTitle;
   
   sigE = (TH1*)origSigE->Clone("sigEffi");
   bgdE = (TH1*)origBgdE->Clone("bgdEffi");
   
   Int_t nbins   = sigE->GetNbinsX();
   Double_t low  = sigE->GetBinLowEdge(1);
   Double_t high = sigE->GetBinLowEdge(nbins+1);
   purS    = new TH1F(pname, pname, nbins, low, high);
   sSig    = new TH1F(ssigname, ssigname, nbins, low, high);
   effpurS = new TH1F(epname, epname, nbins, low, high);        
   
   // chop off useless stuff
   sigE->SetTitle( Form("Cut efficiencies for %s ", varName.Data()) );
   //sigE->SetTitle( Form("Cut efficiencies for %s classifier", methodTitle.Data()) );
   
   // set the histogram style
   TMVA::TMVAGlob::SetSignalAndBackgroundStyle( sigE, bgdE );
   TMVA::TMVAGlob::SetSignalAndBackgroundStyle( purS, bgdE );
   TMVA::TMVAGlob::SetSignalAndBackgroundStyle( effpurS, bgdE );
   sigE->SetFillStyle( 0 );
   bgdE->SetFillStyle( 0 );
   sSig->SetFillStyle( 0 );
   sigE->SetLineWidth( 3 );
   bgdE->SetLineWidth( 3 );
   sSig->SetLineWidth( 3 );
   
   // the purity and quality
   purS->SetFillStyle( 0 );
   purS->SetLineWidth( 2 );
   purS->SetLineStyle( 5 );
   effpurS->SetFillStyle( 0 );
   effpurS->SetLineWidth( 2 );
   effpurS->SetLineStyle( 6 );
}

void MineTMVA::StatDialogMVAEffs::SetNSignal() 
{
   fNSignal = fSigInput->GetNumber();
}

void MineTMVA::StatDialogMVAEffs::SetNBackground() 
{
   fNBackground = fBkgInput->GetNumber();
}

TString MineTMVA::StatDialogMVAEffs::GetFormula() 
{
   // replace all occurrence of S and B but only if neighbours are not alphanumerics
   auto replace_vars = [](TString & f, char oldLetter, char newLetter ) {
      auto pos = f.First(oldLetter);
      while(pos != kNPOS) {
         if ( ( pos > 0 && !TString(f[pos-1]).IsAlpha() ) ||
              ( pos < f.Length()-1 &&  !TString(f[pos+1]).IsAlpha() ) )
         {
            f[pos] = newLetter;
         }
      int pos2 = pos+1;
      pos = f.Index(oldLetter,pos2);
      }
   };

   TString formula = fFormula;
   replace_vars(formula,'S','x');
   replace_vars(formula,'B','y');
   // f.ReplaceAll("S","x");
   // f.ReplaceAll("B","y");
   return formula;
}


TString MineTMVA::StatDialogMVAEffs::GetLatexFormula() 
{
   TString f = fFormula;
   f.ReplaceAll("(","{");
   f.ReplaceAll(")","}");
   f.ReplaceAll("sqrt","#sqrt");
   return f;
}

void MineTMVA::StatDialogMVAEffs::Redraw() 
{
   UpdateSignificanceHists();
   UpdateCanvases();
}

void MineTMVA::StatDialogMVAEffs::Close() 
{
   delete this;
}

MineTMVA::StatDialogMVAEffs::~StatDialogMVAEffs() 
{
   if (fInfoList) { 
      TIter next(fInfoList);
      MethodInfo *info(0);
      while ( (info = (MethodInfo*)next()) ) {
         delete info;
      }
      delete fInfoList;
      fInfoList=0;
   }


   fSigInput->Disconnect();
   fBkgInput->Disconnect();
   fDrawButton->Disconnect();
   fCloseButton->Disconnect();

   fMain->CloseWindow();
   fMain->Cleanup();
   fMain = 0;
}

MineTMVA::StatDialogMVAEffs::StatDialogMVAEffs(TString ds,const TGWindow* p, Float_t ns, Float_t nb) :
   fNSignal(ns),
   fNBackground(nb),
   fFormula(""),
   dataset(ds),
   fInfoList(0),
   fSigInput(0),
   fBkgInput(0),
   fButtons(0),
   fDrawButton(0),
   fCloseButton(0),
   maxLenTitle(0)
{
   UInt_t totalWidth  = 500;
   UInt_t totalHeight = 300;

   // main frame
   fMain = new TGMainFrame(p, totalWidth, totalHeight, kMainFrame | kVerticalFrame);

   TGLabel *sigLab = new TGLabel(fMain,"Signal events");
   fMain->AddFrame(sigLab, new TGLayoutHints(kLHintsLeft | kLHintsTop,5,5,5,5));

   fSigInput = new TGNumberEntry(fMain, (Double_t) fNSignal,5,-1,(TGNumberFormat::EStyle) 5);
   fSigInput->SetLimits(TGNumberFormat::kNELLimitMin,0,1);
   fMain->AddFrame(fSigInput, new TGLayoutHints(kLHintsLeft | kLHintsTop,5,5,5,5));
   fSigInput->Resize(100,24);

   TGLabel *bkgLab = new TGLabel(fMain, "Background events");
   fMain->AddFrame(bkgLab, new TGLayoutHints(kLHintsLeft | kLHintsTop,5,5,5,5));

   fBkgInput = new TGNumberEntry(fMain, (Double_t) fNBackground,5,-1,(TGNumberFormat::EStyle) 5);
   fBkgInput->SetLimits(TGNumberFormat::kNELLimitMin,0,1);
   fMain->AddFrame(fBkgInput, new TGLayoutHints(kLHintsLeft | kLHintsTop,5,5,5,5));
   fBkgInput->Resize(100,24);

   fButtons = new TGHorizontalFrame(fMain, totalWidth,30);

   fCloseButton = new TGTextButton(fButtons,"&Close");
   fButtons->AddFrame(fCloseButton, new TGLayoutHints(kLHintsLeft | kLHintsTop));

   fDrawButton = new TGTextButton(fButtons,"&Draw");
   fButtons->AddFrame(fDrawButton, new TGLayoutHints(kLHintsRight | kLHintsTop,15));
  
   fMain->AddFrame(fButtons,new TGLayoutHints(kLHintsLeft | kLHintsBottom,5,5,5,5));

   fMain->SetWindowName("Significance");
   fMain->SetWMPosition(0,0);
   fMain->MapSubwindows();
   fMain->Resize(fMain->GetDefaultSize());
   fMain->MapWindow();

   fSigInput->Connect("ValueSet(Long_t)","MineTMVA::StatDialogMVAEffs",this, "SetNSignal()");
   fBkgInput->Connect("ValueSet(Long_t)","MineTMVA::StatDialogMVAEffs",this, "SetNBackground()");

//   fDrawButton->Connect("Clicked()","TGNumberEntry",fSigInput, "ValueSet(Long_t)");
//   fDrawButton->Connect("Clicked()","TGNumberEntry",fBkgInput, "ValueSet(Long_t)");
   fDrawButton->Connect("Clicked()", "MineTMVA::StatDialogMVAEffs", this, "Redraw()");   

   fCloseButton->Connect("Clicked()", "MineTMVA::StatDialogMVAEffs", this, "Close()");
}

void MineTMVA::StatDialogMVAEffs::UpdateCanvases() 
{
   if (fInfoList==0) return;
   if (fInfoList->First()==0) return;
   MethodInfo* info = (MethodInfo*)fInfoList->First();
   if ( info->canvas==0 ) {
      DrawHistograms();
      return;
   }
   TIter next(fInfoList);
   while ( (info = (MethodInfo*)next()) ) {
      info->canvas->Update();
      info->rightAxis->SetWmax(1.1*info->maxSignificance);
      info->canvas->Modified(kTRUE);
      info->canvas->Update();
      info->canvas->Paint();
   }
}

void MineTMVA::StatDialogMVAEffs::UpdateSignificanceHists() 
{
   TFormula f("sigf",GetFormula());
   TIter next(fInfoList);
   MethodInfo* info(0);
   TString cname = "Classifier";
   if (cname.Length() >  maxLenTitle)  maxLenTitle = cname.Length();
   TString str = Form( "%*s   (  #signal, #backgr.)  Optimal-cut  %s      NSig      NBkg   EffSig   EffBkg", 
                       maxLenTitle, cname.Data(), GetFormulaString().Data() );
   cout << "--- " << setfill('=') << setw(str.Length()) << "" << setfill(' ') << endl;
   cout << "--- " << str << endl;
   cout << "--- " << setfill('-') << setw(str.Length()) << "" << setfill(' ') << endl;
   Double_t maxSig    = -1;
   Double_t maxSigErr = -1;
   while ((info = (MethodInfo*)next())) {
      for (Int_t i=1; i<=info->origSigE->GetNbinsX(); i++) {
         Float_t eS = info->origSigE->GetBinContent( i );
         Float_t S = eS * fNSignal;
         Float_t B = info->origBgdE->GetBinContent( i ) * fNBackground;
         info->purS->SetBinContent( i, (S+B==0)?0:S/(S+B) );
         
         Double_t sig = f.Eval(S,B);
         if (sig > maxSig) {
            maxSig    = sig;
            if (GetFormulaString() == "S/sqrt(B)") {
               maxSigErr = sig * sqrt( 1./S + 1./(2.*B));
            }
         }
         info->sSig->SetBinContent( i, sig );
         info->effpurS->SetBinContent( i, eS*info->purS->GetBinContent( i ) );
      }
      
      info->maxSignificance    = info->sSig->GetMaximum();
      info->maxSignificanceErr = (maxSigErr > 0) ? maxSigErr : 0;
      info->sSig->Scale(1/info->maxSignificance);

      // update the text in the lower left corner
      PrintResults( info );
   }
   cout << "--- " << setfill('-') << setw(str.Length()) << "" << setfill(' ') << endl << endl;
}
void MineTMVA::StatDialogMVAEffs::MyVarSignificanceHists() 
{
  cout<<"Significance formula: "<<GetFormula()<<endl;
   TFormula f("sigf",GetFormula());
   TIter next(fInfoList);
   MethodInfo* info(0);
   TString cname = "AVariable";
   //TString cname = "Classifier";
   if (cname.Length() >  maxLenTitle)  maxLenTitle = cname.Length();
   TString str = Form( "%*s   (  #signal, #backgr.)  Optimal-cut  %s      NSig      NBkg   EffSig   EffBkg", 
                       maxLenTitle, cname.Data(), GetFormulaString().Data() );
   cout << "--- " << setfill('=') << setw(str.Length()) << "" << setfill(' ') << endl;
   cout << "--- " << str << endl;
   cout << "--- " << setfill('-') << setw(str.Length()) << "" << setfill(' ') << endl;
   Double_t maxSig    = -1;
   Double_t maxSigErr = -1;
   while ((info = (MethodInfo*)next())) {
      Int_t Nbins = info->origSigE->GetNbinsX();
      for (Int_t i=1; i <= Nbins; i++) {
	 Float_t totS = info->sig->Integral(1, Nbins+1);
	 Float_t totB = info->bgd->Integral(1, Nbins+1);
         Float_t eS = info->origSigE->GetBinContent( i );
         Float_t S = eS * totS;
         //Float_t S = eS * fNSignal;
         Float_t B = info->origBgdE->GetBinContent( i ) * totB;
         //Float_t B = info->origBgdE->GetBinContent( i ) * fNBackground;
         info->purS->SetBinContent( i, (S+B==0)?0:S/(S+B) );
         
         Double_t sig = f.Eval(S,B); // default as S/sqrt(S+B)
         if (sig > maxSig) {
            maxSig    = sig;
            if (GetFormulaString() == "S/sqrt(B)") {
               maxSigErr = sig * sqrt( 1./S + 1./(2.*B));
            }
         }
         info->sSig->SetBinContent( i, sig );
         info->effpurS->SetBinContent( i, eS*info->purS->GetBinContent( i ) );
      }
      
      info->maxSignificance    = info->sSig->GetMaximum();
      info->maxSignificanceErr = (maxSigErr > 0) ? maxSigErr : 0;
      info->sSig->Scale(1/info->maxSignificance);

      // update the text in the lower left corner
      PrintResults( info );
   }
   cout << "--- " << setfill('-') << setw(str.Length()) << "" << setfill(' ') << endl << endl;
}

void MineTMVA::StatDialogMVAEffs::ReadHistograms(TFile* file) 
{
   if (fInfoList) {
      TIter next(fInfoList);
      MethodInfo *info(0);
      while ( (info = (MethodInfo*)next()) ) {
         delete info;
      }
      delete fInfoList;
      fInfoList=0;
   }
   fInfoList = new TList;

   // search for the right histograms in full list of keys
   TIter next(file->GetDirectory(dataset.Data())->GetListOfKeys());
   TKey *key(0);   
   while( (key = (TKey*)next()) ) {

      if (!TString(key->GetName()).BeginsWith("Method_")) continue;
      if( ! gROOT->GetClass(key->GetClassName())->InheritsFrom("TDirectory") ) continue;

      cout << "--- Found directory: " << ((TDirectory*)key->ReadObj())->GetName() << endl;

      TDirectory* mDir = (TDirectory*)key->ReadObj();

      TIter keyIt(mDir->GetListOfKeys());
      TKey *titkey;
      while((titkey = (TKey*)keyIt())) {
         if( ! gROOT->GetClass(titkey->GetClassName())->InheritsFrom("TDirectory") ) continue;
        
         MethodInfo* info = new MethodInfo();
         TDirectory* titDir = (TDirectory *)titkey->ReadObj();

	 TMVA::TMVAGlob::GetMethodName(info->methodName,key);
	 TMVA::TMVAGlob::GetMethodTitle(info->methodTitle,titDir);        
         if (info->methodTitle.Length() > maxLenTitle) maxLenTitle = info->methodTitle.Length();
         TString hname = "MVA_" + info->methodTitle;
        
         cout << "--- Classifier: " << info->methodTitle << endl;
        
         info->sig = dynamic_cast<TH1*>(titDir->Get( hname + "_S" ));
         info->bgd = dynamic_cast<TH1*>(titDir->Get( hname + "_B" ));
         info->origSigE = dynamic_cast<TH1*>(titDir->Get( hname + "_effS" ));
         info->origBgdE = dynamic_cast<TH1*>(titDir->Get( hname + "_effB" ));      
         if (info->origSigE==0 || info->origBgdE==0) { delete info; continue; }

         info->SetResultHists();
         fInfoList->Add(info);
      }
   }
   return;
}
void MineTMVA::StatDialogMVAEffs::ReadVariableToHisto(TFile* file, Int_t Nbins, Float_t maxX) 
{
   if (fInfoList) {
      TIter next(fInfoList);
      MethodInfo *info(0);
      while ( (info = (MethodInfo*)next()) ) {
         delete info;
      }
      delete fInfoList;
      fInfoList=0;
   }
   fInfoList = new TList;

   // search for the right histograms in full list of keys
   TIter next(file->GetDirectory(dataset.Data())->GetListOfKeys());
   TKey *key(0);   
   TString varName = "";
   while( (key = (TKey*)next()) ) {
      //cout<<key->GetName()<<endl;

      // Get the variable name
      if (TString(key->GetName()).BeginsWith("InputVariables_")){
	if(gROOT->GetClass(key->GetClassName())->InheritsFrom("TDirectory")){
	  TDirectory* mDir = (TDirectory*)key->ReadObj();
	  TIter keyIt(mDir->GetListOfKeys() );
	  TKey *varKey;
	  while( (varKey = (TKey*) keyIt() ) ){
	    if( gROOT->GetClass(varKey->GetClassName())->InheritsFrom("TDirectory")  ) continue;
	    varName = ((TString)varKey->GetName()).ReplaceAll("__Signal_Id","");
	    break;
	  }
        cout<<"The used variable is "<<varName<<endl;
	}
      }

 
      // Only for TestTree
      if ( TString(key->GetName()) != "TestTree") continue;
      //if (!TString(key->GetName()).BeginsWith("TestTree")) continue;
      //cout<<key->GetClassName()<<endl;
      if( ! gROOT->GetClass(key->GetClassName())->InheritsFrom("TTree") ) continue;

      TTree* myTree =  (TTree*)key->ReadObj();
      cout << "--- Found tree: " << myTree->GetName() << endl;




      MethodInfo* info = new MethodInfo();
      info->varName = varName;
      info->methodName = TString("Variable");
      //info->methodName = TString(key->GetName());
      //cout<<"info methodName "<<info->methodName<<endl;
      info->methodTitle = TString("Variable");
      //info->methodTitle = TString(key->GetName());
      TH1F* testSig = new TH1F("testSig","testSig",Nbins, 0, maxX);
      TH1F* testBkg = new TH1F("testBkg","testBkg",Nbins, 0, maxX);
      //info->testSig = new TH1F("testSig","testSig",40, 0, 1.5);
      //info->testBkg = new TH1F("testBkg","testBkg",40, 0, 1.5);
      myTree->Draw("P_SovB>>testSig", "(classID==0)*weight","goff");
      myTree->Draw("P_SovB>>testBkg", "(classID==1)*weight","goff");
      info->sig = (TH1*)testSig->Clone();
      info->bgd = (TH1*)testBkg->Clone();
      info->origSigE = (TH1*)info->sig->Clone("origSigEffi");
      info->origBgdE = (TH1*)info->bgd->Clone("origBgdEffi");
      //info->origBgdE->Draw();
      //
      Double_t total_S(0);
      Double_t total_B(0);
      Double_t integral_S(0);
      Double_t integral_B(0);
      Double_t eff_S(0);
      Double_t eff_B(0);
      //For signal
      total_S = testSig->Integral(1,Nbins+1);//including overflow, if 0, then underflow
      total_B = testBkg->Integral(1,Nbins+1);
      fNSignal = total_S;
      fNBackground = total_B;
      for( int i(1); i<= Nbins; i++){
	integral_S = testSig->Integral(i, Nbins+1);
	integral_B = testBkg->Integral(i, Nbins+1);
	eff_S = integral_S/total_S;
	eff_B = integral_B/total_B;
	//cout<<"bin:"<<i<<" eff:"<<eff<<endl;
	info->origSigE->SetBinContent(i,eff_S);
	info->origBgdE->SetBinContent(i,eff_B);
      }
      //info->origSigE->Draw();
      //info->origBgdE->Draw("same");

      //myTree->Draw("P_SovB>>testSig", "classID==0 && P_SovB < 1.5");
      //  
      //   info->sig = dynamic_cast<TH1*>(titDir->Get( hname + "_S" ));
      //   info->bgd = dynamic_cast<TH1*>(titDir->Get( hname + "_B" ));
      //   info->origSigE = dynamic_cast<TH1*>(titDir->Get( hname + "_effS" ));
      //   info->origBgdE = dynamic_cast<TH1*>(titDir->Get( hname + "_effB" ));      
      //   if (info->origSigE==0 || info->origBgdE==0) { delete info; continue; }

      info->SetVariableHists();
      fInfoList->Add(info);
   }
   return;
}

void MineTMVA::StatDialogMVAEffs::DrawHistograms() 
{
   // counter variables
   Int_t countCanvas = 0;

   // define Canvas layout here!
   const Int_t width = 600;   // size of canvas
   Int_t signifColor = TColor::GetColor( "#00aa00" );

   TIter next(fInfoList);
   MethodInfo* info(0);
   while ( (info = (MethodInfo*)next()) ) {

      // create new canvas
      TCanvas *c = new TCanvas( Form("canvas%d", countCanvas+1), 
                                Form("Cut efficiencies for %s classifier",info->methodTitle.Data()), 
                                countCanvas*50+200, countCanvas*20, width, Int_t(width*0.78) ); 
      info->canvas = c;

      // draw grid
      c->SetGrid(1);
      c->SetTickx(0);
      c->SetTicky(0);

      TStyle *TMVAStyle = gROOT->GetStyle("Plain"); // our style is based on Plain
      TMVAStyle->SetLineStyleString( 5, "[32 22]" );
      TMVAStyle->SetLineStyleString( 6, "[12 22]" );
         
      c->SetTopMargin(.2);
      
      // and the signal purity and quality
      info->effpurS->SetTitle("Cut efficiencies and optimal cut value");
      if (info->methodTitle.Contains("Cuts")) {
         info->effpurS->GetXaxis()->SetTitle( "Signal Efficiency" );
      }
      else {
         info->effpurS->GetXaxis()->SetTitle( TString("Cut value applied on ") + info->methodTitle + " output" );
      }
      info->effpurS->GetYaxis()->SetTitle( "Efficiency (Purity)" );
      TMVA::TMVAGlob::SetFrameStyle( info->effpurS );

      c->SetTicks(0,0);
      c->SetRightMargin ( 2.0 );

      info->effpurS->SetMaximum(1.1);
      info->effpurS->Draw("histl");

      info->purS->Draw("samehistl");      

      // overlay signal and background histograms
      info->sigE->Draw("samehistl");
      info->bgdE->Draw("samehistl");

      info->sSig->SetLineColor( signifColor );
      info->sSig->Draw("samehistl");

      // redraw axes
      info->effpurS->Draw( "sameaxis" );

      // Draw legend               
      TLegend *legend1= new TLegend( c->GetLeftMargin(), 1 - c->GetTopMargin(), 
                                     c->GetLeftMargin() + 0.4, 1 - c->GetTopMargin() + 0.12 );
      legend1->SetFillStyle( 1 );
      legend1->AddEntry(info->sigE,"Signal efficiency","L");
      legend1->AddEntry(info->bgdE,"Background efficiency","L");
      legend1->Draw("same");
      legend1->SetBorderSize(1);
      legend1->SetMargin( 0.3 );

      TLegend *legend2= new TLegend( c->GetLeftMargin() + 0.4, 1 - c->GetTopMargin(), 
                                     1 - c->GetRightMargin(), 1 - c->GetTopMargin() + 0.12 );
      legend2->SetFillStyle( 1 );
      legend2->AddEntry(info->purS,"Signal purity","L");
      legend2->AddEntry(info->effpurS,"Signal efficiency*purity","L");
      legend2->AddEntry(info->sSig,GetLatexFormula().Data(),"L");
      legend2->Draw("same");
      legend2->SetBorderSize(1);
      legend2->SetMargin( 0.3 );
         
      // line to indicate maximum efficiency
      TLine* effline = new TLine( info->sSig->GetXaxis()->GetXmin(), 1, info->sSig->GetXaxis()->GetXmax(), 1 );
      effline->SetLineWidth( 1 );
      effline->SetLineColor( 1 );
      effline->Draw();

      // print comments
      TLatex tl;
      tl.SetNDC();
      tl.SetTextSize( 0.033 );
      Int_t maxbin = info->sSig->GetMaximumBin();
      info->line1 = tl.DrawLatex( 0.15, 0.23, Form("For %5.3f signal and %5.3f background", fNSignal, fNBackground));
      tl.DrawLatex( 0.15, 0.19, "events the maximum "+GetLatexFormula()+" is");

      if (info->maxSignificanceErr > 0) {
         info->line2 = tl.DrawLatex( 0.15, 0.15, Form("%5.2f +- %4.2f when cutting at %5.2f", 
                                                      info->maxSignificance, 
                                                      info->maxSignificanceErr, 
                                                      info->sSig->GetXaxis()->GetBinCenter(maxbin)) );
      }
      else {
         info->line2 = tl.DrawLatex( 0.15, 0.15, Form("%4.2f when cutting at %8.6f", 
                                                      info->maxSignificance, 
                                                      info->sSig->GetXaxis()->GetBinCenter(maxbin)) );
      }

      // add comment for Method cuts
      if (info->methodTitle.Contains("Cuts")){
         tl.DrawLatex( 0.13, 0.77, "Method Cuts provides a bundle of cut selections, each tuned to a");
         tl.DrawLatex(0.13, 0.74, "different signal efficiency. Shown is the purity for each cut selection.");
      }
      // save canvas to file
      c->Update();

      // Draw second axes
      info->rightAxis = new TGaxis(c->GetUxmax(), c->GetUymin(),
                                   c->GetUxmax(), c->GetUymax(),0,1.1*info->maxSignificance,510,"+L");
      info->rightAxis->SetLineColor ( signifColor );
      info->rightAxis->SetLabelColor( signifColor );
      info->rightAxis->SetTitleColor( signifColor );

      info->rightAxis->SetTitleSize( info->sSig->GetXaxis()->GetTitleSize() );
      info->rightAxis->SetTitle( "Significance" );
      info->rightAxis->Draw();

      c->Update();

      // switches
      const Bool_t Save_Images = kTRUE;

      if (Save_Images) {
	TMVA::TMVAGlob::imgconv( c, Form("%s/plots/mvaeffs_%s",dataset.Data(), info->methodTitle.Data()) ); 
      }
      countCanvas++;
   }
}

void MineTMVA::StatDialogMVAEffs::PrintResults( const MethodInfo* info )
{
   Int_t maxbin = info->sSig->GetMaximumBin();
   if (info->line1 !=0 )
      info->line1->SetText( 0.15, 0.23, Form("For %1.0f signal and %1.0f background", fNSignal, fNBackground));
   
   if (info->line2 !=0 ) {
      if (info->maxSignificanceErr > 0) {
         info->line2->SetText( 0.15, 0.15, Form("%3.2g +- %3.2g when cutting at %3.2g", 
                                                info->maxSignificance, 
                                                info->maxSignificanceErr, 
                                                info->sSig->GetXaxis()->GetBinCenter(maxbin)) );
      }
      else {
         info->line2->SetText( 0.15, 0.15, Form("%3.4f when cutting at %3.4f", info->maxSignificance, 
                                                info->sSig->GetXaxis()->GetBinCenter(maxbin)) );
      }

   }

   if (info->maxSignificanceErr <= 0) {
      TString opt = Form( "%%%is:  (%%9.8g,%%9.8g)    %%9.4f   %%10.6g  %%8.7g  %%8.7g %%8.4g %%8.4g", 
                          maxLenTitle );
      cout << "--- " 
           << Form( opt.Data(),
                    info->methodTitle.Data(), fNSignal, fNBackground, 
                    info->sSig->GetXaxis()->GetBinCenter( maxbin ),
                    info->maxSignificance,
                    info->origSigE->GetBinContent( maxbin )*fNSignal,   
                    info->origBgdE->GetBinContent( maxbin )*fNBackground,
                    info->origSigE->GetBinContent( maxbin ),
                    info->origBgdE->GetBinContent( maxbin ) )
           << endl;
   }
   else {
      TString opt = Form( "%%%is:  (%%9.8g,%%9.8g)    %%9.4f   (%%8.3g  +-%%6.3g)  %%8.7g  %%8.7g %%8.4g %%8.4g", 
                          maxLenTitle );
      cout << "--- " 
           << Form( opt.Data(),
                    info->methodTitle.Data(), fNSignal, fNBackground, 
                    info->sSig->GetXaxis()->GetBinCenter( maxbin ),
                    info->maxSignificance,
                    info->maxSignificanceErr,
                    info->origSigE->GetBinContent( maxbin )*fNSignal,   
                    info->origBgdE->GetBinContent( maxbin )*fNBackground,
                    info->origSigE->GetBinContent( maxbin ),
                    info->origBgdE->GetBinContent( maxbin ) )
           << endl;
   }
}





// this macro plots the signal and background efficiencies
// as a function of the MVA cut.
void mvaeffscxxMod(TString dataset, TString fin = "TMVA.root" , Int_t nbins = 40, Float_t maxX = 1., 
                   Bool_t useTMVAStyle = kTRUE, TString formula = "S/sqrt(S+B)" )
{
   TMVA::TMVAGlob::Initialize( useTMVAStyle );
   TFile* tmpfile = TFile::Open( fin );
   if (!tmpfile) {
     cout << "==> Abort TMVAGui, please verify filename" << endl;
     return;
   }
   if(tmpfile->GetListOfKeys()->GetEntries()<=0){
     cout << "==> Abort TMVAGui, please verify if dataset exist" << endl;
     return;
   }
   if( (dataset==""||dataset.IsWhitespace()) && (tmpfile->GetListOfKeys()->GetEntries()>=1)){
     TKey *key=(TKey*)tmpfile->GetListOfKeys()->At(0);
     dataset=key->GetName();
   }
   tmpfile->Close();
   delete tmpfile;
   //gROOT->Reset();
   //gStyle->SetScreenFactor(2); // if you have a large screen, select 1,2 or 1.4


   TGClient * graphicsClient = TGClient::Instance();
   if (graphicsClient == nullptr) {
      // When including mvaeffs in a stand-alone macro, the graphics subsystem
      // is not initialised and `TGClient::Instance` is a nullptr.
      graphicsClient = new TGClient();
   }
   MineTMVA::StatDialogMVAEffs* gGui = new MineTMVA::StatDialogMVAEffs(dataset, 
      graphicsClient->GetRoot(), 1000, 1000);


   TFile* file = TMVA::TMVAGlob::OpenFile( fin );
   //gGui->ReadHistograms(file);
   gGui->ReadVariableToHisto(file, nbins, maxX);
   gGui->SetFormula(formula);
   //gGui->UpdateSignificanceHists();
   gGui->MyVarSignificanceHists();
   gGui->DrawHistograms();
   gGui->RaiseDialog();   
}
