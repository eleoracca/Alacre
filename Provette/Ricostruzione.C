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
void StampaInformazioni(const bool &onoff, const TString &distribuzione, const double &parametro1, const double &parametro2);

// Funzione per fare lo smearing dei punti
int Smearing(const TClonesArray *strato1Gen, const TClonesArray *strato2Gen, TClonesArray *strato1Reco, TClonesArray *strato2Reco, Rivelatore *detector);

// Funzione per aggiungere il rumore eventualmente
int RumoreGauss(const double &media, const double &deviazionestandard, TClonesArray *strato1Reco, TClonesArray *strato2Reco, Rivelatore *detector);
int RumoreFissa(const double &parametro1, TClonesArray *strato1Reco, TClonesArray *strato2Reco, Rivelatore *detector);



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
      return Ricostruzione(detector, kFALSE);
    }  
    in >> commento >> onoff >> distribuzione >> parametro1 >> parametro2;
    in.close();
    
    StampaInformazioni(onoff, distribuzione, parametro1, parametro2);
  }

  // ----------------------------------------------------------------------------
  // File della simulazione e della ricostruzione
  TFile *fileinput = new TFile("Output/Simulazione.root", "READ");
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

  // Vertice - simulazione
  Vertice *PuntatoreVertice = new Vertice();
  TBranch *BranchVGen = gelso -> GetBranch("Vertice");
  BranchVGen -> SetAddress(&PuntatoreVertice);

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
  int numeroRumore = 0;
  int numeroMolteplicita = 0;

  // ----------------------------------------------------------------------------
  fileoutput -> cd();
  
  // Tree della ricostruzione
  TTree *rovere = new TTree("rovere", "Tree della ricostruzione");

  // Numero di rumore - ricostruzione
  TClonseArray *PuntatoreRumore = new TClonesArray("int[2]", 100);
  TClonesArray& IndPuntRumore = *PuntatoreRumore;

  // Urti sul primo rivelatore - ricostruzione
  TClonesArray *PuntatoreRiv1Reco = new TClonesArray("Urto", 100);
  TClonesArray& IndPuntRiv1Reco = *PuntatoreRiv1Reco;

  // Urti sul secondo rivelatore - ricostruzione
  TClonesArray *PuntatoreRiv2Reco = new TClonesArray("Urto", 100);
  TClonesArray& IndPuntRiv2Reco = *PuntatoreRiv2Reco;

  // Dichiariamo i branch del tree
  rovere -> Branch("NumeroRumore", &PuntatoreRumore);
  rovere -> Branch("UrtiRivelatore1Reco", &PuntatoreRiv1Reco);
  rovere -> Branch("UrtiRivelatore2Reco", &PuntatoreRiv2Reco);

  // ----------------------------------------------------------------------------
  // Smearing degli urti generati e aggiunta del rumore
  for(int i = 0; i < (int)numeroEventi; i++){
    gelso -> GetEntry(i);
    
    // Smearing degli urti
    numeroMolteplicita = Smearing(UrtiRiv1Gen, UrtiRiv2Gen, PuntatoreRiv1Reco, PuntatoreRiv2Reco, detector);
    
    if(numeroMolteplicita != PuntatoreVertice -> GetMolteplicita()){
      //cout << Azzurro("Il numero di urti ricostruiti sul secondo layer è diverso dalla molteplicità dell'evento.") << endl;
      //cout << "Molteplicità = " << PuntatoreVertice -> GetMolteplicita() << "\t Numero urti = " << numeroMolteplicita << endl;
    }

    // Aggiunta del rumore
    if(onoff && distribuzione == "gaussiana"){
      numeroRumore = RumoreGauss(parametro1, parametro2, PuntatoreRiv1Reco, PuntatoreRiv2Reco, detector);
    }
    else if(onoff && distribuzione == "fissa"){
      numeroRumore = RumoreFissa(parametro1, PuntatoreRiv1Reco, PuntatoreRiv2Reco, detector);
    }
    else if(onoff && distribuzione != "fissa" && distribuzione != "gaussiana"){
      cout << Avvertimento("Problema con la distribuzione del rumore") << endl;
      return kFALSE;
    }

    // Si riempie di nuovo per sicurezza il tree e si cancellano gli array per il nuovo ciclo
    rovere -> Fill();
    gelso -> Fill();
    UrtiRiv1Gen -> Clear();
    UrtiRiv2Gen -> Clear();
    PuntatoreRiv1Reco -> Clear();
    PuntatoreRiv2Reco -> Clear();
  }


  // ----------------------------------------------------------------------------
  // Si riempie il file di output e si chiudono entrambi i file
  fileoutput -> Write();

  fileinput -> Close();
  fileoutput -> Close();
  
  return kTRUE;
}


