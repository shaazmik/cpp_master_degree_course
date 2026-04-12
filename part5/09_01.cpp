//@compile g++ part5/09_01.cpp -std=c++20 -Wall -Wextra -Wpedantic
#include <iostream>
#include <source_location>

///////////////////////////////////////////////////////////////////////////////////////////////

class Tracer
{
public:

    explicit Tracer(std::source_location location = std::source_location::current())
        : m_location(location)
    {
        std::cout << "[TRACE] enter: " << m_location.function_name()
                  << " (" << m_location.file_name() << ":" << m_location.line() << ")\n";
    }

    ~Tracer()
    {
        std::cout << "[TRACE] exit:  " << m_location.function_name() << '\n';
    }

    Tracer(const Tracer &)             = delete;
    Tracer & operator=(const Tracer &) = delete;

private:

    std::source_location m_location;
};

///////////////////////////////////////////////////////////////////////////////////////////////

#ifndef NDEBUG
#   define trace() Tracer tracer_
#else
#   define trace() ((void)0)
#endif

///////////////////////////////////////////////////////////////////////////////////////////////

void leaf()
{
    trace();
    std::cout << "    leaf body\n";
}

void middle()
{
    trace();
    std::cout << "    middle body\n";
    leaf();
}

int compute(int x, int y)
{
    trace();
    std::cout << "    compute body: " << x << " + " << y << '\n';
    return x + y;
}

///////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    trace();

    std::cout << "--- calling middle ---\n";
    middle();

    std::cout << "--- calling compute ---\n";
    auto result = compute(3, 4);
    std::cout << "    result: " << result << '\n';

    std::cout << "--- end of main ---\n";
}

///////////////////////////////////////////////////////////////////////////////////////////////
