#include <iostream>
#include <vector>
#include <cassert>
#include <gtest/gtest.h>
#include "pr.hpp"


struct OddNumbersGen {
  size_t operator()() {
    size_t res = num;
    num += 2;
    return res;
  }

 private:
  size_t num = 1;
};

TEST(generator_test, test_for_notation) {
  std::vector<size_t> res;
  auto gen = cls_07::generate(OddNumbersGen{});

  for (size_t odd_num = gen.current_value(); res.size() < 5; gen.move_next(), odd_num = gen.current_value()) {
    res.push_back(odd_num);
  }
  ASSERT_EQ(res, (std::vector<size_t>{{1,3,5,7,9}}));
}


TEST(generator_test, test_iterator_notation) {
  std::vector<size_t> res;
  for (size_t odd_num : cls_07::generate(OddNumbersGen{})) {
    res.push_back(odd_num);
    if (res.size() == 5) {
      break;
    }
  }
  ASSERT_EQ(res, (std::vector<size_t>{{1,3,5,7,9}}));
}


template<class T>
void test_copy_construct(const T& a) {
  T b = a;
}

template<class T>
void test_copy_assign(T& a, T& b) {
  a = b;
}


TEST(generator_test, test_move_copy) {
  auto gen = cls_07::generate(OddNumbersGen{});

  if constexpr (std::is_copy_constructible_v<decltype(gen)>) {
    test_copy_construct(gen);
  }

  ASSERT_TRUE(std::is_move_constructible_v<decltype(gen)>);
  {
    auto gen1 = std::move(gen);
  }

  if constexpr (std::is_copy_assignable_v<decltype(gen)>) {
    auto gen1 = cls_07::generate(OddNumbersGen{});
    auto gen2 = cls_07::generate(OddNumbersGen{});
    test_copy_assign(gen1, gen2);
  }

  ASSERT_TRUE(std::is_move_assignable_v<decltype(gen)>);
  {
    auto gen1 = cls_07::generate(OddNumbersGen{});
    auto gen2 = cls_07::generate(OddNumbersGen{});
    gen2 = std::move(gen1);
  }
}


int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
