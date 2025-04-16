#include <iostream>
#include <iomanip>
#include "benchmark.cpp"
#define MATRIX_SIZE 5

void thomas_step1(long double (&l)[MATRIX_SIZE-1],
                  long double (&d)[MATRIX_SIZE],
                  long double (&u)[MATRIX_SIZE-1]);
void thomas_step2(long double (&l)[MATRIX_SIZE-1],
                  long double (&d)[MATRIX_SIZE],
                  long double (&u)[MATRIX_SIZE-1],
                  long double (&b)[MATRIX_SIZE]);
void thomas_solve(long double (&l)[MATRIX_SIZE-1],
                  long double (&d)[MATRIX_SIZE],
                  long double (&u)[MATRIX_SIZE-1],
                  long double (&b)[MATRIX_SIZE],
                  long double (&x)[MATRIX_SIZE]);

void show_matrix(const long double (&l)[MATRIX_SIZE-1],
                  const long double (&d)[MATRIX_SIZE],
                  const long double (&u)[MATRIX_SIZE-1],
                  const long double (&b)[MATRIX_SIZE],
                  const long double (&x)[MATRIX_SIZE]);

int main()
{
    // reprezentacja macierzy trojdiagonalnej 5x5
    long double u[4] = {-1.l,-3.l,5.l,-7.l};
    long double d[5] = {100.l, 200.l, 300.l, 200.l, 100.l};
    long double l[4] = {2.l, 4.l, -6.l,-8.l};

    // wektor wyrazow wolnych
    long double b[5] = {199.l, 195.l, 929.l, 954.l, 360.l};

    // wektor rozwiazan
    long double x[5];

    // wykonaj dzialania na macierzy a
    thomas_step1(l, d, u);

    // teraz korzystaj z nich i wykona dzjalania na wektorze b
    thomas_step2(l, d, u, b);

    // rozwiaz uklad rownan
    thomas_solve(l, d, u, b, x);

    // wyswietlenie wynikow
    show_matrix(l, d, u, b, x);

    return 0;
}

void thomas_step1(long double (&l)[MATRIX_SIZE-1],
                  long double (&d)[MATRIX_SIZE],
                  long double (&u)[MATRIX_SIZE-1])
{
    // dziala tyko na macierzy A

    for(int i = 1; i < MATRIX_SIZE; i++)
    {
        d[i] = d[i] - l[i-1] / d[i-1] * u[i-1];
    }
}

void thomas_step2(long double (&l)[MATRIX_SIZE-1],
                  long double (&d)[MATRIX_SIZE],
                  long double (&u)[MATRIX_SIZE-1],
                  long double (&b)[MATRIX_SIZE])
{
    // dziala na wektorze B przy wykorzystaniu obliczen z kroku 1
    for (int i = 1; i < MATRIX_SIZE; i++)
    {
        b[i] = b[i] - l[i-1] / d[i-1] * b[i-1];
    }

}

void show_matrix(const long double (&l)[MATRIX_SIZE-1],
                  const long double (&d)[MATRIX_SIZE],
                  const long double (&u)[MATRIX_SIZE-1],
                    const long double (&b)[MATRIX_SIZE],
                    const long double (&x)[MATRIX_SIZE])
{
    std::cout<<"------------MACIERZ  A PO WYKONANIU ALGORYTMU THOMASA----------------"<<std::endl;
    std::cout << std::setprecision(10) << std::fixed;
    for(int i = 0; i < MATRIX_SIZE-1; i++)
    {
        std::cout << u[i] << " ";
    }
    std::cout << std::endl;

    for(int i = 0; i < MATRIX_SIZE; i++)
    {
        std::cout << d[i] << " ";
    }
    std::cout << std::endl;

    for(int i = 0; i < MATRIX_SIZE-1; i++)
    {
        std::cout << l[i] << " ";
    }
    std::cout << std::endl;

    std::cout<<"------------WEKTOR B PO WYKONANIU ALGORYTMU THOMASA----------------"<<std::endl;
    for(int i = 0; i < MATRIX_SIZE; i++)
    {
        std::cout << b[i] << " ";
    }
    std::cout << std::endl;

    std::cout<<"------------WEKTOR X PO WYKONANIU ALGORYTMU THOMASA----------------"<<std::endl;
    for(int i = 0; i < MATRIX_SIZE; i++)
    {
        std::cout << x[i] << " ";
    }
    std::cout << std::endl;
}

void thomas_solve(long double (&l)[MATRIX_SIZE-1],
                  long double (&d)[MATRIX_SIZE],
                  long double (&u)[MATRIX_SIZE-1],
                  long double (&b)[MATRIX_SIZE],
                  long double (&x)[MATRIX_SIZE])
{
    x[MATRIX_SIZE-1] = b[MATRIX_SIZE-1] / d[MATRIX_SIZE-1];

    for (int i=MATRIX_SIZE-2; i >= 0; i--)
    {
        x[i] = (b[i] - u[i]*x[i+1]) / d[i];
    }
}