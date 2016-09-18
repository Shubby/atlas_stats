//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Aug  2 14:59:33 2013 by ROOT version 5.32/00
// from TTree PUB/atlaspapers.root
// found on file: atlaspapers.root
//////////////////////////////////////////////////////////

#ifndef AtlasPapersSimple_h
#define AtlasPapersSimple_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include <vector>
#include <string>

// Fixed size dimensions of array or collections stored in the TTree if any.

class AtlasPapersSimple {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           NPaper;
   vector<string>  *Title;
   vector<string>  *Year;
   vector<string>  *Group;
   vector<string>  *Status;
   Int_t           NEditors;
   Int_t           NCountry[500];   //[NEditors]
   vector<string>  *EdName;
   vector<string>  *EdCountry;
   vector<string>  *CoEditor;
   Int_t           NEBoard;
   Int_t           NBCountry[500];   //[NEBoard]
   vector<string>  *EBName;
   vector<string>  *EBCountry;
   vector<string>  *EBChair;
   vector<string>  *PublRun;


   // List of branches
   TBranch        *b_NPaper;   //!
   TBranch        *b_Title;   //!
   TBranch        *b_Year;   //!
   TBranch        *b_Group;   //!
   TBranch        *b_Status;   //!
   TBranch        *b_NEditors;   //!
   TBranch        *b_NCountry;   //!
   TBranch        *b_EdName;   //!
   TBranch        *b_EdCountry;   //!
   TBranch        *b_CoEditor;   //!
   TBranch        *b_NEBoard;   //!
   TBranch        *b_NBCountry;   //!
   TBranch        *b_EBName;   //!
   TBranch        *b_EBCountry;   //!
   TBranch        *b_EBChair;   //!
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
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("atlaspapers.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("atlaspapers.root");
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
   Title = 0;
   Year = 0;
   Group = 0;
   Status = 0;
   EdName = 0;
   EdCountry = 0;
   CoEditor = 0;
   EBName = 0;
   EBCountry = 0;
   EBChair = 0;
   PublRun = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("NPaper", &NPaper, &b_NPaper);
   fChain->SetBranchAddress("Title", &Title, &b_Title);
   fChain->SetBranchAddress("Year", &Year, &b_Year);
   fChain->SetBranchAddress("Group", &Group, &b_Group);
   fChain->SetBranchAddress("Status", &Status, &b_Status);
   fChain->SetBranchAddress("NEditors", &NEditors, &b_NEditors);
   fChain->SetBranchAddress("NCountry", NCountry, &b_NCountry);
   fChain->SetBranchAddress("EdName", &EdName, &b_EdName);
   fChain->SetBranchAddress("EdCountry", &EdCountry, &b_EdCountry);
   fChain->SetBranchAddress("CoEditor", &CoEditor, &b_CoEditor);
   fChain->SetBranchAddress("NEBoard", &NEBoard, &b_NEBoard);
   fChain->SetBranchAddress("NBCountry", NBCountry, &b_NBCountry);
   fChain->SetBranchAddress("EBName", &EBName, &b_EBName);
   fChain->SetBranchAddress("EBCountry", &EBCountry, &b_EBCountry);
   fChain->SetBranchAddress("EBChair", &EBChair, &b_EBChair);
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
