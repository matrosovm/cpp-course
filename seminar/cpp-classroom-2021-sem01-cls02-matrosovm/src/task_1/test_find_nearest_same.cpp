#include <cassert>
#include "find_nearest_same.hpp"


static void testFindNearestSame() {
    const char s[] = "abdaaacb";

    assert(findNearestSame(s, s) == 3);
    assert(findNearestSame(s, s + 1) == 6);
    assert(findNearestSame(s, s + 2) == 0);
    assert(findNearestSame(s, s + 3) == 1);
    assert(findNearestSame(s, s + 4) == 1);
    assert(findNearestSame(s, s + 5) == -1);
    assert(findNearestSame(s, s + 6) == 0);
    assert(findNearestSame(s, s + 7) == -6);
}

int main() {
    testFindNearestSame();
}
