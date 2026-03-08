//@compile g++ part4/08_05.cpp -std=c++23 -Wall -Wextra -Wpedantic
#include <cassert>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <numeric>
#include <print>
#include <string>
#include <vector>

///////////////////////////////////////////////////////////////////////////////////////////////

class Timer
{
public:

    Timer(std::string const & scope) : m_scope(scope) {}

    //  -------------------------------------------------------------------------------------------

    void start()
    {
        if (!m_running)
        {
            m_begin   = clock_t::now();
            m_running = true;
        }
    }

    void stop()
    {
        if (m_running)
        {
            m_durations.push_back(std::chrono::duration_cast < duration_t > (clock_t::now() - m_begin));
            m_running = false;
        }
    }

    //  -------------------------------------------------------------------------------------------

    auto const & durations() const { return m_durations; }

    double average() const
    {
        if (m_durations.empty()) return 0.0;

        auto sum = std::accumulate(m_durations.begin(), m_durations.end(), duration_t{});
        return (sum / m_durations.size()).count();
    }

private:

    using clock_t    = std::chrono::steady_clock;
    using duration_t = std::chrono::duration < double >;

    std::string m_scope;
    clock_t::time_point      m_begin;
    bool                     m_running = false;
    std::vector < duration_t > m_durations;
};

///////////////////////////////////////////////////////////////////////////////////////////////

auto calculate(std::size_t size)
{
    auto x = 0.0;

    for (auto i = 0uz; i < size; ++i)
    {
        x += std::pow(std::sin(i), 2) + std::pow(std::cos(i), 2);
    }

    return x;
}

auto equal(double x, double y, double epsilon = 1e-6)
{
    return std::abs(x - y) < epsilon;
}

///////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    Timer timer("calculate");

    for (auto i = 0; i < 5; ++i)
    {
        timer.start();
        assert(equal(calculate(1'000'000), 1'000'000));
        timer.stop();
    }

    for (auto i = 0; i < 5; ++i)
        std::print("calculate [{}] : {:.6f}\n", i, timer.durations()[i].count());

    std::print("calculate : average = {:.6f}\n", timer.average());
}

///////////////////////////////////////////////////////////////////////////////////////////////
