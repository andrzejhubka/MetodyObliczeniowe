#include <iostream>
#include <cfloat>
#include <cmath>
#include <iomanip>

// kryteria zakonczenia iteracji
constexpr long double TOL_X = 0.1;
constexpr long double TOL_Y = 0.1;
constexpr long double MAX_N = 200;

// funkcje i ich pochodne
long double fun1(long double x);
long double fun1_dfdx(long double x);

long double fun2(long double x);
long double fun2_dfdx(long double x);


// realiazcja metod rozwiazywan pojedynczych rownan nieliniowych
long double solver_picard(long double x);
long double solver_bisection(long double x);
long double solver_gauss(long double x);
long double solver_secant(long double x);

int main()
{
    // picard
    long double x_picard = 1.l;
    for (int i=0; i < MAX_N; i++)
    {
        x_picard = solver_picard(x_picard);
    }
    std::cout <<std::setprecision(40)<< x_picard << std::endl;

    // bisekcja
    long double bisection_a = -100;
    long double bisection_b = 100;
    long double bisection_c = 0;
    for (int i=0; i < MAX_N; i++)
    {
        std::cout<<"BIsection A:" <<bisection_a << std::endl;
        if ((fun1(bisection_a) >= 0 && fun1(bisection_b) < 0)
            || (fun1(bisection_a) <= 0 && fun1(bisection_b) > 0))
        {
            // gdy przedzial [a, b] zawiera rozwiazanie
            bisection_c = bisection_b;
            bisection_b = (bisection_a + bisection_c) / 2;
        }
        else if ((fun1(bisection_b) >= 0 && fun1(bisection_c) < 0)
            || (fun1(bisection_b) <= 0 && fun1(bisection_c) > 0))
        {
            // gdy przedzial [b, c ] zawiera rozwiazanie
            bisection_a = bisection_b;
            bisection_b = (bisection_a + bisection_c) / 2;
        }
        else
        {
            // brak rozwiazan
            std::cout << "Brak rozwiazan dla bisekcji"<<std::endl;
        }
    }
    std::cout << "bisekcja: "<< (bisection_c - bisection_a)/2 << std::endl;

    // metoda newtona
    long double x_newton = 1.0l;
    for (int i=0; i < MAX_N; i++)
    {
        std::cout<<"x_newton:"<<x_newton << std::endl;
        x_newton = x_newton - (fun1(x_newton) / fun1_dfdx(x_newton));
    }
    std::cout << "Newton: "<< x_newton<< std::endl;

    // metoda siecznych
    long double x_2_sieczne = 1;
    long double x_1_sieczne = 2;
    long double x_0_sieczne = 3;

    for (int i=0; i < MAX_N; i++)
    {
        x_2_sieczne = x_1_sieczne - fun1(x_1_sieczne) / ((fun1(x_1_sieczne)-fun1(x_0_sieczne))/(x_1_sieczne-x_0_sieczne));
        x_0_sieczne = x_1_sieczne;
        x_1_sieczne = x_2_sieczne;
        std::cout << "Sieczne: "<< x_2_sieczne << std::endl;
    }
    std::cout << "Sieczne: "<< x_2_sieczne << std::endl;
    return 0;
}

long double solver_picard(long double x)
{
    return 1 - 0.5l * std::tanh(x);
}


long double fun1(long double x)
{
    return std::tanh(x) + 2 * (x - 1);
}
long double fun1_dfdx(long double x)
{
    return (1 / (std::cosh(x) * std::cosh(x))) + 2;
}
long double fun2(long double x)
{
    ;
}
long double fun2_dfdx(long double x)
{
    ;
}
