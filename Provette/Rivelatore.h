/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~ Dichiarazione della classe Rivelatore                   ~
  ~ Autori: Racca Eleonora - eleonora.racca288@edu.unito.it ~
  ~         Sauda Cristina - cristina.sauda@edu.unito.it    ~
  ~ Ultima modifica: 26/08/2018                             ~
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#ifndef RIVELATORE_H
#define RIVELATORE_H

#include "TMaterial.h"
#include "TObject.h"

class Rivelatore : public TObject{
  
 public:

  // ------------- Costruttori --------------
  Rivelatore();  
  Rivelatore(TString Configurazione);

  // ------------- Distruttori --------------
  virtual ~Rivelatore();

  // ---------------- Setter ----------------
  void SetEtaMin(const double Etamin);
  void SetEtaMax(const double Etamax);
  void SetSmearZ(const double SmearZ);
  void SetSmearRPhi(const double SmearRPhi);
  
  // ---------------- Getter ----------------
  double GetEtaMin();
  double GetEtaMax();
  double GetSmearZ();
  double GetSmearRPhi();
  double GetVerticeX();
  double GetVerticeY();
  double GetVerticeZ();
  double GetVerticeSX();
  double GetVerticeSY();
  double GetVerticeSZ();
  
 private:

  // ----------- Member functions -----------

  // ------------- Data Member --------------
  double dmRaggioBP; //! Raggio della Beam Pipe in cm
  double dmRaggio1L; //! Raggio del primo strato in cm
  double dmRaggio2L; //! Raggio del secondo strato in cm
  double dmSpessoreBP; //! Spessore della Beam Pipe in cm
  double dmSpessoreL; //! Spessore del primo strato in cm
  double dmLunghezza; //! Lunghezza del rivelatore in cm
  double dmEtaMinimo; //! Pseudorapidità minima rilevabile
  double dmEtaMassimo; //! Pseudorapidità massima rilevabile
  double dmSmearingZ; //! Smearing nella direzione Z in cm
  double dmSmearingRPhi; //! Smearing nelle direzioni R e #phi in cm
  double dmTaglioPhi; //! Taglio su #phi in mrad
  double dmTheta; //! #theta del multiple scattering nella RMS della generazione gaussiana in rad
  TMaterial dmMaterialeBP; //! Materiale della Beam Pipe
  
  double dmXVertice; //! Coordinata X del vertice generato
  double dmYVertice; //! Coordinata Y del vertice generato
  double dmZVertice; //! Coordinata Z del vertice generato
  double dmXVerticeSigma; //! Deviazione standard della coordinata X del vertice generato
  double dmYVerticeSigma; //! Deviazione standard della coordinata Y del vertice generato
  double dmZVerticeSigma; //! Deviazione standard della coordinata Z del vertice generato

  ClassDef(Rivelatore,1)
};

#endif


