//@compile clang++ part3/04_06.cpp -std=c++2b -Wall -Wextra -Wpedantic
#include <iostream>
#include <array>
#include <iomanip>

constexpr double my_abs(double x) {
    return x < 0 ? -x : x;
}

consteval double calculate_pi(double epsilon) {
    double pi = 0.0;
    double denominator = 1.0;
    int sign = 1;

    while (true) {
        double term = 4.0 / denominator;
        
        if (term < epsilon) {
            break; 
        }

        pi += sign * term;
        sign = -sign;
        denominator += 2.0;
    }
    return pi;
}

consteval double calculate_e(double epsilon) {
    double e = 1.0;
    double term = 1.0;
    int n = 1;

    while (term >= epsilon) {
        term /= n;
        e += term;
        n++;
    }
    return e;
}

constexpr std::array<double, 3> epsilons = { 1e-3, 1e-4, 1e-5 };

static_assert(my_abs(calculate_pi(0.01) - 3.14) < 0.02, "Error for Pi (0.01)");

static_assert(calculate_pi(epsilons[0]) > 3.13 && calculate_pi(epsilons[0]) < 3.15, "Error Pi (1e-3)");

static_assert(calculate_e(0.01) > 2.7 && calculate_e(0.01) < 2.72, "Error E (0.01)");
static_assert(my_abs(calculate_e(epsilons[1]) - 2.71828) < 0.0001, "Error E (1e-4)");


int main() {
    constexpr double pi_val_1 = calculate_pi(epsilons[0]); // 1e-3
    constexpr double pi_val_2 = calculate_pi(epsilons[1]); // 1e-4
    
    constexpr double e_val_1 = calculate_e(epsilons[0]);
    constexpr double e_val_2 = calculate_e(epsilons[2]); // 1e-5

    std::cout << std::fixed << std::setprecision(10);
    
    std::cout << "Compile-time calculation results:\n";
    std::cout << "---------------------------------\n";
    
    std::cout << "Epsilon: " << epsilons[0] << "\n";
    std::cout << "Pi: " << pi_val_1 << "\n";
    std::cout << "e:  " << e_val_1 << "\n\n";

    std::cout << "Epsilon: " << epsilons[1] << "\n";
    std::cout << "Pi: " << pi_val_2 << "\n"; // 1e-4
    
    std::cout << "\nEpsilon: " << epsilons[2] << "\n";
    std::cout << "e:  " << e_val_2 << "\n";

    return 0;
}