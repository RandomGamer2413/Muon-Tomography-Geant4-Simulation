#ifndef EVENT_HH
#define EVENT_HH

#include "G4UserEventAction.hh"
#include "G4Event.hh"
#include "G4AnalysisManager.hh"
//#include "globals.hh"

class MyEventAction : public G4UserEventAction
{
public:
    MyEventAction();
    ~MyEventAction();
    
    virtual void BeginOfEventAction(const G4Event* event);
    virtual void EndOfEventAction(const G4Event* event);
};

#endif
