#include "physics.hh"

//Implementation of the physics to be considered when running a simulation
MyPhysicsList::MyPhysicsList()
{
    RegisterPhysics (new G4EmStandardPhysics()); //Standard E&M
    RegisterPhysics (new G4OpticalPhysics()); //Optical interactions
} //Constructor

MyPhysicsList::~MyPhysicsList()
{} //Destructor

