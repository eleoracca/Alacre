//da eseguire in ~/TANS/Alacre/Provette/$
//$root -l
//.x compile.C("force");

void compile(TString myopt="fast"){
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
}
