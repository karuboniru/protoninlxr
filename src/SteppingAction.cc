#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include <string>
#include <vector>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

std::vector<std::string> elasticlist{"CoulombScat", "hadElastic", "msc"};
std::vector<std::string> inelasticlist{"hIoni", "protonInelastic", "hBrems", "hPairProd"};

SteppingAction::SteppingAction(EventAction *eventAction)
    : G4UserSteppingAction(),
      fEventAction(eventAction),
      fScoringVolume(0)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step *step)
{
  if (step->GetTrack()->GetTrackID() == 1)
  {
    fEventAction->AddEdep(step->GetTrack()->GetStepLength());
    if (step->GetPostStepPoint()->GetProcessDefinedStep() != nullptr)
    {
      if (std::any_of(elasticlist.begin(), elasticlist.end(), [&](auto &str) { return str == step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName(); }))
      {
        fEventAction->addel(1);
      }
      else if (std::any_of(inelasticlist.begin(), inelasticlist.end(), [&](auto str) { return str == step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName(); }))
      {
        fEventAction->addnel(1);
      }
      else
      {
        G4cout << "Another: \t" << step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() << G4endl;
      }
    }
    if (step->GetPostStepPoint()->GetProcessDefinedStep() != nullptr && step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() == "protonInelastic")
    {
      fEventAction->setel(1);
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
