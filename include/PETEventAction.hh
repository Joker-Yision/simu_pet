#ifndef PETEVENTACTION_HH
#define PETEVENTACTION_HH 1  

#include "G4UserRunAction.hh"
#include "globals.hh"  
#include "G4UserEventAction.hh"

class PETEventAction : public G4UserEventAction
{
public:
    PETEventAction();
    virtual ~PETEventAction();  

    virtual void BeginOfEventAction(const G4Event* anEvent) override;
    virtual void EndOfEventAction(const G4Event* anEvent) override;

    void AddPhotonCount(){ fPhotonCount++; }


private:
    G4int fPhotonCount; // Counter for detected photons per event
};
#endif