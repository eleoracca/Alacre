/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~ Dichiarazione della classe Trasporto                    ~
  ~ Autori: Racca Eleonora - eleonora.racca288@edu.unito.it ~
  ~         Sauda Cristina - cristina.sauda@edu.unito.it    ~
  ~ Ultima modifica: 27/08/2018                             ~
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#if !defined (__CINT__) || defined (__MAKECINT__)
#include "Trasporto.h"
#include <TMath.h>
#include <TObject.h>
#include <TString.h>
#endif

ClassImp(Trasporto)

// ----------- Costruttori -----------
Trasporto::Trasporto(): TObject(){
  dmID = 0;
  dmTheta = 0.;
  dmPhi = 0.;
  dmCDx = 0.;
  dmCDy = 0.;
  dmCDz = 0.;
  dmIsrotated = kFALSE;
};

Trasporto::Trasporto(double Theta, double Phi, int ID): TObject(){  
  dmID = ID;
  dmTheta = Theta;
  dmPhi = Phi;
  dmIsrotated = kFALSE;
  dmCDx = TMath::Sin(Theta) * TMath::Cos(Phi);
  dmCDy = TMath::Sin(Theta) * TMath::Sin(Phi);
  dmCDz = TMath::Cos(Theta);
};

// ----------- Distruttori -----------
Trasporto::~Trasporto()
{}

// ----------- Setter -----------
void Trasporto::SetDirTheta(const double Theta){
  dmTheta = Theta;
}

void Trasporto::SetDirPhi(const double Phi){
  dmPhi = Phi;
}

void Trasporto::SetCosDirX(const double Cx){
  dmCDx = Cx;
}

void Trasporto::SetCosDirY(const double Cy){
  dmCDy = Cy;
}

void Trasporto::SetCosDirZ(const double Cz){
  dmCDz = Cz;
}

void Trasporto::SetCosDir(const double Cx, const double Cy, const double Cz){
  dmCDx = Cx;
  dmCDy = Cy;
  dmCDz = Cz;
  AggiornaAng();
}

void Trasporto::SetAngoli(const double Theta, const double Phi){
  dmTheta = Theta;
  dmPhi = Phi;
  AggiornaDirCos();
}

void Trasporto::SetDirID(const double ID){
  dmID = ID;
}

// ----------- Getter -----------

int Trasporto::GetID(){
  return dmID;
}

double Trasporto::GetDirTheta(){
  return dmTheta;
}

double Trasporto::GetDirPhi(){
  return dmPhi;
}

double Trasporto::GetDirCosX(){
  return dmCDx;
}

double Trasporto::GetDirCosY(){
  return dmCDy;
}

double Trasporto::GetDirCosZ(){
  return dmCDz;
}

bool Trasporto::GetRotStatus(){
  return dmIsrotated;
}

void Trasporto::AggiornaAng(){
  
  // Siccome D[ATan(x)] = [-Pi,+Pi], bisogna discutere i valori dei coseni direttori per trovare il valore vero di Phi.
  
  dmTheta = TMath::ACos(dmCDz);
  
  if(dmCDx > 0){
    if(dmCDy >= 0){
      dmPhi = TMath::ATan(dmCDy/dmCDx);
    }
    else{
      dmPhi = 2*TMath::Pi() + TMath::ATan(dmCDy/dmCDx);
    }
  } 
  else{
    if(dmCDx != 0){ // CDx < 0
      if(dmCDy >= 0){
	dmPhi = TMath::Pi() + TMath::ATan(dmCDy/dmCDx);
      }
      else{
	dmPhi = TMath::Pi() + TMath::ATan(dmCDy/dmCDx);
      }
    }
    else{ // CDx = 0
      if(dmCDx >= 0){
	dmPhi = 0.5 * TMath::Pi();
      }
      else{
	dmPhi = 1.5 * TMath::Pi();
      }
    }
  }
}

void Trasporto::AggiornaDirCos(){
  dmCDx = TMath::Sin(dmTheta) * TMath::Cos(dmPhi);
  dmCDy = TMath::Sin(dmTheta) * TMath::Sin(dmPhi);
  dmCDz = TMath::Cos(dmTheta);
}

void Trasporto::FlipBit(){
  if(dmIsrotated) {
    dmIsrotated = kFALSE;
  }
  else {
    dmIsrotated = kTRUE;
  }
}

void Trasporto::Rotazione(double Theta, double Phi){
  
  // Definizione della matrice di Rotazione 2D
  double mr[3][3];
  mr[0][0] = -TMath::Sin(dmPhi);
  mr[1][0] = TMath::Cos(dmPhi);
  mr[2][0] = 0.;
  mr[0][1] = -TMath::Cos(dmPhi) * TMath::Cos(dmTheta);
  mr[1][1] = -TMath::Cos(dmTheta) * TMath::Sin(dmPhi);
  mr[2][1] = TMath::Sin(dmTheta);
  mr[0][2] = TMath::Sin(dmTheta) * TMath::Cos(dmPhi);
  mr[1][2] = TMath::Sin(dmTheta) * TMath::Sin(dmPhi);
  mr[2][2] = TMath::Cos(dmTheta);
  
  // Estrazione delle coordinate
  double cdp[3];
  cdp[0] = TMath::Sin(Theta) * TMath::Cos(Phi);
  cdp[1] = TMath::Sin(Theta) * TMath::Sin(Phi);
  cdp[2] = TMath::Cos(Theta);
  
  // Definizione dell'array risultante
  double Cd[3];
  
  // Risultato
  for(int i = 0; i < 3; i++){
    Cd[i] = 0;
    for(int j  =0; j < 3; j++){
      Cd[i] = Cd[i] + mr[i][j] * cdp[j];
    }
  }
  
  dmCDx = Cd[0];
  dmCDy = Cd[1];
  dmCDz = Cd[2];
  this->FlipBit();
  
  // Aggiorna Theta and Phi
  this->AggiornaAng();
}
