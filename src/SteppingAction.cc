#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include "modes.hh"
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

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step *step)
{
  if (step->GetTrack()->GetTrackID() == 1)
  {
    fEventAction->SetZ(step->GetPostStepPoint()->GetPosition().z());
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
    // if (step->GetPostStepPoint()->GetProcessDefinedStep() != nullptr) //stopped~
    {
      auto mode = std::distance(
          list.begin(),
          std::find(
              list.begin(),
              list.end(),
              step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName()));
      fEventAction->setDisppearMode(mode);
      hist->RecordStep(step->GetPostStepPoint()->GetPosition().z(),
                       mode,
                       ((step->GetPreStepPoint()->GetKineticEnergy() - step->GetPostStepPoint()->GetKineticEnergy()) / MeV) / (step->GetStepLength()),
                       ((step->GetPreStepPoint()->GetKineticEnergy() - step->GetPostStepPoint()->GetKineticEnergy()) / MeV));
      if (step->GetPostStepPoint()->GetKineticEnergy() == 0)
      {
        fEventAction->trySetStopMode(mode);
      }
      // G4double dz;
      // if ((dz=(step->GetPostStepPoint()->GetPosition().z() - step->GetPreStepPoint()->GetPosition().z()))<0)
      //   G4cout<<"dz<0!\t ="<<dz<< "@ z = "<< fEventAction->getlen()<< G4endl;
    }
  }
  else
  {
    step->GetTrack()->SetTrackStatus(fStopAndKill);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
