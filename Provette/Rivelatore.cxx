/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~ Dichiarazione della classe Rivelatore                   ~
  ~ Autori: Racca Eleonora - eleonora.racca288@edu.unito.it ~
  ~         Sauda Cristina - cristina.sauda@edu.unito.it    ~
  ~ Ultima modifica: 24/08/2018                             ~
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#include "TMath.h"

// ------------- Costruttori --------------
Rivelatore::Rivelatore(): TObject(){
  dmRaggioBP = 0.;
  dmRaggio1L = 0.;
  dmRaggio2L = 0.;
  dmSpessoreBP = 0.;
  dmSpessoreL = 0.;
  dmLunghezza = 0.;
  dmEtaMinimo = 0.;
  dmEtaMassimo = 0.;
  dmSmearingZ = 0.;
  dmSmearingRPhi = 0.;
  dmTaglioPhi = 0.;
  dmTheta = 0.;
  dmMaterialeBP = 0.;
  
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
  dmEtaMinimo = -2.;
  dmEtaMassimo = 2.;
  dmTaglioPhi = 0.01;
  dmTheta = TMath::Sqrt(2.)*0.001;
  dmMaterialeBP = TMaterial("dmMaterialeBP", "Be", 8, 4, 1.85, 35.28, 0.08);

  TString commento;
  double *SpostamentoSuZ;
  double *SpostamentoSuRPhi;
  double *VerticeX;
  double *VerticeY;
  double *VerticeZ;
  double *SVerticeX;
  double *SVerticeX;
  double *SVerticeX;
  
  ifstream in(Configurazione);
  if(!in){
    cout << "!! File di configurazione del rivelatore on trovato !!\nLa simulazione terminerà immediatamente." << endl;
  }
  in >> commento >> SpostamentoSuZ >> SpostamentoSuRPhi >> VerticeX >> SVerticeX >> VerticeY >> SVerticeY >> VerticeZ >> SVerticeZ;
  in.close();

  dmSmearingZ = &SpostamentoSuZ;
  dmSmearingRPhi = &SpostamentoSuRPhi;
  dmXVertice = &VerticeX;
  dmYVertice = &VerticeY;
  dmZVertice = &VerticeZ;
  dmXVerticeSigma = &SVerticeX;
  dmYVerticeSigma = &SVerticeX;
  dmZVerticeSigma = &SVerticeX;
  
  delete SpostamentoSuZ;
  delete *SpostamentoSuRPhi;
  delete *VerticeX;
  delete *VerticeY;
  delete *VerticeZ;
  delete *SVerticeX;
  delete *SVerticeX;
  delete *SVerticeX;  
}

// ------------- Distruttori --------------
Rivelatore::~Rivelatore()
{}

// ---------------- Setter ----------------
void Rivelatore::SetEtaMin(const double Etamin)
{
  dmEtaMinimo = Etamin;
}

void Rivelatore::SetEtaMax(const double Etamax)
{
  dmEtaMassimo = Etamax;
}

void Rivelatore::SetSmearZ(const double SmearZ)
{
  dmSmearingZ = SmearZ;
}

void Rivelatore::SetSmearRPhi(const double SmearRPhi)
{
  dmSmearingRPhi = SmearRPhi;
}

// ---------------- Getter ----------------
double Rivelatore::GetEtaMin()
{
  return dmEtaMinimo;
}

double Rivelatore::GetEtaMax()
{
  return dmEtaMassimo;
}

double Rivelatore::GetSmearZ()
{
  return dmSmearingZ;
}

double Rivelatore::GetSmearRPhi()
{
  return dmSmearingRPhi;
}

double GetVerticeX()
{
  return dmXVertice;
}

double GetVerticeY()
{
  return dmYVertice;
}

double GetVerticeZ()
{
  return dmZVertice;
}

double GetVerticeSX()
{
  return dmXVerticeSigma;
}

double GetVerticeSY()
{
  return dmYVerticeSigma;
}

double GetVerticeSZ()
{
  return dmZVerticeSigma;
}


// ----------- Member functions -----------
