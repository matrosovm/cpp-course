#include "curry.hpp"

#include <cassert>
#include <memory>

// #define task_2_functions
// #define task_2_functors
// #define task_2_movable_only

[[maybe_unused]]
static int constant() {
    return 32;
}

[[maybe_unused]]
static int id(int i) {
    return i;
}

[[maybe_unused]]
static int add(int lhs, int rhs) {
    return lhs + rhs;
}

static void test_curry_with_simple_regular_functions() {
#ifdef task_2_functions
    {
        assert(control_03::curry(constant)() == 32);
    }

    {
        assert(control_03::curry(id)(10) == 10);
        assert(control_03::curry(id)(20) == 20);
    }

    {
        assert(control_03::curry(add)(1)(2) == 3);
        assert(control_03::curry(add)(3)(4) == 7);
    }
#endif
}

static void test_curry_with_simple_functors() {
#ifdef task_2_functors
    {
        auto constant = []() { return 32; };
        assert(control_03::curry(constant)() == 32);
    }

    {
        auto id = [](int i) { return i; };
        assert(control_03::curry(id)(10) == 10);
        assert(control_03::curry(id)(20) == 20);
    }

    {
        auto add = [](int lhs, int rhs) { return lhs + rhs; };

        assert(control_03::curry(add)(1)(2) == 3);
        assert(control_03::curry(add)(3)(4) == 7);
    }
#endif
}

static void test_move_only_args() {
#ifdef task_2_movable_only
    {
        {
            auto unwrap = [](std::unique_ptr<int> p1) {
                return *p1;
            };

            assert(control_03::curry(unwrap)(std::make_unique<int>(10)) == 10);
        }

        {
            auto unwrap = [](std::unique_ptr<int> p1) {
                return *p1;
            };

            auto curried_unwrap = control_03::curry(unwrap);
            assert(curried_unwrap(std::make_unique<int>(10)) == 10);
        }
    }

    {
        auto sum_pointers = [](std::unique_ptr<int> p1, std::unique_ptr<int> p2) {
            return *p1 + *p2;
        };

        assert(control_03::curry(sum_pointers)(std::make_unique<int>(10))(std::make_unique<int>(20)) == 30);
    }

    {
        auto sum_pointers = [](std::unique_ptr<int> p1, std::unique_ptr<int> p2) {
            return *p1 + *p2;
        };

        auto curried_sum_pointers = control_03::curry(sum_pointers);

        assert(curried_sum_pointers(std::make_unique<int>(10))(std::make_unique<int>(20)) == 30);
    }
#endif
}

int main() {
    test_curry_with_simple_functors();
    test_curry_with_simple_regular_functions();
    test_move_only_args();
}