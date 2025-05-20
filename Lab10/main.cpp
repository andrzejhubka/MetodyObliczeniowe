#include <math.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>

struct dataPoint
{
    long double t;
    long double y_analitical;
    long double BME_result;
    long double PME_result;
    long double PMT_result;
    long double BME_blad_bezwzgledny;
    long double PME_blad_bezwzgledny;
    long double PMT_blad_bezwzgledny;
};

long double Y0 = 2.l, dt = 0.0001l, tf = 1.l;
// GENERUJEMY TABLICE Z WYNIKAMI
int n = static_cast<int>(tf/dt);
std::vector<dataPoint> data(n);


// F z rownania RR
long double f(long double t, long double y)
{
    return -1.l * (((100.l * t + 10) / (t+1.l)) * (y-1.l));
}

// rozw analityczne
double y_analytical(long double t)
{
    return 1.l + pow((1.l + t),90.l) * exp(-100.l * t);
}

void BME(long double y0, long double dt, long double tf)
{
    long double t = 0;
    long double y = y0;
    data[0].BME_result = y;

    for (int i = 1; t < tf; i++)
    {
        t += dt;
        y = y + f(t, y) * dt;
        data[i].BME_result = y;
        data[i].BME_blad_bezwzgledny = fabs(y - y_analytical(t));
    }

}

void PME(long double y0, long double dt, long double tf)
{
    long double t = 0;
    long double y = y0;
    data[0].PME_result = y;

    for (int i = 1; t < tf; i++)
    {
        t = t + dt;
        y = (y + dt * (100.l*t + 10.l) / (t+1.l)) / (1 + dt * (100.l *t + 10.l) / (t + 1.l));
        data[i].PME_result = y;
        data[i].PME_blad_bezwzgledny = fabs(y - y_analytical(t));
    }
}

void PMT(long double y0, long double dt, long double tf)
{
    long double t = 0;
    long double y = y0;
    data[0].PMT_result = y;

    for (int i = 0; i < data.size(); i++)
    {
        t += dt;
        y = (2.l*y + dt * f(t, y) + dt * (100*t+10) / (t+1)) / (2.l + dt * (100.l*t + 10.l) / (t+1.l));
        data[i].PMT_result = y;
        data[i].PMT_blad_bezwzgledny = fabs(y - y_analytical(t));
    }
}

int main()
{
    for (int i = 0; i < n; i++)
    {
        data[i].t = i*dt;
        data[i].y_analitical = y_analytical(data[i].t);
    }

    //ZGODNIE Z OBLCIZENIAMI ABY BME BYŁA STABILNA dt < 1/50
    BME(Y0, dt, tf);

    // PME BEZWARUNKOWO STABILNA
    PME(Y0, dt, tf);

    // PMT BEZWARUNKOWO STABILNA
    PMT(Y0, dt, tf);

    // ZAPISUJEMY WYNIKI
    std::ofstream file("wyniki.csv");
    if (!file.is_open())
    {
        std::cerr << "Nie udało się otworzyć pliku do zapisu!\n";
        return 1;
    }
    file << "t,analityczne,BME,BME_blad,PME,PME_blad,PMT,PMT_blad\n";


    for (const auto& point : data)
    {
        file <<std::fixed<<std::setprecision(16)<<
            point.t << "," << point.y_analitical << ","
        << point.BME_result << ","<<point.BME_blad_bezwzgledny<<","
        << point.PME_result << "," <<point.PME_blad_bezwzgledny<<","
        << point.PMT_result <<","<< point.PMT_blad_bezwzgledny <<"\n";
    }

    file.close();
    std::cout << "Wyniki zapisane do pliku wyniki.csv\n";

    return 0;

}
