#include "Riostream.h"
#include "TBranch.h"
#include "TClonesArray.h"
#include "TFile.h"
#include "TMath.h"
#include "TRandom3.h"
#include "TString.h"
#include "TTree.h"

#include "Urto.h"
#include "Varie.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~ Generazione degli eventi                                ~
  ~ Autori: Racca Eleonora - eleonora.racca288@edu.unito.it ~
  ~         Sauda Cristina - cristina.sauda@edu.unito.it    ~
  ~ Ultima modifica: 13/06/2018                             ~
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// ******************************************************************************
// ************************ Dichiarazione delle funzioni ************************
// ******************************************************************************

// Funzione da chiamare per effettuare la simulazione
void Albero(bool fileconfig);

// Funzione che richiede all'utente i parametri per effettuare la simulazione
void RichiestaInformazioni(unsigned int &numeroeventi, TString &distmolteplicita, double &par1molteplicita, double &par2molteplicita, bool &multiplescattering, bool &rumore, bool &disteta, TString &distrumore, double &par1rumore, double &par2rumore);

// Funzione che stampa i parametri della simulazione
void StampaInformazioni(unsigned int &numeroeventi, TString &distmolteplicita, double &par1molteplicita, double &par2molteplicita, bool &multiplescattering, bool &rumore, bool &disteta, TString &distrumore, double &par1rumore, double &par2rumore);



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
    ifstream in("Configurazione.txt");
    if(!in){
      cout << "!! File di configurazione non trovato !!" << endl << "La simulazione ripartirà automaticamente chiedendo di inserire a mano i parametri.";
      Albero(kFALSE);
    }  
    in >> commento >> numeroeventi >> numeroeventi >> distmolteplicita >> par1molteplicita >> par2molteplicita >> multiplescattering >> rumore >> disteta >> distrumore >> par1rumore >> par2rumore;
    in.close();
    
    StampaInformazioni(numeroeventi, distmolteplicita, par1molteplicita, par2molteplicita, multiplescattering, rumore, disteta, distrumore, par1rumore, par2rumore);
  }
  
  // File di output per i dati generati dal Monte Carlo
  TFile fileoutput("AlberoDati.root", "RECREATE");

  // Tree della simulazione
  TTree *alberello = new TTree("alberello", "Tree della simulazione di Racca e Sauda");

  // Vertice della collisione
  TClonesArray *PuntatoreVertice = new TClonesArray("Vertice", numeroeventi);
  TClonesArray &IndPuntVertice = *PuntatoreVertice;

  // Urti sulla beam pipe
  TClonesArray *PuntatoreBP = new TClonesArray("Urto", numeroeventi);
  TClonesArray &IndPuntBP = *PuntatoreBP;

  // Urti sul primo rivelatore
  TClonesArray *PuntatoreRiv1 = new TClonesArray("Urto", numeroeventi);
  TClonesArray &IndPuntRiv1 = *PuntatoreRiv1;
  
  // Urti sul secondo rivelatore
  TClonesArray *PuntatoreRiv2 = new TClonesArray("Urto", numeroeventi);
  TClonesArray &IndPuntRiv2 = *PuntatoreRiv2;

  // Dichiaro i branch del tree
  alberello->Branch("VerticeMolteplicita", &PuntatoreVertice);
  alberello->Branch("UrtiBeamPipe", &PuntatoreBP);
  alberello->Branch("UrtiRivelatore1", &PuntatoreRiv1);
  alberello->Branch("UrtiRivelatore2", &RuntatoreRiv2);

  // Loop sugli eventi per creare i dati della simulazione
  for(int i = 0; i < numeroeventi; i++){

    // Generazione della molteplicità dell'evento
    int numeroparticelle = 0;

    //if con condizioni a seconda della forma della molteplicità
    while(numeroparticelle <= 0){
      numeroparticelle = (int)(0.5 + gRandom->Gaus(50., 20.));
    }
        
    // Generazione del vertice dell'evento, rms in centimetri
    /*
    vertice.molteplicita = numeroparticelle;
    vertice.X = gRandom->Gaus(0., 0.01);
    vertice.Y = gRandom->Gaus(0., 0.01);
    vertice.Z = gRandom->Gaus(0., 5.3);
    */

    // Generazione degli hit dell'evento
    /*
    for(j = 0; j < numeroparticelle; j++){
      new(IndPuntRiv1[j])
	
    }
    */
  }
}


