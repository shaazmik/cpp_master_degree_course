////////////////////////////////////////////////////////////////////////////////////

// support : www.cs.usfca.edu/~galles/visualization/ComparisonSort.html
//@compile clang++ part3/04_01.cpp -std=c++2b -Wall -Wextra -Wpedantic

////////////////////////////////////////////////////////////////////////////////////

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <vector>
#include <iostream>
#include <random>
#include <chrono>

////////////////////////////////////////////////////////////////////////////////////
template <typename T>
void order(std::vector<T>& vector, std::size_t left, std::size_t right)
{
    for (auto i = left + 1; i < right; ++i)
    {
        for (auto j = i; j > left; --j)
        {
            if (vector[j - 1] > vector[j])
            {
                std::swap(vector[j], vector[j - 1]);
            }
        }
    }
}

template <typename T>
void merge(std::vector<T>& vector_1, std::size_t left, std::size_t right)
{
	auto middle = std::midpoint(left, right), size = right - left;

	std::vector < T > vector_2(size, 0);

	for (auto i = left, j = middle, k = 0uz; k < size; ++k) 
	{
		if (i < middle && ((j < right && vector_1[i] <= vector_1[j]) || j == right))
		{
			vector_2[k] = vector_1[i++];
		}
		else
		{
			vector_2[k] = vector_1[j++];
		}
	}

	for (auto i = 0uz, j = 0uz; j < size; ++j) 
	{
		vector_1[left + i++] = vector_2[j];
	}
}

template <typename T>
void split(std::vector<T>& vector, std::size_t left, std::size_t right)
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
		order(vector, left,   right );
	}
}

template <typename T>
void merge_sort(std::vector<T>& vector)
{
	split(vector, 0, std::size(vector));
}

////////////////////////////////////////////////////////////////////////////////////
template <typename T>
T median_of_three(std::vector<T>& vec, std::size_t left, std::size_t right)
{
    std::size_t mid = left + (right - left) / 2;

    if (vec[left] > vec[mid])
        std::swap(vec[left], vec[mid]);
    if (vec[left] > vec[right])
        std::swap(vec[left], vec[right]);
    if (vec[mid] > vec[right])
        std::swap(vec[mid], vec[right]);

    return vec[mid]; 
}

template <typename T>
void quick_sort_step(std::vector<T>& vec, std::size_t left, std::size_t right)
{
    if (right - left < 16) {
        return order(vec, left, right + 1);
    }

    T pivot = median_of_three(vec, left, right); // pivot того же типа, что и элементы

    std::size_t i = left;
    std::size_t j = right;

    while (true)
    {
        while (vec[i] < pivot) ++i;
        while (vec[j] > pivot) --j;

        if (i >= j) break;

        std::swap(vec[i], vec[j]);
        ++i;
        --j;
    }

    quick_sort_step(vec, left, j);
    quick_sort_step(vec, j + 1, right);
}

template <typename T>
void quick_sort(std::vector<T>& vector)
{
    if (vector.size() <= 1) return;
    quick_sort_step(vector, 0, vector.size() - 1);
}

////////////////////////////////////////////////////////////////////////////////////
std::vector<int> generate_random_vector(size_t size, int min_val = 1) {
    std::vector<int> result(size);
    std::iota(result.begin(), result.end(), min_val);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(result.begin(), result.end(), gen);

    return result;
}

std::vector<int> generate_sorted_vector(size_t size) {
    std::vector<int> result(size);
    for (size_t i = 0; i < size; ++i) {
        result[i] = static_cast<int>(i) + 1;
    }
    return result;
}

std::vector<int> generate_reverse_sorted_vector(size_t size) {
    std::vector<int> result(size);
    for (size_t i = 0; i < size; ++i) {
        result[i] = static_cast<int>(size - i);
    }
    return result;
}

std::vector<int> generate_vector_with_duplicates(size_t size, int distinct_values_count = 100) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1, distinct_values_count);
    
    std::vector<int> result(size);
    for (size_t i = 0; i < size; ++i) {
        result[i] = dist(gen);
    }
    return result;
}

void print_vector(const std::vector<int>& vec, const std::string& name) {
    std::cout << name << ": ";
    for (size_t i = 0; i < std::min(vec.size(), size_t(20)); ++i) {
        std::cout << vec[i] << " ";
    }
    if (vec.size() > 20) {
        std::cout << "...";
    }
    std::cout << std::endl;
}

void test_sort_algorithm(const std::vector<int>& original, const std::string& algorithm_name, 
                         void (*sort_function)(std::vector<int>&)) {
    auto vec = original;
    auto start = std::chrono::high_resolution_clock::now();
    
    sort_function(vec);
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::cout << algorithm_name << ": " << duration.count() << " ms" << std::endl;
    std::cout << "Verification: " << (std::ranges::is_sorted(vec) ? "SUCCESS" : "FAILED") << std::endl;
}

int main()
{
    std::cout << "=== TEMPLATE SORTING TEST ===\n";

    // -------------------------------
    // Test 1: int
    // -------------------------------
    std::vector<int> vi = {5, 1, 9, 3, 7, 2};

    std::cout << "\nOriginal int vector: ";
    for (auto& x : vi) std::cout << x << " ";

    auto int_merge = vi;
    merge_sort(int_merge);

    std::cout << "\nMerge sorted:        ";
    for (auto& x : int_merge) std::cout << x << " ";

    auto int_quick = vi;
    quick_sort(int_quick);

    std::cout << "\nQuick sorted:        ";
    for (auto& x : int_quick) std::cout << x << " ";
    std::cout << "\n";


    // -------------------------------
    // Test 2: double
    // -------------------------------
    std::vector<double> vd = {2.5, 1.1, 3.14, -0.7, 2.5};

    std::cout << "\nOriginal double vector: ";
    for (auto& x : vd) std::cout << x << " ";

    auto double_merge = vd;
    merge_sort(double_merge);

    std::cout << "\nMerge sorted:           ";
    for (auto& x : double_merge) std::cout << x << " ";

    auto double_quick = vd;
    quick_sort(double_quick);

    std::cout << "\nQuick sorted:           ";
    for (auto& x : double_quick) std::cout << x << " ";
    std::cout << "\n";


    // -------------------------------
    // Test 3: string
    // -------------------------------
    std::vector<std::string> vs = {"www", "apple", "banana", "qqq"};

    std::cout << "\nOriginal string vector: ";
    for (auto& x : vs) std::cout << x << " ";

    auto string_merge = vs;
    merge_sort(string_merge);

    std::cout << "\nMerge sorted:           ";
    for (auto& x : string_merge) std::cout << x << " ";

    auto string_quick = vs;
    quick_sort(string_quick);

    std::cout << "\nQuick sorted:           ";
    for (auto& x : string_quick) std::cout << x << " ";

    std::cout << "\n\n=== DONE ===\n";
    return 0;
}
////////////////////////////////////////////////////////////////////////////////////