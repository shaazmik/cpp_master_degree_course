//@compile g++ part5/09_09.cpp -std=c++23 -Wall -Wextra -Wpedantic -I boost_1_85_0 -L /usr/local/lib -lbenchmark_main -lbenchmark -lpthread
#include <cassert>
#include <cstddef>
#include <new>
#include <print>
#include <utility>
#include <vector>

#include <boost/noncopyable.hpp>

#include <benchmark/benchmark.h>

///////////////////////////////////////////////////////////////////////////////////////////////

class Allocator : private boost::noncopyable
{
private:

    struct Node;

public:

    using find_t = std::pair < Node *, Node * > (Allocator::*)(std::size_t) const;

    static find_t const first_fit;
    static find_t const best_fit;

    //  -------------------------------------------------------------------

    explicit Allocator(std::size_t size, find_t find = first_fit) :
        m_size(size), m_find(find)
    {
        assert(m_size >= sizeof(Node) + 1);

        m_begin = operator new(m_size, std::align_val_t(s_alignment));

        m_head = get_node(m_begin);

        m_head->size = m_size - sizeof(Header);
        m_head->next = nullptr;
    }

    //  -------------------------------------------------------------------

    ~Allocator()
    {
        operator delete(m_begin, m_size, std::align_val_t(s_alignment));
    }

    //  -------------------------------------------------------------------

