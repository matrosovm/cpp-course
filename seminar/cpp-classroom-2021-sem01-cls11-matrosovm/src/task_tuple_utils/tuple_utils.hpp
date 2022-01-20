#pragma once

#include <tuple>

namespace cls11 {
    template <class ArgKey, class... Containers>
    decltype(auto) at_many(ArgKey&& key, Containers&&... containers) {
        return std::make_tuple(std::forward<Containers>(containers)[std::forward<ArgKey>(key)]...);
    }

    namespace details {
        template <class Func, class ArgType>
        decltype(auto) func(Func&& f, ArgType&& arg) {
            if constexpr (std::is_invocable_v<Func, ArgType> != 0)
                return std::forward<Func>(f)(std::forward<ArgType>(arg));
            else
                return std::forward<ArgType>(arg);
        }

        template <class Tuple, class Func, size_t... ind>
        decltype(auto) transformElementsImpl(Tuple&& t, Func&& f, std::index_sequence<ind...>) {
            return std::make_tuple(func(std::forward<Func>(f),
                        std::get<ind>(std::forward<Tuple>(t)))...);
        }

        template <class Tuple, class Func, size_t... ind>
        decltype(auto) applyImpl(Tuple&& t, Func&& f, std::index_sequence<ind...>) {
            return std::forward<Func>(f)(std::get<ind>(std::forward<Tuple>(t))...);
        }
    }

    template <class Tuple, class Func, class Indices =
            std::make_index_sequence<std::tuple_size_v<std::decay_t<Tuple>>>>
    decltype(auto) transformElements(Tuple&& t, Func&& f) {
        return details::transformElementsImpl(std::forward<Tuple>(t), std::forward<Func>(f), Indices());
    }

    template <class Tuple, class Func, class Indices =
            std::make_index_sequence<std::tuple_size_v<std::decay_t<Tuple>>>>
    decltype(auto) apply(Func&& f, Tuple&& t) {
        return details::applyImpl(std::forward<Tuple>(t), std::forward<Func>(f), Indices());
    }
}