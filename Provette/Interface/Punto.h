#ifndef PUNTO_H
#define PUNTO_H

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~ Dichiarazione della classe Punto                        ~
  ~ Autori: Racca Eleonora - eleonora.racca288@edu.unito.it ~
  ~         Sauda Cristina - cristina.sauda@edu.unito.it    ~
  ~ Ultima modifica: 26/08/2018                             ~
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#include "TObject.h"
#include "TString.h"

class Punto : public TObject{

 public:
   
  // ------------- Costruttori --------------
  Punto();
  Punto(double X, double Y, double Z);
  Punto(double R, double Phi, double X3, TString tipo);
  
  // ------------- Distruttori --------------
  virtual ~Punto();
  
  // ---------------- Setter ----------------
  void SetCartesiane(const double X, const double Y, const double Z); //! Cartesiane: X, Y, Z 
  void SetCilindriche(const double RaggioC, const double Phi, const double Z); //! Cilindriche: R, #phi, Z
  void SetSferiche(const double RaggioS, const double Theta, const double Phi); //! Sferiche: R, #theta, #phi
  void SetX(const double X);
  void SetY(const double Y);
  void SetZ(const double Z);
  void SetTheta(const double Theta);
  void SetPhi(const double Phi);
  void SetRaggioC(const double RaggioC);
  void SetRaggioS(const double RaggioS);
  
  // ---------------- Getter ----------------
  double GetX() const;
  double GetY() const;
  double GetZ() const;
  double GetTheta() const;
  double GetPhi() const;
  double GetRaggioC() const;
  double GetRaggioS() const;
  static double GetDistanza(Punto &PuntoUno, Punto &PuntoDue);
  static double GetDeltaPhi(Punto &PuntoUno, Punto &PuntoDue);

  // ----------- Member functions -----------
  
 private:

  // ----------- Member functions -----------
  void CartesianeCilindriche();
  void CartesianeSferiche();
  void SfericheCartesiane();
  void CilindricheCartesiane();

 protected:

  // ------------- Data Member --------------
  double dmX; // Cartesiane: X
  double dmY; // Cartesiane: Y
  double dmZ; // Cartesiane: Z
  double dmRaggioC; // Cilindriche: #rho raggio
  double dmRaggioS; // Sferiche: #rho raggio
  double dmTheta; // Sferiche: #theta angolo assiale
  double dmPhi; // Cilindriche e sferiche: #phi angolo azimutale
  
  ClassDef(Punto,1)
};

#endif
