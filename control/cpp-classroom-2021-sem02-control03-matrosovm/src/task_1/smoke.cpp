#include "extension.hpp"

#include <cassert>
#include <string>

int double_int(int val) {
    return val * 2;
}

auto double_int_ext = control_03::make_ext(double_int);

int multiply(int lhs, int rhs) {
    return lhs * rhs;
}

auto multiply_ext = control_03::make_ext(multiply);

static void test_simple_functions() {
    {
        assert((10 | double_int_ext()) == 20);
        assert((20 | double_int_ext()) == 40);

        assert((1 | double_int_ext() | double_int_ext()) == 4);
    }

    {
        assert((0 | multiply_ext(10)) == 0);
        assert((1 | multiply_ext(10)) == 10);
        assert((2 | multiply_ext(10)) == 20);

        assert((2 | multiply_ext(20) | multiply_ext(3)) == 2 * 20 * 3);
    }

    {
        assert((1 | double_int_ext() | multiply_ext(20)) == 40);
    }
}

static void test_simple_functors() {
    {
        auto ext = control_03::make_ext([](int i) { return i; });
        assert((10 | ext()) == 10);
    }

    {
        auto ext = control_03::make_ext([](int lhs, int rhs) { return lhs * rhs; });

        assert((10 | ext(20)) == 200);
    }
}


struct add {
    template <typename L, typename R>
    auto operator()(L left, R right) const {
        return left + right;
    }
};

constexpr auto add_ext = control_03::make_ext(add{});

static void test_functor_overloads() {
    assert((std::string("hello") | add_ext("hi")) == "hellohi");
    assert(("hi" | add_ext(std::string("hello"))) == "hihello");
    assert((10 | add_ext(20)) == 30);
}

static void test_mutating_extensions() {
    auto starts_with_ext = control_03::make_ext([](std::string const& s, std::string prefix) {
        return s.substr(0, prefix.size()) == prefix;
    });

    auto trim_prefix = control_03::make_ext([starts_with_ext](std::string& s, std::string prefix) {
        if (s | starts_with_ext(prefix)) {
            s = s.substr(prefix.size());
        }
    });

    std::string value {"hello!"};
    value | trim_prefix("he");

    assert(value == "llo!");
}

int main() {
    test_simple_functions();
    test_simple_functors();
    test_functor_overloads();

    test_mutating_extensions();
}