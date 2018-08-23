/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~ Implementazione della classe Vertice                    ~
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

//Costruttori
Vertice::Vertice(): Punto(),
                    dmMolteplicita(0),
                    //dmID(0),
                    dmRumore(0)
{}
//Vertice::Vertice(double X,double Y,double Z,int Mult,int IDnumber,double Noiselvl):
Vertice::Vertice(double X,double Y,double Z,int Molteplicita, double Rumore): Punto(X,Y,Z),
									      dmMolteplicita(Molteplicita),
									      //dmID(ID),
									      dmRumore(Rumore)
{}

//Distruttori
Vertice::~Vertice() 
{}

// ---------- Getter ----------
int Vertice::GetMolteplicita()
{
  return dmMolteplicita;
}

/*int Vertice::GetID()
{
  return dmID;
}*/

double Vertice::GetRumore()
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
