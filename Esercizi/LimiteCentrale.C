#if !defined(__CINT__) || defined(__MAKECINT__)
#include <Riostream.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TH1F.h>
#include <TRandom.h>

#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH
#endif

float gener(){
  float x = gRandom->Rndm();

  if(x <= 0.5)
    return x*(0.2 - 0.)*2;
  else
    return x*(1. - 0.8)*2 + 0.8 - (1. - 0.8);
}

void LimiteCentrale(){
  const int elementi = 200000;

  TCanvas *c1 = new TCanvas("c1","Numeri da sommare", 50, 10, 600, 400);
  TH1F *histo = new TH1F("limitecentrale", "Termini da sommare", 100, 0., 1.);
  
  TCanvas *c2 = new TCanvas("c2","Somma di due numeri", 675, 10, 600, 400);
  TH1F *histo2 = new TH1F("somma2", "Somma di due termini", 100, 0., 2.);
  
  TCanvas *c3 = new TCanvas("c3","Somma di tre numeri", 50, 460, 600, 400);
  TH1F *histo3 = new TH1F("somma3", "Somma di tre termini", 100, 0., 3.);
  
  TCanvas *c4 = new TCanvas("c4","Somma di dieci numeri", 675, 460, 600, 400);
  TH1F *histo4 = new TH1F("somma4", "Somma di dieci termini", 100, 0., 10.);
  
  TCanvas *c5 = new TCanvas("c5","Somma di trecento numeri", 1300, 230, 600, 400);
  TH1F *histo5 = new TH1F("somma5", "Somma di trecento termini", 100, 0., 300.);
  
  for(int i=0; i < elementi; i++){
    float y = gener();
    histo->Fill(y);

    y += gener();
    histo2->Fill(y);

    y += gener();
    histo3->Fill(y);

    for(int j=0; j<7; j++){
      y += gener();
    }
    histo4->Fill(y);

    for(int k=0; k<290; k++){
      y += gener();
    }
    histo5->Fill(y);
  }

  c1->cd();
  histo->Draw();
  c2->cd();
  histo2->Draw();
  c3->cd();
  histo3->Draw();
  c4->cd();
  histo4->Draw();
  c5->cd();
  histo5->Draw();
}
