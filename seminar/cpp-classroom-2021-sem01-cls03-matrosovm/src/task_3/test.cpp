#include <cassert>
#include "funcs.hpp"

void test_funcs() {
    assert(inc_first() == 0);
    assert(inc_second() == 1);
    assert(inc_first() == 2);
    assert(inc_second() == 3);
    assert(inc_second() == 4);
    assert(inc_second() == 5);
    assert(inc_first() == 6);
}

int main() {
    test_funcs();

    return 0;
}
