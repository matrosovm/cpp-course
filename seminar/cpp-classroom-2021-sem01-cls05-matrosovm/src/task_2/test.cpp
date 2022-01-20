#include "read_write.hpp"

#include <iostream>
#include <cassert>
#include <cstring>
#include <vector>

static void test_writing_and_reading() {
    char array[sizeof(int) + sizeof(size_t) + sizeof("Hello world!")]{};
    char* write_ptr = array;
    char const* read_ptr = array;

    write_ptr = write(write_ptr, 10);
    assert(write_ptr == array + sizeof(int));

    {
        int i;
        read_ptr = read(read_ptr, i);
        assert(i == 10);
    }

    write_ptr = write(write_ptr, size_t(20));
    assert(write_ptr == array + sizeof(int) + sizeof(size_t));

    {
        size_t i;
        read_ptr = read(read_ptr, i);
        assert(i == 20);
    }

    write_ptr = write(write_ptr, "Hello world!");
    assert(write_ptr == array + sizeof(array));
    {
        char* buffer = nullptr;
        read_ptr = read(read_ptr, buffer);
        assert(strcmp(buffer, "Hello world!") == 0);
        assert(read_ptr == array + sizeof(array));
        delete[] buffer;
    }
}


static void test_vector_writing_and_reading() {
    std::vector<int> const v1 { 1, 2, 3, 4, 5 };
    std::vector<int> const v2{ 6, 7, 8 };

    char buffer[1024]{}; // enough to store those two vectors
    char* write_ptr = buffer;
    char const* read_ptr = buffer;
    write_ptr = write(write_ptr, v1);
    write_ptr = write(write_ptr, v2);

    std::vector<int> result;
    read_ptr = read(read_ptr, result);
    read_ptr = read(read_ptr, result);

    assert((result == std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8}));
}

int main() {
    test_writing_and_reading();
    test_vector_writing_and_reading();

    std::cout << "Task 2 tests completed successfully!" << std::endl;
}