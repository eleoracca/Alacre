/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~ Macro per la compilazione                               ~
  ~ Autori: Racca Eleonora - eleonora.racca288@edu.unito.it ~
  ~         Sauda Cristina - cristina.sauda@edu.unito.it    ~
  ~ Ultima modifica: 05/09/2018                             ~
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

//da eseguire in ~/TANS/Alacre/Provette/$
//$root -l
//.x compila.C("force");

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
  
  gSystem->CompileMacro("Albero.C", opt.Data());
  gSystem->CompileMacro("Ricostruzione.C", opt.Data());
}
