#include <TROOT.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
#include <TApplication.h>
#include <TTree.h>
#include <TH2D.h>
#include <THStack.h>
#include <TFile.h>
#include <TLegend.h>
#include <TAxis.h>
#include <TRootCanvas.h>
#include <TPaveStats.h>
#include <TObjArray.h>
#include <TVirtualHistPainter.h>
#include "modes.hh"

std::vector<int> colors = {632, 416, 600, 400, 616, 432, 800, 820, 840, 860, 880, 900};

int main(int argc, char **argv)
{
    auto app = new TApplication("app", &argc, argv);
    auto _file0 = new TFile("RootOut.root");
    TTree *Tree = nullptr;
    auto leg = new TLegend(.8, .8, .95, .95);
    auto hs = new THStack("hs", "hs");
    _file0->GetObject("Ntuple1", Tree);
    std::vector<TH2D *> hists;
    auto c1 = new TCanvas();
    Tree->Draw("enddedx:Depth>>hist","","box gOff");
    auto hist1 = (TH2D *)gDirectory->Get("hist");
    auto max = hist1->GetXaxis()->GetXmax();
    delete hist1;
    for (long unsigned int i = 0; i < list.size(); i++)
    {
        if (Tree->Draw(("enddedx:Depth>>hist" + std::to_string(i)+ "(40,0,"+ std::to_string(max) +",40,0,70)").c_str(),
                       (std::string((argc == 1 ? "stop_mode" : argv[1])) + std::string("==") + std::to_string(i)).c_str(), "gOff") != 0)
        {
            hists.push_back((TH2D *)gDirectory->Get(("hist" + std::to_string(i)).c_str()));
            hists[hists.size() - 1]->SetLineColor(i+1);
            hists[hists.size() - 1]->SetFillStyle(1001);
            hists[hists.size() - 1]->SetFillColorAlpha(i+1, 0.5);
            hists[hists.size() - 1]->SetMarkerColor(i+1);
            hists[hists.size() - 1]->SetTitle(list[i].c_str());
            hists[hists.size() - 1]->Scale(1, "width");
            leg->AddEntry(hists[hists.size() - 1], list[i].c_str());
        }
    }

    {
        for (auto &i : hists)
        {
            i->Scale(1, "width");
            hs->Add(i, "");
        }
    }
    hs->SetTitle((std::string("Stack by ") + (argc == 1 ? "stop_mode" : argv[1]) + ";Depth (cm);dedx(MeV/cm)").c_str());
    hs->Draw("box");
    leg->Draw();
    c1->Draw();
    ((TRootCanvas *)c1->GetCanvasImp())->Connect("CloseWindow()", "TApplication", app, "Terminate()");
    app->Run();
    delete app;
    delete c1;
    delete Tree;
    delete _file0;
}
