#include "Riostream.h"
#include "TStopwatch.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~ Macro per l'esecuzione della simulazione d'esame        ~
  ~ Autori: Racca Eleonora - eleonora.racca288@edu.unito.it ~
  ~         Sauda Cristina - cristina.sauda@edu.unito.it    ~
  ~ Ultima modifica: 15/11/2017                             ~
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void EsecuzioneEsame(TString myopt = "fast"){
  // Dichiarazione dei parametri della simulazione
  int numeroesperimenti = 0;

  // Dichiarazione dei timer
  TStopwatch tempototale;
  TStopwatch tempogenerazione;
  TStopwatch tempotrasporto;
  TStopwatch temporicostruzione;
  
  // Interimento dei parametri della simulazione
  cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" << endl;
  cout << "---------- Inserire i parametri per la generazione ---------" << endl;
  cout << "+ numero di eventi: ";
  cin >> numeroesperimenti;
  cout << "\n\n----------- Inserire i parametri per il trasporto ----------" << endl;
  cout << "\n\n--------- Inserire i parametri per la ricostruzione --------" << endl;
  

  // --------- Simulazione e analisi ---------
  tempototale.Start(kTRUE);

  char * opt;
  if(myopt.Contains("force")){
    opt = "kfg";
  }
  else {
    opt = "kg";
  }

  // Caricamento delle macro necessarie
  gSystem->CompileMacro("Colpi.cxx", opt);
  gSystem->CompileMacro("Albero.C", opt);

  // Generazione degli eventi
  cout << "~~~~~~~~~~~~~~~~ Generazione degli eventi ~~~~~~~~~~~~~~~~" << endl;
  tempogenerazione.Start(kTRUE);
  Albero(numeroesperimenti);
  tempogenerazione.Stop();

  // Trasporto degli eventi
  cout << "~~~~~~~~~~~~~~~~~ Trasporto degli eventi ~~~~~~~~~~~~~~~~~" << endl;
  tempotrasporto.Start(kTRUE);
  
  tempotrasporto.Stop();

  // Ricostruzione degli eventi
  cout << "~~~~~~~~~~~~~~~ Ricostruzione degli eventi ~~~~~~~~~~~~~~~" << endl;
  temporicostruzione.Start(kTRUE);
  
  temporicostruzione.Stop();

  // Dati finali
  tempototale.Stop();
  cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" << endl;
  cout << "Simulazione completata" << endl;
  cout << "La durata totale è:                     " << tempototale.Print() << endl;
  cout << "La generazione degli eventi è durata:   " << tempogenerazione.Print() << endl;
  cout << "Il trasporto degli eventi è durata:     " << tempotrasporto.Print() << endl;
  cout << "La ricostruzione degli eventi è durata: " << temporicostruzione.Print() << endl;
  
}
