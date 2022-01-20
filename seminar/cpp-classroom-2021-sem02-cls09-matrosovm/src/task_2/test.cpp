#include "hookah.hpp"
#include <gtest/gtest.h>

using namespace cls09;

TEST(CountSpentMoney, Basic) {
  std::vector<int> visits{5, 10, 5, 10, 15, 10, 5};
  std::map<std::string, int> things{{"hookah", 5}, {"iphone", 30}};
  auto [keys, values] = CountSpentMoney(visits, 10, 5, things);
  std::vector<std::string> names{keys.begin(), keys.end()};
  std::vector<int> counts{values.begin(), values.end()};
  std::vector<std::string> should_be_names{"hookah", "iphone"};
  std::vector<int> should_be_counts{90, 15};
  ASSERT_EQ(names, should_be_names);
  ASSERT_EQ(counts, should_be_counts);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}