#include "Rational.hpp"

#include <cmath>
#include <numeric> // std::gcd, std::lcm
#include <iostream>

Rational::Rational(int num, int den) : m_num(num), m_den(den)
{
    reduce();
}

void Rational::reduce()
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

Rational& Rational::operator+=(Rational const& other)
{
    auto lcm = std::lcm(m_den, other.m_den);
    m_num = m_num * (lcm / m_den) + other.m_num * (lcm / other.m_den);
    m_den = lcm;
    reduce();
    return *this;
}

Rational& Rational::operator-=(Rational const& other)
{
    return *this += Rational(other.m_num * -1, other.m_den);
}

Rational& Rational::operator*=(Rational const& other)
{
    m_num *= other.m_num;
    m_den *= other.m_den;
    reduce();
    return *this;
}

Rational& Rational::operator/=(Rational const& other)
{
    return *this *= Rational(other.m_den, other.m_num);
}

std::strong_ordering operator<=>(Rational const& lhs, Rational const& rhs)
{
    long long left = static_cast<long long>(lhs.m_num) * rhs.m_den;
    long long right = static_cast<long long>(rhs.m_num) * lhs.m_den;

    if (left < right) return std::strong_ordering::less;
    if (left > right) return std::strong_ordering::greater;
    return std::strong_ordering::equal;
}

bool operator==(Rational const& lhs, Rational const& rhs)
{
    return lhs.m_num == rhs.m_num && lhs.m_den == rhs.m_den;
}

std::istream& operator>>(std::istream& stream, Rational& rational)
{
    return (stream >> rational.m_num).ignore() >> rational.m_den;
}

std::ostream& operator<<(std::ostream& stream, Rational const& rational)
{
    return stream << rational.m_num << '/' << rational.m_den;
}