//@compile g++-11 part5/10_05/10_05.cpp -std=c++23 -Wall -Wextra -Wpedantic -O3 -m32
#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <fstream>
#include <functional>
#include <iostream>
#include <random>
#include <set>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

///////////////////////////////////////////////////////////////////////////////////////////////

using uint = unsigned int;

///////////////////////////////////////////////////////////////////////////////////////////////

static auto rs_hash(std::string const & s) -> uint
{
    uint b = 378551u, a = 63689u, hash = 0u;

    for (auto c : s)
    {
        hash = hash * a + static_cast < uint > (static_cast < unsigned char > (c));
        a    = a * b;
    }

    return hash;
}

static auto js_hash(std::string const & s) -> uint
{
    uint hash = 1315423911u;

    for (auto c : s)
        hash ^= (hash << 5u) + static_cast < uint > (static_cast < unsigned char > (c)) + (hash >> 2u);

    return hash;
}

static auto pjw_hash(std::string const & s) -> uint
{
    const uint bits           = sizeof(uint) * 8u;
    const uint three_quarters = bits * 3u / 4u;
    const uint one_eighth     = bits / 8u;
    const uint high_bits      = ~0u << (bits - one_eighth);

    uint hash = 0u, test = 0u;

    for (auto c : s)
    {
        hash = (hash << one_eighth) + static_cast < uint > (static_cast < unsigned char > (c));

        if ((test = hash & high_bits) != 0u)
            hash = (hash ^ (test >> three_quarters)) & ~high_bits;
    }

    return hash;
}

static auto elf_hash(std::string const & s) -> uint
{
    uint hash = 0u, x = 0u;

    for (auto c : s)
    {
        hash = (hash << 4u) + static_cast < uint > (static_cast < unsigned char > (c));

        if ((x = hash & 0xF0000000u) != 0u)
            hash ^= x >> 24u;

        hash &= ~x;
    }

    return hash;
}

static auto bkdr_hash(std::string const & s) -> uint
{
    const uint seed = 131u;
    uint       hash = 0u;

    for (auto c : s)
        hash = hash * seed + static_cast < uint > (static_cast < unsigned char > (c));

    return hash;
}

static auto sdbm_hash(std::string const & s) -> uint
{
    uint hash = 0u;

    for (auto c : s)
        hash = static_cast < uint > (static_cast < unsigned char > (c)) + (hash << 6u) + (hash << 16u) - hash;

    return hash;
}

static auto djb_hash(std::string const & s) -> uint
{
    uint hash = 5381u;

    for (auto c : s)
        hash = (hash << 5u) + hash + static_cast < uint > (static_cast < unsigned char > (c));

    return hash;
}

static auto dek_hash(std::string const & s) -> uint
{
    uint hash = static_cast < uint > (s.size());

    for (auto c : s)
        hash = ((hash << 5u) ^ (hash >> 27u)) ^ static_cast < uint > (static_cast < unsigned char > (c));

    return hash;
}

static auto ap_hash(std::string const & s) -> uint
{
    uint hash = 0xAAAAAAAAu;
    uint i    = 0u;

    for (auto c : s)
    {
        auto ch = static_cast < uint > (static_cast < unsigned char > (c));

        hash ^= (i & 1u) == 0u
                    ?    (hash <<  7u) ^ ch * (hash >> 3u)
                    : ~((hash << 11u) + (ch ^ (hash >> 5u)));
        ++i;
    }

    return hash;
}

///////////////////////////////////////////////////////////////////////////////////////////////

static auto make_strings(std::size_t count, std::size_t length) -> std::vector < std::string >
{
    std::set < std::string >           unique;
    std::string                        s(length, ' ');
    std::uniform_int_distribution<int> dist('a', 'z');
    std::default_random_engine         engine;

    while (unique.size() < count)
    {
        for (auto & ch : s)
            ch = static_cast < char > (dist(engine));

        unique.insert(s);
    }

    return { unique.begin(), unique.end() };
}

///////////////////////////////////////////////////////////////////////////////////////////////

using hash_fn = std::function < uint(std::string const &) >;

struct entry_t { const char * name; hash_fn fn; };

///////////////////////////////////////////////////////////////////////////////////////////////

static auto measure(
    std::vector < std::string > const & strings,
    hash_fn                     const & fn,
    std::size_t                         step
) -> std::vector < std::pair < std::size_t, std::size_t > >
{
    std::unordered_set < uint > seen;
    seen.reserve(strings.size());

    std::vector < std::pair < std::size_t, std::size_t > > points;

    for (auto i = 0uz; i < strings.size(); ++i)
    {
        seen.insert(fn(strings[i]));

        if ((i + 1) % step == 0)
            points.emplace_back(i + 1, (i + 1) - seen.size());
    }

    return points;
}

