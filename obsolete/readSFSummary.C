#include <iostream>
#include <fstream>
#include <TTree.h>
#include <TFile.h>
#include <string>

void readSFSummary() {
  
  TFile f("RiepilogoSF.root", "RECREATE");
  
  std::string parole[100];
  std::string cose[]={"Anno","AnniSF","Posizione"};
  
  TTree *tree = new TTree("T","tree from file");
  for (int i=0;i<3;i++) {
    tree->Branch(cose[i].c_str(),"std::string",&parole[i]);
  }
  
  /**
     char PROVA[20]="OK";
     tree->Branch("PROVA",PROVA,"PROVA/C");  
  **/
  
  

  std::string filename="RiepilogoSF.dat";
  ifstream infile(filename.c_str()); // open file
  if(infile) {
    
    string str="";
    while(getline(infile, str)) {
      
      cout<<str<<endl;
      int nword=0;
      std::size_t oldfound=-1;
      std::size_t found = str.find_first_of("|");      
      while (found!=std::string::npos) {
	string word;
	word.assign(str,oldfound+1,found-oldfound-1);	
	parole[nword]=word;
	nword++;
	oldfound=found;
	std::cout<<word<<std::endl;
	str[found]='*';	
	found=str.find_first_of("|",found+1);
      }
      string word;
      found=str.length();
      word.assign(str,oldfound+1,found-oldfound-1);
      std::cout<<word<<std::endl;
      parole[nword]=word;
      nword++;
      tree->Fill();
    }
    
  }    
  
  
  


  infile.close();
  
  tree->Write();
  f.Write();
  f.Close();
  

}
