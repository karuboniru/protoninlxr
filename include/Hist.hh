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

  void FillNtuple(G4double, G4int, G4int, G4int, G4int);
  void RecordStep(G4double, G4int, G4double, G4double);
  void record_ending_range_de();
  std::tuple<G4double, std::vector<G4double>> getEndDedx();

private:
  std::vector<std::tuple<G4double, G4double, G4int>> range_de_mode;
  G4bool fFactoryOn;
};
