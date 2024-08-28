#include "action.hh"

MyActionInitialization::MyActionInitialization()
{} //Constructor

MyActionInitialization::~MyActionInitialization()
{} //Destructor

//for multi threading mode
//void MyActionInitialization::BuildForMaster() const
//{
//    MyRunAction *runAction = new MyRunAction();
//    SetUserAction(runAction);
//}

//Action/event steps (what happens at each 'tick'/event-step)
void MyActionInitialization::Build() const
{
    //EcoMUG
    //Generate a particle!
//    MyPrimaryGenerator *generator = new MyPrimaryGenerator();
//    SetUserAction(generator);
//    //Write out data to a root file!
//    MyRunAction *runAction = new MyRunAction();
//    SetUserAction(runAction);
    
    //CRY
    PrimaryGeneratorAction *generator = new PrimaryGeneratorAction("setup.file");
    SetUserAction(generator); //Generate a particle
    MyRunAction *runAction = new MyRunAction();
    SetUserAction(runAction); //write out data to ROOT file
    
    //To write to root file after every event as opposed to after every run
//    MyEventAction *eventAction = new MyEventAction();
//    SetUserAction(eventAction);
    
}
