#pragma once

#include <iostream>
#include <string>

namespace cls10 {

    template <class UnaryFunction>
    inline decltype(auto) logging_wrapper(UnaryFunction&& f, const std::string& log_before,
                                   const std::string& log_after) {
        return [&f, &log_before, &log_after](const auto& arg) -> decltype(f(arg)) {
            std::cout << log_before;
            auto ans = f(arg);
            std::cout << log_after;
            return ans;
        };
    }

    template <class ArgType>
    inline decltype(auto) logging_wrapper(void (&f)(const ArgType&), const std::string& log_before,
                                   const std::string& log_after) {
        return [&f, &log_before, &log_after](const auto& arg) {
            std::cout << log_before;
            f(arg);
            std::cout << log_after;
        };
    }

}