//
// Created by andrzej on 4/1/25.
//
#include <iostream>
#include <cmath>
#include <vector>

constexpr int MAX_ITERATIONS = 200;
constexpr long double TOL_X = 1e-10;
constexpr long double TOL_Y = 1e-10;

// obliczanie wartosci wektora funkcij F dla zadanych x,y,z
void calculate_F(const long double (&X)[3], long double (&F)[3]);

// obliczanie wartosci macierzy jacobiego
void calculate_J(const long double (&X)[3], long double (&J)[3][3]);

// rozwiazywanie ukladu rownan, ktorego wynikkiem jest wektor który nalezy odjac od obecnego rozwiazania, zeby przyblizyc sie do miejsca zerowego funkcji.
void solve_linear(long double (&D)[3], long double (&F)[3], long double (&J)[3][3]);

int main()
{
  // wektor przyblizonych wynikow X
  long double X[3] = {1.l, 2.l, 3.l};

  // wektor wartosci funkcji F(X)
  long double F[3];

  // macierz jacobiego J
  long double J[3][3];

  // wektor poprawy wyniku D
  long double D[3];

  // glowna iteracja
  for (int i=0; i<MAX_ITERATIONS; i++)
  {
    // obliczenie wartosci funkcji w wektorze F i macierzy jacobiego
    calculate_F(X, F);
    calculate_J(X, J);

    // obliczenie wektora poprawek
    solve_linear(D, F, J);

    // zastosowanie poprawek
    X[0] -= D[0];
    X[1] -= D[1];
    X[2] -= D[2];
  }

  std::cout << "X: "<<X[0] << std::endl;
  std::cout << "Y: "<<X[1] << std::endl;
  std::cout << "Z: "<<X[2] << std::endl;


  return 0;
}

void calculate_J(const long double (&X)[3], long double (&J)[3][3])
{
  J[0][0] = 2 * X[0];
  J[0][1] = 2 * X[1];
  J[0][2] = 2 * X[2];

  J[1][0] = 2 * X[0];
  J[1][1] = X[1];
  J[1][2] = 0;

  J[2][0] = X[1];
  J[2][1] = X[0];
  J[2][2] = 0;
}

void calculate_F(const long double (&X)[3], long double (&F)[3])
{
  F[0] = X[0]*X[0] + X[1]*X[1] + X[2]*X[2] -4l;
  F[1] = X[0]*X[0] + X[1]*X[1]/2l - 1l;
  F[2] = X[0]*X[1] - 0.5l;
}

void solve_linear(long double (&D)[3], long double (&F)[3], long double (&J)[3][3])
{
  //TODO
  // Tworzymy kopie robocze, aby nie modyfikować oryginalnych danych
  long double J_copy[3][3];
  long double F_copy[3];

  // Kopiujemy dane
  for (int i = 0; i < 3; i++) {
    F_copy[i] = F[i]; // Znak minus, bo rozwiązujemy J*D = -F
    for (int j = 0; j < 3; j++) {
      J_copy[i][j] = J[i][j];
    }
  }

  // Eliminacja Gaussa z częściowym wyborem elementu głównego
  for (int i = 0; i < 3; i++) {
    // Znajdź wiersz z maksymalnym elementem w kolumnie i
    int maxRow = i;
    long double maxVal = std::abs(J_copy[i][i]);
    for (int k = i + 1; k < 3; k++) {
      if (std::abs(J_copy[k][i]) > maxVal) {
        maxVal = std::abs(J_copy[k][i]);
        maxRow = k;
      }
    }

    // Zamień wiersze jeśli potrzeba
    if (maxRow != i) {
      for (int j = 0; j < 3; j++) {
        std::swap(J_copy[i][j], J_copy[maxRow][j]);
      }
      std::swap(F_copy[i], F_copy[maxRow]);
    }

    // Eliminacja
    for (int k = i + 1; k < 3; k++) {
      long double factor = J_copy[k][i] / J_copy[i][i];
      F_copy[k] -= factor * F_copy[i];
      for (int j = i; j < 3; j++) {
        J_copy[k][j] -= factor * J_copy[i][j];
      }
    }
  }

  // Podstawienie wsteczne
  D[2] = F_copy[2] / J_copy[2][2];
  D[1] = (F_copy[1] - J_copy[1][2] * D[2]) / J_copy[1][1];
  D[0] = (F_copy[0] - J_copy[0][2] * D[2] - J_copy[0][1] * D[1]) / J_copy[0][0];
}