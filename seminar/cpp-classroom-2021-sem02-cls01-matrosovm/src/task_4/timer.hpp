#pragma once

#include <chrono>
#include <utility>
#include <iostream>
#include <iomanip>


namespace cls01 {
    template <class Function, class... Args>
    double measure_exec_time(Function&& function, Args&&... args) {
        auto start = std::chrono::steady_clock::now();
        std::forward<Function>(function)(std::forward<Args>(args)...);
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double, std::milli> d = end - start;
        return d.count();
    }
}