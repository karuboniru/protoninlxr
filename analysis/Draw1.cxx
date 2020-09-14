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

int main(int argc, char **argv)
{
    auto app = new TApplication("app", &argc, argv);
    // double energy[] = {5,10,15,20,25,30,35,40,45,50,55,60,65,70,75 80 85 90};
    std::fstream e_list("./energy_list");
    std::vector<double> energy;
    double e;
    while(e_list >> e){
        energy.push_back(e);
    }
    std::vector<std::string> ss = {"proton", "pi+", "pi-"};
    std::vector<TGraphErrors *> grs;
    auto c1 = new TCanvas();
    auto mg = new TMultiGraph("mg", "mg");
    TLegend leg(.05, .75, .25, .95);
    for (int i = 0; i < 3; i++)
    {
        const auto s = ss[i];
        std::fstream dep(s + ".depth"), dedx(s + ".dedx");
        std::vector<double> ndep, ndep_s, ndedx, ndedx_s;
        for (int k = 0; (!(dep.eof() || dedx.eof()))&&k<18;k++)
        {
            double x1, x2, x3, x4;
            dep >> x1;
            ndep.push_back(x1);
            dep >> x2;
            ndep_s.push_back(x2);
            dedx >> x3;
            ndedx.push_back(x3);
            dedx >> x4;
            ndedx_s.push_back(x4);
        }
        dep.close();
        int k = 3;
        auto tg = new TGraphErrors(ndep.size(), &energy[0], &ndep[0], nullptr, &ndep_s[0]);
        tg->SetMarkerColor(i * k + 1);
        tg->SetLineColor(i * k + 1);
        mg->Add(tg, "PEC");
        tg->SetMarkerStyle(20+i);
        leg.AddEntry(tg, s.c_str());
        std::cout << ndep.size() << std::endl;
    }
    mg->SetTitle("Range v. Energy plot;Energy(MeV);Range(cm)");
    mg->Draw("AP");
    leg.Draw();
    c1->Draw();
    ((TRootCanvas *)c1->GetCanvasImp())->Connect("CloseWindow()", "TApplication", app, "Terminate()");
    app->Run();
    delete app;
    delete c1;
}
