void letturaFile(){
  TFile F("kinem.root");
  F.ls();
  TH1F *m = (TH1F*)F.Get("hmul");
  m->SetDirectory(0); //per prendere l'ownership dell'oggetto, altrimenti muore alla chiusura di F
  F.Close;
  int pippo = (int)m->GetRandom()+0.5; //estrae numeri casuali con distribuzione quella del'istogramma, +.5 per non avere 0
  m->Draw("pe");
}

//limitarsi alla regione di pseudorapidità tra -2 e 2 per avere una distribuzione piatta, lo fa manipist.C
// b1 e b2 sono le celle che contengono i valori +-2. di pseudorapidità
// j++: usa j e poi lo incrementa
