#pragma once

#include <cstddef>
#include <type_traits>
#include <variant>

namespace cls02 {

    template <class... Types>
    struct typeset;

    namespace detail {

        template <class T, class Typeset>
        struct belongs_to;
    }

    template<class Head, class... Tail>
    struct typeset<Head, Tail...> {
        using tail_type = typeset<Tail...>;
        using head_type = std::enable_if_t<!detail::belongs_to<Head, tail_type>::value, Head>;
    };

    namespace detail {

        template <class Typeset>
        struct size;

        template <class... Types>
        struct size<typeset<Types...>> {
            static const size_t value = sizeof...(Types);
        };


        template <size_t N, class Typeset>
        struct get {
            using type = typename get<N - 1, typename Typeset::tail_type>::type;
        };

        template <class Typeset>
        struct get<0, Typeset> {
            using type = typename Typeset::head_type;
        };


        template <class T, class Typeset>
        struct belongs_to {
            static const bool value = std::is_same_v<T, typename Typeset::head_type> ||
                    belongs_to<T, typename Typeset::tail_type>::value;
        };

        template <class T>
        struct belongs_to<T, typeset<>> {
            static const bool value = false;
        };


        template <class Typeset1, class Typeset2>
        struct join;

        template <class... Types1, class... Types2>
        struct join<typeset<Types1...>, typeset<Types2...>> {
            using type = typeset<Types1..., Types2...>;
        };


        template <class Typeset1, class Typeset2>
        struct cross;

        template <class Head, class... Tail, class Typeset>
        struct cross<typeset<Head, Tail...>, Typeset> {
            using type = typename join<typename std::conditional_t<belongs_to<Head, Typeset>::value,
                                                        typeset<Head>, typeset<>>,
                    typename cross<typeset<Tail...>, Typeset>::type>::type;
        };

        template <class Typeset>
        struct cross<typeset<>, Typeset> {
            using type = typeset<>;
        };


        template <class Typeset1, class Typeset2>
        struct subtract;

        template <class Head, class... Tail, class Typeset>
        struct subtract<typeset<Head, Tail...>, Typeset> {
            using type = typename join<typename std::conditional_t<!belongs_to<Head, Typeset>::value,
                    typeset<Head>, typeset<>>,
                    typename subtract<typeset<Tail...>, Typeset>::type>::type;
        };

        template <class Typeset>
        struct subtract<typeset<>, Typeset> {
            using type = typeset<>;
        };

        template <>
        struct subtract<typeset<>, typeset<>> {
            using type = typeset<>;
        };


        template <class Typeset>
        struct variant_of;

        template <class Head, class... Tail>
        struct variant_of<typeset<Head, Tail...>> {
            using type = std::variant<Head, Tail...>;
        };

    }

    template<class Typeset> constexpr bool is_empty = detail::size<Typeset>::value == 0;
    template<class Typeset> constexpr size_t size = detail::size<Typeset>::value;
    template<size_t N, class Typeset> using get = typename detail::get<N, Typeset>::type;
    template<class T, class Typeset> constexpr bool belongs_to = detail::belongs_to<T, Typeset>::value;
    template<class Ts1, class Ts2> using cross = typename detail::cross<Ts1, Ts2>::type;
    template<class Ts1, class Ts2> using subtract = typename detail::subtract<Ts1, Ts2>::type;
    template<class Ts1, class Ts2> using join = typename detail::join<subtract<Ts1, Ts2>, Ts2>::type;

    template<class Typeset> using variant_of = typename detail::variant_of<Typeset>::type;
}