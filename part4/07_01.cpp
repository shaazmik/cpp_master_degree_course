//@compile clang++ part4/07_01.cpp -std=c++20 -Wall -Wextra -Wpedantic
#include <cmath>
#include <iostream>
#include <optional>
#include <utility>
#include <variant>

const double epsilon = 1e-10;

bool isEqual(double a, double b) {
    return std::abs(a - b) < epsilon;
}

using Roots = std::variant<std::monostate, double, std::pair<double, double>>;

std::optional<Roots> solve(double a, double b, double c) {
    if (isEqual(a, 0)) {
        if (isEqual(b, 0)) {
            if (isEqual(c, 0))
                return Roots{std::monostate{}};
            return std::nullopt;
        }
        return Roots{-c / b};
    }

    double discriminant = b * b - 4 * a * c;

    if (discriminant < -epsilon)
        return std::nullopt;

    if (isEqual(discriminant, 0))
        return Roots{-b / (2 * a)};

    double sqrt_d = std::sqrt(discriminant);
    return Roots{std::pair{(-b - sqrt_d) / (2 * a), (-b + sqrt_d) / (2 * a)}};
}

int main() {
    double a, b, c;
    std::cout << "Input a, b, c coefficients\n";
    std::cin >> a >> b >> c;

    auto result = solve(a, b, c);

    if (!result) {
        std::cout << "no real roots" << std::endl;
    } else if (std::holds_alternative<std::monostate>(*result)) {
        std::cout << "infinite number of roots" << std::endl;
    } else if (std::holds_alternative<double>(*result)) {
        std::cout << std::get<double>(*result) << std::endl;
    } else {
        auto [x1, x2] = std::get<std::pair<double, double>>(*result);
        std::cout << x1 << " " << x2 << std::endl;
    }

    return 0;
}