void StampaInformazioni(const bool &onoff, const TString &distribuzione, const double &parametro1, const double &parametro2){
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


int Smearing(const TClonesArray *strato1Gen, const TClonesArray *strato2Gen, TClonesArray *strato1Reco, TClonesArray *strato2Reco, Rivelatore *detector){
  int u = 0;
  int v = 0;
  Urto* urto1L = NULL;
  Urto* urto2L = NULL;
  unsigned int numeroUrti1LGen = strato1Gen -> GetEntries();
  unsigned int numeroUrti2LGen = strato2Gen -> GetEntries();
  TClonesArray &strato1RecoInd = *strato1Reco;
  TClonesArray &strato2RecoInd = *strato2Reco;
  
  // Smearing degli urti generati - Layer 1
  for(int i = 0; i < (int)numeroUrti1LGen; i++){
    urto1L = ((Urto*)strato1Gen->At(i));
    
    if(TMath::Abs(urto1L -> GetZ()) <= (detector -> GetLunghezza())/2.){ //se non vale, il rivelatore non vede il passaggio
      urto1L -> SmearingGaussiano(detector, 1);
      new(strato1RecoInd[u]) Urto(*urto1L);
      u++;
    }
  }
  
  // Smearing degli urti generati - Layer 2
  for(int j = 0; j < (int)numeroUrti2LGen; j++){
    urto2L = ((Urto*)strato2Gen->At(j));
    
    if(TMath::Abs(urto2L -> GetZ()) <= (detector -> GetLunghezza())/2.){
      urto2L -> SmearingGaussiano(detector, 2);
      new(strato2RecoInd[v]) Urto(*urto2L);
      v++;
    }
  }

  return v;
}


int RumoreGauss(const double &media, const double &deviazionestandard, TClonesArray *strato1Reco, TClonesArray *strato2Reco, Rivelatore *detector){
  int u = strato1Reco -> GetEntries();
  int v = strato2Reco -> GetEntries();
  TClonesArray &strato1RecoInd = *strato1Reco;
  TClonesArray &strato2RecoInd = *strato2Reco;

  int numeroRumore1 = gRandom->Gaus(media, deviazionestandard);
  int numeroRumore2 = gRandom->Gaus(media, deviazionestandard);
  int numeroRumore = 0;
  Urto* rumore = NULL;
  double phi = 0.;
  double z = 0.;

  for(int i = 0; i < numeroRumore1; i++){
    phi = gRandom->Uniform(2*TMath::Pi());
    z = gRandom->Uniform(- detector->GetLunghezza()/2., detector->GetLunghezza()/2.);

    rumore = new Urto(phi, z, 1, detector, -1);
    new(strato1RecoInd[u]) Urto(*rumore);
    u += 1;
  }

  for(int i = 0; i < numeroRumore2; i++){
    phi = gRandom->Uniform(2*TMath::Pi());
    z = gRandom->Uniform(- detector->GetLunghezza()/2., detector->GetLunghezza()/2.);

    rumore = new Urto(phi, z, 2, detector, -1);
    new(strato2RecoInd[v]) Urto(*rumore);
    v += 1;
  }

  numeroRumore = numeroRumore1 + numeroRumore2;

  return numeroRumore;
}


int RumoreFissa(const double &parametro1, TClonesArray *strato1Reco, TClonesArray *strato2Reco, Rivelatore *detector){
  int u = strato1Reco -> GetEntries();
  int v = strato2Reco -> GetEntries();
  TClonesArray &strato1RecoInd = *strato1Reco;
  TClonesArray &strato2RecoInd = *strato2Reco;

  int numeroRumore = parametro1;
  Urto* rumore = NULL;
  double phi = 0.;
  double z = 0.;

  for(int i = 0; i < numeroRumore; i++){
    phi = gRandom->Uniform(2*TMath::Pi());
    z = gRandom->Uniform(- detector->GetLunghezza()/2., detector->GetLunghezza()/2.);

    rumore = new Urto(phi, z, 1, detector, -1);
    new(strato1RecoInd[u]) Urto(*rumore);
    u += 1;
  }

  for(int i = 0; i < numeroRumore; i++){
    phi = gRandom->Uniform(2*TMath::Pi());
    z = gRandom->Uniform(- detector->GetLunghezza()/2., detector->GetLunghezza()/2.);

    rumore = new Urto(phi, z, 2, detector, -1);
    new(strato2RecoInd[v]) Urto(*rumore);
    v += 1;
  }

  return numeroRumore*2;
}
