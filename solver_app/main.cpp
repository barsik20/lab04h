#include "formatter_ex.h"
#include "solver.h"
#include <iostream>
int main() {
    double a, b, c;
    std::cout << "Enter coefficients a, b, c: ";
    std::cin >> a >> b >> c;

    std::cout << formatter_ex("Solving equation: ")
              << a << "x^2 + " << b << "x + " << c << " = 0" << std::endl;

    solve(a, b, c);
    return 0;
}
