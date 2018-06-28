/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~ Implementazione della classe Urto                       ~
  ~ Autori: Racca Eleonora - eleonora.racca288@edu.unito.it ~
  ~         Sauda Cristina - cristina.sauda@edu.unito.it    ~
  ~ Ultima modifica: 15/11/2017                             ~
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/




Hit Hit::GetHitOnCyl(Direzione& Direct,Double_t Radius,TMaterial& Material,
   Double_t Width,Bool_t Multiscat,Int_t Layno,Double_t P,Int_t Z,Double_t Beta)
{
   if(Multiscat) {

      //////////////////////////////////////////////////////////////////////////
      // θo represents the rms of a gaussian distribution centred
      // in 0.
      // Formula used for the MScattering θo evaluation:
      // Dimensionalities:
      // [x]=cm; [X_0]=cm; [c]=3x10**8; [b]=#; [fP]=MeV/c
      // θo = (13.6 MeV/β*c*p)*Z*(√(x/X_0))[1+0.038*Ln(x/X_0)]
      //////////////////////////////////////////////////////////////////////////
      // Get the Radiation Length: X_0
      const Double_t X_0=Material.GetRadLength();

      //////////////////////////////////////////////////////////////////////////
      // WARNING!
      // Since fP should be passed with a dimensionality of [fP]= MeV/c,
      // it's obvious that "c" wouldn't appear in this operation.
      const Double_t ThetaZero=(13.6/(Beta*P))*Z*TMath::Sqrt(Width/X_0)*(1+
         0.038*TMath::Log(Width/X_0));

      //////////////////////////////////////////////////////////////////////////
      // Generate θ form Gaussian distribution with mean 0 and
      // rms θo. Phi is uniformely distributed between 0<=φ<2Pi
      const Double_t thetalocal=gRandom->Gaus(0.,ThetaZero);
      const Double_t philocal=gRandom->Rndm()*2*TMath::Pi();

      //////////////////////////////////////////////////////////////////////////
      // Rotate direction in order to refer it to the Laboratory
      // Reference System.
      Direct.Direzione::Rotate(thetalocal,philocal);
   }

   // Extract theta and phi from "Direzione".
   const Double_t theta=Direct.GetDirectTheta();
   const Double_t phi=Direct.GetDirectPhi();

   /////////////////////////////////////////////////////////////////////////////
   // Manage the ϑ=0 exception
   if(theta==0.f||TMath::Abs(theta)==TMath::Pi()) {
      Hit OnCyl;
      if (gDebug) Printf("ϑ=0 exception -> no scattering.");

      return OnCyl;
   } else {

      //////////////////////////////////////////////////////////////////////////
      // Compute "t" value.
      const Double_t t=ComputeT(theta,phi,fX,fY,Radius);

      // Returned item.
      Hit OnCyl=Hit(fX+t*Direct.GetDirCos1(),fY+t*Direct.GetDirCos2(),
         fZ+t*Direct.GetDirCos3(),Layno,fIdnumber);
      return OnCyl;
   }
}

////////////////////////////////////////////////////////////////////////////////
// One can find the theta RMS dividing RPhi by the detector radius.
void Hit::GausSmearing(Double_t DRadius,Double_t RMSZeta,Double_t RMSRPhi)
{
      this->SetPuntoZ(this->GetPuntoZ()+gRandom->Gaus(0,RMSZeta));
      this->SetPuntoPhi(this->GetPuntoPhi()+gRandom->Gaus(0,RMSRPhi/DRadius));
}


Hit Hit::NoiseOnCyl(Double_t CRadius,Double_t ZetaMin,Double_t ZetaMax)
{
   Hit Noise;
   Noise.SetPuntoCRadius(CRadius);
   Noise.SetPuntoZ(ZetaMin+(ZetaMax-ZetaMin)*gRandom->Rndm());
   Noise.SetPuntoPhi(2*TMath::Pi()*gRandom->Rndm());
   Noise.SetHitID(-1);

   return Noise;
}

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
							dmNumLayer(0.);
							dmID(0.),
							dmUrtoReale(kFALSE)
{}

Urto::Urto(double x, double y, double z, double NumLayer, int ID): Punto(x, y, z),
																										    					 dmNumLayer(NumLayer);
																																	 dmID(ID),
																																	 dmUrtoReale(kFALSE)
{}

// Distruttori
Urto::~Urto()
{}

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

void Hit::NowRealHit()
{
	dmUrtoReale = kTRUE;
}

void Hit::SetIDUrto(in ID)
{
	dmID = ID;
}

void Hit::SetNumLayerUrto(int NumLayer)
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

int GetNumeroLayer()
{
	return dmNumLayer;
}
 
bool IsReal()
{
	return dmID;
}

int GetIDUrto()
{
	return dmUrtoReale;
}

double Hit::ComputeT(double Theta, double Phi, double XO, double YO, double Raggio)
{
	const double sintheta = TMath::Sin(Theta);
  const double sinphi = TMath::Sin(Phi);
  const double cosphi = TMath::Cos(Phi);
  const double t = (-(XO * cosphi + YO * sinphi) + TMath::Sqrt((XO * cosphi + YO * sinphi) * (XO * cosphi + YO * sinphi) - XO * XO - YO * YO + Raggio * Raggio))/sintheta;
  return t;
}

Hit Hit::UrtoSuVertice(Vertice& Origine, Direzione& Direttrice, double Raggio, int IDe, int NumLayer)
{
			// Prende ϑ e phi da "Direzione".
      const double theta = Direttrice.GetDirTheta();
      const Double_t phi = Direttrice.GetDirPhi();

      // Eccezione ϑ = 0
      if( theta == 0.f || theta == TMath::Pi() ) {
         Hit SuRivelatore;
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
      const Double_t t=ComputeT(theta,phi,xO,yO,Radius);

      // Build item in return.
      Hit OnCyl=Hit(xO+t*Direct.GetDirCos1(),yO+t*Direct.GetDirCos2(),
         zO+t*Direct.GetDirCos3(),Layerno,Ide);

      return OnCyl;
   }
}
