#pragma once

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "Hist.hh"
class RunAction;

/// Event action class
///

class EventAction : public G4UserEventAction
{
  public:
    EventAction(RunAction* runAction, HistoManager* hist);
    virtual ~EventAction();

    virtual void BeginOfEventAction(const G4Event* event);
    virtual void EndOfEventAction(const G4Event* event);

    void AddEdep(G4double dlen) { len += dlen; }
    void setel(G4bool nel){mnel = nel;}

  private:
    RunAction* fRunAction;
    G4double     len;
    HistoManager* hist;
    G4bool mnel ;
};
