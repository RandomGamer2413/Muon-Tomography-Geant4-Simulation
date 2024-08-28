//******************************************************************************
// PrimaryGeneratorAction.cc
//
// 1.00 JMV, LLNL, Jan-2007:  First version.
//******************************************************************************
//

#include <iomanip>
#include "PrimaryGeneratorAction.hh"
using namespace std;

#include "G4Event.hh"

#include "G4SystemOfUnits.hh"

#include <math.h>
#include <cstdlib>
#include <ctime>

//Random number generator
double randFunc()
{
    return((double) rand()/RAND_MAX);
}

//----------------------------------------------------------------------------//
PrimaryGeneratorAction::PrimaryGeneratorAction(const char *inputfile)
{
  // define a particle gun
  particleGun = new G4ParticleGun();

  // Read the cry input file
  std::ifstream inputFile;
  inputFile.open(inputfile,std::ios::in);
  char buffer[1000];

  if (inputFile.fail()) {
    if( *inputfile !=0)  //....only complain if a filename was given
      G4cout << "PrimaryGeneratorAction: Failed to open CRY input file= " << inputfile << G4endl;
    InputState=-1;
  }else{
    std::string setupString("");
    while ( !inputFile.getline(buffer,1000).eof()) {
      setupString.append(buffer);
      setupString.append(" ");
    }
//      G4cout << "Whattup " << setupString << G4endl; //This is just for debugging

    CRYSetup *setup=new CRYSetup(setupString,"../data");

    gen = new CRYGenerator(setup);

    // set random number generator
//    RNGWrapper<CLHEP::HepRandomEngine>::set(CLHEP::HepRandom::getTheEngine(),&CLHEP::HepRandomEngine::flat);
//      G4cout << "Random function number is: " << RNGWrapper<CLHEP::HepRandomEngine>::rng() << G4endl;
//    setup->setRandomFunction(RNGWrapper<CLHEP::HepRandomEngine>::rng);
      
      srand(static_cast<unsigned int>(time(0)));
      G4cout << "RANDOM DEBUGGING: " << randFunc() << G4endl;
      setup->setRandomFunction(randFunc);
    InputState=0;
  }
  // create a vector to store the CRY particle properties
  vect=new std::vector<CRYParticle*>;

  // Create the table containing all particle names
  particleTable = G4ParticleTable::GetParticleTable();

  // Create the messenger file
  gunMessenger = new PrimaryGeneratorMessenger(this);
}

//----------------------------------------------------------------------------//
PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
}

//----------------------------------------------------------------------------//
void PrimaryGeneratorAction::InputCRY()
{
  InputState=1;
}

//----------------------------------------------------------------------------//
void PrimaryGeneratorAction::UpdateCRY(std::string* MessInput)
{
  CRYSetup *setup=new CRYSetup(*MessInput,"../data");

  gen = new CRYGenerator(setup);

  // set random number generator
//  RNGWrapper<CLHEP::HepRandomEngine>::set(CLHEP::HepRandom::getTheEngine(),&CLHEP::HepRandomEngine::flat);
//  setup->setRandomFunction(RNGWrapper<CLHEP::HepRandomEngine>::rng);
    srand(static_cast<unsigned int>(time(0)));
    setup->setRandomFunction(randFunc);
  InputState=0;

}

//----------------------------------------------------------------------------//
void PrimaryGeneratorAction::CRYFromFile(G4String newValue)
{
  // Read the cry input file
  std::ifstream inputFile;
  inputFile.open(newValue,std::ios::in);
  char buffer[1000];

  if (inputFile.fail()) {
    G4cout << "Failed to open input file " << newValue << G4endl;
    G4cout << "Make sure to define the cry library on the command line" << G4endl;
    InputState=-1;
  }else{
    std::string setupString("");
    while ( !inputFile.getline(buffer,1000).eof()) {
      setupString.append(buffer);
      setupString.append(" ");
    }

    CRYSetup *setup=new CRYSetup(setupString,"../data");

    gen = new CRYGenerator(setup);

  // set random number generator
//    RNGWrapper<CLHEP::HepRandomEngine>::set(CLHEP::HepRandom::getTheEngine(),&CLHEP::HepRandomEngine::flat);
//    setup->setRandomFunction(RNGWrapper<CLHEP::HepRandomEngine>::rng);
      srand(static_cast<unsigned int>(time(0)));
      setup->setRandomFunction(randFunc);
    InputState=0;
  }
}

