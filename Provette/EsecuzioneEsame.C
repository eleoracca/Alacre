/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~ Macro per l'esecuzione della simulazione d'esame        ~
  ~ Autori: Racca Eleonora - eleonora.racca288@edu.unito.it ~
  ~         Sauda Cristina - cristina.sauda@edu.unito.it    ~
  ~ Ultima modifica: 26/08/2018                             ~
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#if !defined(__CINT__) || defined(__MAKECINT__)
#include "Riostream.h"
#include "TStopwatch.h"
#include "TSystem.h"

#include "Albero.C"
#endif

void EsecuzioneEsame(bool fileconfig = kFALSE, TString myopt = "force"){
  
  // Dichiarazione dei timer
  TStopwatch tempototale;
  TStopwatch tempogenerazione;
  TStopwatch temporicostruzione;
  TStopwatch tempoanalisi;

  /*
  TString opt;
  if(myopt.Contains("force")){
    opt = "kfg";
  }
  else {
    opt = "kg";
  }

  // Caricamento delle macro necessarie
  gSystem->CompileMacro("Punto.cxx", opt);
  gSystem->CompileMacro("Vertice.cxx", opt);
  gSystem->CompileMacro("Rivelatore.cxx", opt);
  gSystem->CompileMacro("Albero.C", opt);
  */
  
  // --------- Simulazione e analisi ---------
  tempototale.Start(kTRUE);

  // Generazione degli eventi
  cout << "~~~~~~~~~~~~~~~~ Generazione degli eventi ~~~~~~~~~~~~~~~~" << endl;
  tempogenerazione.Start(kTRUE);
  Albero(fileconfig);
  tempogenerazione.Stop();

  // Ricostruzione degli eventi
  cout << "~~~~~~~~~~~~~~~ Ricostruzione degli eventi ~~~~~~~~~~~~~~~" << endl;
  temporicostruzione.Start(kTRUE);
  
  temporicostruzione.Stop();

  // Ricostruzione degli eventi
  cout << "~~~~~~~~~~~~~~~~~~ Analisi degli eventi ~~~~~~~~~~~~~~~~~~" << endl;
  tempoanalisi.Start(kTRUE);
  
  tempoanalisi.Stop();

  // Dati finali
  tempototale.Stop();
  cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" << endl;
  cout << "Simulazione completata" << endl;
  cout << "La durata totale è:                     "; tempototale.Print(); cout << endl;
  cout << "La generazione degli eventi è durata:   "; tempogenerazione.Print(); cout << endl;
  cout << "La ricostruzione degli eventi è durata: "; temporicostruzione.Print(); cout << endl;
  cout << "L'analisi degli eventi è durata:        "; tempoanalisi.Print(); cout << endl;
  cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" << endl;
  
}
