/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~ Generazione degli eventi                                ~
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

// Funzione da chiamare per effettuare la simulazione
bool Albero(Rivelatore* detector, bool fileconfig);

// Funzione che richiede all'utente i parametri per effettuare la simulazione
bool RichiestaInformazioni(unsigned int &numeroeventi, TString &distmolteplicita, double &par1molteplicita, double &par2molteplicita, bool &multiplescattering, bool &disteta);

// Funzione che stampa i parametri della simulazione
void StampaInformazioni(unsigned int &numeroeventi, TString &distmolteplicita, double &par1molteplicita, double &par2molteplicita, bool &multiplescattering, bool &disteta);

// Funzione che decide la molteplicità
int DecisioneMolteplicita(TString &distribuzione, double &parametro1, double &parametro2, TH1F *istogramma);



// ******************************************************************************
// *********************** Implementazione delle funzioni ***********************
// ******************************************************************************

bool Albero(Rivelatore* detector, bool fileconfig = kTRUE){
  
  // ----------------------------------------------------------------------------
  // Inizializzazione e dichiazione dei parametri a zero
  bool multiplescattering = kFALSE;
  bool disteta = kFALSE;
  bool richiesta = kTRUE;
  double par1molteplicita = 0.;
  double par2molteplicita = 0.;
  TString commento = "\0";
  TString distmolteplicita = "\0";
  unsigned int numeroeventi = 1;
  
  // Inserimento dei parametri della simulazione
  if(!fileconfig){
    richiesta = RichiestaInformazioni(numeroeventi, distmolteplicita, par1molteplicita, par2molteplicita, multiplescattering, disteta);
  
    if(!richiesta){
      return kFALSE;
    }
  }
  else{
    ifstream in("Configurazioni/Generazione.txt");
    if(!in){
      cout << Violetto("!! File di configurazione non trovato !!") << endl << "La simulazione riparte automaticamente chiedendo di inserire a mano i parametri.";
      Albero(detector, kFALSE);
    }  
    in >> commento >> numeroeventi >> distmolteplicita >> par1molteplicita >> par2molteplicita >> multiplescattering >> disteta;
    in.close();
    
    StampaInformazioni(numeroeventi, distmolteplicita, par1molteplicita, par2molteplicita, multiplescattering, disteta);
  }
  
  // File di output per i dati generati dal Monte Carlo
  TFile *fileoutput = new TFile("Output/Simulazione.root", "RECREATE");
  if(fileoutput->IsZombie()){
    cout << Avvertimento("Problema nel creare il file Simulazione.root. \nLa simulazione si interrompe.") << endl;
    return kFALSE;
  }
  
  // ----------------------------------------------------------------------------
  fileoutput -> cd();

  // Tree della simulazione
  TTree *gaggia = new TTree("gaggia", "Tree della simulazione");

  // Vertice della collisione e direzione
  Vertice *PuntatoreVertice = new Vertice();
  Vertice& IndPuntVertice = *PuntatoreVertice;  
  Trasporto *PuntatoreDirezione = new Trasporto();
  Trasporto& IndPuntDirezione = *PuntatoreDirezione;

  // Urti sulla beam pipe
  TClonesArray *PuntatoreBP = new TClonesArray("Urto", 100);
  TClonesArray& IndPuntBP = *PuntatoreBP;
  Urto UrtoBP;

  // Urti sul primo rivelatore
  TClonesArray *PuntatoreRiv1 = new TClonesArray("Urto", 100);
  TClonesArray& IndPuntRiv1 = *PuntatoreRiv1;
  Urto Urto1L;
  
  // Urti sul secondo rivelatore
  TClonesArray *PuntatoreRiv2 = new TClonesArray("Urto", 100);
  TClonesArray& IndPuntRiv2 = *PuntatoreRiv2;
  Urto Urto2L;    

  // Dichiaro i branch del tree
  gaggia -> Branch("Vertice", &PuntatoreVertice);
  gaggia -> Branch("UrtiBeamPipe", &PuntatoreBP);
  gaggia -> Branch("UrtiRivelatore1", &PuntatoreRiv1);
  gaggia -> Branch("UrtiRivelatore2", &PuntatoreRiv2);

  // ----------------------------------------------------------------------------
  // Dichiarazione della molteplicità ed eventuale caricamento della distribuzione da istogramma
  int numeroparticelle = 0;
  TH1F *istogrammamolteplicita = new TH1F();
  if(distmolteplicita == "istogramma"){
    istogrammamolteplicita = ImportaIstogramma("Configurazioni/kinem.root", "hmul");
    istogrammamolteplicita -> SetDirectory(0);
  }

  // Dichiarazione degli angoli ed eventuale caricamento della distribuzione di pseudorapidità da istogramma
  double phi;
  double theta;
  TH1F *istogrammapseudorapidita = new TH1F();
  if(disteta){
    istogrammapseudorapidita = ImportaIstogramma("Configurazioni/kinem.root", "heta");
    istogrammapseudorapidita -> SetDirectory(0);
  }

  // ----------------------------------------------------------------------------
  // Loop sugli eventi per creare i dati della simulazione
  for(int i = 0; i < (int)numeroeventi; i++){

    // Generazione della molteplicità dell'evento, ovvero il numero di particelle generato
    numeroparticelle = DecisioneMolteplicita(distmolteplicita, par1molteplicita, par2molteplicita, istogrammamolteplicita);
    
    // Generazione del vertice dell'evento, rms in centimetri
    PuntatoreVertice = new Vertice(detector, numeroparticelle);
    
    // Generazione degli urti dell'evento
    for(int j = 0; j < numeroparticelle; j++){

      // Generazione della direzione della particella j
      theta = EtaTheta(disteta, detector->GetEtaMin(), detector->GetEtaMax(), istogrammapseudorapidita);
      phi = gRandom -> Uniform(0., 2*TMath::Pi());
      PuntatoreDirezione -> SetAngoli(theta, phi);
      
      // Generazione dell'urto sulla beam pipe
      UrtoBP = Urto::UrtodaVertice(PuntatoreVertice, PuntatoreDirezione, detector->GetRaggioBP(), j, 0);
      UrtoBP.SetUrtoReale();
      
      // Generazione dell'urto sul primo strato
      Urto1L = UrtoBP.UrtodaUrto(PuntatoreDirezione, detector, multiplescattering, 1);
      Urto1L.SetUrtoReale();
      
      if(multiplescattering){
        PuntatoreDirezione->FlipBit();
      }
      
      // Generazione dell'urto sul secondo strato
      Urto2L = Urto1L.UrtodaUrto(PuntatoreDirezione, detector, multiplescattering, 2);
      Urto2L.SetUrtoReale();
      
      // Registrazione dei dati sul Tree
      new(IndPuntBP[j]) Urto(UrtoBP);
      new(IndPuntRiv1[j]) Urto(Urto1L);
      new(IndPuntRiv2[j]) Urto(Urto2L);	
    }

  // ----------------------------------------------------------------------------
    // Si riempie il tree e si cancellano gli array per il nuovo ciclo
    gaggia -> Fill();
    PuntatoreVertice -> Clear();
    PuntatoreBP -> Clear();
    PuntatoreRiv1 -> Clear();
    PuntatoreRiv2 -> Clear();
  }

  // Finisce la scrittura del file e viene chiuso
  fileoutput -> Write();
  fileoutput -> Close();

  // Distruzione delle variabili nello heap
  istogrammapseudorapidita->~TH1F();
  istogrammamolteplicita->~TH1F();
  detector->~Rivelatore();
  PuntatoreVertice->~Vertice();
  PuntatoreDirezione->~Trasporto();
  UrtoBP.~Urto();
  Urto1L.~Urto();
  Urto2L.~Urto();
  
  return kTRUE;
}