void StampaInformazioni(unsigned int &numeroeventi, TString &distmolteplicita, double &par1molteplicita, double &par2molteplicita, bool &multiplescattering, bool &rumore, bool &disteta, TString &distrumore, double &par1rumore, double &par2rumore){

  cout << "------- Parametri per la generazione degli eventi --------" << endl;
  cout << "I parametri vengono letti dal file Configurazione.txt" << endl;
  cout << "+ Numero di eventi:                   " << numeroeventi << endl;
  cout << "+ Distribuzione della molteplicità:   " << distmolteplicita << endl;
  
  if(distmolteplicita == "gaussiana"){
    cout << "  - Media:                            " << par1molteplicita << endl;
    cout << "  - Deviazione standard:              " << par2molteplicita << endl;
  }
  else if(distmolteplicita == "uniforme"){
    cout << "  - Minimo:                           " << par1molteplicita << endl;
    cout << "  - Massimo:                          " << par2molteplicita << endl;
  }
  else if(distmolteplicita == "fissa"){
    cout << "  - Valore:                           " << par1molteplicita << endl;
  }
  
  cout << "+ Distribuzione della pseudorapidità: ";
  
  if(disteta){
    cout << "distribuzione assegnata" << endl;
  }
  else{
    cout << "uniforme" << endl;
    cout << "  - Minimo:                             -2" << endl;
    cout << "  - Massimo:                            2" << endl;
  }
  cout << "+ Scattering multiplo:                " << multiplescattering << endl;
  cout << "+ Rumore                              ";
  
  if(distrumore){
    cout << "acceso"  << endl;
    cout << "  - Distribuzione:                    " << distrumore << endl;
    if(distrumore == "gaussiana"){
      cout << "    * Media:                          " << par1rumore << endl;
      cout << "    * Deviazione standard:            " << par2rumore << endl;
    }
    else if(distrumore == "fissa"){
      cout << "    * Numero di rivelazioni:          " << par1rumore << endl;
    }
  }
  else cout << "spento" << endl << endl;
}


void RichiestaInformazioni(unsigned int &numeroeventi, TString &distmolteplicita, double &par1molteplicita, double &par2molteplicita, bool &multiplescattering, bool &rumore, bool &disteta, TString &distrumore, double &par1rumore, double &par2rumore){

    cout << "------- Parametri per la generazione degli eventi --------" << endl;
    cout << "Inserire i parametri per la simulazione" << endl;
    cout << "+ Numero di eventi:                   ";
    cin >> numeroeventi;
    cout << endl << "+ Distribuzione della molteplicità:   ";
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
    cin << par1molteplicita;
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
      cin << par1molteplicita;
    }
    else{
      cout << "Inizializzazione sbagliata: la simulazione si interromperà.";
      return;
    }
  }
  
  cout << endl << "+ Distribuzione della pseudorapidità: ";
  cin >> disteta;
  
  if(disteta){
    cout << "distribuzione assegnata" << endl;
  }
  else{
    cout << "uniforme" << endl;
    cout << "  - Minimo:                             -2" << endl;
    cout << "  - Massimo:                             2" << endl;
  }
  cout << endl << "+ Scattering multiplo:                ";
  cin >> multiplescattering;
  cout << endl << "+ Rumore                              ";
  cin >> distrumore;
  
  if(distrumore){
    cout << endl << "acceso"  << endl;
    cout << endl << "  - Distribuzione:                    ";
    cin >> distrumore;
    
    if(distrumore == "gaussiana"){
      cout << endl << "    * Media:                          ";
      cin >> par1rumore;
      cout << endl << "    * Deviazione standard:            ";
      cin >> par2rumore << endl;
    }
    else if(distrumore == "fissa"){
      cout << endl << "    * Numero di rivelazioni:          ";
      cin >> par1rumore << endl;
    }
    else{
      cout << "Inizializzazione sbagliata per la distribuzione di rumore." << endl;
      cout << "Scrivere gaussiana o fissa: " << endl;
      cin >> distrumore;
      
      if(distrumore == "gaussiana"){
	cout << endl << "    * Media:                            ";
	cin >> par1rumore;
	cout << endl << "    * Deviazione standard:              ";
	cin >> par2rumore;
      }
      else if(distrumore == "fissa"){
	cout << endl << "    * Valore:                           ";
	cin << par1rumore;
      }
      else{
	cout << "Inizializzazione sbagliata: la simulazione si interromperà.";
	return;
      }
    }
  }
  else cout << endl << "spento" << endl << endl;
  
}
