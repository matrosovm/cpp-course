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

class ParallelWorkTestWithException : public ::testing::TestWithParam<std::tuple<size_t, size_t>> {
protected:
    // params
    size_t num_items() const { return std::get<0>(GetParam()); }
    size_t num_threads() const { return std::get<1>(GetParam()); }

    // guard
    std::mutex mutex_;

    // personal property
    std::thread::id main_thread_id_;

    // data to process
    std::list<size_t> data_;

    // diagnostics
    std::unordered_set<std::thread::id> triggered_;

    ParallelWorkTestWithException() {
        main_thread_id_ = std::this_thread::get_id();
        for (size_t i = 0, n = num_items(); i != n; ++i) {
            data_.emplace_back(i);
        }
    }

    bool IsMainThread() const {
        return std::this_thread::get_id() == main_thread_id_;
    }

    void Throw() {
        {
            std::lock_guard lock(mutex_);
            EXPECT_FALSE(triggered_.count(std::this_thread::get_id())) << " thread must not go on after exception";
            triggered_.insert(std::this_thread::get_id());
        }
        throw std::runtime_error("triggered!");
    }

    void Touch(size_t i) {
        std::this_thread::sleep_for((i % 3) * 100us);
    }

    void TouchOrThrowInWorkerThread(size_t i) {
        size_t trigger = std::min(num_items() / 5, (size_t)1);
        if ((i + 1) % trigger == 0 && !IsMainThread()) {
            Throw();
        } else {
            Touch(i);
        }
    }
    void TouchOrThrowInMainThread(size_t i) {
        if (IsMainThread()) {
            Throw();
        } else {
            Touch(i);
        }
    }
};

TEST_P(ParallelWorkTestWithException, ThrowInWorkerThread) {
    EXPECT_THROW(
            cls_06::parallel_for(
                    num_threads(),
                    std::begin(data_), std::end(data_),
                    [this](size_t i) { TouchOrThrowInWorkerThread(i); }),
            std::runtime_error);
}

TEST_P(ParallelWorkTestWithException, ThrowInMainThread) {
    EXPECT_THROW(
            cls_06::parallel_for(
                    num_threads(),
                    std::begin(data_), std::end(data_),
                    [this](size_t i) { TouchOrThrowInMainThread(i); }),
            std::runtime_error);
}

INSTANTIATE_TEST_SUITE_P(
        Races, ParallelWorkTestWithException,
        ::testing::Combine(
                // num_items
                testing::Values(5, 25, 100, 1000),
                // num_threads
                testing::Values(2, 3, 4, 40)
        ));
