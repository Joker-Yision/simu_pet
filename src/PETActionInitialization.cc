#include "PETActionInitialization.hh"
#include "PETPrimaryGeneratorAction.hh"
#include "PETRunAction.hh"
#include "PETEventAction.hh"
#include "PETSteppingAction.hh"

PETActionInitialization::PETActionInitialization()
{
}

PETActionInitialization::~PETActionInitialization()
{
}

void PETActionInitialization::BuildForMaster() const
{
    // Set user action classes for master thread here
    PETRunAction* runAction = new PETRunAction();
    SetUserAction(runAction);
}

void PETActionInitialization::Build() const
{
    // Set user action classes here
    SetUserAction(new PETPrimaryGeneratorAction());

    PETRunAction* runAction = new PETRunAction();
    SetUserAction(runAction);

    PETEventAction* eventAction = new PETEventAction();
    SetUserAction(eventAction);

    PETSteppingAction* steppingAction = new PETSteppingAction(eventAction);
    SetUserAction(steppingAction);
}