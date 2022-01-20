#pragma once

#include <algorithm>
#include <vector>
#include <mutex>
#include <thread>

namespace cls_06 {

    template<class It, class Fun>
    void parallel_for(size_t num_threads, It first, It last, Fun&& fun) {
        std::vector<std::thread> worker_threads(num_threads - 1);
        size_t distance = std::distance(first, last);
        if (distance == 0) {
            return;
        }
        size_t tail = distance % num_threads;
        size_t package_size = distance / num_threads;
        std::exception_ptr ex_ptr;
        std::mutex ex_ptr_lock;
        auto curr_it_begin = first;
        auto curr_it_end = curr_it_begin;
        std::advance(curr_it_end, tail == 0 ? package_size : (--tail, package_size + 1));

        auto thread_routine = [&ex_ptr_lock, &ex_ptr, fun](auto& begin, auto& end) {
            try {
                for (auto it = begin; it != end; ++it) {
                    {
                        std::lock_guard lg(ex_ptr_lock);
                        if (ex_ptr) {
                            break;
                        }
                    }
                    fun(*it);
                }
            } catch (...) {
                std::lock_guard lg(ex_ptr_lock);
                ex_ptr = std::current_exception();
            }
        };

        while (curr_it_end != last) {
            worker_threads.emplace_back([curr_it_begin, curr_it_end, &thread_routine]() {
                    thread_routine(curr_it_begin, curr_it_end);
                });
            curr_it_begin = curr_it_end;
            std::advance(curr_it_end, tail == 0 ? package_size : (--tail, package_size + 1));
        }

        thread_routine(curr_it_begin, curr_it_end);

        for (auto& item : worker_threads) {
            if (item.joinable()) {
                item.join();
            }
        }

        if (ex_ptr) {
            std::rethrow_exception(ex_ptr);
        }
    }
}
