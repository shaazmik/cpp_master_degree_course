//@compile g++ part4/08_03.cpp -std=c++20 -Wall -Wextra -Wpedantic
#include <cassert>
#include <climits>
#include <cmath>
#include <iostream>

///////////////////////////////////////////////////////////////////////////////////////////////

int ilog2(int n)
{
    unsigned int u = static_cast<unsigned int>(n);
    int result = 0;
    while (u >>= 1) ++result;
    return result;
}

///////////////////////////////////////////////////////////////////////////////////////////////

int ilog2(float n)
{
    union { float f; unsigned int u; } fb;
    fb.f = n;

    unsigned int exponent = (fb.u >> 23) & 0xFFu;

    //  -------------------------------------------------------------------------------------------
    // inf / nan: exponent == 255 — undefined, return INT_MIN as sentinel

    if (exponent == 0xFFu) return INT_MIN;

    //  -------------------------------------------------------------------------------------------
    // normalized: value = 1.mantissa * 2^(exp-127), so floor(log2) = exp - 127

    if (exponent != 0u) return static_cast<int>(exponent) - 127;

    //  -------------------------------------------------------------------------------------------
    // denormalized: exp == 0, value = 0.mantissa * 2^(-126) = mantissa * 2^(-149)
    // floor(log2) = bit_position_of_highest_bit(mantissa) - 149

    unsigned int mantissa = fb.u & 0x7FFFFFu;
    int bit_pos = 0;
    while (mantissa >>= 1) ++bit_pos;
    return bit_pos - 149;
}

///////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    //  -------------------------------------------------------------------------------------------
    // int: floor(log2)

    assert(ilog2(1)          == 0);
    assert(ilog2(2)          == 1);
    assert(ilog2(3)          == 1);
    assert(ilog2(4)          == 2);
    assert(ilog2(7)          == 2);
    assert(ilog2(8)          == 3);
    assert(ilog2(1024)       == 10);
    assert(ilog2(0x7FFFFFFF) == 30);    // INT_MAX = 2^31 - 1

    std::cout << "ilog2(int) ok\n";

    //  -------------------------------------------------------------------------------------------
    // float: normalized numbers

    assert(ilog2(1.0f)   ==   0);   // exp = 127 → 127-127 = 0
    assert(ilog2(2.0f)   ==   1);   // exp = 128
    assert(ilog2(3.0f)   ==   1);   // exp = 128, floor(log2(3)) = 1
    assert(ilog2(4.0f)   ==   2);
    assert(ilog2(0.5f)   ==  -1);   // exp = 126 → 126-127 = -1
    assert(ilog2(0.75f)  ==  -1);   // exp = 126
    assert(ilog2(0.25f)  ==  -2);   // exp = 125

    //  -------------------------------------------------------------------------------------------
    // float: denormalized numbers (very small, exp == 0)
    // smallest positive float: mantissa = 1 → bit_pos = 0 → 0 - 149 = -149

    float smallest_denorm = 1.401298e-45f;          // 2^(-149)
    assert(ilog2(smallest_denorm) == -149);

    // mantissa with highest bit at position 22: value = 2^(-127)
    float near_normalized = std::ldexp(1.0f, -127); // smallest normalized is 2^(-126)
    assert(ilog2(near_normalized) == -127);

    std::cout << "ilog2(float) ok\n";

    //  -------------------------------------------------------------------------------------------
    // float: inf and nan return sentinel INT_MIN

    assert(ilog2(1.0f / 0.0f) == INT_MIN);      // +inf
    assert(ilog2(0.0f / 0.0f) == INT_MIN);      // nan

    std::cout << "ilog2(inf/nan) ok\n";
}

///////////////////////////////////////////////////////////////////////////////////////////////
