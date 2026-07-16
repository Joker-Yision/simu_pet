#include "PETSteppingAction.hh"
#include "PETRunAction.hh"
#include "G4RunManager.hh"
#include "G4Step.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleDefinition.hh"
#include "G4OpticalPhoton.hh"
#include "G4OpBoundaryProcess.hh"


PETSteppingAction::PETSteppingAction(PETEventAction* eventAction)
    : G4UserSteppingAction(), fEventAction(eventAction)
{
}

PETSteppingAction::~PETSteppingAction()
{
}       

void PETSteppingAction::UserSteppingAction(const G4Step* step)
{
    // Get the current run action
    //const PETRunAction* runAction = static_cast<const PETRunAction*>(G4RunManager::GetRunManager()->GetUserRunAction());

    //Get optical photon definition
    G4ParticleDefinition* opticalPhoton = G4OpticalPhoton::OpticalPhotonDefinition();
    G4ParticleDefinition* particleType = step->GetTrack()->GetDefinition();

    // Check if the particle is an optical photon
    if (particleType != opticalPhoton) return;

    // check if the photon is on boundary
    G4StepPoint* postStepPoint = step->GetPostStepPoint();
    G4StepStatus stepStatus = postStepPoint->GetStepStatus();
    if (stepStatus != fGeomBoundary) return;

    // check if the photon status is "Detection"
    G4OpBoundaryProcessStatus boundaryStatus = Undefined;
    G4ProcessManager* opManager = opticalPhoton->GetProcessManager();
    G4int n_proc = opManager->GetPostStepProcessVector(typeDoIt)->entries();
    G4ProcessVector* prcessVector = opManager->GetPostStepProcessVector(typeDoIt);
    for (G4int i = 0; i < n_proc; ++i)
    {
        G4VProcess* currentProcess = (*prcessVector)[i];
        G4OpBoundaryProcess* boundaryProcess = dynamic_cast<G4OpBoundaryProcess*>(currentProcess);
      if(boundaryProcess)
        {
            boundaryStatus = boundaryProcess->GetStatus();
        }
    }
    
    if(boundaryStatus == Detection)
    {
        fEventAction->AddPhotonCount();
    }


  
}