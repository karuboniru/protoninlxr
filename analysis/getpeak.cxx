#include <TCanvas.h>
#include <iostream>
#include <TApplication.h>
#include <TTree.h>
#include <TH1F.h>
#include <THStack.h>
#include <TFile.h>
#include <TLegend.h>
#include <TAxis.h>
#include <TRootCanvas.h>
#include <TPaveStats.h>
#include <TObjArray.h>
#include <TVirtualHistPainter.h>
#include <TSpectrum.h>
#include <TVirtualFitter.h>
#include "modes.hh"

void peaks(TH1F *h)
{
    int npeaks = 5;
    TH1F *h2 = (TH1F *)h->Clone("h2");
    //Use TSpectrum to find the peak candidates
    TSpectrum *s = new TSpectrum(2 * npeaks);
    Int_t nfound = s->Search(h, 1, "new");
    // printf("Found %d candidate peaks to fitn", nfound);
    s->Print();
}

int main(int argc, char **argv)
{
    auto _file0 = new TFile(argc == 1 ? "RootOut.root" : argv[1]);
    TTree *Tree = nullptr;
    _file0->GetObject("Ntuple1", Tree);
    Tree->Draw("Depth>>hist_depth",
               "stop_mode==1", "goff");
    auto hist_depth = (TH1F *)(gDirectory->Get("hist_depth"));
    peaks(hist_depth);
    Tree->Draw("enddedx>>hist_dedx",
               "stop_mode==1", "goff");
    auto hist_dedx = (TH1F *)(gDirectory->Get("hist_dedx"));
    peaks(hist_dedx);

    delete Tree;
    delete _file0;
}