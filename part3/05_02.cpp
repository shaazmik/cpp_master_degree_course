//@compile clang++ part3/05_02.cpp -std=c++2b -Wall -Wextra -Wpedantic

#include <iostream>
#include <string>

////////////////////////////////////////////////////////////////////////////////////////////////

class Entity
{
public:
    virtual ~Entity() = default;
    virtual void process() const = 0;
};

////////////////////////////////////////////////////////////////////////////////////////////////

class CoreEntity : virtual public Entity
{
public:
    void process() const override
    {
        std::cout << "Core data";
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////

template < typename T >
class Decorator : virtual public Entity, public T
{
    // No explicit constructor or reference to Entity as per requirements
};

////////////////////////////////////////////////////////////////////////////////////////////////

template < typename T >
class BracketDecorator : public Decorator < T >
{
public:
    void process() const override
    {
        std::cout << "[";
        T::process();
        std::cout << "]";
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////

template < typename T >
class StarDecorator : public Decorator < T >
{
public:
    void process() const override
    {
        std::cout << "*";
        T::process();
        std::cout << "*";
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    // Layering decorators at compile time
    StarDecorator < BracketDecorator < CoreEntity > > decorated_entity;

//  ------------------------------------------------------------------------------------------

    decorated_entity.process(); // Output: *[Core data]*
    std::cout << std::endl;

//  ------------------------------------------------------------------------------------------

    Entity* entity_ptr = &decorated_entity;
    entity_ptr->process();

    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////