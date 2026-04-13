//@compile g++ part5/11_03.cpp -std=c++23 -Wall -Wextra -Wpedantic
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <deque>
#include <functional>
#include <iterator>
#include <print>
#include <string>
#include <vector>

///////////////////////////////////////////////////////////////////////////////////////////////

template < typename It, typename Comp >
void order(It first, It last, Comp comp)
{
    for (auto i = std::next(first); i != last; ++i)
    {
        for (auto j = i; j != first && comp(*j, *std::prev(j)); --j)
            std::iter_swap(std::prev(j), j);
    }
}

template < typename It >
void order(It first, It last)
{
    order(first, last, std::less<>{});
}

///////////////////////////////////////////////////////////////////////////////////////////////

template < typename It, typename Comp >
void merge_step(It first, It middle, It last, Comp comp)
{
    using value_t = typename std::iterator_traits < It > ::value_type;

    std::vector < value_t > buffer;
    buffer.reserve(static_cast < std::size_t > (std::distance(first, last)));

    auto i = first;
    auto j = middle;

    while (i != middle && j != last)
    {
        if (!comp(*j, *i)) buffer.push_back(*i++);
        else               buffer.push_back(*j++);
    }

    while (i != middle) buffer.push_back(*i++);
    while (j != last)   buffer.push_back(*j++);

    std::copy(buffer.begin(), buffer.end(), first);
}

template < typename It, typename Comp >
void merge_sort(It first, It last, Comp comp)
{
    if (std::distance(first, last) > 16)
    {
        auto middle = std::next(first, std::distance(first, last) / 2);

        merge_sort(first,  middle, comp);
        merge_sort(middle, last,   comp);
        merge_step(first,  middle, last, comp);
    }
    else
    {
        order(first, last, comp);
    }
}

template < typename It >
void merge_sort(It first, It last)
{
    merge_sort(first, last, std::less<>{});
}

///////////////////////////////////////////////////////////////////////////////////////////////

template < typename It, typename Comp >
It median_of_three(It first, It last, Comp comp)
{
    auto mid   = std::next(first, std::distance(first, last) / 2);
    auto right = std::prev(last);

    if (comp(*mid,   *first)) std::iter_swap(first, mid);
    if (comp(*right, *first)) std::iter_swap(first, right);
    if (comp(*right, *mid))   std::iter_swap(mid,   right);

    return mid;
}

template < typename It, typename Comp >
void quick_sort(It first, It last, Comp comp)
{
    if (std::distance(first, last) <= 16)
    {
        order(first, last, comp);
        return;
    }

    auto pivot = *median_of_three(first, last, comp);

    auto i = first;
    auto j = std::prev(last);

    while (true)
    {
        while (comp(*i, pivot)) std::advance(i,  1);
        while (comp(pivot, *j)) std::advance(j, -1);

        if (!(i < j)) break;

        std::iter_swap(i, j);
        std::advance(i,  1);
        std::advance(j, -1);
    }

    quick_sort(first,        std::next(j), comp);
    quick_sort(std::next(j), last,         comp);
}

template < typename It >
void quick_sort(It first, It last)
{
    quick_sort(first, last, std::less<>{});
}

///////////////////////////////////////////////////////////////////////////////////////////////

static bool descending_int(int a, int b) { return a > b; }

///////////////////////////////////////////////////////////////////////////////////////////////

static void test_merge_sort()
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

    std::print("test_merge_sort (default): passed\n");
}

///////////////////////////////////////////////////////////////////////////////////////////////

static void test_quick_sort()
{
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

    std::print("test_quick_sort (default): passed\n");
}

///////////////////////////////////////////////////////////////////////////////////////////////

static void demo_comparators()
{
    const std::vector < int > src = { 5, 1, 9, 3, 7, 2, 8, 4, 6 };

    const auto is_asc  = [](auto const & r) { return std::ranges::is_sorted(r); };
    const auto is_desc = [](auto const & r) { return std::ranges::is_sorted(r, std::greater<>{}); };

    {
        auto v = src;
        merge_sort(v.begin(), v.end(), descending_int);
        assert(is_desc(v));
        std::print("merge_sort + free function (desc): passed\n");
    }
    {
        auto v = src;
        merge_sort(v.begin(), v.end(), std::less < int > {});
        assert(is_asc(v));
        std::print("merge_sort + std::less     (asc):  passed\n");
    }
    {
        auto v = src;
        merge_sort(v.begin(), v.end(), [](int a, int b) { return a > b; });
        assert(is_desc(v));
        std::print("merge_sort + lambda        (desc): passed\n");
    }

    {
        auto v = src;
        quick_sort(v.begin(), v.end(), descending_int);
        assert(is_desc(v));
        std::print("quick_sort + free function (desc): passed\n");
    }
    {
        auto v = src;
        quick_sort(v.begin(), v.end(), std::less < int > {});
        assert(is_asc(v));
        std::print("quick_sort + std::less     (asc):  passed\n");
    }
    {
        auto v = src;
        quick_sort(v.begin(), v.end(), [](int a, int b) { return a > b; });
        assert(is_desc(v));
        std::print("quick_sort + lambda        (desc): passed\n");
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    test_merge_sort();
    test_quick_sort();
    demo_comparators();
}

///////////////////////////////////////////////////////////////////////////////////////////////

/*Results:
test_merge_sort (default): passed
test_quick_sort (default): passed
merge_sort + free function (desc): passed
merge_sort + std::less     (asc):  passed
merge_sort + lambda        (desc): passed
quick_sort + free function (desc): passed
quick_sort + std::less     (asc):  passed
quick_sort + lambda        (desc): passed
*/
