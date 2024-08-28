#include <iostream>
#include "G4RunManager.hh"
#include "G4MTRunManager.hh"
#include "G4UImanager.hh"
#include "G4VisManager.hh"
#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"

#include "construction.hh"
#include "physics.hh"
#include "action.hh"

#include "FTFP_BERT.hh"

//All of this is to make the Geant4 Application Run Properly
int main(int argc, char** argv)
{
    
    #ifdef G4MULTITHREADED
        G4MTRunManager *runManager = new G4MTRunManager();
        G4cout << "Multi threaded mode activated." << G4endl;
    #else
        G4RunManager *runManager = new G4RunManager();
        G4cout << "Single threaded mode activated." << G4endl;
    #endif
    
    //Initialize any classes you may have wrote (detector, physics list, etc ...)
    runManager->SetUserInitialization(new MyDetectorConstruction());
    //runManager->SetUserInitialization(new MyPhysicsList());
    
    //Trying out a new physics list for muon tomography (recommended for HEP applications)
    G4VModularPhysicsList *physics = new FTFP_BERT();
//    physics->RegisterPhysics(new G4OpticalPhysics());
    runManager->SetUserInitialization(physics);
    
    runManager->SetUserInitialization(new MyActionInitialization());
    
//    G4int nThreads = 4; //Number of threads to use in multi-threading mode
//    runManager->SetNumberOfThreads(nThreads);
    runManager->Initialize();
    
    G4UIExecutive *ui = 0;
    
    if(argc == 1)
    {
        ui = new G4UIExecutive(argc, argv);
    }
    
    G4VisManager *visManager = new G4VisExecutive();
    visManager->Initialize();
    
    G4UImanager *UImanager = G4UImanager::GetUIpointer();
    
    if(ui) //If no input passed in at time of running sim executable, then open in GUI mode
    {
        UImanager->ApplyCommand("/vis/open OGL");
        UImanager->ApplyCommand("/vis/drawVolume");
        UImanager->ApplyCommand("/vis/viewer/set/viewpointVector 1 1 1");
        UImanager->ApplyCommand("/vis/scene/add/trajectories smooth");
        UImanager->ApplyCommand("/vis/viewer/set/autoRefresh true");
        UImanager->ApplyCommand("/vis/scene/endOfEventAction accumulate 10");
        UImanager->ApplyCommand("/vis/scene/add/scale 10 cm");
        UImanager->ApplyCommand("/vis/scene/add/axes");
        UImanager->ApplyCommand("/vis/scene/add/eventID");
        ui->SessionStart();
    }
    
    else //Open and run in batch mode
    {
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command+fileName);
    }

    
    
    return 0;
}
