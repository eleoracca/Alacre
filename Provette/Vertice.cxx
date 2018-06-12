/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~ Implementazione della classe Vertice                      ~
  ~ Autori: Racca Eleonora - eleonora.racca288@edu.unito.it ~
  ~         Sauda Cristina - cristina.sauda@edu.unito.it    ~
  ~ Ultima modifica: 12/06/2018                             ~
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#if !defined (__CINT__) || defined (__MAKECINT__)
#include "Vertice.h"
#include "Punto.h"
#include "TMath.h"
#endif

ClassImp(Vertice)
Vertice::Vertice(): Punto(),
   dmMolteplicita(0),
  // dmID(0),
   dmRumore(0)
{}

//Costruttori
//Vertice::Vertice(double X,double Y,double Z,int Mult,int IDnumber,double Noiselvl):
Vertice::Vertice(double X,double Y,double Z,int Molteplicita, double Rumore):
   Punto(X,Y,Z),
   dmMolteplicita(Mlteplicita),
   //dmID(ID),
   dmRumore(Rumore)
{}

//Distruttori
Vertice::~Vertice() 

// ---------- Getter ----------
int GetMolteplicita()
{
   return dmMolteplicita;
}

/*int GetID()
{
   return dmID;
}*/

double GetRumore()
{
   return dmRumore;
}

// ---------- Setter ----------
void Vertice::SetMolteplicita(int Molteplicita) 
{
   dmMolteplicita = Molteplicita;
}

/*void Vertice::SetID(int ID) 
{
   dmID = ID;
}*/

void Vertice::SetRumore(double Rumore) 
{
   dmRumore = Rumore;
}
