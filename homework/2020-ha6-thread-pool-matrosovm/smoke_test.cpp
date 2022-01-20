#include <cassert>
#include <chrono>
#include <thread>
#include <iostream>
#include <atomic>

#include "thread_pool.hpp"

void set_true(std::atomic_bool *p)
{
    *p = true;
}

void trivial_enqueue_test()
{
    utils::thread_pool tp(1);
    std::atomic_bool runned = false;
    tp.enqueue(&set_true, &runned);

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    assert(runned);
}

void trivial_submit_test()
{
    utils::thread_pool tp(1);
    bool runned = false;
    tp.submit([&runned]() -> bool
        {
            runned = true;
            return true;
        }).get();

    assert(runned);
}

void size_test()
{
    utils::thread_pool tp(5);
    assert(tp.threads_count() == 5);
}


void fibonacci()
{
    utils::thread_pool tp(25);

    std::function<size_t(size_t)> fib;

    fib = [&tp, &fib](size_t n) -> size_t {
        if (n <= 1) {
            return 1;
        }

        auto res1 = tp.submit(fib, n - 1);
        auto res2 = tp.submit(fib, n - 2);
        return res1.get() + res2.get();
    };

    assert(tp.submit(fib, 6).get() == 13);
}

int trivial_waiter(int i)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    return i;
}

void task_is_running()
{
    utils::thread_pool tp(1);

    auto dead_task = tp.submit(&trivial_waiter, 1);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    auto task = tp.submit(&trivial_waiter, 2);
    assert(!task.is_running());
    assert(!task.is_done());
    assert(!task.is_canceled());

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    assert(task.is_running());
    assert(!task.is_done());
    assert(!task.is_canceled());

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    assert(!task.is_running());
    assert(task.is_done());
    assert(!task.is_canceled());

    assert(task.get() == 2);
}

void cancel_in_destructor()
{
    utils::thread_pool tp(1);

    for (int i = 0; i != 10; ++i)
    {
        tp.submit(&trivial_waiter, i);
    }

    auto task = tp.submit(&trivial_waiter, 42);

    std::this_thread::sleep_for(std::chrono::milliseconds(250));

    assert(!task.is_running());
    assert(task.is_done());
    assert(!task.is_canceled());

    assert(task.get() == 42);
}

void cancel()
{
    utils::thread_pool tp(1);

    auto dead_task = tp.submit(&trivial_waiter, 1);
    auto task_for_cancel = tp.submit(&trivial_waiter, 4);
    std::this_thread::sleep_for(std::chrono::milliseconds(25));

    auto task = tp.submit(&trivial_waiter, 2);
    assert(!task.is_running());
    assert(!task.is_done());
    assert(!task.is_canceled());

    std::this_thread::sleep_for(std::chrono::milliseconds(25));
    task_for_cancel.cancel();

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    assert(task.is_running());
    assert(!task.is_done());
    assert(!task.is_canceled());

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    assert(!task.is_running());
    assert(task.is_done());
    assert(!task.is_canceled());

    assert(task.get() == 2);
}

void cancel_too_late()
{
    utils::thread_pool tp(1);

    auto task_for_cancel = tp.submit(&trivial_waiter, 1);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    task_for_cancel.cancel();

    std::this_thread::sleep_for(std::chrono::milliseconds(40));

    auto task = tp.submit(&trivial_waiter, 2);
    assert(!task.is_running());
    assert(!task.is_done());
    assert(!task.is_canceled());

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    assert(task.is_running());
    assert(!task.is_done());
    assert(!task.is_canceled());

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    assert(!task.is_running());
    assert(task.is_done());
    assert(!task.is_canceled());

    assert(task.get() == 2);
}

int main()
{
    trivial_enqueue_test();
    trivial_submit_test();
    size_test();

    fibonacci();
    task_is_running();
    cancel_in_destructor();
    cancel();
    cancel_too_late();
}
