#include <cmath>
#include <iostream>
#include <numbers>
#include <limits>
#include <algorithm>
#include <iomanip>
#include <vector>
#include <memory>  // for std::unique_ptr

//////////////////////////////////////////////////////////////
// Abstract base class Shape
//////////////////////////////////////////////////////////////

class Shape {
public:
    virtual ~Shape() = default;
    virtual double perimeter() const = 0;
    virtual double area() const = 0;
};

//////////////////////////////////////////////////////////////
// Triangle class
//////////////////////////////////////////////////////////////

class Triangle : public Shape {
private:
    double m_side1{}, m_side2{}, m_side3{};

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

    double perimeter() const override final {
        return is_valid() ? (m_side1 + m_side2 + m_side3) : 0.0;
    }

    double area() const override final {
        if (!is_valid()) return 0.0;
        const double s = perimeter() / 2.0;
        const double value = s * (s - m_side1) * (s - m_side2) * (s - m_side3);
        return std::isgreater(value, 0.0) ? std::sqrt(value) : 0.0;
    }
};

//////////////////////////////////////////////////////////////
// Square class (final)
//////////////////////////////////////////////////////////////

class Square final : public Shape {
private:
    double m_side{};

public:
    explicit Square(double side) : m_side(side) {}

    bool is_valid() const {
        return std::isfinite(m_side) && std::isgreater(m_side, 0.0);
    }

    double perimeter() const override {
        return is_valid() ? 4.0 * m_side : 0.0;
    }

    double area() const override {
        return is_valid() ? m_side * m_side : 0.0;
    }
};

//////////////////////////////////////////////////////////////
// Circle class (final)
//////////////////////////////////////////////////////////////

class Circle final : public Shape {
private:
    double m_radius{};

public:
    explicit Circle(double radius) : m_radius(radius) {}

    bool is_valid() const {
        return std::isfinite(m_radius) && std::isgreater(m_radius, 0.0);
    }

    double perimeter() const override {
        return is_valid() ? 2.0 * std::numbers::pi * m_radius : 0.0;
    }

    double area() const override {
        return is_valid() ? std::numbers::pi * m_radius * m_radius : 0.0;
    }
};


// class Test1 : public Triangle {
// private:
//     double m_test{};
// public:
//     double area() const override {
//         return m_test;
//     }
// };

// class Test2 : public Circle {
// private:
//     double m_test{};
// public:
//  double test() {return m_test;}
// };

//////////////////////////////////////////////////////////////
// Demonstration
//////////////////////////////////////////////////////////////

int main() {
    std::cout << std::scientific << std::setprecision(6);

    // Vector of polymorphic shapes
    std::vector<std::unique_ptr<Shape>> shapes;

    shapes.emplace_back(std::make_unique<Triangle>(3.0, 4.0, 5.0));
    shapes.emplace_back(std::make_unique<Square>(5.0));
    shapes.emplace_back(std::make_unique<Circle>(3.0));

    std::cout << "Polymorphic output via Shape*:\n";
    for (const auto& shape : shapes) {
        std::cout << "  Perimeter = " << shape->perimeter()
                  << ", Area = " << shape->area() << "\n";
    }

    // Automatic cleanup (unique_ptr)
    std::cout << "All shapes processed successfully.\n";
    return 0;
}
