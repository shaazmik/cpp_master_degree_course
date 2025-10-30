//@compile clang++ part2/02_09.cpp -std=c++17 -Wall -Wextra -Wpedantic
#include <cassert>
#include <numeric>
#include <iostream>
#include <vector> 

int gcd_recursive(int a, int b) {
    if (b == 0) {
        return a;
    }
    return gcd_recursive(b, a % b);
}

int gcd_iterative(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int lcm(int a, int b) {
    return (a / gcd_iterative(a, b)) * b;
}

void run_unit_tests() {
    struct TestCase {
        int a, b;
        std::string desc;
    };
    
    std::vector<TestCase> tests = {
        {228, 98, "Basic case #1"},
        {48, 180, "Different factors"},
        {48, 56, "Basic case #2"},
        {17, 31, "Prime numbers"},
        {0, 5, "With zero"},
        {1337, 1488, "Medium numbers"},
        {42, 42, "Equal numbers"}
    };
    
    std::cout << "RUNNING UNIT TESTS\n";
    std::cout << "==================\n";
    
    for (const auto& test : tests) {
        std::cout << "Test: " << test.desc << " (" << test.a << ", " << test.b << ")\n";
        
        int gcd_rec = gcd_recursive(test.a, test.b);
        int gcd_iter = gcd_iterative(test.a, test.b);
        int lcm_val = lcm(test.a, test.b);
        
        int std_gcd = std::gcd(test.a, test.b);
        int std_lcm = std::lcm(test.a, test.b);
        
        std::cout << "  GCD recursive: " << gcd_rec << " " 
                  << (gcd_rec == std_gcd ? "✓" : "✗") << "\n";
        std::cout << "  GCD iterative: " << gcd_iter << " " 
                  << (gcd_iter == std_gcd ? "✓" : "✗") << "\n";
        std::cout << "  LCM: " << lcm_val << " " 
                  << (lcm_val == std_lcm ? "✓" : "✗") << "\n";
        
        assert(gcd_rec == std_gcd);
        assert(gcd_iter == std_gcd);
        assert(lcm_val == std_lcm);
        
        std::cout << "  PASSED\n\n";
    }

    std::cout << "All unit tests passed! ✓\n" << std::endl;
}

int main() {
    run_unit_tests();
    return 0;
}