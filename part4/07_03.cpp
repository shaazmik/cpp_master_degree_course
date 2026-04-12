//@compile clang++ part4/07_03.cpp -std=c++20 -Wall -Wextra -Wpedantic
#include <iostream>
#include <stdexcept>
#include <string>

///////////////////////////////////////////////////////////////////////////////////////////////

enum class Status { success, failure };

class Person
{
public:

    Person(std::string name, int grade, double salary, int id)
        : m_name(std::move(name)), m_grade(grade), m_salary(salary), m_id(id) {}

    std::string const & name()   const { return m_name;   }
    int                 grade()  const { return m_grade;  }
    double              salary() const { return m_salary; }
    int                 id()     const { return m_id;     }

private:

    std::string m_name;
    int         m_grade  = 0;
    double      m_salary = 0.0;
    int         m_id     = 0;
};

void save(Status status, int id)
{
    std::cout << "save: " << (status == Status::success ? "success" : "failure")
              << " id=" << id << '\n';
}

///////////////////////////////////////////////////////////////////////////////////////////////

void test(Person const & person)
{
    std::cout << "test : " << person.name() << '\n'; // [E] name() — user method, may throw
                                                      // [E] operator<< — may throw std::ios_base::failure

    if (person.grade() == 10                   // [B] normal if/else branch
                                               // [E] grade() — may throw
        || person.salary() > 1'000'000)        // [B] short-circuit: salary() not called if grade() == 10
                                               // [E] salary() — may throw
    {
        save(Status::success, person.id());    // [E] id() — may throw
                                               // [E] save() — may throw
    }
    else
    {
        save(Status::failure, person.id());    // [E] id() — may throw
                                               // [E] save() — may throw
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    try
    {
        Person alice  ("Alice",    10, 500'000,   1);
        Person bob    ("Bob",      8,  2'000'000, 2);
        Person ivan   ("Ivan",     5,  100,       3);

        test(alice);
        test(bob);
        test(ivan);
    }
    catch (std::exception const & e)
    {
        std::cerr << e.what() << '\n';
    }
    catch (...)
    {
        std::cerr << "unknown exception\n";
    }

    return 0;
}

// [B] — normal branches:
//   1. if/else on grade==10 || salary>1M
//   2. short-circuit of ||: if grade()==10, salary() is never called
//
// [E] — possible exception sources:
//   name(), grade(), salary(), id() — user methods, implementation unknown
//   save()                          — user function, implementation unknown
//   operator<<                      — may throw std::ios_base::failure if stream exceptions are enabled

///////////////////////////////////////////////////////////////////////////////////////////////
