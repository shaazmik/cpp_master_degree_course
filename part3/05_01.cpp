//@compile clang++ part3/05_01.cpp -std=c++2b -Wall -Wextra -Wpedantic

#include <iostream>
#include <string>

////////////////////////////////////////////////////////////////////////////////////////////////

class Person
{
public:

    std::string m_name;
    int m_age = 0;
    int m_grade = 0;

    void print() const
    {
        std::cout << "Name: " << m_name << ", Age: " << m_age << ", Grade: " << m_grade << std::endl;
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////

class Builder
{
public:

    Builder() = default;

//  ------------------------------------------------------------------------------------------

    Builder& name(const std::string& name)
    {
        m_person.m_name = name;
        return *this;
    }

    Builder& age(int age)
    {
        m_person.m_age = age;
        return *this;
    }

    Builder& grade(int grade)
    {
        m_person.m_grade = grade;
        return *this;
    }

//  ------------------------------------------------------------------------------------------

    Person get() const
    {
        return m_person;
    }

private:

    Person m_person;
};

////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    Builder builder;

    auto person = builder.name("Daniil").age(22).grade(1337).get();

//  ------------------------------------------------------------------------------------------

    person.print();

    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////