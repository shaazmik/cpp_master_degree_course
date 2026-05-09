//@compile g++ part6/12_01.cpp -std=c++20 -Wall -Wextra -Wpedantic
//@setup sudo locale-gen ru_RU.UTF-8 en_US.UTF-8
#include <cassert>
#include <iomanip>
#include <iostream>
#include <locale>
#include <optional>
#include <sstream>
#include <string>

///////////////////////////////////////////////////////////////////////////////////////////////

std::string format_rub(long double kopecks)
{
    std::stringstream ss;
    ss.imbue(std::locale("ru_RU.utf8"));
    ss << std::put_money(kopecks, true);
    return ss.str();
}

std::optional < long double > parse_rub(std::string const & s)
{
    std::stringstream ss(s);
    ss.imbue(std::locale("ru_RU.utf8"));
    long double kopecks = 0.0L;
    ss >> std::get_money(kopecks, true);
    return ss.fail() ? std::nullopt : std::optional < long double >{kopecks};
}

std::string format_usd(long double cents)
{
    std::stringstream ss;
    ss.imbue(std::locale("en_US.utf8"));
    ss << std::put_money(cents, true);
    return ss.str();
}

///////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    constexpr long double rub_per_usd = 74.24L;

    const long double cases[] =
    {
        900000.0L,   //  9000.00 RUB -> ~121.23 USD
        4500000.0L,  // 45000.00 RUB -> ~606.14 USD
        100.0L,      //     1.00 RUB ->    0.01 USD
    };

    for (long double kopecks : cases)
    {
        std::string rub_str = format_rub(kopecks);
        auto        parsed  = parse_rub(rub_str);

        assert(parsed.has_value());
        assert(*parsed == kopecks);

        std::cout << rub_str << " -> " << format_usd(*parsed / rub_per_usd) << '\n';
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////
