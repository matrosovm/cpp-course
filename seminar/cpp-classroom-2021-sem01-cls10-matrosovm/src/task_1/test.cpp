#include "logger.hpp"

#include <string>
#include <functional>
#include <iostream>
#include <sstream>
#include <cassert>
#include <type_traits>

std::string get_cout(const std::function<void()> &action) {
  std::ostringstream buffer;
  std::streambuf *coutbuf = std::cout.rdbuf();
  std::cout.rdbuf(buffer.rdbuf());

  action();
  std::cout.flush();
  std::cout.rdbuf(coutbuf);
  return buffer.str();
}

void f(const std::string& msg) {
  std::cout << msg;
}

static void test_simple() {
  std::string res = get_cout([](){
    std::cout << "some text!";
  });
  assert(res == "some text!");

  std::string before = "before f()\n";
  std::string after = "after f()\n";
  auto logged_f = cls10::logging_wrapper(f, before, after);
  std::string msg = "Hello from f()!";
  res = get_cout([&logged_f, &msg](){
    logged_f(msg);
  });

  std::string expected_result = before + msg + after;
  assert(res == expected_result);
}

static void test_return_value() {
  std::string before = "before f()\n";
  std::string after = "after f()\n";

  auto logged_f = cls10::logging_wrapper([](const std::string& msg) -> size_t {
    std::cout << msg;
    return msg.size();
  }, before, after);
  std::string msg = "Hello from lambda!";
  std::string res = get_cout([&logged_f, &msg](){
    auto ret_val = logged_f(msg);
    assert(ret_val == msg.size());
  });

  assert(res == before + msg + after);
}
#include <type_traits>

#include "logger.hpp"

template<class T>
struct Checker {
    static_assert(std::is_same<T, decltype(cls10::logging_wrapper(std::declval<T(int)>(), "a", "b")(1))>::value);
    Checker() {
        if constexpr (!std::is_same_v<T, void>) {
            static_assert(std::is_same<T&, decltype(cls10::logging_wrapper(std::declval<T&(int)>(), "a", "b")(1))>::value);
            static_assert(std::is_same<T&&, decltype(cls10::logging_wrapper(std::declval<T&&(int)>(), "a", "b")(1))>::value);
            static_assert(std::is_same<const T&, decltype(cls10::logging_wrapper(std::declval<const T&(int)>(), "a", "b")(1))>::value);
        }
        if constexpr (!std::is_reference_v<T>) {
            static_assert(std::is_same<T*, decltype(cls10::logging_wrapper(std::declval<T*(int)>(), "a", "b")(1))>::value);
        }
    }
};

template struct Checker<void>;
template struct Checker<int>;
template struct Checker<float&>;
template struct Checker<float**>;
int main() {
  test_simple();
  test_return_value();
  return 0;
}