void StampaInformazioni(unsigned int &numeroeventi, TString &distmolteplicita, double &par1molteplicita, double &par2molteplicita, bool &multiplescattering, bool &disteta){

  TString scattering = "\0";
  if(multiplescattering){
    scattering = "acceso";
  }
  else{
    scattering = "spento";
  }

  cout << "-------------- " << Arancione("Parametri per la generazione degli eventi") << " ---------------" << endl;
  cout << "I parametri per la generazione vengono letti dal file " << Azzurro("Generazione.txt") << endl;
  cout << "+ Numero di eventi:                    " << numeroeventi << endl;
  cout << "+ Distribuzione della molteplicita:    " << distmolteplicita << endl;
  
  if(distmolteplicita == "gaussiana"){
    cout << "  - Media:                             " << par1molteplicita << endl;
    cout << "  - Deviazione standard:               " << par2molteplicita << endl;
  }
  else if(distmolteplicita == "uniforme"){
    cout << "  - Minimo:                            " << par1molteplicita << endl;
    cout << "  - Massimo:                           " << par2molteplicita << endl;
  }
  else if(distmolteplicita == "fissa"){
    cout << "  - Valore:                            " << par1molteplicita << endl;
  }
  
  cout << "+ Distribuzione della pseudorapidita:  ";  
  if(disteta){
    cout << "distribuzione assegnata" << endl;
  }
  else{
    cout << "uniforme" << endl;
    cout << "  - Minimo:                            -2" << endl;
    cout << "  - Massimo:                            2" << endl;
  }
  
  cout << "+ Scattering multiplo:                 " << scattering << endl;
}


