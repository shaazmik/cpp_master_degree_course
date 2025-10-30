//@compile clang++ part2/03_02.cpp -std=c++2b -Wall -Wextra -Wpedantic
#include <cmath>
#include <iostream>
#include <numbers>
#include <limits>
#include <algorithm>
#include <iomanip>   // for std::setprecision and std::scientific


// -------------------------------------------------------------
class Triangle {
private:
    double m_side1, m_side2, m_side3 = 0.0;

public:
    Triangle(double s1, double s2, double s3)
        : m_side1(s1), m_side2(s2), m_side3(s3) {}

    bool is_valid() const {
        if (!std::isfinite(m_side1) || !std::isfinite(m_side2) || !std::isfinite(m_side3))
            return false;

        if (!std::isgreater(m_side1, 0.0) || !std::isgreater(m_side2, 0.0) || !std::isgreater(m_side3, 0.0))
            return false;

        return (std::isgreater(m_side1 + m_side2, m_side3)) &&
               (std::isgreater(m_side1 + m_side3, m_side2)) &&
               (std::isgreater(m_side2 + m_side3, m_side1));
    }

    double perimeter() const {
        return is_valid() ? (m_side1 + m_side2 + m_side3) : 0.0;
    }

    double area() const {
        if (!is_valid()) return 0.0;
        const double s = perimeter() / 2.0;
        const double value = s * (s - m_side1) * (s - m_side2) * (s - m_side3);
        return std::isgreater(value, 0.0) ? std::sqrt(value) : 0.0; // avoid NaN due to rounding
    }
};

// -------------------------------------------------------------
class Square {
private:
    double m_side = 0.0;

public:
    explicit Square(double side) : m_side(side) {}

    bool is_valid() const {
        return std::isfinite(m_side) && std::isgreater(m_side, 0.0);
    }

    double perimeter() const {
        return is_valid() ? 4.0 * m_side : 0.0;
    }

    double area() const {
        return is_valid() ? m_side * m_side : 0.0;
    }
};


// -------------------------------------------------------------
class Circle {
private:
    double m_radius = 0.0;

public:
    explicit Circle(double radius) : m_radius(radius) {}

    bool is_valid() const {
        return std::isfinite(m_radius) && std::isgreater(m_radius, 0.0);
    }

    double perimeter() const {
        return is_valid() ? 2.0 * std::numbers::pi * m_radius : 0.0;
    }

    double area() const {
        return is_valid() ? std::numbers::pi * m_radius * m_radius : 0.0;
    }
};


