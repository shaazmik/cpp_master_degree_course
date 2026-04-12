//@compile clang++ part4/07_05.cpp -std=c++20 -O2 -Wall -Wextra -Wpedantic -I/usr/local/include -L/usr/local/lib -lbenchmark_main -lbenchmark -lpthread
#include <algorithm>
#include <cstddef>
#include <numeric>
#include <vector>

#include <benchmark/benchmark.h>

///////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
void order(std::vector<T> & vector, std::size_t left, std::size_t right)
{
    for (auto i = left + 1; i < right; ++i)
        for (auto j = i; j > left; --j)
            if (vector[j - 1] > vector[j])
                std::swap(vector[j], vector[j - 1]);
}

template <typename T>
T median_of_three(std::vector<T> & vec, std::size_t left, std::size_t right)
{
    std::size_t mid = left + (right - left) / 2;

    if (vec[left] > vec[mid])   std::swap(vec[left], vec[mid]);
    if (vec[left] > vec[right]) std::swap(vec[left], vec[right]);
    if (vec[mid]  > vec[right]) std::swap(vec[mid],  vec[right]);

    return vec[mid];
}

template <typename T>
void quick_sort_step(std::vector<T> & vec, std::size_t left, std::size_t right, std::size_t threshold)
{
    if (right - left < threshold)
        return order(vec, left, right + 1);

    T pivot = median_of_three(vec, left, right);

    std::size_t i = left;
    std::size_t j = right;

    while (true)
    {
        while (vec[i] < pivot) ++i;
        while (vec[j] > pivot) --j;
        if (i >= j) break;
        std::swap(vec[i], vec[j]);
        ++i; --j;
    }

    quick_sort_step(vec, left,    j,     threshold);
    quick_sort_step(vec, j + 1, right,   threshold);
}

template <typename T>
void quick_sort(std::vector<T> & vector, std::size_t threshold)
{
    if (vector.size() <= 1) return;
    quick_sort_step(vector, 0, vector.size() - 1, threshold);
}

///////////////////////////////////////////////////////////////////////////////////////////////

static std::vector<double> make_reverse_sorted(std::size_t n)
{
    std::vector<double> v(n);
    std::iota(v.begin(), v.end(), 1.0);
    std::reverse(v.begin(), v.end());
    return v;
}

///////////////////////////////////////////////////////////////////////////////////////////////

static void BM_QuickSort(benchmark::State & state)
{
    auto threshold = static_cast<std::size_t>(state.range(0));
    auto original  = make_reverse_sorted(10000);

    for (auto _ : state)
    {
        state.PauseTiming();
        auto v = original;
        state.ResumeTiming();

        quick_sort(v, threshold);

        benchmark::DoNotOptimize(v.data());
        benchmark::ClobberMemory();
    }
}

BENCHMARK(BM_QuickSort)
    ->Arg(1)
    ->Arg(4)
    ->Arg(8)
    ->Arg(16)
    ->Arg(32)
    ->Arg(64)
    ->Arg(128)
    ->Arg(256)
    ->Unit(benchmark::kMicrosecond);

///////////////////////////////////////////////////////////////////////////////////////////////


/*
Threshold   Time        Note
1           56 µs       pure quicksort, too many recursive calls
4           44 µs       better
8           42 µs       optimal
16          44 µs       slightly worse
32          53 µs       insertion sort on large segments — slow
256         267 µs      almost pure insertion sort — very slow for 10k elements
*/