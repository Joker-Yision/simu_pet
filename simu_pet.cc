#include<iostream>

#include"G4RunManager.hh"
#include"G4UImanager.hh"
#include"G4UIExecutive.hh"
#include"G4VisManager.hh"
#include"G4VisExecutive.hh"

#include"FTFP_BERT.hh"
#include"G4VModularPhysicsList.hh"
#include"G4OpticalPhysics.hh"

#include"PETDetectorConstruction.hh"
#include"PETActionInitialization.hh"


int main(int argc, char** argv){
    //construct runmanager 
    G4RunManager* runManager = new G4RunManager();

    //register physics list
    G4VModularPhysicsList* physicsList = new FTFP_BERT();
    G4OpticalPhysics* opticalPhysics = new G4OpticalPhysics();
    physicsList->RegisterPhysics(opticalPhysics);
    runManager->SetUserInitialization(physicsList);

    //register detector construction
    PETDetectorConstruction* detectorConstruction = new PETDetectorConstruction();
    runManager->SetUserInitialization(detectorConstruction);

    //register action initialization
    runManager->SetUserInitialization(new PETActionInitialization());
    
    //initialize 
    /*runManager->Initialize();*/ //we can initialize in .mac

    //initialize vis manager
    G4VisManager* visManager = new G4VisExecutive();
    visManager->Initialize();

    //judge whether start UI session
    G4UIExecutive* ui = nullptr;
    if(argc == 1){ ui = new G4UIExecutive(argc,argv);}
    
    G4UImanager* uiManager = G4UImanager::GetUIpointer();
    if(!ui){
        G4String command = "/control/execute ";
        G4String filename = argv[1];
        uiManager->ApplyCommand(command + filename);
    }
    else{
        uiManager->ApplyCommand("/control/execute vis.mac");
        ui->SessionStart();
        delete ui;    
    }

    delete visManager;
    delete runManager;
    
    return 0;
}