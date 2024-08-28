#include "event.hh"

MyEventAction::MyEventAction()
{} //constructor

MyEventAction::~MyEventAction()
{} //destructor

void MyEventAction::BeginOfEventAction(const G4Event* event)
{
}

void MyEventAction::EndOfEventAction(const G4Event* event)
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    man->Write();
}
