/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~ Ricostruzione degli eventi                              ~
  ~ Autori: Racca Eleonora - eleonora.racca288@edu.unito.it ~
  ~         Sauda Cristina - cristina.sauda@edu.unito.it    ~
  ~ Ultima modifica: 23/09/2018                             ~
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#if !defined(__CINT__) || defined(__MAKECINT__)

#include "Riostream.h"
#include "TBranch.h"
#include "TClonesArray.h"
#include "TFile.h"
#include "TH1F.h"
#include "TMath.h"
#include "TRandom3.h"
#include "TString.h"
#include "TTree.h"

#include "Colori.h"
#include "Punto.h"
#include "Rivelatore.h"
#include "Trasporto.h"
#include "Urto.h"
#include "Varie.h"
#include "Vertice.h"
#endif

using namespace std;
using namespace colore;

// ******************************************************************************
// ************************ Dichiarazione delle funzioni ************************
// ******************************************************************************

// Funzione da chiamare per effettuare la ricostruzione
bool Ricostruzione(Rivelatore* detector, bool fileconfig);

// Funzione che richiede all'utente i parametri per la generazione del rumore
bool RichiestaInformazioni(bool &onoff, TString &distribuzione, double &parametro1, double &parametro2);

// Funzione che stampa i parametri per la generazione del rumore
void StampaInformazioni(bool &onoff, TString &distribuzione, double &parametro1, double &parametro2);

// Funzione per fare lo smearing dei punti
void Smearing(unsigned int &u, unsigned int &v, TClonesArray *strato1Gen, TClonesArray *strato2Gen, TClonesArray *strato1Reco, TClonesArray *strato2Reco, Rivelatore *detector, Urto *urto1L, Urto *urto2L);

// Funzione per aggiungere il rumore eventualmente
void RumoreGauss(unsigned int &u, unsigned int &v, double &parametro1, double &parametro2, TClonesArray *strato1Reco, TClonesArray *strato2Reco, Rivelatore *detector, Urto *urto1L, Urto *urto2L);
void RumoreFissa(unsigned int &u, unsigned int &v, double &parametro1, TClonesArray *strato1Reco, TClonesArray *strato2Reco, Rivelatore *detector, Urto *urto1L, Urto *urto2L);

// Funzione per ricostruire il vertice
void RicostruzioneVertice(TTree *modificato);



// ******************************************************************************
// *********************** Implementazione delle funzioni ***********************
// ******************************************************************************

