#include <utility>
#include <iterator>
#include <algorithm>

namespace cls09 {

    template <class ForwardIt>
    inline void bubble_sort(ForwardIt it_begin, ForwardIt it_end) {
        std::ptrdiff_t dist = std::distance(it_begin, it_end);
        for (std::ptrdiff_t i = 0; i + 1 < dist; --dist) {
            auto it1 = it_begin;
            auto it2 = it1;
            ++it2;
            for (std::ptrdiff_t j = 0; j + 1 < dist; ++j) {
                if (*it2 < *it1) {
                    std::swap(*it1, *it2);
                }
                ++it1;
                ++it2;
            }
        }
    }

    template <class ForwardIt, class Comparator>
    inline void bubble_sort(ForwardIt it_begin, ForwardIt it_end, Comparator compare) {
        std::ptrdiff_t dist = std::distance(it_begin, it_end);
        for (std::ptrdiff_t i = 0; i + 1 < dist; --dist) {
            auto it1 = it_begin;
            auto it2 = it1;
            ++it2;
            for (std::ptrdiff_t j = 0; j + 1 < dist; ++j) {
                if (compare(*it2, *it1)) {
                    std::swap(*it1, *it2);
                }
                ++it1;
                ++it2;
            }
        }
    }

    template <class BidirectionalIt>
    inline void merge_sort(BidirectionalIt it_begin, BidirectionalIt it_end) {
        std::ptrdiff_t dist = std::distance(it_begin, it_end);
        if (dist < 2) {
            return;
        }
        auto it_mid = it_begin;
        std::advance(it_mid, dist / 2);
        merge_sort(it_begin, it_mid);
        merge_sort(it_mid, it_end);
        std::inplace_merge(it_begin, it_mid, it_end);
    }

    template <class BidirectionalIt, class Comparator>
    inline void merge_sort(BidirectionalIt it_begin, BidirectionalIt it_end,
            Comparator compare) {
        std::ptrdiff_t dist = std::distance(it_begin, it_end);
        if (dist < 2) {
            return;
        }
        auto it_mid = it_begin;
        std::advance(it_mid, dist / 2);
        merge_sort(it_begin, it_mid, compare);
        merge_sort(it_mid, it_end, compare);
        std::inplace_merge(it_begin, it_mid, it_end, compare);
    }

    template <class BidirectionalIt>
    inline void quick_sort(BidirectionalIt it_begin, BidirectionalIt it_end) {
        if (std::distance(it_begin, it_end) < 2) {
            return;
        }
        auto it_last = it_begin;
        auto it_mid = it_end;
        --it_mid;

        for (auto it_current = it_begin; it_current != it_mid; ++it_current) {
            if (*it_current < *it_mid) {
                std::swap(*it_last, *it_current);
                ++it_last;
            }
        }
        std::swap(*it_last, *it_mid);
        quick_sort(it_begin, it_last);
        quick_sort(it_last, it_end);
    }

    template <class BidirectionalIt, class Comparator>
    inline void quick_sort(BidirectionalIt it_begin, BidirectionalIt it_end,
            Comparator compare) {
        if (std::distance(it_begin, it_end) < 2) {
            return;
        }
        auto it_last = it_begin;
        auto it_mid = it_end;
        --it_mid;

        for (auto it_current = it_begin; it_current != it_mid; ++it_current) {
            if (compare(*it_current, *it_mid)) {
                std::swap(*it_last, *it_current);
                ++it_last;
            }
        }
        std::swap(*it_last, *it_mid);
        quick_sort(it_begin, it_last, compare);
        quick_sort(it_last, it_end, compare);
    }

}