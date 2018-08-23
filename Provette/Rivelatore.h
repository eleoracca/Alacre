/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~ Dichiarazione della classe Vertice                        ~
  ~ Autori: Racca Eleonora - eleonora.racca288@edu.unito.it ~
  ~         Sauda Cristina - cristina.sauda@edu.unito.it    ~
  ~ Ultima modifica: 12/06/2018                             ~
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#ifndef RIVELATORE_H
#define RIVELATORE_H

#include "TMaterial.h"
#include "TObject.h"

class Rivelatore : public TObject{
  
 public:
  Rivelatore();
  
  Rivelatore();
} 
/*namespace detector {
  
  static const Double_t rBP = 3.;   //cm    R Beam Pipe
  static const Double_t tBP = 0.08; //cm    thickness Beam Pipe
  
  static const Double_t r1L = 4.;   //cm    R 1st Layer
  static const Double_t t1L = 0.02; //cm    thickness 1st Layer
  
  static const Double_t r2L = 7.;   //cm    R 2nd Layer
  static const Double_t t2L = 0.02; //cm    thickness 2nd Layer
  
  static const Double_t length = 27.; //cm
  
  static const Double_t etaMin = -2.; // pseudorapidity acceptance  
  static const Double_t etaMax = 2.;
  
  static const Double_t smZ = 0.012; //cm  smearing Z direction  (120 #mum)
  static const Double_t smRPhi = 0.003; //cm  smearing r#phi direction (30 #mum)

  static const Double_t cutPhi = 0.01; // rad = 10 mrad

  static const Double_t thetaSpace = TMath::Sqrt(2.)*0.001; //[rad] RMS generazione gaussiana theta multiple scattering
  
}*/


#endif
