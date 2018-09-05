/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~ Ricostruzione degli eventi                              ~
  ~ Autori: Racca Eleonora - eleonora.racca288@edu.unito.it ~
  ~         Sauda Cristina - cristina.sauda@edu.unito.it    ~
  ~ Ultima modifica: 05/09/2018                             ~
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
#include "TSystem.h"
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
void Ricostruzione();



// ******************************************************************************
// *********************** Implementazione delle funzioni ***********************
// ******************************************************************************

void Ricostruzione(){

  // File della simulazione e della ricostruzione
  TFile *fileinput = new TFile("Output/Simulazione.root");
  TFile *fileoutput = new TFile("Output/Ricostruzione.root", "RECREATE");

  // Smearing dei punti perché ricostruiti da un rivelatore reale

  // Aggiunta del rumore, se richiesto
  
}
