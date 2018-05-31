#if !defined(__CINT__) || defined(__MAKECINT__)
#include <Riostream.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TH1F.h>
#include <TF1.h>
#include <TFile.h>
#include <TRandom.h>
#include <TVirtualFitter.h>

#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH
#endif

void Decadimento(int ntot = 50000, const float deltat = 1., const float ttot = 300., const float alfa = 0.05){

  const int dimensione = (int)ttot/deltat;
  const int tempotot = dimensione*deltat;
  //const int tempotot = ttot;
  const float Ntot = ntot;
  
  TH1F *istogramma = new TH1F("istogramma", "Simulazione di un decadimento", dimensione, 0., tempotot);
  
  for(int i = 0; i < dimensione; i++){
    int Ndec = 0;
    
    if(i != 0){
      for(int j = 1; j < ntot; j++){
	if(gRandom->Rndm() <= alfa*deltat)
	  Ndec++;
      }
    }
    
    ntot = ntot - Ndec;
    istogramma->TH1F::AddBinContent(i+1, (double)ntot);
  }

  TCanvas *chisto = new TCanvas("chisto", "Simulazione di un decadimento", 10, 10, 1280, 1024);
  chisto->cd();
  istogramma->Draw();

  TF1 *fhisto = new TF1("fhisto", "[1]*TMath::Exp(-[0]*x)");
  TVirtualFitter::SetMaxIterations(100000);
  fhisto->SetParameter(0, alfa);
  fhisto->SetParameter(1, Ntot);
  fhisto->SetLineColor(2);

  istogramma->Fit(fhisto, "M+");
  cout << "Chi^2:" << fhisto->GetChisquare() << ", number of DoF: " << fhisto->GetNDF() << " (Probability: " << fhisto->GetProb() << ")." << endl;
  cout << "--------------------------------------------------------------------------------------------------------" << endl;
}
