#include "Riostream.h"
#include "TF1.h"
#include "TH1D.h"
#include "TH1I.h"
#include "TCanvas.h"
#include "TMath.h"
#include "Rivelatore.h"
#include "Urto.h"
#include "Punto.h"
#include "Vertice.h"
#include "Varie.h"
#include "Colori.h"
#include "TBranch.h"
#include "TClonesArray.h"
#include "TTree.h"
#include "TString.h"
#include "TFile.h"
#include "TEfficiency.h"
#include "TGraphErrors.h"

#include "Colori.h"
#include "Punto.h"
#include "Rivelatore.h"
#include "Trasporto.h"
#include "Urto.h"
#include "Varie.h"
#include "Vertice.h"

#define NBINS 2500, -27, 27

using namespace std;
using namespace colore;

bool Analisi(const double larghezza, const int maxMolteplicita, Rivelatore* detector){
  
  // ----------------------------------------------------------------------------
  // Lettura della generazione del vertice
  TFile *fileGenerazione = new TFile("Output/Simulazione.root", "READ");
  
  if(fileGenerazione->IsZombie()){
    cout << Avvertimento("Problema nel creare il file Ricostruzione.root \nLa ricostruzione si interrompe.") << endl;
    return kFALSE;
  }
  
  fileGenerazione -> cd();
  TTree *ginko = (TTree*) fileGenerazione -> Get("gaggia");
  int eventi = ginko -> GetEntries();
  
  Vertice *PuntatoreVertice = new Vertice();
  TBranch *BranchVGen = ginko -> GetBranch("Vertice");
  BranchVGen -> SetAddress(&PuntatoreVertice);

  
  // ----------------------------------------------------------------------------
  // Lettura della ricostruzione degli urti sui layer
  TFile *fileRicostruzione = new TFile("Output/Ricostruzione.root", "READ");
  
  if(fileRicostruzione->IsZombie()){
    cout << Avvertimento("Problema nel leggere il file Simulazione.root \nLa ricostruzione si interrompe.") << endl;
    return kFALSE;
  }
  
  fileRicostruzione -> cd();

  TTree *robinia = (TTree*) fileRicostruzione -> Get("rovere");
  //int eventi = robinia -> GetEntries();

  TClonesArray *UrtiRiv1Reco = new TClonesArray("Urto", 100);
  TBranch *Branch1LReco = robinia -> GetBranch("UrtiRivelatore1Reco");
  Branch1LReco -> SetAddress(&UrtiRiv1Reco);
  
  TClonesArray *UrtiRiv2Reco = new TClonesArray("Urto", 100);
  TBranch *Branch2LReco = robinia -> GetBranch("UrtiRivelatore2Reco");
  Branch2LReco -> SetAddress(&UrtiRiv2Reco);
  

  // ----------------------------------------------------------------------------
  // Lettura della configurazione per l'analisi
  
  // File di output Analisi
  TFile *fileAnalisi = new TFile("Output/Analisi.root", "RECREATE");
  if(fileAnalisi->IsZombie()){
    cout << Avvertimento("Problema nel creare il file Analisi.root. \nLa simulazione si interrompe.") << endl;
    return kFALSE;
  }

  fileAnalisi->cd();

  
  // ----------------------------------------------------------------------------
  // Inizializzazione di istogrammi e variabili utili
  TH1D *hzDiff = new TH1D("hzDiff", "Differenza tra z generata e z ricostruita", NBINS);
  TH1I *hMolTutti = new TH1I("hMolTutti", "Molteplicità di tutti gli eventi", maxMolteplicita, -0.5, (float)maxMolteplicita + .5); //Istogramma per raccogliere Molteplicità di tutti gli eventi
  TH1I *hMolReco = new TH1I("hMolReco", "Molteplicità di tutti gli eventi ricostruiti", maxMolteplicita, -0.5, (float)maxMolteplicita + .5); //Istogramma per raccogliere Molteplicità di tutti gli eventi ricostruiti
  TH1I *hMolReco1s = new TH1I("hMolReco1s", "Molteplicità degli eventi ricostruiti entro 1 dev std da z = 0", maxMolteplicita, -0.5, (float)maxMolteplicita + .5); //Istogramma per raccogliere Molteplicità di eventi ricostruiti entro 1 sigma dallo 0
  TH1I *hMolReco3s = new TH1I("hMolReco3s", "Molteplicità degli eventi ricostruiti entro 3 dev std da z = 0", maxMolteplicita, -0.5, (float)maxMolteplicita + .5); //Istogramma per raccogliere Molteplicità di eventi ricostruiti entro 3 sigma dallo 0

  // Grafico conteggi per valutare moda del vertice z reco
  double zReco = -50.;
  TH1D *hzReco[eventi];
  TH1D *hzModa = new TH1D("hzModa", "Coordinata del vertice ricostruito", NBINS);
  Urto *u1 = NULL;
  Urto *u2 = NULL;
  double moda = -50.;
  double zDiff = -50.;

  // Inizializzazione vettori per TGraphErrors per la risoluzione

  double vMolt[eventi];
  double vsMolt[eventi];
  double vSz[eventi];
  double vsSz[eventi];
  for(int i = 0; i < eventi; i++){
    vMolt[i] = 0;
    vsMolt[i] = 0;
    vSz[i] = 0.;
    vsSz[i] = 0.;
  }
  
  
  // ----------------------------------------------------------------------------
  // -------------------------- RICOSTRUZIONE VERTICE ---------------------------
  // ----------------------------------------------------------------------------
  
  for(int ev = 0; ev < eventi; ev++) {
    hzReco[ev] = new TH1D(TString::Format("hzReco_%d", ev), TString::Format("Distribuzione coordinate z, evento %d", ev), NBINS);
    ginko -> GetEvent(ev);
    robinia -> GetEvent(ev);
    vMolt[ev] = PuntatoreVertice->GetMolteplicita();
    
    for(int i = 0; i < UrtiRiv1Reco -> GetEntries(); i++) {
      u1 = (Urto*) UrtiRiv1Reco -> At(i);
      for(int j = 0; j < UrtiRiv2Reco -> GetEntries(); j++) {
        u2 = (Urto*) UrtiRiv2Reco -> At(j);
        zReco = -50;
        if(TMath::Abs(u1 -> GetPhi() - u2 -> GetPhi()) < detector->GetPhiLimite()) {
          zReco = Vertice::TrovaVertice(u1, u2);
          hzReco[ev] -> Fill(zReco);
        }
      }
    }
    
    moda = Moda(hzReco[ev], larghezza);
    if(moda == -500){
      //return kFALSE;
      //cout << "Moda del vertice = 0" << " Evento numero: " << ev << "\t Coordinata vertice generato: " << PuntatoreVertice->GetZ() << endl;
    }
    else if(moda == -600){
      //cout << "Moda del vertice = 1" << " Evento numero: " << ev << "\t Coordinata vertice generato: " << PuntatoreVertice->GetZ() << endl;      
    }
    else if(moda != -600){
      hzModa -> Fill(moda);
      zDiff = PuntatoreVertice->GetZ() - moda;
      hzDiff -> Fill(zDiff);
      hMolReco -> Fill(PuntatoreVertice->GetMolteplicita());

      if(TMath::Abs(moda) < detector->GetVerticeSZ()){
        hMolReco1s -> Fill(PuntatoreVertice->GetMolteplicita());
      }

      if(TMath::Abs(moda) < 3*detector->GetVerticeSZ()){
        hMolReco3s -> Fill(PuntatoreVertice->GetMolteplicita());
      }
    
      vSz[ev] = hzReco[ev]->GetStdDev();
      vsSz[ev] = hzReco[ev]->GetStdDevError();
      
      delete hzReco[ev];
    }
    
    hMolTutti -> Fill(PuntatoreVertice->GetMolteplicita());
  }


  // ----------------------------------------------------------------------------
  // --------------------------------- GRAFICI ----------------------------------
  // ----------------------------------------------------------------------------
  
  // Grafico della moda per valutare z ricostruita
  TCanvas* cModa = new TCanvas("cModa", "Coordinata del vertice ricostruito", 0, 0, 1280, 1024);
  hzModa->GetXaxis()->SetTitle("zReco [cm]");
  hzModa->GetYaxis()->SetTitle("Conteggi");
  hzModa -> Draw();
  cModa -> SaveAs("Output/Z_Ricostruiti.pdf");


  // Risoluzione dell'apparato: distanza minima per cui vengono riconosciuti due eventi vicini
  // per ogni evento riempio un bin dell'istogramma

  // Grafico risoluzione in funzione della molteplicità
  
  cout << "************** Grafico Risoluzione vs Molteplicita **************" << endl;
  TCanvas *cRisMolt = new TCanvas("cRisMolt","Ris vs Molt",0,0,1280,1024);
  cRisMolt->SetFillColor(0);
  cRisMolt->cd();

  TGraphErrors *gRisMolt = new TGraphErrors(eventi,vMolt,vsMolt,vSz,vsSz);
  gRisMolt->SetTitle("Risoluzione");
  gRisMolt->GetXaxis()->SetTitle("Molteplicita");
  gRisMolt->GetYaxis()->SetTitle("Risoluzione [cm]");
  gRisMolt->Draw(); 
  cRisMolt->SaveAs("Output/Ris_vs_Molt.pdf");

  // Grafico della risoluzione in funzione di zGen
  /*
  cout << "************** Grafico Risoluzione vs zGen **************" << endl;
  TCanvas *cRisoluzioneGen = new TCanvas("cRisoluzioneGen","Risoluzione vs zGen",200,10,600,400);
  cRisoluzioneGen->SetFillColor(0);
  cRisoluzioneGen->cd();
  
  TH1I* hRisoluzioneGen = new TH1I("hRisoluzioneGen","Risoluzione vs zGen", 200,-0.2,j.2); //non ricordo come funzionano
  hRisoluzioneGen->SetTitle("Risoluzione");
  hRisoluzioneGen->GetXaxis()->SetTitle("zGen [cm]");
  hRisoluzioneGen->GetYaxis()->SetTitle("Conteggi");
  hRisoluzioneGen->Draw();
  cRisoluzioneGen->SaveAs(".pdf");
  */


  //Efficienza: #particelle ricostruite/#particelle generate
  
  // Grafico efficienza in funzione della molteplicità
  cout << "************** Grafico Efficienza vs Molteplicità **************" << endl;
  TCanvas *cEffMolt = new TCanvas("cEffMolt","Efficienza vs Molteplicità", 0, 0, 1280, 1024);
  cEffMolt->SetFillColor(0);
  cEffMolt->cd();
  
  TEfficiency* hEffMolt = new TEfficiency(*hMolReco, *hMolTutti);
  hEffMolt->SetTitle("Efficienza della ricostruzione;Molteplicit�;#frac{eventi Ricostruiti}{eventi Generati}");
  hEffMolt->Draw();
  cEffMolt->SaveAs("Output/Efficienza_Molteplicita_Tot.pdf");
  

  // Grafico efficienza in funzione della molteplicità valutando 1sigma
  cout << "************** Grafico Efficienza vs Molteplicità in 1 /sigma **************" << endl;
  TCanvas *cEffMolt1S = new TCanvas("cEffMolt1S","Efficienza vs Molteplicità in 1 sigma", 0, 0, 1280, 1024);
  cEffMolt1S->SetFillColor(0);
  cEffMolt1S->cd();
  
  TEfficiency* hEffMolt1S = new TEfficiency(*hMolReco1s, *hMolTutti);
  hEffMolt1S->SetTitle("Efficienza della ricostruzione entro 1#sigma dal centro;Molteplicit�;#frac{eventi Ricostruiti}{eventi Generati}");
  hEffMolt1S->Draw();
  cEffMolt1S->SaveAs("Output/Efficienza_Molteplicita_1s.pdf");

  
  // Grafico efficienza in funzione della molteplicità valutando 3sigma
  cout << "************** Grafico Efficienza vs Molteplicità in 3 /sigma **************" << endl;
  TCanvas *cEffMolt3S = new TCanvas("cEffMolt3S","Efficienza vs Molteplicità in 3 sigma", 0, 0, 1280, 1024);
  cEffMolt3S->SetFillColor(0);
  cEffMolt3S->cd();
  
  TEfficiency* hEffMolt3S = new TEfficiency(*hMolReco3s, *hMolTutti);
  hEffMolt3S->SetTitle("Efficienza della ricostruzione entro 3#sigma dal centro;Molteplicit�;#frac{eventi Ricostruiti}{eventi Generati}");
  hEffMolt3S->Draw();
  cEffMolt3S->SaveAs("Output/Efficienza_Molteplicita_3s.pdf");
  

 // Chiusura dei file e deallocazione della memoria
  fileAnalisi -> Write();
  fileAnalisi -> Close();
  fileRicostruzione -> Close();
  fileGenerazione -> Close();

  delete cModa;
  /*
  for(int i = 0; i < eventi; i++) {
    delete hzReco[i];
  }
  */
  return kTRUE;
}
