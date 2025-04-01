#include <iostream>
#include <cmath>
#include <iomanip>
#include <vector>
#include <math.h>
#include <chrono>
#include <sstream>
#include <cstdlib>

constexpr int MAX_ITERATIONS = 30;
constexpr long double TOL_X = 1e-100;
constexpr long double TOL_Y = 1e-100;

// obliczanie wartosci wektora funkcij F dla zadanych x,y,z
void calculate_F(const long double (&X)[3], long double (&F)[3]);

// obliczanie wartosci macierzy jacobiego
void calculate_J(const long double (&X)[3], long double (&J)[3][3]);

// obliczanie wyznacznika macierzy 3x3
long double calculate_determinant(long double (&D)[3][3]);

// rozwiazywanie ukladu rownan, ktorego wynikkiem jest wektor który nalezy odjac od obecnego rozwiazania, zeby przyblizyc sie do miejsca zerowego funkcji.
void solve_linear(long double (&D)[3], long double (&F)[3], long double (&J)[3][3]);

inline long double f1(long double x, long double y, long double z);
inline long double f2(long double x, long double y, long double z);
inline long double f3(long double x, long double y, long double z);

int main()
{
  // wektor przyblizonych wynikow X
  long double X[3] = {1.l, 2.l, 3.l};

  // wektor poprzednich wyników -> do obliczania estymatora bledu
  long double X_prev[3] = {1.l, 2.l, 3.l};

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

    X_prev[0] = X[0];
    X_prev[1] = X[1];
    X_prev[2] = X[2];

    // zastosowanie poprawek
    X[0] -= D[0];
    X[1] -= D[1];
    X[2] -= D[2];

    std::cout<<"Iteracja; "<< i+1<<std::endl;
    std::cout << "X: " <<std::setprecision(70) << std::fixed << X[0]<<std::endl;
    std::cout <<  "Y: "<<std::setprecision(70) << std::fixed << X[1]<<std::endl;
    std::cout <<  "Z: "<<std::setprecision(70) << std::fixed << X[2]<<std::endl;
    std::cout << "suma residuum dla rownan 1,2,3: " << f1(X[0], X[1], X[2]) + f2(X[0], X[1], X[2]) + f3(X[0], X[1], X[2]) << std::endl;
    std::cout << "suma estymatorow bledow Xn: " << abs(X_prev[0] - X[0]) + abs(X_prev[1] - X[1]) + abs(X_prev[2] - X[2]);
    std::cout << std::endl << std::endl;

    // jesli spelniono kryteria wiarygodnosci przyblizenia pierwiastka i dokladnosci wyznaczenia
    if ((abs(f1(X[0], X[1], X[2])) <= TOL_Y) &&
        (abs(f2(X[0], X[1], X[2])) <= TOL_Y)&&
         (abs(f2(X[0], X[1], X[2]) <= TOL_Y)) &&
           (abs(X_prev[0] - X[0]) <= TOL_X) &&
             (abs(X_prev[1] -X[1] <= TOL_X)) &&
               (abs(X_prev[2] -X[2] <= TOL_X)))

    {
      std::cout << "Spelniono kryteria dokladnosci wyznaczenia Xn i wiarygodnosci Xn jako pierwiastka" <<std::endl;
      break;
    }
  }




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
  // sprawdz czy macierz nie jest osobliwa
  if (abs(calculate_determinant(J)) < 1e-16)
  {
    std::cout<<"Rownanie jest osobliwe a układ równań nie jest LN";
    exit(0);
  }

  // Eliminacja Gaussa z częściowym wyborem elementu głównego
  int order[3] = {0,1,2};

  for (int i = 0; i < 2; i++)
  {
    // Znajdź wiersz z maksymalnym elementem w kolumnie i
    int maxRow = i;
    long double maxVal = std::abs(J[order[i]][i]);
    for (int k = i + 1; k < 3; k++)
    {
      if (std::abs(J[order[k]][i]) > maxVal)
      {
        maxVal = std::abs(J[order[k]][i]);
        maxRow = k;
      }
    }

    // Zamień wiersze jeśli potrzeba
    if (maxRow != i)
    {
      std::swap(order[i], order[maxRow]);
    }

    // Eliminacja
    for (int k = i + 1; k < 3; k++)
    {
      long double factor = J[order[k]][i] / J[order[i]][i];
      F[order[k]] -= factor * F[order[i]];
      J[order[k]][i] = 0;
      for (int j = i+1; j < 3; j++)
      {
        J[order[k]][j] -= factor * J[order[i]][j];
      }
    }
  }

  D[2] = F[order[2]] / J[order[2]][2];
  D[1] = (F[order[1]] - J[order[1]][2] * D[2]) / J[order[1]][1];
  D[0] = (F[order[0]] - J[order[0]][1] * D[1] - J[order[0]][2] * D[2]) / J[order[0]][0];
}

long double calculate_determinant(long double (&X)[3][3])
{
  // regula sarrusa
  const long double result = X[0][0] * X[1][1] * X[2][2] + X[0][1] * X[1][2] * X[2][0] + X[0][2]*X[1][0]*X[2][1]
          - X[0][2] * X[1][1] * X[2][0] - X[0][0] * X[1][2] * X[2][1] + X[0][1]*X[1][0]*X[2][2];

  return result;
}

inline long double f1(long double x, long double y, long double z)
{
  return x*x + y*y + z*z - 4.l;
}
inline long double f2(long double x, long double y, long double z)
{
  return x*x + y*y/2.l -1.l;
}
inline long double f3(long double x, long double y, long double z)
{
  return x*y -0.5l;
}