/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~ Macro per l'esecuzione della simulazione d'esame        ~
  ~ Autori: Racca Eleonora - eleonora.racca288@edu.unito.it ~
  ~         Sauda Cristina - cristina.sauda@edu.unito.it    ~
  ~ Ultima modifica: 18/09/2018                             ~
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#if !defined(__CINT__) || defined(__MAKECINT__)
#include "Riostream.h"
#include "TStopwatch.h"
#include "TString.h"

#include "Rivelatore.h"
#include "Albero.C"
#include "Ricostruzione.C"
#include "Analisi.C"
#include "Colori.h"
#endif

using namespace std;
using namespace colore;

void EsecuzioneEsame(bool simulazioniMultiple, bool filegenerazione, bool filerumore);
bool SingolaEsecuzione(bool filegenerazione, bool filerumore, bool generazioneFatta, TString nomeRicostruzione, TString nomeGenerazione);

void EsecuzioneEsame(bool simulazioniMultiple = kFALSE, bool filegenerazione = kTRUE, bool filerumore = kTRUE) {
  #define NFILES 4
  TString ricostruzioni[NFILES] = {"Rumore0", "Rumore10", "Rumore50", "Rumore100"};
  bool esitoEsecuzione[NFILES] = {kFALSE, kFALSE, kFALSE, kFALSE};
  bool generazioneFatta = kFALSE;


  if(simulazioniMultiple) {
    for(int i = 0; i < NFILES; i++) {
      esitoEsecuzione[i] = SingolaEsecuzione(kTRUE, kTRUE, generazioneFatta, ricostruzioni[i], "\0");
      if(!esitoEsecuzione[0] && i == 0) {
        cout << Errore("Prima esecuzione fallita!") << endl;
        return;
      }
      generazioneFatta = kTRUE;
    }

    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    for(int i = 0; i < NFILES; i++) {
      cout << "Analisi " + TString::Itoa(i, 10) + " eseguita con il seguente esito: " << esitoEsecuzione[i] << endl;
    }
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
  }
  else {
    esitoEsecuzione[0] = SingolaEsecuzione(kTRUE, kTRUE, kFALSE, "\0", "\0");
  }

  return;
}

bool SingolaEsecuzione(bool filegenerazione = kTRUE, bool filerumore = kTRUE, bool generazioneFatta = kFALSE, TString nomeRicostruzione = "\0", TString nomeGenerazione = "\0") {
  
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
    return kFALSE;
  }
  
  // Booleani
  bool generazione = kFALSE;
  bool ricostruzione = kFALSE;
  bool analisi = kFALSE;

  // Generazione degli eventi
  if(!generazioneFatta) {
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    cout << "~~~~~~~~~~~~~~~~~~~~~~~ " << Rosso("Generazione degli eventi") << " ~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    tempogenerazione.Start(kTRUE);
    generazione = Albero(detector, filegenerazione, nomeGenerazione);
    tempogenerazione.Stop();
  }
  else {
    generazione = kTRUE;
  }

  // Ricostruzione degli eventi
  cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
  cout << "~~~~~~~~~~~~~~~~~~~~~~ " << Rosso("Ricostruzione degli eventi") << " ~~~~~~~~~~~~~~~~~~~~~~" << endl;
  cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
  temporicostruzione.Start(kTRUE);
  if(generazione){
    ricostruzione = Ricostruzione(detector, filerumore, nomeRicostruzione, nomeGenerazione);
  }
  temporicostruzione.Stop();

  // Analisi degli eventi
  cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
  cout << "~~~~~~~~~~~~~~~~~~~~~~~~~ " << Rosso("Analisi degli eventi") << " ~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
  cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
  tempoanalisi.Start(kTRUE);
  if(generazione && ricostruzione){
    analisi = Analisi(0.5, 100, detector, nomeRicostruzione, nomeGenerazione);
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

  return generazione && ricostruzione && analisi;
}