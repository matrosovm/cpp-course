#include "functions.hpp"

#include <cassert>
#include <iterator>
#include <vector>

static void test_fill() {
    {
        int array[3]{};
        cls_08::fill(std::begin(array), std::end(array), 10);

        for (auto i : array) {
            assert(i == 10);
        }
    }

    {
        std::vector<std::string> vec{"1", "2", "3", "4"};
        cls_08::fill(std::begin(vec), std::begin(vec) + 3, "0");

        assert((vec == std::vector<std::string>{"0", "0", "0", "4"}));
    }

    {
        struct strange {
            int value;

            explicit strange(int i) : value(i) {}

            strange& operator=(int i) {
                value = i;
                return *this;
            }
        };

        strange arr[]{strange{1}, strange{2}};
        cls_08::fill(std::begin(arr), std::end(arr), 10);

        for (auto i : arr) {
            assert(i.value == 10);
        }
    }
}

 static auto stable_zero() -> int {
     return 0;
 }

 static void test_generate() {
     {
         int arr[]{1, 2, 3, 4};

         cls_08::generate(std::begin(arr), std::end(arr) - 1, stable_zero);

         assert(arr[0] == 0);
         assert(arr[1] == 0);
         assert(arr[2] == 0);
         assert(arr[3] == 4);
     }

     {
         std::vector<char> vec{10};
         assert(vec.size() == 1);
     }
 }

 static void test_copy() {
     {
         char const* source[]{"3", "2", "1"};
         std::vector<std::string> vec;

         cls_08::copy(std::rbegin(source), std::rend(source) - 1, std::back_inserter(vec));

         assert((vec == std::vector<std::string>{"1", "2"}));
     }
 }

 static void test_for_each() {
     {
         int arr[]{1, 2, 3};
         int counter = 0;

         cls_08::for_each(std::begin(arr), std::end(arr) - 1, [&counter](int i) { counter += i; });

         assert(counter == 1 + 2);
     }

     {
         std::vector<std::string> vec{"a", "b", "c"};

         cls_08::for_each(std::begin(vec), std::end(vec), [&vec](std::string const& s) {
             static size_t pos = 0;
             assert(vec[pos].c_str() == s.c_str());
             ++pos;
         });
     }
 }

int main() {
    test_fill();
    test_generate();
    test_copy();
    test_for_each();
}