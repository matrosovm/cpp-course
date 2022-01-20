#pragma once

#include <vector>
#include <functional>

namespace cls10 {

    template <class>
    class Event;

    template <class ArgType, class ReturnType>
    class Event<ReturnType(ArgType)> {
    public:
        Event() = default;

        template <class F>
        void subscribe(F f);

        void fire(ArgType&& arg);

    private:
        std::vector<std::function<ReturnType(ArgType)>> data_;
    };

}

namespace cls10 {

    template <class ArgType, class ReturnType>
    template <class F>
    inline void Event<ReturnType(ArgType)>::subscribe(F f) {
        data_.push_back(f);
    }

    template <class ArgType, class ReturnType>
    inline void Event<ReturnType(ArgType)>::fire(ArgType&& arg) {
        for (const auto& func : data_) {
            func(std::forward<ArgType>(arg));
        }
    }

}