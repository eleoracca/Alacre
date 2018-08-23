/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~ Implementazione della classe Urto                       ~
  ~ Autori: Racca Eleonora - eleonora.racca288@edu.unito.it ~
  ~         Sauda Cristina - cristina.sauda@edu.unito.it    ~
  ~ Ultima modifica: 15/11/2017                             ~
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#if !defined (__CINT__) || defined (__MAKECINT__)
#include "Direzione.h"
#include "Punto.h"
#include "TMaterial.h"
#include "TMath.h"
#include "TRandom3.h"
#include "TSystem.h"
#include "Urto.h"
#include "Vertice.h"
#endif

ClassImp(Urto)

// Costruttori
Urto::Urto(): TObject(),
              dmX(0.),
              dmY(0.),
              dmZ(0.),
              dmNumLayer(0.),
              dmID(0.),
              dmUrtoReale(kFALSE)
{}

Urto::Urto(double x, double y, double z, double NumLayer, int ID): Punto(x, y, z),
								   dmNumLayer(NumLayer),
								   dmID(ID),
								   dmUrtoReale(kFALSE)
{}

// Distruttori
Urto::~Urto()
{}

// Setter
void Urto::SetX(double x)
{
  dmX = x;
}

void Urto::SetY(double y)
{
  dmY = y;
}

void Urto::SetZ(double z)
{
  dmZ = z;
}

void Urto::OraUrtoReale()
{
  dmUrtoReale = kTRUE;
}

void Urto::SetIDUrto(in ID)
{
  dmID = ID;
}

void Urto::SetNumLayerUrto(int NumLayer)
{
  dmNumLayer = NumLayer;
}

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

int Urto::GetNumeroLayer()
{
  return dmNumLayer;
}

bool Urto::TestReale()
{
  return dmID;
}

int Urto::GetIDUrto()
{
  return dmUrtoReale;
}

double Urto::CalcoloT(double Theta, double Phi, double XO, double YO, double Raggio)
{
  const double sintheta = TMath::Sin(Theta);
  const double sinphi = TMath::Sin(Phi);
  const double cosphi = TMath::Cos(Phi);
  const double t = (-(XO * cosphi + YO * sinphi) + TMath::Sqrt((XO * cosphi + YO * sinphi) * (XO * cosphi + YO * sinphi) - XO * XO - YO * YO + Raggio * Raggio))/sintheta;
  return t;
}

Urto Urto::UrtoSuVertice(Vertice& Origine, Trasporto& Direttrice, double Raggio, int IDe, int NumLayer)
{
  // Prende ϑ e phi da "Direzione".
  const double theta = Direttrice.GetDirTheta();
  const double phi = Direttrice.GetDirPhi();
  
  // Eccezione ϑ = 0
  if( theta == 0.f || theta == TMath::Pi() ) {
    Urto SuRivelatore;
    //  if (gDebug) Printf("ϑ=0 exception -> no scattering.");
    return SuRivelatore;
  }
  else {
    // Generare coordinate origine: xO, yO, zO "Vertice"
    const double xO = Origine.GetX();
    const double yO = Origine.GetY();
    const double zO = Origine.GetZ();
    
    //////////////////////////////////////////////////////////////////////////
    // Compute the "t" value.
    const double t = CalcoloT(theta,phi,xO,yO,Radius);
    
    // Build item in return.
    Urto OnCyl = Urto(xO+t*Direct.GetDirCos1(), yO+t*Direct.GetDirCos2(), zO+t*Direct.GetDirCos3(), Layerno, IDe);
    
    return OnCyl;
  }
}
