//@compile g++ part5/09_10.cpp -std=c++23 -Wall -Wextra -Wpedantic -I boost_1_85_0
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <memory>
#include <new>
#include <utility>
#include <vector>

#include <boost/noncopyable.hpp>

///////////////////////////////////////////////////////////////////////////////////////////////

class Allocator : private boost::noncopyable
{
public:

    virtual ~Allocator() = default;

    virtual auto allocate  (std::size_t size) -> void * = 0;
    virtual void deallocate(void * x)                   = 0;

protected:

    template < typename T >
    auto get(void * x) const -> T *
    {
        return static_cast < T * > (x);
    }
};

///////////////////////////////////////////////////////////////////////////////////////////////

class ArenaAllocator : public Allocator
{
public:

    explicit ArenaAllocator(std::size_t size) : m_size(size)
    {
        m_begin = operator new(m_size, std::align_val_t(s_alignment));
    }

    ~ArenaAllocator() override
    {
        operator delete(m_begin, m_size, std::align_val_t(s_alignment));
    }

    //  -------------------------------------------------------------------

    auto allocate(std::size_t size) -> void * override
    {
        void * begin = get < std::byte > (m_begin) + m_offset;

        auto free = m_size - m_offset;

        if (begin = std::align(s_alignment, size, begin, free); begin)
        {
            m_offset = m_size - free + size;

            return begin;
        }

        return nullptr;
    }

    void deallocate(void *) override {}

private:

    std::size_t m_size = 0, m_offset = 0;

    void * m_begin = nullptr;

    static inline auto s_alignment = alignof(std::max_align_t);
};

///////////////////////////////////////////////////////////////////////////////////////////////

class StackAllocator : public Allocator
{
public:

    explicit StackAllocator(std::size_t size) : m_size(size)
    {
        m_begin = operator new(m_size, std::align_val_t(s_alignment));
    }

    ~StackAllocator() override
    {
        operator delete(m_begin, m_size, std::align_val_t(s_alignment));
    }

    //  -------------------------------------------------------------------

    auto allocate(std::size_t size) -> void * override
    {
        void * begin = get < std::byte > (m_begin) + m_offset + sizeof(header_t);

        auto free = m_size - m_offset - sizeof(header_t);

        if (begin = std::align(s_alignment, size, begin, free); begin)
        {
            auto header = get < header_t > (get < std::byte > (begin) - sizeof(header_t));

            *header = static_cast < header_t >
            (
                std::distance(get < std::byte > (m_begin) + m_offset,
                              get < std::byte > (begin))
            );

            m_offset = get < std::byte > (begin) - get < std::byte > (m_begin) + size;

            return begin;
        }

        return nullptr;
    }

    void deallocate(void * x) override
    {
        auto header = get < header_t > (get < std::byte > (x) - sizeof(header_t));

        m_offset = get < std::byte > (x) - get < std::byte > (m_begin) - *header;
    }

private:

    using header_t = std::uint8_t;

    std::size_t m_size = 0, m_offset = 0;

    void * m_begin = nullptr;

    static inline auto s_alignment = alignof(std::max_align_t);
};

///////////////////////////////////////////////////////////////////////////////////////////////

class ListAllocator : public Allocator
{
public:

    explicit ListAllocator(std::size_t size, std::size_t step) :
        m_size(size), m_step(step)
    {
        assert(m_size % m_step == 0 && m_step >= sizeof(Node));

        resize();

        m_begin = m_head;
    }

    ~ListAllocator() override
    {
        for (auto list : m_lists)
            operator delete(list, m_size, std::align_val_t(s_alignment));
    }

    //  -------------------------------------------------------------------

    auto allocate(std::size_t) -> void * override
    {
        if (!m_head)
        {
            if (m_offset == std::size(m_lists)) resize();
            else                                m_head = get < Node > (m_lists[++m_offset - 1]);
        }

        auto node = m_head;

        if (!node->next)
        {
            auto next = get < std::byte > (node) + m_step;

            if (next != get < std::byte > (m_lists[m_offset - 1]) + m_size)
            {
                m_head       = get < Node > (next);
                m_head->next = nullptr;
            }
            else
            {
                m_head = m_head->next;
            }
        }
        else
        {
            m_head = m_head->next;
        }

        return node;
    }

    void deallocate(void * x) override
    {
        auto node  = get < Node > (x);
        node->next = m_head;
        m_head     = node;
    }

private:

