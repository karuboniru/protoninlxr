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
  EventAction(RunAction *runAction, HistoManager *hist);
  virtual ~EventAction();

  virtual void BeginOfEventAction(const G4Event *event);
  virtual void EndOfEventAction(const G4Event *event);

  void SetZ(G4double z) { len = len > z ? len : z; }
  void setDisppearMode(G4int nel) { disappear_mode = nel; }
  void trySetStopMode(G4int mode) { stop_mode = (stop_mode == -1 ? mode : stop_mode); }
  void addel(G4bool x) { countel += x; }
  void addnel(G4bool x) { countnel += x; }
  G4double getlen() { return len; };

private:
  RunAction *fRunAction;
  G4double len;
  HistoManager *hist;
  G4int countel, countnel;
  G4int disappear_mode;
  G4int stop_mode;
};
