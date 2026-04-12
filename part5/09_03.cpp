//@compile g++ part5/09_03.cpp -std=c++23 -Wall -Wextra -Wpedantic
#include <cassert>
#include <cstddef>
#include <iostream>
#include <memory>
#include <print>
#include <vector>

///////////////////////////////////////////////////////////////////////////////////////////////
// 05.01 : Pattern Builder
///////////////////////////////////////////////////////////////////////////////////////////////

namespace ex01
{
    struct Entity { int x = 0, y = 0; };

    class Builder
    {
    public:

        virtual ~Builder() = default;

        std::unique_ptr < Entity > make_entity()
        {
            m_entity = std::make_unique < Entity > ();
            set_x();
            set_y();
            return std::move(m_entity);
        }

        virtual void set_x() const = 0;
        virtual void set_y() const = 0;

    protected:

        std::unique_ptr < Entity > m_entity;
    };

    class Builder_Client : public Builder
    {
    public:
        void set_x() const override { m_entity->x = 1; }
        void set_y() const override { m_entity->y = 1; }
    };

    class Builder_Server : public Builder
    {
    public:
        void set_x() const override { m_entity->x = 2; }
        void set_y() const override { m_entity->y = 2; }
    };

    void run()
    {
        std::unique_ptr < Builder > builder = std::make_unique < Builder_Client > ();
        auto entity = builder->make_entity();
        assert(entity->x == 1 && entity->y == 1);
    }

} // namespace ex01

///////////////////////////////////////////////////////////////////////////////////////////////
// 05.03 : Pattern Abstract Factory
///////////////////////////////////////////////////////////////////////////////////////////////

namespace ex03
{
    class Entity { public: virtual ~Entity() = default; };

    class Client : public Entity {};
    class Server : public Entity {};

    class Factory
    {
    public:
        virtual ~Factory() = default;
        virtual std::unique_ptr < Entity > make_entity() const = 0;
    };

    class Factory_Client : public Factory
    {
    public:
        std::unique_ptr < Entity > make_entity() const override
        {
            return std::make_unique < Client > ();
        }
    };

    class Factory_Server : public Factory
    {
    public:
        std::unique_ptr < Entity > make_entity() const override
        {
            return std::make_unique < Server > ();
        }
    };

    void run()
    {
        std::unique_ptr < Factory > factory = std::make_unique < Factory_Client > ();
        auto entity = factory->make_entity();
        assert(entity != nullptr);
    }

} // namespace ex03

///////////////////////////////////////////////////////////////////////////////////////////////
// 05.04 : Pattern Prototype
///////////////////////////////////////////////////////////////////////////////////////////////

namespace ex04
{
    class Entity
    {
    public:
        virtual ~Entity() = default;
        virtual std::unique_ptr < Entity > copy() const = 0;
    };

    class Client : public Entity
    {
    public:
        std::unique_ptr < Entity > copy() const override
        {
            return std::make_unique < Client > (*this);
        }
    };

    class Server : public Entity
    {
    public:
        std::unique_ptr < Entity > copy() const override
        {
            return std::make_unique < Server > (*this);
        }
    };

    class Prototype
    {
    public:

        Prototype()
        {
            m_entities.push_back(std::make_unique < Client > ());
            m_entities.push_back(std::make_unique < Server > ());
        }

        auto make_client() { return m_entities.at(0)->copy(); }
        auto make_server() { return m_entities.at(1)->copy(); }

    private:

        std::vector < std::unique_ptr < Entity > > m_entities;
    };

    void run()
    {
        Prototype proto;
        auto client = proto.make_client();
        auto server = proto.make_server();
        assert(client != nullptr && server != nullptr);
    }

} // namespace ex04

///////////////////////////////////////////////////////////////////////////////////////////////
// 05.09 : Pattern Composite
///////////////////////////////////////////////////////////////////////////////////////////////

namespace ex09
{
    class Entity
    {
    public:
        virtual ~Entity() = default;
        virtual int test() const = 0;
    };

    class Client : public Entity { public: int test() const override { return 1; } };
    class Server : public Entity { public: int test() const override { return 2; } };

    class Composite : public Entity
    {
    public:

        void add(std::unique_ptr < Entity > entity)
        {
            m_entities.push_back(std::move(entity));
        }

        int test() const override
        {
            auto x = 0;
            for (auto const & entity : m_entities)
                if (entity) x += entity->test();
            return x;
        }

    private:

        std::vector < std::unique_ptr < Entity > > m_entities;
    };

    auto make_composite(std::size_t size_1, std::size_t size_2) -> std::unique_ptr < Entity >
    {
        auto composite = std::make_unique < Composite > ();
        for (auto i = 0uz; i < size_1; ++i) composite->add(std::make_unique < Client > ());
        for (auto i = 0uz; i < size_2; ++i) composite->add(std::make_unique < Server > ());
        return composite;
    }

    void run()
    {
        auto composite = std::make_unique < Composite > ();
        for (auto i = 0uz; i < 5; ++i)
            composite->add(make_composite(1, 1));
        std::unique_ptr < Entity > entity = std::move(composite);
        assert(entity->test() == 15);
    }

} // namespace ex09

///////////////////////////////////////////////////////////////////////////////////////////////
// 05.13 : Pattern Observer
///////////////////////////////////////////////////////////////////////////////////////////////

namespace ex13
{
    class Observer
    {
    public:
        virtual ~Observer() = default;
        virtual void test(int x) const = 0;
    };

    class Entity
    {
    public:

        void add(std::unique_ptr < Observer > observer)
        {
            m_observers.push_back(std::move(observer));
        }

        void set(int x) { m_x = x; notify_all(); }

        void notify_all() const
        {
            for (auto const & observer : m_observers)
                if (observer) observer->test(m_x);
        }

    private:

        int m_x = 0;
        std::vector < std::unique_ptr < Observer > > m_observers;
    };

    class Client : public Observer
    {
    public:
        void test(int x) const override { std::print("Client::test : x = {}\n", x); }
    };

    class Server : public Observer
    {
    public:
        void test(int x) const override { std::print("Server::test : x = {}\n", x); }
    };

    void run()
    {
        Entity entity;
        entity.add(std::make_unique < Client > ());
        entity.add(std::make_unique < Server > ());
        for (auto i = 0; i < 2; ++i)
            entity.set(i + 1);
    }

} // namespace ex13

///////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    ex01::run();
    ex03::run();
    ex04::run();
    ex09::run();
    ex13::run();
}

///////////////////////////////////////////////////////////////////////////////////////////////
