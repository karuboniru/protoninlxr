
#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"
#include "QBBC.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4StoppingPhysics.hh"
#include "G4IonPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmExtraPhysics.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "Randomize.hh"
#include "Hist.hh"

#include "Randomize.hh"
#include "time.h"
#include <thread>
#include <mutex>
#include <chain_helper.h>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc, char **argv)
{
  // Detect interactive mode (if no arguments) and define UI session
  //
  //choose the Random engine
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine());
  //set random seed with system time
  G4long seed = time(NULL);
  CLHEP::HepRandom::setTheSeed(seed);

  G4UIExecutive *ui = 0;

#ifdef G4MULTITHREADED
  G4MTRunManager *runManager = new G4MTRunManager;
  runManager->SetNumberOfThreads(std::thread::hardware_concurrency());
#else
  G4RunManager *runManager = new G4RunManager;
#endif

  runManager->SetUserInitialization(new DetectorConstruction());
  auto physicsList = new QBBC;
  physicsList->SetVerboseLevel(2);
  runManager->SetUserInitialization(physicsList);
  // User action initialization
  root_chain<int, int[100], int[100], double[100][4], double[100][4], TObjString>
      chain("/home/yan/neutrino/generated/flat/G00_00b_00_000@flat@Nu/Filelist.list", "gRooTracker", {"StdHepN", "StdHepPdg", "StdHepStatus", "StdHepP4", "EvtVtx", "EvtCode"});
  auto iter = chain.begin();
  runManager->SetUserInitialization(new ActionInitialization(iter));
  const auto max_hard = 50000;
  auto max = chain.get_up() > max_hard ? max_hard : chain.get_up();
  // auto max = chain.get_up();
  // Get the pointer to the User Interface manager
  G4UImanager *UImanager = G4UImanager::GetUIpointer();
  // batch mode
  G4String command = "/run/beamOn ";
  // G4String fileName = argv[1];
  UImanager->ApplyCommand("/run/initialize");
  UImanager->ApplyCommand(command + std::to_string(max));

  delete runManager;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
