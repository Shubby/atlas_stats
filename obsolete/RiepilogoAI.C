#define RiepilogoAI_cxx
#include "RiepilogoAI.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

#include <map>
#include <TDirectory.h>
#include <iostream>
#include <TH1F.h>

void RiepilogoAI::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L RiepilogoAI.C
//      Root > RiepilogoAI t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   TFile fout("histoSummaryAI.root","recreate");  
   
   TStyle *MyStyle= new TStyle("MyStyle","my style");  
   MyStyle->SetOptFit(0);
   MyStyle->SetPalette(1);
   MyStyle->SetOptStat("e");
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



   int fillCol=7;
   
   TH1F* HPOSTYEAR = new TH1F("HPOSTYEAR","Anni POST Doc",10,1,11);

   HPOSTYEAR->GetYaxis()->SetTitle("N. Persone");
   HPOSTYEAR->GetXaxis()->SetTitle("Anni da PhD");
   HPOSTYEAR->SetFillColor(fillCol);

   int NTri=0,NLaurea=0,NPhd=0,NPostD=0,NStaff=0;
   int NATri=0,NALaurea=0,NAPhd=0,NAPostD=0,NAStaff=0;
   int NDTri=0,NDLaurea=0,NDPhd=0,NDPostD=0,NDStaff=0;
   int NCTri=0,NCLaurea=0,NCPhd=0,NCPostD=0,NCStaff=0;
   

   std::map<std::string,int> MapAnalisi;
   int iana=0;
   Long64_t nentries = fChain->GetEntriesFast();
   
   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
     Long64_t ientry = LoadTree(jentry);
     if (ientry < 0) break;
     nb = fChain->GetEntry(jentry);   nbytes += nb;
     // if (Cut(ientry) < 0) continue;      
     if (MapAnalisi.find(*Analisi)==MapAnalisi.end()&&*Analisi!="") {MapAnalisi[*Analisi]=iana++;}
   }
   
   int Nanalisi = MapAnalisi.size();
   TH1F* HAnalisiALL = new TH1F("HAnalisiALL","Analisi ALL",Nanalisi,0,Nanalisi);
   TH1F* HAnalisiTRI = new TH1F("HAnalisiTRI","Analisi TRI",Nanalisi,0,Nanalisi);
   TH1F* HAnalisiLAU = new TH1F("HAnalisiLAU","Analisi LAU",Nanalisi,0,Nanalisi);
   TH1F* HAnalisiPHD = new TH1F("HAnalisiPHD","Analisi PHD",Nanalisi,0,Nanalisi);
   TH1F* HAnalisiPOD = new TH1F("HAnalisiPOD","Analisi POD",Nanalisi,0,Nanalisi);
   TH1F* HAnalisiSTF = new TH1F("HAnalisiSTF","Analisi STF",Nanalisi,0,Nanalisi);

   HAnalisiALL->GetYaxis()->SetTitle( "N. Persone");
   HAnalisiTRI->GetYaxis()->SetTitle( "N. Persone");
   HAnalisiLAU->GetYaxis()->SetTitle( "N. Persone");
   HAnalisiPHD->GetYaxis()->SetTitle( "N. Persone");
   HAnalisiPOD->GetYaxis()->SetTitle( "N. Persone");
   HAnalisiSTF->GetYaxis()->SetTitle( "N. Persone");
   



   HAnalisiALL->SetFillColor(fillCol);
   HAnalisiTRI->SetFillColor(fillCol);
   HAnalisiLAU->SetFillColor(fillCol);
   HAnalisiPHD->SetFillColor(fillCol);
   HAnalisiPOD->SetFillColor(fillCol);
   HAnalisiSTF->SetFillColor(fillCol);
   
   
   std::map<std::string,int>::iterator  iteMap=MapAnalisi.begin();
   for(;iteMap!=MapAnalisi.end();iteMap++){
     HAnalisiALL->GetXaxis()->SetBinLabel(iteMap->second+1,iteMap->first.c_str());
     HAnalisiTRI->GetXaxis()->SetBinLabel(iteMap->second+1,iteMap->first.c_str());
     HAnalisiLAU->GetXaxis()->SetBinLabel(iteMap->second+1,iteMap->first.c_str());
     HAnalisiPHD->GetXaxis()->SetBinLabel(iteMap->second+1,iteMap->first.c_str());
     HAnalisiPOD->GetXaxis()->SetBinLabel(iteMap->second+1,iteMap->first.c_str());
     HAnalisiSTF->GetXaxis()->SetBinLabel(iteMap->second+1,iteMap->first.c_str());
   }

   
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
     Long64_t ientry = LoadTree(jentry);
     if (ientry < 0) break;
     nb = fChain->GetEntry(jentry);   nbytes += nb;
     // if (Cut(ientry) < 0) continue;      
     
     int anno = atoi(Anno->c_str());
     //std::cout<<anno<<" "<<*Tipo<<std::endl;
     bool isPOSTDOC=false;
     if (*Tipo!="TRIENNALE"&&*Tipo!="LAUREA"&&*Tipo!="PHD"&&*Tipo!="STAFF") isPOSTDOC=true;
     
     std::cout<<"ALL "<<anno<<" "<<*Tipo<<std::endl;
     if (isPOSTDOC) std::cout<<anno<<" "<<*Tipo<<std::endl;
     if (isPOSTDOC) HPOSTYEAR->Fill(anno);
     
     if (*Analisi!="")                     HAnalisiALL->Fill(MapAnalisi[*Analisi]);
     if (*Tipo=="TRIENNALE"&&*Analisi!="") HAnalisiTRI->Fill(MapAnalisi[*Analisi]);
     if (*Tipo=="LAUREA"&&*Analisi!="")    HAnalisiLAU->Fill(MapAnalisi[*Analisi]);
     if (*Tipo=="PHD"&&*Analisi!="")       HAnalisiPHD->Fill(MapAnalisi[*Analisi]);
     if (isPOSTDOC&&*Analisi!="")          HAnalisiPOD->Fill(MapAnalisi[*Analisi]);
     if (*Tipo=="STAFF"&&*Analisi!="")     HAnalisiSTF->Fill(MapAnalisi[*Analisi]);
     
     
     if (*Tipo=="TRIENNALE") NTri++;
     if (*Tipo=="LAUREA") NLaurea++;
     if (*Tipo=="PHD") NPhd++;
     if (isPOSTDOC) NPostD++;
     if (*Tipo=="STAFF") NStaff++;
     
     if (*Tipo=="TRIENNALE"&&*Analisi!="") NATri++;
     if (*Tipo=="LAUREA"&&*Analisi!="") NALaurea++;
     if (*Tipo=="PHD"&&*Analisi!="") NAPhd++;
     if (isPOSTDOC&&*Analisi!="") NAPostD++;
     if (*Tipo=="STAFF"&&*Analisi!="") NAStaff++;
     
     if (*Tipo=="TRIENNALE"&&*Detector!="") NDTri++;
     if (*Tipo=="LAUREA"&&*Detector!="") NDLaurea++;
     if (*Tipo=="PHD"&&*Detector!="") NDPhd++;
     if (isPOSTDOC&&*Detector!="") NDPostD++;
     if (*Tipo=="STAFF"&&*Detector!="") NDStaff++;
     
     if (*Tipo=="TRIENNALE"&&*Computing!="") NCTri++;
     if (*Tipo=="LAUREA"&&*Computing!="") NCLaurea++;
     if (*Tipo=="PHD"&&*Computing!="") NCPhd++;
     if (isPOSTDOC&&*Computing!="") NCPostD++;
     if (*Tipo=="STAFF"&&*Computing!="") NCStaff++;
     
     
   }
   
   
   std::cout<<"*********Totale************"<<std::endl;
   std::cout<<"Triennale "<<NTri<<std::endl;
   std::cout<<"Laurea    "<<NLaurea<<std::endl;
   std::cout<<"Phd       "<<NPhd<<std::endl;
   std::cout<<"Post Doc  "<<NPostD<<std::endl;
   std::cout<<"Staff     "<<NStaff<<std::endl;
   std::cout<<"*********Analisi************ "<<std::endl;
   std::cout<<"Triennale "<<NATri<<std::endl;
   std::cout<<"Laurea    "<<NALaurea<<std::endl;
   std::cout<<"Phd       "<<NAPhd<<std::endl;
   std::cout<<"Post Doc  "<<NAPostD<<std::endl;
   std::cout<<"Staff     "<<NAStaff<<std::endl;
   std::cout<<"*********Detector************"<<std::endl;
   std::cout<<"Triennale "<<NDTri<<std::endl;
   std::cout<<"Laurea    "<<NDLaurea<<std::endl;
   std::cout<<"Phd       "<<NDPhd<<std::endl;
   std::cout<<"Post Doc  "<<NDPostD<<std::endl;
   std::cout<<"Staff     "<<NDStaff<<std::endl;
   std::cout<<"*********Computing************"<<std::endl;
   std::cout<<"Triennale "<<NCTri<<std::endl;
   std::cout<<"Laurea    "<<NCLaurea<<std::endl;
   std::cout<<"Phd       "<<NCPhd<<std::endl;
   std::cout<<"Post Doc  "<<NCPostD<<std::endl;
   std::cout<<"Staff     "<<NCStaff<<std::endl;
   std::cout<<"***************************"<<std::endl;
   std::cout<<"***************************"<<std::endl;
   std::cout<<"***************************"<<std::endl;

   
   
   for (int i=0;i<HPOSTYEAR->GetXaxis()->GetNbins()+1;i++){
     std::cout<<i+1<<" "<<HPOSTYEAR->GetBinContent(i+1)<<std::endl;
   }





   TObject* histo=0;
   TDirectory* hDir = gDirectory;
   TList* hList = (TList*)hDir->GetList();
   TIter hIter(hList);
   while((histo = (TObject*)hIter.Next())) { 
     histo->Write();
   }
   fout.Close();
   

}

