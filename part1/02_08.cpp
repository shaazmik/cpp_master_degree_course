#include <iostream>
#include <vector>

using CollatzValue = unsigned long long;
using CollatzLength = std::size_t;

CollatzLength collatz_length(CollatzValue n, std::vector<CollatzLength>& cache) {
    if (n < cache.size() && cache[n] != 0) {
        return cache[n];
    }

    CollatzLength length;
    if (n == 1) {
        length = 1;
    } else if (n % 2 == 0) {
        length = 1 + collatz_length(n / 2, cache);
    } else {
        length = 1 + collatz_length(3 * n + 1, cache);
    }

    if (n < cache.size()) {
        cache[n] = length;
    }
    return length;
}

int main() {
    const CollatzValue max_start = 100;
    std::vector<CollatzLength> cache(max_start + 1, 0);

    CollatzValue best_start = 1;
    CollatzLength best_length = 1;

    for (CollatzValue i = 1; i <= max_start; ++i) {
        CollatzLength length = collatz_length(i, cache);
        if (length > best_length) {
            best_length = length;
            best_start = i;
        }
    }

    std::cout << "Наибольшая длина последовательности Коллатца: " 
              << best_length << "\n"
              << "Начальное значение: " << best_start << std::endl;

    return 0;
}
