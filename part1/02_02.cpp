#include <iostream>
#include <cmath>

const double epsilon = 1e-10;

bool isEqual(double a, double b) {
    return std::abs(a-b) < epsilon;
}

int main() {
    double a, b, c = NAN;
    std::cout << "Input a, b, c coefficients\n";
    std::cin >> a >> b >> c;

    if (isEqual(a, 0)) {
        // Линейное уравнение b*x + c = 0
        if (isEqual(b, 0)) {
            if (isEqual(c, 0)) {
                std::cout << "infinite number of roots" << std::endl;
            } else {
                std::cout << "no solution" << std::endl;
            }
        } else {
            if (isEqual(c, 0)) {
                std::cout << "Negative zero case: ";
            }
            double x = -c / b;
            std::cout << x << std::endl;
        }
    } else {
        // Квадратное уравнение a*x^2 + b*x + c = 0
        double discriminant = b * b - 4 * a * c;
        if (discriminant < -epsilon) {
            std::cout << "no real roots" << std::endl;
        } else if (isEqual(discriminant, 0)) {
            double x = -b / (2 * a);
            std::cout << x << std::endl;
        } else {
            double sqrt_d = std::sqrt(discriminant);
            double x1 = (-b - sqrt_d) / (2 * a);
            double x2 = (-b + sqrt_d) / (2 * a);
            std::cout << x1 << " " << x2 << std::endl;
        }
    }

    return 0;
}