    auto allocate(std::size_t size) -> void *
    {
        void * end = get_byte(m_begin) + sizeof(Header) + size, * next = end;

        auto free = 2 * alignof(Header);

        if (next = std::align(alignof(Header), sizeof(Header), next, free); next)
        {
            auto padding = get_byte(next) - get_byte(end);

            if (auto [current, previous] = (this->*m_find)(size + padding); current)
            {
                if (current->size >= size + padding + sizeof(Node) + 1)
                {
                    auto step = sizeof(Header) + size + padding;
                    auto node = get_node(get_byte(current) + step);

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

                auto header  = get_header(current);
                header->size = size + padding;

                return get_byte(current) + sizeof(Header);
            }
        }

        return nullptr;
    }

    //  -------------------------------------------------------------------

    void deallocate(void * x)
    {
        auto node = get_node(get_byte(x) - sizeof(Header));

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

    //  -------------------------------------------------------------------

    void show() const
    {
        std::print
        (
            "Allocator::show : m_size = {} m_begin = {} m_head = {}",
            m_size, m_begin, static_cast < void * > (m_head)
        );

        if (m_head && m_head->next)
            std::print(" m_head->next = {}\n", static_cast < void * > (m_head->next));
        else
            std::print("\n");
    }

private:

    struct Node
    {
        std::size_t size = 0;
        Node * next      = nullptr;
    };

    struct alignas(std::max_align_t) Header
    {
        std::size_t size = 0;
    };

    //  -------------------------------------------------------------------

    auto get_byte(void * x) const -> std::byte *
    {
        return static_cast < std::byte * > (x);
    }

    auto get_node(void * x) const -> Node *
    {
        return static_cast < Node * > (x);
    }

    auto get_header(void * x) const -> Header *
    {
        return static_cast < Header * > (x);
    }

    //  -------------------------------------------------------------------

    auto find_first(std::size_t size) const -> std::pair < Node *, Node * >
    {
        Node * current = m_head, * previous = nullptr;

        while (current && size > current->size)
        {
            previous = current;
            current  = current->next;
        }

        return { current, previous };
    }

    auto find_best(std::size_t size) const -> std::pair < Node *, Node * >
    {
        Node * best = nullptr, * best_prev = nullptr;
        Node * current = m_head, * previous = nullptr;

        while (current)
        {
            if (current->size >= size && (!best || current->size < best->size))
            {
                best      = current;
                best_prev = previous;
            }

            previous = current;
            current  = current->next;
        }

        return { best, best_prev };
    }

    //  -------------------------------------------------------------------

    void merge(Node * previous, Node * node) const
    {
        if (node->next &&
            get_byte(node) + sizeof(Header) + node->size == get_byte(node->next))
        {
            node->size += sizeof(Header) + node->next->size;
            node->next  = node->next->next;
        }

        if (previous &&
            get_byte(previous) + sizeof(Header) + previous->size == get_byte(node))
        {
            previous->size += sizeof(Header) + node->size;
            previous->next  = node->next;
        }
    }

    //  -------------------------------------------------------------------

    std::size_t m_size = 0;

    void * m_begin = nullptr;
    Node * m_head  = nullptr;

    find_t m_find = first_fit;

    static inline auto s_alignment = alignof(std::max_align_t);
};

Allocator::find_t const Allocator::first_fit = &Allocator::find_first;
Allocator::find_t const Allocator::best_fit  = &Allocator::find_best;

///////////////////////////////////////////////////////////////////////////////////////////////

void test_first(benchmark::State & state)
{
    auto kb = 1'024uz, mb = kb * kb, gb = kb * kb * kb;

    std::vector < void * > vector(kb, nullptr);

    for ([[maybe_unused]] auto element : state)
    {
        Allocator allocator(16 * gb, Allocator::first_fit);

        for (auto i = 0uz; i < kb; ++i)
            vector[i] = allocator.allocate(mb);

        for (auto i = 0uz; i < kb; i += 32)
            allocator.deallocate(vector[i]);

        for (auto i = 0uz; i < kb; i += 32)
            vector[i] = allocator.allocate(mb);

        for (auto i = 0uz; i < kb; ++i)
            allocator.deallocate(vector[i]);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////

void test_best(benchmark::State & state)
{
    auto kb = 1'024uz, mb = kb * kb, gb = kb * kb * kb;

    std::vector < void * > vector(kb, nullptr);

    for ([[maybe_unused]] auto element : state)
    {
        Allocator allocator(16 * gb, Allocator::best_fit);

        for (auto i = 0uz; i < kb; ++i)
            vector[i] = allocator.allocate(mb);

        for (auto i = 0uz; i < kb; i += 32)
            allocator.deallocate(vector[i]);

        for (auto i = 0uz; i < kb; i += 32)
            vector[i] = allocator.allocate(mb);

        for (auto i = 0uz; i < kb; ++i)
            allocator.deallocate(vector[i]);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////

BENCHMARK(test_first);
BENCHMARK(test_best);

///////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char ** argv)
{
    // first_fit picks the first block that fits (even if larger than needed)
    {
        Allocator allocator(1'024);

        auto p1 = allocator.allocate(16);  //  header at offset   0, data at  16
        auto p2 = allocator.allocate(16);  //  header at offset  32, data at  48
                   allocator.allocate(16); //  barrier
        auto p4 = allocator.allocate(16);  //  header at offset  96, data at 112
                   allocator.allocate(16); //  barrier

        allocator.deallocate(p1);          //  free: [0:16]
        allocator.deallocate(p2);          //  merge: [0:48]
        allocator.deallocate(p4);          //  free: [0:48] -> [96:16]

        auto z = allocator.allocate(16);   //  first_fit: block [0:48] wins
        assert(z == p1);
    }

    //  -----------------------------------------------------------------------

    // best_fit picks the smallest block that fits
    {
        Allocator allocator(1'024, Allocator::best_fit);

        auto p1 = allocator.allocate(16);  //  header at offset   0, data at  16
        auto p2 = allocator.allocate(16);  //  header at offset  32, data at  48
                   allocator.allocate(16); //  barrier
        auto p4 = allocator.allocate(16);  //  header at offset  96, data at 112
                   allocator.allocate(16); //  barrier

        allocator.deallocate(p1);          //  free: [0:16]
        allocator.deallocate(p2);          //  merge: [0:48]
        allocator.deallocate(p4);          //  free: [0:48] -> [96:16]

        auto z = allocator.allocate(16);   //  best_fit: block [96:16] wins (exact fit)
        assert(z == p4);
    }

    //  -----------------------------------------------------------------------

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
}

///////////////////////////////////////////////////////////////////////////////////////////////
/*
#1
-----------------------------------------------------
Benchmark           Time             CPU   Iterations
-----------------------------------------------------
test_first    2993310 ns      2993292 ns          227
test_best     2909229 ns      2909180 ns          244


#2
-----------------------------------------------------
Benchmark           Time             CPU   Iterations
-----------------------------------------------------
test_first    2963953 ns      2963526 ns          227
test_best     2937970 ns      2935288 ns          240


#3
-----------------------------------------------------
Benchmark           Time             CPU   Iterations
-----------------------------------------------------
test_first    2933666 ns      2933673 ns          228
test_best     2899714 ns      2899702 ns          243
*/