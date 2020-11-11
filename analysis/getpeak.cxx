#include <iostream>
#include <fstream>
#include <TApplication.h>
#include <TTree.h>
#include <TH1F.h>
#include <THStack.h>
#include <TFile.h>
#include <TLegend.h>
#include <TAxis.h>
#include <TPaveStats.h>
#include <TObjArray.h>
#include <TVirtualHistPainter.h>
#include <TSpectrum.h>
#include <TVirtualFitter.h>
#include <TMath.h>
#include <TF1.h>
#include <tuple>
#include "modes.hh"

double peaks(TH1F *h)
{
    int npeaks = 5;
    Double_t par[3000];
    TH1F *h2 = (TH1F *)h->Clone("h2");
    //Use TSpectrum to find the peak candidates
    TSpectrum *s = new TSpectrum(2 * npeaks);
    Int_t nfound = s->Search(h, 2, "new");
    double *xs = s->GetPositionX();
    double *ys = s->GetPositionY();
    auto n = s->GetNPeaks();
    int max_index = -1;
    double max_value = -1;
    for (int i = 0; i < n; i++)
    {
        if (ys[i] > max_value)
        {
            max_index = i;
            max_value = ys[i];
        }
    }
    double fit_x = xs[max_index], fit_y = 100000, fit_sigma = 1, fit_back = 10;
    // double range_x = 
    // auto func = new TF1("f1", "[0]*exp(-0.5*((x-[1])/[2])^2)/([2] *TMath::Sqrt(2*TMath::Pi()))+[3]", xs[max_index] - 1, xs[max_index] + 1);
    // func->SetParameter(0, fit_y);
    // func->SetParameter(1, fit_x);
    // func->SetParameter(2, fit_sigma);
    // func->SetParameter(3, fit_back);
    // h->Fit(func, "", "", xs[max_index] - 1, xs[max_index] + 1);
    // return std::make_tuple(func->GetParameter(1), func->GetParameter(2));
    return xs[max_index];
}

int main(int argc, char **argv)
{
    auto _file0 = new TFile("RootOut.root");
    std::fstream *f1, *f2;
    if (argc > 1)
    {
        f1 = new std::fstream(argv[1] + std::string(".depth"), std::fstream::app);
        f2 = new std::fstream(argv[1] + std::string(".dedx"), std::fstream::app);
        if (!(f1->is_open() && f2->is_open()))
        {
            return 1;
        }
    }
    TTree *Tree = nullptr;
    _file0->GetObject("Ntuple1", Tree);
    Tree->Draw("Depth>>hist_depth",
               "stop_mode==1||stop_mode==11", "goff");
    auto hist_depth = (TH1F *)(gDirectory->Get("hist_depth"));
    auto mean_depth = hist_depth->GetMean();
    auto mean_depth_err = hist_depth->GetMeanError();
    auto res_depth = peaks(hist_depth);

    Tree->Draw("enddedx>>hist_dedx",
               "stop_mode==1||stop_mode==11", "goff");
    auto hist_dedx = (TH1F *)(gDirectory->Get("hist_dedx"));
    auto mean_dedx = hist_dedx->GetMean();
    auto mean_dedx_err = hist_dedx->GetMeanError();
    auto res_dedx = peaks(hist_dedx);
    // if (argc == 1)
    // {
    //     std::cout << "depth:\tpos\t" << std::get<0>(res_depth) << "\tres:\t" << std::get<1>(res_depth) << std::endl;
    //     std::cout << "dedx:\tpos\t" << std::get<0>(res_dedx) << "\tres:\t" << std::get<1>(res_dedx) << std::endl;
    // }
    // else
    {
        *f1 << mean_depth << "\t" << mean_depth_err << std::endl;
        *f2 << mean_dedx << "\t" << mean_dedx_err << std::endl;
    }

    delete Tree;
    delete _file0;
}