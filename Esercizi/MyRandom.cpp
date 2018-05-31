#include <Riostream.h>
#include "MyRandom.h"
#include "TString.h"
#include "TMath.h"

ClassImp(MyRandom)

MyRandom::MyRandom():
  TRandom(),
  fMaggiorante(0.),
  fParametroa(0.),
  fPi(0.),
  fRadiceParametro(0.)
{}

MyRandom::MyRandom(Double_t parametroa):
  TRandom(),
  fMaggiorante(0.),
  fParametroa(parametroa),
  fPi(0.),
  fRadiceParametro(0.)
{
  SetName("MyRandom");
  SetTitle("Random number generator: Mersenne Twister");

  Inizializzazione();
}

MyRandom::Inizializzazione():
{
  fPi = TMath::Pi();
  if(fParametroa <= 0. || fParametroa > 1.){
    Warning("Inizializzazione","Parametro a non valido = %f8.3 \n",fParametroa);
    Warning("Init","Bisognerebbe inizializzarlo appropriatamente\n");
    fParametroa = 0.;
    fMaggiorante = 0.;
    fRadiceParametro = 0.;
  }
  else {
    fMaggiorante = 1/fParametroa;
    fRadiceParametro = TMath::Sqrt(fParametroa);
  }
}
  
MyRandom::MyRandom(const MyRandom &source):
  TRandom(source),
  fMaggiorante(source.fMaggiorante),
  fParametroa(source.fParametroa),
  fPi(source.fPi),
  fRadiceParametro(source.fRadiceParametro)
{}

MyRandom::~MyRandom()
{}

MyRandom& MyRandom::operator=(const MyRandom& source){
  if(this != &source){
    TRandom::operator=(source);
    fMaggiorante = source.fMaggiorante;
    fParametroa = source.fParametroa;
    fPi = source.fPi;
    fRadiceParametro = source.fRadiceParametro;
  }

  return *this;
}

MyRandom::SetA(Double_t parametroa){
  fParametroa = parametroa;
}

Double_t MyRandom::GetA(){
  return fParametroa;
}

Double_t Funzione1(double teta)
{

  double senquadro = TMath::Sin(teta) * TMath::Sin(teta);
  double cosquadro = TMath::Cos(teta) * TMath::Cos(teta);

  return 1/(senquadro + fParametroa*cosquadro);
}

Double_t Funzione2(double teta)
{
  double senquadro = TMath::Sin(teta) * TMath::Sin(teta);
  double tetaquadro = teta*teta;

  return senquadro/tetaquadro;
}

Double_t F1Reiezione1()
{
  double x, y;

  do{
    x = 2 * fPi * Rndm();
    y = Rndm() * fMaggiorante;
  }while(Funzione1(x) >= y);

  return x;
}

Double_t F1Reiezione2()
{
  double x = 2. * Rndm() * fPi;
  double y = Rndm() * fMaggiorante;
  
  if(Funct1(x) >= y)
    return x;
  else
    return F1Reiezione2();
}

Double_t F1Inversione()
{
  double u = Rndm();
  double teta = TMath::ATan(fRadiceParametro * TMath::Tan(fPi * u - fPi/2));

  double w = Rndm();
  if(w < 0.5){
    teta = teta + fPi;
  }
  else if(teta < 0){
    teta = teta + 2*fPi;
  }

  return teta;
}

Double_t F2Sampling(double xmin, double xmax, double x1, double x2, double ymax1, double ymax2)
{
  //xmin = -10, xmax = 10, x1 = -2.6, x2 = 2.6, ymax1 = 0.05, ymax2 = 1
  double u = Rndm();
  
  double norm1 = ymax1 * (x1 - xmin);
  double norm2 = ymax2 * (x2 - x1);
  double norm3 = ymax1 * (xmax - x2);
  double normtot = norm1 + norm2 + norm3;

  if(u < norm1/normtot){ //questo dovrebbe andare perché è il primo intervallo a sinistra, quindi u sicuramente parte da 0 e arriva fino al primo pezzo della normalizzazione totale
    x = u/(ymax1 * normtot) + xmin;
  }
}
