#include <vector>
#include <cassert>
#include "permutations.hpp"

namespace {


void test_unique_permutations() {
  std::vector<int> numbers = {1, 1, 1};
  std::vector<std::vector<int>> permutations = cls01::get_all_unique_permutations(
          numbers.begin(), numbers.end());
  assert(permutations.size() == 1);
  assert(numbers == permutations[0]);
}


void test_permutations() {
  std::vector<int> numbers = {1, 1, 1};
  std::vector<std::vector<int>> permutations = cls01::get_all_permutations(
          numbers.begin(), numbers.end());
  assert(permutations.size() == 6);

  for (size_t i = 0; i < 6; i++) {
    assert(numbers == permutations[i]);
  }
}

}

int main() {
  test_unique_permutations();
  test_permutations();
  return 0;
}