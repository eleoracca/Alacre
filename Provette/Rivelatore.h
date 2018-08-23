/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~ Dichiarazione della classe Rivelatore                   ~
  ~ Autori: Racca Eleonora - eleonora.racca288@edu.unito.it ~
  ~         Sauda Cristina - cristina.sauda@edu.unito.it    ~
  ~ Ultima modifica: 23/08/2018                             ~
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
  void SetRBP(const double RaggioBP);
  void SetR1L(const double Raggio1L);
  void SetR2L(const double Raggio2L);
  void SetSBP(const double SpessoreBP);
  void SetS1L(const double Spessore1L);
  void SetLunghezza(const double Lunghezza);
  void SetEtaMin(const double Etamin);
  void SetEtaMax(const double Etamax);
  void SetSmearZ(const double SmearZ);
  void SetSmearRP(const double SmearRPhi);
  void SetMaterialeBP(const TMaterial Materiale);
  
  // ---------------- Getter ----------------
  double GetRBP() const;
  double GetR1L() const;
  double GetR2L() const;
  double GetSBP() const;
  double GetS1L() const;
  double GetLunghezza() const;
  double GetEtaMin() const;
  double GetEtaMax() const;
  double GetSmearZ() const;
  double GetSmearRP() const;
  
 private:
  // ----------- Member functions -----------

  // ------------- Data Member --------------
  static const double dmRaggioBP; //! Raggio della Beam Pipe in cm
  static const double dmRaggio1L; //! Raggio del primo strato in cm
  static const double dmRaggio2L; //! Raggio del secondo strato in cm
  static const double dmSpessoreBP; //! Spessore della Beam Pipe in cm
  static const double dmSpessoreL; //! Spessore del primo strato in cm
  static const double dmLunghezza; //! Lunghezza del rivelatore in cm
  static const double dmEtaMinimo; //! Pseudorapidità minima rilevabile
  static const double dmEtaMassimo; //! Pseudorapidità massima rilevabile
  static const double dmSmearingZ; //! Smearing nella direzione Z in cm
  static const double dmSmearingRPhi; //! Smearing nelle direzioni R e #phi in cm
  static const double dmTaglioPhi; //! Taglio su #phi in mrad
  static const double dmTheta; //! #theta del multiple scattering nella RMS della generazione gaussiana in rad
  static const TMaterial dmMaterialeBP; //! Materiale della Beam Pipe
};

#endif