//----------------------------------------------------------------------------//
void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    if (InputState != 0) {
        G4String* str = new G4String("CRY library was not successfully initialized");
        //G4Exception(*str);
        G4Exception("PrimaryGeneratorAction", "1",
                    RunMustBeAborted, *str);
    }
    
    G4String particleName;
    vect->clear();
    gen->genEvent(vect);
    
    //....debug output
    //    G4cout << "Event=" << anEvent->GetEventID() << G4endl;
    //        << " "
    //        << "CRY generated nparticles=" << vect->size()
    //        << G4endl;
    
    //    G4cout << "For debugging 1!" << G4endl;
    
    //Important variables for muon filtering inside the for loop
    double const detectorZPosition = (-100)*m; //meters
    double const detectorDimension = 3.8*m;
//    double const xOffset = 20.0*m;
//    double const yOffset = 20.0*m;

    
    //    G4cout << "For debugging 2!" << G4endl;
    
    for ( unsigned j=0; j<vect->size(); j++)
    {
        particleName=CRYUtils::partName((*vect)[j]->id());
        
        //....debug output
        //        G4cout << "  "          << particleName << " "
        //            << "charge="      << (*vect)[j]->charge() << " "
        //            << setprecision(4)
        //            << "energy (MeV)=" << (*vect)[j]->ke()*MeV << " "
        //            << "pos (m)"
        //            << G4ThreeVector((*vect)[j]->x(), (*vect)[j]->y(), (*vect)[j]->z())
        //            << " " << "direction cosines "
        //            << G4ThreeVector((*vect)[j]->u(), (*vect)[j]->v(), (*vect)[j]->w())
        //            << " " << "time (s)=" << (*vect)[j]->t() << G4endl;
        
        //Filter out muons that will not reach our detector (don't waste time and simulate them)
        double initPosZ = ((*vect)[j]->z())*m;
        double initDirectionZ = (*vect)[j]->w();
        if (initDirectionZ >= 0)
        {
            delete (*vect)[j];
            continue;
        }
        double initPosX = ((*vect)[j]->x())*m;
        double initPosY = ((*vect)[j]->y())*m;
        double initDirectionX = (*vect)[j]->u();
        double initDirectionY = (*vect)[j]->v();
//        
        double t_z = (detectorZPosition-initPosZ)/initDirectionZ; //Solve for parameter t in z-component of parametric line equation for muon trajectory
        double x_final = initPosX+initDirectionX*t_z; //Calculate approximate x-position of muon when it reaches the detector's z-position
        double y_final = initPosY+initDirectionY*t_z; //Calculate approximate y-position of muon when it reaches the detector's z-position
        
        //for debugging (assumes 2 billion events per run)
        double timeOfArrival = (*vect)[j]->t();
        G4int evt = anEvent->GetEventID();
        if (evt % 20000000 == 0)
        {
            G4cout << "Percentage done: " << (double)(anEvent->GetEventID())/(20000000.0) << G4endl;
            G4cout << "Time of muon arrival: " << timeOfArrival << G4endl;
//            G4cout << evt+1 << ": Time [s]: " << timeOfArrival << ": Position: " << initPosX << ", " << initPosY << ", " << initPosZ << G4endl;

        }
        
        bool filterCondition = x_final <= detectorDimension && x_final >= detectorDimension*-1 && y_final <= detectorDimension && y_final >= detectorDimension*-1;
//        G4bool filterCondition = true;
//        bool filterCondition = x_final <= (detectorDimension+xOffset) && x_final >= (xOffset+detectorDimension*-1) && y_final <= (detectorDimension+yOffset) && y_final >= (yOffset+detectorDimension*-1);
        if (filterCondition)
        {
            //            G4cout << "Particle generated! Time from CRY: " << (*vect)[j]->t() << ". Event ID: " << anEvent->GetEventID() << "." << G4endl;
            particleGun->SetParticleDefinition(particleTable->FindParticle((*vect)[j]->PDGid()));
            particleGun->SetParticleEnergy((*vect)[j]->ke()*MeV);
            particleGun->SetParticlePosition(G4ThreeVector(initPosX, initPosY, initPosZ));
//            particleGun->SetParticlePosition(G4ThreeVector(0, 1*cm, 0));
            
            particleGun->SetParticleMomentumDirection(G4ThreeVector(initDirectionX, initDirectionY, initDirectionZ));
//            particleGun->SetParticleMomentumDirection(G4ThreeVector(0, 0, -1));
            particleGun->SetParticleTime(timeOfArrival);
            particleGun->GeneratePrimaryVertex(anEvent);
            
            delete (*vect)[j];
            continue;
        }
        else
        {
            delete(*vect)[j];
            continue;
        }
    }
}
