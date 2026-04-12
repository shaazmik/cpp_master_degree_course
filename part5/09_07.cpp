//@compile g++ part5/09_07.cpp -std=c++23 -Wall -Wextra -Wpedantic
#include <array>
#include <bit>
#include <cassert>
#include <cstddef>
#include <memory>
#include <new>
#include <print>

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

    ~Entity() { std::destroy_at(get()); }

    Entity(Entity const &)             = delete;
    Entity & operator=(Entity const &) = delete;
    Entity(Entity &&)                  = delete;
    Entity & operator=(Entity &&)      = delete;

    //  -------------------------------------------------------------------

    auto get() -> Implementation *
    {
        return std::launder(std::bit_cast < Implementation * > (m_storage.data()));
    }

    auto get() const -> Implementation const *
    {
        return std::launder(std::bit_cast < Implementation const * > (m_storage.data()));
    }

    //  -------------------------------------------------------------------

    void test() const { get()->test(); }

private:

    alignas(std::max_align_t) std::array < std::byte, 16 > m_storage;
};

///////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    Entity entity;

    entity.test();

    assert(entity.get() != nullptr);
}

///////////////////////////////////////////////////////////////////////////////////////////////
