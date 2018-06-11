/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~ Implementazione della classe Punti                      ~
  ~ Autori: Racca Eleonora - eleonora.racca288@edu.unito.it ~
  ~         Sauda Cristina - cristina.sauda@edu.unito.it    ~
  ~ Ultima modifica: 15/11/2017                             ~
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#include "TMath.h"
#include "Punto.h"
#include "TString.h"

ClassImp(Punto)

// ----------- Costruttori -----------
Punto::Punto(): TObject(),
                dmX(0.),
                dmY(0.),
                dmZ(0.),
                dmTheta(0.),
                dmPhi(0.),
                dmRaggioC(0.),
                dmRaggioS(0.) 
{}

Punto::Punto(double X, double Y, double Z): TObject(),
                                            dmX(X),
                                            dmY(Y),
                                            dmZ(Z)
{
      CartesianeSferiche(); // Assegna i valori delle coordinate sferiche
      CartesianeCilindriche(); // Assegna i valori delle coordinate cilindriche
}

// ----------- Distruttori -----------
Punto::~Punto()
{}

// ----------- Setters -----------
void Punto::SetCartesiane(const double X, const double Y, const double Z)
{
      dmX = X;
      dmY = Y;
      dmZ = Z;

      CartesianeSferiche();
      CartesianeCilindriche();
}

void Punto::SetCilindriche(const double RaggioC, const double Phi, const double Z)
{
      dmZ = Z;
      dmPhi = Phi;
      dmRaggioC = RaggioC;

      CilindricheCartesiane();
      CartesianeSferiche();
}

void Punto::SetSferiche(const double RaggioS, const double Theta, const double Phi)
{
      dmTheta = Theta;
      dmPhi = Phi;
      dmRaggioS = RaggioS;

      SfericheCartesiane();
      CartesianeCilindriche();
}

void Punto::SetX(const double X)
{
   dmX = X;

   CartesianeSferiche();
   CartesianeCilindriche();
}

void Punto::SetY(const double Y)
{
   dmY = Y;

   CartesianeSferiche();
   CartesianeCilindriche();
}

void Punto::SetZ(const double Z)
{
   dmZ = Z;

   CartesianeSferiche();
   CartesianeCilindriche();
}

void Punto::SetTheta(const double Theta)
{
   dmTheta = Theta;
   
   SfericheCartesiane();
   CartesianeCilindriche();
}

void Punto::SetPhi(const double Phi)
{
   dmPhi = Phi;

   CilindricheCartesiane();
   CartesianeSferiche();
}

void Punto::SetRaggioC(const double RaggioC)
{
   dmRaggioC = RaggioC;

   CilindricheCartesiane();
   CartesianeSferiche();

   if (dmX == 0. && dmY == 0.){
      Printf("Attenzione: Coordinate X e Y nulle! \
         \n Potrebbero esserci problemi con le conversioni delle coordinate. \
         \n Controlla i valori inseriti e il codice!");
   }
}

void Punto::SetRaggioS(const double RaggioS)
{
   dmRaggioS = RaggioS;

   SfericheCartesiane();
   CartesianeCilindriche();

   if (dmX == 0. && dmY == 0. && dmZ == 0.){
      Printf("Attenzione: Coordinate X, Y e Z nulle! \
         \n Potrebbero esserci problemi con le conversioni delle coordinate. \
         \n Controlla i valori inseriti e il codice!");
   }
}


// ----------- Getters -----------
double Punto::GetDistanza(Punto &PuntoUno, Punto &PuntoDue)
{
   return TMath::Sqrt((PuntoUno.GetX() - PuntoDue.GetX()) * (PuntoUno.GetX() - PuntoDue.GetX()) + (PuntoUno.GetY() - PuntoDue.GetY()) * (PuntoUno.GetY() - PuntoDue.GetY()) + (PuntoUno.GetZ() - PuntoDue.GetZ()) * (PuntoUno.GetZ() - PuntoDue.GetZ()));
}

double Punto::GetDeltaPhi(Punto &PuntoUno, Punto &PuntoDue)
{
   return TMath::Abs(PuntoUno.GetPhi() - PuntoDue.GetPhi());
};


// ----------- Member functions -----------
void Punto::CartesianeCilindriche() {   
   if (dmX > 0.) {
      if (dmY > 0. || dmY == 0.){
	dmPhi = TMath::ATan(dmY/dmX); // per X > 0. e Y >= 0.
      }
      else {
         dmPhi = 2*TMath::Pi() + TMath::ATan(dmY/dmX); // per X > 0. e Y < 0.
      }
   } 
   else {
      if (dmX == 0.) {
         if (dmY > 0.){
           dmPhi = TMath::Pi()/2; // per X = 0. e Y > 0.
         }
         else {
            if (dmY == 0.){
              dmPhi = dmTheta = 0.; // per X=0. e Y=0.
            }
            else {
              dmPhi = TMath::Pi() + TMath::Pi()/2; // per X = 0. e Y < 0.
            }
         }
      } 
      else {
         dmPhi = TMath::Pi() + TMath::ATan(dmY/dmX); // per X <0. e ∀Y
      }
   }
}

void Punto::CartesianeSferiche(){
   if (dmRaggioS != 0.){
      
      dmTheta = TMath::ACos(dmZ/fSRadius);

      if (dmX > 0.){
         if (dmY > 0. || dmY == 0.){
           dmPhi = TMath::ATan(dmY/dmX); // per X > 0. e Y>=0.
         }
         else {
           dmPhi = 2*TMath::Pi() + TMath::ATan(dmY/dmX); // per X>0. e Y<0.
         }
      }
      else {
         if (dmX == 0.){
            if (dmY > 0.){
              dmPhi = TMath::Pi()/2; // per X = 0. e Y > 0.
            }
            else {
               if (dmY == 0.){
                 dmPhi = 0.;
                 dmTheta=0.; // per X = 0. e Y = 0.
               }
               else{
                 dmPhi = TMath::Pi() + TMath::Pi()/2; // per X = 0. e Y < 0.
               }
            }
         } 
         else {
            dmPhi = TMath::Pi() + TMath::ATan(dmY/dmX); // X < 0. e ∀Y
         }
      }
   } 
   else {
      dmPhi = 0.;
      dmTheta = 0.;   // valore di default per evitare errori
   }
}


void Punto::SphericaltoCartesian() {
   dmX=fSRadius*TMath::Sin(dmTheta)*TMath::Cos(dmPhi);
   dmY=fSRadius*TMath::Sin(dmTheta)*TMath::Sin(dmPhi);
   dmZ=fSRadius*TMath::Cos(dmTheta);
}

void Punto::CylindricaltoCartesian() {
   dmX=fCRadius*TMath::Cos(dmPhi);
   dmY=fCRadius*TMath::Sin(dmPhi);
}
