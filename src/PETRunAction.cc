#include "PETRunAction.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"

PETRunAction::PETRunAction()
{
    // Create an instance of the analysis manager
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->CreateH1("PhotonDetection", "Photon number Detected", 50, 0, 3000);
}

PETRunAction::~PETRunAction()
{
}

void PETRunAction::BeginOfRunAction(const G4Run* aRun)
{

    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

    //Open an output file for each thread run
    G4int runID = aRun->GetRunID();
    G4cout << "### Run " << runID << " start." << G4endl;

    std::stringstream runThreadId;
    runThreadId << runID;

    G4String fileName = "PET_Simulation_Run_" + runThreadId.str() + ".root";
    analysisManager->OpenFile(fileName);
    G4cout << "Output file: " << fileName << " had been opened." << G4endl;
}

void PETRunAction::EndOfRunAction(const G4Run* aRun)
{
    //Close the output file for each thread run
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

    G4int runID = aRun->GetRunID();
    G4cout << "### Run " << runID << " end." << G4endl;

    analysisManager->Write();
    analysisManager->CloseFile();
    G4cout << "Output file for run " << runID << " had been written and closed." << G4endl;
}   