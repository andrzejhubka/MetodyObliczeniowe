#include <math.h>
#include <iostream>


long double f(long double t, long double y)
{
    return -1.l * (((100.l * t + 10) / (t+1.l)) * (y-1.l));
}

double y_analytical(long double t)
{
    return 1.l + pow((1.l + t),90.l) * exp(-100.l * t);
}

void BME(long double y0, long double dt, long double tf)
{
    long double t = 0;
    long double y = y0;

    while (t<tf)
    {
        t = t + dt;
        y = y + f(t, y) * dt;
    }
}

void PME(long double y0, long double dt, long double tf)
{
    long double t = 0;
    long double y = y0;

    while (t<tf)
    {
        t = t + dt;
        y = (y + dt * (100.l*t + 10.l) / (t+1.l)) / (1 + dt * (100.l *t + 10.l) / (t + 1.l));
    }
}

void PMT(long double y0, long double dt, long double tf)
{
    long double t = 0;
    long double y = y0;

    while (t<tf)
    {
        t += dt;
        y = (2.l*y + dt * f(t, y) + dt * (100*t+10) / (t+1)) / (2.l + dt * (100.l*t + 10.l) / (t+1.l));
        std::cout<<"BME:"<<y<<" ";
        std::cout<<"Analitycznie:"<<y_analytical(t)<<std::endl;
    }
}

int main()
{
    //ZGODNIE Z OBLCIZENIAMI ABY BME BYŁA STABILNA dt < 1/50
    long double y0 = 2.l, dt = 0.0001l, tf = 1.l;
    BME(y0, dt, tf);

    // PME BEZWARUNKOWO STABILNA
    PME(y0, dt, tf);

    // PMT BEZWARUNKOWO STABILNA
    PME(y0, dt, tf);

    return 0;

}
