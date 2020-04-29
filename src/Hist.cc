
#include "Hist.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include <string>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::HistoManager()
    : fFactoryOn(false)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::~HistoManager()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Book()
{
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetVerboseLevel(0);
    analysisManager->SetNtupleMerging(true);

    G4bool fileOpen = analysisManager->OpenFile("RootOut");
    if (!fileOpen)
    {
        G4cerr << "\n---> HistoManager::Book(): cannot open "
               << analysisManager->GetFileName() << G4endl;
        return;
    }
    analysisManager->CreateNtuple("Ntuple1", "123");
    analysisManager->CreateNtupleDColumn(0, "Depth"); // column Id = 0
    analysisManager->CreateNtupleIColumn(0, "disappear_mode");
    analysisManager->CreateNtupleIColumn(0, "stop_mode");
    analysisManager->CreateNtupleIColumn(0, "cel");
    analysisManager->CreateNtupleIColumn(0, "cnel");
    analysisManager->FinishNtuple();
    analysisManager->CreateNtuple("process", "per_step");
    analysisManager->CreateNtupleDColumn(1, "Depth");
    analysisManager->CreateNtupleIColumn(1, "process");
    analysisManager->CreateNtupleDColumn(1, "dedx");
    analysisManager->CreateNtupleDColumn(1, "de");
    analysisManager->FinishNtuple();


    fFactoryOn = true;

    G4cout << "\n----> Output file is open in "
           << analysisManager->GetFileName() << "."
           << analysisManager->GetFileType() << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Save()
{
    if (!fFactoryOn)
        return;

    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();

    G4cout << "\n----> Histograms and ntuples are saved\n"
           << G4endl;

    delete G4AnalysisManager::Instance();
    fFactoryOn = false;
}

void HistoManager::FillNtuple(G4double length, G4int disappearmode,G4int stopmode,  G4int cel, G4int cnel)
{
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillNtupleDColumn(0, 0, length/cm);
    analysisManager->FillNtupleIColumn(0, 1, disappearmode);
    analysisManager->FillNtupleIColumn(0, 2, stopmode);
    analysisManager->FillNtupleIColumn(0, 3, cel);
    analysisManager->FillNtupleIColumn(0, 4, cnel);
    analysisManager->AddNtupleRow(0);
}
void HistoManager::RecordStep(G4double len, G4int mode, G4double dedx, G4double de)
{
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillNtupleDColumn(1, 0, len/cm);
    analysisManager->FillNtupleIColumn(1, 1, mode);
    analysisManager->FillNtupleDColumn(1, 2, dedx);
    analysisManager->FillNtupleDColumn(1, 3, de);
    analysisManager->AddNtupleRow(1);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
