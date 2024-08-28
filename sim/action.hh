#ifndef ACTION_HH
#define ACTION_HH

#include "G4VUserActionInitialization.hh"

//#include "generator.hh"
#include "run.hh"
#include "PrimaryGeneratorAction.hh"
//#include "event.hh"

//Boiler plate to implement "event steps" or "events"
class MyActionInitialization : public G4VUserActionInitialization
{
public:
    MyActionInitialization();
    ~MyActionInitialization();
    
    virtual void Build() const; //Runs our particle gun, computes stepping, etc ...
//    virtual void BuildForMaster() const; //Multithreading code
};

#endif
