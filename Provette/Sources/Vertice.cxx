/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~ Implementazione della classe Vertice                    ~
  ~ Autori: Racca Eleonora - eleonora.racca288@edu.unito.it ~
  ~         Sauda Cristina - cristina.sauda@edu.unito.it    ~
  ~ Ultima modifica: 26/09/2018                             ~
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#if !defined (__CINT__) || defined (__MAKECINT__)
#include "TMath.h"
#include "TRandom3.h"
#include "Vertice.h"
#include "Punto.h"
#endif

ClassImp(Vertice)

// ------------- Costruttori --------------
Vertice::Vertice(): Punto(){
  dmMolteplicita = 0;
  dmRumore = 0;
}

Vertice::Vertice(double X, double Y, double Z, int Molteplicita, int Rumore): Punto(X, Y, Z){
  dmMolteplicita = Molteplicita;
  dmRumore = Rumore;
}

Vertice::Vertice(Rivelatore *Detector, int  Molteplicita, int  Rumore){
  dmX = gRandom->Gaus(Detector->GetVerticeX(), Detector->GetVerticeSX());
  dmY = gRandom->Gaus(Detector->GetVerticeY(), Detector->GetVerticeSY());
  dmZ = gRandom->Gaus(Detector->GetVerticeZ(), Detector->GetVerticeSZ());
  dmMolteplicita = Molteplicita;
  dmRumore = Rumore;
}

// ------------- Distruttori --------------
Vertice::~Vertice() 
{}

// ---------------- Setter ----------------
void Vertice::SetMolteplicita(int Molteplicita){
  dmMolteplicita = Molteplicita;
}

void Vertice::SetRumore(int Rumore){
  dmRumore = Rumore;
}

// ---------------- Getter ----------------
int Vertice::GetMolteplicita(){
  return dmMolteplicita;
}

int Vertice::GetRumore(){
  return dmRumore;
}

Punto Vertice::GetCoordinate(){
  return Punto(dmX, dmY, dmZ);
}

double Vertice::TrovaVertice(Urto L1, Urto L2){
  double a = 0;
  double b = 0;

  a = (L2.GetRaggioC() - L1.GetRaggioC()) / (L2.GetZ() - L1.GetZ());
  b = (L1.GetRaggioC() * L2.GetZ() - L2.GetRaggioC() * L1.GetZ()) / (L2.GetZ() - L1.GetZ());

  return -b/a;
}