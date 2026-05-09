//@compile g++ part5/10_04.cpp -std=c++23 -Wall -Wextra -Wpedantic -I boost_1_85_0
#include <cassert>
#include <cstddef>
#include <iostream>
#include <iterator>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"  //  boost 1.85 uses deprecated std::iterator base in ublas/detail/iterator.hpp
#include <boost/numeric/ublas/matrix.hpp>
#pragma GCC diagnostic pop

///////////////////////////////////////////////////////////////////////////////////////////////

namespace ublas = boost::numeric::ublas;

using ull      = unsigned long long;
using matrix_t = ublas::matrix < ull >;

///////////////////////////////////////////////////////////////////////////////////////////////

static auto mat_mul(matrix_t const & a, matrix_t const & b) -> matrix_t
{
    return ublas::prod(a, b);
}

///////////////////////////////////////////////////////////////////////////////////////////////

static auto mat_pow(matrix_t m, std::size_t n) -> matrix_t
{
    matrix_t result = ublas::identity_matrix < ull > (2);

    while (n > 0)
    {
        if (n & 1uz) result = mat_mul(result, m);
        m = mat_mul(m, m);
        n >>= 1;
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////////////////////////

static auto fibonacci(std::size_t n) -> ull
{
    //  [[1,1],[1,0]]^n == [[F(n+1), F(n)], [F(n), F(n-1)]]
    matrix_t base(2, 2);
    base(0, 0) = 1; base(0, 1) = 1;
    base(1, 0) = 1; base(1, 1) = 0;

    return mat_pow(base, n)(0, 1);
}

///////////////////////////////////////////////////////////////////////////////////////////////

static void test_fibonacci()
{
    static constexpr ull table[] = { 0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55 };

    for (auto i = 0uz; i < std::size(table); ++i)
        assert(fibonacci(i) == table[i]);

    assert(fibonacci(20) ==          6'765ULL);
    assert(fibonacci(30) ==        832'040ULL);
    assert(fibonacci(50) == 12'586'269'025ULL);
}

///////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    test_fibonacci();

    const auto n = 15uz;

    std::cout << "fibonacci sequence:\n";

    for (auto i = 0uz; i <= n; ++i)
        std::cout << "  F(" << i << ") = " << fibonacci(i) << '\n';
}

///////////////////////////////////////////////////////////////////////////////////////////////
/*
Complexity comparison of all known algorithms for computing N-th Fibonacci number:

  naive recursive:       O(2^N)   -- very bad, calls itself two times on each step,
                                      so tree of calls grows exponentially, no cache
  iterative / DP:        O(N)     -- simple loop from 1 to N, nothing interesting
  memoized recursive:    O(N)     -- same as iterative but with recursion and hashmap,
                                      each subproblem computed only one time
  closed-form (Binet):   O(1)*    -- looks nice in tehory, but in practice
                                      floating point kills everything after N ~ 70
  matrix exponentiation: O(log N) -- best asymptotic complexity among exact methods,
                                      this is what we implemented here

*  Binet formula uses sqrt(5) which is irrational, so double cannot represent it
   exactly. Rounding errors accumulate and after some N the answer becomes wrong.
   Not suitable for serious computations.

Our algorithm: we know that [[1,1],[1,0]]^N gives us [[F(N+1),F(N)],[F(N),F(N-1)]].
So we just need to raise this matrix to power N using binary exponentiation (fast power).
On each step we either multiply result by current matrix (if bit is set) and then square
current matrix. Total number of steps is O(log N). Each matrix multiplication for 2x2
is constant time (8 multiplications + 4 additions). So final complexity is O(log N).

This is significantly better than O(N) iterative approach for very large N, for example
N = 10^18 would require 10^18 iterations but only about 60 matrix multiplications.
*/
