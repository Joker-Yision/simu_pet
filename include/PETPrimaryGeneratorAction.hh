#ifndef PETPRIMARYGENERATORACTION_HH
#define PETPRIMARYGENERATORACTION_HH 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"

class PETPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
    PETPrimaryGeneratorAction();
    virtual ~PETPrimaryGeneratorAction();
    virtual void GeneratePrimaries(G4Event* anEvent);

private:
    G4ParticleGun* fParticleGun;    
};

#endif