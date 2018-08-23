/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~ Dichiarazione della classe Vertice                        ~
  ~ Autori: Racca Eleonora - eleonora.racca288@edu.unito.it ~
  ~         Sauda Cristina - cristina.sauda@edu.unito.it    ~
  ~ Ultima modifica: 12/06/2018                             ~
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#ifndef VERTICE_H
#define VERTICE_H
#include "Punto.h"

class Vertice : public Punto {
 public:
  Vertice();
  //Vertice(double X, double Y, double Z, int Molteplicita, int ID, int Rumore = 0);
  Vertice(double X, double Y, double Z, int Molteplicita, int Rumore = 0);
  virtual ~Vertice();
  
  // ---------- Setters ----------
  void SetMolteplicita(int Molteplicita);
  //void SetID(int ID);
  void SetRumore(double Rumore);
  
  // ---------- Getters ----------
  int GetMolteplicita() const;
  //int GetID() const;
  double GetRumore() const;
  
  
 private:
  int dmMolteplicita;   // Numero di tracce da generare
  //int dmID;       // Etichetta utile
  double dmRumore;  // Assegna il livello del rumore
  
  
  ClassDef(Vertice,1)
};
#endif
