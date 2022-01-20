#include "timer.hpp"

#include <iostream>
#include <chrono>
#include <thread>
#include <cassert>

namespace {
void heavy_func(int iterations) {
  using namespace std::chrono_literals;

  for (int i = 0; i < iterations; i++) {
    std::this_thread::sleep_for(1000ms);
  }
}

void test() {
  double one_iter_time = cls01::measure_exec_time(heavy_func, 1);
  assert(one_iter_time >= 1000.);
  assert(one_iter_time < 1500.); // we hope cpu is not extensively loaded
  double two_iter_time = cls01::measure_exec_time(heavy_func, 2);
  assert(two_iter_time >= 2000.);
}
}

int main() {
  test();
  return 0;
}
