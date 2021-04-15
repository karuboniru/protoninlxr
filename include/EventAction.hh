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

  void AddL(G4double dl) { len += dl; }
  void setL(G4double dl) { len = len>dl?len:dl; }
  void addE(G4double x) { E_vis += x; }
  G4double getlen() { return len; };
  void SetENU(double E) { E_nu = E; }
  void SetCC(bool is) { isCC = is; }

private:
  RunAction *fRunAction;
  G4double len;
  G4double E_vis;
  HistoManager *hist;
  G4double E_nu;
  bool isCC = 0;
};
