/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~ Dichiarazione della classe Colpi                        ~
  ~ Autori: Racca Eleonora - eleonora.racca288@edu.unito.it ~
  ~         Sauda Cristina - cristina.sauda@edu.unito.it    ~
  ~ Ultima modifica: 15/11/2017                             ~
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#ifndef COLPI_H
#define COLPI_H

#include "Punto.h"

class Colpi : public Punto{
  
 public:

  // Costruttori
  Colpi();
  Colpi(double x, double y, double z);

  // Distruttori
  virtual ~Colpi();

  // Member function
  // Getter
  double GetX() const;
  double GetY() const;
  double GetZ() const;

  // Cambi di coordinate
  void CartesianeCilindriche(Colpi hit);
  void CilindricheCartesiane(Colpi hit);
  void Rotazione(double teta, double phi, double tetaprimo, double phiprimo, double *vettorecoordinate);

 private:

  // Setter
  void SetX(double x);
  void SetY(double y);
  void SetZ(double z);
  
  // Data Member
  double dmX; //! Coordinata x delle cartesiane e rho delle cilindriche
  double dmY; //! Coordinata y delle cartesiane e teta delle cilindriche
  double dmZ; //! Coordinata z delle cartesiane e delle cilindriche

  ClassDef(Colpi,1)
};

#endif
