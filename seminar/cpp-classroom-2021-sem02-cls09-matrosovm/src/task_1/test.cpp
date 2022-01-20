#include <gtest/gtest.h>

#include "pythagorean.hpp"

#include <ranges>

using namespace cls09;

namespace rs = std::ranges;
namespace rv = std::ranges::views;

TEST(Pythagorean, Basic) {
  std::vector<std::tuple<int, int, int>> should_be{
      {5, 4, 3},     {10, 8, 6},   {13, 12, 5},  {15, 12, 9},  {17, 15, 8},
      {20, 16, 12},  {25, 20, 15}, {25, 24, 7},  {26, 24, 10}, {29, 21, 20},
      {30, 24, 18},  {34, 30, 16}, {35, 28, 21}, {37, 35, 12}, {39, 36, 15},
      {40, 32, 24},  {41, 40, 9},  {45, 36, 27}, {50, 40, 30}, {50, 48, 14},
      {51, 45, 24},  {52, 48, 20}, {53, 45, 28}, {55, 44, 33}, {58, 42, 40},
      {60, 48, 36},  {61, 60, 11}, {65, 52, 39}, {65, 56, 33}, {65, 60, 25},
      {65, 63, 16},  {68, 60, 32}, {70, 56, 42}, {73, 55, 48}, {74, 70, 24},
      {75, 60, 45},  {75, 72, 21}, {78, 72, 30}, {80, 64, 48}, {82, 80, 18},
      {85, 68, 51},  {85, 75, 40}, {85, 77, 36}, {85, 84, 13}, {87, 63, 60},
      {89, 80, 39},  {90, 72, 54}, {91, 84, 35}, {95, 76, 57}, {97, 72, 65},
      {100, 80, 60}, {100, 96, 28}};
  int i = 0;
  auto p = Pythagoryan();
  if constexpr (isPythagoryanFlat) {
    for (auto x : p | rv::take(52)) {
      ASSERT_EQ(should_be[i++], x);
    }
  } else {
    for (auto x : p | rv::take(100)) {
      for (auto t : x) {
        ASSERT_EQ(should_be[i++], t);
      }
    }
  }
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}