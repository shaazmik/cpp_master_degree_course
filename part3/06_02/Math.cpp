module Math;


import std;

namespace MyLib
{

    void Rational::reduce()
    {
        if (m_den < 0)
        {
            m_num = -m_num;
            m_den = -m_den;
        }

        auto common = std::gcd(m_num, m_den);
        m_num /= common;
        m_den /= common;
    }

    Rational::Rational(int num, int den) : m_num(num), m_den(den)
    {
        reduce();
    }

    Rational& Rational::operator+=(Rational const& other)
    {
        auto common_den = std::lcm(m_den, other.m_den);
        m_num = m_num * (common_den / m_den) + other.m_num * (common_den / other.m_den);
        m_den = common_den;
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
        int n = 0, d = 1;
        if ((stream >> n).ignore() >> d)
        {
            rational = Rational(n, d);
        }
        return stream;
    }

    std::ostream& operator<<(std::ostream& stream, Rational const& rational)
    {
        return stream << rational.m_num << '/' << rational.m_den;
    }
}