#include <iostream>
#include <cmath>
#include <iomanip>

// kryteria zakonczenia iteracji
constexpr long double TOL_X = 1E-16;
constexpr long double TOL_Y = 1E-16;
constexpr long double MAX_N = 200;

#define SHOW_F1 0
#define SHOW_F2 1
#define SHOW_PICARD 1
#define SHOW_BISECTION 1
#define SHOW_NEWTON 1
#define SHOW_SECANT 1

// realiazcja metod rozwiazywan pojedynczych rownan nieliniowych
long double solver_picard(long double x_prev,
                            long double (*defining_function)(long double),
                            long double (*function)(long double),
                            const std::string label);

long double solver_bisection(long double a,
                            long double b,
                            long double(*function)(long double),
                            std::string label);

long double solver_newton(long double x,
                            long double (*function)(long double),
                            long double (*function_derivative)(long double),
                            std::string label);

long double solver_secant(long double x0,
                            long double x1,
                            long double x2,
                            long double (*function)(long double),
                            std::string label);

int main()
{
    if (SHOW_PICARD)
    {
        if (SHOW_F1)
        {
            solver_picard(1.1,
        [](long double x){ return 1.l - std::tanh(x)/2.l; },
        [](long double x){return 2.l*x - 2.l + std::tanh(x);},
        "Metoda picarda dla F1:");
        }
        if (SHOW_F2)
        {
            solver_picard(1.1,
        [](long double x){ return std::asinh(1.l - x/4.l);},
        [](long double x){return std::asinh(1.l - x/4.l)-x;},
        "Metoda picarda dla F2:");
        }
    }

    if (SHOW_BISECTION)
    {
        if(SHOW_F1)
        {
            solver_bisection(0, 100,
        [](long double x){return std::tanh(x) + 2.l * (x - 1.l);},
        "Metoda bisekcji dla F1:");
        }
        if(SHOW_F2)
        {
            solver_bisection(0, 100,
       [](long double x){return std::asinh(1.l - x/4.l)-x;},
       "Metoda bisekcji dla F2:");
        }
    }

    if (SHOW_NEWTON)
    {
        if(SHOW_F1)
        {
            solver_newton(1.l,
        [](long double x){return 2.l*x-2.l+std::tanh(x);},
        [](long double x){return 2.l + 1/(std::cosh(x)*std::cosh(x));},
        "Metoda newtona dla F1");
        }
        if(SHOW_F2)
        {
            solver_newton(1.l,
        [](long double x){return std::asinh(1.l - x/4.l)-x;},
        [](long double x){return -1.l/4.l * 1.l/std::sqrt(x*x+1)-1;},
        "Metoda newtona dla F2");
        }
    }

    if (SHOW_SECANT)
    {
        if(SHOW_F1)
        {
            solver_secant(100,
               10,
               5,
               [](long double x){return 2.l*x - 2.l + std::tanh(x);},
               "Metoda siecznych dla F1");
        }
        if(SHOW_F2)
        {
            solver_secant(100,
                10,
                5,
                [](long double x){return std::asinh(1.l - x/4.l)-x;},
                "Metoda siecznych dla F2");
        }
    }
    return 0;
}

long double solver_picard(long double x_prev,
                            long double (*defining_function)(long double),
                            long double (*function)(long double),
                            const std::string label)
{
    long double x_current = x_prev;
    long double estymator;
    long double reziduum;
    for (int i=0; i < MAX_N; i++)
    {
        x_prev = x_current;
        x_current = defining_function(x_prev);

        estymator = std::abs(x_current-x_prev);
        reziduum = std::abs(function(x_current));

        std::cout<<"--------"<<label<<"       iteracja: "<<i<<std::endl;
        std::cout <<std::setprecision(70)<< "X:"<< x_prev << std::endl;
        std::cout <<"Estymator bledu Xn: " <<estymator<<std::endl;
        std::cout <<"Reziduum rownania: " << reziduum<<std::endl<<std::endl;;

        if ((reziduum < TOL_Y) && (estymator < TOL_X))
        {
            std::cout << "Spelniono kryteria zakonczenia iteracji"<<std::endl<<std::endl;
            break;
        }
    }

    return x_current;
}

