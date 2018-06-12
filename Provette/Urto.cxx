/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~ Implementazione della classe Urto                       ~
  ~ Autori: Racca Eleonora - eleonora.racca288@edu.unito.it ~
  ~         Sauda Cristina - cristina.sauda@edu.unito.it    ~
  ~ Ultima modifica: 15/11/2017                             ~
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#include "Urto.h"
#include "TMath.h"

ClassImp(Urto)

// Costruttori
Urto::Urto(): TObject(),
               dmX(0.),
               dmY(0.),
               dmZ(0.)
{}

Urto::Urto(double x, double y, double z): TObject(),
					    dmX(x),
					    dmY(y),
					    dmZ(z)
{}

// Distruttori
Urto::~Urto()
{}

// Getter
double Urto::GetX()
{
  return dmX;
}

double Urto::GetY()
{
  return dmY;
}

double Urto::GetZ()
{
  return dmZ;
}

// Setter
void Urto::SetX(double x)
{
  dmx = x;
}

void Urto::SetY(double y)
{
  dmY = y;
}

void Urto::SetZ(double z)
{
  dmZ = z;
}
