#ifndef G4RUNACTION_HH
#define G4RUNACTION_HH 1

#include "G4UserRunAction.hh"
#include "globals.hh"

class PETRunAction : public G4UserRunAction
{
public:
  PETRunAction();
  virtual ~PETRunAction();

    virtual void BeginOfRunAction(const G4Run* aRun) override;
    virtual void EndOfRunAction(const G4Run* aRun) override;
};

#endif