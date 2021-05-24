# Alacre
Progetto per l'esame del corso di Tecniche di Analisi Numerica e Simulazione di Cristina Sauda ed Eleonora Racca.

Obiettivi:
- Simulare la generazione di particelle di alto momento in un esperimento a collider:
  - generazione casuale del vertice;
  - generazione della molteplicità fissa o da distribuzioni assegnata o uniforme;
  - generazione della direzione delle particelle con distribuzione uniforme in azumut e assegnata in pseudorapidità;
  - trasporto delle particelle:
    - intersezioni con beam pipe e due layer di rivelatori;
    - scattering multiplo spegnibile.
- Ricostruzione degli eventi:
  - smearing gaussiano dei punti di impatto;
  - aggiunta di rumore spegnibile.
- Analisi delle prestazioni:
  - ricostruzione del vertice;
  - efficienza in funzione della molteplicità;
  - risoluzione.
  
Esecuzione del programma
------------------------
Per eseguire il programma è consigliata la versione 6.22/02 di Root e versione c++11, usate durante la creazione del progetto. Il programma dovrebbe funzionare con versioni di Root a partire dalla 5.00, ma è necessario usare lo standard di c++ 2011 per eseguire il codice.

Comandi principali:
- compilazione:

    posizionarsi nella cartella dove è stata scaricata la repository, poi eseguire i comandi:
  
    $cd Alacre/Provette/
    $root -l
    
    dopo aver avviato Root, eseguire:
    
    root [0] .x compila.C("force");
    
- esecuzione della simulazione:
  
    eseguire le istruzioni riportate alla voce "compilazione" e, senza uscire da Root digitare il comando:
    
    root [1] Rivelatore *detector = new Rivelatore("Configurazioni/Rivelatore.txt")
    root [2] Albero(detector, kTRUE)
    
- esecuzione della ricostruzione:
  
    eseguire le istruzioni riportate alla voce "compilazione" e, senza uscire da Root digitare il comando:
    
    root [1] Rivelatore *detector = new Rivelatore("Configurazioni/Rivelatore.txt")
    root [2] Ricostruzione(detector, kTRUE)
    
- esecuzione dell'analisi:
  
    eseguire le istruzioni riportate alla voce "compilazione" e, senza uscire da Root digitare il comando:
    
    root [1] Analisi()
    
- esecuzione completa del programma:
  
    eseguire le istruzioni riportate alla voce "compilazione" e, senza uscire da Root digitare il comando:
    
    root [1] EsecuzioneEsame()
    
