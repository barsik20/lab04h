#include "solver.h"
#include <iostream>
#include <cmath>

void solve(double a, double b, double c) {
     if (a == 0) {
         std::cout << "Not a quadratic equation (a = 0)" << std::endl;
         return;
     }
     double d = b * b - 4 * a * c;
     if (d < 0) {
         std::cout << "No real roots" << std::endl;
     } else if (d == 0) {
         double x = -b / (2 * a);
         std::cout << "One root: x = " << x << std::endl;
     } else {
         double x1 = (-b + sqrt(d)) / (2 * a);
         double x2 = (-b - sqrt(d)) / (2 * a);
         std::cout << "Two roots: x1 = " << x1 << ", x2 = " << x2 << std::endl;
     }
}
