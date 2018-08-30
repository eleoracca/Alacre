/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~ Dichiarazione della classe Trasporto                    ~
  ~ Autori: Racca Eleonora - eleonora.racca288@edu.unito.it ~
  ~         Sauda Cristina - cristina.sauda@edu.unito.it    ~
  ~ Ultima modifica: 27/08/2018                             ~
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#ifndef TRASPORTO_H
#define TRASPORTO_H

#include "TObject.h"
#include "TString.h"

class Trasporto : public TObject {
  
 public:
  // ----------- Costruttori -----------
  Trasporto();
  Trasporto(double Theta, double Phi, int ID);
  
  // ----------- Distruttori -----------
  virtual ~Trasporto();
  
  // ----------- Setter -----------
  void SetDirTheta(const double Theta);
  void SetDirPhi(const double Phi);
  void SetCosDirX(const double Cx);
  void SetCosDirY(const double Cy);
  void SetCosDirZ(const double Cz);
  void SetCosDir(const double Cx, const double Cy, const double Cz);
  void SetAngoli(const double Theta, const double Phi);
  void SetDirID(const double ID);
  void FlipBit();
  void Rotazione(double Theta, double Phi);
  
  // ----------- Getter -----------
  int GetID();
  double GetDirTheta();
  double GetDirPhi();
  double GetDirCosX();
  double GetDirCosY();
  double GetDirCosZ();
  bool GetRotStatus();
  
 private:
  
  void AggiornaAng();
  void AggiornaDirCos();
  
  // ----------- Data Member -----------
  int dmID; //! Numero identificativo
  double dmTheta; //! Angolo #theta
  double dmPhi; //! Angolo #phi
  double dmCDx; //! Coseno Direttore x.
  double dmCDy; //! Coseno Direttore y.
  double dmCDz; //! Coseno Direttore z.
  bool dmIsrotated; //! Debug flag.
  
  ClassDef(Trasporto, 1)
    
};

#endif
