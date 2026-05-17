//@compile g++ part6/13_03.cpp -std=c++23 -Wall -Wextra -Wpedantic

#include <algorithm>
#include <cassert>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <print>
#include <sstream>
#include <string>

///////////////////////////////////////////////////////////////////////////////////////////////

static std::string remove_comments(std::string s)
{
    for (auto it = s.begin(); it != s.end(); )
    {
        if (*it == '\'')
        {
            ++it;
            while (it != s.end() && !(*it == '\'' && *std::prev(it) != '\\'))
                ++it;
            if (it != s.end()) ++it;
        }
        else if (*it == 'R' && std::next(it) != s.end() && *std::next(it) == '"')
        {
            it += 2;

            std::string delim;
            while (it != s.end() && *it != '(')
                delim += *it++;

            if (it != s.end()) ++it;

            std::string const terminator = ")" + delim + "\"";
            auto const pos = s.find(terminator, static_cast < std::size_t > (it - s.begin()));

            it = (pos != std::string::npos)
                ? std::next(s.begin(), static_cast < std::ptrdiff_t > (pos + terminator.size()))
                : s.end();
        }
        else if (*it == '"')
        {
            ++it;
            while (it != s.end() && !(*it == '"' && *std::prev(it) != '\\'))
                ++it;
            if (it != s.end()) ++it;
        }
        else if (*it == '/' && std::next(it) != s.end() && *std::next(it) == '/')
        {
            it = s.erase(it, std::find(it, s.end(), '\n'));
        }
        else if (*it == '/' && std::next(it) != s.end() && *std::next(it) == '*')
        {
            auto end = it + 2;
            while (end != s.end() && !(*end == '/' && *std::prev(end) == '*'))
                ++end;
            if (end != s.end()) ++end;
            it = s.erase(it, end);
        }
        else
        {
            ++it;
        }
    }

    return s;
}

///////////////////////////////////////////////////////////////////////////////////////////////

static std::string remove_blank_lines(std::string const & s)
{
    std::istringstream iss(s);
    std::string        result;
    std::string        line;

    while (std::getline(iss, line))
    {
        auto const is_blank = std::all_of(line.cbegin(), line.cend(),
            [](unsigned char c) -> bool { return std::isspace(c) != 0; });

        if (!is_blank)
            result += line + '\n';
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////////////////////////

static void transform(std::string const & path_in, std::string const & path_out)
{
    auto s = (std::stringstream() << std::fstream(path_in, std::ios::in).rdbuf()).str();
    s = remove_comments(s);
    s = remove_blank_lines(s);
    std::fstream(path_out, std::ios::out) << s;
}

///////////////////////////////////////////////////////////////////////////////////////////////

static void test_line_comments()
{
    assert(remove_comments("int x = 1; // comment\n") == "int x = 1; \n");
    assert(remove_comments("// full line\nint y;\n")   == "\nint y;\n");
    assert(remove_comments("a // b\nc // d\n")         == "a \nc \n");
    std::print("test_line_comments: passed\n");
}

static void test_block_comments()
{
    assert(remove_comments("int x = /* hi */ 1;\n") == "int x =  1;\n");
    assert(remove_comments("/* a\nb */int z;\n")    == "int z;\n");
    std::print("test_block_comments: passed\n");
}

static void test_string_literals()
{
    assert(remove_comments("\"// not a comment\"\n") == "\"// not a comment\"\n");
    assert(remove_comments("\"/* not */\"\n")         == "\"/* not */\"\n");
    assert(remove_comments("'/'  // x\n")            == "'/'  \n");
    std::print("test_string_literals: passed\n");
}

static void test_raw_string_literals()
{
    assert(remove_comments("R\"(// safe)\"\n")         == "R\"(// safe)\"\n");
    assert(remove_comments("R\"(/* safe */)\"\n")      == "R\"(/* safe */)\"\n");
    assert(remove_comments("R\"foo(// safe)foo\"\n")   == "R\"foo(// safe)foo\"\n");
    assert(remove_comments("R\"(line1\nline2)\"\n")    == "R\"(line1\nline2)\"\n");
    std::print("test_raw_string_literals: passed\n");
}

static void test_blank_lines()
{
    assert(remove_blank_lines("a\n\nb\n")     == "a\nb\n");
    assert(remove_blank_lines("a\n   \nb\n")  == "a\nb\n");
    assert(remove_blank_lines("\n\na\n\n")    == "a\n");
    assert(remove_blank_lines("a\n")           == "a\n");
    std::print("test_blank_lines: passed\n");
}

static void test_combined()
{
    std::string const input =
        "int main() // entry\n"
        "{\n"
        "    /* setup */\n"
        "    auto s = R\"(// safe\n/* safe */\n)\";\n"
        "\n"
        "    return 0; // done\n"
        "}\n";

    std::string const expected =
        "int main() \n"
        "{\n"
        "    auto s = R\"(// safe\n"
        "/* safe */\n"
        ")\";\n"
        "    return 0; \n"
        "}\n";

    assert(remove_blank_lines(remove_comments(input)) == expected);
    std::print("test_combined: passed\n");
}

///////////////////////////////////////////////////////////////////////////////////////////////

static constexpr char const * DEMO_IN  = "/tmp/13_03_in.cpp";
static constexpr char const * DEMO_OUT = "/tmp/13_03_out.cpp";

static void demo()
{
    std::fstream(DEMO_IN, std::ios::out) <<
        "// Demonstration file\n"
        "\n"
        "#include <string> // standard\n"
        "\n"
        "/* Block\n"
        "   comment */\n"
        "int main()\n"
        "{\n"
        "    auto s1 = \"// not removed\";\n"
        "    auto s2 = R\"(// also safe\n"
        "/* also safe */\n"
        ")\";\n"
        "\n"
        "    return 0; // end\n"
        "}\n";

    transform(DEMO_IN, DEMO_OUT);

    std::print("--- Input ({}) ---\n", DEMO_IN);
    std::print("{}", (std::stringstream() << std::fstream(DEMO_IN,  std::ios::in).rdbuf()).str());
    std::print("--- Output ({}) ---\n", DEMO_OUT);
    std::print("{}", (std::stringstream() << std::fstream(DEMO_OUT, std::ios::in).rdbuf()).str());

    std::filesystem::remove(DEMO_IN);
    std::filesystem::remove(DEMO_OUT);
}

///////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    test_line_comments();
    test_block_comments();
    test_string_literals();
    test_raw_string_literals();
    test_blank_lines();
    test_combined();
    demo();
}

///////////////////////////////////////////////////////////////////////////////////////////////
