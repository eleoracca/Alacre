#ifndef Varie_h
#define Varie_h

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~ Implementazione dell'utility Varie                      ~
  ~ Autori: Racca Eleonora - eleonora.racca288@edu.unito.it ~
  ~         Sauda Cristina - cristina.sauda@edu.unito.it    ~
  ~ Ultima modifica: 25/08/2018                             ~
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#include "Riostream.h"
#include "TFile.h"
#include "TH1F.h"
#include "TMath.h"
#include "TRandom3.h"
#include "TString.h"
#include "Colori.h"

using namespace std;
using namespace colore;


// ******************************************************************************
// ************************ Dichiarazione delle funzioni ************************
// ******************************************************************************

// Funzione che trasforma pseudorapidit� in theta
double EtaTheta(bool &distribuzione, const double inferiore, const double superiore, TH1F* istogramma);

// Funzione che trasforma theta in pseudorapidità
double ThetaEta(const double theta);

// Funzione che importa istogrammi
TH1F* ImportaIstogramma(TString file, TString istogramma);

// Funzione che trova la moda
double Moda(TH1D *istogramma, double larghezza);



// ******************************************************************************
// *********************** Implementazione delle funzioni ***********************
// ******************************************************************************

double EtaTheta(bool &distribuzione, const double inferiore, const double superiore, TH1F* istogramma){  
  double eta = 0.0;
  
  do{
    if(distribuzione){
      eta = istogramma -> GetRandom();
    }
    else{
      eta = gRandom -> Uniform(inferiore, superiore);
    }
  }while(eta<inferiore || eta>superiore);
  
  return 2*TMath::ATan(TMath::Exp(-eta));
}


double ThetaEta(const double theta){
  return -TMath::Log(TMath::Tan(theta/2));
}


TH1F* ImportaIstogramma(TString file, TString istogramma){
  TFile *fileistogramma = new TFile(file);
  TH1F *isto = (TH1F*)fileistogramma -> Get(istogramma);

  isto -> SetDirectory(0);
  fileistogramma -> Close();
  delete fileistogramma;
  
  return isto;
}


double Moda(TH1D *istogramma, double larghezza){
	// Cerco la moda più a sinistra
	int binDelMassimo = istogramma -> GetMaximumBin();
	double moda = istogramma -> GetXaxis() -> GetBinCenter(binDelMassimo);

	// Controllo che esista una moda
	if(istogramma->GetBinContent(binDelMassimo)==0){
		cout << Errore("Moda della coordinata z = 0") << endl;
		return moda = -500;
	}
	else if(istogramma->GetBinContent(binDelMassimo)==1){
		cout << Avvertimento("Moda della coordinata z = 1") << endl;
		return moda = -600;
	}

	// Controllo che non ci siano altri massimi a destra
	int binDelMassimo2 = istogramma -> FindLastBinAbove(istogramma -> GetBinContent(binDelMassimo) - 1, 1, binDelMassimo +1, -1);
	double moda2 = istogramma -> GetXaxis() -> GetBinCenter(binDelMassimo2);
	
	if(istogramma -> GetBinContent(binDelMassimo2) == istogramma -> GetBinContent(binDelMassimo)){
		// Cerco il massimo migliore
		if(fabs(moda - moda2) < larghezza){
			double media = (moda + moda2)/2.;
			return media;
		}
		else{
			if(fabs(moda) <= fabs(moda2)){
				return moda;
			}
			else{
				return moda2;
			}
		}
	}
	else
		return moda;
}

#endif
