#pragma once

#include <tuple>

namespace cls11 {
    namespace detail {
        template <class Tuple, class Func, size_t... ind>
        void for_each(Tuple& t, Func&& f, std::index_sequence<ind...>) {
            ((std::forward<Func>(f)(std::get<ind>(t)), void(0)), ...);
        }
    }

    template <class Tuple, class Func, class Indices =
            std::make_index_sequence<std::tuple_size_v<Tuple>>>
    void for_each(Tuple& t, Func&& f) {
        detail::for_each(t, std::forward<Func>(f), Indices());
    }
}