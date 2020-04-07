#pragma once
#include "G4UserSteppingAction.hh"
#include "globals.hh"

class EventAction;

class G4LogicalVolume;

/// Stepping action class
/// 

class SteppingAction : public G4UserSteppingAction
{
  public:
    SteppingAction(EventAction* eventAction);
    virtual ~SteppingAction();

    // method from the base class
    virtual void UserSteppingAction(const G4Step*);

  private:
    EventAction*  fEventAction;
    G4LogicalVolume* fScoringVolume;
};

