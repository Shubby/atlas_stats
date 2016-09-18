//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sat Jan 11 23:15:04 2014 by ROOT version 5.32/00
// from TTree T/tree from file
// found on file: RiepilogoAI.root
//////////////////////////////////////////////////////////

#ifndef RiepilogoAI_h
#define RiepilogoAI_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include <string>

// Fixed size dimensions of array or collections stored in the TTree if any.

class RiepilogoAI {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   string          *Sezione;
   string          *Tipo;
   string          *Anno;
   string          *Analisi;
   string          *Detector;
   string          *Computing;
   string          *Altro;

   // List of branches
   TBranch        *b_Sezione;   //!
   TBranch        *b_Tipo;   //!
   TBranch        *b_Anno;   //!
   TBranch        *b_Analisi;   //!
   TBranch        *b_Detector;   //!
   TBranch        *b_Computing;   //!
   TBranch        *b_Altro;   //!

   RiepilogoAI(TTree *tree=0);
   virtual ~RiepilogoAI();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef RiepilogoAI_cxx
RiepilogoAI::RiepilogoAI(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("RiepilogoAI.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("RiepilogoAI.root");
      }
      f->GetObject("T",tree);

   }
   Init(tree);
}

RiepilogoAI::~RiepilogoAI()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t RiepilogoAI::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t RiepilogoAI::LoadTree(Long64_t entry)
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

void RiepilogoAI::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   Sezione = 0;
   Tipo = 0;
   Anno = 0;
   Analisi = 0;
   Detector = 0;
   Computing = 0;
   Altro = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("Sezione", &Sezione, &b_Sezione);
   fChain->SetBranchAddress("Tipo", &Tipo, &b_Tipo);
   fChain->SetBranchAddress("Anno", &Anno, &b_Anno);
   fChain->SetBranchAddress("Analisi", &Analisi, &b_Analisi);
   fChain->SetBranchAddress("Detector", &Detector, &b_Detector);
   fChain->SetBranchAddress("Computing", &Computing, &b_Computing);
   fChain->SetBranchAddress("Altro", &Altro, &b_Altro);
   Notify();
}

Bool_t RiepilogoAI::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void RiepilogoAI::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t RiepilogoAI::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef RiepilogoAI_cxx
