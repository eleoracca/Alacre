#include "Riostream.h"
#include "TF1.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TMath.h"
#include "Colori.h"
#include "TString.h"
#include "TEfficiency.h"

#define NBINS 108, -27, 27

using namespace std;
using namespace colore;

bool Analisi(const double sogliaPhi, const double larghezza){

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

  // Lettura ricostruzione
  fileRicostruzione -> cd();
  TTree *robinia = (TTree*) fileRicostruzione -> Get("rovere");

  TClonesArray *UrtiRiv1Reco = new TClonesArray("Urto", 100);
  TBranch *Branch1LReco = robinia -> GetBranch("UrtiRivelatore1Reco");
  Branch1LReco -> SetAddress(&UrtiRiv1Reco);
  
  TClonesArray *UrtiRiv2Reco = new TClonesArray("Urto", 100);
  TBranch *Branch2LReco = robinia -> GetBranch("UrtiRivelatore2Reco");
  Branch2LReco -> SetAddress(&UrtiRiv2Reco);

  // Lettura generazione
  fileGenerazione -> cd();
  TTree *ginko = (TTree*) fileGenerazione -> Get("gaggia");
  TBranch *BranchVGen = ginko -> GetBranch("Vertice");

  // File di output Analisi
  TFile *fileAnalisi = new TFile("Output/Analisi.root", "RECREATE");
  if(fileAnalisi->IsZombie()){
    cout << Avvertimento("Problema nel creare il file Analisi.root. \nLa simulazione si interrompe.") << endl;
    return kFALSE;
  }

  fileAnalisi->cd();

  // Grafico conteggi per valutare moda del vertice z reco
  int neventi = robinia -> GetEntries();
  double zReco = -50;
  TH1D *hzReco[neventi];
  TH1D *hzModa = new TH1D("hzModa", "Coordinata del vertice ricostruito", NBINS);
  Urto *u1, *u2;
  double moda = -50;

  TH1D *hzDiff = new TH1D("hzDiff", "Differenza tra z generata e z ricostruita", NBINS);
  
  for(int nev = 0; nev < neventi; nev++) {
    hzReco[nev] = new TH1D(TString::Format("hzReco_%d", nev), TString::Format("Distribuzione coordinate z, evento %d", nev), NBINS);
    robinia -> GetEvent(nev);
    for(int i = 0; i < UrtiRiv1Reco -> GetEntries(); i++) {
      u1 = (Urto*) UrtiRiv1Reco -> At(i);
      for(int j = 0; j < UrtiRiv2Reco -> GetEntries(); j++) {
        u2 = (Urto*) UrtiRiv2Reco -> At(j);
        zReco = -50;
        if(TMath::Abs(u1 -> GetPhi() - u2 -> GetPhi()) < sogliaPhi) {
          zReco = Vertice::TrovaVertice(u1, u2);
          hzReco[nev] -> Fill(zReco);
        }
      }
    }
    moda = Moda(hzReco[nev], larghezza);
    if(moda == -500){
      return kFALSE;
    }
    else if(moda != -600){
      hzModa -> Fill(moda);
    }

  }

  TCanvas* cModa = new TCanvas("cModa", "Coordinata del vertice ricostruito", 0, 0, 1280, 1024);
  hzModa->GetXaxis()->SetTitle("zReco [cm]");
  hzModa->GetYaxis()->SetTitle("Conteggi");
  hzModa -> Draw();
  cModa -> SaveAs(".pdf");


  // --------------- GRAFICI ---------------
  // Risoluzione dell'apparato: distanza minima per cui vengono riconosciuti due eventi vicini
  // per ogni evento riempio un bin dell'istogramma
  /*
  cout << "************** Grafico Risoluzione **************" << endl;
  TCanvas *cRisoluzioneApp = new TCanvas("cRisoluzioneApp","Risoluzione",0,0,1280,1024);
  cRisoluzioneApp->SetFillColor(0);
  cRisoluzioneApp->cd();
  TH1D* hRisoluzioneApp = new TH1D("hRisoluzioneApp","Risoluzione", NBiNS,-27,27); //non ricordo come funzionano
  hRisoluzioneApp->SetTitle("Risoluzione");
  hRisoluzioneApp->GetXaxis()->SetTitle("zGen - zReco [cm]");
  hRisoluzioneApp->GetYaxis()->SetTitle("Conteggi");
  hRisoluzioneApp->Draw(); 
  cRisoluzioneApp->SaveAs(".pdf");*/

  // Grafico della risoluzione in funzione di zGen
  /*
  cout << "************** Grafico Risoluzione vs zGen **************" << endl;
  TCanvas *cRisoluzioneGen = new TCanvas("cRisoluzioneGen","Risoluzione vs zGen",200,10,600,400);
  cRisoluzioneGen->SetFillColor(0);
  cRisoluzioneGen->cd();
  
  TH1I* hRisoluzioneGen = new TH1I("hRisoluzioneGen","Risoluzione vs zGen", 200,-0.2,0.2); //non ricordo come funzionano
  hRisoluzioneGen->SetTitle("Risoluzione");
  hRisoluzioneGen->GetXaxis()->SetTitle("zGen [cm]");
  hRisoluzioneGen->GetYaxis()->SetTitle("Conteggi");
  hRisoluzioneGen->Draw();
  cRisoluzioneGen->SaveAs(".pdf");
  */

  // Grafico risoluzione in funzione della molteplicità

   /*
  cout << "************** Grafico Risoluzione vs Molteplicità **************" << endl;
  TCanvas *cRisMolt = new TCanvas("cRisMolt","Risoluzione vs Molteplicità",200,10,600,400);
  cRisMolt->SetFillColor(0);
  cRisMolt->cd();
  
  TH1I* hRisMolt = new TH1I("hRisMolt","Risoluzione vs Molteplicità", 200,-0.2,0.2); //non ricordo come funzionano
  hRisMolt->SetTitle("Risoluzione");
  hRisMolt->GetXaxis()->SetTitle("# particelle");
  hRisMolt->GetYaxis()->SetTitle("Risoluzione su z");

  hRisMolt->Draw();
  cRisMolt->SaveAs(".pdf");
  */

  //Efficienza: #particelle ricostruite/#particelle generate

  // Grafico efficienza in funzione della molteplicità
 
  cout << "************** Grafico Efficienza vs Molteplicità **************" << endl;
  TCanvas *cEffMolt = new TCanvas("cEffMolt","Efficienza vs Molteplicità",200,10,600,400);
  cEffMolt->SetFillColor(0);
  cEffMolt->cd();
  
  //TEfficiency* hEffMolt = new TEfficiency("hEffMolt","Efficienza vs Molteplicità", 200,-0.2,0.2); //non ricordo come funzionano
  /*
  hEffMolt->SetTitle("Risoluzione");
  hEffMolt->GetXaxis()->SetTitle("# particelle generate");
  hEffMolt->GetYaxis()->SetTitle("Efficienza");
  hEffMolt->Draw();
  cEffMolt->SaveAs(".pdf");
  */

  // Grafico efficienza in funzione della molteplicità valutando 1sigma
   /*
  cout << "************** Grafico Efficienza vs Molteplicità in 1 /sigma **************" << endl;
  TCanvas *cEffMolt1S = new TCanvas("cEffMolt1S","Efficienza vs Molteplicità in 1 sigma",200,10,600,400);
  cEffMolt1S->SetFillColor(0);
  cEffMolt1S->cd();
  
  TH1I* hEffMolt1S = new TH1I("hEffMolt1S","Efficienza vs Molteplicità in 1 sigma", 200,-0.2,0.2); //non ricordo come funzionano
  hEffMolt1S->SetTitle("Risoluzione");
  hEffMolt1S->GetXaxis()->SetTitle("# particelle vicino al vertice");
  hEffMolt1S->GetYaxis()->SetTitle("Efficienza");
  hEffMolt1S->Draw();
  cEffMolt1s->SaveAs(".pdf");
  */


 // Chiusura dei file e deallocazione della memoria
  fileAnalisi -> Write();
  fileAnalisi -> Close();
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