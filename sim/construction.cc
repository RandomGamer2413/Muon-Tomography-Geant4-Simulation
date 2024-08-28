#include "construction.hh"
#include "OrganicScintillatorFactory.hh"

MyDetectorConstruction::MyDetectorConstruction()
{} //Constructor

MyDetectorConstruction::~MyDetectorConstruction()
{} //Destructor

G4VPhysicalVolume *MyDetectorConstruction::Construct()
{
    G4NistManager *nist = G4NistManager::Instance();
    
    //See if I can define a muon detector scintillator material
//    G4String osCode = "OPSC-100";
//    G4bool isMPTOn = true;
//    G4Material *scntMat1 = OrganicScintillatorFactory::GetInstance()->Get(osCode, isMPTOn);

//    //Define refractive indexes for world/air at different light energies/momenta
//    G4double energy[2] = {1.239841939*eV/0.9, 1.239841939*eV/0.2};
//    G4double rindexWorld[2] = {1.0, 1.0};
    
    G4Material *worldMat = nist->FindOrBuildMaterial("G4_AIR"); //Define "air" as a material
    
//    //Add refractive index to world/air volume
//    G4MaterialPropertiesTable *mptWorld = new G4MaterialPropertiesTable();
//    mptWorld->AddProperty("RINDEX", energy, rindexWorld, 2);
//    
//    //Set material properties
//    worldMat->SetMaterialPropertiesTable(mptWorld);
    
    //All volumes and detectors need a solid volume, a logical volume, and a physical volume
    
    //Build the world
    G4Box *solidWorld = new G4Box("solidWorld", 150*m, 150*m, 150*m);
    G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld"); //Defining the properties of the solid volume by giving it a material (the "logical volume")
    G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logicWorld, "physWorld", 0, false, 0, true); //The "physical volume" that places the volume in the simulation world
    //paramters are: (rotation, coordinates, logical volume, name, whether to place this volume in a "mother volume", want to implement boolean operations, copy number/index, want to check for overlaps with other volumes?)
    
    //Define and construct concrete with variable density (LITHIUM ORE)
    double variableDensity = 2.7; // g/cm^3
    G4Material *materialOfSolid1 = nist->BuildMaterialWithNewDensity("lithiumOre", "G4_CONCRETE", variableDensity*g/cm3);
    G4Box *solidSlab1 = new G4Box("solidSlab1", 3.28*m, 3.28*m, 3.28*m);
    G4LogicalVolume *logicSlab1 = new G4LogicalVolume(solidSlab1, materialOfSolid1, "logicSlab1");
    G4VPhysicalVolume *physSlab1 = new G4PVPlacement(0, G4ThreeVector(0., 0., (-50+3.28)*m), logicSlab1, "physSlab1", logicWorld, false, 0, true);
    
    //Construct soil around lithium ore
    variableDensity = 1.7;
    G4Material *materialOfSolid2 = nist->BuildMaterialWithNewDensity("soil", "G4_CONCRETE", variableDensity*g/cm3);
    
    G4Box *solidSlab2 = new G4Box("solidSlab2", 150*m, 150*m, (21.72)*m);
    G4LogicalVolume *logicSlab2 = new G4LogicalVolume(solidSlab2, materialOfSolid2, "logicSlab2");
    G4VPhysicalVolume *physSlab2 = new G4PVPlacement(0, G4ThreeVector(0., 0., (-21.72)*m), logicSlab2, "physSlab2", logicWorld, false, 0, true);
    
    G4Box *solidSlab3 = new G4Box("solidSlab3", 73.36*m, 3.28*m, (3.28)*m);
    G4LogicalVolume *logicSlab3 = new G4LogicalVolume(solidSlab3, materialOfSolid2, "logicSlab3");
    G4VPhysicalVolume *physSlab3 = new G4PVPlacement(0, G4ThreeVector(76.64*m, 0., (-50+3.28)*m), logicSlab3, "physSlab3", logicWorld, false, 0, true);
    
    G4VPhysicalVolume *physSlab4 = new G4PVPlacement(0, G4ThreeVector(-76.64*m, 0., (-50+3.28)*m), logicSlab3, "physSlab4", logicWorld, false, 0, true);
    
    G4Box *solidSlab4 = new G4Box("solidSlab4", 150*m, (73.36)*m, (3.28)*m);
    G4LogicalVolume *logicSlab4 = new G4LogicalVolume(solidSlab4, materialOfSolid2, "logicSlab4");
    G4VPhysicalVolume *physSlab5 = new G4PVPlacement(0, G4ThreeVector(0., -76.64*m, (-50+3.28)*m), logicSlab4, "physSlab5", logicWorld, false, 0, true);
    
    G4VPhysicalVolume *physSlab6 = new G4PVPlacement(0, G4ThreeVector(0., 76.64*m, (-50+3.28)*m), logicSlab4, "physSlab6", logicWorld, false, 0, true);
    
    G4Box *solidSlab5 = new G4Box("solidSlab5", 150*m, (150)*m, (25)*m);
    G4LogicalVolume *logicSlab5 = new G4LogicalVolume(solidSlab5, materialOfSolid2, "logicSlab5");
    G4VPhysicalVolume *physSlab7 = new G4PVPlacement(0, G4ThreeVector(0., 0., (-75)*m), logicSlab5, "physSlab7", logicWorld, false, 0, true);
    
    //Define and construct "experiment 2": half concrete, half air
