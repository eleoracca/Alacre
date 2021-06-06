#include "Riostream.h"
#include "TF1.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TMath.h"
#include "Colori.h"
#include "TString.h"

#define NBINS 108

bool Analisi(const double sogliaPhi, const double larghezza){

  // --------------- RICOSTRUZIONE VERTICE---------------

  // funzioni per leggere i tree
  //cout << "Prima di apertura file";
  TFile *fileRicostruzione = new TFile("Output/Ricostruzione.root", "READ");
  TFile *fileGenerazione = new TFile("Output/Simulazione.root", "READ");

  if(fileRicostruzione->IsZombie()){
    cout << Avvertimento("Problema nel leggere il file Simulazione.root \nLa ricostruzione si interrompe.") << endl;
    return kFALSE;
  }
  if(fileGenerazione->IsZombie()){
    cout << Avvertimento("Problema nel creare il file Ricostruzione.root \nLa ricostruzione si interrompe.") << endl;
    return kFALSE;
  }
  //cout << "Prima di lettura ricostruzione";

  // lettura ricostruzione
  fileRicostruzione -> cd();
  TTree *robinia = (TTree*) fileRicostruzione -> Get("rovere");

  TClonesArray *UrtiRiv1Reco = new TClonesArray("Urto", 100);
  TBranch *Branch1LReco = robinia -> GetBranch("UrtiRivelatore1Reco");
  //cout << "Prima";
  Branch1LReco -> SetAddress(&UrtiRiv1Reco);
  //cout << "Dopo";
  TClonesArray *UrtiRiv2Reco = new TClonesArray("Urto", 100);
  TBranch *Branch2LReco = robinia -> GetBranch("UrtiRivelatore2Reco");
  Branch2LReco -> SetAddress(&UrtiRiv2Reco);

  // lettura generazione
  fileGenerazione -> cd();
  TTree *ginko = (TTree*) fileGenerazione -> Get("gaggia");
  TBranch *BranchVGen = ginko -> GetBranch("Vertice");

  // --------------- GRAFICI ---------------
  // Grafico della risoluzione dell'apparato
  // per ogni evento riempio un bin dell'istogramma
  /*
  cout << "************** Grafico Risoluzione **************" << endl;
  TCanvas *cRisoluzioneApp = new TCanvas("cRisoluzioneApp","Risoluzione",200,10,600,400);
  cRisoluzioneApp->SetFillColor(0);
  cRisoluzioneApp->cd();
  TH1I* hRisoluzioneApp = new TH1I("hRisoluzioneApp","Risoluzione", 200,-0.2,0.2); //non ricordo come funzionano
  hRisoluzioneApp->SetTitle("Risoluzione");
  hRisoluzioneApp->GetXaxis()->SetTitle("zGen - zReco [cm]");
  hRisoluzioneApp->GetYaxis()->SetTitle("Conteggi");
  hRisoluzioneApp->Draw("AP"); */

  // Grafio della risoluzione in funzione di zGen
  /*
  cout << "************** Grafico Risoluzione vs zGen **************" << endl;
  TCanvas *cRisoluzioneGen = new TCanvas("cRisoluzioneGen","Risoluzione vs zGen",200,10,600,400);
  cRisoluzioneGen->SetFillColor(0);
  cRisoluzioneGen->cd();
  */
  //TH1I* hRisoluzioneGen = new TH1I("hRisoluzioneGen","Risoluzione", 200,-0.2,0.2); //non ricordo come funzionano
  //hRisoluzioneGen->SetTitle("Risoluzione");
  //hRisoluzioneGen->GetXaxis()->SetTitle("zGen [cm]");
  //hRisoluzioneGen->GetYaxis()->SetTitle("Conteggi");
  //hRisoluzioneGen->Draw("AP");

  // Grafico efficienza in funzione della molteplicità

  
  // Grafico risoluzione in funzione della molteplicità
  // Grafico efficienza in funzione della molteplicità valutando 1sigma
  // Grafico conteggi per valutare moda del vertice z reco
  int neventi = robinia -> GetEntries();
  double zReco = -50;
  TH1D *hzReco[neventi];
  TH1D *hzModa = new TH1D("hzModa", "Coordinata del vertice ricostruito", NBINS, -27, 27);
  Urto *u1, *u2;
  double moda = -50;
  //cout << "Prima ciclo for eventi";
  for(int nev = 0; nev < neventi; nev++) {
    //cout << "Creo hzReco" << nev;
    hzReco[nev] = new TH1D(TString::Format("hzReco_%d", nev), TString::Format("Distribuzione coordinate z, evento %d", nev), NBINS, -27, 27);
    robinia -> GetEvent(nev);
    for(int i = 0; i < UrtiRiv1Reco -> GetEntries(); i++) {
      u1 = (Urto*) UrtiRiv1Reco -> At(i);
      for(int j = 0; j < UrtiRiv2Reco -> GetEntries(); j++) {
        u2 = (Urto*) UrtiRiv2Reco -> At(j);
        zReco = -50;
        if(TMath::Abs(u1 -> GetPhi() - u2 -> GetPhi()) < sogliaPhi) {
          //cout << "Prima di trovavertice";
          zReco = Vertice::TrovaVertice(u1, u2);
          //cout << "Dopo trovavertice";
          hzReco[nev] -> Fill(zReco);
        }
      }
    }
    moda = Moda(hzReco[nev], larghezza);
    hzModa -> Fill(moda);
  }

  TCanvas* cModa = new TCanvas("cModa", "Coordinata del vertice ricostruito", 0, 0, 1280, 1024);
  hzModa -> Draw();
  cModa -> SaveAs(".pdf");

  fileRicostruzione -> Close();
  fileGenerazione -> Close();
  delete cModa;
  /*
  for(int i = 0; i < neventi; i++) {
    delete hzReco[i];
  }
  */

  return kTRUE;
}