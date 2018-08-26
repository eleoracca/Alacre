/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~ Implementazione dell'utility Varie                      ~
  ~ Autori: Racca Eleonora - eleonora.racca288@edu.unito.it ~
  ~         Sauda Cristina - cristina.sauda@edu.unito.it    ~
  ~ Ultima modifica: 25/08/2018                             ~
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#ifndef Varie_h
#define Varie_h

#include "TFile.h"
#include "TH1F.h"
#include "TMath.h"
#include "TRandom3.h"
#include "TString.h"

// ******************************************************************************
// ************************ Dichiarazione delle funzioni ************************
// ******************************************************************************

// Funzione che trasforma pseudorapidità in theta
double EtaTheta(bool &distribuzione, double inferiore, double superiore, TH1F* istogramma);

// Funzione che importa istogrammi
TH1F* ImportaIstogramma(TString file, TString istogramma);



// ******************************************************************************
// *********************** Implementazione delle funzioni ***********************
// ******************************************************************************

double EtaTheta(bool &distribuzione, double inferiore, double superiore, TH1F* istogramma){  
  double eta;
  
  do{
    if(distribuzione){
      eta = istogramma -> GetRandom();
    }
    else{
      eta = gRandom -> Uniform(inferiore, superiore);
    }
  }while((eta<0 && eta<=inferiore) || (eta>0 && eta>=superiore));
  
  return 2*TMath::ATan(TMath::Exp(-eta));
}


TH1F* ImportaIstogramma(TString file, TString istogramma){
  TFile *fileistogramma = new TFile(file);
  TH1F *isto = (TH1F*)fileistogramma -> Get(istogramma);

  isto -> SetDirectory(0);
  fileistogramma -> Close();
  delete fileistogramma;
  
  return isto;
}

#endif
