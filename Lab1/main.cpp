#include <float.h>
#include <iostream>

template<typename Typ>
void oblicz()
{
    // epsilon -------------------------------------------------------
    // definiujemy maksymalny epsilon
    Typ e = static_cast<Typ>(1);
    int mantysa = 0;
    while (static_cast<Typ>(1) + e > static_cast<Typ>(1))
    {
        e /= static_cast<Typ>(2);
    }
    e *= static_cast<Typ>(2);
    std::cout<<"--TYP:"<<typeid(Typ).name()<<"\nEpsilon: "<<e<<std::endl;

    // mantysa -------------------------------------------------------
    Typ a = static_cast<Typ>(1);
    Typ b = static_cast<Typ>(0.5);
    int bity = 0;

    while (a + b > a)
    {
        a += b;
        b /= static_cast<Typ>(2);
        bity++;
    }

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
