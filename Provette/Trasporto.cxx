/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~ Dichiarazione della classe Trasporto                        ~
  ~ Autori: Racca Eleonora - eleonora.racca288@edu.unito.it ~
  ~         Sauda Cristina - cristina.sauda@edu.unito.it    ~
  ~ Ultima modifica: 13/06/2018                             ~
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#if !defined (__CINT__) || defined (__MAKECINT__)
#include "Trasporto.h"
#include <TMath.h>
#include <TObject.h>
#include <TString.h>
#endif

ClassImp(Trasporto)

// ----------- Costruttori -----------
Trasporto::Trasporto():
   TObject(),
   dmID(0),
   dmTheta(0.),
   dmPhi(0.),
   dmCDx(0.),
   dmCDy(0.),
   dmCDz(0.),
   dmIsrotated(kFALSE) { };

Trasporto::Trasporto(double Theta, double Phi, int ID):
      TObject(),
      dmID(ID),
      dmTheta(Theta),
      dmPhi(Phi),
      dmIsrotated(kFALSE)
{
         dmCDx = TMath::Sin(Theta) * TMath::Cos(Phi);
         dmCDy = TMath::Sin(Theta) * TMath::Sin(Phi);
         dmCDz = TMath::Cos(Theta);
};

// ----------- Distruttori -----------
Trasporto::~Trasporto()
{}

// ----------- Setter -----------
void Trasporto::SetDirTheta(const double Theta)
{
   dmTheta = Theta;
}

void Trasporto::SetDirPhi(const double Phi)
{
   dmPhi = Phi;
}

void Trasporto::SetCosDirx(const double Cx)
{
   dmCDx = Cx;
}

void Tasporto::SetCosDiry(const double Cy)
{
   dmCDy = Cy;
}

void Trasporto::SetCosDirz(const double Cz)
{
   dmCDz = Cz;
}

void Trasporto::SetAllCos(const double Cx, const double Cy, const double Cz)
{
   dmCDx = Cx;
   dmCDy = Cy;
   dmCDz = Cz;
   UpdateAng();
}

void Trasporto::SetAllAngles(const double Theta, const double Phi)
{
   dmTheta = Theta;
   dmPhi = Phi;
   UpdateDirCos();
}

void Trasporto::SetDirID(const double id)
{
   dmID = id;
}

// ----------- Getter -----------

int Trasporto::GetDirectID()
{
  return dmID;
}

double Trasporto::GetDirectTheta()
{
  return dmTheta;
}

double Trasporto::GetDirectPhi()
{
  return dmPhi;
}

double Trasporto::GetDirCos1()
{
  return dmCDx;
}

double Trasporto::GetDirCos2()
{
  return dmCDy;
}

double Trasporto::GetDirCos3()
{
  return dmCDz;
}

bool Trasporto::GetRotStatus()
{
  return dmIsrotated;
}

void Trasporto::UpdateAng()
{

   ///////////////////////////////////////////////////////////////////
   // Since D[ATan(x)] = [-Pi,+Pi] -> One have to discuss the cosines
   // directors values in order to find the true value of Phi.
   ///////////////////////////////////////////////////////////////////
   
   dmTheta = TMath::ACos(dmCDz);
   if(dmCDx > 0) {
      if(dmCDy >= 0) {
        dmPhi = TMath::ATan(dmCDy/dmCDx);
      }
      else {
        dmPhi = 2*TMath::Pi() + TMath::ATan(dmCDy/dmCDx);
      }
   } 
   else {
      if(dmCDx != 0) { // CDx < 0
        if(dmCDy >= 0) {
         	dmPhi = TMath::Pi() + TMath::ATan(dmCDy/dmCDx);
        }
        else {
         	dmPhi = TMath::Pi() + TMath::ATan(dmCDy/dmCDx);
        }
      }
			else { // CDx = 0
        if(dmCDx >= 0) {
					dmPhi = 0.5 * TMath::Pi();
        }
				else {
					dmPhi = 1.5 * TMath::Pi();
      	}
			}
   }
}

void Trasporto::UpdateDirCos()
{
   dmCDx = TMath::Sin(dmTheta) * TMath::Cos(dmPhi);
   dmCDy = TMath::Sin(dmTheta) * TMath::Sin(dmPhi);
   dmCDz = TMath::Cos(dmTheta);
}

void Trasporto::FlipBit()
{
	if(dmIsrotated) {
		dmIsrotated = kFALSE;
	}
  else {
		dmIsrotated = kTRUE;
	}
}

void Trasporto::Rotate(double Theta, double Phi)
{
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
	for(int i = 0; i < 3; i++) {
		Cd[i] = 0;
    for(int j  =0; j < 3; j++){
			Cd[i] = Cd[i] + mr[i][j] * cdp[j];
		}
  }
  
	dmCDx = Cd[0];
  dmCDy = Cd[1];
  dmCDz = Cd[2];
  this->FlipBit();

// Update Theta and Phi
  this->UpdateAng();
}
