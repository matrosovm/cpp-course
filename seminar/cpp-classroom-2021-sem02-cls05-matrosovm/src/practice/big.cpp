#include "big.hpp"

#include <map>

namespace big {

template <typename K>
int useless() {
    std::map<K, K> map;
    map[0] = 1;
    map.erase(0);
    map[1] = 2;
    const auto f = map.begin();
    map.erase(f);
    map.insert({2, 3});
    const auto e = std::prev(map.begin());
    map.erase(e);
    return map.size();
}

int someFunc() {
    return useless<int>() + useless<size_t>() + useless<short>() + useless<long long>();
}

int f(const filesystem::File& file) {
    return file.getPath().size();
}

}
