
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
    // analysisManager->CreateNtupleDColumn(0, "enddedx");
    analysisManager->FinishNtuple();
    analysisManager->CreateNtuple("process", "per_step");
    analysisManager->CreateNtupleDColumn(1, "Depth");
    analysisManager->CreateNtupleIColumn(1, "process");
    analysisManager->CreateNtupleDColumn(1, "dedx");
    analysisManager->CreateNtupleDColumn(1, "de");
    analysisManager->FinishNtuple();
    analysisManager->CreateNtuple("ending", "ending_record");
    analysisManager->CreateNtupleDColumn(2, "range");
    analysisManager->CreateNtupleDColumn(2, "dedx");
    analysisManager->CreateNtupleIColumn(2, "process");
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

void HistoManager::FillNtuple(G4double length, G4int disappearmode, G4int stopmode, G4int cel, G4int cnel)
{
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillNtupleDColumn(0, 0, length / cm);
    analysisManager->FillNtupleIColumn(0, 1, disappearmode);
    analysisManager->FillNtupleIColumn(0, 2, stopmode);
    analysisManager->FillNtupleIColumn(0, 3, cel);
    analysisManager->FillNtupleIColumn(0, 4, cnel);
    // analysisManager->FillNtupleDColumn(0, 5, getEndDedx());
    analysisManager->AddNtupleRow(0);
    record_ending_range_de();
}
void HistoManager::RecordStep(G4double len, G4int mode, G4double dedx, G4double de)
{
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillNtupleDColumn(1, 0, len / cm);
    analysisManager->FillNtupleIColumn(1, 1, mode);
    analysisManager->FillNtupleDColumn(1, 2, dedx);
    analysisManager->FillNtupleDColumn(1, 3, de);
    analysisManager->AddNtupleRow(1);
    range_de_mode.push_back(std::make_tuple(len / cm, de, mode));
}

std::tuple<G4double, std::vector<G4double>> HistoManager::getEndDedx()
{
    const int n = range_de_mode.size();
    G4double range = std::get<0>(range_de_mode[n - 1]);
    std::vector<G4double> de;
    de.resize(list.size());
    for (auto &i : de)
    {
        i = 0;
    }
    for (int i = n - 1; i >= 0; i--)
    {
        if (std::get<0>(range_de_mode[i]) > 0.9 * range)
        {
            de[std::get<2>(range_de_mode[i])] += std::get<1>(range_de_mode[i]);
        }
        else
        {
            break;
        }
    }
    range_de_mode.clear();
    return make_tuple(range, de);
}

void HistoManager::record_ending_range_de()
{
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
    const auto t = getEndDedx();
    const auto de = std::get<1>(t);
    const auto range = std::get<0>(t);
    for (int i = 0; i < list.size(); i++)
    {
        if (de[i] != 0)
        {
            analysisManager->FillNtupleDColumn(2, 0, range);
            analysisManager->FillNtupleDColumn(2, 1, de[i]/(0.1*range));
            analysisManager->FillNtupleIColumn(2, 2, i);
            analysisManager->AddNtupleRow(2);
        }
    }
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
