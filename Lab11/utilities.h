#ifndef UTILITIES_H
#define UTILITIES_H

#include <vector>

void thomasAlgorithm(std::vector<long double>& a, 
                     std::vector<long double>& b, 
                     std::vector<long double>& c, 
                     std::vector<long double>& d, 
                     std::vector<long double>& x, 
                     int n);

long double U(const long double x, const long double t, const long double D);

#endif // UTILITIES_H