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

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// G4double GetDispatch(const G4Step *step){
//   return step->GetPostStepPoint()->GetPosition().z();
// }

void SteppingAction::UserSteppingAction(const G4Step *step)
{
  fEventAction->addIonE(step->GetTotalEnergyDeposit());
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
        // G4cout << "Another: \t" << step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() << G4endl;
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
      // if(mode == -1){
      //   G4cout<< step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() << "not recorded"<<G4endl;
      // }
      fEventAction->setDisppearMode(mode);
      auto ran = G4UniformRand();
      hist->RecordStep(step->GetPreStepPoint()->GetPosition().z()+ran*(step->GetPostStepPoint()->GetPosition().z()-step->GetPreStepPoint()->GetPosition().z()),
                       mode,
                       ((step->GetTotalEnergyDeposit()) / MeV) / (ran * step->GetStepLength()),
                       ((step->GetTotalEnergyDeposit()) / MeV),
                       step->GetPreStepPoint()->GetPosition().z(),
                       step->GetPostStepPoint()->GetPosition().z());
      if (step->GetPostStepPoint()->GetKineticEnergy() == 0||step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() == "Decay")
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
