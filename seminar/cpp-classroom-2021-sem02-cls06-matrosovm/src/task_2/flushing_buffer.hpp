#pragma once

#include <functional>
#include <mutex>

namespace cls_06 {

    template<class T>
    class Buffer {
    public:
        using Pack = std::vector<T>;
        using Callback = std::function<void(const Pack&)>;

        Buffer(size_t limit, Callback callback):
                limit_(limit),
                callback_(callback) {}

        void push(const T& v) {
            std::unique_lock ul(mutex_);
            pack_.emplace_back(v);
            if (pack_.size() == limit_) {
                flush_not_lock(ul);
            }
        }

        void flush() {
            Pack tmp;
            {
                std::lock_guard ul(mutex_);
                std::swap(tmp, pack_);
            }
            callback_(tmp);
        }

    private:
        const size_t limit_;
        const Callback callback_;
        Pack pack_;
        std::mutex mutex_;

        void flush_not_lock(std::unique_lock<std::mutex>& ul) {
            Pack tmp;
            std::swap(tmp, pack_);
            ul.unlock();
            callback_(tmp);
        }
    };

}
