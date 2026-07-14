#ifndef PETACTIONINITIALIZATION_HH
#define PETACTIONINITIALIZATION_HH 1

#include "G4VUserActionInitialization.hh"

class PETActionInitialization : public G4VUserActionInitialization
{
public:
    PETActionInitialization();
    virtual ~PETActionInitialization();

    virtual void Build() const override;
    virtual void BuildForMaster() const override;
};

#endif