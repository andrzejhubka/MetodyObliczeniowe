#include <vector>
#include <math.h>
#include "dependencies/calerf.h"
#include "utilities.h"

void thomasAlgorithm(std::vector<long double>& a,
                     std::vector<long double>& b,
                     std::vector<long double>& c,
                     std::vector<long double>& d,
                     std::vector<long double>& x,
                     int n) {
    // Forward elimination (eliminacja w przód)
    for (int i = 1; i < n; i++) {
        long double m = a[i] / b[i-1];
        b[i] = b[i] - m * c[i-1];
        d[i] = d[i] - m * d[i-1];
    }

    // Back substitution (podstawianie wsteczne)
    x[n-1] = d[n-1] / b[n-1];
    for (int i = n-2; i >= 0; i--) {
        x[i] = (d[i] - c[i] * x[i+1]) / b[i];
    }
}

long double U(const long double x, const long double t, const long double D)
{
    if (t <= 0) return 1.0L;  // Warunek początkowy
    long double z = x / (2.0L * sqrt(D * t));
    return calerfpack::erf_LD(z);
}