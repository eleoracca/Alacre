/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~ Macro per l'esecuzione della simulazione d'esame        ~
  ~ Autori: Racca Eleonora - eleonora.racca288@edu.unito.it ~
  ~         Sauda Cristina - cristina.sauda@edu.unito.it    ~
  ~ Ultima modifica: 06/09/2018                             ~
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#if !defined(__CINT__) || defined(__MAKECINT__)
#include "Riostream.h"
#include "TStopwatch.h"

#include "Rivelatore.h"
#include "Albero.C"
#include "Ricostruzione.C"
#endif


void EsecuzioneEsame(bool filegenerazione = kTRUE, bool filerumore = kTRUE){
  
  // Dichiarazione dei timer
  TStopwatch tempototale;
  TStopwatch tempogenerazione;
  TStopwatch temporicostruzione;
  TStopwatch tempoanalisi;
  
  // --------- Simulazione e analisi ---------
  tempototale.Start(kTRUE);

  // Rivelatore per la generazione e la ricostruzione
  Rivelatore *detector = new Rivelatore("Configurazioni/Rivelatore.txt");
  
  // Booleani
  bool generazione = kFALSE;
  bool ricostruzione = kFALSE;
  bool analisi = kFALSE;

  // Generazione degli eventi
  cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
  cout << "~~~~~~~~~~~~~~~~~~~~~~~ Generazione degli eventi ~~~~~~~~~~~~~~~~~~~~~~~" << endl;
  cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
  tempogenerazione.Start(kTRUE);
  generazione = Albero(detector, filegenerazione);
  tempogenerazione.Stop();

  // Ricostruzione degli eventi
  cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
  cout << "~~~~~~~~~~~~~~~~~~~~~~ Ricostruzione degli eventi ~~~~~~~~~~~~~~~~~~~~~~" << endl;
  cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
  temporicostruzione.Start(kTRUE);
  if(generazione){
    ricostruzione = Ricostruzione(detector, filerumore);
  }
  temporicostruzione.Stop();

  // Ricostruzione degli eventi
  cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
  cout << "~~~~~~~~~~~~~~~~~~~~~~~~~ Analisi degli eventi ~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
  cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
  tempoanalisi.Start(kTRUE);
  if(generazione && ricostruzione){
    analisi = kTRUE;
  }  
  tempoanalisi.Stop();

  // Dati finali TEMPO IMPIEGATO
  tempototale.Stop();
  cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
  cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~ Tempo impiegato ~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
  cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
  if(generazione && ricostruzione && analisi){
    cout << "Simulazione completata con successo." << endl;
  }
  else{
    cout << "Simulazione completata con errori." << endl;
  }
  cout << "La durata totale e:                     "; tempototale.Print();
  cout << "La generazione degli eventi e durata:   "; tempogenerazione.Print();
  cout << "La ricostruzione degli eventi e durata: "; temporicostruzione.Print();
  cout << "L'analisi degli eventi e durata:        "; tempoanalisi.Print();
  cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
  cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
  cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" << endl;
  
}
