//@compile g++ part5/10_01.cpp -std=c++23 -Wall -Wextra -Wpedantic
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <deque>
#include <print>
#include <vector>

///////////////////////////////////////////////////////////////////////////////////////////////

static void test_vector_growth()
{
    const auto n = 1'024uz;

    std::vector < int > v;

    std::size_t prev = 0;

    std::print("vector capacity growth:\n");

    for (auto i = 0uz; i < n; ++i)
    {
        v.push_back(static_cast < int > (i));

        if (v.capacity() != prev)
        {
            if (prev != 0)
            {
                double factor = static_cast < double > (v.capacity()) /
                                static_cast < double > (prev);

                std::print("  {} -> {} (factor = {:.2f})\n", prev, v.capacity(), factor);
            }

            prev = v.capacity();
        }
    }

    std::print("\n");
}

///////////////////////////////////////////////////////////////////////////////////////////////

static void test_vector_assertions()
{
    {
        std::vector < int > v;
        assert(v.capacity() == 0 && v.size() == 0);
    }

    {
        std::vector < int > v;

        while (v.size() < 64)
            v.push_back(0);

        auto cap_before = v.capacity();

        while (v.size() <= cap_before)
            v.push_back(0);

        auto cap_after = v.capacity();

        assert(cap_after > cap_before);
        assert(cap_after >= cap_before * 3 / 2);
    }

    std::print("vector assertions: passed\n\n");
}

///////////////////////////////////////////////////////////////////////////////////////////////

static void test_deque_pages()
{
    const auto n = 1'024uz;

    std::deque < int > d;

    for (auto i = 0uz; i < n; ++i)
        d.push_back(static_cast < int > (i));

    std::size_t max_streak = 0;
    std::size_t streak     = 1;
    std::size_t boundaries = 0;

    std::print("deque page boundaries (first 5):\n");

    for (auto i = 0uz; i + 1 < n; ++i)
    {
        auto diff = reinterpret_cast < const std::byte * > (&d[i + 1]) -
                    reinterpret_cast < const std::byte * > (&d[i]);

        if (diff == static_cast < std::ptrdiff_t > (sizeof(int)))
        {
            ++streak;
        }
        else
        {
            if (boundaries < 5)
                std::print("  boundary at index {}: page had {} elements\n", i, streak);

            max_streak = std::max(max_streak, streak);
            ++boundaries;
            streak = 1;
        }
    }

    max_streak = std::max(max_streak, streak);

    std::print("\ndeque page size: {} elements ({} bytes)\n",
               max_streak, max_streak * sizeof(int));
    std::print("boundaries detected: {}\n\n", boundaries);

    assert(max_streak > 1);
    assert(boundaries > 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////

static void test_deque_assertions()
{
    std::deque < int > d;

    for (auto i = 0; i < 512; ++i)
        d.push_back(i);

    bool found_boundary = false;

    for (auto i = 0uz; i + 1 < d.size(); ++i)
    {
        auto diff = reinterpret_cast < const std::byte * > (&d[i + 1]) -
                    reinterpret_cast < const std::byte * > (&d[i]);

        if (diff != static_cast < std::ptrdiff_t > (sizeof(int)))
        {
            found_boundary = true;
            break;
        }
    }

    assert(found_boundary);

    std::print("deque assertions: passed\n\n");
}

///////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    test_vector_growth();
    test_vector_assertions();
    test_deque_pages();
    test_deque_assertions();
}

///////////////////////////////////////////////////////////////////////////////////////////////

/*Results:
vector capacity growth:
  1 -> 2 (factor = 2.00)
  2 -> 4 (factor = 2.00)
  4 -> 8 (factor = 2.00)
  8 -> 16 (factor = 2.00)
  16 -> 32 (factor = 2.00)
  32 -> 64 (factor = 2.00)
  64 -> 128 (factor = 2.00)
  128 -> 256 (factor = 2.00)
  256 -> 512 (factor = 2.00)
  512 -> 1024 (factor = 2.00)

vector assertions: passed

deque page boundaries (first 5):
  boundary at index 127: page had 128 elements
  boundary at index 255: page had 128 elements
  boundary at index 383: page had 128 elements
  boundary at index 511: page had 128 elements
  boundary at index 639: page had 128 elements

deque page size: 128 elements (512 bytes)
boundaries detected: 7

deque assertions: passed
*/