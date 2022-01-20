#pragma once

#include <tuple>
#include <utility>

namespace control_03 {

    namespace details {

        template <class F, class... Types>
        class helper {
        public:
            helper(F func, const Types&... args) : args_(std::make_tuple(args...)), func_(func) { }

            template <class T, class... Args>
            friend decltype(auto) operator|(T&& arg, const helper<F, Args...>& h) {
                return std::apply(h.func_, std::tuple_cat(std::make_tuple(std::forward<T>(arg)), h.args_));
            }

        private:
            std::tuple<Types...> args_;
            F func_;
        };

        template <class F>
        class extension {
        public:
            constexpr extension(F func) : func_(func) { }

            template <class... Types>
            helper<F, Types...> operator()(const Types&... args) const {
                return helper<F, Types...>(func_, args...);
            }

        private:
            F func_;
        };

    }

    template <class F>
    constexpr auto make_ext(F&& func) {
        return details::extension<F>{std::forward<F>(func)};
    }

}