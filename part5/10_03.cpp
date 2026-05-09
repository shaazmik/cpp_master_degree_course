//@compile g++ part5/10_03.cpp -std=c++23 -Wall -Wextra -Wpedantic -I boost_1_85_0
#include <cassert>
#include <cstddef>
#include <iostream>

#include <boost/multi_array.hpp>

///////////////////////////////////////////////////////////////////////////////////////////////

using grid_t = boost::multi_array < bool, 2 >;

static constexpr auto rows = 10uz;
static constexpr auto cols = 10uz;

///////////////////////////////////////////////////////////////////////////////////////////////

static auto count_neighbors(grid_t const & grid, std::size_t row, std::size_t col) -> int
{
    int count = 0;

    for (auto dr = -1; dr <= 1; ++dr)
    for (auto dc = -1; dc <= 1; ++dc)
    {
        if (dr == 0 && dc == 0) continue;

        auto r = static_cast < int > (row) + dr;
        auto c = static_cast < int > (col) + dc;

        if (r >= 0 && r < static_cast < int > (rows) &&
            c >= 0 && c < static_cast < int > (cols))
        {
            if (grid[static_cast < std::size_t > (r)][static_cast < std::size_t > (c)])
                ++count;
        }
    }

    return count;
}

///////////////////////////////////////////////////////////////////////////////////////////////

static void step(grid_t const & current, grid_t & next)
{
    for (auto r = 0uz; r < rows; ++r)
    for (auto c = 0uz; c < cols; ++c)
    {
        auto n = count_neighbors(current, r, c);

        next[r][c] = current[r][c] ? (n == 2 || n == 3) : (n == 3);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////

static void print(grid_t const & grid, std::size_t generation)
{
    std::cout << "generation " << generation << ":\n";

    for (auto r = 0uz; r < rows; ++r)
    {
        for (auto c = 0uz; c < cols; ++c)
            std::cout << (grid[r][c] ? '@' : '.');

        std::cout << '\n';
    }

    std::cout << '\n';
}

///////////////////////////////////////////////////////////////////////////////////////////////

static void test_blinker()
{
    grid_t grid(boost::extents[rows][cols]);
    grid_t next(boost::extents[rows][cols]);

    // horizontal blinker at row 5
    grid[5][4] = true;
    grid[5][5] = true;
    grid[5][6] = true;

    step(grid, next);

    // after one step: vertical blinker
    assert( next[4][5] && next[5][5] && next[6][5]);
    assert(!next[5][4] && !next[5][6]);

    grid_t next2(boost::extents[rows][cols]);
    step(next, next2);

    // after two steps: back to horizontal
    assert( next2[5][4] && next2[5][5] && next2[5][6]);
    assert(!next2[4][5] && !next2[6][5]);
}

///////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    test_blinker();

    grid_t grid(boost::extents[rows][cols]);

    // glider pattern
    // .@.
    // ..@
    // @@@
    grid[1][2] = true;
    grid[2][3] = true;
    grid[3][1] = true;
    grid[3][2] = true;
    grid[3][3] = true;

    grid_t next(boost::extents[rows][cols]);

    grid_t * current = &grid;
    grid_t * buffer  = &next;

    const auto generations = 16uz;

    for (auto g = 0uz; g < generations; ++g)
    {
        print(*current, g);
        step(*current, *buffer);
        std::swap(current, buffer);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////
