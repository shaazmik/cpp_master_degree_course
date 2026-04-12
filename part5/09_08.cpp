//@compile g++ part5/09_08.cpp -std=c++23 -Wall -Wextra -Wpedantic
#include <cstddef>
#include <new>
#include <print>

///////////////////////////////////////////////////////////////////////////////////////////////

template < typename D > class Entity
{
public:

    static auto operator new(std::size_t size) -> void *
    {
        std::print("Entity::operator new\n");

        return ::operator new(size);
    }

    static void operator delete(void * x)
    {
        std::print("Entity::operator delete\n");

        ::operator delete(x);
    }

    //  -------------------------------------------------------------------

    static auto operator new[](std::size_t size) -> void *
    {
        std::print("Entity::operator new[]\n");

        return ::operator new[](size);
    }

    static void operator delete[](void * x)
    {
        std::print("Entity::operator delete[]\n");

        ::operator delete[](x);
    }

    //  -------------------------------------------------------------------

    static auto operator new(std::size_t size, std::nothrow_t const &) noexcept -> void *
    {
        std::print("Entity::operator new (nothrow)\n");

        return ::operator new(size, std::nothrow);
    }

    static void operator delete(void * x, std::nothrow_t const &) noexcept
    {
        std::print("Entity::operator delete (nothrow)\n");

        ::operator delete(x);
    }

    static auto operator new[](std::size_t size, std::nothrow_t const &) noexcept -> void *
    {
        std::print("Entity::operator new[] (nothrow)\n");

        return ::operator new[](size, std::nothrow);
    }

    static void operator delete[](void * x, std::nothrow_t const &) noexcept
    {
        std::print("Entity::operator delete[] (nothrow)\n");

        ::operator delete[](x);
    }

protected:

    Entity()  = default;
    ~Entity() = default;
};

///////////////////////////////////////////////////////////////////////////////////////////////

class Client : private Entity < Client >
{
private:

    using base_t = Entity < Client >;

public:

    Client()  { std::print("Client:: Client\n"); }
    ~Client() { std::print("Client::~Client\n"); }

    //  -------------------------------------------------------------------

    using base_t::operator new,    base_t::operator delete;
    using base_t::operator new[],  base_t::operator delete[];
};

///////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    delete new Client;

    std::print("\n");

    delete[] new Client[2];

    std::print("\n");

    delete new (std::nothrow) Client;

    std::print("\n");

    delete[] new (std::nothrow) Client[2];
}

///////////////////////////////////////////////////////////////////////////////////////////////
