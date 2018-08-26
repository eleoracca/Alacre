/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~ Implementazione della classe Vertice                    ~
  ~ Autori: Racca Eleonora - eleonora.racca288@edu.unito.it ~
  ~         Sauda Cristina - cristina.sauda@edu.unito.it    ~
  ~ Ultima modifica: 26/08/2018                             ~
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#if !defined (__CINT__) || defined (__MAKECINT__)
#include "Punto.h"
#include "TMath.h"
#include "TRandom3.h"
#include "Vertice.h"
#endif

ClassImp(Vertice)

// ------------- Costruttori --------------
Vertice::Vertice(): Punto(){
  dmMolteplicita = 0;
  //dmID = 0;
  dmRumore = 0;
}

//Vertice::Vertice(double X, double Y, double Z, int Molteplicita, int Rumore, int ID)
Vertice::Vertice(double X, double Y, double Z, int Molteplicita, int Rumore): Punto(){
  Punto(X, Y, Z);
  dmMolteplicita = Molteplicita;
  //dmID = ID;
  dmRumore = Rumore;
}

Vertice::Vertice(double X, double  sX, double  Y, double  sY, double  Z, double  sZ, int  Molteplicita, int  Rumore): Punto(){
  Punto(gRandom->Gaus(X, sX), gRandom->Gaus(Y, sY), gRandom->Gaus(Z, sZ));
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

/*void Vertice::SetID(int ID){
   dmID = ID;
}*/

void Vertice::SetRumore(double Rumore){
  dmRumore = Rumore;
}

// ---------------- Getter ----------------
int Vertice::GetMolteplicita(){
  return dmMolteplicita;
}

/*int Vertice::GetID(){
  return dmID;
}*/

double Vertice::GetRumore(){
  return dmRumore;
}
