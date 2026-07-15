#ifndef PETDETECTORCONSTRUCTION_HH
#define PETDETECTORCONSTRUCTION_HH 1

#include "G4VUserDetectorConstruction.hh"

class PETDetectorConstruction : public G4VUserDetectorConstruction
{
public:
  PETDetectorConstruction();
  virtual ~PETDetectorConstruction();   
  virtual G4VPhysicalVolume* Construct();

  void DefineMaterials();
  G4VPhysicalVolume* DefineVolumes();
  //void DefineSensitiveDetectors();

private:
  // Add any private member variables or methods here
  G4Material* fWorldMaterial = nullptr;
  G4Material* fCryMaterial = nullptr;
  G4Material* fDetectorMaterial = nullptr;
  G4Material* fLightGuideMaterial = nullptr;
  G4Material* fTeflonMaterial = nullptr;

  G4LogicalVolume* fWorldLogical = nullptr;
  G4LogicalVolume* fDetectorLogical = nullptr;
  G4LogicalVolume* fCryLogical = nullptr;
};

#endif