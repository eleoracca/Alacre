#include "Riostream.h"
#include "TF1.h"
#include "TH1D.h"
#include "TH1I.h"
#include "TCanvas.h"
#include "TMath.h"
#include "TBranch.h"
#include "TClonesArray.h"
#include "TTree.h"
#include "TString.h"
#include "TFile.h"
#include "TEfficiency.h"
#include "TGraphErrors.h"
#include "TLegend.h"
#include "TMultiGraph.h"
#include "TGraphAsymmErrors.h"
#include "TObjString.h"

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

bool Analisi(const double larghezza, const int maxMolteplicita, Rivelatore* detector, TString nomefileRicostruzione, TString nomefileGenerazione){
  // Mettere il check sulle desineze per l'analisi di molte simulazioni
  
  // ----------------------------------------------------------------------------
  // Lettura della generazione del vertice
  TFile *fileGenerazione = new TFile("Output/Simulazione" + nomefileGenerazione + ".root", "READ");
  
  if(fileGenerazione->IsZombie()){
    cout << Avvertimento("Problema nel leggere il file Simulazione" + nomefileGenerazione + ".root \nLa ricostruzione si interrompe.") << endl;
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
  TFile *fileRicostruzione = new TFile("Output/Ricostruzione" + nomefileRicostruzione + ".root", "READ");
  
  if(fileRicostruzione->IsZombie()){
    cout << Avvertimento("Problema nel leggere il file Ricostruzione" + nomefileRicostruzione + ".root \nLa ricostruzione si interrompe.") << endl;
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
  TFile *fileAnalisi = new TFile("Output/Analisi" + nomefileGenerazione + "_" + nomefileRicostruzione + ".root", "RECREATE");
  if(fileAnalisi->IsZombie()){
    cout << Avvertimento("Problema nel creare il file Analisi.root. \nLa simulazione si interrompe.") << endl;
    return kFALSE;
  }

  fileAnalisi->cd();

  
  // ----------------------------------------------------------------------------
  // Inizializzazione di istogrammi e variabili utili
  TH1D *hzDiff = new TH1D("hzDiff", "Differenza tra z generata e z ricostruita", 1000, -0.15, 0.15);
  TH1I *hMolTutti = new TH1I("hMolTutti", "Molteplicità di tutti gli eventi", maxMolteplicita, -0.5, (float)maxMolteplicita + .5); //Istogramma per raccogliere Molteplicità di tutti gli eventi
  TH1I *hMolTutti1s = new TH1I("hMolTutti1s", "Molteplicità di tutti gli eventi", maxMolteplicita, -0.5, (float)maxMolteplicita + .5); //Istogramma per raccogliere Molteplicità di tutti gli eventi
  TH1I *hMolTutti3s = new TH1I("hMolTutti3s", "Molteplicità di tutti gli eventi", maxMolteplicita, -0.5, (float)maxMolteplicita + .5); //Istogramma per raccogliere Molteplicità di tutti gli eventi
  TH1I *hMolReco = new TH1I("hMolReco", "Molteplicità di tutti gli eventi ricostruiti", maxMolteplicita, -0.5, (float)maxMolteplicita + .5); //Istogramma per raccogliere Molteplicità di tutti gli eventi ricostruiti
  TH1I *hMolReco1s = new TH1I("hMolReco1s", "Molteplicità degli eventi ricostruiti entro 1 dev std da z = 0", maxMolteplicita, -0.5, (float)maxMolteplicita + .5); //Istogramma per raccogliere Molteplicità di eventi ricostruiti entro 1 sigma dallo 0
  TH1I *hMolReco3s = new TH1I("hMolReco3s", "Molteplicità degli eventi ricostruiti entro 3 dev std da z = 0", maxMolteplicita, -0.5, (float)maxMolteplicita + .5); //Istogramma per raccogliere Molteplicità di eventi ricostruiti entro 3 sigma dallo 0

  // Istogrammi per analisi risoluzione
  // risoluzione vs zTrue
  #define PUNTI_RISOL_Z_VERO 20
  TH1D *hRisZ[PUNTI_RISOL_Z_VERO];
  double vZ[PUNTI_RISOL_Z_VERO];
  double vsZ[PUNTI_RISOL_Z_VERO];
  for(int i = 0; i < PUNTI_RISOL_Z_VERO; i++) {
    hRisZ[i] = new TH1D(TString::Format("hRisZ_%d", i), TString::Format("Distribuzione risoluzione, gruppo %d", i), 1000, -5, 5);
    vZ[i] = -19 + i * 2;
    vsZ[i] = 1;
  }

  // risoluzione vs molteplicita'
  #define PUNTI_RISOL_MOLT 20
  TH1D *hRisMolt[PUNTI_RISOL_MOLT];
  double vMolt[PUNTI_RISOL_MOLT];
  double vsMolt[PUNTI_RISOL_MOLT];
  for(int i = 0; i < PUNTI_RISOL_MOLT; i++) {
    hRisMolt[i] = new TH1D(TString::Format("hRisMolt_%d", i), TString::Format("Distribuzione risoluzione, gruppo %d", i), 1000, -5, 5);
    vMolt[i] = i * 3 + 2;
    vsMolt[i] = 1.5;
  }

  // Grafico conteggi per valutare moda del vertice z reco
  double zReco = -50.;
  TH1D *hzReco[eventi];
  TH1D *hzModa = new TH1D("hzModa", "Coordinata del vertice ricostruito", 200, -27., 27.);
  Urto *u1 = NULL;
  Urto *u2 = NULL;
  double moda = -50.;
  double zDiff = -50.;

  // Inizializzazione vettori per TGraphErrors per la risoluzione
  double vRisolZVero[PUNTI_RISOL_Z_VERO];
  double vsRisolZVero[PUNTI_RISOL_Z_VERO];
  double vRisolMolt[PUNTI_RISOL_MOLT];
  double vsRisolMolt[PUNTI_RISOL_MOLT];
  
  
  // ----------------------------------------------------------------------------
  // -------------------------- RICOSTRUZIONE VERTICE ---------------------------
  // ----------------------------------------------------------------------------
  
  for(int ev = 0; ev < eventi; ev++) {
    hzReco[ev] = new TH1D(TString::Format("hzReco_%d", ev), TString::Format("Distribuzione coordinate z, evento %d", ev), NBINS);
    ginko -> GetEvent(ev);
    robinia -> GetEvent(ev);
    
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
   if(moda != -600 && moda != -500){
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
    
      for(int i = 0; i < PUNTI_RISOL_Z_VERO; i++){
        if(TMath::Abs(PuntatoreVertice -> GetZ() - vZ[i]) < vsZ[i]) {
            hRisZ[i] -> Fill(zDiff);
        }
      }
      for(int i = 0; i < PUNTI_RISOL_MOLT; i++) {
        if(TMath::Abs(PuntatoreVertice -> GetMolteplicita() - vMolt[i]) < vsMolt[i]) {
          hRisMolt[i] -> Fill(zDiff);
        }
      }
      
    }
    if(ev % 1000 != 0) {
      delete hzReco[ev];
    }

    // Riempimento dei grafici con il totale della molteplicit�
    hMolTutti -> Fill(PuntatoreVertice->GetMolteplicita());
    
    if(TMath::Abs(PuntatoreVertice->GetZ()) < 1.1*detector->GetVerticeSZ()){
      hMolTutti1s -> Fill(PuntatoreVertice->GetMolteplicita());
    }

    if(nomefileRicostruzione != "Rumore100"){
      if(TMath::Abs(PuntatoreVertice->GetZ()) < 3.1*detector->GetVerticeSZ()){      
	hMolTutti3s -> Fill(PuntatoreVertice->GetMolteplicita());	
      }
    }
    else{
      hMolTutti3s -> Fill(PuntatoreVertice->GetMolteplicita());	
    }

  }


  // ----------------------------------------------------------------------------
  // --------------------------------- GRAFICI ----------------------------------
  // ----------------------------------------------------------------------------
  
  // Grafico della moda per valutare z ricostruita
  hzModa->GetXaxis()->SetTitle("zReco [cm]");
  hzModa->GetYaxis()->SetTitle("Conteggi");
  hzModa -> Write();


  // Risoluzione dell'apparato: distanza minima per cui vengono riconosciuti due eventi vicini per ogni evento riempio un bin dell'istogramma
  // Grafico risoluzione in funzione della molteplicit�
  
  cout << "******************* Grafico Risoluzione vs Molteplicit\u00e0 ********************" << endl;
  for(int i = 0; i < PUNTI_RISOL_MOLT; i++){
    vRisolMolt[i] = hRisMolt[i] -> GetStdDev();
    vsRisolMolt[i] = hRisMolt[i] -> GetStdDevError();
  }

  TGraphErrors *gRisMolt = new TGraphErrors(PUNTI_RISOL_MOLT, vMolt, vRisolMolt, vsMolt, vsRisolMolt);
  gRisMolt->SetTitle("Risoluzione vs Molteplicit\u00e0");
  gRisMolt->GetXaxis()->SetTitle("Molteplicit\u00e0");
  gRisMolt->GetYaxis()->SetTitle("Risoluzione [cm]");
  gRisMolt->Draw("AP");
  gRisMolt->Write();

  
  // Grafico della risoluzione in funzione di zVero
  cout << "*********************** Grafico Risoluzione vs zVero ***********************" << endl;
  for(int i = 0; i < PUNTI_RISOL_Z_VERO; i++){
    vRisolZVero[i] = hRisZ[i] -> GetStdDev();
    vsRisolZVero[i] = hRisZ[i] -> GetStdDevError();
  }
  
  TGraphErrors *gRisolZVero = new TGraphErrors(PUNTI_RISOL_Z_VERO, vZ, vRisolZVero, vsZ, vsRisolZVero);
  gRisolZVero->SetTitle("Risoluzione vs zVero");
  gRisolZVero->GetXaxis()->SetTitle("zVero [cm]");
  gRisolZVero->GetYaxis()->SetTitle("Conteggi");
  gRisolZVero->Write();


  //Efficienza: #particelle ricostruite/#particelle generate  
  // Grafico efficienza in funzione della molteplicit�
  cout << "******************** Grafico Efficienza vs Molteplicit\u00e0 ********************" << endl;
  TEfficiency* hEffMolt = new TEfficiency(*hMolReco, *hMolTutti);
  hEffMolt->SetTitle("Efficienza della ricostruzione;Molteplicit�;#frac{eventi Ricostruiti}{eventi Generati}");
  hEffMolt->Write();
  

  // Grafico efficienza in funzione della molteplicit� valutando 1sigma
  cout << "**************** Grafico Efficienza vs Molteplicit\u00e0 in 1/\u03c3 *****************" << endl;  
  TEfficiency* hEffMolt1S = new TEfficiency(*hMolReco1s, *hMolTutti1s);
  hEffMolt1S->SetTitle("Efficienza della ricostruzione entro 1#sigma dal centro;Molteplicit�;#frac{eventi Ricostruiti}{eventi Generati}");
  hEffMolt1S->Write();

  
  // Grafico efficienza in funzione della molteplicit� valutando 3sigma
  cout << "**************** Grafico Efficienza vs Molteplicità in 3/\u03c3 *****************" << endl;
  TEfficiency* hEffMolt3S = new TEfficiency(*hMolReco3s, *hMolTutti3s);
  hEffMolt3S->SetTitle("Efficienza della ricostruzione entro 3#sigma dal centro;Molteplicit�;#frac{eventi Ricostruiti}{eventi Generati}");
  hEffMolt3S->Write();
  

  // Chiusura dei file e deallocazione della memoria
  fileAnalisi -> Write();
  fileAnalisi -> Close();
  fileRicostruzione -> Close();
  fileGenerazione -> Close();

  delete gRisMolt;
  delete gRisolZVero;
  delete hEffMolt;
  delete hEffMolt1S;
  delete hEffMolt3S;

  delete PuntatoreVertice;
  delete UrtiRiv1Reco;
  delete UrtiRiv2Reco;
  delete fileGenerazione;
  delete fileRicostruzione;
  delete fileAnalisi;

  return kTRUE;
}


void PostAnalisi(const vector<TString>& nomiFile, const char* desinenza="") {
  TFile *filetemp;

  vector<TString> nomiEfficienze = {"hMolTutti_clone;1", "hMolTutti1s_clone;1", "hMolTutti3s_clone;1"};
  vector<TString> nomiRisoluzioni = {"Graph;1", "Graph;2"};
  vector<TString> vettoreRumore;
  vector<TString> titoliEfficienze = {"Efficienza della ricostruzione", "Efficienza della ricostruzione entro 1#sigma dal centro", "Efficienza della ricostruzione entro 3#sigma dal centro"};
  vector<TString> titoliRisoluzione = {"Risoluzione vs Molteplicit\u00e0", "Risoluzione vs zVero"};
  vector<TString> titoliAsseXRisoluzioni = {"Molteplicita'", "zVero [cm]"};
  vector<EColor> colori = {kRed, kBlue, kBlack, kGreen, kViolet};

  TCanvas *cMultiEff[nomiEfficienze.size()];
  TMultiGraph *multiEff[nomiEfficienze.size()];
  TCanvas *cMultiRisol[nomiRisoluzioni.size()];
  TMultiGraph *multiRisol[nomiRisoluzioni.size()];
  TLegend *legendeEfficienze[nomiEfficienze.size()];
  TLegend *legendeRisoluzioni[nomiRisoluzioni.size()];

  for(unsigned int i = 0; i < nomiEfficienze.size(); i++) {
    cMultiEff[i] = new TCanvas(TString::Format("cMultiEff_%d", i), nomiEfficienze[i], 0, 0, 1280, 1024);
    multiEff[i] = new TMultiGraph();
    legendeEfficienze[i] = new TLegend(0.3, 0.1, 0.6, 0.4);
  }
  
  for(unsigned int i = 0; i < nomiRisoluzioni.size(); i++) {
    cMultiRisol[i] = new TCanvas(TString::Format("cMultiRisol_%d", i), nomiRisoluzioni[i], 0, 0, 1280, 1024);
    multiRisol[i] = new TMultiGraph();
    legendeRisoluzioni[i] = new TLegend(0.55, 0.55, 0.85, 0.85);
  }


  for(unsigned int i = 0; i < nomiFile.size(); i++) {
    filetemp = TFile::Open("Output/" + nomiFile[i] + ".root");
    TString rumore = ((TObjString *)(nomiFile[i].Tokenize("_") -> At(1))) -> String();
    vettoreRumore.push_back(rumore);

    for(unsigned int j = 0; j < nomiEfficienze.size(); j++) {
      TEfficiency *efficienza = (TEfficiency*)filetemp -> Get(nomiEfficienze[j]);
      efficienza -> SetTitle(titoliEfficienze[i]);
      efficienza -> SetLineColor(colori[i]);
      TGraphAsymmErrors *graficoEfficienza = efficienza -> CreateGraph();
      legendeEfficienze[j] -> AddEntry(graficoEfficienza, rumore);
      multiEff[j] -> Add((TGraph*) graficoEfficienza);
    }

    for(unsigned int j = 0; j < nomiRisoluzioni.size(); j++) {
      TGraphErrors *risoluzione = (TGraphErrors*)filetemp -> Get(nomiRisoluzioni[j]);
      risoluzione -> SetTitle(titoliRisoluzione[0]);
      risoluzione -> GetXaxis() -> SetTitle("Molteplicit\u00e0");
      risoluzione -> GetYaxis() -> SetTitle("Risoluzione [cm]");
      risoluzione -> SetLineColor(colori[i]);
      legendeRisoluzioni[j] -> AddEntry(risoluzione, rumore);
      multiRisol[j] -> Add(risoluzione);
    }

    filetemp -> Close();
  }

  for(unsigned int i = 0; i < nomiEfficienze.size(); i++) {
    cMultiEff[i] -> cd();
    multiEff[i] -> SetTitle(titoliEfficienze[i] + ";Molteplicit\u00e0;#frac{eventi Ricostruiti}{eventi Generati}");
    multiEff[i] -> Draw("ap");
    legendeEfficienze[i] -> Draw();
    cMultiEff[i] -> SaveAs(TString::Format("Output/efficienza_molteplicita_%d%s.pdf", i, desinenza));
  }

  for(unsigned int i = 0; i < nomiRisoluzioni.size(); i++) {
    cMultiRisol[i] -> cd();
    multiRisol[i] -> SetTitle(titoliRisoluzione[i]);
    multiRisol[i] -> GetXaxis() -> SetTitle(titoliAsseXRisoluzioni[i]);
    multiRisol[i] -> GetYaxis() -> SetTitle("Risoluzione [cm]");
    multiRisol[i] -> Draw("ap");
    legendeRisoluzioni[i] -> Draw();
    cMultiRisol[i] -> SaveAs(TString::Format("Output/risoluzione_%d%s.pdf", i, desinenza));
  }

  // Distruzione dei grafici e deallocamento memoria  
  for(int i = 0; i < (int)nomiEfficienze.size(); i++){
    delete cMultiRisol[i];
    delete cMultiEff[i];
  }
  
  return;
}
