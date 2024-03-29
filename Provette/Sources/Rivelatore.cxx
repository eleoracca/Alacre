/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~ Dichiarazione della classe Rivelatore                   ~
  ~ Autori: Racca Eleonora - eleonora.racca288@edu.unito.it ~
  ~         Sauda Cristina - cristina.sauda@edu.unito.it    ~
  ~ Ultima modifica: 22/09/2018                             ~
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#if !defined (__CINT__) || defined (__MAKECINT__)
#include "Riostream.h"
#include "Rivelatore.h"
#include "TMath.h"
#include "TString.h"
#endif

using namespace std;

ClassImp(Rivelatore)

// ------------- Costruttori --------------
Rivelatore::Rivelatore(): TObject(){
  dmRaggioBP = 0.;
  dmRaggio1L = 0.;
  dmRaggio2L = 0.;
  dmSpessoreBP = 0.;
  dmSpessoreL = 0.;
  dmLunghezza = 0.;
  dmEnergia = 0.;
  dmMaterialeBP = TMaterial();
  dmMaterialeL = TMaterial();
  
  dmEtaMinimo = 0.;
  dmEtaMassimo = 0.;
  dmSmearingZ = 0.;
  dmSmearingRPhi = 0.;
  dmPhiLimite = 0.;
  
  dmXVertice = 0.;
  dmYVertice = 0.;
  dmZVertice = 0.;
  dmXVerticeSigma = 0.;
  dmXVerticeSigma = 0.;
  dmXVerticeSigma = 0.;
}

Rivelatore::Rivelatore(TString Configurazione): TObject(){
  dmRaggioBP = 3.;
  dmRaggio1L = 4.;
  dmRaggio2L = 7.;
  dmSpessoreBP = 0.08;
  dmSpessoreL = 0.02;
  dmLunghezza = 27.;
  dmMaterialeBP = TMaterial("dmMaterialeBP", "Be", 8, 4, 1.85, 35.28, 0.8);
  dmMaterialeL = TMaterial("dmMaterialeL", "Si", 28, 14, 2.33, 9.37, 0.2);
  
  dmEtaMinimo = -1.;
  dmEtaMassimo = 1.;
  dmPhiLimite = -100;

  TString commento;
  
  ifstream in(Configurazione);
  if(!in){
    cout << "!! File di configurazione del rivelatore on trovato !!\nLa simulazione termina ora." << endl;
    return;
  }
  
  in >> commento >> dmSmearingZ >> dmSmearingRPhi >> dmXVertice >> dmXVerticeSigma >> dmYVertice >> dmYVerticeSigma >> dmZVertice >> dmZVerticeSigma >> dmEnergia >> dmPhiLimite;
  in.close();
}

// ------------- Distruttori --------------
Rivelatore::~Rivelatore()
{}

// ---------------- Setter ----------------
void Rivelatore::SetEnergia(const double Energia){
  dmEnergia = Energia;
}

void Rivelatore::SetEtaMin(const double Etamin){
  dmEtaMinimo = Etamin;
}

void Rivelatore::SetEtaMax(const double Etamax){
  dmEtaMassimo = Etamax;
}

void Rivelatore::SetSmearZ(const double SmearZ){
  dmSmearingZ = SmearZ;
}

void Rivelatore::SetSmearRPhi(const double SmearRPhi){
  dmSmearingRPhi = SmearRPhi;
}

// ---------------- Getter ----------------
double Rivelatore::GetRaggioBP() const{
  return dmRaggioBP;
}

double Rivelatore::GetRaggio1L() const{
  return dmRaggio1L;
}

double Rivelatore::GetRaggio2L() const{
  return dmRaggio2L;
}

double Rivelatore::GetSpessoreBP() const{
  return dmSpessoreBP;
}

double Rivelatore::GetSpessoreL() const{
  return dmSpessoreL;
}

double Rivelatore::GetLunghezza() const{
  return dmLunghezza;
}

double Rivelatore::GetEnergia() const{
  return dmEnergia;
}

TMaterial Rivelatore::GetMaterialeBP() const{
  return dmMaterialeBP;
}

TMaterial Rivelatore::GetMaterialeL() const{
  return dmMaterialeL;
}

double Rivelatore::GetEtaMin() const{
  return dmEtaMinimo;
}

double Rivelatore::GetEtaMax() const{
  return dmEtaMassimo;
}

double Rivelatore::GetSmearZ() const{
  return dmSmearingZ;
}

double Rivelatore::GetSmearRPhi() const{
  return dmSmearingRPhi;
}

double Rivelatore::GetPhiLimite() const{
  return dmPhiLimite;
}

double Rivelatore::GetVerticeX() const{
  return dmXVertice;
}

double Rivelatore::GetVerticeY() const{
  return dmYVertice;
}

double Rivelatore::GetVerticeZ() const{
  return dmZVertice;
}

double Rivelatore::GetVerticeSX() const{
  return dmXVerticeSigma;
}

double Rivelatore::GetVerticeSY() const{
  return dmYVerticeSigma;
}

double Rivelatore::GetVerticeSZ() const{
  return dmZVerticeSigma;
}


// ----------- Member functions -----------
