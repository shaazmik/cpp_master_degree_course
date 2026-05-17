//@compile g++ part6/13_04.cpp -std=c++23 -Wall -Wextra -Wpedantic

#include <algorithm>
#include <array>
#include <cassert>
#include <chrono>
#include <filesystem>
#include <format>
#include <fstream>
#include <print>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

///////////////////////////////////////////////////////////////////////////////////////////////

static auto make_type(std::filesystem::file_status const & status) -> char
{
    if (std::filesystem::is_directory   (status)) return 'd';
    if (std::filesystem::is_regular_file(status)) return 'f';
    if (std::filesystem::is_symlink     (status)) return 'l';
    return '?';
}

///////////////////////////////////////////////////////////////////////////////////////////////

static auto make_permissions(std::filesystem::perms permissions) -> std::string
{
    auto lambda = [permissions](auto x, auto y)
    {
        return (permissions & x) == std::filesystem::perms::none ? '-' : y;
    };

    return
    {
        lambda(std::filesystem::perms::owner_read,  'r'),
        lambda(std::filesystem::perms::owner_write, 'w'),
        lambda(std::filesystem::perms::owner_exec,  'x')
    };
}

///////////////////////////////////////////////////////////////////////////////////////////////

static auto dir_bytes(std::filesystem::path const & path) -> std::size_t
{
    std::size_t total = 0;

    if (std::filesystem::exists(path) && std::filesystem::is_directory(path))
    {
        for (auto const & entry : std::filesystem::recursive_directory_iterator(path))
        {
            if (!std::filesystem::is_directory(entry.status()))
                total += std::filesystem::file_size(entry);
        }
    }

    return total;
}

///////////////////////////////////////////////////////////////////////////////////////////////

static auto format_size(std::filesystem::directory_entry const & entry) -> std::string
{
    static constexpr std::size_t KILO = 1024;
    static constexpr std::array < char, 4 > UNITS = { 'B', 'K', 'M', 'G' };

    auto n = std::filesystem::is_regular_file(entry.status())
           ? std::filesystem::file_size(entry)
           : dir_bytes(entry.path());

    auto i = 0uz;

    while (i + 1 < std::size(UNITS) && n >= KILO)
    {
        n /= KILO;
        ++i;
    }

    return (std::stringstream() << std::format("{: >4} ({})", n, UNITS[i])).str();
}

///////////////////////////////////////////////////////////////////////////////////////////////

// Returns directory entries whose filenames match filter sorted by path
// Analogous to: ls <path> | grep -E <pattern>
static auto filter_entries(std::filesystem::path const & path, std::regex const & filter)
    -> std::vector < std::filesystem::directory_entry >
{
    std::vector < std::filesystem::directory_entry > result;

    if (!std::filesystem::exists(path) || !std::filesystem::is_directory(path))
        return result;

    for (auto const & entry : std::filesystem::directory_iterator(path))
    {
        if (std::regex_search(entry.path().filename().string(), filter))
            result.push_back(entry);
    }

    std::sort(result.begin(), result.end(),
        [](std::filesystem::directory_entry const & a,
           std::filesystem::directory_entry const & b)
        { return a.path() < b.path(); });

    return result;
}

///////////////////////////////////////////////////////////////////////////////////////////////

static void show(std::filesystem::path const & path, std::regex const & filter)
{
    for (auto const & entry : filter_entries(path, filter))
    {
        std::print
        (
            "show : entry : {} | {} | {} | {} | {}\n",
            make_type(entry.status()),
            make_permissions(entry.status().permissions()),
            format_size(entry),
            std::chrono::floor < std::chrono::seconds >
            (
                std::chrono::file_clock::to_sys(entry.last_write_time())
            ),
            entry.path().filename().string()
        );
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////

static void test_filter_entries()
{
    auto const dir = std::filesystem::temp_directory_path() / "13_04_test";
    std::filesystem::create_directories(dir);

    for (auto const * name : { "alpha.cpp", "beta.hpp", "gamma.txt", "delta.cpp" })
        std::fstream(dir / name, std::ios::out);

    auto const cpp  = filter_entries(dir, std::regex(".+\\.cpp"));
    auto const hpp  = filter_entries(dir, std::regex(".+\\.hpp"));
    auto const all  = filter_entries(dir, std::regex(".*"));
    auto const none = filter_entries(dir, std::regex("xyz_no_match"));
    auto const pfx  = filter_entries(dir, std::regex("^beta"));

    assert(cpp.size()  == 2uz);
    assert(hpp.size()  == 1uz);
    assert(all.size()  == 4uz);
    assert(none.empty());
    assert(pfx.size()  == 1uz);
    assert(pfx[0].path().filename().string() == "beta.hpp");

    assert(cpp[0].path().filename().string() == "alpha.cpp");
    assert(cpp[1].path().filename().string() == "delta.cpp");

    std::filesystem::remove_all(dir);
    std::print("test_filter_entries: passed\n");
}

///////////////////////////////////////////////////////////////////////////////////////////////

static void demo()
{
    auto const path   = std::filesystem::current_path() / "part6";
    auto const filter = std::regex(".+\\.cpp");

    std::print("demo : path   : {}\n",   path.string());
    std::print("demo : filter : .+\\.cpp\n");
    std::print("demo : grep   : ls {} | grep -E '.+\\.cpp'\n\n", path.string());

    show(path, filter);
}

///////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    test_filter_entries();
    demo();
}

///////////////////////////////////////////////////////////////////////////////////////////////
