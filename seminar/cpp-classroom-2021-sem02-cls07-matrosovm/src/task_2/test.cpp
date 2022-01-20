#include "task.hpp"

#include <cassert>
#include <optional>
#include <string>
#include <vector>

void test_task() {
    using cls_07::task;

    {
        auto producer = []() -> task<int> {
            co_return 1;
        };

        auto t = producer();

        assert(!t.ready());

        t.execute();
        assert(t.ready());
        assert(t.result() == 1);
    }

    {
        int result{0};

        auto int_producer = []() -> task<int> {
            co_return 1;
        };

        auto awaiter = [&]() -> task<void> {
            auto i1 = co_await int_producer();
            auto i2 = co_await int_producer();

            result = i1 + i2;
        };

        auto t = awaiter();

        assert(!t.ready());

        t.execute();
        assert(t.ready());
        assert(result == 2);
    }

    {
        auto my_to_string = [](int i) -> task<std::string> {
            co_return std::to_string(i);
        };

        auto convert_numbers = [&]() -> task<std::vector<std::string>> {
            std::vector<task<std::string>> numbers;
            auto t1 = my_to_string(1);
            numbers.push_back(my_to_string(1));
            numbers.push_back(my_to_string(2));
            numbers.push_back(my_to_string(3));

            std::vector<std::string> result;
            for (auto& number: numbers) {
                result.push_back(co_await number);
            }

            co_return result;
        };

        auto task = convert_numbers();
        assert(!task.ready());

        task.execute();
        assert(task.ready());
        assert((task.result() == std::vector<std::string>{"1", "2", "3"}));
    }
}

int main() {
    test_task();
}