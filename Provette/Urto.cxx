/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~ Implementazione della classe Urto                       ~
  ~ Autori: Racca Eleonora - eleonora.racca288@edu.unito.it ~
  ~         Sauda Cristina - cristina.sauda@edu.unito.it    ~
  ~ Ultima modifica: 28/08/2018                             ~
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#if !defined (__CINT__) || defined (__MAKECINT__)
#include "TMath.h"
#include "TRandom3.h"
#include "TSystem.h"
#include "Punto.h"
#include "Trasporto.h"
#include "Urto.h"
#include "Vertice.h"
#endif

ClassImp(Urto)

// ------------- Costruttori --------------
Urto::Urto(): Punto(0., 0., 0.){
  dmNumLayer = 0.;
  dmID = 0.;
  dmUrtoReale = kFALSE;
}

Urto::Urto(double x, double y, double z, int NumLayer, int ID): Punto(x, y, z){
  dmNumLayer = NumLayer;
  dmID = ID;
  dmUrtoReale = kFALSE;
}

// ------------- Distruttori --------------
Urto::~Urto()
{}

// ---------------- Setter ----------------
/*
void Urto::SetX(double x){
  dmX = x;
}

void Urto::SetY(double y){
  dmY = y;
}

void Urto::SetZ(double z){
  dmZ = z;
}
*/
void Urto::SetUrtoReale(){
  dmUrtoReale = kTRUE;
}

void Urto::SetID(int ID){
  dmID = ID;
}

void Urto::SetLayer(int NumLayer){
  dmNumLayer = NumLayer;
}

// ---------------- Getter ----------------
/*
double Urto::GetX(){
  return dmX;
}

double Urto::GetY(){
  return dmY;
}

double Urto::GetZ(){
  return dmZ;
}
*/
int Urto::GetNumeroLayer(){
  return dmNumLayer;
}

bool Urto::TestReale(){
  return dmID;
}

int Urto::GetID(){
  return dmUrtoReale;
}

// ----------- Member functions -----------
double Urto::CalcoloT(double Theta, double Phi, double XO, double YO, double Raggio){
  const double sintheta = TMath::Sin(Theta);
  const double sinphi = TMath::Sin(Phi);
  const double cosphi = TMath::Cos(Phi);
  const double t = (-(XO * cosphi + YO * sinphi) + TMath::Sqrt((XO * cosphi + YO * sinphi) * (XO * cosphi + YO * sinphi) - XO * XO - YO * YO + Raggio * Raggio))/sintheta;
  
  return t;
}

Urto Urto::UrtoSuVertice(Vertice& Origine, Trasporto& Direttrice, double Raggio, int IDe, int NumLayer){
  // Prende #theta e #phi da "Trasporto".
  const double theta = Direttrice.GetDirTheta();
  const double phi = Direttrice.GetDirPhi();
  
  // Eccezione #theta = 0
  if(theta == 0.f || theta == TMath::Pi()){
    Urto SuRivelatore;
    
    return SuRivelatore;
  }
  else {
    // Prende le coordinate del vetice da "Vertice"
    const double xO = Origine.GetX();
    const double yO = Origine.GetY();
    const double zO = Origine.GetZ();
    
    const double t = CalcoloT(theta, phi, xO, yO, Raggio);
    
    Urto SuRivelatore = Urto(xO + t * Direttrice.GetDirCosX(), yO + t * Direttrice.GetDirCosY(), zO + t * Direttrice.GetDirCosZ(), NumLayer, IDe);
    
    return SuRivelatore;
  }
}
  
Urto Urto::UrtodaUrto(Trasporto &Direttrice, double Raggio,TMaterial &Materiale, double Larghezza, bool ScatteringMult, int NumLayer, double P, int Z, double Beta){
  if(ScatteringMult){
    
    // #theta0 è la rms di una gaussiana con centro 0
    // Formula per il multiple scattering: #theta0 = (13.6 MeV/#beta*c*p)*Z*(#sqrt(x/X_0))[1+0.038*Ln(x/X_0)]
    
    // Lunghezza di radiazione, X0
    const double X0 = Materiale.GetRadLength();

    // #theta0; per le dimensioni giuste, c non compare nel conto 
    const double ThetaZero = (13.6/(Beta * P)) * Z * TMath::Sqrt(Larghezza/X0) * (1 + 0.038 * TMath::Log(Larghezza/X0));
    
    // #theta viene generato da una distribuzione gaussiana di media 0 e rms #theta0
    const double thetalocale = gRandom -> Gaus(0., ThetaZero);
    
    // #phi è distribuito tra 0 e 2#pi uniformemente
    const double philocale = gRandom -> Uniform(0., 2*TMath::Pi());
    
    // Rotazione delle coordinate per tornare al laboratorio
    Direttrice.Trasporto::Rotazione(thetalocale, philocale);
  }
  
  // Extract theta and phi from "Direzione".
  const double theta = Direttrice.GetDirTheta();
  const double phi = Direttrice.GetDirPhi();
  
  // Eccezione #theta=0
  if(theta == 0.f || TMath::Abs(theta) == TMath::Pi()){
    Urto SuRivelatore;
    
    return SuRivelatore;
  }
  else{
    
    // Calcolo di t
    const double t = CalcoloT(theta, phi, dmX, dmY, Raggio);
    
    // Ritorna l'urto
    Urto SuRivelatore = Urto(dmX + t * Direttrice.GetDirCosX(), dmY + t * Direttrice.GetDirCosY(), dmZ + t * Direttrice.GetDirCosZ(), NumLayer, dmID);
    
    return SuRivelatore;
  }
}

void Urto::SmearingGaussiano(double RaggioDetector, double ZDeviazioneStandard, double RPhiDeviazioneStandard){
  this -> SetZ(this -> GetZ() + gRandom -> Gaus(0., ZDeviazioneStandard));
  
  // Si può trovare la rms di #theta dividendo RPhi per il raggio
  this -> SetPhi(this -> GetPhi() + gRandom -> Gaus(0., RPhiDeviazioneStandard/RaggioDetector));
}


Urto Urto::RumoreRivelatore(double RaggioC, double Zmin, double Zmax){
  Urto Rumore;
  Rumore.SetRaggioC(RaggioC);
  Rumore.SetZ(gRandom -> Uniform(Zmin, Zmax));
  Rumore.SetPhi(gRandom -> Uniform(2 * TMath::Pi()));
  Rumore.SetID(-1);
  
  return Rumore;
}
