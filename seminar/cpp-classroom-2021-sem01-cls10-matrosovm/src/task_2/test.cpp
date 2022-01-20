#include <cstddef>
#include <cassert>
#include <utility>
#include <stdexcept>

//#define CHECK_TEST_VALIDITY

#ifdef CHECK_TEST_VALIDITY
#include <deque>
namespace cls10 = std;
#else
#include "deque.hpp"
#endif  // CHECK_TEST_VALIDITY


static void smoke_test1() {
  cls10::deque<int> d1;
  for (int i = 0; i < 100; i++) {
    d1.push_front(i);
    d1.push_back(i);
  }

  auto check1 = [](const cls10::deque<int>& d) {
    assert(d.size() == 200);
    for (size_t i = 0; i < 100; i++) {
      assert(d[99 - i] == d[100 + i]);
    }
  };
  check1(d1);

  cls10::deque<int> d2(d1);
  check1(d2);

  d1.clear();
  assert(d1.empty());
  check1(d2);

  d1 = std::move(d2);
  check1(d1);
  d2 = std::move(d1);
  check1(d2);

  cls10::deque<int> d3(std::move(d2));
  check1(d3);
  for (size_t i = 0; i < 200; i++) {
    d3[i] = 0;
  }
  assert(d3[100] == 0);
}

static void smoke_test2() {
  struct NonCopiable {
    NonCopiable() = delete;
    NonCopiable(const NonCopiable&) = delete;
    NonCopiable(NonCopiable&&) = default;
    size_t i;
  };
  cls10::deque<NonCopiable> d1;
  d1.push_back(NonCopiable{1} /* aggregate initialization */);
  d1.push_front(NonCopiable{2});
  cls10::deque<NonCopiable> d2;
  d2 = std::move(d1);
  assert(d2.front().i == 2);
  assert(d2.back().i == 1);
}

static void smoke_test3() {
  struct MayThrowOnCopy {
    MayThrowOnCopy(size_t id) : id(id) {}

    MayThrowOnCopy(const MayThrowOnCopy& other) : id(other.id) {
      if (id % 5 == 0) {
        throw std::runtime_error("I warned you!");
      }
    }

    MayThrowOnCopy(MayThrowOnCopy&& other) : id(other.id) {
      if (id % 5 == 0) {
        throw std::runtime_error("I warned you!");
      }
    }

    size_t id;
  };

  cls10::deque<MayThrowOnCopy> d1;
  for (size_t i = 0; i < 10; i++) {
    cls10::deque<MayThrowOnCopy> before = d1;
    try {
      d1.push_front({i});
    } catch (std::runtime_error& e) {
      assert(d1.size() == before.size());
      for (size_t j = 0; j < d1.size(); j++) {
        assert(d1[j].id == before[j].id);
      }
    }
  }
}

int main() {
  smoke_test1();
  smoke_test2();
  smoke_test3();
  return 0;
}
