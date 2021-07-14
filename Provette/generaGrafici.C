#if !defined(__CINT__) || defined(__MAKECINT__)
#include "Riostream.h"
#include "TStopwatch.h"
#include "TString.h"

#include "Interface/Vertice.h"
#include "TFile.h"
#include "TH1D.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TLegend.h"

#endif

using namespace std;

void generaGrafici() {
    // rifare i grafici hMolTutti, Graph1 e Graph (anche in versione senza rumore 100)
    // sovrapporre Simulazione.root -> Vertice.dmZ con Analisi_Rumore0.root -> hzModa
    TFile *fileSimulazione = TFile::Open("Output/Simulazione.root");
    TFile *fileAnalisi = TFile::Open("Output/Analisi_Rumore0.root");

    TTree *gelso = (TTree*) fileSimulazione -> Get("gaggia");
    
    Vertice *PuntatoreVertice = new Vertice();
    TBranch *BranchVGen = gelso -> GetBranch("Vertice");
    BranchVGen -> SetAddress(&PuntatoreVertice);

    TH1D *hDmZ = new TH1D("hDmZ", "Coordinata del vertice ricostruito", 100, -27., 27.);

    for(unsigned int i = 0; i < gelso -> GetEntries(); i++) {
        gelso -> GetEvent(i);
        hDmZ -> Fill(PuntatoreVertice -> GetZ());
    }

    TH1D *hZReco = (TH1D*) fileAnalisi -> Get("hzModa");
    TCanvas c("c", "pippo", 0, 0, 1280, 1024);
    hDmZ -> SetTitle("Coordinata Z del vertice");
    hDmZ -> GetYaxis() -> SetTitle("Conteggi");
    hDmZ -> GetXaxis() -> SetTitle("Z [cm]");
    hDmZ -> SetLineColor(kBlue);

    hZReco -> SetLineColor(kRed);
    hZReco -> Rebin(2);
    hDmZ -> Draw("histo");
    hZReco -> Draw("histosame");
    TLegend legenda(0.95, 0.95, 0.75, 0.8);
    legenda.AddEntry(hDmZ, "Z generata");
    legenda.AddEntry(hZReco, "Z ricostruita");
    legenda.Draw();
    gStyle -> SetOptStat(0);
    c.SaveAs("grafici_z_sovrapposti.pdf");
}