#ifndef URTO_H
#define URTO_H

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~ Dichiarazione della classe Urto                         ~
  ~ Autori: Racca Eleonora - eleonora.racca288@edu.unito.it ~
  ~         Sauda Cristina - cristina.sauda@edu.unito.it    ~
  ~ Ultima modifica: 22/09/2018                             ~
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#include "Punto.h"
#include "Trasporto.h"
#include "TString.h"
#include "TMaterial.h"
#include "Rivelatore.h"

class Vertice;

class Urto : public Punto{
  
 public:
  
  // ------------- Costruttori --------------
  Urto();
  Urto(double x, double y, double z, int NumLayer, int ID = -2);
  Urto(double phi, double z, double R, Rivelatore *detector, int ID = -2);
  
  // ------------- Distruttori --------------
  virtual ~Urto();
  
  // ---------------- Setter ----------------
  void SetUrtoReale();
  void SetID(int ID);
  void SetNumeroLayer(int NumLayer);
  
  // ---------------- Getter ----------------
  int GetNumeroLayer() const;
  bool TestReale();
  int GetID() const;
  
  // ----------- Member functions -----------
  // Calcolo dell'intersezione cilindrica e restituzione del parametro
  static double CalcoloT(double Theta, double Phi, double XO, double YO, double Raggio);
  
  // Genera un urto partendo da un vertice
  static Urto UrtodaVertice(Vertice *Origine, Trasporto *Direttrice, double Raggio, int IDe, int NumLayer = -1);
  
  // Genera un urto partendo dall'urto, contemplando anche lo cattering multiplo con il materiale lungo la traiettoria
  Urto UrtodaUrto(Trasporto *Direttrice, Rivelatore *Detector, bool ScatteringMult = kTRUE, int NumLayer = -1);
  
  // Smearing: modifica delle coordinate a causa della risoluzione definita
  void SmearingGaussiano(Rivelatore *Detector, int NumLayer);
  
  // Rumore: crea una distribuzione uniforme di urti sul rivelatore
  static Urto RumoreRivelatore(Rivelatore *Detector, int NumLayer);
  
 private:

  // ------------- Data Member --------------
  int  dmNumLayer; // Layer su cui avviene l'urto
  int  dmID; // Label per ricordare a quale particella della simulazione MC appartiene
  bool dmUrtoReale; // Permette di decidere se l'urto reale oppure no
  
  ClassDef(Urto,1)
};

#endif
