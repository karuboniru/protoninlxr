#include "EventAction.hh"
#include "RunAction.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "Hist.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction(RunAction *runAction, HistoManager *his)
    : G4UserEventAction(),
      fRunAction(runAction),
      len(0.),
      hist(his),
      E_vis(0)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event *)
{
  len = 0.;
  E_vis = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event *)
{
  //fill my Ntuple
  if(len<0){
    G4cout<<"Wrong"<<"z = "<< len<<G4endl;
    return;
  }
  hist->FillNtuple(E_nu, E_vis, len, isCC);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
