#include "run.hh"
#include <string>

MyRunAction::MyRunAction()
{} //constructor

MyRunAction::~MyRunAction()
{} //destructor

//Controls what happens at the start of a run
//Using it to write out detector hits into a root file
void MyRunAction::BeginOfRunAction(const G4Run* run)
{
    //Used to manage data input/output
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    //Open a new root file to write data into
    std::string fileName = "output"+std::to_string(run->GetRunID())+".root";
    man->OpenFile(fileName);
    //Create Ntuple and its columns/branches
    man->CreateNtuple("Hits", "Hits");
    man->CreateNtupleIColumn("detectorID");
    man->CreateNtupleDColumn("time");
    man->CreateNtupleIColumn("eventID");
    man->FinishNtuple(0);
}

//Controls what happens at the end of a run
void MyRunAction::EndOfRunAction(const G4Run*)
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    
    //Always write to a file and then close it
    man->Write();
    man->CloseFile();
}