// -------------------------------------------------------------
int main() {
    std::cout << std::boolalpha; // print booleans as true/false
    std::cout << std::scientific << std::setprecision(15); // high precision, scientific format

    // Normal valid shapes
    std::cout << "Valid triangle (3,4,5):\n";
    Triangle t1(3.0, 4.0, 5.0);
    std::cout << "  Perimeter: " << t1.perimeter()   << "\n"
              << "  Area:      " << t1.area()        << "\n"
              << "  Valid:     " << t1.is_valid()    << "\n\n";

    std::cout << "Valid square (5):\n";
    Square s1(5.0);
    std::cout << "  Perimeter: " << s1.perimeter()   << "\n"
              << "  Area:      " << s1.area()        << "\n"
              << "  Valid:     " << s1.is_valid()    << "\n\n";

    std::cout << "Valid circle (r=3):\n";
    Circle c1(3.0);
    std::cout << "  Perimeter: " << c1.perimeter()   << "\n"
              << "  Area:      " << c1.area()        << "\n"
              << "  Valid:     " << c1.is_valid()    << "\n\n";

    // Invalid shapes
    std::cout << "Invalid triangle (1,2,10):\n";
    Triangle t_invalid(1.0, 2.0, 10.0);
    std::cout << "  Perimeter: " << t_invalid.perimeter() << "\n"
              << "  Area:      " << t_invalid.area()      << "\n"
              << "  Valid:     " << t_invalid.is_valid()  << "\n\n";

    std::cout << "Invalid square (-2):\n";
    Square s_invalid(-2.0);
    std::cout << "  Perimeter: " << s_invalid.perimeter() << "\n"
              << "  Area:      " << s_invalid.area()      << "\n"
              << "  Valid:     " << s_invalid.is_valid()  << "\n\n";

    std::cout << "Invalid circle (-1):\n";
    Circle c_invalid(-1.0);
    std::cout << "  Perimeter: " << c_invalid.perimeter() << "\n"
              << "  Area:      " << c_invalid.area()      << "\n"
              << "  Valid:     " << c_invalid.is_valid()  << "\n\n";

    // Edge cases
    std::cout << "Triangle with zero side:\n";
    Triangle t_zero(0.0, 4.0, 5.0);
    std::cout << "  Perimeter: " << t_zero.perimeter()    << "\n"
              << "  Area:      " << t_zero.area()         << "\n"
              << "  Valid:     " << t_zero.is_valid()     << "\n\n";

    std::cout << "Square with zero side:\n";
    Square s_zero(0.0);
    std::cout << "  Perimeter: " << s_zero.perimeter()    << "\n"
              << "  Area:      " << s_zero.area()         << "\n"
              << "  Valid:     " << s_zero.is_valid()     << "\n\n";

    std::cout << "Circle with zero radius:\n";
    Circle c_zero(0.0);
    std::cout << "  Perimeter: " << c_zero.perimeter()    << "\n"
              << "  Area:      " << c_zero.area()         << "\n"
              << "  Valid:     " << c_zero.is_valid()     << "\n\n";

    // NaN and Infinity cases
    double nan = std::numeric_limits<double>::quiet_NaN();
    double inf = std::numeric_limits<double>::infinity();

    std::cout << "Triangle with NaN:\n";
    Triangle t_nan(nan, 3.0, 4.0);
    std::cout << "  Valid: " << t_nan.is_valid() << "\n\n";

    std::cout << "Square with Inf:\n";
    Square s_inf(inf);
    std::cout << "  Valid: " << s_inf.is_valid() << "\n\n";

    std::cout << "Circle with NaN:\n";
    Circle c_nan(nan);
    std::cout << "  Valid: " << c_nan.is_valid() << "\n\n";

    // Large values (risk of overflow)
    double large = 1e154;
    std::cout << "Large triangle (1e154,1e154,1e154):\n";
    Triangle t_large(large, large, large);
    std::cout << "  Perimeter: " << t_large.perimeter()   << "\n"
              << "  Area:      " << t_large.area()        << "\n"
              << "  Valid:     " << t_large.is_valid()    << "\n\n";

    std::cout << "Large square (1e154):\n";
    Square s_large(1e154);
    std::cout << "  Perimeter: " << s_large.perimeter()   << "\n"
              << "  Area:      " << s_large.area()        << "\n"
              << "  Valid:     " << s_large.is_valid()    << "\n\n";

    std::cout << "Large circle (r=1e154):\n";
    Circle c_large(1e154);
    std::cout << "  Perimeter: " << c_large.perimeter()   << "\n"
              << "  Area:      " << c_large.area()        << "\n"
              << "  Valid:     " << c_large.is_valid()    << "\n\n";

    // Very small values
    double tiny = 1e-308;
    std::cout << "Tiny triangle (1e-308,1e-308,1e-308):\n";
    Triangle t_tiny(tiny, tiny, tiny);
    std::cout << "  Perimeter: " << t_tiny.perimeter()    << "\n"
              << "  Area:      " << t_tiny.area()         << "\n"
              << "  Valid:     " << t_tiny.is_valid()     << "\n\n";

    // Near-equality edge case
    std::cout << "Almost-degenerate triangle (1,1,1.999999999):\n";
    Triangle t_almost(1.0, 1.0, 1.999999999);
    std::cout << "  Perimeter: " << t_almost.perimeter()  << "\n"
              << "  Area:      " << t_almost.area()       << "\n"
              << "  Valid:     " << t_almost.is_valid()   << "\n\n";

    // Mixed extreme values
    std::cout << "Mixed triangle (1e-10,1e200,1e200):\n";
    Triangle t_mixed(1e-10, 1e200, 1e200);
    std::cout << "  Perimeter: " << t_mixed.perimeter()   << "\n"
              << "  Area:      " << t_mixed.area()        << "\n"
              << "  Valid:     " << t_mixed.is_valid()    << "\n\n";

    std::cout << "All tests completed.\n";
    return 0;
}
