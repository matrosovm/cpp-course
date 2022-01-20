#pragma once

#include <mutex>
#include <condition_variable>

namespace cls_06 {

    class Semaphore {
    public:
        explicit Semaphore(unsigned value) : value_(value) {}

        void acquire() {
            std::unique_lock<std::mutex> ul(m_);
            cv_.wait(ul, [&]() {
                return value_ != 0;
            });
            --value_;

        }

        void release() {
            std::lock_guard<std::mutex> lg(m_);
            ++value_;
            cv_.notify_one();
        }

    private:
        size_t value_;
        std::mutex m_;
        std::condition_variable cv_;
    };
}
