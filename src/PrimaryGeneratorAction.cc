#include "PrimaryGeneratorAction.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "chain_helper.h"
#include <iostream>
#include <TString.h>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction(EventAction *hist, my_iter_t &citer)
    : G4VUserPrimaryGeneratorAction(),
      fParticleGun(0),
      fEnvelopeBox(0),
      m_hist(hist),
      chain_iter(citer)
{
    G4int n_particle = 1;
    fParticleGun = new G4ParticleGun(n_particle);

    // // default particle kinematic
    particleTable = G4ParticleTable::GetParticleTable();
    // G4String particleName;
    // G4ParticleDefinition *particle = particleTable->FindParticle(particleName = "proton");
    // fParticleGun->SetParticleDefinition(particle);
    // fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));
    // fParticleGun->SetParticleEnergy(1. * GeV);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent)
{
    // G4double x0 = 0;
    // G4double y0 = 0;
    // G4double z0 = 0;
    const std::lock_guard<std::mutex> lock(iterator_mutex);
    auto &res = *chain_iter;
    auto &StdHepN = *std::get<0>(res);
    auto StdHepPdg = std::get<1>(res);
    auto StdHepStatus = std::get<2>(res);
    auto StdHepP4 = std::get<3>(res);
    auto EvtVtx = std::get<4>(res);
    auto EvtCode = std::get<5>(res);
    for (size_t i = 0; i < StdHepN; i++)
    {
        // std::cout << EvtCode->CopyString() << std::endl; // ERROR: empty EvtCode here! It should not be!
        if (StdHepStatus[i] == 1 && abs(StdHepPdg[i]) < 2000000000)
        {
            G4ParticleDefinition *particle = particleTable->FindParticle(StdHepPdg[i]);
            G4PrimaryVertex *vertex = new G4PrimaryVertex(EvtVtx[i][0] * m, EvtVtx[i][1] * m, EvtVtx[i][2] * m, EvtVtx[i][3] * s);
            // G4PrimaryVertex *vertex = new G4PrimaryVertex(0, 0, 0, 0);
            //Make a g4 particle to go in the vertex
            G4PrimaryParticle *g4particle = new G4PrimaryParticle(particle, StdHepP4[i][0] * GeV, StdHepP4[i][1] * GeV, StdHepP4[i][2] * GeV);
            //Add the particle to the vertex
            vertex->SetPrimary(g4particle);
            //Add the vertex to event and let G4 take its course
            anEvent->AddPrimaryVertex(vertex);
        }
        if (StdHepStatus[i] == 0 && abs(StdHepPdg[i]) == 14)
        {
            m_hist->SetENU(StdHepP4[i][3] * GeV);
        }
        m_hist->SetCC(EvtCode->CopyString().Contains("CC"));
    }
    ++chain_iter;
}