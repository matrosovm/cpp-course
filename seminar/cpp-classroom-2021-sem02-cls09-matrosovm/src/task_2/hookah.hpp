#pragma once

#include <map>
#include <vector>
#include <ranges>
#include <numeric>

namespace cls09 {

    namespace rs = std::ranges;
    namespace rv = std::ranges::views;

    auto CountSpentMoney(std::vector<int> &visits, const int hour_price,
                         const int happy_visit_number,
                         std::map<std::string, int> &things) {
        auto not_happy_day = [happy_visit_number](int day) {
            return day % happy_visit_number != 0;
        };
        auto without_happy_days = rs::iota_view(size_t{1}, visits.size() + 1) | rv::filter(not_happy_day) |
            rv::transform([&visits, hour_price](int day) {
                return visits[day - 1] * hour_price;
            });
        int64_t sum = std::accumulate(without_happy_days.begin(), without_happy_days.end(), 0);
        auto product_names = things | rv::transform([](const auto& product) {
            return product.first;
        });
        auto product_equivalent = product_names | rv::transform([&things, sum](const auto& key) {
            return sum / things.at(key);
        });
        return std::make_pair(product_names, product_equivalent);
    }

} // namespace cls09