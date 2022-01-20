#include <iostream>

#include <algorithm>
#include <numeric>
#include <ranges>
#include <chrono>

namespace rs = std::ranges;
namespace rv = std::ranges::views;

// Примеры:
// https://ericniebler.github.io/range-v3/md_examples.html#example-views
// https://en.cppreference.com/w/cpp/ranges
// https://tristanbrindle.com/posts/rvalue-ranges-and-views

template <typename T>
void print(const T& t) {
  std::cout << t << ' ';
}

void hello() {
  rs::for_each(
      rs::iota_view{10, 20} | rv::filter([](const auto x) { return x % 2; }),
      print<int>);
}

void bad_dangling() {
  // https://tristanbrindle.com/posts/rvalue-ranges-and-views
  auto get_odd = [](const std::vector<int>& v) {
    return v | rv::filter([](const int x) { return x % 2; });
  };
  auto iota = rs::iota_view{0, 1000};
  std::vector<int> v(iota.begin(), iota.end());
  auto first_odd = get_odd(v);
  hello();
  //    no_impl<decltype(first_odd)> x;

  rs::for_each(first_odd, print<int>);
}

void good_dangling() {
  auto get_vector = []() -> std::vector<int> {
    static std::vector v{1, 2, 3, 10};
    return v;
  };
  //  auto odd = get_vector() | rv::filter([](const int x) {return x % 2; });
  //  rs::for_each(odd, print<int>);
}

struct Query {
  std::string text;
  bool enabled;
  std::chrono::system_clock::time_point last_executed;
  std::chrono::system_clock::duration period;
};

void execute(const Query& query) {

}

bool should_execute_query(const Query& query) {
  return query.enabled && std::chrono::system_clock::now() - query.last_executed >= query.period;
}

void execute_queries(const std::vector<Query>& queries) {
  auto to_execute = queries | rv::filter(should_execute_query);
  std::ranges::for_each(to_execute, execute);
}

void interesting_case() {
  {
    auto numbers = rs::iota_view{1, 10} | rv::filter([](const int x) {
                     std::cout << x << '\n';
                     return true;
                   });
    std::vector<int> v{numbers.begin(), numbers.end()};
    for (const auto x : v) {
      std::cout << x << '\n';
    }
  }
  std::cout << "---------------------------------------------------------\n";
  {
    bool already_processed = false;
    auto numbers = rs::iota_view{1, 10} | rv::filter([&](const int x) {
                     if (x == 9) {
                       return (already_processed = !already_processed);
                     }
                     return true;
                   });
    std::vector<int> v{numbers.begin(), numbers.end()};
    for (const auto x : v) {
      std::cout << x << '\n';
    }
  }
}

void non_const() {
  auto iota = rs::iota_view{0, 1000};
  std::vector<int> v(iota.begin(), iota.end());
  auto first_odd = v | rv::filter([](const int x) { return x % 2; });
  rs::begin(first_odd);
}

void flatten() {
  auto numbers = rs::iota_view(1) | rv::transform([](const int x) {
    return rs::iota_view(1, x);
  }) | rv::take(50) | rv::join;
  rs::for_each(numbers, print<int>);
}

int times2(int n)
{
  std::cout << "transform " << n << '\n';
  return n * 2;
}

bool isMultipleOf4(int n)
{
  return n % 4 == 0;
}

void tpoiasi() {
  // https://www.fluentcpp.com/2019/02/12/the-terrible-problem-of-incrementing-a-smart-iterator/
  // Input vector
  std::vector<int> numbers = {1, 2, 3, 4, 5};

//Apply transform and filter
  auto x = numbers | rv::transform(times2)
                        | rv::filter(isMultipleOf4);

// Output vector
  std::vector<int> results{x.begin(), x.end()};

// Display results
  for (auto result : results)
  {
    std::cout << result << ' ';
  }
}

int main() {
  tpoiasi();
  return 0;
}
