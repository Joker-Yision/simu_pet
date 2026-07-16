#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "G4NistManager.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4SubtractionSolid.hh"

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
    fCryMaterial = nistManager->FindOrBuildMaterial("G4_BGO");
    fDetectorMaterial = nistManager->FindOrBuildMaterial("G4_Si");
    fTeflonMaterial = nistManager->FindOrBuildMaterial("G4_TEFLON");

    fLightGuideMaterial = new G4Material("LightGuide", 0.93 * g/cm3, 4);
    fLightGuideMaterial->AddElement(nistManager->FindOrBuildElement("C"), 2);
    fLightGuideMaterial->AddElement(nistManager->FindOrBuildElement("H"), 6);
    fLightGuideMaterial->AddElement(nistManager->FindOrBuildElement("O"), 1);
    fLightGuideMaterial->AddElement(nistManager->FindOrBuildElement("Si"), 1);
}

G4VPhysicalVolume* PETDetectorConstruction::DefineVolumes()
{   
    G4bool checkOverlaps = true;

    // Define the world volume
    G4double worldSize = 30.0 * cm;
    G4Box* worldSolid = new G4Box("World", worldSize/2, worldSize/2, worldSize/2);
    fWorldLogical = new G4LogicalVolume(worldSolid, fWorldMaterial, "WorldLogical");
    G4VPhysicalVolume* worldPhysical = new G4PVPlacement(nullptr, G4ThreeVector(), fWorldLogical, "WorldPhysical", nullptr, false, 0);

    // Define the Cry volume
    G4double CrySize  = 3.3 * cm;
    G4double CryZSize = 10. * mm;
    G4double CryZPosition = 10 * cm + CryZSize/2; // Position the Cry volume at z = 10 cm
    G4Box* CrySolid = new G4Box("Cry", CrySize/2, CrySize/2, CryZSize/2);
    fCryLogical = new G4LogicalVolume(CrySolid, fCryMaterial, "CryLogical");
    G4VPhysicalVolume* CryPhysical = 
        new G4PVPlacement(nullptr, G4ThreeVector(0, 0, CryZPosition), fCryLogical, "CryPhysical", fWorldLogical, checkOverlaps, 0);

    //Define the Teflon shell surrounding the Cry
    G4double teflonThickness = 1. * mm; // 1 mm thick Teflon shell
    G4double teflonSize = CrySize + 2. * teflonThickness; // 1 mm thick Teflon shell
    G4double teflonZSize = CryZSize + 1. * teflonThickness; // 1 mm thick Teflon shell
    G4ThreeVector ioOffset = G4ThreeVector(0., 0., 0.5 * teflonThickness);
    G4ThreeVector teflonZPos = G4ThreeVector(0., 0., CryZPosition - 0.5 * teflonThickness);
    G4Box* outerBox = new G4Box("OuterBox", teflonSize/2, teflonSize/2, teflonZSize/2);
    G4Box* innerBox = new G4Box("InnerBox", CrySize/2, CrySize/2, CryZSize/2);
    G4SubtractionSolid* teflonSolid = new G4SubtractionSolid("TeflonShell", outerBox, innerBox, nullptr, ioOffset);
    G4LogicalVolume* teflonLogical = new G4LogicalVolume(teflonSolid, fTeflonMaterial, "TeflonLogical");
    G4VPhysicalVolume* teflonPhysical = 
        new G4PVPlacement(nullptr, teflonZPos, teflonLogical, "TeflonPhysical", fWorldLogical, checkOverlaps, 0);

    //Define the light guide volume
    G4double lightGuideSize = 3.3 * cm;
    G4double lightGuideZSize = 2. * mm;
    G4double lightGuideZPosition = CryZPosition + CryZSize/2 + lightGuideZSize/2; // Position the light guide volume just after the Cry volume
    G4Box* lightGuideSolid = new G4Box("LightGuide", lightGuideSize/2, lightGuideSize/2, lightGuideZSize/2);
    G4LogicalVolume* lightGuideLogical = new G4LogicalVolume(lightGuideSolid, fLightGuideMaterial, "LightGuideLogical");
    G4VPhysicalVolume* lightGuidePhysical = 
        new G4PVPlacement(nullptr, G4ThreeVector(0, 0, lightGuideZPosition), lightGuideLogical, "LightGuidePhysical", fWorldLogical, checkOverlaps, 0);

    // Define the detector volume
    G4double detectorSize = 3.3* cm;
    G4double detectorZSize = 2. * mm;
    G4double detectorZPosition = lightGuideZPosition + lightGuideZSize/2 + detectorZSize/2; // Position the detector volume just after the Cry volume
    G4Box* detectorSolid = new G4Box("Detector", detectorSize/2, detectorSize/2, detectorZSize/2);
    fDetectorLogical = new G4LogicalVolume(detectorSolid, fDetectorMaterial, "DetectorLogical");
    G4VPhysicalVolume* detectorPhysical = 
        new G4PVPlacement(nullptr, G4ThreeVector(0, 0, detectorZPosition), fDetectorLogical, "DetectorPhysical", fWorldLogical, checkOverlaps, 0);

    //add optical properties to the cry
    G4MaterialPropertiesTable* cryMPT = new G4MaterialPropertiesTable();
    cryMPT->AddConstProperty("SCINTILLATIONYIELD", 8000./MeV);
    cryMPT->AddConstProperty("RESOLUTIONSCALE", 3.33);
    cryMPT->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 315.*ns);

    std::vector<G4double> cryPhotonEnergies = { 1.82 * eV,1.86 * eV,1.91 * eV,1.95 * eV,2.00 * eV,
                                             2.05 * eV,2.10 * eV,2.16 * eV,2.21 * eV,2.28 * eV,
                                             2.34 * eV,2.41 * eV,2.48 * eV,2.56 * eV,2.64 * eV,
                                             2.73 * eV,2.82 * eV,2.92 * eV,3.02 * eV,3.14 * eV,3.26 * eV };
    std::vector<G4double> cryWaveSpectrum ={ 0.01,0.03,0.05,0.08,0.12,
                                             0.20,0.29,0.39,0.52,0.67,
                                             0.80,0.91,0.98,1.00,0.99,
                                             0.95,0.88,0.71,0.50,0.30,0.15 };  
    std::vector<G4double> cryRefractiveIndex =  { 2.088,2.091,2.094,2.097,2.100,
                                               2.104,2.110,2.113,2.118,2.123,
                                               2.129,2.135,2.143,2.151,2.159,
                                               2.169,2.181,2.193,2.208,2.224,2.224 };
    std::vector<G4double> cryAbsorptionLength =  { 1.5 * cm,3.0 * cm,8.0 * cm,20 * cm,50 * cm,
                                                100 * cm,200 * cm,300 * cm,300 * cm,300 * cm,
                                                300 * cm,300 * cm,300 * cm,300 * cm,300 * cm,
                                                300 * cm,300 * cm,300 * cm,300 * cm,300 * cm,300 * cm };
    
    cryMPT->AddProperty("RINDEX", cryPhotonEnergies, cryRefractiveIndex);
    cryMPT->AddProperty("ABSLENGTH", cryPhotonEnergies, cryAbsorptionLength);
    cryMPT->AddProperty("SCINTILLATIONCOMPONENT1", cryPhotonEnergies, cryWaveSpectrum);
    fCryMaterial->SetMaterialPropertiesTable(cryMPT);

    //define light guide properties
    G4MaterialPropertiesTable* lightGuideMPT = new G4MaterialPropertiesTable();
    const int numEntries = 2;
    G4double photonEnergies[numEntries] = { 1.82 * eV, 3.26 * eV };
    G4double lgRefractiveIndex[numEntries] = { 1.4, 1.4 };
    G4double lgSorptionLength[numEntries] = { 5. *cm, 5. *cm };

    lightGuideMPT->AddProperty("RINDEX", photonEnergies, lgRefractiveIndex, numEntries);
    lightGuideMPT->AddProperty("ABSLENGTH", photonEnergies, lgSorptionLength, numEntries);
    fLightGuideMaterial->SetMaterialPropertiesTable(lightGuideMPT);

    //Define the detector properties
    G4MaterialPropertiesTable* detectorMPT = new G4MaterialPropertiesTable();
    G4double detRefIndex[numEntries] = { 1.7, 1.7 };
    G4double detAbsorptionLength[numEntries] = { 0.1 *cm, 0.1 *cm };

    detectorMPT->AddProperty("RINDEX", photonEnergies, detRefIndex, numEntries);
    detectorMPT->AddProperty("ABSLENGTH", photonEnergies, detAbsorptionLength, numEntries);
    fDetectorMaterial->SetMaterialPropertiesTable(detectorMPT);

    //Define the optical surface between the Cry and the Teflon shell
    G4OpticalSurface* cryTeflonSurface = new G4OpticalSurface("CryTeflonSurface");
    cryTeflonSurface->SetType(dielectric_dielectric);
    cryTeflonSurface->SetFinish(groundbackpainted);
    cryTeflonSurface->SetModel(unified);
    new G4LogicalBorderSurface("CryTeflonSurface", CryPhysical, teflonPhysical, cryTeflonSurface);

    //Define the optical surface between the Cry and the light guide
    G4OpticalSurface* cryLightGuideSurface = new G4OpticalSurface("CryLightGuideSurface");
    cryLightGuideSurface->SetType(dielectric_dielectric);
    cryLightGuideSurface->SetFinish(polished);
    cryLightGuideSurface->SetModel(unified);
    new G4LogicalBorderSurface("CryLightGuideSurface", CryPhysical, lightGuidePhysical, cryLightGuideSurface);


    //Define the optical surface between the light guide and the detector
    G4OpticalSurface* lightGuideDetectorSurface = new G4OpticalSurface("DetectorSurface");
    lightGuideDetectorSurface->SetType(dielectric_metal);
    lightGuideDetectorSurface->SetFinish(ground);
    lightGuideDetectorSurface->SetModel(unified);
    G4MaterialPropertiesTable* detectorSurfaceMPT = new G4MaterialPropertiesTable();
    std::vector<G4double> reflectivity(21, 0.1);
    // std::vector<G4double> efficiency = { 0.055,0.06,0.065,0.07,0.08,
    //                                      0.09,0.10,0.11,0.13,0.15,
    //                                      0.17,0.20,0.24,0.28,0.33,
    //                                      0.37,0.40,0.41,0.40,0.37,0.34 };
    std::vector<G4double> efficiency(21,1.);
    detectorSurfaceMPT->AddProperty("REFLECTIVITY", cryPhotonEnergies, reflectivity);
    detectorSurfaceMPT->AddProperty("EFFICIENCY", cryPhotonEnergies, efficiency);
    lightGuideDetectorSurface->SetMaterialPropertiesTable(detectorSurfaceMPT);
    new G4LogicalBorderSurface("LightGuideDetectorSurface", lightGuidePhysical, detectorPhysical, lightGuideDetectorSurface);

    
    return worldPhysical;
}