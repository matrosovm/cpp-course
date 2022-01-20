#include "tricky_shared_ptr.hpp"

#include <memory>
#include <functional>
#include <cassert>

namespace {

void test_tricky_shared_ptr() {
  struct A {
    int some_field = 1;
  };

  std::shared_ptr<int> field_ptr;
  {
    std::shared_ptr<A> a = std::make_shared<A>();
    std::function<int*(A&)> f = [](A& a) { return &(a.some_field); };
    field_ptr = cls01::field_restricted_shared_ptr(a, f);
  }
  assert(*field_ptr == 1);
}

void test_tricky_shared_ptr_empty() {
  struct A {
    int some_field = 1;
  };

  std::shared_ptr<int> field_ptr;
  {
    std::shared_ptr<A> a;
    std::function<int*(A&)> f = [](A& a) -> int* { throw std::runtime_error("should not be called"); };
    field_ptr = cls01::field_restricted_shared_ptr(a, f);
    assert(!field_ptr);
  }
  assert(!field_ptr);
}

}


int main() {
  test_tricky_shared_ptr();
  test_tricky_shared_ptr_empty();
  return 0;
}
