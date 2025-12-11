//@compile clang++ part3/05_03.cpp -std=c++2b -Wall -Wextra -Wpedantic
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

////////////////////////////////////////////////////////////////////////////////////////////////

// Composite
// also implements the template method pattern
class GameComponent
{
public:

    virtual ~GameComponent() = default;

//  ------------------------------------------------------------------------------------------

    // template method: defines the skeleton of an operation
    void performTurn() const
    {
        prepare();
        execute();
        finalize();
    }

//  ------------------------------------------------------------------------------------------
    virtual void add(GameComponent* component) {}
    
    virtual void remove(GameComponent* component) {}

protected:

    // for the template method
    virtual void prepare() const {}

    virtual void execute() const = 0;

    virtual void finalize() const {}
};

////////////////////////////////////////////////////////////////////////////////////////////////

// leaf class (Composite)
class Unit : public GameComponent
{
public:

    Unit(const std::string& name, int damage)
        : m_name(name), m_damage(damage)
    {}

protected:

    void prepare() const override
    {
        std::cout << "[Unit] " << m_name << " checks ammo..." << std::endl;
    }

    void execute() const override
    {
        std::cout << "[Unit] " << m_name << " attacks for " << m_damage << " damage." << std::endl;
    }

    void finalize() const override
    {
        std::cout << "[Unit] " << m_name << " holds position." << std::endl;
    }

private:

    std::string m_name;
    int m_damage;
};

////////////////////////////////////////////////////////////////////////////////////////////////

// composite class
class Squad : public GameComponent
{
public:

    void add(GameComponent* component) override
    {
        m_components.push_back(component);
    }

    void remove(GameComponent* component) override
    {
        auto it = std::remove(m_components.begin(), m_components.end(), component);
        m_components.erase(it, m_components.end());
    }

protected:

    void prepare() const override
    {
        std::cout << "[Squad] Forming up formation..." << std::endl;
    }

    void execute() const override
    {
        std::cout << "[Squad] Commanding units to engage:" << std::endl;
        for (const auto* component : m_components)
        {
            component->performTurn();
        }
    }

private:

    std::vector<GameComponent*> m_components;
};

////////////////////////////////////////////////////////////////////////////////////////////////

// builder pattern to create complex Units
class UnitBuilder
{
public:

    UnitBuilder() { reset(); }

    void reset()
    {
        m_name = "Unknown";
        m_health = 100;
        m_damage = 10;
    }

//  ------------------------------------------------------------------------------------------

    UnitBuilder& setName(const std::string& name)
    {
        m_name = name;
        return *this;
    }

    UnitBuilder& setHealth(int health)
    {
        m_health = health;
        return *this;
    }

    UnitBuilder& setDamage(int damage)
    {
        m_damage = damage;
        return *this;
    }

//  ------------------------------------------------------------------------------------------

    Unit* getResult()
    {
        return new Unit(m_name, m_damage);
    }

private:

    std::string m_name;
    int m_health;
    int m_damage;
};

////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    // 1. Create units using Builder
    UnitBuilder builder;

    Unit* sniper = builder.setName("Sniper").setHealth(50).setDamage(100).getResult();
    
    builder.reset();
    Unit* tank = builder.setName("Heavy Tank").setHealth(500).setDamage(50).getResult();

//  ------------------------------------------------------------------------------------------

    // 2. Organize units using Composite
    Squad* alphaSquad = new Squad();
    alphaSquad->add(sniper);
    alphaSquad->add(tank);

//  ------------------------------------------------------------------------------------------

    // 3. execute behavior using Template Method
    // Calls prepare() -> execute() -> finalize() for the squad,
    // which recursively calls it for units
    std::cout << "--- START GOMZA ---" << std::endl;
    alphaSquad->performTurn();
    std::cout << "--- GG WP ---" << std::endl;

//  ------------------------------------------------------------------------------------------
    delete sniper;
    delete tank;
    delete alphaSquad;

    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////