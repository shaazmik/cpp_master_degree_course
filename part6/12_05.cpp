//@compile g++ part6/12_05.cpp -std=c++23 -Wall -Wextra -Wpedantic
//
// Grammar (EBNF):
//   statement  = 'set' name expression | expression
//   expression = term (('+' | '-') term)*
//   term       = power (('*' | '/' | '%') power)*
//   power      = factorial | factorial '^' power     <- right-associative
//   factorial  = primary '!'*
//   primary    = '(' expr ')' | '[' expr ']' | '{' expr '}' |
//                '+' primary | '-' primary | number | name
#include <cassert>
#include <cctype>
#include <cmath>
#include <print>
#include <sstream>
#include <string>
#include <unordered_map>
#include <variant>

///////////////////////////////////////////////////////////////////////////////////////////////

class Stream
{
public:

    using token_t = std::variant < char, double, std::string >;

    explicit Stream(std::string const & s) : m_stream(s + ';') {}

    bool empty()
    {
        m_stream >> std::ws;
        return m_stream.peek() == ';';
    }

    token_t get()
    {
        if (m_has_token)
        {
            m_has_token = false;
            return m_token;
        }

        char x = '\0';
        m_stream >> x;

        switch (x)
        {
            case '+': case '-': case '*': case '/': case '%':
            case '^': case '!':
            case '(': case ')': case '[': case ']': case '{': case '}':
            case ';':
            {
                return token_t(x);
            }

            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
            case '.':
            {
                m_stream.unget();
                double y = 0.0;
                m_stream >> y;
                return token_t(y);
            }

            default:
            {
                std::string name(1, x);
                while (m_stream.get(x) && (std::isalpha(x) || std::isdigit(x)))
                    name += x;
                if (!std::isspace(x))
                    m_stream.unget();
                return token_t(name);
            }
        }
    }

    void put(token_t const & token)
    {
        m_token    = token;
        m_has_token = true;
    }

private:

    std::stringstream m_stream;
    token_t           m_token;
    bool              m_has_token = false;
};

///////////////////////////////////////////////////////////////////////////////////////////////

class Calculator
{
public:

    double evaluate(std::string const & s)
    {
        Stream stream(s);
        if (stream.empty())
            return 0.0;
        return statement(stream);
    }

private:

    auto statement(Stream & stream) -> double
    {
        auto token = stream.get();

        if (std::holds_alternative < std::string > (token) &&
            std::get < std::string > (token) == "set")
        {
            return declaration(stream);
        }

        stream.put(token);
        return expression(stream);
    }

//  ------------------------------------------------------------------------------------

    auto declaration(Stream & stream) -> double
    {
        auto name           = std::get < std::string > (stream.get());
        m_variables[name]   = expression(stream);
        return m_variables[name];
    }

//  ------------------------------------------------------------------------------------

    auto expression(Stream & stream) -> double
    {
        auto x     = term(stream);
        auto token = stream.get();

        while (true)
        {
            switch (std::get < char > (token))
            {
                case '+': x += term(stream); break;
                case '-': x -= term(stream); break;

                default:
                    stream.put(token);
                    return x;
            }

            token = stream.get();
        }
    }

//  ------------------------------------------------------------------------------------

    auto term(Stream & stream) -> double
    {
        auto x     = power(stream);
        auto token = stream.get();

        while (true)
        {
            switch (std::get < char > (token))
            {
                case '*': x *= power(stream);              break;
                case '/': x /= power(stream);              break;
                case '%': x = std::fmod(x, power(stream)); break;

                default:
                    stream.put(token);
                    return x;
            }

            token = stream.get();
        }
    }

//  ------------------------------------------------------------------------------------

    auto power(Stream & stream) -> double
    {
        auto x     = factorial(stream);
        auto token = stream.get();

        if (std::holds_alternative < char > (token) && std::get < char > (token) == '^')
            return std::pow(x, power(stream));

        stream.put(token);
        return x;
    }

//  ------------------------------------------------------------------------------------

    auto factorial(Stream & stream) -> double
    {
        auto x     = primary(stream);
        auto token = stream.get();

        while (std::holds_alternative < char > (token) && std::get < char > (token) == '!')
        {
            auto n = static_cast < int > (x);
            x = 1.0;
            for (int i = 2; i <= n; ++i)
                x *= i;
            token = stream.get();
        }

        stream.put(token);
        return x;
    }

//  ------------------------------------------------------------------------------------

