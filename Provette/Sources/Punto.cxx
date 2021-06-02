/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~ Implementazione della classe Punto                      ~
  ~ Autori: Racca Eleonora - eleonora.racca288@edu.unito.it ~
  ~         Sauda Cristina - cristina.sauda@edu.unito.it    ~
  ~ Ultima modifica: 26/08/2018                             ~
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#if !defined (__CINT__) || defined (__MAKECINT__)
#include "Punto.h"
#include "TMath.h"
#include "TRandom3.h"
#include "TString.h"
#endif

ClassImp(Punto)

// ------------- Costruttori --------------
Punto::Punto(): TObject() {
  dmX = 0.;
  dmY = 0.;
  dmZ = 0.;
  dmTheta = 0.;
  dmPhi = 0.;
  dmRaggioC = 0.;
  dmRaggioS = 0.; 
}

Punto::Punto(double X, double Y, double Z): TObject() {
  dmX = X;
  dmY = Y;
  dmZ = Z;

  CartesianeSferiche(); // Assegna i valori delle coordinate sferiche
  CartesianeCilindriche(); // Assegna i valori delle coordinate cilindriche
}

Punto::Punto(double R, double Phi, double X3, char tipo){
  if(tipo == "c"){
    dmRaggioC = R;
    dmPhi = Phi;
    dmZ = X3;

    CilindricheCartesiane();
    CartesianeSferiche();
  }
  else if(tipo == "s"){
    dmRaggioS = R;
    dmPhi = Phi;
    dmTheta = X3;

    SfericheCartesiane();
    CartesianeCilindriche();
  }
  else{
    cout << "Tipo di coordinate sconosciuto: inserire s per sferiche o c per cilindriche" << endl;
    return kFALSE;
  }
}

// ------------- Distruttori --------------
Punto::~Punto(){}

// ---------------- Setter ----------------
void Punto::SetCartesiane(const double X, const double Y, const double Z){
  dmX = X;
  dmY = Y;
  dmZ = Z;
  
  CartesianeSferiche();
  CartesianeCilindriche();
}

void Punto::SetCilindriche(const double RaggioC, const double Phi, const double Z){
  dmZ = Z;
  dmPhi = Phi;
  dmRaggioC = RaggioC;
  
  CilindricheCartesiane();
  CartesianeSferiche();
}

void Punto::SetSferiche(const double RaggioS, const double Theta, const double Phi){
  dmTheta = Theta;
  dmPhi = Phi;
  dmRaggioS = RaggioS;
  
  SfericheCartesiane();
  CartesianeCilindriche();
}

void Punto::SetX(const double X){
  dmX = X;
  
  CartesianeSferiche();
  CartesianeCilindriche();
}

void Punto::SetY(const double Y){
  dmY = Y;
  
  CartesianeSferiche();
  CartesianeCilindriche();
}

void Punto::SetZ(const double Z){
  dmZ = Z;
  
  CartesianeSferiche();
  CartesianeCilindriche();
}

void Punto::SetTheta(const double Theta){
  dmTheta = Theta;
  
  SfericheCartesiane();
  CartesianeCilindriche();
}

void Punto::SetPhi(const double Phi){
  dmPhi = Phi;
  
  CilindricheCartesiane();
  CartesianeSferiche();
}

void Punto::SetRaggioC(const double RaggioC){
  dmRaggioC = RaggioC;
  
  CilindricheCartesiane();
  CartesianeSferiche();
  
  if (dmX == 0. && dmY == 0.){
    Printf("Attenzione: Coordinate X e Y nulle! \
           \n Potrebbero esserci problemi con le conversioni delle coordinate. \
           \n Controlla i valori inseriti e il codice!");
  }
}

void Punto::SetRaggioS(const double RaggioS){
  dmRaggioS = RaggioS;
  
  SfericheCartesiane();
  CartesianeCilindriche();
  
  if (dmX == 0. && dmY == 0. && dmZ == 0.){
    Printf("Attenzione: Coordinate X, Y e Z nulle! \
           \n Potrebbero esserci problemi con le conversioni delle coordinate. \
           \n Controlla i valori inseriti e il codice!");
  }
}


// ---------------- Getter ----------------
double Punto::GetX(){
  return dmX;
}

double Punto::GetY(){
  return dmY;
}

double Punto::GetZ(){
  return dmZ;
}

double Punto::GetTheta(){
  return dmTheta;
}

double Punto::GetPhi(){
  return dmPhi;
}

double Punto::GetRaggioC(){
  return dmRaggioC;
}

double Punto::GetRaggioS(){
  return dmRaggioS;
}

double Punto::GetDistanza(Punto &PuntoUno, Punto &PuntoDue){
  return TMath::Sqrt((PuntoUno.GetX() - PuntoDue.GetX()) * (PuntoUno.GetX() - PuntoDue.GetX()) + (PuntoUno.GetY() - PuntoDue.GetY()) * (PuntoUno.GetY() - PuntoDue.GetY()) + (PuntoUno.GetZ() - PuntoDue.GetZ()) * (PuntoUno.GetZ() - PuntoDue.GetZ()));
}

double Punto::GetDeltaPhi(Punto &PuntoUno, Punto &PuntoDue){
  return TMath::Abs(PuntoUno.GetPhi() - PuntoDue.GetPhi());
}


// ----------- Member functions -----------
void Punto::CartesianeCilindriche(){
  dmRaggioC = TMath::Sqrt(dmX*dmX + dmY*dmY);
  // dmZ rimane sè stesso

  if (dmX != 0. && dmY != 0.){
    dmPhi = TMath::ATan2(dmY, dmX);
  }
  else {
    dmPhi = 0.;
  }
}

void Punto::CartesianeSferiche(){
  dmRaggioS = TMath::Sqrt(dmX*dmX + dmY*dmY + dmZ*dmZ);

  if (dmRaggioS != 0.){
    
    dmTheta = TMath::ACos(dmZ/dmRaggioS);
    
    if (dmX != 0. && dmY != 0.){
      dmPhi = TMath::ATan2(dmY, dmX);
    }
    else {
      dmPhi = 0.;
      dmTheta = 0.;
    }
  }
  else {
    dmPhi = 0.;
    dmTheta = 0.;   // valore di default per evitare errori
  }
}

void Punto::SfericheCartesiane(){
  dmX = dmRaggioS * TMath::Sin(dmTheta) * TMath::Cos(dmPhi);
  dmY = dmRaggioS * TMath::Sin(dmTheta) * TMath::Sin(dmPhi);
  dmZ = dmRaggioS * TMath::Cos(dmTheta);
}

void Punto::CilindricheCartesiane(){
  dmX = dmRaggioC * TMath::Cos(dmPhi);
  dmY = dmRaggioC * TMath::Sin(dmPhi);
}
