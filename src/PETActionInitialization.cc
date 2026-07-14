#include "PETActionInitialization.hh"
#include "PETPrimaryGeneratorAction.hh"

PETActionInitialization::PETActionInitialization()
{
}

PETActionInitialization::~PETActionInitialization()
{
}

void PETActionInitialization::BuildForMaster() const
{
    // Set user action classes for master thread here
}

void PETActionInitialization::Build() const
{
    // Set user action classes here
    SetUserAction(new PETPrimaryGeneratorAction());
}