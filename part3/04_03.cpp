//@compile clang++ part3/04_03.cpp -std=c++2b -Wall -Wextra -Wpedantic
#include <iostream>
#include <vector>
#include <string>


template <typename Container>
void handle(Container& c, int value) {
    c.push_back(value);
}

template <typename Container, typename T>
void handle(Container& c, T&&) {
    // ignores all except int
}

template <typename Container, typename... Args>
void insertInts(Container& c, Args&&... args) {
    (handle(c, std::forward<Args>(args)), ...);
}

int main() {
    std::vector<int> vec;

    insertInts(vec, 1, 2.5, "hello", 3, 4, 'a', 5);

    std::cout << "Vector contents: ";
    for (int x : vec) std::cout << x << " ";
    std::cout << "\n";

    return 0;
}
