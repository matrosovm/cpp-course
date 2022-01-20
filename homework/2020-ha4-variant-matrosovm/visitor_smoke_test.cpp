#include <string>
#include <vector>

#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_FAST_COMPILE
#include "catch.hpp"

#include "variant.hpp"

using util::variant;
using util::get;
using util::bad_get;

TEST_CASE("Apply visitor") {
    struct visitor {
        std::string result;

        void operator()(int) {
            result += "int,";
        }
        void operator()(std::vector<int> const &) {
            result += "vec,";

        }
        void operator()(std::string const &) {
            result += "string,";
        }
        void operator()() {
            result += "empty,";
        }
    };

    visitor v;
    variant<int, std::vector<int>, std::string> const a1;
    apply_visitor(v, a1);
    variant<int, std::vector<int>, std::string> const a2(5);
    apply_visitor(v, a2);
    variant<int, std::vector<int>, std::string> const a3(std::vector<int>{});
    apply_visitor(v, a3);
    variant<int, std::vector<int>, std::string> const a4("Hey");
    apply_visitor(v, a4);
    CHECK(v.result == "empty,int,vec,string,");
}

TEST_CASE("Apply visitor reference") {
    struct visitor {
        std::string result;

        void operator()(int &) {
            result += "int,";
        }
        void operator()(std::vector<int> &) {
            result += "vec,";

        }
        void operator()(std::string &) {
            result += "string,";
        }
        void operator()() {
            result += "empty,";
        }
    };

    variant<int, std::vector<int>, std::string> a;
    visitor v;
    apply_visitor(v, a);
    a = 5;
    apply_visitor(v, a);
    a = std::vector<int>();
    apply_visitor(v, a);
    a = "Hey";
    apply_visitor(v, a);
    CHECK(v.result == "empty,int,vec,string,");
}

TEST_CASE("Apply visitor rvalue") {
    struct visitor {
        std::string result;

        void operator()(int &&) {
            result += "int,";
        }
        void operator()(std::vector<int> &&) {
            result += "vec,";

        }
        void operator()(std::string &&) {
            result += "string,";
        }
        void operator()() {
            result += "empty,";
        }
    };

    variant<int, std::vector<int>, std::string> a;
    visitor v;
    apply_visitor(v, std::move(a));
    a = 5;
    apply_visitor(v, std::move(a));
    a = std::vector<int>();
    apply_visitor(v, std::move(a));
    a = "Hey";
    apply_visitor(v, std::move(a));
    CHECK(v.result == "empty,int,vec,string,");
}
