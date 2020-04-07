#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"

ActionInitialization::ActionInitialization()
    : G4VUserActionInitialization()
{
}

ActionInitialization::~ActionInitialization()
{
}

void ActionInitialization::BuildForMaster() const
{
  HistoManager *histo = new HistoManager();
  RunAction *runAction = new RunAction(histo);
  SetUserAction(runAction);
}

void ActionInitialization::Build() const
{
  SetUserAction(new PrimaryGeneratorAction);
  HistoManager *histo = new HistoManager();

  RunAction *runAction = new RunAction(histo);
  SetUserAction(runAction);

  EventAction *eventAction = new EventAction(runAction, hist);
  SetUserAction(eventAction);

  SetUserAction(new SteppingAction(eventAction));
}
