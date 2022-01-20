#include "fibonacci.hpp"

static void test_fib() {
    static_assert(cls_08::FibonacciNumber<0>::value == 0);
    static_assert(cls_08::FibonacciNumber<1>::value == 1);

    static_assert(cls_08::FibonacciNumber<6>::value == 8);
}

int main() {
    test_fib();
}