bool Ricostruzione(Rivelatore* detector, bool fileconfig = kFALSE){

  // ----------------------------------------------------------------------------
  // Inizializzazione e dichiarazione dei parametri a zero
  bool onoff = kFALSE;
  bool richiesta = kFALSE;
  double parametro1 = 0.;
  double parametro2 = 0.;
  TString commento = "\0";
  TString distribuzione = "\0";
  Urto *urto1L;
  Urto *urto2L;
  Urto *rumore1L;
  Urto *rumore2L;
  unsigned int u = 0;
  unsigned int v = 0;

  // Inserimento dei parametri per il rumore
  if(!fileconfig){
    richiesta = RichiestaInformazioni(onoff, distribuzione, parametro1, parametro2);
  
    if(!richiesta){
      return kFALSE;
    }
  }
  else{
    ifstream in("Configurazioni/Ricostruzione.txt");
    if(!in){
      cout << Violetto("!! File di configurazione non trovato !!") << endl << "La simulazione riparte automaticamente chiedendo di inserire a mano i parametri.";
      Ricostruzione(detector, kFALSE);
    }  
    in >> commento >> onoff >> distribuzione >> parametro1 >> parametro2;
    in.close();
    
    StampaInformazioni(onoff, distribuzione, parametro1, parametro2);
  }

  // ----------------------------------------------------------------------------
  // File della simulazione e della ricostruzione
  TFile *fileinput = new TFile("Output/Simulazione.root");
  TFile *fileoutput = new TFile("Output/Ricostruzione.root", "RECREATE");

  if(fileinput->IsZombie()){
    cout << Avvertimento("Problema nel leggere il file Simulazione.root \nLa ricostruzione si interrompe.") << endl;
    return kFALSE;
  }
  if(fileoutput->IsZombie()){
    cout << Avvertimento("Problema nel creare il file Ricostruzione.root \nLa ricostruzione si interrompe.") << endl;
    return kFALSE;
  }

  // ----------------------------------------------------------------------------
  fileinput -> cd();
  
  // Tree della simulazione
  TTree *gelso = (TTree*)fileinput->Get("gaggia");

  // Urti sul primo rivelatore - simulazione
  TClonesArray *UrtiRiv1Gen = new TClonesArray("Urto", 100);
  TBranch *Branch1LGen = gelso -> GetBranch("UrtiRivelatore1");
  Branch1LGen -> SetAddress(&UrtiRiv1Gen);
  
  // Urti sul secondo rivelatore - simulazione
  TClonesArray *UrtiRiv2Gen = new TClonesArray("Urto", 100);
  TBranch *Branch2LGen = gelso -> GetBranch("UrtiRivelatore2");
  Branch2LGen -> SetAddress(&UrtiRiv2Gen);
  
  // Numero degli eventi
  unsigned int numeroEventi = gelso -> GetEntries();

  // ----------------------------------------------------------------------------
  fileoutput -> cd();
  
  // Tree della ricostruzione
  TTree *rovere = new TTree("rovere", "Tree della ricostruzione");

  // Urti sul primo rivelatore - ricostruzione
  TClonesArray *PuntatoreRiv1Reco = new TClonesArray("Urto", 100);
  TClonesArray& IndPuntRiv1Reco = *PuntatoreRiv1Reco;

  // Urti sul secondo rivelatore - ricostruzione
  TClonesArray *PuntatoreRiv2Reco = new TClonesArray("Urto", 100);
  TClonesArray& IndPuntRiv2Reco = *PuntatoreRiv2Reco;

  rovere -> Branch("UrtiRivelatore1Reco", &PuntatoreRiv1Reco);
  rovere -> Branch("UrtiRivelatore2Reco", &PuntatoreRiv2Reco);

  // ----------------------------------------------------------------------------
  // Smearing degli urti generati e aggiunta del rumore
  for(int i = 0; i < (int)numeroEventi; i++){
    gelso -> GetEntry(i);
    
    Smearing(u, v, UrtiRiv1Gen, UrtiRiv2Gen, PuntatoreRiv1Reco, PuntatoreRiv2Reco, detector, urto1L, urto2L);
    rovere -> Fill();
    
    if(onoff && distribuzione == "gaussiana"){
      RumoreGauss(u, v, parametro1, parametro2, PuntatoreRiv1Reco, PuntatoreRiv2Reco, detector, rumore1L, rumore2L);
      rovere -> Fill();
    }
    else if(onoff && distribuzione == "fissa"){
      RumoreFissa(u, v, parametro1, PuntatoreRiv1Reco, PuntatoreRiv2Reco, detector, rumore1L, rumore2L);
      rovere -> Fill();
    }
    else if(onoff && distribuzione != "fissa" && distribuzione != "gaussiana"){
      cout << Avvertimento("Problema con la distribuzione del rumore") << endl;
    }
    
    u = 0;
    v = 0;
    UrtiRiv1Gen -> Clear();
    UrtiRiv2Gen -> Clear();
  }

  // ----------------------------------------------------------------------------
  // Ricostruzione del vertice
  
  // ----------------------------------------------------------------------------
  fileoutput -> Write();

  fileinput -> Close();
  fileoutput -> Close();
  
  return kTRUE;
}


void StampaInformazioni(bool &onoff, TString &distribuzione, double &parametro1, double &parametro2){
  cout << "------------- " << Arancione("Parametri per la ricostruzione degli eventi") << " --------------" << endl;
  cout << "I parametri per il rumore vengono letti dal file " << Azzurro("Ricostruzione.txt") << endl;
  cout << "Rumore:                                ";
  if(onoff){
    cout << "acceso"  << endl;
    cout << "  - Distribuzione:                       " << distribuzione << endl;
    if(distribuzione == "gaussiana"){
      cout << "    * Media:                             " << parametro1 << endl;
      cout << "    * Deviazione standard:               " << parametro2 << endl;
    }
    else if(distribuzione == "fissa"){
      cout << "    * Numero di rivelazioni:             " << parametro1 << endl;
    }
  }
  else cout << "spento" << endl;
}


