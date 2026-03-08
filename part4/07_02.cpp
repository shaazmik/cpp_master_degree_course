//@compile clang++ part4/07_02.cpp -std=c++20 -Wall -Wextra -Wpedantic
#include <cassert>
#include <cmath>
#include <compare>
#include <exception>
#include <istream>
#include <iostream>
#include <limits>
#include <numeric>
#include <optional>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <variant>
#include <vector>

///////////////////////////////////////////////////////////////////////////////////////////////

class Exception : public std::exception
{
public:

    Exception(std::string message) : m_message(std::move(message)) {}

    char const * what() const noexcept override
    {
        return m_message.c_str();
    }

private:

    std::string m_message;
};

///////////////////////////////////////////////////////////////////////////////////////////////

template <typename Derived>
class addable
{
    friend Derived operator+(Derived lhs, Derived const & rhs) { return lhs += rhs; }
};

template <typename Derived>
class subtractable
{
    friend Derived operator-(Derived lhs, Derived const & rhs) { return lhs -= rhs; }
};

template <typename Derived>
class multipliable
{
    friend Derived operator*(Derived lhs, Derived const & rhs) { return lhs *= rhs; }
};

template <typename Derived>
class dividable
{
    friend Derived operator/(Derived lhs, Derived const & rhs) { return lhs /= rhs; }
};

template <typename Derived>
class incrementable
{
public:
    Derived operator++(int)
    {
        auto & self = static_cast<Derived &>(*this);
        auto x = self;
        ++self;
        return x;
    }
};

template <typename Derived>
class decrementable
{
public:
    Derived operator--(int)
    {
        auto & self = static_cast<Derived &>(*this);
        auto x = self;
        --self;
        return x;
    }
};

///////////////////////////////////////////////////////////////////////////////////////////////

template <typename T = int>
class Rational
    : public addable      < Rational<T> >
    , public subtractable < Rational<T> >
    , public multipliable < Rational<T> >
    , public dividable    < Rational<T> >
    , public incrementable< Rational<T> >
    , public decrementable< Rational<T> >
{
public:

    Rational(T num = 0, T den = 1) : m_num(num), m_den(den)
    {
        if (m_den == 0)
            throw Exception("Rational: zero denominator");

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

    using incrementable<Rational>::operator++;
    using decrementable<Rational>::operator--;

    auto & operator++() { *this += 1; return *this; }
    auto & operator--() { *this -= 1; return *this; }

    //  -------------------------------------------------------------------------------------------

    friend std::strong_ordering operator<=>(Rational const & lhs, Rational const & rhs)
    {
        long long left  = static_cast<long long>(lhs.m_num) * rhs.m_den;
        long long right = static_cast<long long>(rhs.m_num) * lhs.m_den;

        if (left < right) return std::strong_ordering::less;
        if (left > right) return std::strong_ordering::greater;
        return std::strong_ordering::equal;
    }

    friend bool operator==(Rational const & lhs, Rational const & rhs)
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

    x += y; assert(x == Rational<int>(3, 1));
    x -= y; assert(x == Rational<int>(1, 1));
    x *= y; assert(x == Rational<int>(2, 1));
    x /= y; assert(x == Rational<int>(1, 1));

    //  -------------------------------------------------------------------------------------------

    Rational<int> p(1, 2), q(1, 3);

    assert(p + q == Rational<int>(5, 6));
    assert(p - q == Rational<int>(1, 6));
    assert(p * q == Rational<int>(1, 6));
    assert(p / q == Rational<int>(3, 2));

    //  -------------------------------------------------------------------------------------------

    Rational<int> r(3, 2);

    auto pre   = ++r; assert(pre   == Rational<int>(5, 2));
    auto post  = r++; assert(post  == Rational<int>(5, 2)); assert(r == Rational<int>(7, 2));
    auto pred  = --r; assert(pred  == Rational<int>(5, 2));
    auto postd = r--; assert(postd == Rational<int>(5, 2)); assert(r == Rational<int>(3, 2));

    //  -------------------------------------------------------------------------------------------

    Rational<long long> big1(10000LL, 20001LL), big2(9999LL, 20000LL);
    assert(big1 > big2);

    std::stringstream ss("3/4");
    Rational<int> t;
    ss >> t;
    assert(t == Rational<int>(3, 4));

    //  -------------------------------------------------------------------------------------------
    // Exception: Rational with zero denominator — our Exception class (inherits std::exception)

    try
    {
        Rational<int> bad(1, 0);
    }
    catch (std::exception const & e)
    {
        std::cerr << "Exception:           " << e.what() << '\n';
    }
    catch (...)
    {
        std::cerr << "unknown exception\n";
    }

    //  -------------------------------------------------------------------------------------------
    // bad_alloc: trying to allocate SIZE_MAX bytes — system cannot give that much

    try
    {
        ::operator new(std::numeric_limits<std::size_t>::max());
    }
    catch (std::exception const & e)
    {
        std::cerr << "bad_alloc:           " << e.what() << '\n';
    }
    catch (...)
    {
        std::cerr << "unknown exception\n";
    }

    //  -------------------------------------------------------------------------------------------
    // bad_variant_access: variant holds int, but we call get<double>

    try
    {
        std::variant<int, double> v = 42;
        std::get<double>(v);
    }
    catch (std::exception const & e)
    {
        std::cerr << "bad_variant_access:  " << e.what() << '\n';
    }
    catch (...)
    {
        std::cerr << "unknown exception\n";
    }

    //  -------------------------------------------------------------------------------------------
    // bad_optional_access: calling .value() on an empty optional

    try
    {
        std::optional<int> empty;
        empty.value();
    }
    catch (std::exception const & e)
    {
        std::cerr << "bad_optional_access: " << e.what() << '\n';
    }
    catch (...)
    {
        std::cerr << "unknown exception\n";
    }

    //  -------------------------------------------------------------------------------------------
    // length_error: vector::reserve(n) when n > max_size() — size is too large

    try
    {
        std::vector<int> v;
        v.reserve(std::numeric_limits<std::size_t>::max());
    }
    catch (std::exception const & e)
    {
        std::cerr << "length_error:        " << e.what() << '\n';
    }
    catch (...)
    {
        std::cerr << "unknown exception\n";
    }

    //  -------------------------------------------------------------------------------------------
    // out_of_range: vector::at(i) when i >= size() — index out of bounds

    try
    {
        std::vector<int> v = {1, 2, 3};
        [[maybe_unused]] auto val = v.at(100);
    }
    catch (std::exception const & e)
    {
        std::cerr << "out_of_range:        " << e.what() << '\n';
    }
    catch (...)
    {
        std::cerr << "unknown exception\n";
    }

    //  -------------------------------------------------------------------------------------------
    // catch(...): catches anything that does not inherit from std::exception

    try
    {
        throw 42;
    }
    catch (std::exception const & e)
    {
        std::cerr << "std::exception: " << e.what() << '\n';
    }
    catch (...)
    {
        std::cerr << "unknown exception (not std::exception)\n";
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////
