#pragma once

#include "globals.hh"
#include "g4root.hh"
#include "modes.hh"
#include <tuple>
#include <vector>

class HistoManager
{
public:
    HistoManager();
    ~HistoManager();

    void Book();
    void Save();

    void FillNtuple(G4double E_in, G4double E_vis, G4double L_muon, bool isCC);
    // void SetE_in(G4double E_in);

private:
    G4bool fFactoryOn;
};
