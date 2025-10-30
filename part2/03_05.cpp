//@compile clang++ part2/03_05.cpp -std=c++2b -Wall -Wextra -Wpedantic
#include <iostream>

/////////////////////////////////////////////////////////////
// Base interface #1
/////////////////////////////////////////////////////////////

class Entity_v1
{
public:
    virtual ~Entity_v1() = default;

    virtual void test() const
    {
        std::cout << "Entity_v1::test\n";
    }
};

/////////////////////////////////////////////////////////////
// Base interface #2
/////////////////////////////////////////////////////////////

class Entity_v2
{
public:
    virtual ~Entity_v2() = default;

    virtual void test() const
    {
        std::cout << "Entity_v2::test\n";
    }
};

/////////////////////////////////////////////////////////////
// Adapter between Entity_v1 and Client
/////////////////////////////////////////////////////////////

class Adapter_v1 : public virtual Entity_v1
{
public:
    virtual void test_v1() const
    {
        std::cout << "Adapter_v1::test_v1 -> ";
        Entity_v1::test();
    }

    // override test() but redirect to test_v1()
    void test() const override
    {
        test_v1();
    }
};

/////////////////////////////////////////////////////////////
// Adapter between Entity_v2 and Client
/////////////////////////////////////////////////////////////

class Adapter_v2 : public virtual Entity_v2
{
public:
    virtual void test_v2() const
    {
        std::cout << "Adapter_v2::test_v2 -> ";
        Entity_v2::test();
    }

    // override test() but redirect to test_v2()
    void test() const override
    {
        test_v2();
    }
};

/////////////////////////////////////////////////////////////
// Client derived from both adapters
/////////////////////////////////////////////////////////////

class Client final : public Adapter_v1, public Adapter_v2
{
public:
    ~Client() override
    {
        std::cout << "Client::~Client\n";
    }

    // Independent behavior for v1 side
    void test_v1() const override
    {
        std::cout << "Client::test_v1 (handling Entity_v1 logic)\n";
    }

    // Independent behavior for v2 side
    void test_v2() const override
    {
        std::cout << "Client::test_v2 (handling Entity_v2 logic)\n";
    }
};

/////////////////////////////////////////////////////////////
// Demonstration
/////////////////////////////////////////////////////////////

int main()
{
    Client client;

    // Access through v1 interface
    Entity_v1* entity1 = &client;
    // Access through v2 interface
    Entity_v2* entity2 = &client;

    std::cout << "Calling via Entity_v1 pointer:\n";
    entity1->test(); // calls Client::test_v1

    std::cout << "Calling via Entity_v2 pointer:\n";
    entity2->test(); // calls Client::test_v2

    return 0;
}
