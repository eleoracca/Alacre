#ifndef MYRANDOM_H
#define MYRANDOM_H

#include "TObject.h"
#include "TRandom3.h"

Class MyRandom : public TRandom3{
  //
 public:

  MyRandom(); //! costruttore vuoto
  MyRandom(Double_t parametroa); //! costruttore standard
  MyRandom( const MyRandom& source); //! copyconstructor
  virtual ~MyRandom(); //! distruttore

  MyRandom& operator=(const MyRandom& source); //! operatore =

  SetA(Double_t parametroa);
  Double_t GetA() const;

  Double_t Funzione1(double teta); //! funzione 1/tuttoquellocheera
  Double_t Funzione2(double teta); //! funzione (sen(x)/x)^2

  Double_t F1Reiezione1();
  Double_t F1Reiezione2();
  Double_t F1Inversione();
  Double_t F2Sampling(double xmin, double xmax, double x1, double x2, double ymax1, double ymax2);


 private:
  void Inizializzazione();
  
  Double_t fMaggiorante;
  Double_t fParametroa;
  Double_t fPi;
  Double_t fRadiceParametro;
  
  ClassDef(MyRandom,1)
};

#endif