long double solver_bisection(long double a,
                            long double b,
                            long double (*function)(long double), std::string label)
{
    long double c = 0;
    long double estymator;
    long double reziduum;
    long double x_current=0;

    for (int i=0; i < MAX_N; i++)
    {
        if ((function(a) >= 0 && function(b) < 0)
            || (function(a) <= 0 && function(b) > 0))
        {
            // gdy przedzial [a, b] zawiera rozwiazanie
            c = b;
            b = (a + c) / 2;
        }
        else if ((function(b) >= 0 && function(c) < 0)
            || (function(b) <= 0 && function(c) > 0))
        {
            // gdy przedzial [b, c ] zawiera rozwiazanie
            a = b;
            b = (a + c) / 2;
        }
        else
        {
            // brak rozwiazan
            std::cout << "Brak rozwiazan dla bisekcji"<<std::endl;
        }
        x_current = (a+c)/2.l;
        estymator = std::abs(b - a)/2;
        reziduum = std::abs(function(x_current));


        std::cout<<"--------"<<label<<"       iteracja: "<<i<<std::endl;
        std::cout<<"X:" << x_current<< std::endl;
        std::cout<<"Reziduum: " << reziduum<<std::endl;
        std::cout<<"Estymator bledu xn: " <<estymator<<std::endl<<std::endl;

        if ((estymator < TOL_X) && (reziduum < TOL_Y))
        {
            std::cout<<"Spelniono warunki zakoczenia iteracji"<<std::endl;
            break;
        }
    }
    return x_current;
}

long double solver_newton(long double x_prev,
                            long double (*function)(long double),
                            long double (*function_derivative)(long double),
                            std::string label)
                        {
                            long double estymator;
                            long double reziduum;
                            long double x_current=x_prev;

                            for (int i=0; i < MAX_N; i++)
                            {
                                x_prev = x_current;
                                x_current -= function(x_prev) / function_derivative(x_prev);

                                estymator = x_current - x_prev;
                                reziduum = std::abs(function(x_current));


                                std::cout<<"--------"<<label<<"       iteracja: "<<i<<std::endl;
                                std::cout<<"X:" << x_current<< std::endl;
                                std::cout<<"Reziduum: " << reziduum<<std::endl;
                                std::cout<<"Estymator bledu xn: " <<estymator<<std::endl<<std::endl;

                                if ((estymator < TOL_X) && (reziduum < TOL_Y))
                                {
                                    std::cout<<"Spelniono warunki zakoczenia iteracji"<<std::endl;
                                    break;
                                }
                            }
                            return x_current;
                        }

long double solver_secant(long double x0,
                            long double x1,
                            long double x2,
                            long double (*function)(long double),
                            std::string label)
{
    long double estymator;
    long double reziduum;
    long double x_prev=x2;

    for (int i=0; i < MAX_N; i++)
    {
        x_prev=x2;
        x2 = x1 - function(x1) / ((function(x1)-function(x0))/(x1-x0));
        x0 = x1;
        x1 = x2;

        estymator = x2 - x_prev;
        reziduum = std::abs(function(x2));


        std::cout<<"--------"<<label<<"       iteracja: "<<i<<std::endl;
        std::cout<<"X:" << x2<< std::endl;
        std::cout<<"Reziduum: " << reziduum<<std::endl;
        std::cout<<"Estymator bledu xn: " <<estymator<<std::endl<<std::endl;;

        if ((estymator < TOL_X) && (reziduum < TOL_Y))
        {
            std::cout<<"Spelniono warunki zakoczenia iteracji"<<std::endl;
            break;
        }
    }
    return x2;
}