    auto primary(Stream & stream) -> double
    {
        auto token = stream.get();

        if (std::holds_alternative < char > (token))
        {
            switch (std::get < char > (token))
            {
                case '(': { auto x = expression(stream); stream.get(); return x; }
                case '[': { auto x = expression(stream); stream.get(); return x; }
                case '{': { auto x = expression(stream); stream.get(); return x; }

                case '+': return          primary(stream);
                case '-': return -1.0 *  primary(stream);
            }
        }

        if (std::holds_alternative < double > (token))
            return std::get < double > (token);

        return m_variables.at(std::get < std::string > (token));
    }

//  ------------------------------------------------------------------------------------

    std::unordered_map < std::string, double > m_variables;
};

///////////////////////////////////////////////////////////////////////////////////////////////

static void test_modulo()
{
    Calculator calc;
    assert(calc.evaluate("10 % 3")  == 1.0);
    assert(calc.evaluate("10 % 5")  == 0.0);
    assert(calc.evaluate("7 % 4")   == 3.0);
    assert(calc.evaluate("15 % 4")  == 3.0);
    std::print("test_modulo: passed\n");
}

static void test_power()
{
    Calculator calc;
    assert(calc.evaluate("2 ^ 10")    == 1024.0);
    assert(calc.evaluate("3 ^ 3")     ==   27.0);
    assert(calc.evaluate("2 ^ 0")     ==    1.0);
    assert(calc.evaluate("2 ^ 3 ^ 2") ==  512.0);  // right-assoc: 2^(3^2) = 2^9
    std::print("test_power: passed\n");
}

static void test_factorial()
{
    Calculator calc;
    assert(calc.evaluate("0!")  ==   1.0);
    assert(calc.evaluate("1!")  ==   1.0);
    assert(calc.evaluate("5!")  == 120.0);
    assert(calc.evaluate("6!")  == 720.0);
    std::print("test_factorial: passed\n");
}

static void test_brackets()
{
    Calculator calc;
    assert(calc.evaluate("(2 + 3) * 4")      == 20.0);
    assert(calc.evaluate("[2 + 3] * 4")      == 20.0);
    assert(calc.evaluate("{2 + 3} * 4")      == 20.0);
    assert(calc.evaluate("{[2 + 3] * (4)}")  == 20.0);
    std::print("test_brackets: passed\n");
}

static void test_combined()
{
    Calculator calc;
    assert(calc.evaluate("(3 + 2)!")       == 120.0);  // 5! = 120
    assert(calc.evaluate("[2 * 3 - 1]!")   == 120.0);  // 5! = 120
    assert(calc.evaluate("2 ^ 3 % 3")      ==   2.0);  // (2^3) % 3 = 8 % 3 = 2
    assert(calc.evaluate("{2 + 1} ^ 2")    ==   9.0);  // 3^2 = 9
    std::print("test_combined: passed\n");
}

static void test_variables()
{
    Calculator calc;
    assert(calc.evaluate("set x 5") ==   5.0);
    assert(calc.evaluate("set y 3") ==   3.0);
    assert(calc.evaluate("x + y")   ==   8.0);
    assert(calc.evaluate("x ^ y")   == 125.0);  // 5^3
    assert(calc.evaluate("x % y")   ==   2.0);  // 5 % 3
    assert(calc.evaluate("x!")      == 120.0);
    std::print("test_variables: passed\n");
}

static void demo()
{
    const std::string cases[] =
    {
        R"(2 + 3 * 4)",
        R"(10 % 3)",
        R"(2 ^ 10)",
        R"(2 ^ 3 ^ 2)",
        R"(5!)",
        R"((3 + 2)!)",
        R"([4 * 2] ^ 2)",
        R"({1 + 2} * {3 + 4})",
        R"(2 ^ 3 % 3)",
    };

    Calculator calc;

    for (auto const & s : cases)
        std::print("{:<25} = {}\n", s, calc.evaluate(s));
}

///////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    test_modulo();
    test_power();
    test_factorial();
    test_brackets();
    test_combined();
    test_variables();
    demo();
}

///////////////////////////////////////////////////////////////////////////////////////////////
