#include <cmath>
#include <cassert>
#include <iostream>
#include "composition.hpp"

namespace {

double sqr(double x) {
  return x * x;
}

}  // anonymous namespace

int main() {
  FuncT cos_func = static_cast<FuncT>(cos);

  assert(Compose(0, 2.0) == 2.0);
  assert(Compose(1, sqr, 2.0) == 4.0);
  assert(Compose(2, cos_func, sqr, 0.0) == 1.0);
  assert(Compose(2, sqr, cos_func, 0.0) == 1.0);

  Composer composer(2, cos_func, sqr);
  assert(composer(0.0) == 1.0);
  assert(composer(1.0) == cos(1.0));
  return 0;
}