bool RichiestaInformazioni(unsigned int &numeroeventi, TString &distmolteplicita, double &par1molteplicita, double &par2molteplicita, bool &multiplescattering, bool &disteta){

  TString scattering = "\0";
  
  cout << "-------------- " << Arancione("Parametri per la generazione degli eventi") << " ---------------" << endl;
  cout << "Inserire i parametri per la simulazione della generazione" << endl;
  cout << "+ Numero di eventi:                    ";
  cin >> numeroeventi;
  cout << endl << "+ Distribuzione della molteplicita:    ";
  cin >> distmolteplicita;
  
  if(distmolteplicita == "gaussiana"){
    cout << endl << "  - Media:                             ";
    cin >> par1molteplicita;
    cout << endl << "  - Deviazione standard:               ";
    cin >> par2molteplicita;
  }
  else if(distmolteplicita == "uniforme"){
    cout << endl << "  - Minimo:                            ";
    cin >> par1molteplicita;
    cout << endl << "  - Massimo:                           ";
    cin >> par2molteplicita;
  }
  else if(distmolteplicita == "fissa"){
    cout << endl << "  - Valore:                            ";
    cin >> par1molteplicita;
  }
  else{
    cout << Violetto("Inizializzazione sbagliata per la distribuzione di molteplicita.") << endl;
    cout << "Scrivere gaussiana, uniforme o fissa: " << endl;
    cin >> distmolteplicita;
    
    if(distmolteplicita == "gaussiana"){
      cout << endl << "  - Media:                            ";
      cin >> par1molteplicita;
      cout << endl << "  - Deviazione standard:              ";
      cin >> par2molteplicita;
    }
    else if(distmolteplicita == "uniforme"){
      cout << endl << "  - Minimo:                           ";
      cin >> par1molteplicita;
      cout << endl << "  - Massimo:                          ";
      cin >> par2molteplicita;
    }
    else if(distmolteplicita == "fissa"){
      cout << endl << "  - Valore:                           ";
      cin >> par1molteplicita;
    }
    else{
      cout << Avvertimento("Inizializzazione sbagliata: la simulazione si interrompe ora.");
      return kFALSE;
    }
  }
  
  cout << endl << "+ Distribuzione della pseudorapidita:  ";
  cin >> disteta;
  
  if(disteta){
    cout << "distribuzione assegnata dall'istogramma" << endl;
  }
  else{
    cout << "uniforme" << endl;
    cout << "  - Minimo:                              -2" << endl;
    cout << "  - Massimo:                              2" << endl;
  }
  
  cout << endl << "+ Scattering multiplo:                 ";
  cin >> scattering;
  if(scattering == "acceso"){
    multiplescattering = kTRUE;
  }
  else if(scattering == "spento"){
    multiplescattering = kFALSE;
  }
  else{
    cout << Violetto("Inizializzazione sbagliata per lo scattering multiplo inserire acceso o spento: ");
    cin >> scattering;
    if(scattering == "acceso"){
      multiplescattering = kTRUE;
    }
    else if(scattering == "spento"){
      multiplescattering = kFALSE;
    }
    else{
      cout << Avvertimento("Inizializzazione sbagliata: la simulazione si interrompe ora.");
      return kFALSE;
    }
  }
  
  return kTRUE;
}


int DecisioneMolteplicita(TString &distribuzione, double &parametro1, double &parametro2, TH1F *istogramma){
  int numero = 0;

  if(distribuzione == "gaussiana"){
    while(numero == 0){
      numero = (int)(0.5 + gRandom->Gaus(parametro1, parametro2));
    }
  }
  else if(distribuzione == "uniforme"){
    while(numero == 0){
      numero = (int)(0.5 + gRandom->Uniform(parametro1, parametro2));
    }
  }
  else if(distribuzione == "istogramma"){
    while(numero == 0){      
      numero = istogramma -> GetRandom();
    }
  }
  else if(distribuzione == "fissa"){
    while(numero == 0){
      numero = (int)parametro1;
    }
  }
  
  return numero;
}
