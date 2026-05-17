//@compile g++ part6/13_01.cpp -std=c++23 -Wall -Wextra -Wpedantic
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <print>
#include <sstream>
#include <string>
#include <vector>

///////////////////////////////////////////////////////////////////////////////////////////////

std::string bytes_to_hex(std::vector < std::uint8_t > const & bytes)
{
    std::stringstream ss;
    ss << std::hex << std::right << std::setfill('0');

    for (auto b : bytes)
        ss << std::setw(2) << static_cast < unsigned > (b);

    return ss.str();
}

///////////////////////////////////////////////////////////////////////////////////////////////

static std::uint8_t hex_digit(char c)
{
    if (c >= 'a')
        return static_cast < std::uint8_t > (c - 'a' + 10);

    return static_cast < std::uint8_t > (c - '0');
}

std::vector < std::uint8_t > hex_to_bytes(std::string const & hex)
{
    std::vector < std::uint8_t > result;
    result.reserve(hex.size() / 2);

    for (std::size_t i = 0; i < hex.size(); i += 2)
    {
        auto high = hex_digit(hex[i]);
        auto low  = hex_digit(hex[i + 1]);

        result.push_back(static_cast < std::uint8_t > ((high << 4) | low));
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////////////////////////

static void test_bytes_to_hex()
{
    assert(bytes_to_hex({})                         == "");
    assert(bytes_to_hex({ 0x00 })                   == "00");
    assert(bytes_to_hex({ 0xff })                   == "ff");
    assert(bytes_to_hex({ 0x0f })                   == "0f");
    assert(bytes_to_hex({ 0x0a, 0xbc, 0xde })       == "0abcde");
    assert(bytes_to_hex({ 0x00, 0x01, 0x0f, 0xff }) == "00010fff");

    std::print("test_bytes_to_hex: passed\n");
}

static void test_hex_to_bytes()
{
    using bytes = std::vector < std::uint8_t >;

    assert(hex_to_bytes("")         == bytes{});
    assert(hex_to_bytes("00")       == bytes{ 0x00 });
    assert(hex_to_bytes("ff")       == bytes{ 0xff });
    assert(hex_to_bytes("0f")       == bytes{ 0x0f });
    assert(hex_to_bytes("0abcde")   == (bytes{ 0x0a, 0xbc, 0xde }));
    assert(hex_to_bytes("00010fff") == (bytes{ 0x00, 0x01, 0x0f, 0xff }));

    std::print("test_hex_to_bytes: passed\n");
}

static void test_roundtrip()
{
    std::vector < std::uint8_t > const bytes = { 0x00, 0x01, 0x7f, 0x80, 0xfe, 0xff };

    assert(hex_to_bytes(bytes_to_hex(bytes))          == bytes);
    assert(bytes_to_hex(hex_to_bytes("deadbeefcafe")) == "deadbeefcafe");

    std::print("test_roundtrip: passed\n");
}

static void demo()
{
    std::vector < std::uint8_t > const data = { 0xde, 0xad, 0xbe, 0xef, 0xca, 0xfe };

    auto hex = bytes_to_hex(data);
    std::print("bytes -> hex : {}\n", hex);

    auto back = hex_to_bytes(hex);
    std::print("hex   -> bytes:");
    for (auto b : back)
        std::print(" {:02x}", b);
    std::print("\n");
}

///////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    test_bytes_to_hex();
    test_hex_to_bytes();
    test_roundtrip();
    demo();
}

///////////////////////////////////////////////////////////////////////////////////////////////
