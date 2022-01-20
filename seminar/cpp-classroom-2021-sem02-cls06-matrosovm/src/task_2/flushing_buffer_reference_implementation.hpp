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
            std::unique_lock lock(mux_);
            pack_.push_back(v);
            if (pack_.size() == limit_) {
                flush_impl(std::move(lock));
            }
        }
        void flush() {
            std::unique_lock lock(mux_);
            flush_impl(std::move(lock));
        }

        void flush_impl(std::unique_lock<std::mutex>&& present_lock) {
            Pack temp_pack;
            {
                std::unique_lock lock = std::move(present_lock);
                temp_pack.swap(pack_);
            }
            callback_(temp_pack);
        }
    private:
        const size_t limit_;
        const Callback callback_;
        Pack pack_;
        std::mutex mux_;
    };

}
