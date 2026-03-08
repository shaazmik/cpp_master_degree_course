//@compile clang++ part4/07_04.cpp -std=c++20 -Wall -Wextra -Wpedantic -I/usr/local/include -L/usr/local/lib -lgtest_main -lgtest -lpthread
#include <algorithm>
#include <cstddef>
#include <numeric>
#include <string>
#include <vector>

#include <gtest/gtest.h>

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
void merge(std::vector<T> & vector_1, std::size_t left, std::size_t right)
{
    auto middle = std::midpoint(left, right), size = right - left;

    std::vector<T> vector_2(size);

    for (auto i = left, j = middle, k = std::size_t(0); k < size; ++k)
    {
        if (i < middle && ((j < right && vector_1[i] <= vector_1[j]) || j == right))
            vector_2[k] = vector_1[i++];
        else
            vector_2[k] = vector_1[j++];
    }

    for (std::size_t i = 0, j = 0; j < size; ++j)
        vector_1[left + i++] = vector_2[j];
}

template <typename T>
void split(std::vector<T> & vector, std::size_t left, std::size_t right)
{
    if (right - left > 16)
    {
        auto middle = std::midpoint(left, right);
        split(vector, left,   middle);
        split(vector, middle, right );
        merge(vector, left,   right );
    }
    else
    {
        order(vector, left, right);
    }
}

template <typename T>
void merge_sort(std::vector<T> & vector)
{
    split(vector, 0, std::size(vector));
}

///////////////////////////////////////////////////////////////////////////////////////////////

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
void quick_sort_step(std::vector<T> & vec, std::size_t left, std::size_t right)
{
    if (right - left < 16)
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

    quick_sort_step(vec, left,    j);
    quick_sort_step(vec, j + 1, right);
}

template <typename T>
void quick_sort(std::vector<T> & vector)
{
    if (vector.size() <= 1) return;
    quick_sort_step(vector, 0, vector.size() - 1);
}

///////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
std::vector<T> sorted_copy_std(std::vector<T> v)
{
    std::sort(v.begin(), v.end());
    return v;
}

///////////////////////////////////////////////////////////////////////////////////////////////

TEST(MergeSort, Empty)
{
    std::vector<int> v;
    merge_sort(v);
    EXPECT_TRUE(v.empty());
}

TEST(MergeSort, SingleElement)
{
    std::vector<int> v = {42};
    merge_sort(v);
    EXPECT_EQ(v, (std::vector<int>{42}));
}

TEST(MergeSort, TwoElementsSorted)
{
    std::vector<int> v = {1, 2};
    merge_sort(v);
    EXPECT_EQ(v, (std::vector<int>{1, 2}));
}

TEST(MergeSort, TwoElementsReverse)
{
    std::vector<int> v = {2, 1};
    merge_sort(v);
    EXPECT_EQ(v, (std::vector<int>{1, 2}));
}

TEST(MergeSort, AlreadySorted)
{
    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8};
    merge_sort(v);
    EXPECT_TRUE(std::is_sorted(v.begin(), v.end()));
}

TEST(MergeSort, ReverseSorted)
{
    std::vector<int> v = {8, 7, 6, 5, 4, 3, 2, 1};
    merge_sort(v);
    EXPECT_TRUE(std::is_sorted(v.begin(), v.end()));
}

TEST(MergeSort, Duplicates)
{
    std::vector<int> v = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
    auto expected = sorted_copy_std(v);
    merge_sort(v);
    EXPECT_EQ(v, expected);
}

TEST(MergeSort, LargeRandom)
{
    std::vector<int> v(1000);
    std::iota(v.begin(), v.end(), 0);
    std::reverse(v.begin(), v.end());
    auto expected = sorted_copy_std(v);
    merge_sort(v);
    EXPECT_EQ(v, expected);
}

TEST(MergeSort, Doubles)
{
    std::vector<double> v = {3.14, -1.5, 2.7, 0.0, -3.14, 2.7};
    auto expected = sorted_copy_std(v);
    merge_sort(v);
    EXPECT_EQ(v, expected);
}

TEST(MergeSort, Strings)
{
    std::vector<std::string> v = {"banana", "apple", "cherry", "apple", "date"};
    auto expected = sorted_copy_std(v);
    merge_sort(v);
    EXPECT_EQ(v, expected);
}

///////////////////////////////////////////////////////////////////////////////////////////////

TEST(QuickSort, Empty)
{
    std::vector<int> v;
    quick_sort(v);
    EXPECT_TRUE(v.empty());
}

TEST(QuickSort, SingleElement)
{
    std::vector<int> v = {42};
    quick_sort(v);
    EXPECT_EQ(v, (std::vector<int>{42}));
}

TEST(QuickSort, TwoElementsSorted)
{
    std::vector<int> v = {1, 2};
    quick_sort(v);
    EXPECT_EQ(v, (std::vector<int>{1, 2}));
}

TEST(QuickSort, TwoElementsReverse)
{
    std::vector<int> v = {2, 1};
    quick_sort(v);
    EXPECT_EQ(v, (std::vector<int>{1, 2}));
}

TEST(QuickSort, AlreadySorted)
{
    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8};
    quick_sort(v);
    EXPECT_TRUE(std::is_sorted(v.begin(), v.end()));
}

TEST(QuickSort, ReverseSorted)
{
    std::vector<int> v = {8, 7, 6, 5, 4, 3, 2, 1};
    quick_sort(v);
    EXPECT_TRUE(std::is_sorted(v.begin(), v.end()));
}

TEST(QuickSort, Duplicates)
{
    std::vector<int> v = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
    auto expected = sorted_copy_std(v);
    quick_sort(v);
    EXPECT_EQ(v, expected);
}

TEST(QuickSort, LargeRandom)
{
    std::vector<int> v(1000);
    std::iota(v.begin(), v.end(), 0);
    std::reverse(v.begin(), v.end());
    auto expected = sorted_copy_std(v);
    quick_sort(v);
    EXPECT_EQ(v, expected);
}

TEST(QuickSort, Doubles)
{
    std::vector<double> v = {3.14, -1.5, 2.7, 0.0, -3.14, 2.7};
    auto expected = sorted_copy_std(v);
    quick_sort(v);
    EXPECT_EQ(v, expected);
}

TEST(QuickSort, Strings)
{
    std::vector<std::string> v = {"banana", "apple", "cherry", "apple", "date"};
    auto expected = sorted_copy_std(v);
    quick_sort(v);
    EXPECT_EQ(v, expected);
}

///////////////////////////////////////////////////////////////////////////////////////////////
