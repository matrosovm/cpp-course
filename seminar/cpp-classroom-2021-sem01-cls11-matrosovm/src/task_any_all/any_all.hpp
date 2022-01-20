#pragma once

#include <utility>

namespace cls11 {
    template <class... Types>
    bool any(Types&&... args) {
        return (std::forward<Types>(args) || ...);
    }

    template <class... Types>
    bool all(Types&&... args) {
        return (std::forward<Types>(args) && ...);
    }
}