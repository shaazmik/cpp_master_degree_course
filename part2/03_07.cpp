#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <iostream>
#include <utility>

////////////////////////////////////////////////////////////////////////////////////

class Vector
{
public:
    Vector() noexcept
        : m_array(nullptr), m_size(0), m_capacity(0)
    {
        std::cout << "Vector::Vector (1)\n";
    }

    // ------------------------------------------------------------------------

    explicit Vector(std::initializer_list<int> list)
        : m_array(nullptr), m_size(std::size(list)), m_capacity(std::size(list))
    {
        std::cout << "Vector::Vector (2)\n";

        if (m_capacity > 0)
        {
            m_array = new int[m_capacity]{};
            std::ranges::copy(list, m_array);
        }
    }

    // ------------------------------------------------------------------------

    Vector(Vector const & other)
        : m_array(nullptr), m_size(other.m_size), m_capacity(other.m_capacity)
    {
        std::cout << "Vector::Vector (3)\n";

        if (m_capacity > 0)
        {
            m_array = new int[m_capacity]{};
            std::ranges::copy(other.m_array, other.m_array + other.m_size, m_array);
        }
    }

    // ------------------------------------------------------------------------

    Vector(Vector&& other) noexcept
        : m_array(std::exchange(other.m_array, nullptr)),
          m_size(std::exchange(other.m_size, 0)),
          m_capacity(std::exchange(other.m_capacity, 0))
    {
        std::cout << "Vector::Vector (4)\n";
    }

    // ------------------------------------------------------------------------

    ~Vector()
    {
        std::cout << "Vector::~Vector\n";
        delete[] m_array;
    }

    // ------------------------------------------------------------------------

    auto& operator=(Vector other) noexcept
    {
        std::cout << "Vector::operator=\n";
        swap(other);
        return *this;
    }

    // ------------------------------------------------------------------------

    [[nodiscard]] std::size_t size() const noexcept
    {
        return m_size;
    }

    [[nodiscard]] std::size_t capacity() const noexcept
    {
        return m_capacity;
    }

    [[nodiscard]] bool empty() const noexcept
    {
        return m_size == 0;
    }

    // ------------------------------------------------------------------------
    void push_back(int value)
    {
        if (m_size >= m_capacity)
        {
            std::size_t new_capacity = (m_capacity == 0) ? 1 : m_capacity * 2;

            auto* new_array = new int[new_capacity]{};

            for (std::size_t i = 0; i < m_size; ++i)
            {
                new_array[i] = m_array[i];
            }

            delete[] m_array;
            m_array = new_array;
            m_capacity = new_capacity;
        }

        m_array[m_size++] = value;
    }

    // ------------------------------------------------------------------------

    void clear() noexcept
    {
        m_size = 0;
    }

    // ------------------------------------------------------------------------

    void swap(Vector& other) noexcept
    {
        std::swap(m_array, other.m_array);
        std::swap(m_size, other.m_size);
        std::swap(m_capacity, other.m_capacity);
    }

private:
    int* m_array           = nullptr;
    std::size_t m_size     = 0;
    std::size_t m_capacity = 0;
};

////////////////////////////////////////////////////////////////////////////////////

void swap(Vector& lhs, Vector& rhs) noexcept
{
    lhs.swap(rhs);
}

////////////////////////////////////////////////////////////////////////////////////

int main()
{
    Vector v1;
    v1.push_back(10);
    v1.push_back(20);
    v1.push_back(30);

    std::cout << "v1.size() = " << v1.size() << " v1.capacity() = "  << v1.capacity() << "\n";

    v1.clear();
    std::cout << "After clear: v1.size() = " << v1.size() << " v1.empty() = " << v1.empty() << "\n";

    Vector v2{1, 2, 3, 4, 5};
    v2.push_back(6);
    std::cout << "v2.size() = " << v2.size() << " v2.capacity() = "  << v2.capacity() << "\n";

    std::cout << "\n=====Check copying=====\n";
    Vector v3 = v2;  // == Vector v3(v2);
    v3.push_back(100);
    v2 = v3;

    std::cout << "\n=====Check moving=====\n";
    Vector v4 = std::move(v3);
    v4.push_back(200);
    v3 = std::move(v4);

    swap(v1, v4);
    std::cout << "After swap v1 and v4: v1.size() = " << v1.size() << " v1.capacity() = "  << v1.capacity() << "\n";
}

////////////////////////////////////////////////////////////////////////////////////
