#include <cassert>
#include "array_utils.hpp"

static void testSum() {
    assert(sum({}, 0) == 0 && "Sum of empty array should be equal zero!");

    {
        int array[]{1, 2, 3, 4, 5};

        assert(sum(array, 0) == 0 && "Sum of empty array should be equal zero!");

        assert(sum(array + 0, 1) == 1);
        assert(sum(array + 1, 1) == 2);
        assert(sum(array + 2, 1) == 3);
        assert(sum(array + 3, 1) == 4);
        assert(sum(array + 4, 1) == 5);

        assert(sum(array, 5) == 15);
    }

    {
        int const array[10]{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

        assert(sum(array, sizeof(array) / sizeof(int)) == 55);
    }
}

static void testCountNumbers() {
    assert(countNumbers({}, 0) == 0);

    {
        int const arr[] {1, 2, 3, 4, 5, 4, 3, 2, 1};

        assert(countNumbers(arr, 0) == 0);
        assert(countNumbers(arr, 1) == 1);
        assert(countNumbers(arr, 3) == 3);

        assert(countNumbers(arr, sizeof(arr) / sizeof(int)) == 5);
    }
}

static void runPopZerosTest(const_int_pointer_t *in, size_t s, const_int_pointer_t* expectedArray) {
    popZeros(in, s);
    for (size_t i = 0; i < s; ++i) {
        if (expectedArray[i] == nullptr) {
            assert(in[i] == nullptr);
        } else {
            assert(in[i] != nullptr);

            if (*(expectedArray[i]) == 0) {
                // равенство нулей, мб другой порядок
                assert(*(in[i]) == 0);
            } else {
                // равенство адресов
                assert(in[i] == expectedArray[i]);
            }
        }
    }
}

static void testPopZeros() {
    int i = 2, j = 0, k = -2, l = 0;

    const size_t N = 4;
    const_int_pointer_t a[N] = {&i, nullptr, &j, &k};
    const_int_pointer_t expected[N] = {&i, nullptr, &k, &j};
    runPopZerosTest(a, N, expected);

    const_int_pointer_t sameZerosPair[] = {&j, &j};
    const_int_pointer_t expected2[] = {&j, &j};
    runPopZerosTest(sameZerosPair, 2, expected2);

    const_int_pointer_t differentZerosPair[] = {&j, &l};
    const_int_pointer_t expected3[] = {&j, &l};
    runPopZerosTest(differentZerosPair, 2, expected3);

    const_int_pointer_t differentZerosAndNum[] = {&j, &l, &k};
    const_int_pointer_t expected4[] = {&k, &j, &l};
    runPopZerosTest(differentZerosAndNum, 3, expected4);

    const size_t N2 = 6;
    const_int_pointer_t anotherLongArray[N2] = {&l, &i, nullptr, &k, &j, nullptr};
    const_int_pointer_t expected5[N2] = {&i, nullptr, &k, nullptr, &l, &j};
    runPopZerosTest(anotherLongArray, N2, expected5);
}


int main() {
    testSum();
    testCountNumbers();
    testPopZeros();
}
