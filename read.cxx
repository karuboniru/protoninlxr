#include<TCanvas.h>
#include<TApplication.h>
#include<TTree.h>
#include<TFile.h>
#include<TRootCanvas.h>

int main(int argc, char **argv)
{
    auto app = new TApplication("app", &argc, argv);

    TFile *_file0 = TFile::Open("RootOut.root");
    TTree *Tree = nullptr;
    _file0->GetObject("Ntuple1", Tree);
    auto c1 = new TCanvas();
    Tree->Draw("Depth", "disappear_mode==1");
    c1->Draw();
		((TRootCanvas *)c1->GetCanvasImp())->Connect("CloseWindow()", "TApplication", app, "Terminate()");
		app->Run();
    delete app;
    delete c1;
    delete Tree;
    delete _file0;
}
