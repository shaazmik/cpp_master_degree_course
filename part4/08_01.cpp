//@compile clang++ part4/08_01.cpp -std=c++20 -Wall -Wextra -Wpedantic
#include <cstring>
#include <iostream>

///////////////////////////////////////////////////////////////////////////////////////////////

class Entity_v1
{
public:
    int value() const { return m_value; }
private:
    int m_value = 1337;
};

class Entity_v2
{
public:
    int m_value = 0;
};

///////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    Entity_v1 e;

    std::cout << "sizeof(Entity_v1) = " << sizeof(Entity_v1) << '\n';
    std::cout << "sizeof(Entity_v2) = " << sizeof(Entity_v2) << '\n';

    //  -------------------------------------------------------------------------------------------
    // Method 1: reinterpret_cast
    // Entity_v1 and Entity_v2 have the same memory layout (one int)
    // reinterpret_cast reads the bytes of e as Entity_v2, which has a public field

    auto & e2 = reinterpret_cast<Entity_v2 &>(e);
    e2.m_value = 42;
    std::cout << "method 1 (reinterpret_cast): " << e.value() << '\n';

    //  -------------------------------------------------------------------------------------------
    // Method 2: direct pointer cast
    // Cast Entity_v1* to int* and write directly
    // Works because the int field is at offset 0 (it is the only member)

    int * raw = reinterpret_cast<int *>(&e);
    *raw = 99;
    std::cout << "method 2 (raw pointer):      " << e.value() << '\n';

    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////
