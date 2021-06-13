#ifndef VERTICE_H
#define VERTICE_H

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~ Dichiarazione della classe Vertice                      ~
  ~ Autori: Racca Eleonora - eleonora.racca288@edu.unito.it ~
  ~         Sauda Cristina - cristina.sauda@edu.unito.it    ~
  ~ Ultima modifica: 26/09/2018                             ~
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#include "Punto.h"
#include "Rivelatore.h"

class Urto;

class Vertice : public Punto{

 public:

  // ------------- Costruttori --------------
  Vertice();
  Vertice(double X, double Y, double Z, int Molteplicita);
  Vertice(Rivelatore *Detector, int  Molteplicita);

  // ------------- Distruttori --------------
  virtual ~Vertice();
  
  // ---------------- Setter ----------------
  void SetMolteplicita(int Molteplicita);

  // ---------------- Getter ----------------
  int GetMolteplicita();
  Punto GetCoordinate();

  static double TrovaVertice(Urto* L1, Urto* L2);
    
 private:

  int dmMolteplicita; // Numero di tracce da generare
  
  ClassDef(Vertice,1)
};

#endif
