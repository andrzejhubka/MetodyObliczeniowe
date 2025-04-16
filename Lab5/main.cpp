#include <iomanip>
#include <iostream>



void decompose_lu(long double (&A)[25]);
void solve_linear_lu(long double (&A)[25], long double (&B)[5], long double (&X)[5]);

static int order[5] = {0,1,2,3,4};
int main()
{
    // macierz A
    long double A[25] = {5.l,4.l,3.l,2.l,1.l,
                  10.l,8.l,7.l,6.l,5.l,
                  -1.l,2.l,-3.l,4.l,-5.l,
                  6.l,5.l,-4.l,3.l,-2.l,
                  1.l,2.l,3.l,4.l,5.l};
    // wektor B
    long double B[5] = {37.l,99.l,-9.l,12.l,53.l};

    // wektor X -> rozwiazania ukladu rownan
    long double X[5] = {0,0,0,0,0};

    // wykonaj dekompozycje lu macierzy
    decompose_lu(A);

    // jesli macierz A zostala poddana dekompozycji i przechowuje infromacje o macierzach L i U
    // rozwiaz uklad rownan korzystajac z dwoch prostych rownan z macierzami L i U
    solve_linear_lu(A, B, X);

    // wyswietl wynik
    std::cout << "Rozwiazania ukladu rownan: " << std::setprecision(40)<<std::endl;
    for (int i = 0; i < 5; i++)
    {
        std::cout << X[i] << std::endl;
    }
    return 1;
}

void decompose_lu(long double (&A)[25])
{
    for (int i = 0; i < 5; i++)
    {
        // gdy element na przekatnej jest bliski zero
        if (A[i*5 + i] < 1e-10)
        {
            // Znajdź wiersz z maksymalnym elementem w kolumnie i
            int maxRow = i;
            long double maxVal = std::abs(A[order[i]*5 + i]);
            for (int k = i + 1; k < 5; k++)
            {
                if (std::abs(A[order[k]*5 + i]) > maxVal)
                {
                    maxVal = std::abs(A[order[k]*5 +i]);
                    maxRow = k;
                }
            }

            // Zamień wiersze jeśli potrzeba
            if (maxRow != i)
            {
                std::swap(order[i], order[maxRow]);
            }
        }

        // Eliminacja
        for (int k = i + 1; k < 5; k++)
        {
            long double factor = A[order[k] * 5 + i] / A[order[i] * 5 + i];
            A[order[k]*5 + i] = factor;
            for (int j = i+1; j < 5; j++)
            {
                A[order[k]*5 + j] -= factor * A[order[i]*5 + j];
            }
        }
    }






    // wyswietlenie wynikow dekompozycji
    std::cout<< "--------------------"<<std::endl;
    std::cout<< "Wykonano dekompozycje. Macierze L i U zapisane w ramach jednej macierzy: " << std::endl;
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            std::cout <<std::setprecision(4)<< A[order[i]*5 + j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout<< "--------------------"<<std::endl;
}

void solve_linear_lu(long double (&A)[25], long double (&B)[5], long double (&X)[5])
{
    long double Y[5];

    // Ly = b
    for (int i = 0; i < 5; i++)
    {
        long double suma = 0.L;
        for (int j = 0; j < i; j++)
        {
            suma += A[order[i]*5 + j] * Y[j];
        }
        Y[i] = B[i] - suma;
        // tutaj nie dzielimy przez Lnn jest zawsze rowne 1 dla rozkladu LU !
    }

    // Ux = Y
    for (int i = 4; i >= 0; i--)
    {
        long double suma = 0.L;
        for (int j = i + 1; j < 5; j++)
        {
            suma += A[order[i]*5 + j] * X[j];
        }
        X[i] = (Y[i] - suma) / A[order[i]*5 + i];
    }
}
