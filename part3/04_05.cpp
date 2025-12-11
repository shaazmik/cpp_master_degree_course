//@compile clang++ part3/04_05.cpp -std=c++2b -Wall -Wextra -Wpedantic
#include <limits>
#include <type_traits>

////////////////////////////////////////////////////////////////////////////////
template <int N>
struct Fibonacci {
    static_assert(N > 0, "Fibonacci number index must be positive");

    static constexpr int value = Fibonacci<N - 1>::value + Fibonacci<N - 2>::value;

    static_assert(value >= 0, "Integer overflow in Fibonacci calculation");
};

template <>
struct Fibonacci<1> {
    static constexpr int value = 1;
};

template <>
struct Fibonacci<2> {
    static constexpr int value = 1;
};

////////////////////////////////////////////////////////////////////////////////
template <int N>
inline constexpr int Fibonacci_v = Fibonacci<N>::value;

////////////////////////////////////////////////////////////////////////////////
int main() {
    static_assert(Fibonacci<1>::value == 1,     "Test failed");
    static_assert(Fibonacci<2>::value == 1,     "Test failed");
    static_assert(Fibonacci<3>::value == 2,     "Test failed");
    static_assert(Fibonacci<4>::value == 3,     "Test failed");
    static_assert(Fibonacci<5>::value == 5,     "Test failed");
    static_assert(Fibonacci<10>::value == 55,   "Test failed");
    static_assert(Fibonacci<20>::value == 6765, "Test failed");

    constexpr int fib10 = Fibonacci_v<10>;
    constexpr int fib20 = Fibonacci_v<20>;

    return 0;
}
