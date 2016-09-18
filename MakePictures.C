#include <cstring>
#include <iostream>
#include <stdio.h>
#include <TH1.h>
#include <TF1.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "TFile.h"
#include "TLegend.h"
#include "THStack.h"
#include "TROOT.h"
#include <vector>
#include <map>
#include "TKey.h"
#include "TGaxis.h"
#include "TFrame.h"




void MakePictures(std::string type="papers",bool PNG=true) {

  //bool PNG=true;

  
  std::string dir="fig";
  TFile *f;
  
  std::cout<<type<<std::endl;
  if (PNG)   std::cout<<"PNG"<<std::endl;
  else   std::cout<<"PDF"<<std::endl;

  if (type=="papers") {
    dir="fig";
    f = new TFile("histopapers.root");      
  } else if (type=="conf") {
    dir="figconf";
    f = new TFile("histoconfnote.root");      
  }  else if (type=="talks") {
    dir="figtalks";
    f = new TFile("histotalks.root");      
  }
  
  /*Get List of histograms*/
  std::vector<TH1F*> vH;
  TKey *key;
  TIter next(f->GetListOfKeys());
  while ((key = (TKey *) next())) {    
    std::string n = key->GetClassName();
    std::string s = key->GetName();
    //std::cout<<n<<" "<<s<<std::endl;
    if (n=="TH1F"){
      TH1F* h=(TH1F*)f->Get(s.c_str());
      vH.push_back(h);      
    }
  }
  


  TStyle *MyStyle= new TStyle("MyStyle","my style");  
  MyStyle->SetOptFit(0);
  MyStyle->SetPalette(1);
  if (PNG) MyStyle->SetOptStat("e");
  MyStyle->SetMarkerStyle(20);
  MyStyle->SetLineWidth(2);
  MyStyle->SetMarkerSize(0.8);
  MyStyle->SetMarkerColor(4);
  //MyStyle->SetOptTitle(1);
  MyStyle->SetOptTitle(0);
  Int_t icol=0;
  MyStyle->SetFrameBorderMode(icol);
  MyStyle->SetCanvasBorderMode(icol);
  MyStyle->SetPadBorderMode(icol);
  MyStyle->SetPadColor(icol);
  MyStyle->SetCanvasColor(icol);
  MyStyle->SetStatColor(icol);
  MyStyle->cd();
  
  
  TCanvas *PUB = new TCanvas("PUB","PUB Plots",930,400);
  PUB->SetLeftMargin(0.1);
  PUB->SetBottomMargin(0.2);
  //PUB->SetRightMargin(0.1);
  PUB->cd();
  PUB->SetGridy(1);
  PUB->SetTickx(0);
  PUB->SetTicky(0);
  
  
  std::vector<TH1F*>::iterator iteVec=vH.begin();
  int npub=vH.size();
  int ipub=0;    
  for (;iteVec!=vH.end();iteVec++) {
    ipub++;
    PUB->Clear();
    (*iteVec)->Draw();
    std::string name=(*iteVec)->GetName();
    if (PNG) PUB->Print( (dir+"/"+name+".png").c_str() );
    else {
      if (ipub==1) PUB->Print((dir+"/ATLASpubblication.pdf(").c_str());
      if (ipub==npub) PUB->Print((dir+"/ATLASpubblication.pdf)").c_str());
      else  PUB->Print((dir+"/ATLASpubblication.pdf").c_str());
    }
  }
  
}
