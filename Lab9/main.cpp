#include <iostream>
#include <vector>
#include <cmath>
#include <functional>
#include <iomanip>
#include <fstream>

// Parametry równania
const double a = 0.0l;
const double b = 1.0l;
const double UA = 2.0;
const double UB = -2.0;

struct dataPoint
{
    double x;
    double u_analitycznie;
    double u_strzaly;
    double u_thomas;
    double blad_thomas;
    double blad_strzaly;
};

double u_analitycznie(double x)
{
    double sqrt5 = std::sqrt(5.0l);
    return -(
        9.l
        - 95.l * std::exp((-1.l - sqrt5) * (-1.l + x))
        + 55.l * std::exp((-1.l + sqrt5) * x)
        + 95.l * std::exp(1.l + sqrt5 + (-1.l + sqrt5) * x)
        - 55.l * std::exp(2.l * sqrt5 - (1.l + sqrt5) * x)
        + 2.l * x * (6.l + x * (3.l + 2.l * x))
        - std::exp(2.l * sqrt5) * (9.l + 2.l * x * (6.l + x * (3.l + 2.l * x)))
    ) * (-1.l + 1.l / std::tanh(sqrt5)) / 64.0l;
}

double metoda_strzalow(double p, double dx, std::vector<dataPoint> &data)
{
    double u2;
    double u1;
    double u0;
    double x = a + 2.l*dx;
    int idx = 2.l;

    u0 = 2.l;
    u1 = u0 + dx * p;

    data[0].u_strzaly = u0;
    data[0].blad_strzaly = fabs(u0 - u_analitycznie(a));

    data[1].u_strzaly = u1;
    data[1].blad_strzaly = fabs(u1 - u_analitycznie(a + dx));

    while (x <= b + 1e-12 )
    {
        double A = 1.0l/(dx*dx) + 1.0l/dx;
        double B = -2.0l/(dx*dx) - 4.0l;
        double C = 1.0l/(dx*dx) - 1.0l/dx;
        double F = -0.5l * x * x * x;

        u2 = (F - B * u1 - C * u0) / A;

        data[idx].u_strzaly = u2;
        data[idx].blad_strzaly = fabs(u2 - data[idx].u_analitycznie);
        idx++;

        u0 = u1;
        u1 = u2;
        x += dx;

    }

    return u1;
}

void thomas_algorithm(const std::vector<double>& lower,
                      std::vector<double>& main,
                      const std::vector<double>& upper,
                      std::vector<double>& rhs)
{
    size_t n = main.size();
    for (size_t i = 1; i < n; ++i)
    {
        double m = lower[i-1]/main[i-1];
        main[i] -= m * upper[i-1];
        rhs[i] -= m * rhs[i-1];
    }

    rhs[n-1] /= main[n-1];
    for (int i = n-2; i >= 0; --i)
    {
        rhs[i] = (rhs[i] - upper[i] * rhs[i+1]) / main[i];
    }
}

void thomas_method(double h, const std::string& filename, std::vector<dataPoint>& data)
{
    int N = static_cast<int>((b - a)/h);
    std::vector<double> x(N+1), U(N+1);
    std::vector<double> lower(N), main(N+1), upper(N), rhs(N+1);

    // Ustaw siatkę i układ równań
    for (int i = 0; i <= N; ++i)
        x[i] = a + i*h;

    for (int i = 1; i < N; ++i) {
        lower[i] = 1.0/(h*h) - 1.0/h;
        main[i]  = -2.0/(h*h) - 4.0;
        upper[i] = 1.0/(h*h) + 1.0/h;
        rhs[i]   = -0.5 * x[i] * x[i] * x[i];
    }
    // Warunki brzegowe
    main[0] = 1.0; upper[0] = 0.0; rhs[0] = UA;
    // lower[0] = 0.0; // niepotrzebne, bo nieużywane
    main[N] = 1.0; rhs[N] = UB;
    // upper[N] i lower[N] NIE ISTNIEJĄ

    // Algorytm Thomasa
    // Eliminacja w przód
    for (int i = 1; i <= N; ++i)
    {
        double m = lower[i] / main[i-1];
        main[i] -= m * upper[i-1];
        rhs[i]  -= m * rhs[i-1];
    }
    // Rozwiązanie wstecz
    U[N] = rhs[N] / main[N];
    for (int i = N-1; i >= 0; --i)
    {
        U[i] = (rhs[i] - upper[i] * U[i+1]) / main[i];
    }

    // Wypełnij dane do dataPoint
    for (int i = 0; i <= N; ++i)
    {
        data[i].u_thomas = U[i];
        data[i].blad_thomas = std::abs(U[i] - data[i].u_analitycznie);
    }
}

int main()
{
    std::vector<double> dx_values = {0.2, 0.1421, 0.1010, 0.0718, 0.0510, 0.0363, 0.0258, 0.0183, 0.0130, 0.0093, 0.0066, 0.0047, 0.0033, 0.0024, 0.0017, 0.0012, 0.00085, 0.00060, 0.00043, 0.00030, 0.00022, 0.00015, 0.00011};
    double p1 = -10.0;
    double p2 = 10.0;


    // OTWARCIE PLIKU
    std::ofstream file("wyniki.csv");
    if (!file.is_open())
    {
        std::cerr << "Nie udało się otworzyć pliku do zapisu!\n";
        return 1;
    }
    file << "dx,x,analityczne,strzaly,strzaly_blad,thomas,thomas_blad\n";

    for (auto dx : dx_values)
    {
        int n = static_cast<int>(1/dx)+1;
        std::vector<dataPoint> data(n);
        for (int i = 0; i < n; i++)
        {
            data[i].x = i*dx;
            data[i].u_analitycznie = u_analitycznie(data[i].x);
        }



        double U1 = metoda_strzalow(p1, dx, data);
        double U2 = metoda_strzalow(p2, dx, data);

        double p_opt = p1 + (p2 - p1)*(UB - U1)/(U2 - U1);

        std::cout << "\np: " << std::setprecision(10) << p_opt << "\n\n";
        metoda_strzalow(p_opt, dx, data);


        // Metoda Thomasa
        thomas_method(dx, "thomas_results.txt", data);



        for (const auto& point : data)
        {
            file << std::fixed << std::setprecision(16)
                 << dx << ","
                 << point.x << "," << point.u_analitycznie << ","
                 << point.u_strzaly << "," << point.blad_strzaly << ","
                 << point.u_thomas << "," << point.blad_thomas << std::endl;
        }
    }
    file.close();
    std::cout << "Wyniki zapisane do pliku wyniki.csv\n";
}