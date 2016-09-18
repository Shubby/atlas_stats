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
#include <string>



void plotsPapersSimple(std::string input_file="histopaperssimple.root",
		       std::string type="papers",bool PNG=true) {

  //bool PNG=true;

  
  std::string dir="fig";
  TFile *f;
  
  std::cout<<type<<std::endl;
  if (PNG)   std::cout<<"PNG"<<std::endl;
  else   std::cout<<"PDF"<<std::endl;

  f = new TFile(input_file.c_str());      
  if (type=="papers") {
    dir="fig";  
  } else if  (type=="conf") {
    dir="figconf";
  }
  //else if (type=="conf") {
  //  dir="figconf";
  //  f = new TFile("histoconfnote.root");      
  //}
  

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
  
  
  //  TCanvas *PUB = new TCanvas("PUB","PUB Plots",930,400);
  TCanvas *PUB = new TCanvas("PUB","PUB Plots",2);
  PUB->SetLeftMargin(0.15);
  PUB->SetBottomMargin(0.2);
  //PUB->SetRightMargin(0.1);
  PUB->cd();
  PUB->SetGridy(1);
  PUB->SetTickx(0);
  PUB->SetTicky(0);
/* number of papers per year */
  TH1F *Hpapers = (TH1F*)f->Get("Hpapers_All");
  Hpapers->Draw();
  Hpapers->GetXaxis()->SetLabelSize(0.05);
  Hpapers->GetXaxis()->SetTitleSize(0.05);
  Hpapers->GetYaxis()->SetLabelSize(0.05);
  Hpapers->GetYaxis()->SetTitleSize(0.05);
  Hpapers->GetYaxis()->SetTitleOffset(1.5);
  float max_papers= Hpapers->GetMaximum();
  Hpapers->GetYaxis()->SetRangeUser(0,max_papers*1.2);

  PUB->Print("fig/PapersByYear.png");

  
/* number of papers per year and per Run */
  THStack *Hpapers_run = new THStack("Hpapers_run","Papers per year per Run");  
  TH1F *Hpapers_1 = (TH1F*)f->Get("Hpapers_Run1");
  Hpapers_1->SetFillColor(8);
  Hpapers_run->Add(Hpapers_1);
  TH1F *Hpapers_2 = (TH1F*)f->Get("Hpapers_Run2");
  Hpapers_2->SetFillColor(29);
  Hpapers_run->Add(Hpapers_2);
  Hpapers_run->Draw();
  TLegend *legend = new TLegend(0.85,0.74,0.95,0.95);
  legend->AddEntry(Hpapers_1,"Run1","f");
  legend->AddEntry(Hpapers_2,"Run2","f");
  legend->Draw();
  PUB->Print("fig/PapersByYearperRun.png");

  
/* number of papers per physics group */
  //HIN_PREP->Add(H2013,HIN_PREP);
  TH1F *HIN_PREP = (TH1F*)f->Get("HIN_PREP");
  HIN_PREP->SetTitle("in preparation");
  TH1F *HALL = (TH1F*)f->Get("HALL");
  HALL->SetTitle("ALL");
  HALL->GetXaxis()->SetLabelSize(0.05);
  HALL->GetXaxis()->SetTitleSize(0.05);
  HALL->GetYaxis()->SetLabelSize(0.05);
  HALL->GetYaxis()->SetTitleSize(0.05);
  HALL->GetYaxis()->SetTitle("N.Papers");
  float max_all= HALL->GetMaximum();
  HALL->GetYaxis()->SetRangeUser(0,max_all*1.2);
  HALL->Draw();
  HIN_PREP->SetFillColor(kGray);
  HIN_PREP->Draw("same");
  PUB->BuildLegend(.2,.75,.6,.9);
  PUB->Print("fig/PapersByGroup.png");

  /* number of papers per physics group in 2016*/
  
  TH1F *H2016 = (TH1F*)f->Get("H2016");
  H2016->SetTitle("2016");
  H2016->GetXaxis()->SetLabelSize(0.05);
  H2016->GetXaxis()->SetTitleSize(0.05);
  H2016->GetYaxis()->SetLabelSize(0.05);
  H2016->GetYaxis()->SetTitleSize(0.05);
  H2016->GetYaxis()->SetTitle("N.Papers");
  float max_2016= H2016->GetMaximum();
  H2016->GetYaxis()->SetRangeUser(0,max_2016*1.2);
  H2016->Draw();
  PUB->Print("fig/PapersByGroup2016.png");

  /* fraction of IT paper editors by group */
  TH1F *HEdItalyALL = 0;
  f->GetObject("HEdItalyALL",HEdItalyALL);
  TH1F *HDALL = 0;
  f->GetObject("HDALL",HDALL);
  TH1F* N=(TH1F*)HEdItalyALL->Clone();
  TH1F* R=(TH1F*)HEdItalyALL->Clone();
  TH1F* D=(TH1F*)HDALL->Clone();
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
 R->GetYaxis()->SetTitleOffset(1.5);
 R->Draw("e");
 PUB->Print("fig/ItEdByGroup.png");

  /* fraction of IT paper editors by group in last year */
 TH1F *HEdItaly2016 = 0;
  f->GetObject("HEdItaly2016",HEdItaly2016);
  TH1F *HD2016 = 0;
  f->GetObject("HD2016",HD2016);
  TH1F* Na=(TH1F*)HEdItaly2016->Clone();
  TH1F* Ra=(TH1F*)HEdItaly2016->Clone();
  TH1F* Da=(TH1F*)HD2016->Clone();
  Na->Sumw2();
  Da->Sumw2();
 Ra->Divide(Na,Da,1,1,"B");
 Ra->SetTitle("Fraction of Italian Editors in 2016");
 Ra->GetXaxis()->SetLabelSize(0.05);
 Ra->GetXaxis()->SetTitleSize(0.05);
 Ra->GetYaxis()->SetLabelSize(0.05);
 Ra->GetYaxis()->SetTitleSize(0.05);
 Ra->GetYaxis()->SetTitle("fraction");
 MyStyle->SetOptStat(0);
 Ra->SetMarkerSize(1.2);
 Ra->GetYaxis()->SetTitleOffset(1.5);
 Ra->Draw("e");
 PUB->Print("fig/ItEdByGroup2016.png");

/* fraction of IT paper EB members by group */
 TH1F *HEbItalyALL = (TH1F*)(f->Get("HEbItalyALL"));
  TH1F *HDBALL = (TH1F*)(f->Get("HDBALL"));
  TH1F* Nb=(TH1F*)HEbItalyALL->Clone();
  TH1F* Rb=(TH1F*)HEbItalyALL->Clone();
  TH1F* Db=(TH1F*)HDBALL->Clone();
 Nb->Sumw2();
 Db->Sumw2();
 Rb->Divide(Nb,Db,1,1,"B");
 Rb->SetTitle("Fraction of Italian EB members");
 Rb->GetXaxis()->SetLabelSize(0.05);
 Rb->GetXaxis()->SetTitleSize(0.05);
 Rb->GetYaxis()->SetLabelSize(0.05);
 Rb->GetYaxis()->SetTitleSize(0.05);
 Rb->GetYaxis()->SetTitle("fraction");
 float max_rb= Rb->GetMaximum();
 Rb->GetYaxis()->SetRangeUser(0,max_rb*1.2);
 MyStyle->SetOptStat(0);
 Rb->SetMarkerSize(1.2);
 Rb->GetYaxis()->SetTitleOffset(1.5);
 Rb->Draw("e");
 PUB->Print("fig/ItEBmembersByGroup.png");

 TH1F* HBALL = (TH1F*)(f->Get("HBALL"));
/* % of EB members */
cout << "**** Fraction of EB members per country"<<endl;
  for(int i=1; i<HBALL->GetNbinsX()+1; i++) {
    cout << HBALL->GetXaxis()->GetBinLabel(i)<<" "<<(int)(1000*(HBALL->GetBinContent(i)/HBALL->GetEntries()))*0.1<<"%" <<endl;
  }
  TH1F* HBChairALL = (TH1F*)(f->Get("HBChairALL"));
/* % of EB members */
cout << "**** Fraction of EB Chairs per country"<<endl;
  for(int i=1; i<HBChairALL->GetNbinsX()+1; i++) {
    cout << HBChairALL->GetXaxis()->GetBinLabel(i)<<" "<<(int)(1000*(HBChairALL->GetBinContent(i)/HBChairALL->GetEntries()))*0.1<<"%" <<endl;
  }

/* % of authors */
 TH1F* HCALL = (TH1F*)(f->Get("HCALL"));
cout << "**** Fraction of paper editors  per country"<<endl;
  for(int i=1; i<HCALL->GetNbinsX()+1; i++) {
    cout << HCALL->GetXaxis()->GetBinLabel(i)<<" "<<(int)(1000*(HCALL->GetBinContent(i)/HCALL->GetEntries()))*0.1<<"%" <<endl;
  }
  
  
  TH1F* HCPALL = (TH1F*)(f->Get("HCPALL"));
  /* % of edited papers */
  cout << "**** Fraction of paper edited by a country"<<endl;
  for(int i=1; i<HCPALL->GetNbinsX()+1; i++) {
    cout << HCPALL->GetXaxis()->GetBinLabel(i)<<" "<<(int)(1000*(HCPALL->GetBinContent(i)/Hpapers->GetEntries()))*0.1<<"%" <<endl;
  }
}
