//@compile g++ part6/12_04.cpp -std=c++23 -Wall -Wextra -Wpedantic
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iterator>
#include <print>
#include <regex>
#include <string>
#include <utility>
#include <vector>

///////////////////////////////////////////////////////////////////////////////////////////////

using namespace std::literals;

using Email = std::pair < std::string, std::string >;  // { address, domain }

///////////////////////////////////////////////////////////////////////////////////////////////

// submatch 0 = full address, submatch 1 = domain (capture group after @)
std::vector < Email > extract_emails(std::string const & text)
{
    const std::regex pattern(R"([a-zA-Z0-9._%+-]+@([a-zA-Z0-9.-]+\.[a-zA-Z]{2,}))");

    std::vector < std::string > tokens;

    std::ranges::for_each
    (
        std::sregex_token_iterator(std::cbegin(text), std::cend(text), pattern, {0, 1}),

        std::sregex_token_iterator(),

        [&tokens](auto const & match){ tokens.push_back(match); }
    );

    std::vector < Email > result;
    result.reserve(tokens.size() / 2);

    for (std::size_t i = 0; i + 1 < tokens.size(); i += 2)
        result.emplace_back(tokens[i], tokens[i + 1]);

    return result;
}

///////////////////////////////////////////////////////////////////////////////////////////////

static void test_empty()
{
    assert(extract_emails(""s).empty());
    assert(extract_emails(R"(no emails here! @nodomain, justname@)"s).empty());

    std::print("test_empty: passed\n");
}

static void test_basic()
{
    auto text = R"(Contact us at support@example.com or sales@company.org and my@hadal.ru)"s;

    assert(extract_emails(text) == std::vector < Email >
    ({
        { "support@example.com", "example.com" },
        { "sales@company.org",   "company.org" },
        { "my@hadal.ru",         "hadal.ru"    },
    }));

    std::print("test_basic: passed\n");
}

static void test_mixed_text()
{
    auto text = R"(
        From: alice@mail.ru
        To:   bob@gmail.com
        CC:   charlie@university.edu
        Not emails: @nodomain, justname@, plain-text
    )"s;

    assert(extract_emails(text) == std::vector < Email >
    ({
        { "alice@mail.ru",          "mail.ru"        },
        { "bob@gmail.com",          "gmail.com"      },
        { "charlie@university.edu", "university.edu" },
    }));

    std::print("test_mixed_text: passed\n");
}

static void test_subdomains()
{
    auto text = R"(admin@mail.server.example.co.org)"s;

    assert(extract_emails(text) == std::vector < Email >
    ({
        { "admin@mail.server.example.co.org", "mail.server.example.co.org" },
    }));

    std::print("test_subdomains: passed\n");
}

static void demo()
{
    auto text = R"(
        Hello! Please reach out:
          - dev@github.com
          - info@open-source.io
          - noreply@notifications.gitlab.com
        Invalid: @nodomain, missing@, plain-text
    )"s;

    std::print("input:{}\n", text);
    std::print("{:<42} {}\n", "address", "domain");
    std::print("{:-<42} {:-<30}\n", "", "");

    for (auto const & [address, domain] : extract_emails(text))
        std::print("{:<42} {}\n", address, domain);
}

///////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    test_empty();
    test_basic();
    test_mixed_text();
    test_subdomains();
    demo();
}

///////////////////////////////////////////////////////////////////////////////////////////////
