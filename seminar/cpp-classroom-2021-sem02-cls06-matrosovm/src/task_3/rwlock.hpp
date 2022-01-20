#pragma once

#include <mutex>
#include <condition_variable>

namespace cls_06 {

    class RWLock {
    public:
        void lock_read() {
            std::unique_lock ul(m_);
            cv_.wait(ul, [&]() {
                return !writer;
            });
            ++readers_count_;
        }

        void unlock_read() {
            std::unique_lock ul(m_);
            cv_.wait(ul, [&]() {
                return readers_count_ != 0;
            });
            --readers_count_;
            if (readers_count_ == 0) {
                cv_.notify_all();
            }
        }

        void lock_write() {
            std::unique_lock ul(m_);
            cv_.wait(ul, [&]() {
                return readers_count_ == 0 && !writer;
            });
            writer = true;
        }

        void unlock_write() {
            std::unique_lock ul(m_);
            writer = false;
            cv_.notify_all();
        }

    private:
        size_t readers_count_ = 0;
        bool writer = false;
        std::mutex m_;
        std::condition_variable cv_;
    };
}
