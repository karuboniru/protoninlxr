#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"
// #include "Run.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "Hist.hh"

RunAction::RunAction(HistoManager *his)
    : G4UserRunAction(),
      hist(his)
{
}

RunAction::~RunAction()
{
}

void RunAction::BeginOfRunAction(const G4Run *)
{
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);
  hist->Book();
}

void RunAction::EndOfRunAction(const G4Run *run)
{
  G4cout << "G4Run id:" << run->GetRunID() << " has finished with " << run->GetNumberOfEvent() << " events" << G4endl;
  hist->Save();
}
