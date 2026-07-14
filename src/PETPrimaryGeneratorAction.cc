
#include"PETPrimaryGeneratorAction.hh"

PETPrimaryGeneratorAction::PETPrimaryGeneratorAction()
{
    // Create a particle gun with one particle
    G4int n_particle = 1;
    fParticleGun = new G4ParticleGun(n_particle);

    // Set the default particle type to gamma
    G4ParticleDefinition* particleDefinition = G4ParticleTable::GetParticleTable()->FindParticle("gamma");
    fParticleGun->SetParticleDefinition(particleDefinition);

    // Set the default energy of the particle to 511 keV
    fParticleGun->SetParticleEnergy(511 * keV);

    // Set the default momentum direction of the particle to (0, 0, 1)
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));
}

PETPrimaryGeneratorAction::~PETPrimaryGeneratorAction()
{
    delete fParticleGun;
}

void PETPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{   
    // Set the default position of the particle to (0, 0, -10 cm)
    fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., 0.));
    // Generate the primary vertex for the event
    fParticleGun->GeneratePrimaryVertex(anEvent);
}