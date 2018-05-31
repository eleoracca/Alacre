#include "Colpi.h"
#include "TMath.h"

ClassImp(Colpi)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~ Implementazione della classe Colpi                      ~
  ~ Autori: Racca Eleonora - eleonora.racca288@edu.unito.it ~
  ~         Sauda Cristina - cristina.sauda@edu.unito.it    ~
  ~ Ultima modifica: 15/11/2017                             ~
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// Costruttori
Colpi::Colpi(): TObject(),
                dmX(0.),
                dmY(0.),
                dmZ(0.)
{}

Colpi::Colpi(double x, double y, double z): TObject(),
					    dmX(x),
					    dmY(y),
					    dmZ(z)
{}

// Distruttori
Colpi::~Colpi()
{}

// Getter
double Colpi::GetX()
{
  return dmX;
}

double Colpi::GetY()
{
  return dmY;
}

double Colpi::GetZ()
{
  return dmZ;
}

// Setter
void Colpi::SetX(double x)
{
  dmx = x;
}

void Colpi::SetY(double y)
{
  dmY = y;
}

void Colpi::SetZ(double z)
{
  dmZ = z;
}

// Cambi di coordinate
void Colpi::CartesianeCilindriche(Colpi hit)
{
  double x = hit.GetX();
  double y = hit.GetY();

  double rho = TMath::Sqrt(x*x + y*y);
  double teta = TMath::Atan(y / x);

  Colpi::SetX(rho);
  Colpi::SetY(teta);
  
}

void Colpi::CilindricheCartesiane(Colpi hit)
{
  double rho = hit.GetX();
  double teta = hit.GetY();

  double x = rho * TMath::Cos(teta);
  double y = rho * TMath::Sen(teta);

  Colpi::SetX(x);
  Colpi::SetY(y);  
}

void Colpi::Rotazione(double teta, double phi, double tetaprimo, double phiprimo, double *vettorecoordinate)
{
  // Matrice del cambio di coordinate
  double matrice[3][3];

  matrice[0][0] = -TMath::Sin(phi);
  matrice[1][0] = TMath::Cos(phi);
  matrice[2][0] = 0.;
  matrice[0][1] = -TMath::Cos(phi) * TMath::Cos(teta);
  matrice[1][1] = -TMath::Sin(phi) * TMath::Cos(teta);
  matrice[2][1] = -TMath::Sin(teta);
  matrice[0][2] = TMath::Sin(teta) * TMath::Cos(phi);
  matrice[1][2] = TMath::Sin(teta) * TMath::Sin(phi);
  matrice[2][2] = TMath::Cos(teta);

  // Vettore delle coordinate primate
  double vettoreprimate[3];

  vettoreprimate[0] = TMath::Sin(tetaprimo) * TMath::Cos(phiprimo);
  vettoreprimate[1] = TMath::Sin(tetaprimo) * TMath::Sin(phiprimo);
  vettoreprimate[2] = TMath::Cos(tetaprimo);

  // Cambio di coordinate
  for(int i = 0; i < 3; i++){
    vettorecoordinate[i] = 0.;
    
    for(int j = 0; j < 3; j++){
      vettorecoordinate[i] = vettorecoordinate[i] + matrice[i][j] * vettoreprimate[j];
    }
  }
}
