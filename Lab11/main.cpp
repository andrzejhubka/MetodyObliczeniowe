#include <iostream>
#include "dependencies/calerf.h"
#include <math.h>
#include <vector>
#include <fstream>
#include <iomanip>

struct dataPoint
{
    long double x;
    long double t;
    long double U;
    long double kmb;
};

long double U(const long double x, const long double t, const long double D)
{
    if (t <= 0) return 1.0L;  // Warunek początkowy
    long double z = x / (2.0L * sqrt(D * t));
    return calerfpack::erf_LD(z);
}

void KMB(std::vector<dataPoint> &data, long double L, int K, int I)
{
    for (int k=1; k<K; k++)
    {
        for (int i=1; i<I; i++)
        {
            data[k*I + i].kmb = L*data[(k-1)*I + i-1].kmb + (1.l - 2.l*L)*data[(k-1)*I + i].kmb + L*data[(k-1)*I + i+1].kmb;
        }
    }
}
int main()
{
    // metoda bezposrednia
    {
        long double D = 1.l;
        long double dt = 0.00004l;
        long double h = 0.01l;
        long double tMax = 1.l;
        long double xMax = 1;

        // generowanie siatki
        const int I = int(xMax / h)+1;
        const int K = int(tMax / dt)+1;
        std::vector<dataPoint> dataPoints(I*K);
        // dataPoints[numerWiersza * I + numerKolomny ]

        // nadawanie punktom wspolrzednych x,t
        for (int k = 0; k < K; k++)
        {
            for (int i = 0; i <= I; i++)
            {
                dataPoints[k*I + i].x = i*h;
                dataPoints[k*I + i].t = k*dt;
            }
        }

        // warunek poczatkowy
        for (int i = 0; i < I; i++)
        {
            dataPoints[0 * I + i].U = 1;
            dataPoints[0 * I + i].kmb = 1;
        }

        // warunki brzegowe
        for (int k=0; k<K; k++)
        {
            dataPoints[k*I].U = 0;
            dataPoints[k*I + I-1].U = 1;
        }
        // obliczanie wartosci numerycznych
        for (auto& dataPoint : dataPoints)
        {
            dataPoint.U = U(dataPoint.x, dataPoint.t, D);
        }

        // obliczanie metody KMB
        KMB(dataPoints, 0.4L, K, I);

        // zapisywanie punktow
        std::ofstream file("wyniki.csv");
        if (!file.is_open())
        {
            std::cerr << "Nie udało się otworzyć pliku do zapisu!\n";
            return 1;
        }
        file << "t,x,U,kmb\n";

        for (auto dataPoint : dataPoints)
        {
            file << std::fixed << std::setprecision(16)
                 << dataPoint.t << "," << dataPoint.x << ","<<dataPoint.U <<","<<
                     dataPoint.kmb<< "\n";
        }
        file.close();
        std::cout << "Wyniki zapisane do pliku wyniki.csv\n";
    }

    // metoda posrednia
    {
        ;
    }
    return 0;
}