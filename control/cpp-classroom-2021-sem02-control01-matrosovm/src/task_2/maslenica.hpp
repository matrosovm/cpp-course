#pragma once

#include <type_traits>

namespace control_01 {

    struct blin {};
    struct ikra {};

    struct base_tarelka { };

    template <class Sloy1, class... Sloi>
    struct tarelka : base_tarelka { };

    template <class...>
    struct IsPravilno;

    template <class Sloy1>
    struct IsPravilno<tarelka<Sloy1>> {
        static constexpr bool value = true;
    };

    template <class Sloy1, class Sloy2, class... Sloi>
    struct IsPravilno<tarelka<Sloy1, Sloy2, Sloi...>> {
        static constexpr bool value = !(std::is_same_v<Sloy1, ikra> && std::is_same_v<Sloy2, ikra>) &&
            IsPravilno<tarelka<Sloy2, Sloi...>>::value;
    };

    template <class...>
    struct Begin;

    template <class Sloy1, class... Sloi>
    struct Begin<tarelka<Sloy1, Sloi...>> {
        using type = Sloy1;
    };

    template <class...>
    struct End;

    template <class Sloy1>
    struct End<tarelka<Sloy1>> {
        using type = Sloy1;
    };

    template <class Sloy1, class Sloy2, class... Sloi>
    struct End<tarelka<Sloy1, Sloy2, Sloi...>> {
        using type = typename End<tarelka<Sloy2, Sloi...>>::type;
    };

    template <clas...>
    struct Assem;

    template <class... Types1, class... Types2>
    struct Assem<tarelka<Types1...>, tarelka<Types2...>> {
        using type = tarelka<Types1..., Types2...>;
    };

    template <class Tarelka>
    using begin = typename Begin<Tarelka>::type;

    template <class Tarelka>
    using end = typename End<Tarelka>::type;

    template <class Tarelka>
    constexpr bool pravilno_v = IsPravilno<Tarelka>::value;

    template <class Sleva, class Sprava,
        std::enable_if_t<std::is_same_v<Sleva, blin> && std::is_same_v<Sprava, blin>, bool> = false>
    constexpr auto operator+(Sleva, Sprava) {
        return tarelka<blin, blin> { };
    }

    template <class Sleva, class Sprava,
        std::enable_if_t<std::is_same_v<Sleva, blin> && std::is_same_v<Sprava, ikra>, bool> = false>
    constexpr auto operator+(Sleva, Sprava) {
        return tarelka<blin, ikra> { };
    }

    template <class Sleva, class Sprava,
        std::enable_if_t<std::is_same_v<Sleva, ikra> && std::is_same_v<Sprava, blin>, bool> = false>
    constexpr auto operator+(Sleva, Sprava) {
        return tarelka<ikra, blin> { };
    }

    template <class Sleva, class Sprava, std::enable_if_t<std::is_base_of_v<base_tarelka, Sleva> &&
        (std::is_same_v<blin, Sprava> ||
        (std::is_same_v<ikra, Sprava> && std::is_same_v<end<Sleva>, blin>)), bool> = false>
    constexpr auto operator+(Sleva, Sprava) {
        return (typename Assem<Sleva, tarelka<Sprava>>::type) { };
    }

    template <class Sleva, class Sprava, std::enable_if_t<std::is_base_of_v<base_tarelka, Sprava> &&
        (std::is_same_v<blin, Sleva> ||
        (std::is_same_v<ikra, Sleva> && std::is_same_v<begin<Sprava>, blin>)), bool> = false>
    constexpr auto operator+(Sleva, Sprava) {
        return (typename Assem<tarelka<Sleva>, Sprava>::type) { };
    }

    template <class Sleva, class Sprava,
        std::enable_if_t<std::is_base_of_v<base_tarelka, Sleva> && std::is_base_of_v<base_tarelka, Sprava> &&
        !(std::is_same_v<end<Sleva>, ikra> && std::is_same_v<begin<Sprava>, ikra>), bool> = false>
    constexpr auto operator+(Sleva, Sprava) {
        return (typename Assem<Sleva, Sprava>::type) { };
    }

}  // namespace control_01