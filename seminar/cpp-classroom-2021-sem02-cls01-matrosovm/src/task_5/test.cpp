#include <cassert>
#include <vector>
#include <string>

#include "zip.hpp"


 #define PART_1
// #define PART_2
 #define PART_3
 #define PART_4

#ifdef PART_1

void test_empty() {
  std::vector<int> v;
  std::string s;

  cls01::zip empty_zip(v, s);

  assert(empty_zip.begin() == empty_zip.end());
}

template<class C1, class C2>
void check_simple(C1& c1, C2& c2, cls01::zip<C1, C2>& zipper) {
  typename cls01::zip<C1, C2>::iterator iter = zipper.begin();
  size_t i = 0;
  while (iter != zipper.end()) {
    assert((*iter).template get<0>() == c1[i]);
    assert(iter->template get<1>() == c2[i]);
    ++iter;
    ++i;
  }
  assert(i == std::min(c1.size(), c2.size()));
}

void test_basic() {
  std::vector<int> v{1,2,3};
  std::string s = "hello";

  cls01::zip zipper(v, s);
  check_simple(v, s, zipper);
}


void test_make_zip() {
  std::vector<int> v{1,2,3};
  std::string s = "hello";

  auto zipper = cls01::make_zip(v, s);
  check_simple(v, s, zipper);
}

void test_can_change_through_zip() {
  std::vector<int> v{1,2,3};
  std::string s = "hello";

  cls01::zip zipper(v, s);
  std::next(zipper.begin())->get<0>() = 0;
  std::next(zipper.begin(), 2)->get<1>() = 'L';
  assert(s[2] == 'L');
  assert(v[1] == 0);
  check_simple(v, s, zipper);
}

void test_doesnt_save_state() {
  std::vector<int> v{1,2,3};
  std::string s = "hello";

  cls01::zip zipper(v, s);
  v.clear();
  assert(zipper.begin() == zipper.end());
}

void test_arrow() {
  std::vector<int> v{1,2,3};
  std::string s = "hello";

  cls01::zip zipper(v, s);
  auto it = std::next(zipper.begin());
  it->get<0>();

  v.clear();
  assert(zipper.begin() == zipper.end());
}

#endif // PART_1


#ifdef PART_2
void test_many_containers() {
  std::vector<int> v{1,2,3};
  std::string s = "hello";

  size_t i = 0;
  for (auto&& [x,y,z] : cls01::zip(v, v, v)) {
    assert(x == v[i]);
    assert(y == v[i]);
    assert(z == v[i]);
    i++;
  }
}
#endif // PART_2


#ifdef PART_3
void test_const() {
  const std::vector<int> v{1,2,3};
  std::string s = "hello";

  cls01::zip zipper(v, s);
  static_assert(!std::is_assignable_v<decltype(std::next(zipper.begin())->get<0>()), int>);
  static_assert(!std::is_assignable_v<decltype(std::next(zipper.begin())->get<1>()), char>);
}
#endif // PART_4


#ifdef PART_4
void test_iterator_decrement() {
  std::vector<int> v{1,2,3};
  std::string s = "hello";

  cls01::zip zipper(v, s);
  assert(--(std::next(zipper.begin())) == zipper.begin());
}

void test_reverse_iterators() {
  std::vector<int> v{1,2,3};
  std::string s = "hello";

  cls01::zip zipper(v, s);
  size_t idx = 0;
  for (auto it = zipper.crbegin(); it != zipper.crend(); ++it) {
    assert(it->get<0>() == v[2 - idx]);
    assert(it->get<1>() == s[4 - idx]);
    idx++;
  }
}
#endif // PART_4


int main() {
    std::vector<int> vec{1, 2, 3};
    std::string str = "Hello";
    cls01::zip<std::vector<int>, std::string> test {vec, str};

#ifdef PART_1
  test_empty();
  test_basic();
  test_make_zip();
  test_doesnt_save_state();
  test_can_change_through_zip();
#endif // PART_1

#ifdef PART_2
  test_many_containers();
#endif // PART_2

#ifdef PART_3
  test_const();
#endif // PART_3

#ifdef PART_4
  test_iterator_decrement();
  test_reverse_iterators();
#endif // PART_3

  return 0;
}
