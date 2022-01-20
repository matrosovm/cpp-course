#include "rwlock.hpp"

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

TEST(RWLockTest, ReadersDoNotBlock) {
    cls_06::RWLock rwlock;

    static constexpr int N = 10;
    SimpleBarrier locked[N], hold[N], passed[N];

    auto thread_body = [&](int i) {
        rwlock.lock_read();
        locked[i].set();
        hold[i].wait();
        rwlock.unlock_read();
        passed[i].set();
    };

    std::thread readers[N];
    for (int i = 0; i < N; ++i) readers[i] = std::thread(thread_body, i);

    for (int i = 0; i < N; ++i) ASSERT_TRUE(locked[i].wait_limited(100));
    for (int i = 0; i < N; ++i) hold[i].set();
    for (int i = 0; i < N; ++i) ASSERT_TRUE(passed[i].wait_limited(100));
    for (int i = 0; i < N; ++i) readers[i].join();
}

TEST(RWLockTest, WritersDoNotCross) {
    cls_06::RWLock rwlock;

    static constexpr int N = 10;
    SimpleBarrier start[N], passed[N];
    std::atomic_bool writing = false;

    auto thread_body = [&](int i) {
        start[i].wait();
        rwlock.lock_write();

        EXPECT_FALSE(writing.exchange(true));  // ожидаем, что writing было false
        std::this_thread::sleep_for(20ms);
        EXPECT_TRUE(writing.exchange(false));  // ожидаем, что writing было true

        rwlock.unlock_write();
        passed[i].set();
    };

    std::thread writers[N];
    for (int i = 0; i < N; ++i) writers[i] = std::thread(thread_body, i);

    for (int i = 0; i < N; ++i) start[i].set();
    for (int i = 0; i < N; ++i) ASSERT_TRUE(passed[i].wait_limited(1000));
    for (int i = 0; i < N; ++i) writers[i].join();
}

TEST(RWLockTest, ReadersAfterWriter) {
    cls_06::RWLock rwlock;
    std::atomic_bool value = false;

    SimpleBarrier start_writer, finish_writer, start_readers;
    SimpleBarrier started_writer, passed_writer, passed_readers[2];

    auto writer_body = [&]() {
        rwlock.lock_write();
        start_writer.wait();
        started_writer.set();
        value = true;
        finish_writer.wait();
        rwlock.unlock_write();
        passed_writer.set();
    };
    auto reader_body = [&](int i) {
        start_readers.wait();
        rwlock.lock_read();
        EXPECT_TRUE(value);
        rwlock.unlock_read();
        passed_readers[i].set();
    };

    std::thread writer(writer_body);

    std::thread readers[2];
    for (int i = 0; i < 2; ++i) readers[i] = std::thread(reader_body, i);

    start_writer.set();  // стартовал и захватил блокировку
    ASSERT_TRUE(started_writer.wait_limited(100));
    start_readers.set();  // стартовали и ждут
    std::this_thread::sleep_for(100ms);
    finish_writer.set();

    ASSERT_TRUE(passed_writer.wait_limited(100));
    writer.join();

    for (int i = 0; i < 2; ++i) {
        ASSERT_TRUE(passed_readers[i].wait_limited(100));
        readers[i].join();
    }
}

TEST(RWLockTest, WriterAfterReaders) {
    cls_06::RWLock rwlock;
    std::atomic_bool value = false;

    SimpleBarrier start_writer, start_readers, finish_readers;
    SimpleBarrier started_readers[2], passed_writer, passed_readers[2];

    auto reader_body = [&](int i) {
        rwlock.lock_read();
        start_readers.wait();
        started_readers[i].set();
        EXPECT_FALSE(value);
        finish_readers.wait();
        rwlock.unlock_read();
        passed_readers[i].set();
    };
    auto writer_body = [&]() {
        start_writer.wait();
        rwlock.lock_write();
        EXPECT_FALSE(value);
        value = true;
        rwlock.unlock_write();
        passed_writer.set();
    };

    std::thread writer(writer_body);

    std::thread readers[2];
    for (int i = 0; i < 2; ++i) readers[i] = std::thread(reader_body, i);

    start_readers.set();  // стартовали и захватили
    for (int i = 0; i < 2; ++i) ASSERT_TRUE(started_readers->wait_limited(100));
    start_writer.set();  // стартовал и ждёт
    finish_readers.set();

    ASSERT_TRUE(passed_writer.wait_limited(100));
    writer.join();

    for (int i = 0; i < 2; ++i) {
        ASSERT_TRUE(passed_readers[i].wait_limited(100));
        readers[i].join();
    }
}
