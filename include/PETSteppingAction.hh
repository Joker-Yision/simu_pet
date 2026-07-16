#ifndef PETSTEPPINGACTION_HH
#define PETSTEPPINGACTION_HH 1  

#include "G4UserSteppingAction.hh"
#include "G4Step.hh"
#include "PETEventAction.hh"

class PETSteppingAction : public G4UserSteppingAction
{
public:
    PETSteppingAction(PETEventAction* ); 
    virtual ~PETSteppingAction();

    virtual void UserSteppingAction(const G4Step* step) override;

private:
    PETEventAction* fEventAction = nullptr; // Pointer to the event action
    
};


#endif