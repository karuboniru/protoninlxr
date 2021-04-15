
#pragma once
#include "G4VUserActionInitialization.hh"
#include "Hist.hh"
#include "chain_helper.h"

/// Action initialization class.

class ActionInitialization : public G4VUserActionInitialization
{
  public:
    ActionInitialization(my_iter_t &citer);
    virtual ~ActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;
    // HistoManager * hist;
    private:
      my_iter_t &iter;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


    
