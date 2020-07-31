#include <TROOT.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
#include <TApplication.h>
#include <TTree.h>
#include <TH1D.h>
#include <TH1D.h>
#include <THStack.h>
#include <TFile.h>
#include <TAxis.h>
#include <TLegend.h>
#include <TAxis.h>
#include <TRootCanvas.h>
#include "modes.hh"

std::vector<int> colors = {632, 416, 600, 400, 616, 432, 800, 820, 840, 860, 880, 900};
double getSumAfterCut(TTree *tree, const std::string &cut, const std::string &variable)
{
    TH1D *temp_hist;
    if ((tree->Draw((variable + ">>temp_hist").c_str(), cut.c_str(), "gOff")) != 0 && ((temp_hist = (TH1D *)(gDirectory->Get("temp_hist"))) != nullptr))
    {
        auto sum = temp_hist->GetMean() * temp_hist->GetEntries();
        delete temp_hist;
        return sum;
    }
    else
    {
        return 0;
    }
}

double getEdgeOfTree(TTree *tree, const std::string &variable)
{
    TH1D *temp_hist;
    if ((tree->Draw((variable + ">>temp_hist").c_str(), "", "gOff")) != 0 && (temp_hist = (TH1D *)(gDirectory->Get("temp_hist"))) != nullptr)
    {
        temp_hist = (TH1D *)(gDirectory->Get("temp_hist"));
        auto axis = temp_hist->GetXaxis();
        auto max = axis->GetXmax();
        delete temp_hist;
        return max;
    }
    else
        return -1;
}
std::vector<double> getMeshList(double stop, int points)
{
    std::vector<double> bins;
    for (int i = 0; i <= points; i++)
    {
        bins.push_back(stop / points * i); //{0, step, 2step,...,stop}
    }
    return bins;
}
int getcount(TFile *file)
{
    TTree *Tree = nullptr;
    file->GetObject("Ntuple1", Tree);
    auto count = Tree->Draw("Depth", "", "gOff");
    delete Tree;
    return count;
}

int main(int argc, char **argv)
{
    auto app = new TApplication("app", &argc, argv);
    auto _file0 = new TFile("RootOut.root");
    TTree *Tree = nullptr;
    auto leg = new TLegend(.82, .82, .95, .95);
    auto hs = new THStack("hs", (std::string("Stack by ") + "step process").c_str());
    _file0->GetObject("process", Tree);
    std::vector<TH1D *> hists;
    auto c1 = new TCanvas();
    auto count = getcount(_file0);
    constexpr int ranges = 50;
    double step = 0;
    auto bins = getMeshList(step = getEdgeOfTree(Tree, "Depth"), ranges);
    step /= ranges;
    double sum = 0;
    for (long unsigned int mode = 0; mode < list.size(); mode++)
    {
        if (Tree->Draw("Depth", (std::string("process==") + std::to_string(mode)).c_str(), "gOff") == 0)
            continue;
        hists.push_back(new TH1D(list[mode].c_str(), list[mode].c_str(), ranges, &bins[0]));
        for (int i = 0; i < ranges; i++)
        {
            auto dedx = getSumAfterCut(Tree, "Depth>=" + std::to_string(bins[i]) + "&&Depth <" + std::to_string(bins[i + 1]) + "&&process==" + std::to_string(mode), "de") / step / count;
            hists[hists.size() - 1]->AddBinContent(i, dedx);
        }
        hists[hists.size() - 1]->SetLineColor(colors[hists.size() - 1]);
        hists[hists.size() - 1]->SetFillStyle(1001);
        hists[hists.size() - 1]->SetFillColorAlpha(colors[hists.size() - 1], 0.5);
        hists[hists.size() - 1]->SetTitle(list[mode].c_str());
        leg->AddEntry(hists[hists.size() - 1], list[mode].c_str());
        sum += hists[hists.size() - 1]->Integral();
    }
    std::cout << sum << std::endl;
    int max = 0;
    for (auto &i : hists)
    {
        // std::cout<< i->GetMaximum() <<std::endl;
        max = max > i->GetMaximum() ? max : i->GetMaximum();
        hs->SetMaximum(2.0 * max);
        hs->Add(i);
    }
    hs->Draw("");
    hs->SetTitle("dedv vs. pos; pos at z(cm); dedx(MeV/cm)");
    ((TRootCanvas *)c1->GetCanvasImp())->Connect("CloseWindow()", "TApplication", app, "Terminate()");
    leg->Draw();
    c1->Draw();
    app->Run();
    delete app;
    delete c1;
    delete Tree;
    delete _file0;
}
