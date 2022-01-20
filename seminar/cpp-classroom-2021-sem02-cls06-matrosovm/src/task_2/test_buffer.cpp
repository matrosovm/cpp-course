#include "flushing_buffer.hpp"

#include <gtest/gtest.h>
#include <algorithm>
#include <vector>
#include <thread>
#include <chrono>
using namespace std::chrono_literals;

TEST(BufferTest, SingleThreaded) {
    std::vector<int> result;

    cls_06::Buffer<int> buffer(10, [&](const std::vector<int>& v) {
        result.insert(result.end(), v.begin(), v.end());
    });

    for (int i = 0; i < 9; ++i) {
        buffer.push(i * 10);
        EXPECT_EQ((size_t)0, result.size());
    }

    for (int i = 9; i < 10; ++i) {
        buffer.push(i * 10);
        EXPECT_EQ((size_t)10, result.size());  // залповый сброс
    }

    for (int i = 10; i < 15; ++i) {
        buffer.push(i * 10);
        EXPECT_EQ((size_t)10, result.size());  // пока что ничего не поменялось
    }

    buffer.flush();
    EXPECT_EQ((size_t)15, result.size());  // сброс того, что было

    for (int i = 15; i < 24; ++i) {
        buffer.push(i * 10);
        EXPECT_EQ((size_t)15, result.size());  // пока что ничего не поменялось
    }

    for (int i = 24; i < 25; ++i) {
        buffer.push(i * 10);
        EXPECT_EQ((size_t)25, result.size());  // залповый сброс
    }

    // проверяем накопленное
    for (size_t i = 0; i < result.size(); ++i) {
        EXPECT_EQ(i * 10, result[i]);
    }
}

TEST(BufferTest, MultiThreadedPush) {
    std::mutex mux;
    std::vector<int> result;

    cls_06::Buffer<int> buffer(10, [&](const std::vector<int>& v) {
        std::scoped_lock lock(mux);
        result.insert(result.end(), v.begin(), v.end());
        std::this_thread::sleep_for(10ms);
    });

    auto thread_body = [&buffer](int start, int finish) {
        for (int i = start; i < finish; ++i) {
            buffer.push(i * 10);
            std::this_thread::sleep_for((i % 3) * 1ms);
        }
    };
    std::thread t1(thread_body, 0, 20);
    std::thread t2(thread_body, 20, 40);
    std::thread t3(thread_body, 40, 60);

    t1.join();
    t2.join();
    t3.join();
    std::sort(std::begin(result), std::end(result));
    EXPECT_EQ((size_t)60, result.size());
    for (size_t i = 0; i < result.size(); ++i) {
        EXPECT_EQ(i * 10, result[i]);
    }
}
