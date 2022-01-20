#pragma once

#include <cstddef>
#include <type_traits>

namespace cls02 {

    template <size_t From, size_t To>
    struct range {
        static constexpr size_t size() {
            return To - From;
        }

        constexpr size_t operator()(size_t index) const {
            return From + index < To ? From + index : -1;
        }

        static constexpr size_t begin() {
            return From;
        }

        static constexpr size_t end() {
            return To;
        }

        template<size_t From2, size_t To2>
        static constexpr bool lth(range<From2, To2>) {
            return From < From2 ? true : (To < To2);
        }

        static constexpr bool is_range() {
            return true;
        }
    };


    template<class M1, class M2>
    struct concat {
        constexpr std::size_t size() const {
            return M1::size() + M2::size();
        }

        constexpr std::size_t operator()(std::size_t n) const {
            if constexpr (M1::lth(M2())) {
                return M1()(n) != size_t(-1) ? M1()(n) : M2()(n - M1().size());
            }
            return M2()(n) != size_t(-1) ? M2()(n) : M1()(n - M2().size());
        }

        constexpr static bool is_concat() {
            return true;
        }

        using first_type = M1;
        using second_type = M2;
    };


    template<class M1, class M2>
    constexpr std::enable_if_t<M2::end() != M1::begin() && M1::end() != M2::begin(), concat<M1, M2>>
    operator+(M1, M2) {
        return concat<M1, M2>();
    }

    template<class M1, class M2>
    constexpr std::enable_if_t<M1::end() == M2::begin(), range<M1::begin(), M2::end()>>
    operator+(M1, M2) {
        return range<M1::begin(), M2::end()>();
    }

    template<class M1, class M2>
    constexpr std::enable_if_t<M2::end() == M1::begin(), range<M2::begin(), M1::end()>>
    operator+(M1, M2) {
        return range<M2::begin(), M1::end()> {};
    }

    template<class M1, class M2, std::enable_if_t<M1::is_concat() && M2::is_range(), bool> = true>
    constexpr auto operator+(M1, M2) {
        if constexpr (M1::second_type::is_range() && M1::second_type::end() == M2::begin()) {
            return concat<typename M1::first_type, decltype(typename M1::second_type() + M2())>();
        } else {
            return concat<M1, M2>();
        }
    }

    template<class M1, class M2, std::enable_if_t<M1::is_range() && M2::is_concat(), bool> = true>
    constexpr auto operator+(M1, M2) {
        return (M1() + typename M2::first_type()) + typename M2::second_type();
    }

    template<class M1, class M2, std::enable_if_t<M1::is_concat() && M2::is_concat(), bool> = true>
    constexpr auto operator+(M1, M2) {
        return typename M1::first_type() + typename M1::second_type() +
               typename M2::first_type() + typename M2::second_type();
    }
}
