#include <iostream>
#include "utilities.h"
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
    long double kmb_error;
    long double nicolson;
    long double nicolson_error;
};


void KMB(std::vector<dataPoint> &data, long double L, const int K, const int I)
{

    for (int k=1; k<K; k++)
    {
        // Najpierw ustaw warunki brzegowe
        data[k*I].kmb = 0.0L;        // Lewy brzeg
        data[k*I + (I-1)].kmb = 1.0L; // Prawy brzeg

        // Obliczenia dla punktów wewnętrznych
        for (int i=1; i<I-1; i++)
        {
            auto a = data[(k-1)*I + i-1].kmb;
            auto b = data[(k-1)*I + i].kmb;
            auto c = data[(k-1)*I + i+1].kmb;
            data[k*I + i].kmb = L*data[(k-1)*I + i-1].kmb + (1.0L - 2.0L*L)*data[(k-1)*I + i].kmb + L*data[(k-1)*I + i+1].kmb;
            data[k*I + i].kmb_error = abs(data[k*I + i].kmb - data[k*I + i].U);
        }
    }
}

void Nicolson(std::vector<dataPoint> &data, long double r, int K, int I)
{
    return;
}
int main()
{
    // metoda bezposrednia
    {
        long double D = 1.l;
        long double h = 0.01l;
        long double dt = 0.00001;
        long double tMax = 1l;
        long double xMax = 6L;

        // generowanie siatki
        const int I = int(xMax / h)+1;
        const int K = int(tMax / dt)+1;
        std::vector<dataPoint> dataPoints(I*K);
        // dataPoints[numerWiersza * I + numerKolomny ]

        // nadawanie punktom wspolrzednych x,t
        for (int k = 0; k < K; k++)
        {
            for (int i = 0; i < I; i++)
            {
                dataPoints[k*I + i].x = i*h;
                dataPoints[k*I + i].t = k*dt;
            }
        }

        // obliczanie wartosci numerycznych
        for (auto& dataPoint : dataPoints)
        {
            dataPoint.U = U(dataPoint.x, dataPoint.t, D);
        }

        // warunek poczatkowy
        for (int i = 0; i < I; i++)
        {
            dataPoints[0 * I + i].U = 1;
            dataPoints[0 * I + i].kmb = 1;
        }

        // Rozwiąż konflikt w (0,0) - wybierz warunek brzegowy
        //dataPoints[0].U = 0.3411188968l;
        //dataPoints[0].kmb = 0.3411188968l;

        // Sprawdź stabilność
        long double r = D * dt / (h * h);

        // obliczanie metody KMB
        KMB(dataPoints, 0.1L, K, I);

        // obliczanie metody Crank-Nicolsona
        Nicolson(dataPoints, r, K, I);

        // zapisywanie punktow
        std::ofstream file("wyniki.csv");
        if (!file.is_open())
        {
            std::cerr << "Nie udało się otworzyć pliku do zapisu!\n";
            return 1;
        }
        file << "t,x,U,kmb,kmbError,nicolson,nicolsonError\n";

        for (auto dataPoint : dataPoints)
        {
            file << std::fixed << std::setprecision(16)
                 << dataPoint.t << "," << dataPoint.x << ","<<dataPoint.U <<","<<
                     dataPoint.kmb<< ","<<dataPoint.kmb_error<<","<<
                    dataPoint.nicolson << ","<<dataPoint.nicolson_error<<"\n";


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