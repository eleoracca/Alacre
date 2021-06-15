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
#include "Analisi.C"
#include "Colori.h"
#endif

using namespace std;
using namespace colore;


void EsecuzioneEsame(bool filegenerazione = kTRUE, bool filerumore = kTRUE, bool simulazionimultiple = kFALSE){
  
  // Dichiarazione dei timer
  TStopwatch tempototale;
  TStopwatch tempogenerazione;
  TStopwatch temporicostruzione;
  TStopwatch tempoanalisi;
  
  // --------- Simulazione e analisi ---------
  tempototale.Start(kTRUE);

  // Rivelatore per la generazione e la ricostruzione
  Rivelatore *detector = new Rivelatore("Configurazioni/Rivelatore.txt");

  // Controllo che il rivelatore sia stato creato correttamente
  if(detector -> GetPhiLimite() == -100){
    cout << Errore("Errore con la costruzione del rivelatore. \nLa simulazione si interrompe.") << endl;
    return;
  }

  // Desinenze dei file
  TString desinenze[10] = {"\0"};
  
  // Booleani
  bool generazione[10] = {kFALSE};
  bool ricostruzione[10] = {kFALSE};
  bool analisi = kFALSE;

  // Generazione degli eventi
  cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
  cout << "~~~~~~~~~~~~~~~~~~~~~~~~ " << Rosso("Generazione degli eventi") << " ~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
  cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
  tempogenerazione.Start(kTRUE);
  if(simulazionimultiple){
    for(int i = 0; i < 10; i++){
      generazione[i] = Albero(detector, filegenerazione, desinenze[i]);
    }
  }
  else{
    generazione[0] = Albero(detector, filegenerazione);
  }
  tempogenerazione.Stop();
  
  // Ricostruzione degli eventi
  cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
  cout << "~~~~~~~~~~~~~~~~~~~~~~~ " << Rosso("Ricostruzione degli eventi") << " ~~~~~~~~~~~~~~~~~~~~~~~" << endl;
  cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
  temporicostruzione.Start(kTRUE);
  if(simulazionimultiple){
    for(int i = 0; i < 10; i++){
      if(generazione[i]){
	ricostruzione[i] = Albero(detector, filerumore, desinenze[i]);
      }
    }
  }
  else{
    if(generazione[0]){
      ricostruzione[0] = Ricostruzione(detector, filerumore);
    }
  }
  temporicostruzione.Stop();

  // Analisi degli eventi
  cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
  cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~ " << Rosso("Analisi degli eventi") << " ~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
  cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
  tempoanalisi.Start(kTRUE);
  if(generazione[0] && ricostruzione[0]){
    for(int i = 0; i < 10; i++){
      analisi = Analisi(0.5, 100, detector, desinenze);
    }
  }
  tempoanalisi.Stop();

  // Dati finali - TEMPO IMPIEGATO
  tempototale.Stop();
  cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
  cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~ " << Rosso("Tempo impiegato") << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
  cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
  if(generazione[0] && ricostruzione[0] && analisi){
    cout << Verde("Simulazione completata con successo.") << endl;
  }
  else{
    cout << Errore("Simulazione completata con errori.") << endl;
    if(!generazione[0]) cout << "- Problemi con la generazione degli eventi." << endl;
    if(!ricostruzione[0]) cout << "- Problemi con la ricostruzione degli eventi." << endl;
    if(!analisi) cout << "- Problemi con l'analisi degli eventi." << endl;
  }
  cout << "La durata totale e:                     "; tempototale.Print();
  cout << "La generazione degli eventi e durata:   "; tempogenerazione.Print();
  cout << "La ricostruzione degli eventi e durata: "; temporicostruzione.Print();
  cout << "L'analisi degli eventi e durata:        "; tempoanalisi.Print();
  cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
  cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
  cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" << endl;
}
