#pragma once

#include <iostream>
#include <type_traits>
#include <cstring>
#include <concepts>
#include <iomanip>

namespace cls_08 {

    namespace details {

        template <class T> concept Pointer = requires(const T& value) {
            *value;
        };

        template <class T> concept CString = std::same_as<std::decay_t<std::remove_cvref_t<T>>, char*>;

    }

    template <class T> concept Outputtable = requires(std::ostream& os, const T& value) {
        os << value;
        requires !details::Pointer<T> || details::CString<T>;
    };

    template <class T> concept Binary = std::is_standard_layout_v<std::decay_t<T>> &&
        std::is_trivial_v<std::decay_t<T>> && !Outputtable<T> && !details::Pointer<T>;

    namespace details {

        template<class T>
        constexpr bool PrettyPrintableHelper();

        template<class T> concept Printable = PrettyPrintableHelper<std::decay_t<T>>();

    }

    template <class T> concept PrettyPrintablePointer =  requires(const T& value) {
        requires !std::same_as<void*, std::decay_t<T>>;
        requires !details::CString<T>;
        requires details::Pointer<T>;
        { *value } -> details::Printable;
    };

    template <class T> concept PrettyPrintableCollection = requires(const T& value) {
        { *std::begin(value) } -> details::Printable;
    };

    template <class T> concept PrettyPrintable = !std::same_as<void*, std::decay_t<T>> &&
        (PrettyPrintablePointer<T> || Outputtable<T> || Binary<T> || PrettyPrintableCollection<T>);

    template <class T>
    constexpr bool details::PrettyPrintableHelper() {
        return PrettyPrintable<T>;
    }

    ////////////////////////////////////////////

    template <Outputtable T>
    void pretty_print(std::ostream& os, const T& t) {
        os << t;
    }

    template <Binary T>
    void pretty_print(std::ostream& os, const T& t) {
        ;
    }

    template <PrettyPrintableCollection T> requires (!Outputtable<T>)
    void pretty_print(std::ostream& os, const T& t);

    template <PrettyPrintablePointer T>
    void pretty_print(std::ostream& os, const T& t) {
        ;
    }

    template <PrettyPrintableCollection T> requires (!Outputtable<T>)
    void pretty_print(std::ostream& os, const T& t) {
        os << "{ ";
        for (const auto& el : t) {
            pretty_print(os, el);
            os << ' ';
        }
        os << "}";
    }
    //////////////////////////////////////////////////////

    template <class T, bool Pretty> concept SomehowPrintable = requires(std::ostream &os, const T& value) {
        os << value;
        requires (Pretty && PrettyPrintable<T>) || (!Pretty && !PrettyPrintable<T>);
    };

    template <SomehowPrintable<true> T>
    void somehow_print(std::ostream& os, const T& t) {
        pretty_print(os, t);
    }

    template <SomehowPrintable<false> T>
    void somehow_print(std::ostream& os, const T& t) {
        os << t;
    }

}  // namespace cls_08
