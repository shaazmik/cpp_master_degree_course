//@compile clang++ part3/04_08.cpp -std=c++2b -Wall -Wextra -Wpedantic

#include <cassert>
#include <cstddef>
#include <utility>

//////////////////////////////////////////////////////////////////

template < typename ... Ts > class Tuple;

//////////////////////////////////////////////////////////////////

template <> class Tuple <>
{
public:
    constexpr Tuple() = default;

    static constexpr std::size_t size()
    {
        return 0;
    }
};

//////////////////////////////////////////////////////////////////

template < typename T, typename ... Ts > class Tuple < T, Ts ... >
{
public :

    constexpr Tuple(T && x, Ts && ... xs)
    :
        m_head(std::forward < T  > (x )   ),
        m_tail(std::forward < Ts > (xs)...)
    {}

//  ------------------------------------------------

    template < std::size_t I > 
    constexpr auto get() const
    {
        if constexpr (I > 0)
        {
            return m_tail.template get < I - 1 > ();
        }
        else
        {
            return m_head;
        }
    }

//  ------------------------------------------------

    static constexpr std::size_t size()
    {
        return 1 + sizeof...(Ts);
    }

private :

    T m_head;
    
    Tuple < Ts ... > m_tail;
};

//////////////////////////////////////////////////////////////////

int main()
{
    constexpr Tuple < int, double, int > tuple(1, 2.5, 42);

//  ----------------------------------------------------------

    // compile-time tests (static_assert)

    static_assert(tuple.size() == 3, "Size check failed");

    static_assert(tuple.get < 0 > () == 1,   "Get<0> check failed");
    static_assert(tuple.get < 1 > () == 2.5, "Get<1> check failed");
    static_assert(tuple.get < 2 > () == 42,  "Get<2> check failed");

    return 0;
}