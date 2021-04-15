
#include "Hist.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include <string>
#include <iterator>
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

    G4bool fileOpen = analysisManager->OpenFile("here");
    if (!fileOpen)
    {
        G4cerr << "\n---> HistoManager::Book(): cannot open "
               << analysisManager->GetFileName() << G4endl;
        return;
    }
    analysisManager->CreateNtuple("Ntuple1", "123");
    analysisManager->CreateNtupleDColumn(0, "E"); // column Id = 0
    analysisManager->CreateNtupleDColumn(0, "E_vis");
    analysisManager->CreateNtupleDColumn(0, "len");
    analysisManager->CreateNtupleIColumn(0, "isCC");
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

void HistoManager::FillNtuple(G4double E_in, G4double E_vis, G4double L_muon, bool isCC)
{
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillNtupleDColumn(0, 0, E_in/MeV);
    analysisManager->FillNtupleDColumn(0, 1, E_vis/MeV);
    analysisManager->FillNtupleDColumn(0, 2, L_muon / m);
    analysisManager->FillNtupleIColumn(0, 3, isCC);
    analysisManager->AddNtupleRow(0);
}

// void HistoManager::SetE_in(G4double E_in)
// {
//     G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
//     analysisManager->FillNtupleDColumn(0, 0, E_in/MeV);
// }
