#include <cassert>
#include "ladder_combination.hpp"

static void testLadderCombinations() {
    assert(ladderCombinations(0) == 1);

    assert(ladderCombinations(1) == 1);
    assert(ladderCombinations(2) == 2);
    assert(ladderCombinations(3) == 3);
    assert(ladderCombinations(4) == 5);

    assert(ladderCombinations(20) == 10946);
}

int main() {
    testLadderCombinations();

    return 0;
}