#include "co_optional.hpp"

#include <cassert>
#include <sstream>
#include <string>

using std::operator""s;

template<typename T>
using result_t = std::pair<T, std::string>;

void test_co_optional() {

    {
        auto lambda = []() -> cls_07::co_optional<int> {
            co_return 10;
        };

        auto res = lambda();
        assert(res);
        assert(*res == 10);
    }

    {
        auto lambda = [] () -> cls_07::co_optional<int> {
            co_return 10;
            co_return 20;
            co_return 30;
        };

        auto result = lambda();
        assert(result);
        assert(*result == 10);
    }

    {
        auto lambda = []() -> cls_07::co_optional<int> {
            co_return cls_07::co_nullopt;
        };

        auto res = lambda();
        assert(!res);
    }

    {
        auto char_parser = [](char c, std::string s) -> cls_07::co_optional<result_t<char>> {
            if (!s.empty() && s[0] == c) co_return {c, s.substr(1)};

            co_return cls_07::co_nullopt;
        };

        auto int_parser = [](std::string s) -> cls_07::co_optional<std::pair<int, std::string>> {
            std::istringstream ss {s};
            s.clear();
            int res = 0;
            if (ss >> res) {
                getline(ss, s);
                co_return {res, s};
            }

            co_return cls_07::co_nullopt;
        };

        auto sum_parser = [&int_parser, &char_parser](std::string s) -> cls_07::co_optional<result_t<int>> {
            auto [first, rest1] = co_await int_parser(s);
            auto [_, rest2] = co_await char_parser('+', rest1);
            auto [second, rest3] = co_await int_parser(rest2);

            co_return {first + second, rest3};
        };

        {
            auto res = sum_parser("1+2");
            assert(res);
            assert((*res).first == 3);
            assert((*res).second.empty());
        }

        {
            auto res = sum_parser("1+");
            assert(!res);
        }

        {
            auto res = sum_parser("+23");
            assert(!res);
        }
    }
}


int main() {
    test_co_optional();
}