    struct Node { Node * next = nullptr; };

    //  -------------------------------------------------------------------

    void resize()
    {
        m_head = get < Node > (operator new(m_size, std::align_val_t(s_alignment)));

        m_head->next = nullptr;

        ++m_offset;

        m_lists.push_back(m_head);
    }

    //  -------------------------------------------------------------------

    std::size_t m_size = 0, m_step = 0, m_offset = 0;

    void * m_begin = nullptr;
    Node * m_head  = nullptr;

    std::vector < void * > m_lists;

    static inline auto s_alignment = alignof(std::max_align_t);
};

///////////////////////////////////////////////////////////////////////////////////////////////

class ChainAllocator : public Allocator
{
public:

    explicit ChainAllocator(std::size_t size) : m_size(size)
    {
        assert(m_size >= sizeof(Node) + 1);

        m_begin = operator new(m_size, std::align_val_t(s_alignment));

        m_head       = get < Node > (m_begin);
        m_head->size = m_size - sizeof(Header);
        m_head->next = nullptr;
    }

    ~ChainAllocator() override
    {
        operator delete(m_begin, m_size, std::align_val_t(s_alignment));
    }

    //  -------------------------------------------------------------------

    auto allocate(std::size_t size) -> void * override
    {
        void * end = get < std::byte > (m_begin) + sizeof(Header) + size, * next = end;

        auto free = 2 * alignof(Header);

        if (next = std::align(alignof(Header), sizeof(Header), next, free); next)
        {
            auto padding = get < std::byte > (next) - get < std::byte > (end);

            if (auto [current, previous] = find(size + padding); current)
            {
                if (current->size >= size + padding + sizeof(Node) + 1)
                {
                    auto step = sizeof(Header) + size + padding;
                    auto node = get < Node > (get < std::byte > (current) + step);

                    node->size    = current->size - step;
                    node->next    = current->next;
                    current->next = node;
                }
                else
                {
                    padding += current->size - size - padding;
                }

                if (!previous) m_head          = current->next;
                else           previous->next   = current->next;

                get < Header > (current)->size = size + padding;

                return get < std::byte > (current) + sizeof(Header);
            }
        }

        return nullptr;
    }

    void deallocate(void * x) override
    {
        auto node = get < Node > (get < std::byte > (x) - sizeof(Header));

        Node * previous = nullptr, * current = m_head;

        while (current)
        {
            if (node < current)
            {
                node->next = current;

                if (!previous) m_head          = node;
                else           previous->next   = node;

                break;
            }

            previous = current;
            current  = current->next;
        }

        merge(previous, node);
    }

private:

    struct Node { std::size_t size = 0; Node * next = nullptr; };

    struct alignas(std::max_align_t) Header { std::size_t size = 0; };

    //  -------------------------------------------------------------------

    auto find(std::size_t size) const -> std::pair < Node *, Node * >
    {
        Node * current = m_head, * previous = nullptr;

        while (current && size > current->size)
        {
            previous = current;
            current  = current->next;
        }

        return { current, previous };
    }

    void merge(Node * previous, Node * node) const
    {
        if (node->next &&
            get < std::byte > (node) + sizeof(Header) + node->size ==
            get < std::byte > (node->next))
        {
            node->size += sizeof(Header) + node->next->size;
            node->next  = node->next->next;
        }

        if (previous &&
            get < std::byte > (previous) + sizeof(Header) + previous->size ==
            get < std::byte > (node))
        {
            previous->size += sizeof(Header) + node->size;
            previous->next  = node->next;
        }
    }

    //  -------------------------------------------------------------------

    std::size_t m_size = 0;

    void * m_begin = nullptr;
    Node * m_head  = nullptr;

    static inline auto s_alignment = alignof(std::max_align_t);
};

///////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    ArenaAllocator arena(1'024);
    StackAllocator stack(1'024);
    ListAllocator  list (1'024, 64);
    ChainAllocator chain(1'024);

    //  -----------------------------------------------------------------------

    std::array < Allocator *, 4 > allocators = { &arena, &stack, &list, &chain };

    for (auto * alloc : allocators)
    {
        auto * p1 = alloc->allocate(16);
        auto * p2 = alloc->allocate(16);

        assert(p1 && p2);

        alloc->deallocate(p2);
        alloc->deallocate(p1);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////
