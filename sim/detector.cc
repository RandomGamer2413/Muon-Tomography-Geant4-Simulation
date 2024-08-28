#include "detector.hh"
#include <math.h>

#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"

#include "G4RunManager.hh"

MySensitiveDetector::MySensitiveDetector(G4String name):G4VSensitiveDetector(name)
{} //Constructor

MySensitiveDetector::~MySensitiveDetector()
{} //Destructor

//Function to process hits on the sensitive detectors
G4bool MySensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist)
{
    G4Track *track = aStep->GetTrack();
    
//    track->SetTrackStatus(fStopAndKill); //When a particle enters a detector, it is 'killed' and the track is not propagated further
    
    //When particle enters detector
    G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
    //When particle leaves detector
//    G4StepPoint *postStepPoint = aStep->GetPostStepPoint();
    
    //What detector did a given particle touch?
    const G4VTouchable *touchable = aStep->GetPreStepPoint()->GetTouchable();
    int copyNo = touchable->GetCopyNumber();
    
    //What is the position of the detector a photon touches?
//    G4VPhysicalVolume *physVol = touchable->GetVolume();
//    G4ThreeVector posDetector = physVol->GetTranslation();
    
    //Store time information!! (standard unit is nanoseconds)
    double time = preStepPoint->GetGlobalTime();
    
    //Writing out data to root file
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    
    int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

//    G4cout << "Detector Hit!!" << G4endl; //debugging
    
    if(track->GetDefinition() == G4MuonMinus::Definition() || track->GetDefinition() == G4MuonPlus::Definition()) //filter to only detect muons
    {
//        G4cout << "MUON DETECTION WORKING!!!" << G4endl; //debugging
//        G4ThreeVector particleMomentum = track->GetDynamicParticle()->GetMomentumDirection();
//        double magnitude = sqrt(particleMomentum[0]*particleMomentum[0] + particleMomentum[1]*particleMomentum[1] + particleMomentum[2]*particleMomentum[2]);
//        double zenithAngle = acos(-1*(particleMomentum[2]/magnitude)); //zenith angle of muon at time of detection
//        double const viewAngle = 0.06544985; //view angle of hodoscope in radians (half-angle) //ANGULAR RESOLUTION
        if(true) //filter to only detect muons within the view angle of the hodoscope (if zenith angle is greater, cant detect it)
        {
            if(copyNo < 16)
            {
                G4cout << "Valid muon detected in layer 1!" << G4endl; //debugging
            }
            else
            {
                G4cout << "Valid muon detected in layer 2!" << G4endl; //debugging
            }

            man->FillNtupleIColumn(0, copyNo);
            man->FillNtupleDColumn(1, time);
            man->FillNtupleIColumn(2, evt);
            man->AddNtupleRow(0);
        }
    }
}
