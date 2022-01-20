#include "parallel_for.hpp"

#include <gtest/gtest.h>
#include <algorithm>
#include <mutex>
#include <vector>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <chrono>
using namespace std::chrono_literals;

class ParallelWorkTest : public ::testing::TestWithParam<std::tuple<size_t, size_t>> {
protected:
    // params
    size_t num_items() const { return std::get<0>(GetParam()); }
    size_t num_threads() const { return std::get<1>(GetParam()); }

    // guard
    std::mutex mutex_;

    // data to process
    std::list<size_t> data_;

    // process results
    std::vector<size_t> touched_;
    std::unordered_map<std::thread::id, size_t> actions_;

    ParallelWorkTest() {
        touched_.resize(num_items(), 0);
        for (size_t i = 0, n = num_items(); i != n; ++i) {
            data_.emplace_back(i);
        }
    }

    void Touch(size_t i) {
        std::this_thread::sleep_for((i % 3) * 100us);
        std::lock_guard lock(mutex_);
        ASSERT_LT(i, num_items());
        EXPECT_FALSE(touched_[i]);
        touched_[i]++;
        actions_[std::this_thread::get_id()]++;
    }

    void ExpectAllTouchedOnce() {
        for (size_t i = 0, n = num_items(); i != n; ++i) {
            EXPECT_EQ((size_t)1, touched_[i]);
        }
    }
    void ExpectBalancedActions() {
        size_t actual_num_threads = std::min(num_items(), num_threads());
        EXPECT_EQ(actual_num_threads, actions_.size()) << " must not create redundant threads";

        if (actual_num_threads) {
            EXPECT_TRUE(actions_.count(std::this_thread::get_id())) << " must utilize current thread";
        }

        size_t part_size = num_items() / num_threads();
        for (auto [id, count] : actions_) {
            EXPECT_LE(part_size, count) << " threads must balance their payload well";
            EXPECT_LE(count, part_size + 1) << " threads must balance their payload well";
        }
    }
};

TEST_P(ParallelWorkTest, TouchesAllItemsOnce) {
    cls_06::parallel_for(
            num_threads(),
            std::begin(data_), std::end(data_),
            [this](size_t i) { Touch(i); });
    ExpectAllTouchedOnce();
}

TEST_P(ParallelWorkTest, BalancesPayload) {
    cls_06::parallel_for(
            num_threads(),
            std::begin(data_), std::end(data_),
            [this](size_t i) { Touch(i); });
    ExpectBalancedActions();
}

INSTANTIATE_TEST_SUITE_P(
        Races, ParallelWorkTest,
        ::testing::Combine(
                // num_items
                testing::Values(0, 1, 10, 100),
                // num_threads
                testing::Values(1, 2, 3, 4, 40)
                ));
