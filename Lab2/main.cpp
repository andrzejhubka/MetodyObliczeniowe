#include <cmath>
#include <iostream>
#include <math.h>
#include <fstream>
#include <vector>
#include <filesystem>

#define LOAD_PATH "dane.txt"
#define SAVE_PATH "wyniki.txt"

#define SHOW_RESULTS_CMD 1
#define SET_SMALL_ERROR_TO_PRECISSION 1

struct DataPoint
{
  // dostarczone wraz z zadaniem
  long double log10x_dokladny;
  long double x;
  long double y;

  // obliczone w tym programie
  long double f1;
  long double f1_blad_wzgledny;
  long double f2;
  long double f2_blad_wzgledny;
};

// FUNCKJA Z DUZYM BLEDEM WZGLEDNYM
long double fun1(long double x)
{
  return x*x*x / (6.L * (sinh(x) - x));
}

// FUNCKJA Z BLEDEM WZGLEDNYM NA POZIOMIE BLEDU REPREZENTACJI
long double fun2_denominator(long double x)
{
  return x*x*x/6.L + x*x*x*x*x/120.L + x*x*x*x*x*x*x/5040.L + x*x*x*x*x*x*x*x*x/362880.L + x*x*x*x*x*x*x*x*x*x*x/3628800.L + x*x*x*x*x*x*x*x*x*x*x*x*x/39916800.L;
}
long double fun2 (long double x)
{
  if (x < 0.06)
  {
    // obliczamy wartosc sinh(x) - x jako szereg taylora
    return x*x*x / (6.L * fun2_denominator(x));
  }
  else
  {
    return x*x*x / (6.L * (sinh(x) - x));
  }
}

// LADOWANIE DANYCH Z PLIKU I OBLICZANIE WARTOSCI FUNKCJI
int load_data(std::vector<DataPoint> &data)
{
  // otwarcie pliku
  std::ifstream file(LOAD_PATH);
  if (!file.is_open())
  {
    return -1;
  }

  // pomin pierwsze 3 linie
  for (int i = 0; i < 3;  i++)
  {
    std::string temp;
    std::getline(file, temp);
  }

  // pobieranie wartosci
  long double value;
  std::string line;
  DataPoint container;

  while (std::getline(file, line))
  {
    std::istringstream iss(line);

    // wczytanie dostarczonych danych
    iss >> container.log10x_dokladny;
    iss >> container.x;
    iss >> container.y;

    // obliczenie wartosci funkcji f1
    container.f1 = fun1(container.x);
    container.f1_blad_wzgledny = fabsl((container.y - container.f1) / (container.y + 1e-20L));

    // obliczenie wartosci funkcji f2
    container.f2= fun2(container.x);
    container.f2_blad_wzgledny = fabsl((container.y - container.f2) /(container.y + 1e-20L));

    if (SET_SMALL_ERROR_TO_PRECISSION)
    {
      // gdy bledy wzgledne sa bardzo male, dla uproszcznia zamieniamy je na precyzje arytmetyki
      if (container.f1_blad_wzgledny < pow(10, -16))
      {
        container.f1_blad_wzgledny = pow(10, -16);
      }
      if (container.f2_blad_wzgledny < pow(10, -16))
      {
        container.f2_blad_wzgledny = pow(10, -16);
      }
    }

    data.emplace_back(container);
  }
  file.close();
  return 0;
}

// ZAPISYWANIE WYNIKOW W PLIKU
int save_data(std::vector<DataPoint> &data)
{
  std::ofstream file(SAVE_PATH, std::ios::out | std::ios::trunc);
  if (!file.is_open())
  {
    return -1; // Kod błędu
  }
  // zapisz oznaczenia
  file << "======================================================================\n";
  file << "log10(x) x f(x)=(x^3)/6/(sinh(x)-x)  f1(x)  f1_blad_wzgledny f2(x) f2_blad_wzgledny\n";
  file << "======================================================================\n";

  // Zapisz dane z wektoral
  for (const auto &point : data)
  {
    file <<std::setprecision(36)<<point.log10x_dokladny << " " << point.x <<" "<<point.y <<" "<<point.f1<<" "<<log10(point.f1_blad_wzgledny)<<" "<<point.f2<<" "<<log10(point.f2_blad_wzgledny)<<"\n";
  }

  file.close(); // Zamknij plik
  return 0; // Sukces
}

// GLOWNA METODA
int main()
{
  // przechoowanie zmiennej wejsciowej X
  std::vector<DataPoint> data;

  // pobranie danych z pliku i wykonanie obliczen
  if (load_data(data) != 0)
  {
    std::cout << "Blad podczas ladowania danych z pliku" << std::endl;
    exit(-1);
  }

  // wyswietlenie wynikow w konsoli
  if (SHOW_RESULTS_CMD)
  {
    // test wektora
    for (auto element  : data)
    {
      std::cout << "----------" << std::endl;
      std::cout <<std::setprecision(30)
      << "Log(x):" << element.log10x_dokladny << std::endl
      << "X: " << element.x <<std::endl
      << "Y_dokladne: " << element.f1 << std::endl
      << "Y:" << element.y << std::endl;
    }
  }

  // zapisanie danych do pliku tekstowego
  if (save_data(data) != 0)
  {
    std::cout << "Blad zapisywania wynikow do pliku" << std::endl;
    exit(-1);
  }

  // skrypt rysujacy obrazki
  system("python3 SkryptWykres.py");



  return 0;
}
