/* 
The idea:
Instead of making the entire Tester_v1 a friend of the Entity class, 
we create an intermediate class (usually internal) that has access to private members of the Entity and 
provides them selectively to the outside — only to the necessary classes. 
*/

//@compile clang++ part2/03_04.cpp -std=c++2b -Wall -Wextra -Wpedantic

#include <iostream>
#include <cassert>

//////////////////////////////////////////////////////////////

class Entity
{
private:

    friend class Attorney;

    static void test_v1()
    {
        std::cout << "Entity::test_v1 invoked\n";
    }

    static void test_v2()
    {
        std::cout << "Entity::test_v2 invoked\n";
    }
};

//////////////////////////////////////////////////////////////

class Attorney
{
private:

    friend class Tester_v1;
    friend class Tester_v2;

    static void access_test_v1()
    {
        std::cout << "Attorney grants access to test_v1\n";
        Entity::test_v1();
    }

    static void access_test_v2()
    {
        std::cout << "Attorney grants access to test_v2\n";
        Entity::test_v2();
    }
};

//////////////////////////////////////////////////////////////

class Tester_v1
{
public:
    static void run()
    {
        std::cout << "[Tester_v1] Running private test_v1...\n";
        Attorney::access_test_v1();
        std::cout << "[Tester_v1] test_v1 completed successfully\n\n";
    }
};

//////////////////////////////////////////////////////////////

class Tester_v2
{
public:
    static void run()
    {
        std::cout << "[Tester_v2] Running private test_v2...\n";
        Attorney::access_test_v2();
        std::cout << "[Tester_v2] test_v2 completed successfully\n\n";
    }
};

//////////////////////////////////////////////////////////////

int main()
{
    std::cout << "=== External testing of Entity private functions ===\n\n";

    Tester_v1::run();
    Tester_v2::run();

    std::cout << "All tests finished successfully.\n";
}

//////////////////////////////////////////////////////////////
