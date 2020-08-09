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
    // auto _file1 = new Tile("pi+.root");
    TTree *Tree = nullptr;
    // TTree *Tree1 = nullptr;
    auto leg = new TLegend(.8, .8, .95, .95);
    auto hs = new THStack("hs", (std::string("Stack by ") + (argc == 1 ? "stop_mode" : argv[1])).c_str());
    _file0->GetObject("Ntuple1", Tree);
    // _file1->GetObject("Ntuple1", Tree1);
    std::vector<TH2D *> hists;
    // std::vector<TPaveStats *> tphs;
    auto c1 = new TCanvas();
    // c1->SetLogy();
    gStyle->SetOptStat(0);
    for (long unsigned int i = 0; i < list.size(); i++)
    {
        if (Tree->Draw(("Depth:enddedx>>hist" + std::to_string(i)).c_str(),
                       ("enddedx<40&&"+ std::string((argc == 1 ? "stop_mode" : argv[1])) + std::string("==") + std::to_string(i)).c_str(), "gOff") != 0)
        {
            hists.push_back((TH2D *)gDirectory->Get(("hist" + std::to_string(i)).c_str()));
            hists[hists.size() - 1]->SetLineColor(i+1);
            hists[hists.size() - 1]->SetFillStyle(1001);
            hists[hists.size() - 1]->SetFillColorAlpha(i+1, 0.5);
            hists[hists.size() - 1]->SetMarkerColor(i+1);
            hists[hists.size() - 1]->SetTitle(list[i].c_str());
            // hists[hists.size() - 1]->Scale(1,"width");
            // hists[hists.size() - 1]->SetBinsLength(1);
            leg->AddEntry(hists[hists.size() - 1], list[i].c_str());
            // tphs.push_back((TPaveStats *)(hists[hists.size() - 1]->GetListOfFunctions()->FindObject("stats")));
        }
    }
    //     for (long unsigned int i = 0; i < list.size(); i++)
    // {
    //     if (Tree1->Draw(("enddedx:Depth>>hist" + std::to_string(i)).c_str(),
    //                    ((argc == 1 ? "stop_mode" : argv[1]) + std::string("==") + std::to_string(i)+"&&enddedx<10").c_str(), "gOff") != 0)
    //     {
    //         hists.push_back((TH2D *)gDirectory->Get(("hist" + std::to_string(i)).c_str()));
    //         hists[hists.size() - 1]->SetLineColor(i);
    //         hists[hists.size() - 1]->SetFillStyle(1001);
    //         hists[hists.size() - 1]->SetFillColorAlpha(i, 0.5);
    //         hists[hists.size() - 1]->SetMarkerColor(i);
    //         hists[hists.size() - 1]->SetTitle(list[i].c_str());
    //         leg->AddEntry(hists[hists.size() - 1], list[i].c_str());
    //         // tphs.push_back((TPaveStats *)(hists[hists.size() - 1]->GetListOfFunctions()->FindObject("stats")));
    //     }
    // }
    {
        int max = 0, maxX = 0, maxY = 0;
        for (auto &i : hists)
        {
            i->GetXaxis()->SetLimits(0, 80);
            i->GetYaxis()->SetLimits(0, 40);
            i->Scale(1, "width");
            i = (TH2D*)(i->Rebin2D());
            // max = max > i->GetMaximum() ? max : i->GetMaximum();
            // hs->SetMaximum(max);
            hs->Add(i, "");
        }
    }
    hs->SetTitle((std::string("Stack by ") + (argc == 1 ? "stop_mode" : argv[1]) + ";depth(cm);dedx(MeV/cm)").c_str());
    hs->Draw("box");
    // hs->GetYaxis()->SetRangeUser(0., 80.);
    // hs->GetYaxis()->SetRangeUser(0., 40.);
    leg->Draw();
    c1->Draw();
    ((TRootCanvas *)c1->GetCanvasImp())->Connect("CloseWindow()", "TApplication", app, "Terminate()");
    app->Run();
    delete app;
    delete c1;
    delete Tree;
    delete _file0;
}
