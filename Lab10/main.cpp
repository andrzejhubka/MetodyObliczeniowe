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

void BME(long double y0, long double dt, long double tf, std::vector<dataPoint> &data)
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

void PME(long double y0, long double dt, long double tf, std::vector<dataPoint> &data)
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

void PMT(long double y0, long double dt, long double tf, std::vector<dataPoint> &data)
{
    long double t = 0.L;
    long double y = y0;
    data[0].PMT_result = y;
    data[0].PMT_blad_bezwzgledny = fabsl(y - data[0].y_analitical);

    for (int i = 1; i < data.size(); i++)
    {
        long double t_prev = t;
        t += dt;
        long double a_prev = (100.L * t_prev + 10.L) / (t_prev + 1.L);
        long double a_now  = (100.L * t + 10.L) / (t + 1.L);

        // Poprawiona formuła trapezów:
        y = (y - dt/2.L * a_prev * (y - 1.L) + dt/2.L * a_now) / (1.L + dt/2.L * a_now);

        data[i].PMT_result = y;
        data[i].PMT_blad_bezwzgledny = fabsl(y - data[i].y_analitical);
    }
}

int main()
{
    std::vector<long double> dt_values = {0.2L,0.1L,0.08L,0.05L,0.04L,0.03L,0.025L,0.02L,0.015L,0.01L,0.0075L,0.005L,0.0025L,0.001L,0.0005L,0.0002L,0.0001L,0.00005L,0.00002L,0.00001L};
    long double Y0 = 2.l, tf = 1.l;

    // OTWARCIE PLIKU
    std::ofstream file("wyniki.csv");
    if (!file.is_open())
    {
        std::cerr << "Nie udało się otworzyć pliku do zapisu!\n";
        return 1;
    }
    file << "dt,t,analityczne,BME,BME_blad,PME,PME_blad,PMT,PMT_blad\n";

    for (auto dt : dt_values)
    {
        int n = static_cast<int>(tf/dt)+1;
        std::vector<dataPoint> data(n);

        for (int i = 0; i < n; i++)
        {
            data[i].t = i*dt;
            data[i].y_analitical = y_analytical(data[i].t);
        }

        BME(Y0, dt, tf, data);
        PME(Y0, dt, tf, data);
        PMT(Y0, dt, tf, data);

        for (const auto& point : data)
        {
            file << std::fixed << std::setprecision(16)
                 << dt << ","
                 << point.t << "," << point.y_analitical << ","
                 << point.BME_result << "," << point.BME_blad_bezwzgledny << ","
                 << point.PME_result << "," << point.PME_blad_bezwzgledny << ","
                 << point.PMT_result << "," << point.PMT_blad_bezwzgledny << "\n";
        }
    }

    file.close();
    std::cout << "Wyniki zapisane do pliku wyniki.csv\n";

    return 0;

}
