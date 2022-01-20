#include "tuple_utils.hpp"
#include <cassert>
#include <utility>

struct Transformator {
    template <typename T>
    decltype(auto) operator()(T& t) const {
        return std::make_pair(t, t);
    }

    auto operator()(char& c) const { return c + 10; }
};


void test_transform_all_elements() {
    std::tuple<int, char> tp{42, 'a'};

    auto transformedTuple = cls11::transformElements(tp, Transformator{});

    assert(std::get<0>(transformedTuple) == std::make_pair(42, 42));
    assert(std::get<1>(transformedTuple) == 'k');
}

void test_transform_some_elements() {
    std::tuple<std::pair<int, int>, char> tp{std::make_pair(42, 24), 'a'};

    auto transformedTuple = cls11::transformElements(tp, [](const char &c) { return c + 10; });

    assert(std::get<0>(transformedTuple) == std::make_pair(42, 24));
    assert(std::get<1>(transformedTuple) == 'k');
}

int main() {
    test_transform_all_elements();
    test_transform_some_elements();
}
