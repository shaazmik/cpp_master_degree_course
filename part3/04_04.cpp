//@compile clang++ part3/04_04.cpp -std=c++2b -Wall -Wextra -Wpedantic
#include <cassert>
#include <cmath>
#include <compare>
#include <istream>
#include <numeric>
#include <ostream>
#include <sstream>
#include <vector>
#include <type_traits>

///////////////////////////////////////////////////////////////////////////////////////////////

template <typename T = int>
class Rational
{
public:

    Rational(T num = 0, T den = 1) : m_num(num), m_den(den)
    {
        reduce();
    }

    //  -------------------------------------------------------------------------------------------

    explicit operator double() const
    { 
        return static_cast<double>(m_num) / static_cast<double>(m_den);
    }

    //  -------------------------------------------------------------------------------------------

    auto & operator+=(Rational const & other)
    {
        auto lcm = std::lcm(m_den, other.m_den);

        m_num = m_num * (lcm / m_den) + other.m_num * (lcm / other.m_den);
        m_den = lcm;

        reduce();

        return *this;
    }

    auto & operator-=(Rational const & other) 
    { 
        return *this += Rational(other.m_num * -1, other.m_den);
    }

    auto & operator*=(Rational const & other)
    {
        m_num *= other.m_num;
        m_den *= other.m_den;
        reduce();
        return *this;
    }

    auto & operator/=(Rational const & other) 
    { 
        return *this *= Rational(other.m_den, other.m_num);
    }

    auto operator++(int) { auto x = *this; *this += 1; return x; }
    auto operator--(int) { auto x = *this; *this -= 1; return x; }
    auto & operator++() { *this += 1; return *this; }
    auto & operator--() { *this -= 1; return *this; }

    friend auto operator+ (Rational lhs, Rational const & rhs) { return lhs += rhs; }
    friend auto operator- (Rational lhs, Rational const & rhs) { return lhs -= rhs; }
    friend auto operator* (Rational lhs, Rational const & rhs) { return lhs *= rhs; }
    friend auto operator/ (Rational lhs, Rational const & rhs) { return lhs /= rhs; }

    friend std::strong_ordering operator<=>(Rational const &lhs, Rational const &rhs)
    {
        long long left = static_cast<long long>(lhs.m_num) * rhs.m_den;
        long long right = static_cast<long long>(rhs.m_num) * lhs.m_den;

        if (left < right) return std::strong_ordering::less;
        if (left > right) return std::strong_ordering::greater;
        return std::strong_ordering::equal;
    }

    friend bool operator==(Rational const &lhs, Rational const &rhs)
    {
        return lhs.m_num == rhs.m_num && lhs.m_den == rhs.m_den;
    }

    friend auto & operator>>(std::istream & stream, Rational & rational)
    {
        return (stream >> rational.m_num).ignore() >> rational.m_den;
    }

    friend auto & operator<<(std::ostream & stream, Rational const & rational)
    {
        return stream << rational.m_num << '/' << rational.m_den;
    }

private:

    void reduce()
    {
        if (m_den < 0)
        {
            m_num = -m_num;
            m_den = -m_den;
        }

        auto gcd = std::gcd(m_num, m_den);
        m_num /= gcd;
        m_den /= gcd;
    }

    T m_num = 0;
    T m_den = 1;
};

///////////////////////////////////////////////////////////////////////////////////////////////

auto equal(double x, double y, double epsilon = 1e-6)
{
    return std::abs(x - y) < epsilon;
}

///////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    Rational<int> x = 1, y(2, 1);
    Rational<long> a(1L, 2L), b(2L, 4L);

    assert(equal(static_cast<double>(x), 1));
    assert(a == b);

    x += y;
    assert(x == Rational<int>(3, 1));
    x -= y;
    assert(x == Rational<int>(1, 1));
    x *= y;
    assert(x == Rational<int>(2, 1));
    x /= y;
    assert(x == Rational<int>(1, 1));

    Rational<long long> big1(10000LL, 20001LL), big2(9999LL, 20000LL);
    assert(big1 > big2);

    std::stringstream ss("3/4");
    Rational<int> r;
    ss >> r;
    assert(r == Rational<int>(3,4));

    
    // -------------------------------------------------------
    // The current Rational class uses std::gcd and std::lcm to simplify fractions
    // These functions only work with integer types (int, long, long long, etc.)
    // you cannot use Rational<double> — the compiler will throw an error
    //
    // For example, the following will NOT compile:
    //
    // Rational<double> d1(0.5, 1.0), d2(0.25, 0.5);
    // d1 += d2;
    //
    // To support double, the addition and subtraction methods must be rewritten
    // without using std::gcd and std::lcm.
    // -------------------------------------------------------
    return 0;
}
