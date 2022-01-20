#include "projections.hpp"

#define OPTIMIZE_ADJACENT_RANGES 1
#define OPTIMIZE_CONCATS 1

static constexpr auto r_10_30 = cls02::range<10,30>();
static constexpr auto r_30_50 = cls02::range<30,50>();
static constexpr auto r_40_70 = cls02::range<40,70>();

static_assert(r_10_30.size() == 20);
static_assert(r_10_30(5) == 15);
static_assert(r_10_30(19) == 29);
static_assert(r_10_30(20) == size_t(-1));
static_assert(r_10_30(25) == size_t(-1));

static constexpr auto r_10_70 = r_10_30 + r_40_70;

static_assert(r_10_70.size() == 50);
static_assert(r_10_70(5) == 15);
static_assert(r_10_70(19) == 29);  // относится к левому операнду
static_assert(r_10_70(20) == 40);  // а это уже - к правому операнду
static_assert(r_10_70(49) == 69);
static_assert(r_10_70(50) == size_t(-1));

#if OPTIMIZE_ADJACENT_RANGES
static constexpr auto r_10_50 = r_10_30 + r_30_50;

// одиночный диапазон - очевидно, диапазон
static_assert(std::is_same_v<std::decay_t<decltype(r_10_30)>, cls02::range<10,30>>);

// склеенный диапазон - тоже диапазон
static_assert(std::is_same_v<std::decay_t<decltype(r_10_50)>, cls02::range<10,50>>);
#endif

#if OPTIMIZE_CONCATS

// a+b+c = (a+b)+c оставляем как есть
static_assert(
    std::is_same_v<
        decltype(cls02::range<1,2>() + cls02::range<3,4>() + cls02::range<5,6>()),
        cls02::concat<
            cls02::concat<
                cls02::range<1,2>,
                cls02::range<3,4>>,
            cls02::range<5,6>>
    >);

// a+(b+c) => преобразуем к (a+b)+c
static_assert(
    std::is_same_v<
        decltype(cls02::range<1,2>() + (cls02::range<3,4>() + cls02::range<5,6>())),
        cls02::concat<
            cls02::concat<
                cls02::range<1,2>,
                cls02::range<3,4>>,
            cls02::range<5,6>>
        >);

// (a+b)+(c+d) => преобразуем к (((a+b)+c)+d
static_assert(
    std::is_same_v<
        decltype((cls02::range<1,2>() + cls02::range<3,4>()) + (cls02::range<5,6>() + cls02::range<7,8>())),
        cls02::concat<
            cls02::concat<
                cls02::concat<
                    cls02::range<1,2>,
                    cls02::range<3,4>>,
                cls02::range<5,6>>,
            cls02::range<7,8>>
    >);

// (a+b1)+(b2+c) => склеиваем смежные диапазоны (a+b12)+c
static_assert(
    std::is_same_v<
        decltype((cls02::range<1,2>() + cls02::range<3,4>()) + (cls02::range<4,6>() + cls02::range<7,8>())),
        cls02::concat<
            cls02::concat<
                cls02::range<1,2>,
                cls02::range<3,6>>,
        cls02::range<7,8>>
    >);

#endif

int main() {}
