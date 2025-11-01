//@compile clang++ part2/03_09.cpp -std=c++2b -Wall -Wextra -Wpedantic
#include <iostream>
#include <cstdint>
#include <stdexcept>

class IPv4 {
private:
    std::uint8_t m_addr[4];

    static constexpr std::uint32_t to_uint32(const std::uint8_t addr[4]) noexcept {
        return (static_cast<std::uint32_t>(addr[0]) << 24) |
               (static_cast<std::uint32_t>(addr[1]) << 16) |
               (static_cast<std::uint32_t>(addr[2]) << 8)  |
                static_cast<std::uint32_t>(addr[3]);
    }

    static constexpr void from_uint32(std::uint32_t val, std::uint8_t out[4]) noexcept {
        out[0] = static_cast<std::uint8_t>((val >> 24) & 0xFF);
        out[1] = static_cast<std::uint8_t>((val >> 16) & 0xFF);
        out[2] = static_cast<std::uint8_t>((val >> 8)  & 0xFF);
        out[3] = static_cast<std::uint8_t>( val        & 0xFF);
    }

public:
    constexpr IPv4() noexcept 
        :m_addr{0} {}

    constexpr IPv4(std::uint8_t octet0, std::uint8_t octet1, std::uint8_t octet2, std::uint8_t octet3) noexcept
        :m_addr{octet0, octet1, octet2, octet3}{}
    

    friend bool operator==(const IPv4& lhs, const IPv4& rhs) noexcept {
        for (std::size_t i = 0; i < 4; ++i) {
            if (lhs.m_addr[i] != rhs.m_addr[i]) return false;
        }
        return true;
    }

    friend bool operator!=(const IPv4& lhs, const IPv4& rhs) noexcept {
        return !(lhs == rhs);
    }

    friend bool operator<(const IPv4& lhs, const IPv4& rhs) noexcept {
        return to_uint32(lhs.m_addr) < to_uint32(rhs.m_addr);
    }

    friend bool operator>(const IPv4& lhs, const IPv4& rhs) noexcept {
        return rhs < lhs;
    }

    friend bool operator<=(const IPv4& lhs, const IPv4& rhs) noexcept {
        return !(rhs < lhs);
    }

    friend bool operator>=(const IPv4& lhs, const IPv4& rhs) noexcept {
        return !(lhs < rhs);
    }

    IPv4& operator++() noexcept {
        std::uint32_t val = to_uint32(m_addr);
        ++val;
        from_uint32(val, m_addr);
        return *this;
    }

    IPv4& operator--() noexcept {
        std::uint32_t val = to_uint32(m_addr);
        --val;
        from_uint32(val, m_addr);
        return *this;
    }

    IPv4 operator++(int) noexcept {
        IPv4 tmp = *this;
        ++(*this);
        return tmp;
    }

    IPv4 operator--(int) noexcept {
        IPv4 tmp = *this;
        --(*this);
        return tmp;
    }

    friend std::ostream& operator<<(std::ostream& os, const IPv4& ip) {
        os << static_cast<unsigned>(ip.m_addr[0]) << '.'
           << static_cast<unsigned>(ip.m_addr[1]) << '.'
           << static_cast<unsigned>(ip.m_addr[2]) << '.'
           << static_cast<unsigned>(ip.m_addr[3]);
        return os;
    }

    friend std::istream& operator>>(std::istream& is, IPv4& ip) {
        unsigned octet0, octet1, octet2, octet3 = 0;
        char dot1, dot2, dot3 = 0;
        if (is >> octet0 >> dot1 >> octet1 >> dot2 >> octet2 >> dot3 >> octet3) {
            if (dot1 == '.' && dot2 == '.' && dot3 == '.' &&
                octet0 <= 255 && octet1 <= 255 && octet2 <= 255 && octet3 <= 255) {
                ip.m_addr[0] = static_cast<std::uint8_t>(octet0);
                ip.m_addr[1] = static_cast<std::uint8_t>(octet1);
                ip.m_addr[2] = static_cast<std::uint8_t>(octet2);
                ip.m_addr[3] = static_cast<std::uint8_t>(octet3);
            } else {
                is.setstate(std::ios::failbit);
            }
        }
        return is;
    }
};

int main() {
    IPv4 ip;
    std::cout << "Enter IPv4 address: ";
    if (!(std::cin >> ip)) {
        std::cerr << "Invalid input\n";
        return 1;
    }

    std::cout << "You entered: " << ip << '\n';

    IPv4 next = ip;
    ++next;
    IPv4 prev = ip;
    prev--;
    
    std::cout << "Next address: "     << next << '\n';
    std::cout << "Previous address: " << prev << '\n';

    std::cout << std::boolalpha;
    std::cout << "ip == next: " << (ip == next) << '\n';
    std::cout << "ip <  next: " << (ip < next)  << '\n';
    std::cout << "ip >  prev: " << (ip > prev)  << '\n';
    return 0;
}
