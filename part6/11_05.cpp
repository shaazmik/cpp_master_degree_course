//@compile g++ part6/11_05.cpp -std=c++23 -Wall -Wextra -Wpedantic
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <iterator>
#include <numeric>
#include <random>
#include <ranges>
#include <vector>

///////////////////////////////////////////////////////////////////////////////////////////////

template < std::ranges::input_range R, typename OutputIt,
           typename Transform, typename Pred >
auto transform_if(R && range, OutputIt out, Transform transform, Pred pred)
{
    using value_t = std::ranges::range_value_t < R >;
    std::vector < value_t > filtered;
    std::ranges::copy_if(std::forward < R >(range), std::back_inserter(filtered), pred);
    return std::ranges::transform(filtered, out, transform);
}

///////////////////////////////////////////////////////////////////////////////////////////////

template < std::ranges::sized_range R1, std::ranges::input_range R2 >
double mae(R1 const & actual, R2 const & predicted)
{
    auto const n = static_cast < double >(std::ranges::size(actual));
    return std::transform_reduce(
        std::ranges::begin(actual), std::ranges::end(actual),
        std::ranges::begin(predicted),
        0.0, std::plus < > {},
        [](double a, double p) { return std::abs(a - p); }
    ) / n;
}

template < std::ranges::sized_range R1, std::ranges::input_range R2 >
double mse(R1 const & actual, R2 const & predicted)
{
    auto const n = static_cast < double >(std::ranges::size(actual));
    return std::transform_reduce(
        std::ranges::begin(actual), std::ranges::end(actual),
        std::ranges::begin(predicted),
        0.0, std::plus < > {},
        [](double a, double p) { double const d = a - p; return d * d; }
    ) / n;
}

///////////////////////////////////////////////////////////////////////////////////////////////

class Fibonacci : public std::ranges::view_interface < Fibonacci >
{
public:

    explicit Fibonacci(int count) : m_count(count) {}

    //  -----------------------------------------------------------------------

    auto begin() const { return Iterator{}; }
    auto end()   const { return Sentinel{m_count}; }

private:

    int m_count;

    //  -----------------------------------------------------------------------

    struct Sentinel
    {
        int count = 0;
    };

    class Iterator
    {
    public:

        using iterator_concept = std::forward_iterator_tag;
        using value_type       = long long;
        using difference_type  = std::ptrdiff_t;

        Iterator() = default;

        //  -------------------------------------------------------------------

        long long operator*() const { return m_a; }

        //  -------------------------------------------------------------------

        Iterator & operator++()
        {
            auto const t = m_a + m_b;
            m_a          = m_b;
            m_b          = t;
            ++m_pos;
            return *this;
        }

        Iterator operator++(int)
        {
            auto copy = *this;
            ++(*this);
            return copy;
        }

        //  -------------------------------------------------------------------

        bool operator==(Iterator const & other) const
        {
            return m_a == other.m_a && m_b == other.m_b;
        }

        friend bool operator==(Iterator const & it, Sentinel const & s)
        {
            return it.m_pos >= s.count;
        }

    private:

        long long m_a   = 0;
        long long m_b   = 1;
        int       m_pos = 0;
    };
};

static_assert(std::ranges::range < Fibonacci >);
static_assert(std::ranges::view  < Fibonacci >);

