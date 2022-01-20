#include "semaphore.hpp"

#include <gtest/gtest.h>
#include <thread>
#include <atomic>
#include <chrono>
using namespace std::chrono_literals;

struct SimpleBarrier {
    std::atomic_bool flag;

    SimpleBarrier() : flag(false) {}

    void set() {
        flag = true;
    }

    void wait() {
        while(!flag) {
            std::this_thread::sleep_for(1ms);
        }
    }

    bool wait_limited(size_t n) {
        while(!flag) {
            if (n == 0) return false;
            std::this_thread::sleep_for(1ms);
            --n;
        }
        return true;
    }
};

TEST(SemaphoreTest, AcquireAboveZero) {
    cls_06::Semaphore sem(10);
    SimpleBarrier passed;

    std::thread worker([&]() {
        for (int i = 0; i < 10; ++i) sem.acquire();
        passed.set();
    });
    ASSERT_TRUE(passed.wait_limited(100));
    worker.join();
}

TEST(SemaphoreTest, ReleaseThenAcquire) {
    cls_06::Semaphore sem(0);
    SimpleBarrier passed;

    std::thread worker([&]() {
        for (int i = 0; i < 10; ++i) sem.release();
        for (int i = 0; i < 10; ++i) sem.acquire();
        passed.set();
    });
    ASSERT_TRUE(passed.wait_limited(100));
    worker.join();
}

TEST(SemaphoreTest, AcquireThenRelease) {
    cls_06::Semaphore sem(0);

    SimpleBarrier passed0, passed1, passed2;

    std::thread worker([&]() {
        passed0.set();
        sem.acquire();
        passed1.set();
        sem.acquire();
        passed2.set();
    });
    ASSERT_TRUE(passed0.wait_limited(100));
    ASSERT_FALSE(passed1.wait_limited(100));
    sem.release();
    ASSERT_TRUE(passed1.wait_limited(100));
    ASSERT_FALSE(passed2.wait_limited(100));
    sem.release();
    ASSERT_TRUE(passed2.wait_limited(100));

    worker.join();
}

TEST(SemaphoreTest, AcquireByManyThreads) {
    cls_06::Semaphore sem(0);

    int num_passed = 0;
    SimpleBarrier passed[4];
    auto thread_body = [&](int i) {
        sem.acquire();
        passed[i].set();
    };
    std::thread workers[4];
    for (int i = 0; i < 4; ++i) workers[i] = std::thread(thread_body, i);

    num_passed = 0;
    for (int i = 0; i < 4; ++i) num_passed += passed[i].wait_limited(50);
    ASSERT_EQ(0, num_passed);

    sem.release();
    sem.release();

    num_passed = 0;
    for (int i = 0; i < 4; ++i) num_passed += passed[i].wait_limited(50);
    ASSERT_EQ(2, num_passed);

    sem.release();
    sem.release();
    num_passed = 0;
    for (int i = 0; i < 4; ++i) num_passed += passed[i].wait_limited(50);
    ASSERT_EQ(4, num_passed);

    for (auto& worker : workers) worker.join();
}
