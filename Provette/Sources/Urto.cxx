/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~ Implementazione della classe Urto                       ~
  ~ Autori: Racca Eleonora - eleonora.racca288@edu.unito.it ~
  ~         Sauda Cristina - cristina.sauda@edu.unito.it    ~
  ~ Ultima modifica: 22/09/2018                             ~
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#if !defined (__CINT__) || defined (__MAKECINT__)
#include "Riostream.h"
#include "TMath.h"
#include "TRandom3.h"
#include "TSystem.h"
#include "Punto.h"
#include "Trasporto.h"
#include "Urto.h"
#include "Vertice.h"
#include "Rivelatore.h"
#endif

using namespace std;

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

/*
Urto::Urto(const Urto& urto) : Punto(urto.dmX, urto.dmY, urto.dmZ){
  dmNumLayer = urto.dmNumLayer;
  dmID = urto.dmID;
  dmUrtoReale = urto.dmUrtoReale;
}
*/

// ------------- Distruttori --------------
Urto::~Urto()
{}

// ---------------- Setter ----------------
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
int Urto::GetNumeroLayer(){
  return dmNumLayer;
}

bool Urto::TestReale(){
  return dmUrtoReale;
}

int Urto::GetID(){
  return dmID;
}

// ----------- Member functions -----------
double Urto::CalcoloT(double Theta, double Phi, double XO, double YO, double Raggio){
  const double sentheta = TMath::Sin(Theta);
  const double senphi = TMath::Sin(Phi);
  const double cosphi = TMath::Cos(Phi);
  const double t = (-(XO * cosphi + YO * senphi) + TMath::Sqrt((XO * cosphi + YO * senphi) * (XO * cosphi + YO * senphi) - XO * XO - YO * YO + Raggio * Raggio))/sentheta;
  
  return t;
}

Urto Urto::UrtodaVertice(Vertice *Origine, Trasporto *Direttrice, double Raggio, int IDe, int NumLayer){
  // Prende #theta e #phi da "Trasporto".
  const double theta = Direttrice->GetDirTheta();
  const double phi = Direttrice->GetDirPhi();
  
  // Eccezione #theta = 0
  if(theta == 0.f || theta == TMath::Pi()){
    Urto SuAsse;
    
    return SuAsse;
  }
  else{
    // Prende le coordinate del vertice da "Vertice"
    const double xO = Origine->GetX();
    const double yO = Origine->GetY();
    const double zO = Origine->GetZ();
    
    const double t = CalcoloT(theta, phi, xO, yO, Raggio);
    
    Urto SuRivelatore = Urto(xO + t * Direttrice->GetDirCosX(), yO + t * Direttrice->GetDirCosY(), zO + t * Direttrice->GetDirCosZ(), NumLayer, IDe);
    
    return SuRivelatore;
  }
}
  
Urto Urto::UrtodaUrto(Trasporto *Direttrice, Rivelatore *Detector, bool ScatteringMult, int NumLayer){

  // Scelta del raggio in base al layer  
  double Raggio;
  if(NumLayer == 1){
    Raggio = Detector->GetRaggio1L();
  }
  else{
    Raggio = Detector->GetRaggio2L();
  }
    
  if(ScatteringMult){
    
    // #theta0 � la rms di una gaussiana con centro 0
    // Formula per il multiple scattering: #deltatheta0 = (13.6 MeV/#beta*c*p)*Z*(#sqrt(x/X_0))[1+0.038*Ln(x/X_0)]
    // Z = 1 perchè sono pioni, #beta*c*p = dmEnergia del Detector

    double X0;
    double Larghezza;
    
    if(NumLayer == 1){ // perche' fa ScatteringMultiplo dopo la BeamPipe
      // Lunghezza di radiazione, X0, e larghezza del materiale, x
      X0 = Detector->GetMaterialeBP().GetRadLength();
      Larghezza = Detector->GetSpessoreBP();
    }
    else{
      X0 = Detector->GetMaterialeL().GetRadLength();
      Larghezza = Detector->GetSpessoreL();
    }

    // #theta0 (in realtà è devizione da #theta della particella); per le dimensioni giuste, c non compare nel conto 
    const double ThetaZero = (13.6/(Detector->GetEnergia())) * TMath::Sqrt(Larghezza/X0) * (1 + 0.038 * TMath::Log(Larghezza/X0));
    
    // #theta viene generato da una distribuzione gaussiana di media 0 e rms #theta0
    const double thetalocale = gRandom -> Gaus(0., ThetaZero);
    
    // #phi � distribuito tra 0 e 2#pi uniformemente
    const double philocale = gRandom -> Uniform(0., 2*TMath::Pi());
    
    // Rotazione delle coordinate per tornare al laboratorio
    Direttrice->Rotazione(thetalocale, philocale);
  }
  
  // Si estraggono theta e phi da "Direttrice"
  const double theta = Direttrice->GetDirTheta();
  const double phi = Direttrice->GetDirPhi();
  
  // Eccezione #theta=0
  if(theta == 0.f || TMath::Abs(theta) == TMath::Pi()){
    Urto SuAsse;
    
    return SuAsse;
  }
  else{
    
    // Calcolo di t
    const double t = CalcoloT(theta, phi, dmX, dmY, Raggio);
    
    // Ritorna l'urto
    Urto SuRivelatore = Urto(dmX + t * Direttrice->GetDirCosX(), dmY + t * Direttrice->GetDirCosY(), dmZ + t * Direttrice->GetDirCosZ(), NumLayer, dmID);
    
    return SuRivelatore;
  }
}

void Urto::SmearingGaussiano(Rivelatore *Detector, int NumLayer){
  this -> SetZ(this -> GetZ() + gRandom -> Gaus(0., Detector->GetSmearZ()));
  
  // Si pu� trovare la rms di #theta dividendo RPhi per il raggio
  double Raggio;
  if(NumLayer == 0){
    Raggio = Detector->GetRaggioBP();
  }
  else if(NumLayer == 1){
    Raggio = Detector->GetRaggio1L();
  }
  else if(NumLayer == 2){
    Raggio = Detector->GetRaggio2L();
  }
  else{
    std::cout << "Problema con il raggio del detector." << endl;
  }
  this -> SetPhi(this -> GetPhi() + gRandom -> Gaus(0., Detector->GetSmearRPhi()/Raggio));
}

Urto Urto::RumoreRivelatore(Rivelatore *Detector, int NumLayer){
  double Raggio;
  
  if(NumLayer == 0){
    Raggio = Detector->GetRaggioBP();
  }
  else if(NumLayer == 1){
    Raggio = Detector->GetRaggio1L();
  }
  else if(NumLayer == 2){
    Raggio = Detector->GetRaggio2L();
  }
  else{
    std::cout << "Problema con il raggio del detector." << endl;
  }
  
  Urto Rumore;
  Rumore.SetRaggioC(Raggio);
  Rumore.SetZ(gRandom -> Uniform(-Detector->GetLunghezza()/2., Detector->GetLunghezza()/2.));
  Rumore.SetPhi(gRandom -> Uniform(2 * TMath::Pi()));
  Rumore.SetID(-2);
  
  return Rumore;
}
