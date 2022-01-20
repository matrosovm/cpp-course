#pragma once


#include <condition_variable>
#include <thread>
#include <list>
#include <functional>
#include <atomic>


namespace control_02 {

    class ExecutionQueue {
    public:
        ExecutionQueue() {
            thread_ = std::thread{[this]() {
                spin();
            }};
        }

        void post(const std::function<void()> &f) {
            {
                std::lock_guard lg(m_);
                queue_.emplace_back(f);
            }
            cv_.notify_one();
        }

        ~ExecutionQueue() {
            stopped_ = true;
            cv_.notify_all();
            thread_.join();
        }

    private:
        void spin() {
            while (true) {
                std::unique_lock ul(m_);
                cv_.wait(ul, [&]() {
                    return !queue_.empty() || stopped_;
                });
                if (stopped_) {
                    return;
                }
                std::function<void()> func = queue_.front();
                queue_.pop_front();
                ul.unlock();
                func();
            }
        }

        std::thread thread_;
        std::list<std::function<void()>> queue_;
        bool stopped_ = false;
        std::mutex m_;
        std::condition_variable cv_;
    };

    class Buffer : public std::enable_shared_from_this<Buffer> {
    public:
        static std::shared_ptr<Buffer> make_buffer(const std::optional<size_t> &max_queue_size = std::nullopt) {
            return std::shared_ptr<Buffer>{new Buffer{max_queue_size}};
        }

        void post(const std::function<void()> &f) {
            std::lock_guard lg(m_);
            std::weak_ptr<Buffer> buf = weak_from_this();
            queue_.emplace_back([buf, f]() {
                f();
                if (auto sp = buf.lock()) {
                    sp->has_running_task_ = false;
                    sp->try_send_to_execution_queue();
                }
            });
            if (max_queue_size_ && queue_.size() > *max_queue_size_) {
                queue_.pop_front();
            }
            try_send_to_execution_queue();
        }

        void move_to_execution_queue(const std::shared_ptr<ExecutionQueue> &execution_queue) {
            execution_queue_ = execution_queue;
            try_send_to_execution_queue();
        }

        size_t queue_size() const {
            return queue_.size();
        }

    private:
        explicit Buffer(const std::optional<size_t> &max_queue_size) : max_queue_size_{max_queue_size} {}

        void try_send_to_execution_queue() {
            if (!has_running_task_ && execution_queue_ && !queue_.empty()) {
                auto task = queue_.front();
                queue_.pop_front();
                execution_queue_->post(task);
                has_running_task_ = true;
            }
        }

        std::shared_ptr<ExecutionQueue> execution_queue_;
        bool has_running_task_ = false;
        std::list<std::function<void()>> queue_;
        std::optional<size_t> max_queue_size_;
        std::mutex m_;
    };

}