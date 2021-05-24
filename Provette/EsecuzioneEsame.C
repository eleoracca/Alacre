/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~ Macro per l'esecuzione della simulazione d'esame        ~
  ~ Autori: Racca Eleonora - eleonora.racca288@edu.unito.it ~
  ~         Sauda Cristina - cristina.sauda@edu.unito.it    ~
  ~ Ultima modifica: 18/09/2018                             ~
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#if !defined(__CINT__) || defined(__MAKECINT__)
#include "Riostream.h"
#include "TStopwatch.h"

#include "Rivelatore.h"
#include "Albero.C"
#include "Ricostruzione.C"
#include "Colori.h"
#endif

using namespace std;
using namespace colore;


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
  cout << "~~~~~~~~~~~~~~~~~~~~~~~ " << Rosso("Generazione degli eventi") << " ~~~~~~~~~~~~~~~~~~~~~~~" << endl;
  cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
  tempogenerazione.Start(kTRUE);
  generazione = Albero(detector, filegenerazione);
  tempogenerazione.Stop();

  // Ricostruzione degli eventi
  cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
  cout << "~~~~~~~~~~~~~~~~~~~~~~ " << Rosso("Ricostruzione degli eventi") << " ~~~~~~~~~~~~~~~~~~~~~~" << endl;
  cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
  temporicostruzione.Start(kTRUE);
  if(generazione){
    ricostruzione = Ricostruzione(detector, filerumore);
  }
  temporicostruzione.Stop();

  // Analisi degli eventi
  cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
  cout << "~~~~~~~~~~~~~~~~~~~~~~~~~ " << Rosso("Analisi degli eventi") << " ~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
  cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
  tempoanalisi.Start(kTRUE);
  if(generazione && ricostruzione){
    analisi = kTRUE;
  }  
  tempoanalisi.Stop();

  // Dati finali TEMPO IMPIEGATO
  tempototale.Stop();
  cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
  cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~ " << Rosso("Tempo impiegato") << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
  cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
  if(generazione && ricostruzione && analisi){
    cout << Verde("Simulazione completata con successo.") << endl;
  }
  else{
    cout << Errore("Simulazione completata con errori.") << endl;
    if(!generazione) cout << "- Problemi con la generazione degli eventi." << endl;
    if(!ricostruzione) cout << "- Problemi con la ricostruzione degli eventi." << endl;
    if(!analisi) cout << "- Problemi con l'analisi degli eventi." << endl;
  }
  cout << "La durata totale e:                     "; tempototale.Print();
  cout << "La generazione degli eventi e durata:   "; tempogenerazione.Print();
  cout << "La ricostruzione degli eventi e durata: "; temporicostruzione.Print();
  cout << "L'analisi degli eventi e durata:        "; tempoanalisi.Print();
  cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
  cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
  cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" << endl;
}
