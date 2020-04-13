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
    if (step->GetPreStepPoint()->GetProcessDefinedStep() != nullptr)
    {

      // G4cout << step->GetPreStepPoint()->GetKineticEnergy()/MeV<< "\t" << step->GetPreStepPoint()->GetProcessDefinedStep()->GetProcessName() << "\t" <<  step->GetPostStepPoint()->GetKineticEnergy()/MeV << G4endl;
      // G4cout << step->GetPreStepPoint()->GetProcessDefinedStep()->GetProcessSubType() << G4endl;
      if (std::any_of(elasticlist.begin(), elasticlist.end(), [&](auto &str) { return str == step->GetPreStepPoint()->GetProcessDefinedStep()->GetProcessName(); }))
      {
        fEventAction->addel(1);
      }
      else if (std::any_of(inelasticlist.begin(), inelasticlist.end(), [&](auto str) { return str == step->GetPreStepPoint()->GetProcessDefinedStep()->GetProcessName(); }))
      {
        fEventAction->addnel(1);
      }
      if (step->GetPostStepPoint()->GetKineticEnergy() / MeV < 0.01)
      {
        if (step->GetPreStepPoint()->GetProcessDefinedStep()->GetProcessName() == "protonInelastic")
          fEventAction->setel(1);
        else
          fEventAction->setel(0);
      }
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
