/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~ Dichiarazione della classe Rivelatore                   ~
  ~ Autori: Racca Eleonora - eleonora.racca288@edu.unito.it ~
  ~         Sauda Cristina - cristina.sauda@edu.unito.it    ~
  ~ Ultima modifica: 23/08/2018                             ~
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#include "TMath.h"

// ------------- Costruttori --------------
Rivelatore::Rivelatore(): TObject(),
			  dmRaggioBP(0.),
			  dmRaggio1L(0.),
			  dmRaggio2L(0.),
			  dmSpessoreBP(0.),
			  dmSpessoreL(0.),
			  dmLunghezza(0.),
			  dmEtaMinimo(0.),
			  dmEtaMassimo(0.),
			  dmSmearingZ(0.),
			  dmSmearingRPhi(0.),
			  dmTaglioPhi(0.),
			  dmTheta(0.),
			  dmMaterialeBP(0.)
{}

Rivelatore::Rivelatore(TString Configurazione): TObject(),
						dmRaggioBP(3.),
						dmRaggio1L(4.),
						dmRaggio2L(7.),
						dmSpessoreBP(0.08),
						dmSpessoreL(0.02),
						dmLunghezza(27.),
						dmEtaMinimo(-1.),
						dmEtaMassimo(1.),
						dmTaglioPhi(0.01),
						dmTheta(TMath::Sqrt(2.)*0.001)
{
  TString commento;
  double SpostamentoSuZ;
  double SpostamentoSuRPhi;
  
  ifstream in(Configurazione);
  if(!in){
    cout << "!! File di configurazione del rivelatore on trovato !!\nLa simulazione terminerà immediatamente." << endl;
  }
  in >> commento >> SpostamentoSuZ >> SpostamentoSuRPhi;
  in.close();

  dmSmearingZ = SpostamentoSuZ;
  dmSmearingRPhi = SpostamentoSuRPhi;
  dmMaterialeBP = TMaterial("dmMaterialeBP", "Be", 8, 4, 1.85, 35.28, 0.08);
}

// ------------- Distruttori --------------
Rivelatore::~Rivelatore()
{}

// ---------------- Setter ----------------
void Rivelatore::SetRBP(const double RaggioBP)
{
  dmRaggioBP = RaggioBP;
}

void Rivelatore::SetR1L(const double Raggio1L)
{
  dmRaggio1L = Raggio1L;
}

void Rivelatore::SetR2L(const double Raggio2L)
{
  dmRaggio2L = Raggio2L;
}

void Rivelatore::SetSBP(const double SpessoreBP)
{
  dmSpessoreBP = SpessoreBP;
}

void Rivelatore::SetRBP(const double Spessore1L)
{
  dmSpessoreL = Spessore1L;
}

void Rivelatore::SetLunghezza(const double Lunghezza)
{
  dmLunghezza = Lunghezza;
}

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

void Rivelatore::SetMaterialeBP(const TMaterial Materiale)
{
  dmMaterialeBP = Materiale;
}

// ---------------- Getter ----------------
double Rivelatore::GetRBP()
{
  return dmRaggioBP;
}

double Rivelatore::GetR1L()
{
  return dmRaggio1L;
}

double Rivelatore::GetR2L()
{
  return dmRaggio2L;
}

double Rivelatore::GetSBP()
{
  return dmSpessoreBP;
}

double Rivelatore::GetS1L()
{
  return dmSpessoreL;
}

double Rivelatore::GetLunghezza()
{
  return dmLunghezza;
}

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

// ----------- Member functions -----------
