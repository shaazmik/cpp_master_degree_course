//@compile clang++ part2/03_10.cpp -std=c++2b -Wall -Wextra -Wpedantic
#include <cassert>
#include <cmath>
#include <compare>
#include <istream>
#include <numeric>
#include <ostream>
#include <sstream>
#include <vector>

///////////////////////////////////////////////////////////////////////////////////////////////

class Rational
{
public :

	Rational(int num = 0, int den = 1) : m_num(num), m_den(den)
	{
		reduce();
	}

//  -------------------------------------------------------------------------------------------

	explicit operator double() const
	{ 
		return 1.0 * m_num / m_den;
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

//  -------------------------------------------------------------------------------------------

	auto & operator-=(Rational const & other) 
	{ 
		return *this += Rational(other.m_num * -1, other.m_den);
	}

//  -------------------------------------------------------------------------------------------

	auto & operator*=(Rational const & other)
	{
		m_num *= other.m_num;

		m_den *= other.m_den;

		reduce();

		return *this;
	}

//  -------------------------------------------------------------------------------------------
	
	auto & operator/=(Rational const & other) 
	{ 
		return *this *= Rational(other.m_den, other.m_num);
	}

//  -------------------------------------------------------------------------------------------

	auto operator++(int) { auto x = *this; *this += 1; return x; }

	auto operator--(int) { auto x = *this; *this -= 1; return x; }

//  -------------------------------------------------------------------------------------------

	auto & operator++() { *this += 1; return *this; }

	auto & operator--() { *this -= 1; return *this; }

//  -------------------------------------------------------------------------------------------

	friend auto operator+ (Rational lhs, Rational const & rhs) { return lhs += rhs; }

	friend auto operator- (Rational lhs, Rational const & rhs) { return lhs -= rhs; }

	friend auto operator* (Rational lhs, Rational const & rhs) { return lhs *= rhs; }

	friend auto operator/ (Rational lhs, Rational const & rhs) { return lhs /= rhs; }

//  -------------------------------------------------------------------------------------------

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

//  -------------------------------------------------------------------------------------------

	friend auto & operator>>(std::istream & stream, Rational & rational)
	{
		return (stream >> rational.m_num).ignore() >> rational.m_den;
	}

//  -------------------------------------------------------------------------------------------

	friend auto & operator<<(std::ostream & stream, Rational const & rational)
	{
		return stream << rational.m_num << '/' << rational.m_den;
	}

private :

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

//  -------------------------------------------------------------------------------------------

	int m_num = 0, m_den = 1;
};

///////////////////////////////////////////////////////////////////////////////////////////////

auto equal(double x, double y, double epsilon = 1e-6)
{
	return std::abs(x - y) < epsilon;
}

///////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
	Rational x = 1, y(2, 1);

//  -----------------------------------------------------------------------

//	std::vector < int > vector_1 = 5; // error

	std::vector < int > vector_2(5);

	std::vector < int > vector_3 = { 1, 2, 3, 4, 5 };

//  -----------------------------------------------------------------------

	assert(equal(static_cast < double > (x), 1));

//  -----------------------------------------------------------------------

//	assert(x.operator+=(y) == Rational(3, 1)); // bad

//  -----------------------------------------------------------------------

	assert((x += y) == Rational(+3, 1));

	assert((x -= y) == Rational(+1, 1));

	assert((x *= y) == Rational(+2, 1));

	assert((x /= y) == Rational(+1, 1));

//  -----------------------------------------------------------------------

	assert((x ++  ) == Rational(+1, 1)); // support : compiler-explorer.com

	assert((x --  ) == Rational(+2, 1));

	assert((  ++ y) == Rational(+3, 1)); // support : compiler-explorer.com

	assert((  -- y) == Rational(+2, 1));

//  -----------------------------------------------------------------------

//	x++++; // error

//  -----------------------------------------------------------------------

	[[maybe_unused]] auto z = 0;

//  -----------------------------------------------------------------------

//	z++++; // error

//  -----------------------------------------------------------------------

//	assert(operator+(x, y) == Rational(3, 1)); // bad

//  -----------------------------------------------------------------------

	assert((x +  y) == Rational(+3, 1));

	assert((x -  y) == Rational(-1, 1));

	assert((x *  y) == Rational(+2, 1));

	assert((x /  y) == Rational(+1, 2));

//  -----------------------------------------------------------------------

	assert((x += 1) == Rational(+2, 1));

//	assert((1 += x) == Rational(+3, 1)); // error

	assert((x +  1) == Rational(+3, 1));

	assert((1 +  y) == Rational(+3, 1));

	assert((1 +  1) == Rational(+2, 1));

//  -----------------------------------------------------------------------

	assert((x <  y) == 0);
	
	assert((x >  y) == 0);

	assert((x <= y) == 1);

	assert((x >= y) == 1);

	assert((x == y) == 1);

	assert((x != y) == 0);

//  -----------------------------------------------------------------------

	std::stringstream stream_1("1/2");

	std::stringstream stream_2;
	
//  -----------------------------------------------------------------------

	stream_1 >> x;

	stream_2 << x;

//  -----------------------------------------------------------------------

	assert(stream_2.str() == stream_1.str());
	Rational a(1, 2), b(2, 4), c(-3, -6);
	assert(a == b);
	assert(b == c);
	assert(!(a != c));
	assert((a <=> b) == std::strong_ordering::equal);

	Rational d(3, 4);
	assert((a < d) == true);
	assert((d > a) == true);
	assert((a <= d) == true);
	assert((d >= a) == true);
	assert((a <=> d) == std::strong_ordering::less);
	assert((d <=> a) == std::strong_ordering::greater);

	Rational zero(0, 5), neg_zero(-0, 3);
	assert(zero == neg_zero);
	assert((zero <=> neg_zero) == std::strong_ordering::equal);
	assert(!(zero < neg_zero));
	assert(!(zero > neg_zero));

	Rational neg1(-1, 2), neg2(1, -2);
	assert(neg1 == neg2);
	assert((neg1 < Rational(0, 1)));
	assert((Rational(0, 1) > neg1));
	assert((neg1 <=> Rational(0, 1)) == std::strong_ordering::less);

	Rational p(1, 3), n(-1, 3);
	assert(p != n);
	assert((p <=> n) == std::strong_ordering::greater);
	assert((n <=> p) == std::strong_ordering::less);

	Rational big1(10000, 20001), big2(9999, 20000);
	assert((big1 > big2));
	assert((big2 < big1));

	Rational r1(1, 4), r2(1, 2), r3(3, 4);
	assert((r1 < r2) && (r2 < r3));
	assert(r1 < r3);
	assert((r3 > r1));

	Rational one(1,1), two(2,1);
	assert(one < two);
	assert(two > one);
	assert(!(two == one));
	assert((two >= one));
	assert((one <= two));

	Rational self(7, 9);
	assert(self == self);
	assert(!(self != self));
	assert((self <=> self) == std::strong_ordering::equal);

	Rational z1(1, -2), z2(-1, 2);
	assert(z1 == z2);
	assert((z1 <=> z2) == std::strong_ordering::equal);
	assert((z1 < Rational(0,1)));
	assert(!(z1 > Rational(0,1)));
}