#pragma once

#include <iostream>
#include <sstream>
#include <type_traits>
#include <iomanip>
#include <limits>

namespace cls02 {

    template <class T, std::enable_if_t<std::is_signed_v<T> && std::is_integral_v<T>, bool> = true>
    void pretty_printer(std::ostream& ost, const T& t) {
        ost << "signed int_" << sizeof(T) * 8 << " " << static_cast<int64_t>(t);
    }

    template <class T, std::enable_if_t<std::is_unsigned_v<T> && std::is_integral_v<T>, bool> = true>
    void pretty_printer(std::ostream& ost, const T& t) {
        ost << "unsigned int_" << sizeof(T) * 8 << " " << static_cast<uint64_t>(t);
    }

    template <class T, std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
    void pretty_printer(std::ostream& ost, const T& t) {
        ost << (sizeof(T) <= sizeof(float) ? "float" : "double") << " ";
        ost << std::fixed << std::setprecision(6) << t;
    }

    namespace detail {

        template <class T, class = void>
        struct is_printable : std::false_type {
        };

        template <class T>
        struct is_printable<T, decltype(void(std::declval<std::ostream&>() << std::declval<const T&>()))>
                : std::true_type {
        };

        template <class T>
        inline constexpr bool is_printable_v = is_printable<T>::value;


        template <typename T>
        uint8_t is_complete(int(*)[sizeof(T)]);

        template <typename>
        uint16_t is_complete(...);

        template <class T>
        inline constexpr bool is_complete_v = sizeof(is_complete<T>(0)) == sizeof(uint8_t);
    }

    template <class T, std::enable_if_t<!std::is_integral_v<T> && !std::is_floating_point_v<T> &&
                                        detail::is_printable_v<T>, bool> = true>
    void pretty_printer(std::ostream& ost, const T& t) {
        ost << "some printable " << t;
    }

    template <class T, std::enable_if_t<!std::is_integral_v<T> && !std::is_floating_point_v<T> &&
                                        !detail::is_printable_v<T> && detail::is_complete_v<T>, bool> = true>
    std::enable_if_t<std::is_standard_layout_v<T>, void> pretty_printer(std::ostream& ost, const T& t) {
        ost << sizeof(T) << " bytes";
        for (size_t i = 0; i < sizeof(T); ++i) {
            ost << " " << static_cast<uint16_t>(*(reinterpret_cast<const uint8_t*>(&t) + i));
        }
    }

    template <class T, std::enable_if_t<!std::is_integral_v<T> && !std::is_floating_point_v<T> &&
                                        !detail::is_printable_v<T> && !detail::is_complete_v<T>, bool> = true>
    void pretty_printer(std::ostream& ost, const T&) {
        ost << "incomplete";
    }

    template <class T, std::enable_if_t<!std::is_integral_v<T> && !std::is_floating_point_v<T> &&
                                        !detail::is_printable_v<T> && detail::is_complete_v<T>, bool> = true>
    std::enable_if_t<!std::is_standard_layout_v<T>, void> pretty_printer(std::ostream& ost, const T&) {
        ost << "Some " << sizeof(T) << " bytes";
    }

    namespace detail {

        template <class T>
        class wrapper {
        public:
            wrapper(const T& obj) : obj_(obj) { }

            friend std::ostream& operator<<(std::ostream& ost, const wrapper& wp) {
                ost << std::setprecision(6);
                pretty_printer(ost, wp.obj_);
                return ost;
            }
        private:
            const T& obj_;
        };
    }

    template <class T>
    detail::wrapper<T> pretty(const T& t) {
        return detail::wrapper(t);
    }
}