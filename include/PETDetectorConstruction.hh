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
  G4Material* fWorldMaterial;
  G4Material* fBgo;
  G4Material* fDetectorMaterial;
  
  G4LogicalVolume* fWorldLogical;
  G4LogicalVolume* fDetectorLogical;
  G4LogicalVolume* fBgoLogical;
};

#endif