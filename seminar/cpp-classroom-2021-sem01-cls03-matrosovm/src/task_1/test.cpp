#include <cassert>
#include <iostream>
#include <cstring>
#include "concat.hpp"

void test_ints() {
    int first[]{0, 1, 2, 3};
    int second[]{4, 5, 6, 7, 8, 9};

    size_t result_size;
    char const* result = concat(
            reinterpret_cast<char const*>(first),
            sizeof(first),
            reinterpret_cast<char const*>(second),
            sizeof(second),
            result_size
    );

    assert(result_size == 10 * sizeof(int));
    int const* int_result = reinterpret_cast<int const*>(result);
    for (int i = 0; i < 10; ++i) {
        assert(int_result[i] == i);
    }

    delete[] result;
}

void test_chars() {
    size_t result_size;
    char const* result = concat(
            "Hello, World!",
            7,
            "Everyone!",
            10, // we need to get that \0 byte
            result_size
    );

    assert(result_size == 7 + 10);
    assert(strcmp(result, "Hello, Everyone!") == 0);

    delete[] result;
}

void test_concat() {
    test_ints();
    test_chars();
}

int main() {
    test_concat();

    std::cout << "Task 3 tests completed successfully!" << std::endl;

    return 0;
}