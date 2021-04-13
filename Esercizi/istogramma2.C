#if !defined(__CINT__) || defined(__MAKECINT__)
#include <Riostream.h>
#include <TH1D.h>
#include <TFile.h>
#include <TCanvas.h>

#endif

using namespace std;

void istogramma2(const char *filein, const char *fileout){
  ifstream in(filein);

  if(!in){
    cout << "Non esiste un file con questo nome in questa cartella." << endl;
    return;
  }

  double min;
  double max;
  double temp;
  double dato;
  std::vector<double> vettore;

  in >> temp;

  vettore.push_back(temp);
  min = temp;
  max = temp;
  
  while((in >> dato)){
    vettore.push_back(dato);

    if(dato < min) min = dato;
    else if(dato > max) max = dato;
  }
  
  TH1D *hist = new TH1D("hist", "Istogramma", sqrt(vettore.size()), min, max);
  
  for(const double prova:vettore){
    hist->Fill(prova);
  }
  
  cout << "I dati sono: " << vettore.size() << endl;
  in.close();
  hist->Draw();

  TFile file(fileout, "recreate");
  hist-> Write();
  file.Close();
}
