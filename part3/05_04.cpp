//@compile clang++ part3/05_04.cpp -std=c++2b -Wall -Wextra -Wpedantic

#include <iostream>

////////////////////////////////////////////////////////////////////////////////////////////////

// Concrete Strategy 1
class MeleeStrategy
{
public:
    void execute() const
    {
        std::cout << "Attacking with a sword!" << std::endl;
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////

// Concrete Strategy 2
class TrollStrategy
{
public:
    void execute() const
    {
        std::cout << "Attacking by axe!" << std::endl;
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////

// T represents the Strategy (Policy)
template < typename T >
class Entity : public T
{
public:
    void performAction() const
    {
        std::cout << "[Entity] Preparing to act..." << std::endl;
        
        T::execute();
        
        std::cout << "[Entity] Action complete." << std::endl;
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    Entity < MeleeStrategy > warrior;
    Entity < TrollStrategy > troll;

//  ------------------------------------------------------------------------------------------

    std::cout << "--- Warrior Turn ---" << std::endl;
    warrior.performAction();

    std::cout << "\n--- Troll Turn ---" << std::endl;
    troll.performAction();

    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////