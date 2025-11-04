////////////////////////////////////////////////////////////////////////////////////

// support : www.cs.usfca.edu/~galles/visualization/ComparisonSort.html
//@compile clang++ part2/02_10.cpp -std=c++2b -Wall -Wextra -Wpedantic

////////////////////////////////////////////////////////////////////////////////////

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <vector>
#include <iostream>
#include <random>
#include <chrono>

////////////////////////////////////////////////////////////////////////////////////
void order(std::vector < int > & vector, std::size_t left, std::size_t right)
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

void merge(std::vector < int > & vector_1, std::size_t left, std::size_t right)
{
	auto middle = std::midpoint(left, right), size = right - left;

	std::vector < int > vector_2(size, 0);

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

void split(std::vector < int > & vector, std::size_t left, std::size_t right)
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

void merge_sort(std::vector < int > & vector)
{
	split(vector, 0, std::size(vector));
}

////////////////////////////////////////////////////////////////////////////////////
int median_of_three(std::vector<int>& vec, std::size_t left, std::size_t right)
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

void quick_sort_step(std::vector<int>& vec, std::size_t left, std::size_t right)
{   
	if (right - left < 16) {
        return order(vec, left, right + 1);
    }

    int pivot = median_of_three(vec, left, right);
    
    std::size_t i = left;
    std::size_t j = right;
    
    while (true)
    {
        while (vec[i] < pivot)
            ++i;
        
        while (vec[j] > pivot)
            --j;
        
        if (i >= j)
            break;
        
        std::swap(vec[i], vec[j]);
        ++i;
        --j;
    }
    
    quick_sort_step(vec, left, j);
    quick_sort_step(vec, j + 1, right);
}

void quick_sort(std::vector < int > & vector)
{
    if (vector.size() <= 1) {
        return;
    }
    
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

////////////////////////////////////////////////////////////////////////////////////
int main() {
    std::cout << "=== SORTING ALGORITHMS TESTING ===" << std::endl;
    
    // Test 1: Small vectors for visual verification
    std::cout << "\n--- TEST 1: Small vectors (size 20) ---" << std::endl;
    
    auto small_vector = generate_random_vector(20, 1);
    std::cout << "Original small vector:" << std::endl;
    print_vector(small_vector, "Original");
    
    // Test merge_sort
    {
        auto vec_merge = small_vector;
        std::cout << "\n--- Merge Sort ---" << std::endl;
        merge_sort(vec_merge);
        print_vector(vec_merge, "Sorted");
        std::cout << "Verification: " << (std::ranges::is_sorted(vec_merge) ? "SUCCESS" : "FAILED") << std::endl;
    }
    
    // Test quick_sort
    {
        auto vec_quick = small_vector;
        std::cout << "\n--- Quick Sort ---" << std::endl;
        quick_sort(vec_quick);
        print_vector(vec_quick, "Sorted");
        std::cout << "Verification: " << (std::ranges::is_sorted(vec_quick) ? "SUCCESS" : "FAILED") << std::endl;
    }
    
    // Test 2: Large vectors for performance measurement
    std::cout << "\n--- TEST 2: Large vectors (size 100,000) ---" << std::endl;
    
    const size_t LARGE_SIZE = 100000;
    auto large_vector = generate_random_vector(LARGE_SIZE, 1);
    
    test_sort_algorithm(large_vector, "Merge Sort on random array", merge_sort);
    test_sort_algorithm(large_vector, "Quick Sort on random array", quick_sort);
    
    // Test 3: Special cases with large arrays (100,000 elements)
    std::cout << "\n--- TEST 3: Special cases (size 100,000) ---" << std::endl;
    
    const size_t SPECIAL_SIZE = 100000;
    
    // Already sorted array
    std::cout << "\n--- Already Sorted Array ---" << std::endl;
    auto sorted_vec = generate_sorted_vector(SPECIAL_SIZE);
    test_sort_algorithm(sorted_vec, "Merge Sort", merge_sort);
    test_sort_algorithm(sorted_vec, "Quick Sort", quick_sort);
    
    // Reverse sorted array
    std::cout << "\n--- Reverse Sorted Array ---" << std::endl;
    auto reverse_sorted = generate_reverse_sorted_vector(SPECIAL_SIZE);
    test_sort_algorithm(reverse_sorted, "Merge Sort", merge_sort);
    test_sort_algorithm(reverse_sorted, "Quick Sort", quick_sort);
    
    // Array with duplicate elements
    std::cout << "\n--- Array with Duplicates ---" << std::endl;
    auto duplicates_vec = generate_vector_with_duplicates(SPECIAL_SIZE, 100);
    test_sort_algorithm(duplicates_vec, "Merge Sort", merge_sort);
    test_sort_algorithm(duplicates_vec, "Quick Sort", quick_sort);
    
    std::cout << "\n=== TESTING COMPLETED ===" << std::endl;
    
    return 0;
}
////////////////////////////////////////////////////////////////////////////////////