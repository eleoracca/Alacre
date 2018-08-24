/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~ Dichiarazione della classe Rivelatore                   ~
  ~ Autori: Racca Eleonora - eleonora.racca288@edu.unito.it ~
  ~         Sauda Cristina - cristina.sauda@edu.unito.it    ~
  ~ Ultima modifica: 24/08/2018                             ~
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
  virtual ~Rivelatore(){};

  // ---------------- Setter ----------------
  void SetEtaMin(const double Etamin);
  void SetEtaMax(const double Etamax);
  void SetSmearZ(const double SmearZ);
  void SetSmearRP(const double SmearRPhi);
  
  // ---------------- Getter ----------------
  double GetEtaMin() const;
  double GetEtaMax() const;
  double GetSmearZ() const;
  double GetSmearRP() const;
  double GetVerticeX() const;
  double GetVerticeY() const;
  double GetVerticeZ() const;
  double GetVerticeSX() const;
  double GetVerticeSY() const;
  double GetVerticeSZ() const;
  
 private:
  // ----------- Member functions -----------

  // ------------- Data Member --------------
  static const double dmRaggioBP; //! Raggio della Beam Pipe in cm
  static const double dmRaggio1L; //! Raggio del primo strato in cm
  static const double dmRaggio2L; //! Raggio del secondo strato in cm
  static const double dmSpessoreBP; //! Spessore della Beam Pipe in cm
  static const double dmSpessoreL; //! Spessore del primo strato in cm
  static const double dmLunghezza; //! Lunghezza del rivelatore in cm
  double dmEtaMinimo; //! Pseudorapidità minima rilevabile
  double dmEtaMassimo; //! Pseudorapidità massima rilevabile
  double dmSmearingZ; //! Smearing nella direzione Z in cm
  double dmSmearingRPhi; //! Smearing nelle direzioni R e #phi in cm
  static const double dmTaglioPhi; //! Taglio su #phi in mrad
  static const double dmTheta; //! #theta del multiple scattering nella RMS della generazione gaussiana in rad
  static const TMaterial dmMaterialeBP; //! Materiale della Beam Pipe
  
  static const double dmXVertice; //! Coordinata X del vertice generato
  static const double dmYVertice; //! Coordinata Y del vertice generato
  static const double dmZVertice; //! Coordinata Z del vertice generato
  static const double dmXVerticeSigma; //! Deviazione standard della coordinata X del vertice generato
  static const double dmYVerticeSigma; //! Deviazione standard della coordinata Y del vertice generato
  static const double dmZVerticeSigma; //! Deviazione standard della coordinata Z del vertice generato
};

#endif


