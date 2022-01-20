#include "sort_algorithms.hpp"

#include <vector>
#include <array>
#include <algorithm>
#include <type_traits>
#include <cassert>

 #define BUBBLE_SORT_IMPLEMENTED
 #define MERGE_SORT_IMPLEMENTED
 #define QUICK_SORT_IMPLEMENTED
 #define COMPARATOR_IMPLEMENTED


#define CHECK_SIMPLE_SORT(CONTAINER, ALGORITHM)              \
  do {                                                       \
    auto tmp = CONTAINER;                                    \
    ALGORITHM(tmp.begin(), tmp.end());                       \
    auto ground_truth = CONTAINER;                           \
    std::sort(ground_truth.begin(), ground_truth.end());     \
    assert(tmp.size() == ground_truth.size());               \
    for (auto it1 = tmp.begin(), it2 = ground_truth.begin(); \
            it1 != tmp.end(); ++it1, ++it2) {                \
      assert(*it1 == *it2);                                  \
    }                                                        \
  } while (0)

#define CHECK_SORT_WITH_COMPARATOR(CONTAINER, ALGORITHM, COMPARATOR)  \
  do {                                                                \
    auto tmp = CONTAINER;                                             \
    ALGORITHM(tmp.begin(), tmp.end(), COMPARATOR);                    \
    auto ground_truth = CONTAINER;                                    \
    std::sort(ground_truth.begin(), ground_truth.end(), COMPARATOR);  \
    assert(tmp.size() == ground_truth.size());                        \
    for (auto it1 = tmp.begin(), it2 = ground_truth.begin();          \
            it1 != tmp.end(); ++it1, ++it2) {                         \
      assert(*it1 == *it2);                                           \
    }                                                                 \
  } while (0)

#define CHECK_SORT_DOESNT_COPY(CONTAINER, ALGORITHM)                  \
  do {                                                                \
    auto tmp = CONTAINER;                                             \
    ALGORITHM(tmp.begin(), tmp.end());                                \
    auto ground_truth = CONTAINER;                                    \
    std::sort(ground_truth.begin(), ground_truth.end());              \
    assert(tmp.size() == ground_truth.size());                        \
    for (auto it1 = tmp.begin(), it2 = ground_truth.begin();          \
            it1 != tmp.end(); ++it1, ++it2) {                         \
      assert(*it1 == *it2);                                           \
      assert(it2->CopiedTimes() == 1);                                \
      assert(it1->CopiedTimes() == 1);                                \
    }                                                                 \
  } while (0)


template<class T>
static void check_simple_sorts(T container) {
  // sanity check
  CHECK_SIMPLE_SORT(container, std::sort);

#ifdef BUBBLE_SORT_IMPLEMENTED
  CHECK_SIMPLE_SORT(container, cls09::bubble_sort);
#endif

#ifdef MERGE_SORT_IMPLEMENTED
  CHECK_SIMPLE_SORT(container, cls09::merge_sort);
#endif

#ifdef QUICK_SORT_IMPLEMENTED
  CHECK_SIMPLE_SORT(container, cls09::quick_sort);
#endif
}

template<class T>
static void check_sorts_with_comparator(T container) {
  // sanity check
  CHECK_SORT_WITH_COMPARATOR(container, std::sort, std::greater<typename T::value_type>());

#ifdef COMPARATOR_IMPLEMENTED
#ifdef BUBBLE_SORT_IMPLEMENTED
  CHECK_SORT_WITH_COMPARATOR(container, cls09::bubble_sort, std::greater<typename T::value_type>());
#endif

#ifdef MERGE_SORT_IMPLEMENTED
  CHECK_SORT_WITH_COMPARATOR(container, cls09::merge_sort, std::greater<typename T::value_type>());
#endif

#ifdef QUICK_SORT_IMPLEMENTED
  CHECK_SORT_WITH_COMPARATOR(container, cls09::quick_sort, std::greater<typename T::value_type>());
#endif
#endif
}

template<class T>
static void check_sorts_should_move(T container) {

}

static void test_simple() {
  check_simple_sorts(std::vector<size_t>{});
  check_simple_sorts(std::vector<size_t>{4,2,1,5});
  check_simple_sorts(std::vector<size_t>{1});
  check_simple_sorts(std::vector<std::string>{"a", "b", "aa"});
  check_simple_sorts(std::vector<std::string>{"a", "b", "aa"});
  check_simple_sorts(std::vector<double>{1.2, -9999, 0.1, 1e5, -12});
}

static void test_with_comparator() {
  check_sorts_with_comparator(std::vector<size_t>{});
  check_sorts_with_comparator(std::vector<size_t>{4,2,1,5});
  check_sorts_with_comparator(std::vector<size_t>{1});
  check_sorts_with_comparator(std::vector<std::string>{"a", "b", "aa"});
  check_sorts_with_comparator(std::vector<std::string>{"a", "b", "aa"});
  check_sorts_with_comparator(std::vector<double>{1.2, -9999, 0.1, 1e5, -12});
}

struct HeavyMovableStruct {
  explicit HeavyMovableStruct(const std::string& s) : data_(s) {}
  HeavyMovableStruct(const HeavyMovableStruct& other) {
    copy_counter_++;
    data_ = other.data_;
  }
  HeavyMovableStruct(const HeavyMovableStruct&& other) {
    move_counter_++;
    data_ = std::move(other.data_);
  }

  HeavyMovableStruct& operator=(const HeavyMovableStruct& other) {
    copy_counter_++;
    data_ = other.data_;
    return *this;
  }
  HeavyMovableStruct& operator=(const HeavyMovableStruct&& other) {
    move_counter_++;
    data_ = std::move(other.data_);
    return *this;
  }

  bool operator<(const HeavyMovableStruct& rhs) const {
    return data_ < rhs.data_;
  }

  bool operator==(const HeavyMovableStruct& rhs) const {
    return data_ == rhs.data_;
  }

  size_t CopiedTimes() const {
    return copy_counter_;
  }

  size_t MovedTimes() const {
    return move_counter_;
  }

 private:
  std::string data_;
  size_t copy_counter_ = 0;
  size_t move_counter_ = 0;
};


static void test_sort_should_move() {

  std::array<HeavyMovableStruct, 3> container{HeavyMovableStruct{"c"},
                                            HeavyMovableStruct{"b"},
                                            HeavyMovableStruct{"a"}};
  // sanity check
  CHECK_SORT_DOESNT_COPY(container, std::sort);
#ifdef BUBBLE_SORT_IMPLEMENTED
  CHECK_SORT_DOESNT_COPY(container, cls09::bubble_sort);
#endif

#ifdef MERGE_SORT_IMPLEMENTED
  CHECK_SORT_DOESNT_COPY(container, cls09::merge_sort);
#endif

#ifdef QUICK_SORT_IMPLEMENTED
  CHECK_SORT_DOESNT_COPY(container, cls09::quick_sort);
#endif
}

int main() {
  test_simple();
  test_with_comparator();
  test_sort_should_move();
  return 0;
}