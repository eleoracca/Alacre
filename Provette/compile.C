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

  gSystem->CompileMacro("Punto.cxx", opt.Data());
  gSystem->CompileMacro("Vertice.cxx", opt.Data());
  gSystem->CompileMacro("Rivelatore.cxx", opt.Data());
  gSystem->CompileMacro("Albero.C", opt.Data());
}
