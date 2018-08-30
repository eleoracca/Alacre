/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~ Dichiarazione della classe Urto                         ~
  ~ Autori: Racca Eleonora - eleonora.racca288@edu.unito.it ~
  ~         Sauda Cristina - cristina.sauda@edu.unito.it    ~
  ~ Ultima modifica: 28/08/2018                             ~
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
 
#ifndef URTO_H
#define URTO_H

#include "Punto.h"
#include "Vertice.h"
#include "Trasporto.h"
#include "TString.h"
#include "TMaterial.h"

class Urto : public Punto{
  
 public:
  
  // ------------- Costruttori --------------
  Urto();
  Urto(double x, double y, double z, int NumLayer, int ID = -2);
  
  // ------------- Distruttori --------------
  virtual ~Urto();
  
  // ---------------- Setter ----------------
  /*
  void SetX(double x);
  void SetY(double y);
  void SetZ(double z);
  */
  void SetUrtoReale();
  void SetID(int ID);
  void SetLayer(int NumLayer);
  
  // ---------------- Getter ----------------
  /*
  double GetX();
  double GetY();
  double GetZ();
  */
  int GetNumeroLayer();
  bool TestReale();
  int GetID();
  
  // ----------- Member functions -----------
  // Calcolo dell'intersezione cilindrica e restituzione del parametro
  static double CalcoloT(double Theta, double Phi, double XO, double YO, double Raggio);
  
  // Genera un urto partendo da un vertice
  static Urto UrtoSuVertice(Vertice &Origine, Trasporto &Direttrice, double Raggio, int ID, int NumLayer = 0);
  
  // Genera un urto partendo dall'urto, contemplando anche lo cattering multiplo con il materiale lungo la traiettoria
  Urto UrtodaUrto(Trasporto &Direttrice, double Raggio, TMaterial &Materiale, double Larghezza, bool ScatteringMult = kTRUE, int NumLayer = 0, double P = 750, int Z = 1, double Beta = 1);
  
  // Smearing: modifica delle coordinate a causa della risoluzione definita
  void SmearingGaussiano(double RaggioDetector, double ZDeviazioneStandard, double RPhiDeviazioneStandard);
  
  // Rumore: il primo generatore crea una distribuzione uniforme di urti sul rivelatore
  static Urto RumoreRivelatore(double RaggioC, double Zmin, double Zmax);
  
 private:

  // ------------- Data Member --------------
  /*
  double dmX; //! Coordinata x delle cartesiane e rho delle cilindriche
  double dmY; //! Coordinata y delle cartesiane e teta delle cilindriche
  double dmZ; //! Coordinata z delle cartesiane e delle cilindriche
  */
  int  dmNumLayer; //! Layer su cui avviene l'urto
  int  dmID; //! Label per ricordare a quale simulazione MC appartiene
  bool dmUrtoReale; //! Permette di decidere se l'urto reale oppure no
  
  ClassDef(Urto,1)
};

#endif
