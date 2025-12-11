//@compile clang++ part3/04_07.cpp -std=c++2b -Wall -Wextra -Wpedantic
#include <iostream>
#include <numeric>

//////////////////////////////////////////////////////////////////////////////////////////

template < int N = 0, int D = 1 > struct Ratio
{
    static_assert(D != 0, "Denominator cannot be zero");

    constexpr static auto num = N;
    constexpr static auto den = D;
};

//////////////////////////////////////////////////////////////////////////////////////////

template < typename R1, typename R2 > struct Sum
{
private:
    constexpr static auto raw_num = R1::num * R2::den + R2::num * R1::den;
    constexpr static auto raw_den = R1::den * R2::den;

    constexpr static auto common = std::gcd(raw_num, raw_den);

    constexpr static auto final_num = (raw_den / common < 0) ? -(raw_num / common) : (raw_num / common);
    constexpr static auto final_den = (raw_den / common < 0) ? -(raw_den / common) : (raw_den / common);

public:
    using type = Ratio < final_num, final_den >;
};

//////////////////////////////////////////////////////////////////////////////////////////

template < typename R1, typename R2 > struct Mul
{
private:
    constexpr static auto raw_num = R1::num * R2::num;
    constexpr static auto raw_den = R1::den * R2::den;

    constexpr static auto common = std::gcd(raw_num, raw_den);

    constexpr static auto final_num = (raw_den / common < 0) ? -(raw_num / common) : (raw_num / common);
    constexpr static auto final_den = (raw_den / common < 0) ? -(raw_den / common) : (raw_den / common);

public:
    using type = Ratio < final_num, final_den >;
};

//////////////////////////////////////////////////////////////////////////////////////////

// Implements subtraction using Sum: A - B = A + (-B)
template < typename R1, typename R2 > struct Sub
{
    using neg_R2 = Ratio < -R2::num, R2::den >;
    using type = typename Sum < R1, neg_R2 >::type;
};

//////////////////////////////////////////////////////////////////////////////////////////

// Implements division using Mul: A / B = A * (1/B)
template < typename R1, typename R2 > struct Div
{
    static_assert(R2::num != 0, "Division by zero");

    using inv_R2 = Ratio < R2::den, R2::num >;
    using type = typename Mul < R1, inv_R2 >::type;
};

//////////////////////////////////////////////////////////////////////////////////////////

template < typename R1, typename R2 > using sum = typename Sum < R1, R2 >::type;
template < typename R1, typename R2 > using sub = typename Sub < R1, R2 >::type;
template < typename R1, typename R2 > using mul = typename Mul < R1, R2 >::type;
template < typename R1, typename R2 > using divide = typename Div < R1, R2 >::type;

//////////////////////////////////////////////////////////////////////////////////////////

template < typename T, typename R = Ratio < 1 > > struct Duration 
{
    T x = T();
    
    constexpr Duration(T v = T()) : x(v) {}
};

//////////////////////////////////////////////////////////////////////////////////////////

template 
< 
    typename T1, typename R1, 
    typename T2, typename R2 
>
constexpr auto operator+(Duration < T1, R1 > const & lhs, Duration < T2, R2 > const & rhs)
{
    using ratio_t = Ratio < 1, sum < R1, R2 >::den >;

    auto x = 
    (
        lhs.x * ratio_t::den / R1::den * R1::num +
        rhs.x * ratio_t::den / R2::den * R2::num
    );

    return Duration < decltype(x), ratio_t > (x);
}

//////////////////////////////////////////////////////////////////////////////////////////

template 
< 
    typename T1, typename R1, 
    typename T2, typename R2 
>
constexpr auto operator-(Duration < T1, R1 > const & lhs, Duration < T2, R2 > const & rhs)
{
    return lhs + Duration<T2, R2>(-rhs.x);
}

//////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    // Test Sum with reduction: 1/2 + 1/6 = 2/3
    using R_Sum = sum<Ratio<1, 2>, Ratio<1, 6>>;
    static_assert(R_Sum::num == 2 && R_Sum::den == 3, "Sum logic error");

    // Test Sub: 1/2 - 1/3 = 1/6
    using R_Sub = sub<Ratio<1, 2>, Ratio<1, 3>>;
    static_assert(R_Sub::num == 1 && R_Sub::den == 6, "Sub logic error");

    // Test Mul with reduction: 3/4 * 2/3 = 1/2
    using R_Mul = mul<Ratio<3, 4>, Ratio<2, 3>>;
    static_assert(R_Mul::num == 1 && R_Mul::den == 2, "Mul logic error");

    // Test Div: (1/2) / (3/4) = 2/3
    // Updated usage from 'div' to 'divide'
    using R_Div = divide<Ratio<1, 2>, Ratio<3, 4>>;
    static_assert(R_Div::num == 2 && R_Div::den == 3, "Div logic error");

    // Test Duration operators
    constexpr Duration < int, Ratio < 1, 2 > > d1(10); // 5.0 units
    constexpr Duration < int, Ratio < 1, 4 > > d2(5);  // 1.25 units

    constexpr auto d_sum = d1 + d2; // 6.25 units
    constexpr auto d_sub = d1 - d2; // 3.75 units

    static_assert(d_sum.x == 25, "Duration operator+ error");
    static_assert(d_sub.x == 15, "Duration operator- error");

    std::cout << "All static assertions passed." << std::endl;

    return 0;
}