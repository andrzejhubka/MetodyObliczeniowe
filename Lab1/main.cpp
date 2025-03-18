// ANDRZEJ HUBKA, METODY OBLICZENIOWE ZADANIE LABORATORYJNE NUMER 1
#include <iostream>
#include <typeinfo>
#include <cfloat>
#include <iomanip>

template<typename Typ>
void oblicz()
{
    Typ e = static_cast<Typ>(1);
    int bity = 0;

    while (static_cast<Typ>(1) + e > static_cast<Typ>(1))
    {
        e /= static_cast<Typ>(2);
        bity++;
    }
    e *= static_cast<Typ>(2);
    std::cout<<"--TYP:"<<typeid(Typ).name()<<std::setprecision(36)<<"\nEpsilon: "<<e<<std::endl;
    std::cout<<"Bity mantysy: "<<bity<<std::endl;


}

int main()
{
    // wywolujemy funkcje szablonowe dla kazdego typu zmiennej w standardzie IEEE 754
    oblicz<float>();
    oblicz<double>();
    oblicz<long double>();

    // poprawne wyniki
    std::cout<<"-----------------EPSILON POPRAWNE WYNIKI-----------------"<<std::endl;
    std::cout<<"Poprawny wynik dla float: "<<FLT_EPSILON<<std::endl;
    std::cout<<"Dla double: "<<DBL_EPSILON<<std::endl;
    std::cout<<"Dla long double: "<<LDBL_EPSILON<<std::endl;

    std::cout<<"-----------------MANTYYSA POPRAWNE WYNIKI-----------------"<<std::endl;
    std::cout<<"Poprawny wynik dla float: "<<FLT_MANT_DIG<<std::endl;
    std::cout<<"Dla double: "<<DBL_MANT_DIG<<std::endl;
    std::cout<<"Dla long double: "<<LDBL_MANT_DIG<<std::endl;

    return 0;
}
