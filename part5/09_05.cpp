//@compile g++ part5/09_05.cpp -std=c++20 -Wall -Wextra -Wpedantic
#include <cassert>
#include <iterator>
#include <memory>
#include <type_traits>

///////////////////////////////////////////////////////////////////////////////////////////////

template < typename T >
class List
{
private:

    struct Node
    {
        T value = T();

        std::shared_ptr < Node > next;
        std::weak_ptr   < Node > prev;
    };

public:

    class Iterator
    {
    public:

        using iterator_category = std::bidirectional_iterator_tag;

        //  -------------------------------------------------------------------

        Iterator(std::shared_ptr < Node > node = nullptr,
                 std::shared_ptr < Node > tail = nullptr)
            : m_node(node), m_prev(tail) {}

        //  -------------------------------------------------------------------

        auto const operator++(int)
        {
            auto x = *this;
            m_prev = m_node;
            m_node = m_node->next;
            return x;
        }

        auto & operator++()
        {
            m_prev = m_node;
            m_node = m_node->next;
            return *this;
        }

        //  -------------------------------------------------------------------

        auto const operator--(int)
        {
            auto x = *this;
            m_node = m_node ? m_node->prev.lock() : m_prev.lock();
            return x;
        }

        auto & operator--()
        {
            m_node = m_node ? m_node->prev.lock() : m_prev.lock();
            return *this;
        }

        //  -------------------------------------------------------------------

        auto & operator* () const { return  m_node->value; }
        auto   operator->() const { return &m_node->value; }

        //  -------------------------------------------------------------------

        friend auto operator==(Iterator const & lhs, Iterator const & rhs)
        {
            return lhs.m_node == rhs.m_node;
        }

    private:

        std::shared_ptr < Node > m_node;
        std::weak_ptr   < Node > m_prev;
    };

    //  -----------------------------------------------------------------------

    auto begin() const { return Iterator(m_head        ); }
    auto end  () const { return Iterator(nullptr, m_tail); }

    //  -----------------------------------------------------------------------

    void push_back(T value)
    {
        auto node = std::make_shared < Node > (value);

        if (m_tail)
        {
            node->prev   = m_tail;
            m_tail->next = node;
            m_tail       = node;
        }
        else
        {
            m_head = node;
            m_tail = node;
        }
    }

private:

    std::shared_ptr < Node > m_head;
    std::shared_ptr < Node > m_tail;
};

///////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    List < int > list;

    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    //  -----------------------------------------------------------------------

    for (auto it = std::begin(list); it != std::end(list); ++it);

    //  -----------------------------------------------------------------------

    for (auto it = std::end(list); it != std::begin(list); )
    {
        --it;
    }

    //  -----------------------------------------------------------------------

    for ([[maybe_unused]] auto element : list);

    //  -----------------------------------------------------------------------

    auto first = std::begin(list);
    auto last  = std::end  (list);

    assert(*first  == 1);
    assert(*++first == 2);
    assert(*--last  == 3);
    assert(*--last  == 2);

    //  -----------------------------------------------------------------------

    static_assert(std::is_same_v <
        List < int > ::Iterator::iterator_category,
        std::bidirectional_iterator_tag > );
}

///////////////////////////////////////////////////////////////////////////////////////////////
