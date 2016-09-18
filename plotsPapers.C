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
using namespace std;



void plotsPapers(std::string type="papers",bool PNG=true) {

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
  }
  

  TStyle *MyStyle= new TStyle("MyStyle","my style");  
  MyStyle->SetOptFit(0);
  MyStyle->SetPalette(1);
  if (PNG) MyStyle->SetOptStat("e");
  MyStyle->SetMarkerStyle(20);
  MyStyle->SetLineWidth(2);
  MyStyle->SetMarkerSize(0.8);
  MyStyle->SetMarkerColor(4);
  MyStyle->SetOptTitle(1);
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
  
/* number of papers per year */
  Hpapers->Draw();
  Hpapers->GetXaxis()->SetLabelSize(0.05);
  Hpapers->GetXaxis()->SetTitleSize(0.05);
  Hpapers->GetYaxis()->SetLabelSize(0.05);
  Hpapers->GetYaxis()->SetTitleSize(0.05);
  Hpapers->GetYaxis()->SetRangeUser(0,140);
  
  PUB->Print("PapersByYear.png");

/* number of papers per physics group */
  //HIN_PREP->Add(H2013,HIN_PREP);
  HIN_PREP->SetTitle("in preparation");
  //  TH1F* HALL=(TH1F*)f->Get("Hpapers");
  HALL->SetTitle("ALL");
  HALL->GetXaxis()->SetLabelSize(0.05);
  HALL->GetXaxis()->SetTitleSize(0.05);
  HALL->GetYaxis()->SetLabelSize(0.05);
  HALL->GetYaxis()->SetTitleSize(0.05);
  HALL->GetYaxis()->SetTitle("N.Papers");
  HALL->Draw();
  HIN_PREP->SetFillColor(kGray);
  HIN_PREP->Draw("same");
  PUB->BuildLegend(.4,.57,.65,.78);
  PUB->Print("PapersByGroup.png");

/* fraction of IT paper editors by group */
 TH1F* N=HEdItalyALL->Clone();
 TH1F* R=HEdItalyALL->Clone();
 TH1F* D=HDALL->Clone();
 N->Sumw2();
 D->Sumw2();
 R->Divide(N,D,1,1,"B");
 R->SetTitle("Fraction of Italian Editors");
 R->GetXaxis()->SetLabelSize(0.05);
 R->GetXaxis()->SetTitleSize(0.05);
 R->GetYaxis()->SetLabelSize(0.05);
 R->GetYaxis()->SetTitleSize(0.05);
 R->GetYaxis()->SetTitle("fraction");
 MyStyle->SetOptStat(0);
 R->SetMarkerSize(1.2);
 R->Draw("e");
 PUB->Print("ItEdByGroup.png");


/* % of EB members */
cout << "**** Fraction of EB members per country"<<endl;
  for(int i=0; i<HBALL->GetNbinsX(); i++) {
    cout << HBALL->GetXaxis()->GetBinLabel(i)<<" "<<(int)(1000*(HBALL->GetBinContent(i)/HBALL->GetEntries()))*0.1<<"%" <<endl;
  }

/* % of authors */
cout << "**** Fraction of paper editors  per country"<<endl;
  for(int i=0; i<HCALL->GetNbinsX(); i++) {
    cout << HCALL->GetXaxis()->GetBinLabel(i)<<" "<<(int)(1000*(HCALL->GetBinContent(i)/HCALL->GetEntries()))*0.1<<"%" <<endl;
  }
}
