/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~ Dichiarazione della classe Urto                        ~
  ~ Autori: Racca Eleonora - eleonora.racca288@edu.unito.it ~
  ~         Sauda Cristina - cristina.sauda@edu.unito.it    ~
  ~ Ultima modifica: 15/11/2017                             ~
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#ifndef URTO_H
#define URTO_H

#include "Punto.h"

class Urto : public Punto{
  
 public:

  // Costruttori
  Urto();
  Urto(double x, double y, double z);

  // Distruttori
  virtual ~Urto();

  // Member function
  // Getter
  double GetX() const;
  double GetY() const;
  double GetZ() const;

 private:

  // Setter
  void SetX(double x);
  void SetY(double y);
  void SetZ(double z);
  
  // Data Member
  double dmX; //! Coordinata x delle cartesiane e rho delle cilindriche
  double dmY; //! Coordinata y delle cartesiane e teta delle cilindriche
  double dmZ; //! Coordinata z delle cartesiane e delle cilindriche

  ClassDef(Urto,1)
};

#endif
