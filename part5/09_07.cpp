//@compile g++ part5/09_07.cpp -std=c++23 -Wall -Wextra -Wpedantic
#include <array>
#include <bit>
#include <cassert>
#include <cstddef>
#include <memory>
#include <new>
#include <print>
#include <utility>

///////////////////////////////////////////////////////////////////////////////////////////////

class Entity
{
private:

    class Implementation
    {
    public:
        void test() const { std::print("Entity::Implementation::test\n"); }
    };

public:

    Entity()
    {
        static_assert(sizeof (Implementation) <= sizeof (m_storage));
        static_assert(alignof(Implementation) <= alignof(std::max_align_t));

        ::new (m_storage.data()) Implementation();
    }

    Entity(Entity && other)
    {
        ::new (m_storage.data()) Implementation(std::move(*other.get()));
        std::destroy_at(other.get());
        ::new (other.m_storage.data()) Implementation();
    }

    Entity & operator=(Entity && other)
    {
        if (this != &other)
        {
            std::destroy_at(get());
            ::new (m_storage.data()) Implementation(std::move(*other.get()));
            std::destroy_at(other.get());
            ::new (other.m_storage.data()) Implementation();
        }

        return *this;
    }

    ~Entity() { std::destroy_at(get()); }

    Entity(Entity const &)             = delete;
    Entity & operator=(Entity const &) = delete;

    //  -------------------------------------------------------------------

    auto get() -> Implementation *
    {
        return std::launder(std::bit_cast < Implementation * > (m_storage.data()));
    }

    auto get() const -> Implementation const *
    {
        return std::launder(std::bit_cast < Implementation const * > (m_storage.data()));
    }

private:

    alignas(std::max_align_t) std::array < std::byte, 16 > m_storage;
};

///////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    {
        Entity entity;
        entity.get()->test();
        assert(entity.get() != nullptr);
    }

    {
        Entity a;
        Entity b(std::move(a));

        b.get()->test();
        assert(b.get() != nullptr);
    }

    {
        Entity a;
        Entity b;
        b = std::move(a);

        b.get()->test();
        assert(b.get() != nullptr);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////

/*
    Original Pimpl overhead:
    - Every construction and destruction calls operator new / operator delete: OS-level
      heap calls, allocator lock contention, potential fragmentation and cache misses
    - Implementation lives at an arbitrary heap address, so each access through m_pimpl
      requires an extra pointer dereference and likely a separate cache-line load
    - Move is O(1) pointer swap with no allocation cost

    Fast Pimpl tradeoffs:
    - No heap allocation: m_storage is embedded in Entity, so construction and
      destruction are pure CPU work with zero allocator involvement
    - Better cache locality: Implementation bytes are co-located with Entity metadata
      in the same cache line
    - Move performs a move-construction of Implementation into destination storage
      instead of a pointer steal - same zero heap traffic, but one extra constructor call
    - Compile-time size and alignment constraints (static_assert) catch ABI breakage
      early if Implementation grows beyond the reserved 16-byte buffer
*/
