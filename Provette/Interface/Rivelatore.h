#ifndef RIVELATORE_H
#define RIVELATORE_H

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~ Dichiarazione della classe Rivelatore                   ~
  ~ Autori: Racca Eleonora - eleonora.racca288@edu.unito.it ~
  ~         Sauda Cristina - cristina.sauda@edu.unito.it    ~
  ~ Ultima modifica: 06/09/2018                             ~
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

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
  void SetEnergia(const double Energia);
  void SetEtaMin(const double Etamin);
  void SetEtaMax(const double Etamax);
  void SetSmearZ(const double SmearZ);
  void SetSmearRPhi(const double SmearRPhi);
  
  // ---------------- Getter ----------------
  double GetRaggioBP() const;
  double GetRaggio1L() const;
  double GetRaggio2L() const;
  double GetSpessoreBP() const;
  double GetSpessoreL() const;
  double GetLunghezza() const;
  double GetEnergia() const;
  TMaterial GetMaterialeBP() const;
  TMaterial GetMaterialeL() const;
  
  double GetEtaMin() const;
  double GetEtaMax() const;
  double GetSmearZ() const;
  double GetSmearRPhi() const;
  double GetPhiLimite() const;
  
  double GetVerticeX() const;
  double GetVerticeY() const;
  double GetVerticeZ() const;
  double GetVerticeSX() const;
  double GetVerticeSY() const;
  double GetVerticeSZ() const;
  
 private:

  // ----------- Member functions -----------

  // ------------- Data Member --------------
  double dmRaggioBP; // Raggio della Beam Pipe in cm
  double dmRaggio1L; // Raggio del primo strato in cm
  double dmRaggio2L; // Raggio del secondo strato in cm
  double dmSpessoreBP; // Spessore della Beam Pipe in cm
  double dmSpessoreL; // Spessore del primo strato in cm
  double dmLunghezza; // Lunghezza del rivelatore in cm
  double dmEnergia; // Energia della collisione (#beta p c)
  TMaterial dmMaterialeBP; // Materiale della Beam Pipe
  TMaterial dmMaterialeL; // Materiale degli strati
  
  double dmEtaMinimo; // Pseudorapidit� minima rilevabile
  double dmEtaMassimo; // Pseudorapidit� massima rilevabile
  double dmSmearingZ; // Smearing nella direzione Z in cm
  double dmSmearingRPhi; // Smearing nelle direzioni R e #phi in cm
  double dmPhiLimite; // Taglio su #phi in rad
  
  double dmXVertice; // Coordinata X del vertice generato
  double dmYVertice; // Coordinata Y del vertice generato
  double dmZVertice; // Coordinata Z del vertice generato
  double dmXVerticeSigma; // Deviazione standard della coordinata X del vertice generato
  double dmYVerticeSigma; // Deviazione standard della coordinata Y del vertice generato
  double dmZVerticeSigma; // Deviazione standard della coordinata Z del vertice generato

  ClassDef(Rivelatore,1)
};

#endif
