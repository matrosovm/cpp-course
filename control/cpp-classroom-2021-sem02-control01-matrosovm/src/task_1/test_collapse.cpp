#include "story_points.hpp"

#include <cassert>

namespace {

bool SameParity(const int x, const int y) { return x % 2 == y % 2; }

template <typename T> T Sum(const T &x, const T &y) { return x + y; }

bool IsVowel(char ch) {
  return ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'y' ||
         ch == 'u';
}

void CheckCollapseSequent() {
  {
    std::vector<int> nums{1, 2, 2, 4, 3, 7, 2, 5, 5, 9};
    std::vector<int> res;
    ctrl01::CollapseSequential(nums.begin(), nums.end(),
                               std::back_inserter(res), &SameParity, &Sum<int>);
    const std::vector<int> reference{1, 8, 10, 2, 19};
    assert(res == reference);
  }

  {
    std::vector<int> nums;
    std::vector<int> res;
    ctrl01::CollapseSequential(nums.begin(), nums.end(),
                               std::back_inserter(res), &SameParity, &Sum<int>);
    assert(res.empty());
  }

  {
    std::vector<std::string> names{"Daenerys", "Jon",    "Arya",   "Brienne",
                                   "Theon",    "Tyrion", "Jaimie", "Jorah"};
    std::vector<std::string> res;
    ctrl01::CollapseSequential(
        names.begin(), names.end(), std::back_inserter(res),
        [](const std::string &n1, const std::string &n2) {
          return IsVowel(n1.back()) == IsVowel(n2.back());
        },
        &Sum<std::string>);
    std::vector<std::string> reference{"DaenerysJon", "AryaBrienne",
                                       "TheonTyrion", "Jaimie", "Jorah"};
    assert(res == reference);
  }
}

} // namespace

int main() { CheckCollapseSequent(); }
