#include "for_macro.hpp"

#include <cassert>
#include <stdexcept>
#include <vector>
#include <gtest/gtest.h>

namespace {
template<class... Args>
struct CountingVector : public std::vector<Args...> {
  using std::vector<Args...>::vector;

  constexpr typename std::vector<Args...>::reference operator[](size_t x) {
    accessed_by_index_times_++;
    return std::vector<Args...>::operator[](x);
  }

  constexpr typename std::vector<Args...>::const_reference operator[](size_t x) const {
    accessed_by_index_times_++;
    return std::vector<Args...>::operator[](x);
  }

  size_t accessed_by_index() const {
    return accessed_by_index_times_;
  }

 private:
  mutable size_t accessed_by_index_times_ = 0;
};

}

class ForMacroTest : public ::testing::Test {
 protected:
  ForMacroTest() {
    for (size_t i = 0; i < 6; i++) {
      array[i] = i * i;
    }
  }

  size_t array[6];
  std::vector<char> empty_array;
  CountingVector<int> counting_vector = {1, 2, 3, 4, 5, 6};
  std::vector<bool> bool_vector = {false, false, false, false};
};

TEST_F(ForMacroTest, OnEmptyArrayDoNothing) {
  FOR_EVERY_NTH(y, empty_array, 1) {
    (void)y;
    throw std::runtime_error("Should not be called!");
  }
}

TEST_F(ForMacroTest, ProcessOnlyEvenIndices) {
  std::vector<size_t> expected = {0, 4, 16};
  std::vector<size_t> actual;
  FOR_EVERY_NTH(y, array, 2) {
    actual.push_back(y);
  }
  ASSERT_EQ(expected, actual);
}

TEST_F(ForMacroTest, CheckIsAssignable) {
  std::vector<bool> expected = {true, false, true, false};
  FOR_EVERY_NTH(y, bool_vector, 2) {
    y = true;
  }
  ASSERT_EQ(expected, bool_vector);
}

TEST_F(ForMacroTest, CheckNoExtraOperatorCalls) {
  std::vector<bool> expected = {true, false, true, false};
  FOR_EVERY_NTH(y, counting_vector, 3) {
    y++;
  }
  ASSERT_EQ(counting_vector.accessed_by_index(), 2);
}

TEST_F(ForMacroTest, CheckForEveryNthFrom2) {
  std::vector<size_t> expected = {1, 9, 25};
  std::vector<size_t> actual;
  FOR_EVERY_NTH_FROM(y, array, 2, 1) {
    actual.push_back(y);
  }
  ASSERT_EQ(expected, actual);
}


TEST_F(ForMacroTest, CheckForEvery) {
  std::vector<size_t> expected = {1, 9, 25};
  std::vector<size_t> actual;
  FOR_EVERY(y, array) {
    y++;
  }
  for (size_t i = 0; i < 6; i++) {
    ASSERT_EQ(array[i], i * i + 1);
  }
}

TEST(ForInRange, CheckForInRange) {
  std::vector<int> expected = {0, 1, 2};
  {
    std::vector<int> result;
    FOR_I_IN_RANGE(value, 3) {
      result.push_back(value);
    }
    ASSERT_EQ(expected, result);
  }

  {
    std::vector<int> result;
    const int n = 3;
    FOR_I_IN_RANGE(value, n) {
      result.push_back(value);
    }
    ASSERT_EQ(expected, result);
  }

  {
    std::vector<int> result;
    const int n = 3;
    FOR_I_IN_RANGE(value, (n)) {
      result.push_back(value);
    }
    ASSERT_EQ(expected, result);
  }
}

TEST(ForInRange, CheckForInRangeFromTo) {
  std::vector<int> expected = {2, 3, 4};
  {
    std::vector<int> result;
    FOR_I_IN_RANGE(value, 2, 5) {
      result.push_back(value);
    }
    ASSERT_EQ(expected, result);
  }

  {
    std::vector<int> result;
    const int n = 5;
    FOR_I_IN_RANGE(value, 2, n) {
      result.push_back(value);
    }
    ASSERT_EQ(expected, result);
  }
}

TEST(ForInRange, CheckForInRangeFromToStep) {
  std::vector<int> expected = {2, 4};
  {
    std::vector<int> result;
    FOR_I_IN_RANGE(value, 2, 5, 2) {
      result.push_back(value);
    }
    ASSERT_EQ(expected, result);
  }

  {
    std::vector<int> result;
    const int& from = 2;
    int volatile to = 5;

    FOR_I_IN_RANGE(value, from, to, 1+1) {
      result.push_back(value);
    }
    ASSERT_EQ(expected, result);
  }
}

TEST(ForInRange, CheckForInRangeFromToStepType) {
  std::vector<size_t> expected = {2, 4};
  std::vector<size_t> result;
  FOR_I_IN_RANGE(value, 2ul, 5ul, 1ul + 1ul) {
    result.push_back(value);
    static_assert(std::is_same_v<decltype(value), unsigned long>);
  }
  ASSERT_EQ(expected, result);
}

TEST(ForInRange, CheckForInRangeFromToStepVar) {
  std::vector<size_t> expected = {2, 4};
  std::vector<size_t> result;
  unsigned long x = 2;
  FOR_I_IN_RANGE(value, 2ul, 5ul, x) {
    result.push_back(value);
    static_assert(std::is_same_v<decltype(value), unsigned long>);
  }
  ASSERT_EQ(expected, result);
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
