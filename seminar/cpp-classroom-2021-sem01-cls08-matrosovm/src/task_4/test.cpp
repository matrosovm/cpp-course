#include "priority_queue.hpp"

#include <cassert>
#include <queue>
#include <type_traits>

// раскомментируйте это, если делаете доп-задание
//#define CUSTOM_COMPARATOR

// мы требуем, чтобы front возвращал константную ссылку
// подумайте, почему, и напишите под этой проверкой
using queue_front_return_type = decltype(std::declval<cls_08::priority_queue<size_t>>().front());
static_assert(std::is_same<queue_front_return_type, size_t const&>::value);
// иначе, после изменения элемента, куча могла бы перестать
// быть кучей

static void test_priority_queue_with_custom_comparator() {
#ifdef CUSTOM_COMPARATOR
    cls_08::priority_queue<size_t, std::greater<>> queue;
    for (size_t i = 0; i < 10; ++i)
    {
        queue.push(i);
    }

    for (size_t i = 0; i < 10; ++i)
    {
        assert(queue.front() == static_cast<size_t>(i));
        queue.pop();
    }
#endif
}

static void test_priority_queue() {
    {
        cls_08::priority_queue<size_t> queue;
        for (size_t i = 0; i < 10; ++i)
        {
            queue.push(i);
        }

        for (int i = 9; i >= 0; --i)
        {
            assert(queue.front() == static_cast<size_t>(i));
            queue.pop();
        }
    }
}

int main() {
    test_priority_queue();
    test_priority_queue_with_custom_comparator();
}