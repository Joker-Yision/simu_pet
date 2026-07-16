#include "PETEventAction.hh"
#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"
PETEventAction::PETEventAction()
    : G4UserEventAction(), fPhotonCount(0)
{
}

PETEventAction::~PETEventAction()
{
}   

void PETEventAction::BeginOfEventAction(const G4Event* anEvent)
{
    fPhotonCount = 0; // Reset the photon count at the beginning of each event
}

void PETEventAction::EndOfEventAction(const G4Event* anEvent)
{
    // Access the analysis manager
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

    // Fill the histogram with the photon count for this event
    if(fPhotonCount != 0)
    {
    analysisManager->FillH1(0, fPhotonCount);
    }
}