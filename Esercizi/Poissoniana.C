#if !defined(__CINT__) || defined(__MAKECINT__)
#include <Riostream.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TH1F.h>
#include <TRandom.h>
#include <TRandom3.h>
#endif

using namespace std;
using namespace TMath;

// Dichiarazioni
double Decadimento(int ntot, const float deltat, const float ttot, const float alfa);
double Binomiale(const int nesperimenti, const int nintervalli, float beta, float tempo);
double Poisson(const int nesperimenti, float beta, float tempo);
double LogGamma();
void Poissoniana(int ntot = 1000, const float deltat = 1., const float ttot = 100., const float alfa = 2.5e-5, const int nesp = 1000);

// Implementazioni
void Poissoniana(int ntot, const float deltat, const float ttot, const float alfa, const int nesp){

  const int dimensione = (int)ttot/deltat;
  const int tempotot = dimensione*deltat;
  const float Ntot = ntot;
  const float beta = ntot*alfa;
  const int nbin = Ntot*0.01 + 1;
  double Nrest;
  
  TH1F *histo = new TH1F("histo", "Numero di decadimenti", nbin, - 0.5, nbin - 0.5);
  THF1 *fpoissoniana = new THF1("fpoissoniana", "Curva poissoniana", nbin, -0.5, nbin - 0.5);
  THF1 *fbinomiale = new THF1("fbinomiale", "Curva binomiale", nbin, -0.5, nbin - 0.5);

  ///*
  for(int i = 0; i < nesp; i++){
    if(i%100 == 0){
      cout << "Sto elaborando il decadimento numero " << i << endl;
    }
    Nrest = Decadimento(ntot, deltat, ttot, alfa);
    histo->Fill(Nrest);
  }
  //*/

  for(double x=0.; x < nbin - 0.5; x+=1.){
    fpoissoniana->Fill(x, Ntot*Poisson(x, beta, tempo));
    fbinomiale->Fill(x, Ntot*Binomiale(x, nesp, beta, deltat));
  }
  
  cout << "Ho finito, plotto i grafici." << endl;
  TCanvas *chisto = new TCanvas("chisto", "Numero di decadimenti", 10, 10, 1280, 1024);
  histo->Draw();
  fpoissoniana->Draw("same");
  fbinomiale->Draw("same");
  
}

// /*
double Decadimento(int ntot, const float deltat, const float ttot, const float alfa){
  const int dimensione = (int)ttot/deltat;
  const int numeroiniziale = ntot;
  
  for(int i = 0; i < dimensione; i++){
    int Ndec = 0;
    
    for(int j = 0; j < ntot; j++){
      if(gRandom->Rndm() <= alfa*deltat)
	Ndec++;
    }
    
    ntot = ntot - Ndec;
  }

  return (double)numeroiniziale - ntot;
}
// */

// /*
double Binomiale(const int nesperimenti, const int nintervalli, float beta, float tempo){
  probabilita = beta*tempo;
  return Exp(LnGamma(nintervalli + 1) - LnGamma(nesperimenti + 1) - LnGamma(nintervalli - nesperimenti + 1) + nesperimenti*Log(probabilita) + nesperimenti*Log(nintervalli - probabilita));
}
// */

// /*
double Poisson(const int nrestanti, float beta, float tempo){
  probabilita = beta*tempo;
  return Exp(nrestanti * Log(probabilita) - probabilita - LnGamma(nrestanti + 1));
}
// */

// /*
double LogGamma(){
  return 1.;
}
// */