///////////////////////////////////////////////////////////////////////////////////////////////

static void test_hashes()
{
    static const std::string cases[] = { "hello", "world", "test", "a", "" };

    for (auto const & s : cases)
    {
        assert(rs_hash(s)   == rs_hash(s)  );
        assert(js_hash(s)   == js_hash(s)  );
        assert(pjw_hash(s)  == pjw_hash(s) );
        assert(elf_hash(s)  == elf_hash(s) );
        assert(bkdr_hash(s) == bkdr_hash(s));
        assert(sdbm_hash(s) == sdbm_hash(s));
        assert(djb_hash(s)  == djb_hash(s) );
        assert(dek_hash(s)  == dek_hash(s) );
        assert(ap_hash(s)   == ap_hash(s)  );
    }

    assert(rs_hash("abc")   != rs_hash("xyz") );
    assert(djb_hash("abc")  != djb_hash("xyz"));
    assert(bkdr_hash("abc") != bkdr_hash("xyz"));
}

///////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    test_hashes();

    const auto total = 100'000uz;
    const auto len   = 10uz;
    const auto step  = 1'000uz;

    auto strings = make_strings(total, len);

    const std::array < entry_t, 9 > entries = {{
        { "RS",   rs_hash   },
        { "JS",   js_hash   },
        { "PJW",  pjw_hash  },
        { "ELF",  elf_hash  },
        { "BKDR", bkdr_hash },
        { "SDBM", sdbm_hash },
        { "DJB",  djb_hash  },
        { "DEK",  dek_hash  },
        { "AP",   ap_hash   },
    }};

    struct result_t { const char * name; std::size_t collisions; };

    std::vector < result_t > results;

    std::ofstream csv("10_05.csv");
    csv << "name,strings,collisions\n";

    for (auto const & entry : entries)
    {
        auto series = measure(strings, entry.fn, step);

        for (auto const & [x, y] : series)
            csv << entry.name << ',' << x << ',' << y << '\n';

        results.push_back({ entry.name, series.back().second });
    }

    std::sort(results.begin(), results.end(),
              [](result_t const & a, result_t const & b)
              { return a.collisions < b.collisions; });

    std::cout << "collisions on " << total << " strings (length " << len << "):\n\n";

    for (auto i = 0uz; i < results.size(); ++i)
        std::cout << "  " << (i + 1) << ". "
                  << results[i].name << " : " << results[i].collisions << '\n';

    std::cout << "\nbest  : " << results.front().name
              << " (" << results.front().collisions << ")\n";
    std::cout << "worst : " << results.back().name
              << " (" << results.back().collisions  << ")\n";

    std::cout << "\ndata -> 10_05.csv\n";

    std::ofstream py("10_05_plot.py");

    py << "import csv\n"
          "import matplotlib.pyplot as plt\n"
          "\n"
          "data = {}\n"
          "with open('10_05.csv') as f:\n"
          "    for row in csv.DictReader(f):\n"
          "        n = row['name']\n"
          "        if n not in data:\n"
          "            data[n] = ([], [])\n"
          "        data[n][0].append(int(row['strings']))\n"
          "        data[n][1].append(int(row['collisions']))\n"
          "\n"
          "for name, (x, y) in sorted(data.items()):\n"
          "    plt.plot(x, y, label=name)\n"
          "\n"
          "plt.xlabel('strings hashed')\n"
          "plt.ylabel('collisions')\n"
          "plt.title('hash function collision comparison')\n"
          "plt.grid(True)\n"
          "plt.legend()\n"
          "plt.tight_layout()\n"
          "plt.show()\n";

    std::cout << "plot  -> python3 10_05_plot.py\n";
}

///////////////////////////////////////////////////////////////////////////////////////////////
/*
Results (N = 100,000 strings, length = 10, M = 2^32):

  best  : JS = 0,  AP = 0
  middle: RS = 1,  BKDR = 1, SDBM = 1, DJB = 1, DEK = 3
  worst : PJW = 22, ELF = 22

Expected by birthday paradox: N^2 / (2*M) ~ 1.2  ->  good hashes match this.

Curve shape: step-like, sub-linear, flat until N ~ sqrt(M) ~ 65,000 for good hashes.
PJW/ELF rise from N ~ 20,000 because they use only the upper 4 bits of the 32-bit
output space for short strings, which reduces effective M from 2^32 down to 2^4 = 16.
*/
