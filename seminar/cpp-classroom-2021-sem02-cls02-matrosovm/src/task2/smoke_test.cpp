#include "pretty_print.hpp"

#include <sstream>
#include <cassert>
#include <cstddef>

struct foo { uint8_t a, b; };
inline std::ostream& operator << (std::ostream& ost, const foo&) {
    return ost << "ThisIsFoo";
}

struct bar { uint8_t a, b, c; };

struct buz;
const buz& get_buz() {
    static bar b;
    return reinterpret_cast<const buz&>(b);
}

template<class T> void do_test(const T& t, const char* expected) {
    std::ostringstream oss;
    cls02::pretty_printer(oss, t);
    if (oss.str() != expected) {
        std::cerr
            << __PRETTY_FUNCTION__
            << " : assertion failed : "
            << "'" << oss.str() << "'"
            << " != "
            << "'" << expected << "'"
            << std::endl;
        assert(false);
    }
}

void test_int() {
    do_test((signed char)(-10), "signed int_8 -10");
    do_test((unsigned char)(200), "unsigned int_8 200");
    do_test((unsigned short)(250), "unsigned int_16 250");
    do_test((uint64_t)(1000000000000), "unsigned int_64 1000000000000");
}

void test_float() {
    do_test(0.000123F, "float 0.000123");
    do_test(0.000123, "double 0.000123");
}

void test_printable() {
    do_test("hello", "some printable hello");
    do_test(foo(), "some printable ThisIsFoo");
}

void test_standard_layout() {
    bar b{10, 20, 30};
    do_test(b, "3 bytes 10 20 30");
}

void test_incomplete() {
    do_test(get_buz(), "incomplete");
}

void test_pretty() {
    std::ostringstream oss;
    oss << "[[[" << cls02::pretty(1.23) << "]]]";
    assert(oss.str() == "[[[double 1.230000]]]");
}


int main() {
    test_int();
    test_float();
    test_printable();
    test_standard_layout();
    test_incomplete();
    test_pretty();
}
