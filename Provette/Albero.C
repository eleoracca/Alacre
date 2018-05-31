#include "Riostream.h"
#include "TBranch.h"
#include "TClonesArray.h"
#include "TFile.h"
#include "TMath.h"
#include "TRandom3.h"
#include "TTree.h"

#include "Colpi.h"
#include "Varie.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~ Generazione degli eventi                                ~
  ~ Autori: Racca Eleonora - eleonora.racca288@edu.unito.it ~
  ~         Sauda Cristina - cristina.sauda@edu.unito.it    ~
  ~ Ultima modifica: 15/11/2017                             ~
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void Albero(int numeroeventi){  
  // File di output per i dati generati dal Monte Carlo e TTree
  TFile fileoutput("AlberoDati.root", "RECREATE");
  TTree *alberello = new TTree("quercia", "Tree di Racca e Sauda");

  // Hit sul primo rivelatore
  TClonesArray *PuntatoreRiv1 = new TClonesArray("Colpi", numeroeventi);
  TClonesArray &IndPuntRiv1 = *PuntatoreRiv1;
  
  // Hit sul secondo rivelatore
  TClonesArray *PuntatoreRiv2 = new TClonesArray("Colpi", numeroeventi);
  TClonesArray &IndPuntRiv2 = *PuntatoreRiv2;

  // Vertice della collisione
  static VTX vertice;

  // Dichiaro i branch del tree
  alberello->Branch("VerticeMolteplicita", &vertice.X, "X/D:Y:Z:molteplicita/I");
  alberello->Branch("HitsRivelatore1", &PuntatoreRiv1);
  alberello->Branch("HitsRivelatore2", &RuntatoreRiv2);

  // Loop sugli eventi per creare i dati della simulazione
  for(int i=0; i < numeroeventi; i++){

    // Generazione della molteplicità randomica dell'evento
    // int numeroparticelle = 50;
    // /*
    int numeroparticelle = 0;
    while(numeroparticelle <= 0){
      numeroparticelle = (int)(0.5 + gRandom->Gaus(50., 20.));
    }
    // */
    
    // Generazione del vertice dell'evento, rms in centimetri
    vertice.molteplicita = numeroparticelle;
    vertice.X = gRandom->Gaus(0., 0.01);
    vertice.Y = gRandom->Gaus(0., 0.01);
    vertice.Z = gRandom->Gaus(0., 5.3);

    // Generazione degli hit dell'evento
    for(j = 0; j < numeroparticelle; j++){
      new(IndPuntRiv1[j])
	
    }
  }
}
