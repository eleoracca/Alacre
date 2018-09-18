/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~ Implementazione della classe Vertice                    ~
  ~ Autori: Racca Eleonora - eleonora.racca288@edu.unito.it ~
  ~         Sauda Cristina - cristina.sauda@edu.unito.it    ~
  ~ Ultima modifica: 18/09/2018                             ~
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#if !defined (__CINT__) || defined (__MAKECINT__)
#include "TMath.h"
#include "TRandom3.h"
#include "Punto.h"
#include "Vertice.h"
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

void Vertice::SetRumore(double Rumore){
  dmRumore = Rumore;
}

// ---------------- Getter ----------------
int Vertice::GetMolteplicita(){
  return dmMolteplicita;
}

double Vertice::GetRumore(){
  return dmRumore;
}
