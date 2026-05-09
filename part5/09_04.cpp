//@compile g++ part5/09_04.cpp -std=c++23 -Wall -Wextra -Wpedantic
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <deque>
#include <iterator>
#include <string>
#include <vector>

///////////////////////////////////////////////////////////////////////////////////////////////

template < typename It >
void order(It first, It last)
{
    for (auto i = std::next(first); i != last; ++i)
    {
        for (auto j = i; j != first && *std::prev(j) > *j; --j)
            std::iter_swap(std::prev(j), j);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////

template < typename It >
void merge_step(It first, It middle, It last)
{
    using value_t = typename std::iterator_traits < It > ::value_type;

    std::vector < value_t > buffer;
    buffer.reserve(static_cast < std::size_t > (std::distance(first, last)));

    auto i = first;
    auto j = middle;

    while (i != middle && j != last)
    {
        if (*i <= *j) buffer.push_back(*i++);
        else          buffer.push_back(*j++);
    }

    while (i != middle) buffer.push_back(*i++);
    while (j != last)   buffer.push_back(*j++);

    std::copy(buffer.begin(), buffer.end(), first);
}

template < typename It >
void merge_sort(It first, It last)
{
    if (std::distance(first, last) > 16)
    {
        auto middle = std::next(first, std::distance(first, last) / 2);

        merge_sort(first,  middle);
        merge_sort(middle, last  );
        merge_step(first,  middle, last);
    }
    else
    {
        order(first, last);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////

template < typename It >
It median_of_three(It first, It last)
{
    auto mid   = std::next(first, std::distance(first, last) / 2);
    auto right = std::prev(last);

    if (*first > *mid)   std::iter_swap(first, mid);
    if (*first > *right) std::iter_swap(first, right);
    if (*mid   > *right) std::iter_swap(mid,   right);

    return mid;
}

template < typename It >
void quick_sort(It first, It last)
{
    if (std::distance(first, last) <= 16)
    {
        order(first, last);
        return;
    }

    auto pivot = *median_of_three(first, last);

    auto i = first;
    auto j = std::prev(last);

    while (true)
    {
        while (*i < pivot) std::advance(i,  1);
        while (*j > pivot) std::advance(j, -1);

        if (!(i < j)) break;

        std::iter_swap(i, j);
        std::advance(i,  1);
        std::advance(j, -1);
    }

    quick_sort(first,        std::next(j));
    quick_sort(std::next(j), last        );
}

///////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    {
        std::vector < int > v = { 5, 1, 9, 3, 7, 2, 8, 4, 6 };
        merge_sort(v.begin(), v.end());
        assert(std::ranges::is_sorted(v));
    }
    {
        std::vector < double > v = { 2.5, 1.1, 3.14, -0.7, 2.5 };
        merge_sort(v.begin(), v.end());
        assert(std::ranges::is_sorted(v));
    }
    {
        std::vector < std::string > v = { "www", "apple", "banana", "qqq" };
        merge_sort(v.begin(), v.end());
        assert(std::ranges::is_sorted(v));
    }
    {
        std::deque < int > d = { 5, 3, 8, 1, 9, 2, 7, 4, 6, 0 };
        merge_sort(d.begin(), d.end());
        assert(std::ranges::is_sorted(d));
    }

    //  -------------------------------------------------------------------------------------------

    {
        std::vector < int > v = { 5, 1, 9, 3, 7, 2, 8, 4, 6 };
        quick_sort(v.begin(), v.end());
        assert(std::ranges::is_sorted(v));
    }
    {
        std::vector < double > v = { 2.5, 1.1, 3.14, -0.7, 2.5 };
        quick_sort(v.begin(), v.end());
        assert(std::ranges::is_sorted(v));
    }
    {
        std::vector < std::string > v = { "www", "apple", "banana", "qqq" };
        quick_sort(v.begin(), v.end());
        assert(std::ranges::is_sorted(v));
    }
    {
        std::deque < int > d = { 5, 3, 8, 1, 9, 2, 7, 4, 6, 0 };
        quick_sort(d.begin(), d.end());
        assert(std::ranges::is_sorted(d));
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////
