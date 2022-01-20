#pragma once

#include <mutex>
#include <condition_variable>
#include <queue>

namespace cls_06 {

    template<class T>
    class Queue {
    public:
        explicit Queue(size_t limit) : limit_(limit) {}

        size_t size() const {
            std::lock_guard lg{m_};
            return limit_;
        }

        size_t free_size() const {
            std::lock_guard lg{m_};
            return limit_ - queue_.size();
        }

        void push(const T& elem) {
            std::unique_lock ul{m_};
            cv_non_fill_.wait(ul, [&]() {
                return queue_.size() < limit_;
            });
            queue_.emplace(elem);
            cv_non_empty_.notify_one();
        }

        T pop() {
            std::unique_lock ul{m_};
            cv_non_empty_.wait(ul, [&]() {
                return !queue_.empty();
            });
            T res = queue_.front();
            queue_.pop();
            cv_non_fill_.notify_one();
            return res;
        }

    private:
        const size_t limit_;
        std::queue<T> queue_;
        std::mutex m_;
        std::condition_variable cv_non_empty_;
        std::condition_variable cv_non_fill_;
    };

}
