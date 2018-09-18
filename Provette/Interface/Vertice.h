#ifndef VERTICE_H
#define VERTICE_H

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~ Dichiarazione della classe Vertice                      ~
  ~ Autori: Racca Eleonora - eleonora.racca288@edu.unito.it ~
  ~         Sauda Cristina - cristina.sauda@edu.unito.it    ~
  ~ Ultima modifica: 18/09/2018                             ~
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#include "Punto.h"
#include "Rivelatore.h"

class Vertice : public Punto{

 public:

  // ------------- Costruttori --------------
  Vertice();
  Vertice(double X, double Y, double Z, int Molteplicita, int Rumore = 0);
  Vertice(Rivelatore *Detector, int  Molteplicita, int  Rumore = 0);

  // ------------- Distruttori --------------
  virtual ~Vertice();
  
  // ---------------- Setter ----------------
  void SetMolteplicita(int Molteplicita);
  void SetRumore(double Rumore);
  
  // ---------------- Getter ----------------
  int GetMolteplicita();
  double GetRumore();
    
 private:

  int dmMolteplicita; // Numero di tracce da generare
  double dmRumore; // Numero di particelle di rumore da generare o generate
  
  ClassDef(Vertice,1)
};

#endif
