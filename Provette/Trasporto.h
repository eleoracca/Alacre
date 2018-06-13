/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~ Dichiarazione della classe Trasporto                    ~
  ~ Autori: Racca Eleonora - eleonora.racca288@edu.unito.it ~
  ~         Sauda Cristina - cristina.sauda@edu.unito.it    ~
  ~ Ultima modifica: 13/06/2018                             ~
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#ifndef TRASPORTO_H
#define TRASPORTO_H

#include <TObject.h>
#include <TString.h>

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
      void SetCosDirx(const double Cx);
      void SetCosDiry(const double Cy);
      void SetCosDirz(const double Cz);
      void SetAllCos(const double Cx, const double Cy, const double Cz);
      void SetAllAngles(const double Theta, const double Phi);
      void SetDirectID(const double id);
      void FlipBit();
      void Rotate(double Theta, double Phi);

      // ----------- Getter -----------
      int    GetDirectID()     const; {return dmID;}
      double GetDirectTheta()  const; {return dmTheta;}
      double GetDirectPhi()    const; {return dmPhi;}
      double GetDirCos1()      const; {return dmCDx;}
      double GetDirCos2()      const; {return dmCDy;}
      double GetDirCos3()      const; {return dmCDz;}
      bool   GetRotStatus()    const; {return dmIsrotated;}

   private:

      void UpdateAng();
      void UpdateDirCos();

      // ----------- Data Member -----------
      int    dmID;    			//! Numero identificativo
      double dmTheta;       //! Angolo ϑ
      double dmPhi;         //! Angolo φ
      double dmCDx;         //! Coseno Direttore x.
      double dmCDy;         //! Coseno Direttore y.
      double dmCDz;         //! Coseno Direttore z.
      bool   dmIsrotated;   //! Debug flag.

   ClassDef(Trasporto, 1)

};

#endif
