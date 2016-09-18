#define AtlasPapers_cxx
#include "AtlasPapers.h"
#include <TH2.h>
#include <TStyle.h>
#include <TAxis.h>
#include <TCanvas.h>
#include <map>
#include <TDirectory.h>
#include <iostream>

void AtlasPapers::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L AtlasPapers.C
//      Root > AtlasPapers t
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

   TFile fout("histopapers.root","recreate");  

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


   std::map<std::string,bool> GoodCountries;
   GoodCountries["United Kingdom"]=true;
   GoodCountries["Italy"]=true;
   GoodCountries["United States of America"]=true;
   GoodCountries["Germany"]=true;
   GoodCountries["France"]=true;
   

   Long64_t narticoli = fChain->GetEntriesFast();

   int fillCol=7;//histogram fill color

   std::map<std::string,int> EdCountries,Groups,EBCountries;
   int igruppo=0,ipaese=0,jpaese=0;
   
   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<narticoli;jentry++) {
     Long64_t ientry = LoadTree(jentry);
     if (ientry < 0) break;
     nb = fChain->GetEntry(jentry);   nbytes += nb;
     
     std::string gruppo=Group->at(0);
     if(gruppo=="EGAM") gruppo="PERF"; 
     if(gruppo=="DAPR") gruppo="PERF"; 
     if(gruppo=="IDTR") gruppo="PERF"; 
     if(gruppo=="JETM") gruppo="PERF"; 
     if(gruppo=="FTAG") gruppo="PERF"; 
     if(gruppo=="TRIG") gruppo="DET";
     if(gruppo=="IDET") gruppo="DET"; 
     if(gruppo=="MUON") gruppo="DET"; 
     if(gruppo=="LARG") gruppo="DET"; 
     if(gruppo=="TCAL") gruppo="DET"; 
     if(gruppo=="TAUP") gruppo="DET"; 
     if(gruppo=="DET") continue; 
     if(gruppo=="SOFT") continue; 
    
     if (Groups.find(gruppo)==Groups.end()) {Groups[gruppo]=igruppo;igruppo++;}     
     


     for (int ied=0;ied<NEditors;ied++) {
       std::string paese=EdCountry->at(ied);
       if (!GoodCountries[paese]) paese="ALTRO"; 
       if (EdCountries.find(paese)==EdCountries.end())  {EdCountries[paese]=ipaese;ipaese++;}       
     }
     for (int ied=0;ied<NEBoard;ied++) {
       std::string paese=EBCountry->at(ied);
       if (!GoodCountries[paese]) paese="ALTRO"; 
       if (EBCountries.find(paese)==EBCountries.end())  {EBCountries[paese]=jpaese;jpaese++;}
     }
     
   }
   
   std::string anno[]={"2010","2011","2012","2013","2014","IN_PREP","ALL"};
   std::map<std::string,int>  mapYear;
   mapYear["2010"]=0.5;
   mapYear["2011"]=1.5;
   mapYear["2012"]=2.5;
   mapYear["2013"]=3.5;
   mapYear["2014"]=4.5;
   mapYear["IN_PREP"]=5.5;
   int nyears=7;
   
   TH1F *Hpapers = new TH1F("Hpapers","N. Papers vs year",nyears-1,0,nyears-1);//escluso ALL
   Hpapers->GetYaxis()->SetTitle( "N.Papers" );            
   Hpapers->SetFillColor(fillCol);            
   for (int iy=0;iy<nyears-1;iy++) {
     Hpapers->GetXaxis()->SetBinLabel(iy+1,anno[iy].c_str());            
   }
   

   std::map<std::string,TH1F*> H1D;

   int ngroups=Groups.size();
   int ncountries=EdCountries.size();
   int nbcountries=EBCountries.size();
   


   for (int ia=0;ia<nyears;ia++) {
     
     /** number of papers per year as a function of physgroup **/
     std::string hname="H"+anno[ia];
     std::string title="N.papers_ year_"+anno[ia];
     H1D[hname] = new TH1F(hname.c_str(),title.c_str(),ngroups,0,ngroups);
     //H1D[hname]->GetYaxis()->SetTitle( ("N.Papers year("+anno[ia]+")").c_str() );            
     H1D[hname]->GetYaxis()->SetTitle("N.Papers");            
     H1D[hname]->SetFillColor(fillCol);            
     //H1D[hname]->GetXaxis()->LabelsOption("v");

     /** number of papers per year as a function of physgroup **/
     hname="HD"+anno[ia];
     title="N.editors_ year_"+anno[ia];
     H1D[hname] = new TH1F(hname.c_str(),title.c_str(),ngroups,0,ngroups);
     H1D[hname]->GetYaxis()->SetTitle("N.Editors");            
     //H1D[hname]->GetYaxis()->SetTitle( ("N.Editors year("+anno[ia]+")").c_str() );            
     H1D[hname]->SetFillColor(fillCol);            
     //H1D[hname]->GetXaxis()->LabelsOption("v");
     
     /** number of editors per year as a function of country **/
     hname="HC"+anno[ia];
     title="N.editors_ year_"+anno[ia];
     H1D[hname] = new TH1F(hname.c_str(),title.c_str(),ncountries,0,ncountries);
     H1D[hname]->GetYaxis()->SetTitle( "N.Editors");            
     //H1D[hname]->GetYaxis()->SetTitle( ("N.Editors year("+anno[ia]+")").c_str() );            
     H1D[hname]->SetFillColor(fillCol);            
     //H1D[hname]->GetXaxis()->LabelsOption("v");
     
     std::map<std::string,int>::iterator iteCMap=EdCountries.begin();
     for (;iteCMap!=EdCountries.end();iteCMap++) {
       H1D[hname]->GetXaxis()->SetBinLabel(iteCMap->second+1,iteCMap->first.c_str());            
     }
     /** number of editor board members per year as a function of country **/
     hname="HB"+anno[ia];
     title="N.EB_ year_"+anno[ia];
     H1D[hname] = new TH1F(hname.c_str(),title.c_str(),nbcountries,0,nbcountries);
     H1D[hname]->GetYaxis()->SetTitle("N.EB members");            
     //H1D[hname]->GetYaxis()->SetTitle( ("N.EB members year("+anno[ia]+")").c_str() );            
     H1D[hname]->SetFillColor(fillCol);            
     //H1D[hname]->GetXaxis()->LabelsOption("v");
     
     std::map<std::string,int>::iterator iteCMap2=EBCountries.begin();
     for (;iteCMap2!=EBCountries.end();iteCMap2++) {
       H1D[hname]->GetXaxis()->SetBinLabel(iteCMap2->second+1,iteCMap2->first.c_str());            
     }
     
     /** number of editors/ number of authors per year as a function of country **/
     hname="HC"+anno[ia]+"_norm";
     title="N.NormEditors_ year_"+anno[ia];
     H1D[hname] = new TH1F(hname.c_str(),title.c_str(),ncountries,0,ncountries);
     H1D[hname]->GetYaxis()->SetTitle( ("N.Editors/N.Authors year("+anno[ia]+")").c_str() );            
     H1D[hname]->SetFillColor(fillCol);            
     //H1D[hname]->GetXaxis()->LabelsOption("v");
     
     iteCMap=EdCountries.begin();
     for (;iteCMap!=EdCountries.end();iteCMap++) {
       H1D[hname]->GetXaxis()->SetBinLabel(iteCMap->second+1,iteCMap->first.c_str());            
     }




     /** number of edited papers per year as a function of country **/
     hname="HCP"+anno[ia];
     title="N.papers_ year_"+anno[ia];
     H1D[hname] = new TH1F(hname.c_str(),title.c_str(),ncountries,0,ncountries);
     H1D[hname]->GetYaxis()->SetTitle( ("N.Papers year("+anno[ia]+")").c_str() );            
     H1D[hname]->SetFillColor(fillCol);            
     //H1D[hname]->GetXaxis()->LabelsOption("v");
     
     iteCMap=EdCountries.begin();
     for (;iteCMap!=EdCountries.end();iteCMap++) {
       H1D[hname]->GetXaxis()->SetBinLabel(iteCMap->second+1,iteCMap->first.c_str());            
     }
     
     /** number of edited papers/ number of authors per year as a function of country **/
     hname="HCP"+anno[ia]+"_norm";
     title="N.NormPapers_ year_"+anno[ia];
     H1D[hname] = new TH1F(hname.c_str(),title.c_str(),ncountries,0,ncountries);
     H1D[hname]->GetYaxis()->SetTitle( ("N.Papers/N.Authors year("+anno[ia]+")").c_str() );            
     H1D[hname]->SetFillColor(fillCol);            
     //H1D[hname]->GetXaxis()->LabelsOption("v");
     
     iteCMap=EdCountries.begin();
     for (;iteCMap!=EdCountries.end();iteCMap++) {
       H1D[hname]->GetXaxis()->SetBinLabel(iteCMap->second+1,iteCMap->first.c_str());            
     }


     
     std::map<std::string,int>::iterator iteMap=Groups.begin();     
     for (;iteMap!=Groups.end();iteMap++) {       
       
       hname="H"+anno[ia];
       H1D[hname]->GetXaxis()->SetBinLabel(iteMap->second+1,iteMap->first.c_str());            
       
       hname="HD"+anno[ia];
       H1D[hname]->GetXaxis()->SetBinLabel(iteMap->second+1,iteMap->first.c_str());            
       
       /** number of editors per physgroup and year as a function of country **/
       hname="HEd"+iteMap->first+anno[ia];
       title="N.editors_"+iteMap->first+"_ year_"+anno[ia];;
       H1D[hname] = new TH1F(hname.c_str(),title.c_str(),ncountries,0,ncountries);
       H1D[hname]->SetFillColor(fillCol);            
       
       iteCMap=EdCountries.begin();
       for (;iteCMap!=EdCountries.end();iteCMap++){ 
	 H1D[hname]->GetXaxis()->SetBinLabel(iteCMap->second+1,iteCMap->first.c_str());            
       }
       H1D[hname]->GetYaxis()->SetTitle( "N.Editors");            
       //H1D[hname]->GetYaxis()->SetTitle( ("N.Editors year("+anno[ia]+")").c_str() );            
       
       /** number of editors/number of authors per physgroup and year as a function of country **/
       hname="HEd"+iteMap->first+anno[ia]+"_norm";
       title="N.normeditors_"+iteMap->first+"_ year_"+anno[ia];;
       H1D[hname] = new TH1F(hname.c_str(),title.c_str(),ncountries,0,ncountries);
       H1D[hname]->SetFillColor(fillCol);            
       
       iteCMap=EdCountries.begin();
       for (;iteCMap!=EdCountries.end();iteCMap++){ 
	 H1D[hname]->GetXaxis()->SetBinLabel(iteCMap->second+1,iteCMap->first.c_str());            
       }
       H1D[hname]->GetYaxis()->SetTitle( ("N.Editors/N.Authors year("+anno[ia]+")").c_str() );            



       /** number of edited papers per physgroup and year as a function of country **/
       hname="HEP"+iteMap->first+anno[ia];
       title="N.papers_"+iteMap->first+"_ year_"+anno[ia];;
       H1D[hname] = new TH1F(hname.c_str(),title.c_str(),ncountries,0,ncountries);
       H1D[hname]->SetFillColor(fillCol);            
       
       iteCMap=EdCountries.begin();
       for (;iteCMap!=EdCountries.end();iteCMap++){ 
	 H1D[hname]->GetXaxis()->SetBinLabel(iteCMap->second+1,iteCMap->first.c_str());            
       }
       H1D[hname]->GetYaxis()->SetTitle( ("N.Papers year("+anno[ia]+")").c_str() );            
       
       /** number of edited papers /number of authors per physgroup and year as a function of country **/
       hname="HEP"+iteMap->first+anno[ia]+"_norm";
       title="N.normpapers_"+iteMap->first+"_ year_"+anno[ia];;
       H1D[hname] = new TH1F(hname.c_str(),title.c_str(),ncountries,0,ncountries);
       H1D[hname]->SetFillColor(fillCol);            
       
       iteCMap=EdCountries.begin();
       for (;iteCMap!=EdCountries.end();iteCMap++){ 
	 H1D[hname]->GetXaxis()->SetBinLabel(iteCMap->second+1,iteCMap->first.c_str());            
       }
       H1D[hname]->GetYaxis()->SetTitle( ("N.Papers/N.Authors year("+anno[ia]+")").c_str() );            
       
     }     

     iteCMap=EdCountries.begin();
     for (;iteCMap!=EdCountries.end();iteCMap++) {

       /** number of editors per country and year as a function of physgroup **/
       hname="HEd"+iteCMap->first+anno[ia];
       title="N.editors_"+iteCMap->first+"_ year_"+anno[ia];
       H1D[hname] = new TH1F(hname.c_str(),title.c_str(),ngroups,0,ngroups);
       H1D[hname]->SetFillColor(fillCol);            
       
       iteMap=Groups.begin();     
       for (;iteMap!=Groups.end();iteMap++) {       
	 H1D[hname]->GetXaxis()->SetBinLabel(iteMap->second+1,iteMap->first.c_str());            
       }
       H1D[hname]->GetYaxis()->SetTitle( "N.Editors");            
       //H1D[hname]->GetYaxis()->SetTitle( ("N.Editors year("+anno[ia]+")").c_str() );            


       /** number of editors/number of authors per country and year as a function of physgroup **/
       hname="HEd"+iteCMap->first+anno[ia]+"_norm";
       title="N.normeditors_"+iteCMap->first+"_ year_"+anno[ia];;
       H1D[hname] = new TH1F(hname.c_str(),title.c_str(),ngroups,0,ngroups);
       H1D[hname]->SetFillColor(fillCol);            
       
       iteMap=Groups.begin();     
       for (;iteMap!=Groups.end();iteMap++) {       
	 H1D[hname]->GetXaxis()->SetBinLabel(iteMap->second+1,iteMap->first.c_str());            
       }
       H1D[hname]->GetYaxis()->SetTitle( ("N.Editors/N.Authors year("+anno[ia]+")").c_str() );            


       /** number of edited papers per country and year as a function of physgroup **/
       hname="HEP"+iteCMap->first+anno[ia];
       title="N.papers_"+iteCMap->first+"_ year_"+anno[ia];;
       H1D[hname] = new TH1F(hname.c_str(),title.c_str(),ngroups,0,ngroups);
       H1D[hname]->SetFillColor(fillCol);            
       
       iteMap=Groups.begin();     
       for (;iteMap!=Groups.end();iteMap++) {       
	 H1D[hname]->GetXaxis()->SetBinLabel(iteMap->second+1,iteMap->first.c_str());            
       }
       H1D[hname]->GetYaxis()->SetTitle( ("N.Papers year("+anno[ia]+")").c_str() );            


       /** number of edited papers/number of authors per country and year as a function of physgroup **/
       hname="HEP"+iteCMap->first+anno[ia]+"_norm";
       title="N.normpapers_"+iteCMap->first+"_ year_"+anno[ia];;
       H1D[hname] = new TH1F(hname.c_str(),title.c_str(),ngroups,0,ngroups);
       H1D[hname]->SetFillColor(fillCol);            
       
       iteMap=Groups.begin();     
       for (;iteMap!=Groups.end();iteMap++) {       
	 H1D[hname]->GetXaxis()->SetBinLabel(iteMap->second+1,iteMap->first.c_str());            
       }
       H1D[hname]->GetYaxis()->SetTitle( ("N.Papers/N.Authors year("+anno[ia]+")").c_str() );            
       
       
       
     }
     
     
   }
   
   
   std::cout<<"N articoli "<<narticoli<<std::endl;

   nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<narticoli;jentry++) {
     Long64_t ientry = LoadTree(jentry);
     if (ientry < 0) break;
     nb = fChain->GetEntry(jentry);   nbytes += nb;
     std::string titolo=Title->at(0);
     std::string gruppo=Group->at(0);
     std::string Anno=Year->at(0);
     if (Status->at(0)!="submission_closed") {
        if(Status->at(0)!="phase1_active"&&Status->at(0)!="created") 
          Anno="IN_PREP";
        else 
          continue;
     } 
     std::string hname="H"+Anno;     
     if (H1D.find(hname)!=H1D.end()) H1D[hname]->Fill(Groups[gruppo]);
     hname="HALL";     
     if (H1D.find(hname)!=H1D.end()) H1D[hname]->Fill(Groups[gruppo]);

     hname="HD"+Anno;     
     if (H1D.find(hname)!=H1D.end()) H1D[hname]->Fill(Groups[gruppo],NEditors);
     hname="HDALL";     
     if (H1D.find(hname)!=H1D.end()) H1D[hname]->Fill(Groups[gruppo],NEditors);

     
     //if (mapYear.find(Anno)==mapYear.end()) std::cout<<"non trovo Anno=##"<<Anno<<"##"<<std::endl;
     Hpapers->Fill(mapYear[Anno]);
     
     bool isItaly=false;
     std::map<std::string, bool> CountryMap;
     
     for (int ied=0;ied<NEBoard;ied++) {
       std::string paese=EBCountry->at(ied);
       if (!GoodCountries[paese]) paese="ALTRO"; 
       if (paese=="Italy") {
	 isItaly=true;
//	 std::cout<<Anno<<" "<<titolo<<" "<<EBName->at(ied)<<" EB member "<<std::endl;
       }
       hname="HB"+Anno;
       if (H1D.find(hname)!=H1D.end()) H1D[hname]->Fill(EBCountries[paese]);
       hname="HBALL";
       if (H1D.find(hname)!=H1D.end()) H1D[hname]->Fill(EBCountries[paese]);
     }
     for (int ied=0;ied<NEditors;ied++) {
       //if (CoEditor->at(ied)==" coEditor") continue;
       std::string paese=EdCountry->at(ied);
       if (!GoodCountries[paese]) paese="ALTRO"; 
       if (paese=="Italy") {
	 isItaly=true;
	 std::cout<<Anno<<" "<<titolo<<" "<<EdName->at(ied)<<" "<<CoEditor->at(ied)<<" "<<gruppo<<" "<<Status->at(0)<<" "<<std::endl;
       }
       
       hname="HEd"+gruppo+Anno;
       if (H1D.find(hname)!=H1D.end()) H1D[hname]->Fill(EdCountries[paese]);
       hname="HEd"+gruppo+"ALL";
       if (H1D.find(hname)!=H1D.end()) H1D[hname]->Fill(EdCountries[paese]);
       hname="HC"+Anno;
       if (H1D.find(hname)!=H1D.end()) H1D[hname]->Fill(EdCountries[paese]);
       hname="HCALL";
       if (H1D.find(hname)!=H1D.end()) H1D[hname]->Fill(EdCountries[paese]);
       
       hname="HEd"+gruppo+Anno+"_norm";
       if (H1D.find(hname)!=H1D.end()) H1D[hname]->Fill(EdCountries[paese],1./double(NCountry[ied]));
       hname="HEd"+gruppo+"ALL"+"_norm";
       if (H1D.find(hname)!=H1D.end()) H1D[hname]->Fill(EdCountries[paese],1./double(NCountry[ied]));
       hname="HC"+Anno+"_norm";
       if (H1D.find(hname)!=H1D.end()) H1D[hname]->Fill(EdCountries[paese],1./double(NCountry[ied]));
       hname="HCALL_norm";
       if (H1D.find(hname)!=H1D.end()) H1D[hname]->Fill(EdCountries[paese],1./double(NCountry[ied]));
       
       hname="HEd"+paese+Anno;
       if (H1D.find(hname)!=H1D.end()) H1D[hname]->Fill(Groups[gruppo]);
       hname="HEd"+paese+"ALL";
       if (H1D.find(hname)!=H1D.end()) H1D[hname]->Fill(Groups[gruppo]);

       hname="HEd"+paese+Anno+"_norm";
       if (H1D.find(hname)!=H1D.end()) H1D[hname]->Fill(Groups[gruppo],1./double(NCountry[ied]));
       hname="HEd"+paese+"ALL_norm";
       if (H1D.find(hname)!=H1D.end()) H1D[hname]->Fill(Groups[gruppo],1./double(NCountry[ied]));

       /** only once per paper **/
       if (CountryMap.find(paese)==CountryMap.end()) {
	 CountryMap[paese]=true;

	 hname="HEP"+gruppo+Anno;
	 if (H1D.find(hname)!=H1D.end()) H1D[hname]->Fill(EdCountries[paese]);
	 hname="HEP"+gruppo+"ALL";
	 if (H1D.find(hname)!=H1D.end()) H1D[hname]->Fill(EdCountries[paese]);
	 hname="HCP"+Anno;
	 if (H1D.find(hname)!=H1D.end()) H1D[hname]->Fill(EdCountries[paese]);
	 hname="HCPALL";
	 if (H1D.find(hname)!=H1D.end()) H1D[hname]->Fill(EdCountries[paese]);
	 
	 hname="HEP"+gruppo+Anno+"_norm";
	 if (H1D.find(hname)!=H1D.end()) H1D[hname]->Fill(EdCountries[paese],1./double(NCountry[ied]));
	 hname="HEP"+gruppo+"ALL"+"_norm";
	 if (H1D.find(hname)!=H1D.end()) H1D[hname]->Fill(EdCountries[paese],1./double(NCountry[ied]));
	 hname="HCP"+Anno+"_norm";
	 if (H1D.find(hname)!=H1D.end()) H1D[hname]->Fill(EdCountries[paese],1./double(NCountry[ied]));
	 hname="HCPALL_norm";
	 if (H1D.find(hname)!=H1D.end()) H1D[hname]->Fill(EdCountries[paese],1./double(NCountry[ied]));




	 
	 hname="HEP"+paese+Anno;
	 if (H1D.find(hname)!=H1D.end()) H1D[hname]->Fill(Groups[gruppo]);
	 hname="HEP"+paese+"ALL";
	 if (H1D.find(hname)!=H1D.end()) H1D[hname]->Fill(Groups[gruppo]);
	 
	 hname="HEP"+paese+Anno+"_norm";
	 if (H1D.find(hname)!=H1D.end()) H1D[hname]->Fill(Groups[gruppo],1./double(NCountry[ied]));
	 hname="HEP"+paese+"ALL_norm";
	 if (H1D.find(hname)!=H1D.end()) H1D[hname]->Fill(Groups[gruppo],1./double(NCountry[ied]));

       } 
       
     }

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


