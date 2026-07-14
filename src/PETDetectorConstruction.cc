#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "G4NistManager.hh"

#include "PETDetectorConstruction.hh"

PETDetectorConstruction::PETDetectorConstruction()
{
}

PETDetectorConstruction::~PETDetectorConstruction()
{ 
}

G4VPhysicalVolume* PETDetectorConstruction::Construct()
{
    DefineMaterials();
    return DefineVolumes();
}

void PETDetectorConstruction::DefineMaterials()
{
    // Define materials here
    G4NistManager* nistManager = G4NistManager::Instance();

    fWorldMaterial = nistManager->FindOrBuildMaterial("G4_AIR");
    fBgo = nistManager->FindOrBuildMaterial("G4_BGO");
    fDetectorMaterial = nistManager->FindOrBuildMaterial("G4_Si");
}

G4VPhysicalVolume* PETDetectorConstruction::DefineVolumes()
{   
    G4bool checkOverlaps = true;

    // Define the world volume
    G4double worldSize = 1.0 * m;
    G4Box* worldSolid = new G4Box("World", worldSize/2, worldSize/2, worldSize/2);
    fWorldLogical = new G4LogicalVolume(worldSolid, fWorldMaterial, "WorldLogical");
    G4VPhysicalVolume* worldPhysical = new G4PVPlacement(nullptr, G4ThreeVector(), fWorldLogical, "WorldPhysical", nullptr, false, 0);

    // Define the BGO volume
    G4double bgoSize  = 3.3 * cm;
    G4double bgoZSize = 10. * mm;
    G4double bgoZPosition = 10 * cm + bgoZSize/2; // Position the BGO volume at z = 10 cm  
    G4Box* bgoSolid = new G4Box("BGO", bgoSize/2, bgoSize/2, bgoZSize/2);
    fBgoLogical = new G4LogicalVolume(bgoSolid, fBgo, "BGOLogical");
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, bgoZPosition), fBgoLogical, "BGOPhysical", fWorldLogical, checkOverlaps, 0);

    // Define the detector volume
    G4double detectorSize = 3.3* cm;
    G4double detectorZSize = 0.1 * mm;
    G4double detectorZPosition = 10 * cm + bgoZSize + detectorZSize/2; // Position the detector volume just after the BGO volume
    G4Box* detectorSolid = new G4Box("Detector", detectorSize/2, detectorSize/2, detectorZSize/2);
    fDetectorLogical = new G4LogicalVolume(detectorSolid, fDetectorMaterial, "DetectorLogical");
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, detectorZPosition), fDetectorLogical, "DetectorPhysical", fWorldLogical, checkOverlaps, 0);


    return worldPhysical;
}