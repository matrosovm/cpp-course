#pragma once

#include <algorithm>
#include <vector>
#include <map>


namespace cls01 {
    template<class BidirIt,
            template<typename...> class OutContainer = std::vector,
            template<typename...> class OutContainersContainer = std::vector>
    OutContainersContainer<OutContainer<typename BidirIt::value_type>>
    get_all_unique_permutations(BidirIt first, BidirIt last) {
            typedef typename BidirIt::value_type ValueType;
            OutContainersContainer<OutContainer<ValueType>> result;
            std::sort(first, last);
            do {
                result.insert(std::end(result), OutContainer<ValueType>(first, last));
            } while (std::next_permutation(first, last));
            return result;
    }


    namespace detail {
        size_t factorial(int number) {
            size_t result = 1;
            for (int i = 2; i <= number; ++i) {
                result *= i;
            }
            return result;
        }
    }


    template<class BidirIt,
            template<typename...> class OutContainer = std::vector,
            template<typename...> class OutContainersContainer = std::vector>
    OutContainersContainer<OutContainer<typename BidirIt::value_type>>
    get_all_permutations(BidirIt first, BidirIt last) {
        using ValueType = typename BidirIt::value_type;
        std::map<ValueType, int> repetitions;
        for (auto it = first; it != last; ++it) {
            ++repetitions[*it];
        }
        size_t number_repetitions = 1;
        for (auto item : repetitions) {
            number_repetitions *= detail::factorial(item.second);
        }
        OutContainersContainer<OutContainer<ValueType>> result;
        std::sort(first, last);
        do {
            std::fill_n(std::back_inserter(result), number_repetitions, OutContainer<ValueType>(first, last));
        } while (std::next_permutation(first, last));
        return result;
    }

}