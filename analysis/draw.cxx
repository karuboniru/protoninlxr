#include <TCanvas.h>
#include <TApplication.h>
#include <TTree.h>
#include <TFile.h>
#include <TRootCanvas.h>
#include <fstream>
#include <string>
#include <vector>
#include <TGraphErrors.h>
#include <TMultiGraph.h>
#include <TLegend.h>
#include <iostream>

TGraphErrors *getGraph(std::string s)
{
    std::fstream dep(s + ".depth"), dedx(s + ".dedx");
    std::vector<double> ndep, ndep_s, ndedx, ndedx_s;
    for (int i = 0; !(dep.eof() || dedx.eof());)
    {
        double x1,x2,x3,x4;
        dep >> x1;
        ndep.push_back(x1);
        dep >> x2;
        ndep_s.push_back(x2);
        dedx >> x3;
        ndedx.push_back(x3);
        dedx >> x4;
        ndedx_s.push_back(x4);
        i++;
    }
    return new TGraphErrors(ndep.size(), &ndep[0], &ndedx[0], &ndep_s[0], &ndedx_s[0]);
}

int main(int argc, char **argv)
{
    auto app = new TApplication("app", &argc, argv);
    auto c1 = new TCanvas();
    std::vector<TGraphErrors *> lists;
    auto mg = new TMultiGraph("mg", "mg");
    TLegend leg(.75, .75, .95, .95);
    leg.SetFillColor(0);
    int i = 1;
    const int k = 1;
    for (auto name : {"proton", "pi+", "pi-"})
    {
        int c;
        std::cout << name << std::endl;
        TGraphErrors *tg;
        lists.push_back(tg = getGraph(name));
        tg->SetMarkerColor(c = (i++*k));
        tg->SetLineColor(c);
        // tg->SetFillColor(c);
        tg->SetMarkerStyle(20+i);
        mg->Add(tg, "PEC");
        leg.AddEntry(tg, name);
    }
    mg->SetTitle("End-dedx v. Range;Range(cm);End-dedx(MeV/cm)");
    mg->Draw("AP");
    leg.Draw();
    c1->Draw();
    ((TRootCanvas *)c1->GetCanvasImp())->Connect("CloseWindow()", "TApplication", app, "Terminate()");
    app->Run();
    delete app;
    delete c1;
}
