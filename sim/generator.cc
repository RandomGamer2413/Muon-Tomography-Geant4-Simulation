#include "generator.hh"
#include "G4AnalysisManager.hh"
#include "G4Event.hh"

MyPrimaryGenerator::MyPrimaryGenerator()
{
    mu_plus = G4ParticleTable::GetParticleTable()->FindParticle("mu+");
    mu_minus = G4ParticleTable::GetParticleTable()->FindParticle("mu-");
    
    fParticleGun = new G4ParticleGun(); //Parameters: (num of particles per event, )
    
    fMuonGen.SetUseSky();
    fMuonGen.SetSkySize({{30.*m, 30.*m}});
    fMuonGen.SetSkyCenterPosition({{0., 0., 0.}});
} //Constructor

MyPrimaryGenerator::~MyPrimaryGenerator()
{
//    delete fParticleGun;
    delete fParticleGun;
} //Destructor

//This is for using EcoMUG muon generator. Will mainly use CRY though, so look at
//"PrimaryGeneratorAction.cc" for muon generation code
void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
    std::array<double, 3> muon_position;
    for (int i=0; i < 1; i++)
    {
        fMuonGen.Generate();
        muon_position = fMuonGen.GetGenerationPosition();
        double muon_p = fMuonGen.GetGenerationMomentum();
        double muon_theta = fMuonGen.GetGenerationTheta();
        double muon_phi = fMuonGen.GetGenerationPhi();
        double muon_charge = fMuonGen.GetCharge();
        
        G4AnalysisManager *man = G4AnalysisManager::Instance();
        G4int evt = anEvent->GetEventID();
        
        double estTime = fMuonGen.GetEstimatedTime(evt+1);
        
        fParticleGun->SetParticlePosition(G4ThreeVector(muon_position[0]*mm, muon_position[1]*mm, muon_position[2]*mm));
        fParticleGun->SetParticleMomentum(G4ParticleMomentum(muon_p*sin(muon_theta)*cos(muon_phi)*GeV, muon_p*sin(muon_theta)*sin(muon_phi)*GeV, muon_p*cos(muon_theta)*GeV));
        fParticleGun->SetParticleTime(estTime);
        
        if (evt % 10 == 0)
        {
            G4cout << evt+1 << ": Time [s]: " << estTime << ": Position: " << muon_position[0] << ", " << muon_position[1] << ", " << muon_position[2] << ": Momentum [GeV/c]: " << muon_p << G4endl;
        }
        
        if(muon_charge < 0)
        {
            fParticleGun->SetParticleDefinition(mu_minus);
        }
        else
        {
            fParticleGun->SetParticleDefinition(mu_plus);
        }
        
        fParticleGun->GeneratePrimaryVertex(anEvent);
    }
}
