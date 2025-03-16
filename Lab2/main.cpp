#include <cmath>
#include <iostream>
#include <math.h>
#include <fstream>
#include <vector>
#include <filesystem>

#define LOAD_PATH "dane.txt"
#define SAVE_PATH "wyniki.txt"

struct DataPoint
{
  long double log10x_dokladny;
  long double x;
  long double y_dokladny;
  long double f1;
  long double f1_blad_wzgledny;

  // dla inicjalizacji
  DataPoint(long double x, long double y, long double y_dokladne, long double log10x_dokladny, long double f1_blad_wzgledny):
  x(x), y_dokladny(y), f1(y_dokladne), log10x_dokladny(log10x_dokladny), f1_blad_wzgledny(f1_blad_wzgledny)
  {
    ;
  };
};

// FUNCKJA Z DUZYM BLEDEM WZGLEDNYM
long double fun1(long double x)
{
  return x*x*x / (6.L * (sinh(x) - x));
}

// FUNCKJA Z BLEDEM WZGLEDNYM NA POZIOMIE BLEDU REPREZENTACJI
long double fun2(long double x)
{
  return pow(x,2) / (6.L * (sinh(x) - x));
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
  DataPoint container(0,0,0,0, 0);
  while (std::getline(file, line))
  {
    std::istringstream iss(line);
    iss >> container.log10x_dokladny;
    iss >> container.x;
    iss >> container.f1;
    container.y_dokladny = fun1(container.x);
    container.f1_blad_wzgledny = log10(abs((container.y_dokladny - container.f1) / container.y_dokladny));

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
  file << "log10(x)      x       f(x)=(x^3)/6/(sinh(x)-x)  f1(x)  blad_wzgledny \n";
  file << "======================================================================\n";

  // Zapisz dane z wektora
  for (const auto &point : data)
  {
    file <<std::setprecision(20)<<point.log10x_dokladny << " " << point.x <<" "<<point.y_dokladny <<" "<<point.f1<<" "<<point.f1_blad_wzgledny<<"\n";
  }

  file.close(); // Zamknij plik
  return 0; // Sukces
}
int main()
{
  // przechoowuje zmienne wejsciowe X
  std::vector<DataPoint> data;

  // pobranie danych z pliku i wykonanie obliczen
  if (load_data(data) != 0)
  {
    std::cout << "Blad podczas ladowania danych z pliku" << std::endl;
    exit(-1);
  }

  // test wektora
  for (auto element  : data)
  {
    std::cout << "----------" << std::endl;
    std::cout <<std::setprecision(30)
    << "Log(x):" << element.log10x_dokladny << std::endl
    << "X: " << element.x <<std::endl
    << "Y_dokladne: " << element.f1 << std::endl
    << "Y:" << element.y_dokladny << std::endl;
  }

  // zapisanie danych do pliku tekstowego
  if (save_data(data) != 0)
  {
    std::cout << "Blad zapisywania wynikow do pliku" << std::endl;
    exit(-1);
  }

  return 0;
}
