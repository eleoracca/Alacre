/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~ Generazione degli eventi                                ~
  ~ Autori: Racca Eleonora - eleonora.racca288@edu.unito.it ~
  ~         Sauda Cristina - cristina.sauda@edu.unito.it    ~
  ~ Ultima modifica: 31/08/2018                             ~
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
#include "TSystem.h"

#include "Interface/Punto.h"
#include "Interface/Rivelatore.h"
#include "Interface/Trasporto.h"
#include "Interface/Urto.h"
#include "Interface/Varie.h"
#include "Interface/Vertice.h"
#endif

// ******************************************************************************
// ************************ Dichiarazione delle funzioni ************************
// ******************************************************************************

// Funzione da chiamare per effettuare la simulazione
void Albero(bool fileconfig);

// Funzione che richiede all'utente i parametri per effettuare la simulazione
void RichiestaInformazioni(unsigned int &numeroeventi, TString &distmolteplicita, double &par1molteplicita, double &par2molteplicita, bool &multiplescattering, bool &rumore, bool &disteta, TString &distrumore, double &par1rumore, double &par2rumore);

// Funzione che stampa i parametri della simulazione
void StampaInformazioni(unsigned int &numeroeventi, TString &distmolteplicita, double &par1molteplicita, double &par2molteplicita, bool &multiplescattering, bool &rumore, bool &disteta, TString &distrumore, double &par1rumore, double &par2rumore);

// Funzione che decide la molteplicità
int DecisioneMolteplicita(TString &distribuzione, double &parametro1, double &parametro2);



// ******************************************************************************
// *********************** Implementazione delle funzioni ***********************
// ******************************************************************************