///////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    // ---- ranges::replace ----
    {
        std::vector v = { 1, 2, 3, 2, 1 };
        std::ranges::replace(v, 2, 99);
        assert((v == std::vector{ 1, 99, 3, 99, 1 }));
        std::cout << "replace: ok\n";
    }

    // ---- ranges::fill ----
    {
        std::vector < int > v(5);
        std::ranges::fill(v, 7);
        assert((v == std::vector{ 7, 7, 7, 7, 7 }));
        std::cout << "fill: ok\n";
    }

    // ---- ranges::unique ----
    {
        std::vector v = { 1, 1, 2, 3, 3, 3, 4 };
        auto [first, last] = std::ranges::unique(v);
        v.erase(first, last);
        assert((v == std::vector{ 1, 2, 3, 4 }));
        std::cout << "unique: ok\n";
    }

    // ---- ranges::rotate ----
    {
        std::vector v = { 1, 2, 3, 4, 5 };
        std::ranges::rotate(v, v.begin() + 2);
        assert((v == std::vector{ 3, 4, 5, 1, 2 }));
        std::cout << "rotate: ok\n";
    }

    // ---- ranges::sample ----
    {
        std::vector v = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
        std::vector < int > sampled;
        std::mt19937 rng{ 42 };
        std::ranges::sample(v, std::back_inserter(sampled), 4, rng);
        assert(sampled.size() == 4);
        std::cout << "sample: ok\n";
    }

    // ---- transform_if ----
    {
        std::vector v = { 1, 2, 3, 4, 5, 6 };
        std::vector < int > result;
        transform_if(v, std::back_inserter(result),
            [](int x) { return x * x; },
            [](int x) { return x % 2 == 0; });
        assert((result == std::vector{ 4, 16, 36 }));
        std::cout << "transform_if: ok\n";
    }

    // ---- MAE ----
    {
        std::vector < double > actual    = { 1.0, 2.0, 3.0 };
        std::vector < double > predicted = { 1.5, 2.5, 2.5 };
        assert(std::abs(mae(actual, predicted) - 0.5) < 1e-10);
        std::cout << "MAE: ok\n";
    }

    // ---- MSE ----
    {
        std::vector < double > actual    = { 1.0, 2.0, 3.0 };
        std::vector < double > predicted = { 2.0, 2.0, 2.0 };
        assert(std::abs(mse(actual, predicted) - 2.0 / 3.0) < 1e-10);
        std::cout << "MSE: ok\n";
    }

    // ---- views::filter ----
    {
        std::vector v = { 1, 2, 3, 4, 5, 6 };
        auto evens = v | std::views::filter([](int x) { return x % 2 == 0; });
        std::vector < int > result(evens.begin(), evens.end());
        assert((result == std::vector{ 2, 4, 6 }));
        std::cout << "views::filter: ok\n";
    }

    // ---- views::drop ----
    {
        std::vector v = { 1, 2, 3, 4, 5 };
        auto dropped = v | std::views::drop(2);
        std::vector < int > result(dropped.begin(), dropped.end());
        assert((result == std::vector{ 3, 4, 5 }));
        std::cout << "views::drop: ok\n";
    }

    // ---- views::join ----
    {
        std::vector < std::vector < int > > vv = { { 1, 2 }, { 3, 4 }, { 5 } };
        auto joined = vv | std::views::join;
        std::vector < int > result(joined.begin(), joined.end());
        assert((result == std::vector{ 1, 2, 3, 4, 5 }));
        std::cout << "views::join: ok\n";
    }

    // ---- views::zip ----
    {
        std::vector a = { 1, 2, 3 };
        std::vector b = { 4, 5, 6 };
        std::vector < int > sums;
        for (auto [x, y] : std::views::zip(a, b))
            sums.push_back(x + y);
        assert((sums == std::vector{ 5, 7, 9 }));
        std::cout << "views::zip: ok\n";
    }

    // ---- views::stride ----
    {
        std::vector v = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        auto strided = v | std::views::stride(3);
        std::vector < int > result(strided.begin(), strided.end());
        assert((result == std::vector{ 0, 3, 6, 9 }));
        std::cout << "views::stride: ok\n";
    }

    // ---- Fibonacci view ----
    {
        Fibonacci fib{ 10 };
        std::vector < long long > result;
        std::ranges::copy(fib, std::back_inserter(result));
        assert((result == std::vector < long long >{ 0, 1, 1, 2, 3, 5, 8, 13, 21, 34 }));
        std::cout << "Fibonacci: ok\n";
    }

    // ---- Fibonacci | views::filter via view_interface ----
    {
        Fibonacci fib{ 20 };
        std::vector < long long > result;
        for (auto x : fib | std::views::filter([](long long x) { return x > 100; }))
            result.push_back(x);
        assert(!result.empty() && result.front() == 144);
        std::cout << "Fibonacci | filter: ok\n";
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////
