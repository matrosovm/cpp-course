#include "flushing_buffer.hpp"

#include <gtest/gtest.h>
#include <algorithm>
#include <vector>
#include <thread>
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

TEST(TestBufferHeavyCallback, WillNotLockOthersWhileFlushing) {
    SimpleBarrier entered_callback;
    SimpleBarrier can_leave_callback;
    auto callback = [&](const auto& ) {
        entered_callback.set();
        can_leave_callback.wait();
    };

    cls_06::Buffer<int> buf(10, callback);

    std::thread t1([&]() {
        for (int i = 0; i < 10; ++i) buf.push(i);
    });
    bool has_entered = entered_callback.wait_limited(100);
    EXPECT_TRUE(has_entered);

    SimpleBarrier passed_t2;
    std::thread t2([&]() {
        for (int i = 0; i < 9; ++i) buf.push(i);
        passed_t2.set();
    });
    bool has_passed_t2 = passed_t2.wait_limited(100);
    EXPECT_TRUE(has_passed_t2);

    can_leave_callback.set();
    t1.join();
    t2.join();
}
