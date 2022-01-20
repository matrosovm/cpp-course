#include "parallel_for.hpp"

#include <vector>
#include <utility>
#include <cassert>

int main() {
    const size_t num_threads = std::thread::hardware_concurrency();

    const size_t N = 10000000;

    // исходное число и длина серии Коллатца для этого числа
    std::vector<std::pair<int, int>> numbers(N);

    std::atomic_int counter = 1;
    cls_06::parallel_for(
            num_threads,
            std::begin(numbers), std::end(numbers),
            [&counter](auto& vl) {
                vl = std::pair(counter++, -1);
            });

    cls_06::parallel_for(
            num_threads,
            std::begin(numbers), std::end(numbers),
            [](auto& vl) {
                auto& [v, l] = vl;
                assert(v > 0);
                assert(l == -1);  // убедимся, что мы ещё ничего не вычисляли

                uint64_t x = v;
                int c = 0;
                while (x > 1) {
                    assert(x * 3 + 1 > x);
                    x = (x % 2) ? (x * 3 + 1) : (x / 2);
                    ++c;
                }

                l = c;
            });

    auto& vl = *std::max_element(
            std::begin(numbers), std::end(numbers),
            [](const auto& vl1, const auto& vl2) { return vl1.second < vl2.second; });
    auto [v, l] = vl;
    assert(v == 8400511);
    assert(l == 685);
}
