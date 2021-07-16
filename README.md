# Alacre
Progetto per l'esame del corso di Tecniche di Analisi Numerica e Simulazione di Cristina Sauda (@crisauda) ed Eleonora Racca (@eleoracca).

Obiettivi del progetto:
1. Simulare la generazione di particelle di alto momento in un esperimento a collider:
   - generazione casuale del vertice;
   - generazione della molteplicità fissa o da distribuzioni assegnata o uniforme;
   - generazione della direzione delle particelle con distribuzione uniforme in azumut e assegnata in pseudorapidità;
   - trasporto delle particelle:
     - intersezioni con beam pipe e due layer di rivelatori;
     - scattering multiplo spegnibile.
2. Ricostruzione degli eventi:
   - smearing gaussiano dei punti di impatto;
   - aggiunta di rumore spegnibile.
3. Analisi delle prestazioni:
   - ricostruzione del vertice;
   - efficienza di ricostruzione;
   - risoluzione del vertice.


Esecuzione del programma
------------------------
Per eseguire il programma è consigliata la versione 6.22/02 di Root, usate durante la creazione del progetto. Il programma dovrebbe funzionare con versioni di Root a partire dalla 5.00, ma la versione minima dello standard di C++ necessaria per eseguire il codice è la 2011.

Comandi principali:
- compilazione:

    posizionarsi nella cartella dove è stata scaricata la repository, poi eseguire i comandi:
    ```
    $cd Alacre/Provette/
    $root -l
    ```
    dopo aver avviato Root, eseguire:
    ```
    root [0] .x compila.C("force");
    ```
    ```
- esecuzione completa del programma con parametri di default:
  
    eseguire le istruzioni riportate alla voce "compilazione" e, senza uscire da Root digitare il comando:
    ```
    root [1] EsecuzioneEsame()
    ```
    l'esecuzione di default prende i parametri dai file nella cartella "Configurazioni" ed esegue una sola simulazione. 
    Per cambiare le configurazioni è possibile cambiare i parametri nei file oppure avviare la simulazione con parametri impostabili dall'utente.

- esecuzione completa del programma con parametri impostati dall'utente:

    eseguire le istruzioni riportate alla voce "compilazione" e, senza uscire da Root digitare il comando:
    ```
    root [1] EsecuzioneEsame(kFALSE, kFALSE, kFALSE)
    ```
    in questo caso il programma eseguirà una sola simulazione, i parametri verranno chiesti all'utente quando necessari dalla simulazione stessa.
    
- esecuzione completa del programma con parametri di default e simulazioni multiple:

    eseguire le istruzioni riportate alla voce "compilazione" e, senza uscire da Root digitare il comando:
    ```
    root [1] EsecuzioneEsame(kTRUE)
    ```
    in questo caso il programma eseguirà simulazioni multiple con i parametri definiti nei file con suffisso "Rumore" nella cartella "Configurazioni". Non è possibile impostare manualmente durante la simulazione i parametri per simulazioni multiple.
    
- esecuzione della sola simulazione:
  
    eseguire le istruzioni riportate alla voce "compilazione" e, senza uscire da Root digitare il comando:
    ```
    root [1] Rivelatore *detector = new Rivelatore("Configurazioni/Rivelatore.txt")
    root [2] Albero(detector, kTRUE, "nome_del_file_in_uscita")
    ```
- esecuzione della sola ricostruzione:
  
    eseguire le istruzioni riportate alla voce "compilazione" e, senza uscire da Root digitare il comando:
    ```
    root [1] Rivelatore *detector = new Rivelatore("Configurazioni/Rivelatore.txt")
    root [2] Ricostruzione(detector, kTRUE, "nome_del_file_in_uscita")
    ```
- esecuzione dell'analisi:
  
    eseguire le istruzioni riportate alla voce "compilazione" e, senza uscire da Root digitare il comando:
    ```
    root [1] Rivelatore *detector = new Rivelatore("Configurazioni/Rivelatore.txt")
    root [2] Analisi(0.5, 100, detector, "nome_del_file_di_ricostruzione", "nome_del_file_di_generazione")
    ```
    
    
## Comandi utili per GitHub
Comandi base di Git
-------------------
```
git status
git add
git commit
```
   
Scaricare la repository
-----------------------

Cliccare il tasto verde Code e fare il Download della cartella .zip

oppure

Scaricare tramite tramite il link https://github.com/eleoracca/Alacre.git