void Albero(bool fileconfig = kFALSE){
  
  // Inizializzazione e dichiazione dei parametri a zero
  TString commento = "\0";
  unsigned int numeroeventi = 1;
  TString distmolteplicita = "\0";
  double par1molteplicita = 0.;
  double par2molteplicita = 0.;
  bool multiplescattering = kFALSE;
  bool rumore = kFALSE;
  bool disteta = kFALSE;
  TString distrumore = "\0";
  double par1rumore = 0.;
  double par2rumore = 0.;
  
  // Inserimento dei parametri della simulazione
  if(!fileconfig){
    RichiestaInformazioni(numeroeventi, distmolteplicita, par1molteplicita, par2molteplicita, multiplescattering, rumore, disteta, distrumore, par1rumore, par2rumore);
  }
  else{
    ifstream in("Configurazioni/Configurazione.txt");
    if(!in){
      cout << "!! File di configurazione non trovato !!" << endl << "La simulazione ripartirà automaticamente chiedendo di inserire a mano i parametri.";
      Albero(kFALSE);
    }  
    in >> commento >> numeroeventi >> distmolteplicita >> par1molteplicita >> par2molteplicita >> multiplescattering >> rumore >> disteta >> distrumore >> par1rumore >> par2rumore;
    in.close();
    
    StampaInformazioni(numeroeventi, distmolteplicita, par1molteplicita, par2molteplicita, multiplescattering, rumore, disteta, distrumore, par1rumore, par2rumore);
  }
  
  // File di output per i dati generati dal Monte Carlo
  TFile *fileoutput = new TFile("Output/Simulazione.root", "RECREATE");
  if(fileoutput->IsZombie()){
    cout << "C'è stato un problema nel creare il file Simulazione.root. \nLa simulazione si interrompe." << endl;
    return;
  }
  fileoutput -> cd();

  // Tree della simulazione
  TTree *alberello = new TTree("alberello", "Tree della simulazione di Racca e Sauda");

  // Vertice della collisione e direzione
  Vertice *PuntatoreVertice = new Vertice();
  Vertice &IndPuntVertice = *PuntatoreVertice;
  
  Trasporto *PuntatoreDirezione = new Trasporto();
  Trasporto &IndPuntDirezione = *PuntatoreDirezione;

  // Urti sulla beam pipe
  TClonesArray *PuntatoreBP = new TClonesArray("Urto", 100);
  TClonesArray &IndPuntBP = *PuntatoreBP;

  // Urti sul primo rivelatore
  TClonesArray *PuntatoreRiv1 = new TClonesArray("Urto", 100);
  TClonesArray &IndPuntRiv1 = *PuntatoreRiv1;
  
  // Urti sul secondo rivelatore
  TClonesArray *PuntatoreRiv2 = new TClonesArray("Urto", 100);
  TClonesArray &IndPuntRiv2 = *PuntatoreRiv2;
  

  // Dichiaro i branch del tree
  alberello -> Branch("VerticeMolteplicita", &PuntatoreVertice);
  alberello -> Branch("UrtiBeamPipe", &PuntatoreBP);
  alberello -> Branch("UrtiRivelatore1", &PuntatoreRiv1);
  alberello -> Branch("UrtiRivelatore2", &PuntatoreRiv2);

  // Rivelatore
  Rivelatore *detector = new Rivelatore("Configurazioni/Rivelatore.txt");

  // Dichiarazione di theta ed eventuale caricamento della distribuzione di pseudorapidità
  double theta;
  TH1F *istogrammapseudorapidita = new TH1F();
  if(disteta){
    istogrammapseudorapidita = ImportaIstogramma("Configurazioni/kinem.root", "heta");
    istogrammapseudorapidita -> SetDirectory(0);
  }

  // Dichiarazione dell'azimut
  double phi;

  // Loop sugli eventi per creare i dati della simulazione
  for(int i = 0; i < (int)numeroeventi; i++){

    // Generazione della molteplicità dell'evento, ovvero il numero di particelle generato
    int numeroparticelle = DecisioneMolteplicita(distmolteplicita, par1molteplicita, par2molteplicita);
    
    // Generazione del vertice dell'evento, rms in centimetri
    PuntatoreVertice = new Vertice(detector->GetVerticeX(), detector->GetVerticeSX(), detector->GetVerticeY(), detector->GetVerticeSX(), detector->GetVerticeZ(), detector->GetVerticeSZ(), numeroparticelle);
    
    // Generazione degli urti dell'evento
    for(int j = 0; j < numeroparticelle; j++){

      // Generazione della direzione della particella j
      theta = EtaTheta(disteta, detector->GetEtaMin(), detector->GetEtaMax(), istogrammapseudorapidita);
      phi = gRandom -> Uniform(0., 2*TMath::Pi());
      PuntatoreDirezione -> SetAngoli(theta, phi)
      
      // Generazione dell'urto sulla beam pipe
      //new(IndPuntBP[j]) Urto::UrtoSuVertice(&PuntatoreDirezione, )
	
    }
    
    

    // Generazione del rumore nell'evento
    //int numerorumore = 0;

    alberello -> Fill();
    PuntatoreVertice -> Clear();
    PuntatoreBP -> Clear();
    PuntatoreRiv1 -> Clear();
    PuntatoreRiv2 -> Clear();
    
  }
  
  fileoutput -> Write();
  fileoutput -> Close();
  
  cout << "fine" << endl;
}


