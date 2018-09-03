/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~ Dichiarazione della classe Vertice                        ~
  ~ Autori: Racca Eleonora - eleonora.racca288@edu.unito.it ~
  ~         Sauda Cristina - cristina.sauda@edu.unito.it    ~
  ~ Ultima modifica: 26/08/2018                             ~
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#ifndef VERTICE_H
#define VERTICE_H

#include "Punto.h"
#include "Rivelatore.h"

class Vertice : public Punto{

 public:

  // ------------- Costruttori --------------
  Vertice();
  //Vertice(double X, double Y, double Z, int Molteplicita, int Rumore = 0, int ID);
  Vertice(double X, double Y, double Z, int Molteplicita, TString DistRumore, int Rumore = 0);
  Vertice(Rivelatore *Detector, int  Molteplicita, TString DistRumore, int  Rumore = 0);

  // ------------- Distruttori --------------
  virtual ~Vertice();
  
  // ---------------- Setter ----------------
  void SetMolteplicita(int Molteplicita);
  //void SetID(int ID);
  void SetRumore(double Rumore);
  void SetDistRumore(TString DistRumore);
  
  // ---------------- Getter ----------------
  int GetMolteplicita();
  //int GetID();
  double GetRumore();
  TString GetDistRumore();
    
 private:

  int dmMolteplicita; //! Numero di tracce da generare
  //int dmID; //! Etichetta utile
  double dmRumore; //! Numero di particelle di rumore da generare
  TString dmDistRumore; //! Distribuzione del rumore da generare
  
  ClassDef(Vertice,1)
};

#endif