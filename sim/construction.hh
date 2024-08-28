#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"

#include "detector.hh"

//Boiler-plate to implement/CONSTRUCT detectors/volumes
class MyDetectorConstruction : public G4VUserDetectorConstruction
{
public:
    MyDetectorConstruction();
    ~MyDetectorConstruction();
    
    virtual G4VPhysicalVolume *Construct();
    
private:
    //want to access the logicDetector outside our construction.cc file, so we define it here in construction.hh
    G4LogicalVolume *logicDetector;
    
    //function to construct sensitive detecotrs
    virtual void ConstructSDandField();
};

#endif