bool RichiestaInformazioni(bool &onoff, TString &distribuzione, double &parametro1, double &parametro2){
  cout << "-------------- " << Arancione("Parametri per la ricostruzione degli eventi") << " ---------------" << endl;
  cout << "Inserire i parametri per la simulazione del rumore" << endl;
  cout << "Rumore (0 o 1):                              ";
  cin >> onoff;
  
  if(onoff){
    cout << "Simulazione del rumore attivata.";
    cout << endl << "  - Distribuzione:                       ";
    cin >> distribuzione;
    
    if(distribuzione == "gaussiana"){
      cout << endl << "    * Media:                             ";
      cin >> parametro1;
      cout << endl << "    * Deviazione standard:               ";
      cin >> parametro2;
    }
    else if(distribuzione == "fissa"){
      cout << endl << "    * Valore:                            ";
      cin >> parametro1;
    }
    else{
      cout << Violetto("Inizializzazione sbagliata della distribuzione del rumore.") << endl;
      cout << "Scrivere gaussiana o fissa: " << endl;
      cin >> distribuzione;
      
      if(distribuzione == "gaussiana"){
	cout << endl << "    * Media:                             ";
	cin >> parametro1;
	cout << endl << "    * Deviazione standard:               ";
	cin >> parametro2;
      }
      else if(distribuzione == "fissa"){
	cout << endl << "    * Valore:                            ";
	cin >> parametro1;
      }
      else{
	cout << Avvertimento("Inizializzazione sbagliata: la ricostruzione si interrompe ora.");
	return kFALSE;
      }
    }
  }
  else cout << "Simulazione del rumore disattivata." << endl;
  
  return kTRUE;
}


void Smearing(unsigned int &u, unsigned int &v, TClonesArray *strato1Gen, TClonesArray *strato2Gen, TClonesArray *strato1Reco, TClonesArray *strato2Reco, Rivelatore *detector, Urto *urto1L, Urto *urto2L){
  
  unsigned int numeroUrti1LGen = strato1Gen -> GetEntries();
  unsigned int numeroUrti2LGen = strato2Gen -> GetEntries();
  TClonesArray &strato1RecoInd = *strato1Reco;
  TClonesArray &strato2RecoInd = *strato2Reco;
  
  // Smearing degli urti generati - Layer 1
  for(int i = 0; i < (int)numeroUrti1LGen; i++){
    urto1L = ((Urto*)strato1Gen->At(i));
    
    if(TMath::Abs(urto1L -> GetZ()) <= (detector -> GetLunghezza())/2.){ //se non vale, il rivelatore non vede il passaggio
      urto1L -> SmearingGaussiano(detector, 1);
      new(strato1RecoInd[u]) Urto(urto1L); // come si fa senza fare il costruttore Urto(Urto *urto)???
      u += 1;
    }
    else continue;
  }
  
  // Smearing degli urti generati - Layer 2
  for(int j = 0; j < (int)numeroUrti2LGen; j++){
    urto2L = ((Urto*)strato2Gen->At(j));
    
    if(TMath::Abs(urto2L -> GetZ()) <= (detector -> GetLunghezza())/2.){
      urto2L -> SmearingGaussiano(detector, 1);
      new(strato2RecoInd[v]) Urto(urto2L);
      v += 1;
    }
    else continue;
  }
}


void RumoreGauss(unsigned int &u, unsigned int &v, double &parametro1, double &parametro2, TClonesArray *strato1Reco, TClonesArray *strato2Reco, Rivelatore *detector, Urto *urto1L, Urto *urto2L){

  TClonesArray &strato1RecoInd = *strato1Reco;
  TClonesArray &strato2RecoInd = *strato2Reco;
  
  //occhio al rumore nel vertice!!!
  //new(strato1RecoInd[u]) Urto(urto1L);
  //u += 1;
  //new(strato2RecoInd[v]) Urto(urto2L);
  //v += 1;
  
  cout << "Gauss" << endl;
}


void RumoreFissa(unsigned int &u, unsigned int &v, double &parametro1, TClonesArray *strato1Reco, TClonesArray *strato2Reco, Rivelatore *detector, Urto *urto1L, Urto *urto2L){
  
  TClonesArray &strato1RecoInd = *strato1Reco;
  TClonesArray &strato2RecoInd = *strato2Reco;
  
  //new(strato1RecoInd[u]) Urto(urto1L);
  //u += 1;
  //new(strato2RecoInd[v]) Urto(urto2L);
  //v += 1;
  
  cout << "Fissa" << endl;
}