void StampaInformazioni(unsigned int &numeroeventi, TString &distmolteplicita, double &par1molteplicita, double &par2molteplicita, bool &multiplescattering, bool &rumore, bool &disteta, TString &distrumore, double &par1rumore, double &par2rumore){

  TString scattering = "\0";
  if(multiplescattering){
    scattering = "acceso";
  }
  else{
    scattering = "spento";
  }

  cout << "------- Parametri per la generazione degli eventi --------" << endl;
  cout << "I parametri vengono letti dal file Configurazione.txt" << endl;
  cout << "+ Numero di eventi:                    " << numeroeventi << endl;
  cout << "+ Distribuzione della molteplicità:    " << distmolteplicita << endl;
  
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
  
  cout << "+ Distribuzione della pseudorapidità:  ";
  
  if(disteta){
    cout << "distribuzione assegnata" << endl;
  }
  else{
    cout << "uniforme" << endl;
    cout << "  - Minimo:                            -2" << endl;
    cout << "  - Massimo:                            2" << endl;
  }
  cout << "+ Scattering multiplo:                 " << scattering << endl;
  cout << "+ Rumore                               ";
  
  if(rumore){
    cout << "acceso"  << endl;
    cout << "  - Distribuzione:                     " << distrumore << endl;
    if(distrumore == "gaussiana"){
      cout << "    * Media:                           " << par1rumore << endl;
      cout << "    * Deviazione standard:             " << par2rumore << endl;
    }
    else if(distrumore == "fissa"){
      cout << "    * Numero di rivelazioni:           " << par1rumore << endl;
    }
  }
  else cout << "spento" << endl << endl;
}


void RichiestaInformazioni(unsigned int &numeroeventi, TString &distmolteplicita, double &par1molteplicita, double &par2molteplicita, bool &multiplescattering, bool &rumore, bool &disteta, TString &distrumore, double &par1rumore, double &par2rumore){

  TString scattering = "\0";
  
    cout << "------- Parametri per la generazione degli eventi --------" << endl;
    cout << "Inserire i parametri per la simulazione" << endl;
    cout << "+ Numero di eventi:                    ";
    cin >> numeroeventi;
    cout << endl << "+ Distribuzione della molteplicità:    ";
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
    cout << "Inizializzazione sbagliata per la distribuzione di molteplicità." << endl;
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
      cout << "Inizializzazione sbagliata: la simulazione si interromperà.";
      return;
    }
  }
  
  cout << endl << "+ Distribuzione della pseudorapidità:  ";
  cin >> disteta;
  
  if(disteta){
    cout << "distribuzione assegnata" << endl;
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
    cout << "Inizializzazione sbagliata; inserire acceso o spento: ";
    cin >> scattering;
    if(scattering == "acceso"){
      multiplescattering = kTRUE;
    }
    else if(scattering == "spento"){
      multiplescattering = kFALSE;
    }
  }
  
  cout << endl << "+ Rumore                               ";
  cin >> distrumore;
  
  if(distrumore){
    cout << endl << "acceso"  << endl;
    cout << endl << "  - Distribuzione:                     ";
    cin >> distrumore;
    
    if(distrumore == "gaussiana"){
      cout << endl << "    * Media:                           ";
      cin >> par1rumore;
      cout << endl << "    * Deviazione standard:             ";
      cin >> par2rumore;
    }
    else if(distrumore == "fissa"){
      cout << endl << "    * Numero di rivelazioni:           ";
      cin >> par1rumore;
    }
    else{
      cout << "Inizializzazione sbagliata per la distribuzione di rumore." << endl;
      cout << "Scrivere gaussiana o fissa: " << endl;
      cin >> distrumore;
      
      if(distrumore == "gaussiana"){
	cout << endl << "    * Media:                             ";
	cin >> par1rumore;
	cout << endl << "    * Deviazione standard:               ";
	cin >> par2rumore;
      }
      else if(distrumore == "fissa"){
	cout << endl << "    * Valore:                            ";
	cin >> par1rumore;
      }
      else{
	cout << "Inizializzazione sbagliata: la simulazione si interromperà.";
	return;
      }
    }
  }
  else cout << endl << "spento" << endl << endl;
}


int DecisioneMolteplicita(TString &distribuzione, double &parametro1, double &parametro2){
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
      TH1F *istogramma = new TH1F();
      istogramma = ImportaIstogramma("Configurazioni/kinem.root", "hmul");
      istogramma -> SetDirectory(0);
      
      numero = istogramma -> GetRandom();
      
      delete istogramma;
    }
  }
  else if(distribuzione == "fissa"){
    while(numero == 0){
      numero = (int)parametro1;
    }
  }
  
  return numero;
}
