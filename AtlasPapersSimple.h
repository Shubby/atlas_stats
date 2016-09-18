//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Sep 15 09:08:41 2016 by ROOT version 6.04/00
// from TTree PUB/data/atlas_papers_2016.root
// found on file: data/atlas_papers_2016.root
//////////////////////////////////////////////////////////

#ifndef AtlasPapersSimple_h
#define AtlasPapersSimple_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "vector"

class AtlasPapersSimple {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   vector<string>  *EditCountry;
   vector<string>  *CoedCountry;
   vector<string>  *EdboCountry;
   vector<string>  *ChaiCountry;
   vector<string>  *AlleCountry;
   Int_t           NEditors;
   Int_t           NEBoard;
   Int_t           NCountry[400];   //[NEditors]
   Int_t           NBCountry[400];   //[NEBoard]
   vector<string>  *Title;
   vector<string>  *Group;
   vector<string>  *Status;
   Int_t           Year;
   Int_t           PublRun;

   // List of branches
   TBranch        *b_EditCountry;   //!
   TBranch        *b_CoedCountry;   //!
   TBranch        *b_EdboCountry;   //!
   TBranch        *b_ChaiCountry;   //!
   TBranch        *b_AlleCountry;   //!
   TBranch        *b_NEditors;   //!
   TBranch        *b_NEBoard;   //!
   TBranch        *b_NCountry;   //!
   TBranch        *b_NBCountry;   //!
   TBranch        *b_Title;   //!
   TBranch        *b_Group;   //!
   TBranch        *b_Status;   //!
   TBranch        *b_Year;   //!
   TBranch        *b_PublRun;   //!

   AtlasPapersSimple(TTree *tree=0);
   virtual ~AtlasPapersSimple();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop(bool verbose=false);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
   void Dump();
};

#endif

#ifdef AtlasPapersSimple_cxx
AtlasPapersSimple::AtlasPapersSimple(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("data/atlas_papers_2009_2016.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("data/atlas_papers_2009_2016.root");
      }
      f->GetObject("PUB",tree);

   }
   Init(tree);
}

AtlasPapersSimple::~AtlasPapersSimple()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t AtlasPapersSimple::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t AtlasPapersSimple::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void AtlasPapersSimple::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   EditCountry = 0;
   CoedCountry = 0;
   EdboCountry = 0;
   ChaiCountry = 0;
   AlleCountry = 0;
   Title = 0;
   Group = 0;
   Status = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("EditCountry", &EditCountry, &b_EditCountry);
   fChain->SetBranchAddress("CoedCountry", &CoedCountry, &b_CoedCountry);
   fChain->SetBranchAddress("EdboCountry", &EdboCountry, &b_EdboCountry);
   fChain->SetBranchAddress("ChaiCountry", &ChaiCountry, &b_ChaiCountry);
   fChain->SetBranchAddress("AlleCountry", &AlleCountry, &b_AlleCountry);
   fChain->SetBranchAddress("NEditors", &NEditors, &b_NEditors);
   fChain->SetBranchAddress("NEBoard", &NEBoard, &b_NEBoard);
   fChain->SetBranchAddress("NCountry", NCountry, &b_NCountry);
   fChain->SetBranchAddress("NBCountry", NBCountry, &b_NBCountry);
   fChain->SetBranchAddress("Title", &Title, &b_Title);
   fChain->SetBranchAddress("Group", &Group, &b_Group);
   fChain->SetBranchAddress("Status", &Status, &b_Status);
   fChain->SetBranchAddress("Year", &Year, &b_Year);
   fChain->SetBranchAddress("PublRun", &PublRun, &b_PublRun);
   Notify();
}

Bool_t AtlasPapersSimple::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void AtlasPapersSimple::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t AtlasPapersSimple::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef AtlasPapersSimple_cxx
