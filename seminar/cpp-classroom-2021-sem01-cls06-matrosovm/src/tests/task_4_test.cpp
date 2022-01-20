#include <cassert>
#include <string>
#include <cstdint>
#include "get_size.hpp"

namespace {

    struct Empty {};
    struct Simple1 {
        int x;
    };

    struct Simple2 {
        char x;
        int y;
    };

    struct Simple4 {
        long zz;
        int y;
        char z;
        int x;
    };

    struct Aligned1 {
        alignas(32) int x;
    };

// to be sure we are "on the same page"
    static_assert(sizeof(Empty) == 1);
    static_assert(sizeof(short) == 2);
    static_assert(sizeof(int) == 4);
    static_assert(sizeof(long) == 8);
    static_assert(sizeof(Simple1) == sizeof(int));
    static_assert(sizeof(Simple2) == 2 * sizeof(int)); // because padding
    static_assert(sizeof(Aligned1) == 32);

    [[maybe_unused]] void test_get_size_simple() {
        std::string simple_struct = R"(struct A {
  int x;
})";
        assert(cls_06::GetSize(simple_struct) == sizeof(Simple1));

        std::string empty_struct = R"(struct A {
})";
        assert(cls_06::GetSize(empty_struct) == sizeof(Empty));

        std::string simple_struct2 = R"(struct A {
  char x;
  int y;
})";
        assert(cls_06::GetSize(simple_struct2) == sizeof(Simple2));

        std::string simple_struct4 = R"(struct A {
  long zz;
  int y;
  char z;
  int x;
})";
        assert(cls_06::GetSize(simple_struct4) == sizeof(Simple4));
    }

    [[maybe_unused]] void test_get_size_with_alignment() {
        std::string aligned_struct = R"(struct A {
  alignas(32) int x;
})";
        assert(cls_06::GetSize(aligned_struct) == sizeof(Aligned1));
    }

} // namespace


int main() {
    test_get_size_simple();
    test_get_size_with_alignment();
    return 0;
}