#include "Riostream.h"
#include "TF1.h"
#include "TH1I.h"
#include "TMath.h"
#include "Colori.h"


void Analisi(const double sogliaPhi){

  // --------------- RICOSTRUZIONE VERTICE---------------

  // funzioni per leggere i tree
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

  // lettura ricostruzione
  fileRicostruzione -> cd();
  TTree* robinia = (TTree*) fileRicostruzione -> Get("rovere");

  TClonesArray* UrtiRiv1Reco = new TClonesArray("Urto", 100);
  TBranch* Branch1LReco = robinia -> GetBranch("UrtiRivelatore1");
  Branch1LReco -> SetAddress(&UrtiRiv1Reco);

  TClonesArray* UrtiRiv2Reco = new TClonesArray("Urto", 100);
  TBranch* Branch2LReco = robinia -> GetBranch("UrtiRivelatore2");
  Branch2LReco -> SetAddress(&UrtiRiv2Reco);

  // lettura generazione
  fileGenerazione -> cd();
  TTree* ginko = (TTree*) fileGenerazione -> Get("gaggia");
  TBranch* BranchVGen = ginko -> GetBranch("Vertice");

  for(int i = 0; i < robinia -> GetEntries(); i++) {
    robinia -> GetEvent(i);
    ginko -> GetEvent(i);

    
  }

  // --------------- GRAFICI ---------------
  // Grafico della risoluzione dell'apparato
  // per ogni evento riempio un bin dell'istogramma

  cout << "************** Grafico Risoluzione **************" << endl;
  TCanvas *cRisoluzioneApp = new TCanvas("cRisoluzioneApp","Risoluzione",200,10,600,400);
  cRisoluzioneApp->SetFillColor(0);
  cRisoluzioneApp->cd();
  TH1I* hRisoluzioneApp = new TH1I("hRisoluzioneApp","Risoluzione", 200,-0.2,0.2); //non ricordo come funzionano
  hRisoluzioneApp->SetTitle("Risoluzione");
  hRisoluzioneApp->GetXaxis()->SetTitle("zGen - zReco [cm]");
  hRisoluzioneApp->GetYaxis()->SetTitle("Conteggi");
  hRisoluzioneApp->Draw("AP");

  // Grafio della risoluzione in funzione di zGen
  cout << "************** Grafico Risoluzione vs zGen **************" << endl;
  TCanvas *cRisoluzioneGen = new TCanvas("cRisoluzioneGen","Risoluzione vs zGen",200,10,600,400);
  cRisoluzioneGen->SetFillColor(0);
  cRisoluzioneGen->cd();
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
  TH1D* hzReco[neventi];
  for(int nev = 0; i < robinia -> GetEntries(); i++) {
    hzReco[nev] = new TH1D("hzReco%d" % nev, "Coordinata z del vertice ricostruito, evento %d" % nev, 50, -27, 27);
    robinia -> GetEvent(nev);
    for(int i = 0; i < UrtiRiv1Reco -> GetEntries(); i++) {
      for(int j = 0; j < UrtiRiv2Reco -> GetEntries(); j++) {
        Urto* u1, u2;
        u1 = UrtiRiv1Reco -> At(i);
        u2 = UrtiRiv2Reco -> At(j);

        if(TMath::Abs(u1 -> GetPhi() - u2 -> GetPhi()) < sogliaPhi) {
          double zReco = Vertice::TrovaVertice(u1, u2);
          hzReco[nev] -> Fill(zReco);
        }
      }
    }
  }
}