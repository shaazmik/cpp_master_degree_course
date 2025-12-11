//@compile clang++ part3/04_09.cpp -std=c++2b -Wall -Wextra -Wpedantic
#include <type_traits>

//////////////////////////////////////////////////////////////////
template < typename T, T Val > struct integral_constant
{
    static constexpr T value = Val;
};

using true_type  = integral_constant < bool, true  >;
using false_type = integral_constant < bool, false >;

//////////////////////////////////////////////////////////////////
// 1. is_class implementation
//////////////////////////////////////////////////////////////////
namespace detail
{
    template < typename T >
    struct is_class_impl
    {
        // SFINAE test
        template < typename C > static char test(int C::*);
        
        template < typename C > static long test(...);

        static constexpr bool value = sizeof(test < T > (nullptr)) == sizeof(char);
    };
}

template < typename T > 
struct is_class : integral_constant < bool, detail::is_class_impl < T >::value > {};

template < typename T >
constexpr bool is_class_v = is_class < T >::value;

//////////////////////////////////////////////////////////////////
// 2. conditional implementation
//////////////////////////////////////////////////////////////////

template < bool B, typename T, typename F > struct conditional { using type = T; };

template < typename T, typename F > struct conditional < false, T, F > { using type = F; };

template < bool B, typename T, typename F >
using conditional_t = typename conditional < B, T, F >::type;

//////////////////////////////////////////////////////////////////
// 3. add_const / remove_const implementation
//////////////////////////////////////////////////////////////////

template < typename T > struct remove_const { using type = T; };
template < typename T > struct remove_const < const T > { using type = T; };

template < typename T >
using remove_const_t = typename remove_const < T >::type;

template < typename T > struct add_const { using type = const T; };

template < typename T >
using add_const_t = typename add_const < T >::type;

//////////////////////////////////////////////////////////////////
// 4. decay implementation
//////////////////////////////////////////////////////////////////
template < typename T > struct remove_reference { using type = T; };
template < typename T > struct remove_reference < T& > { using type = T; };
template < typename T > struct remove_reference < T&& > { using type = T; };

template < typename T > struct remove_volatile { using type = T; };
template < typename T > struct remove_volatile < volatile T > { using type = T; };

template < typename T >
struct remove_cv
{
    using type = typename remove_const < typename remove_volatile < T >::type >::type;
};

// decay implementation
// (standard decay also handles array-to-pointer and function-to-pointer decay,
// but for scalar types/objects, it removes references and cv-qualifiers)
template < typename T >
struct decay
{
    using U = typename remove_reference < T >::type;
    
    using type = typename remove_cv < U >::type;
};

template < typename T >
using decay_t = typename decay < T >::type;

//////////////////////////////////////////////////////////////////

struct MyClass {};
union MyUnion {};

//////////////////////////////////////////////////////////////////

int main()
{
//  ----------------------------------------------------------
    // Tests: is_class
    
    static_assert(is_class_v < MyClass > == true,  "MyClass should be a class");
    static_assert(is_class_v < MyUnion > == true,  "Union detects as class here (no is_union check)");
    static_assert(is_class_v < int >     == false, "int is not a class");
    static_assert(is_class_v < int* >    == false, "pointer is not a class");

//  ----------------------------------------------------------
    // Tests: conditional

    using Type1 = conditional_t < true,  int, double >;
    using Type2 = conditional_t < false, int, double >;

    static_assert(std::is_same_v < Type1, int >,    "Conditional true failed");
    static_assert(std::is_same_v < Type2, double >, "Conditional false failed");

//  ----------------------------------------------------------
    // Tests: add_const / remove_const

    static_assert(std::is_same_v < add_const_t < int >, const int >,    "add_const failed");
    static_assert(std::is_same_v < remove_const_t < const int >, int >, "remove_const failed");

//  ----------------------------------------------------------
    // Tests: decay

    // Decay const int -> int
    static_assert(std::is_same_v < decay_t < const int >, int >, "decay const int failed");
    
    // Decay int& -> int
    static_assert(std::is_same_v < decay_t < int& >, int >, "decay int& failed");
    
    // Decay const int& -> int
    static_assert(std::is_same_v < decay_t < const int& >, int >, "decay const int& failed");

    return 0;
}