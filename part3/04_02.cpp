//@compile clang++ part3/04_02.cpp -std=c++2b -Wall -Wextra -Wpedantic
#include <iostream>
#include <iomanip>


double max(double a) {
    return a;
}

template<typename... Args>
double max(double first, Args... rest) {
    double max_rest = max(rest...);
    return (first > max_rest) ? first : max_rest;
}

double min(double a) {
    return a;
}

template<typename... Args>
double min(double first, Args... rest) {
    double min_rest = min(rest...);
    return (first < min_rest) ? first : min_rest;
}

template<typename... Args>
double sum(Args... args) {
    return (args + ...);
}

template<typename... Args>
double average(Args... args) {
    return sum(args...) / sizeof...(args);
}

int main() {
    double a = 3.5, b = 7.2, c = -1.0, d = 4.8, e = 2.2;

    std::cout << std::fixed << std::setprecision(2);

    std::cout << "Values: " << a << ", " << b << ", " << c << ", " << d << ", " << e << "\n";
    std::cout << "Max: " << max(a, b, c, d, e) << "\n";
    std::cout << "Min: " << min(a, b, c, d, e) << "\n";
    std::cout << "Sum: " << sum(a, b, c, d, e) << "\n";
    std::cout << "Average: " << average(a, b, c, d, e) << "\n";

    return 0;
}
