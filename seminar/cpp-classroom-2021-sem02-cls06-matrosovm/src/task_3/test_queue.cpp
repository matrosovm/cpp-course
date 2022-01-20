#include "queue.hpp"
#include <gtest/gtest.h>
#include <thread>
#include <atomic>
#include <chrono>
#include <iostream>

using namespace std::chrono_literals;


TEST(QueueTest, ProducerWaitsForConsumer) {
    static constexpr int N = 10;
    static constexpr int V0 = 12345;
    cls_06::Queue<int> buf(N);

    std::atomic<bool> produced_N = false;
    std::atomic<bool> produced_N1 = false;

    std::thread producer([&]() {
        for (int i = 0; i < N; ++i) buf.push(V0 + i);
        produced_N = true;
        buf.push(V0 + N);
        produced_N1 = true;
    });

    // dummy active polling barrier
    while (!produced_N) std::this_thread::sleep_for(1ms);
    // then, for sure, sleep a bit longer
    std::this_thread::sleep_for(100ms);
    ASSERT_FALSE(produced_N1);

    std::thread consumer([&]() {
        ASSERT_EQ(V0, buf.pop());
    });

    consumer.join();
    producer.join();
}

TEST(QueueTest, ConsumerWaitsForProducer) {
    static constexpr int N = 10;
    static constexpr int V0 = 12345;
    cls_06::Queue<int> buf(N);

    std::atomic<bool> consumed_0 = false;
    std::atomic<bool> consumed_1 = false;
    std::thread consumer([&]() {
        consumed_0 = true;
        ASSERT_EQ(V0, buf.pop());
        consumed_1 = true;
    });

    // dummy active polling barrier
    while (!consumed_0) std::this_thread::sleep_for(1ms);
    // then, for sure, sleep a bit longer
    std::this_thread::sleep_for(100ms);
    ASSERT_FALSE(consumed_1);

    std::thread producer([&]() {
        buf.push(V0);
    });

    producer.join();
    consumer.join();
}

TEST(Queue, OneProducerOneConsumer) {
    static constexpr int N = 10;
    static constexpr int V0 = 12345;
    cls_06::Queue<int> buf(N);
    static constexpr int C = 1000;

    std::thread producer([&]() {
        for (int i = 0; i < C; ++i) {
            buf.push(V0 + i);
        }
    });
    std::thread consumer([&]() {
        for (int i = 0; i < C; ++i) {
            EXPECT_EQ(V0 + i, buf.pop());
        }
    });
    producer.join();
    consumer.join();
}
