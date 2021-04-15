#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"

ActionInitialization::ActionInitialization(my_iter_t &citer)
    : G4VUserActionInitialization(), iter(citer)
{
}

ActionInitialization::~ActionInitialization()
{
}

void ActionInitialization::BuildForMaster() const
{
  // SetUserAction(new PrimaryGeneratorAction);
  HistoManager *histo = new HistoManager();

  RunAction *runAction = new RunAction(histo);
  SetUserAction(runAction);
}

void ActionInitialization::Build() const
{
  HistoManager *histo = new HistoManager();
  RunAction *runAction = new RunAction(histo);
  SetUserAction(runAction);

  EventAction *eventAction = new EventAction(runAction, histo);
  SetUserAction(new PrimaryGeneratorAction(eventAction, iter));

  SetUserAction(eventAction);

  SetUserAction(new SteppingAction(eventAction, histo));
}
