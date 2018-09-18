/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~ Ricostruzione degli eventi                              ~
  ~ Autori: Racca Eleonora - eleonora.racca288@edu.unito.it ~
  ~         Sauda Cristina - cristina.sauda@edu.unito.it    ~
  ~ Ultima modifica: 18/09/2018                             ~
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

#include "Punto.h"
#include "Rivelatore.h"
#include "Trasporto.h"
#include "Urto.h"
#include "Varie.h"
#include "Vertice.h"
#endif

// ******************************************************************************
// ************************ Dichiarazione delle funzioni ************************
// ******************************************************************************

// Funzione da chiamare per effettuare la ricostruzione
bool Ricostruzione(Rivelatore* detector, bool fileconfig);

// Funzione che richiede all'utente i parametri per la generazione del rumore
bool RichiestaInformazioni(bool &onoff, TString &distribuzione, double &parametro1, double &parametro2);

// Funzione che stampa i parametri per la generazione del rumore
void StampaInformazioni(bool &onoff, TString &distribuzione, double &parametro1, double &parametro2);

// Funzione per fare lo smearing dei punti ed eventualmente aggiungere il rumore
void SmearingRumore(TTree *originale, TTree *modificato, Rivelatore *detector);

// Funzione per ricostruire il vertice
void RicostruzioneVertice(TTree *modificato);



// ******************************************************************************
// *********************** Implementazione delle funzioni ***********************
// ******************************************************************************

bool Ricostruzione(Rivelatore* detector, bool fileconfig = kFALSE){

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
      cout << "!! File di configurazione non trovato !!" << endl << "La simulazione riparte automaticamente chiedendo di inserire a mano i parametri.";
      Ricostruzione(detector, kFALSE);
    }  
    in >> commento >> onoff >> distribuzione >> parametro1 >> parametro2;
    in.close();
    
    StampaInformazioni(onoff, distribuzione, parametro1, parametro2);
  }

  // File della simulazione e della ricostruzione
  TFile *fileinput = new TFile("Output/Simulazione.root");
  TFile *fileoutput = new TFile("Output/Ricostruzione.root", "RECREATE");

  if(fileinput->IsZombie()){
    cout << "Problema nel leggere il file Simulazione.root \nLa simulazione si interrompe." << endl;
    return kFALSE;
  }
  if(fileoutput->IsZombie()){
    cout << "Problema nel creare il file Ricostruzione.root \nLa simulazione si interrompe." << endl;
    return kFALSE;
  }
  fileoutput -> cd();

  // Tree della ricostruzione
  TTree *rovere = new TTree("rovere", "Tree della ricostruzione");
  
  // Smearing dei punti e aggiunta del rumore
  //void SmearingRumore();

  fileinput -> Close();
  fileoutput -> Close();
  
  return kTRUE;
}


void StampaInformazioni(bool &onoff, TString &distribuzione, double &parametro1, double &parametro2){
  cout << "------------- Parametri per la ricostruzione degli eventi --------------" << endl;
  cout << "I parametri per il rumore vengono letti dal file Ricostruzione.txt" << endl;
  cout << "Rumore:                              ";
  if(onoff){
    cout << "acceso"  << endl;
    cout << "  - Distribuzione:                     " << distribuzione << endl;
    if(distrumore == "gaussiana"){
      cout << "    * Media:                           " << parametro1 << endl;
      cout << "    * Deviazione standard:             " << parametro2 << endl;
    }
    else if(distrumore == "fissa"){
      cout << "    * Numero di rivelazioni:           " << parametro1 << endl;
    }
  }
  else cout << "spento" << endl << endl;
}


bool RichiestaInformazioni(bool &onoff, TString &distribuzione, double &parametro1, double &parametro2){
    cout << "-------------- Parametri per la generazione degli eventi ---------------" << endl;
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
      cout << "Inizializzazione sbagliata." << endl;
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
	cout << "Inizializzazione sbagliata: la ricostruzione si interrompe ora.";
	return kFALSE;
      }
    }
  }
  else cout << "Simulazione del rumore disattivata." << endl << endl;
  
  return kTRUE;
}
