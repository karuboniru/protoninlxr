#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include "modes.hh"
#include "Randomize.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(EventAction *eventAction, HistoManager *his)
    : G4UserSteppingAction(),
      fEventAction(eventAction),
      fScoringVolume(0),
      hist(his)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction()
{
}

void SteppingAction::UserSteppingAction(const G4Step *step)
{
    fEventAction->addE(step->GetTotalEnergyDeposit());
    if (step->GetTrack()->GetParticleDefinition()->GetParticleName().contains("mu+") || step->GetTrack()->GetParticleDefinition()->GetParticleName().contains("mu-"))
    {
        fEventAction->setL(step->GetPostStepPoint()->GetPosition().getR());
        // fEventAction->AddL(step->GetStepLength());
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
