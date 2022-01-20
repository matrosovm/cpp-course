#include "optional.hpp"

#include <string>
#include <cassert>

static void test_optional_smoke() {
    {
        cls_04::optional<int> const empty;

        assert(!empty);
    }

    {
        cls_04::optional<int> const o1{10};

        assert(o1 && *o1 == 10);
    }

    {
        cls_04::optional<int> o2{20};

        assert(o2 && o2 == 20);

        *o2 = 30;

        assert(o2 && o2 == 30);

        o2 = 40;

        assert(o2 && o2 == 40);

        o2.reset();

        assert(!o2);
    }

    {
        cls_04::optional<std::string> const o3{std::string("hello!")};

        assert(o3->size() == 6);

        auto copy = o3;

        assert(o3.value() == copy.value() && &o3.value() != &copy.value());
    }

    {
        char const* original = "hello world! This is a long string!";
        std::string str{original};
        auto buffer = str.c_str();

        cls_04::optional<std::string> o4{std::move(str)};

        cls_04::optional<std::string> o5{std::move(o4)};

        assert(o4); // string was moved, not the optional
        assert(o5 && *o5 == original);

        str = std::move(*o5);
        o5 = std::move(str);
        o4 = std::move(o5);

        assert(o5 && o4 && o4 == original && o4->c_str() == buffer);
    }
}

constexpr cls_04::optional<int> if_even(int value) {
    if (value % 2 == 0) return {value}; else return {};
}

template <size_t N>
constexpr int sum_even(int const (&array)[N]) {
    int sum{0};
    for (auto i : array) {
        sum += if_even(i).value_or(0);
    }

    return sum;
}

void test_constexpr() {
    constexpr int array[]{1, 2, 3, 4};

    static_assert(sum_even(array) == 6, "Should be 6!");
}

int main() {
    test_optional_smoke();
    test_constexpr();
}