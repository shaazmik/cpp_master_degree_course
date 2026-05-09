//@compile g++ part5/09_06.cpp -std=c++20 -Wall -Wextra -Wpedantic -I boost_1_85_0
#include <cassert>

#include <boost/iterator/iterator_facade.hpp>

///////////////////////////////////////////////////////////////////////////////////////////////

class Iterator
{
public:

    Iterator() : m_a(0), m_b(1) {}

    //  -------------------------------------------------------------------

    auto & operator++()
    {
        auto t = m_a + m_b;
        m_a    = m_b;
        m_b    = t;
        return *this;
    }

    auto const operator++(int)
    {
        auto x = *this;
        ++(*this);
        return x;
    }

    //  -------------------------------------------------------------------

    auto operator*() const { return m_a; }

    //  -------------------------------------------------------------------

    friend auto operator==(Iterator const & lhs, Iterator const & rhs)
    {
        return lhs.m_a == rhs.m_a && lhs.m_b == rhs.m_b;
    }

private:

    int m_a;
    int m_b;
};

///////////////////////////////////////////////////////////////////////////////////////////////

class IteratorBoost
    : public boost::iterator_facade < IteratorBoost, int const, boost::forward_traversal_tag >
{
public:

    IteratorBoost() : m_a(0), m_b(1) {}

private:

    friend boost::iterator_core_access;

    //  -------------------------------------------------------------------

    void increment()
    {
        auto t = m_a + m_b;
        m_a    = m_b;
        m_b    = t;
    }

    auto & dereference() const { return m_a; }

    auto equal(IteratorBoost const & other) const
    {
        return m_a == other.m_a && m_b == other.m_b;
    }

    //  -------------------------------------------------------------------

    int m_a;
    int m_b;
};

///////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    {
        Iterator it;

        assert(*it++ == 0);
        assert(*it++ == 1);
        assert(*it++ == 1);
        assert(*it++ == 2);
        assert(*it++ == 3);
        assert(*it   == 5);
    }
    {
        Iterator a, b;
        assert(a == b);
        ++a;
        assert(a != b);
    }

    //  ---------------------------------------------------------------------------

    {
        IteratorBoost it;

        assert(*it++ == 0);
        assert(*it++ == 1);
        assert(*it++ == 1);
        assert(*it++ == 2);
        assert(*it++ == 3);
        assert(*it   == 5);
    }
    {
        IteratorBoost a, b;
        assert(a == b);
        ++a;
        assert(a != b);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////
