//@compile g++ part6/12_03.cpp -std=c++23 -Wall -Wextra -Wpedantic
#include <cassert>
#include <cstddef>
#include <print>
#include <string_view>
#include <vector>

///////////////////////////////////////////////////////////////////////////////////////////////

// dp[i*n + j] == true iff s[i..j] is a palindrome
// Fills the table bottom-up: length 1 -> 2 -> ... -> n
std::string_view longest_palindrome(std::string_view s)
{
    const std::size_t n = s.size();

    if (n == 0)
        return s;

    std::vector < bool > dp(n * n, false);

    std::size_t best_start  = 0;
    std::size_t best_length = 1;

    for (std::size_t i = 0; i < n; ++i)
        dp[i * n + i] = true;

    for (std::size_t i = 0; i + 1 < n; ++i)
    {
        if (s[i] == s[i + 1])
        {
            dp[i * n + (i + 1)] = true;
            best_start  = i;
            best_length = 2;
        }
    }

    for (std::size_t len = 3; len <= n; ++len)
    {
        for (std::size_t i = 0; i + len <= n; ++i)
        {
            const std::size_t j = i + len - 1;

            if (s[i] == s[j] && dp[(i + 1) * n + (j - 1)])
            {
                dp[i * n + j] = true;

                if (len > best_length)
                {
                    best_start  = i;
                    best_length = len;
                }
            }
        }
    }

    return s.substr(best_start, best_length);
}

///////////////////////////////////////////////////////////////////////////////////////////////

static void test_empty()
{
    assert(longest_palindrome("") == "");
    std::print("test_empty: passed\n");
}

static void test_single_char()
{
    assert(longest_palindrome("a") == "a");
    assert(longest_palindrome("z") == "z");
    std::print("test_single_char: passed\n");
}

static void test_two_chars()
{
    assert(longest_palindrome("aa") == "aa");
    assert(longest_palindrome("ab") == "a");
    std::print("test_two_chars: passed\n");
}

static void test_palindrome_full()
{
    assert(longest_palindrome("racecar")  == "racecar");
    assert(longest_palindrome("abcba")    == "abcba");
    assert(longest_palindrome("abccba")   == "abccba");
    std::print("test_palindrome_full: passed\n");
}

static void test_embedded()
{
    assert(longest_palindrome("babad")             == "bab");
    assert(longest_palindrome("cbbd")              == "bb");
    assert(longest_palindrome("xyzracecarabc")     == "racecar");
    assert(longest_palindrome("forgeeksskeegfor")  == "geeksskeeg");
    std::print("test_embedded: passed\n");
}

static void demo()
{
    const std::string_view cases[] =
    {
        "babad",
        "cbbd",
        "racecar",
        "forgeeksskeegfor",
        "abcba",
        "amanaplanacanalpanama",
    };

    for (auto sv : cases)
        std::print("longest_palindrome(\"{}\") = \"{}\"\n", sv, longest_palindrome(sv));
}

///////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    test_empty();
    test_single_char();
    test_two_chars();
    test_palindrome_full();
    test_embedded();
    demo();
}

///////////////////////////////////////////////////////////////////////////////////////////////
