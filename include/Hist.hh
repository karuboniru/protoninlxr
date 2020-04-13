#pragma once

#include "globals.hh"
#include "g4root.hh"

class HistoManager
{
public:
  HistoManager();
  ~HistoManager();

  void Book();
  void Save();

  void FillNtuple(G4double, G4bool, G4int, G4int);

private:
  G4bool fFactoryOn;
};
