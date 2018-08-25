/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~ Dichiarazione della classe Punto                        ~
  ~ Autori: Racca Eleonora - eleonora.racca288@edu.unito.it ~
  ~         Sauda Cristina - cristina.sauda@edu.unito.it    ~
  ~ Ultima modifica: 25/08/2018                             ~
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#ifndef PUNTO_H
#define PUNTO_H

#include "TObject.h"

class Punto : public TObject{

 public:
   
  // ------------- Costruttori --------------
  Punto();
  Punto(double X, double Y, double Z);
  
  // ------------- Distruttori --------------
  virtual ~Punto(){}
  
  // ---------------- Setter ----------------
  void SetCartesiane(const double X, const double Y, const double Z); //! Cartesiane: X, Y, Z 
  void SetCilindriche(const double RaggioC, const double Phi, const double Z); //! Cilindriche: R, φ, Z
  void SetSferiche(const double RaggioS, const double Theta, const double Phi); //! Sferiche: R, ϑ, φ
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
  static double EtaTheta(bool &distribuzione, const double &inferiore, const double &superiore, TH1F* istogramma);
  
 private:

  // ----------- Member functions -----------
  void CartesianeCilindriche();
  void CartesianeSferiche();
  void SfericheCartesiane();
  void CilindricheCartesiane();

  // ------------- Data Member --------------
  double dmX; //! Cartesiane: X
  double dmY; //! Cartesiane: Y
  double dmZ; //! Cartesiane: Z
  double dmRaggioC; //! Cilindriche: ρ raggio
  double dmRaggioS; //! Sferiche: ρ raggio
  double dmTheta; //! Sferiche: ϑ angolo assiale
  double dmPhi; //! Cilindriche e sferiche: φ angolo azimutale
  
  ClassDef(Punto,1)
};

#endif
