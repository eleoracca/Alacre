/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~ Macro per la compilazione                               ~
  ~ Autori: Racca Eleonora - eleonora.racca288@edu.unito.it ~
  ~         Sauda Cristina - cristina.sauda@edu.unito.it    ~
  ~ Ultima modifica: 05/09/2018 
  ~ Modifiche recenti: 30/04/2021                          ~
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

//da eseguire in ~/TANS/Alacre/Provette/$
//$root -l
//.x compila.C("force");

#if !defined(__CINT__) || defined(__MAKECINT__)
#include "TString.h"
#include "TSystem.h"
#endif

void compila(TString myopt="fast"){
  TString opt;
  if(myopt.Contains("force")){
    opt = "kfg";
  }
  else{
    opt = "kg";
  }

  gSystem->AddIncludePath("-I./Interface");
  
  gSystem->CompileMacro("Sources/Punto.cxx", opt.Data());
  gSystem->CompileMacro("Sources/Rivelatore.cxx", opt.Data());
  gSystem->CompileMacro("Sources/Vertice.cxx", opt.Data());
  gSystem->CompileMacro("Sources/Trasporto.cxx", opt.Data());
  gSystem->CompileMacro("Sources/Urto.cxx", opt.Data());
  
  gSystem->CompileMacro("EsecuzioneEsame.C", opt.Data());
}

/*
Per eseguire EsecuzioneEsame:
- Avviare Root
- Compilare le classi e le macro come scritto alle righe 8-10
- Chiamare dentro Root EsecuzioneEsame()
*/