//    G4Box *solidHalfAir = new G4Box("solidHalfAir", 75*m, 150*m, 50*m);
//    G4LogicalVolume *logicHalfAir = new G4LogicalVolume(solidHalfAir, worldMat, "logicHalfAir");
//    G4VPhysicalVolume *physHalfAir = new G4PVPlacement(0, G4ThreeVector(75*m, 0., -50.01*m), logicHalfAir, "physHalfAir", logicWorld, false, 0, true);
    //half concrete
//    G4Material *materialOfSolid1 = nist->FindOrBuildMaterial("G4_CONCRETE");
//    G4Box *solidHalfSlab = new G4Box("solidHalfSlab", 75*m, 150*m, 50*m);
//    G4LogicalVolume *logicHalfSlab = new G4LogicalVolume(solidHalfSlab, materialOfSolid1, "logicHalfSlab");
//    G4VPhysicalVolume *physHalfSlab = new G4PVPlacement(0, G4ThreeVector(-75*m, 0., -50.01*m), logicHalfSlab, "physHalfSlab", logicWorld, false, 0, true);
    
    //Construct muon sensors/detectors
    G4Box *solidDetector = new G4Box("solidDetector", 0.0125*m, 0.0125*m, 0.005*m);
    logicDetector = new G4LogicalVolume(solidDetector, worldMat, "logicDetector"); //logicDetector is defined in the header file
    double const xOffset = 0.0;
    double const yOffset = 0.0;
    for (G4int i = 0; i < 4; i++)
    {
        for (G4int j = 0; j < 4; j++)
        {
            G4int detectorID = j+i*4;
            G4VPhysicalVolume *physDetector = new G4PVPlacement(0, G4ThreeVector(((-0.0375+xOffset)+j*(0.025))*m, ((0.0375+yOffset)-i*(0.025))*m, (-100-0.005)*m), logicDetector, "physDetector", logicWorld, false, detectorID, true);
        }
    }
    //2nd layer
    for (G4int i = 0; i < 4; i++)
    {
        for (G4int j = 0; j < 4; j++)
        {
            G4int detectorID = (j+i*4)+16;
            G4VPhysicalVolume *physDetector = new G4PVPlacement(0, G4ThreeVector(((-0.0375+xOffset)+j*(0.025))*m, ((0.0375+yOffset)-i*(0.025))*m, (-100-0.005-0.76)*m), logicDetector, "physDetector", logicWorld, false, detectorID, true);
        }
    }
    
    return physWorld; //Always return physical world volume at end
}

//Function to construct sensitive detectors
void MyDetectorConstruction::ConstructSDandField()
{
    MySensitiveDetector *sensDet = new MySensitiveDetector("SensitiveDetector");
    
    logicDetector->SetSensitiveDetector(sensDet);
}
