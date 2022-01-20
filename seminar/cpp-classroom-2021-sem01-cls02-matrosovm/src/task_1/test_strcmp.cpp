#include <cassert>
#include "strcmp.hpp"


static void testStrcmp() {
    assert(strcmp("a", "b") < 0);
    assert(strcmp("a", "") > 0);
    assert(strcmp("aaa", "aac") < 0);
    assert(strcmp("aaac", "aaabb") > 0);
    assert(!strcmp("aaa", "aaa"));
    assert(!strcmp("bb", "bb"));
    assert(!strcmp("", ""));
}

int main() {
    testStrcmp();
}
