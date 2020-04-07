#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

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
    // fEventAction->setel(step->GetNonIonizingEnergyDeposit());
    if (step->GetTrack()->GetKineticEnergy() < 2.4 * MeV)
    {
      // step->GetTrack()->GetCreatorProcess()->GetProcessType();
      if (step->GetTrack()->GetKineticEnergy() != 0)
      {
        fEventAction->setel(0);
      }
    }
    else
    {
      fEventAction->setel(1);
      // G4cout << step->GetTrack()->GetKineticEnergy() /MeV <<G4endl;
    }
  }
  // else
  // {

  //   // G4cout<< step->GetTrack()->GetCreatorModelName() << "\t"<< step->GetTrack()->GetDefinition()->GetParticleName()  <<G4endl;
  // }
  // if (step->GetTrack()->GetParentID()==1)
  // {
  //   auto name = step->GetTrack()->GetParticleDefinition()->GetParticleName();
  //   if ( name != "Ar38" && name != "gamma" && name != "proton" && name != "e-"){
  //     fEventAction->setel(1);
  //     // G4cout << "GetParticleName() gets " << step->GetTrack()->GetParticleDefinition()->GetParticleName()<<G4endl;
  //   }
  // }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
