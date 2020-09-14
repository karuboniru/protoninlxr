
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
  if (argc == 1)
  {
    ui = new G4UIExecutive(argc, argv);
  }

#ifdef G4MULTITHREADED
  G4MTRunManager *runManager = new G4MTRunManager;
  runManager->SetNumberOfThreads(std::thread::hardware_concurrency());
#else
  G4RunManager *runManager = new G4RunManager;
#endif

  runManager->SetUserInitialization(new DetectorConstruction());
  auto physicsList = new QBBC;
  // G4VModularPhysicsList *physicsList = new G4VModularPhysicsList;
  // physicsList->RegisterPhysics( new G4HadronElasticPhysics() );
  // physicsList->RegisterPhysics( new G4StoppingPhysics() );
  // physicsList->RegisterPhysics( new G4IonPhysics());
  // physicsList->RegisterPhysics( new G4EmStandardPhysics() );
  // physicsList->RegisterPhysics( new G4EmExtraPhysics() );
  // physicsList->DumpCutValuesTable();
  physicsList->SetVerboseLevel(2);
  runManager->SetUserInitialization(physicsList);
  // User action initialization
  runManager->SetUserInitialization(new ActionInitialization());

  // Initialize visualization
  //
  G4VisManager *visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();

  // Get the pointer to the User Interface manager
  G4UImanager *UImanager = G4UImanager::GetUIpointer();

  // Process macro or start UI session
  //
  if (!ui)
  {
    // batch mode
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command + fileName);
  }
  else
  {
    // interactive mode
    UImanager->ApplyCommand("/control/execute init_vis.mac");
    ui->SessionStart();
    delete ui;
  }

  delete visManager;
  delete runManager;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
