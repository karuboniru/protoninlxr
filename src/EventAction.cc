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
      disappear_mode(0),
      stop_mode(-1),
      ionE(0)
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
  disappear_mode = 0;
  stop_mode = -1;
  countel = 0;
  countnel = 0;
  ionE=0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event *)
{
  //fill my Ntuple
  if(len<0){
    G4cout<<"Wrong"<<"z = "<< len<<G4endl;
    return;
  }
  hist->FillNtuple(len, disappear_mode, stop_mode, countel, countnel, ionE);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
