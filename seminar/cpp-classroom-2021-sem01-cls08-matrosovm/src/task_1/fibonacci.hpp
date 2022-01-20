#pragma once

#include <cstddef>
#include <cstdint>

namespace cls_08 {
	template <size_t Number>
	struct FibonacciNumber {
		const static uint64_t value = FibonacciNumber<Number - 1>::value + 
									FibonacciNumber<Number - 2>::value;
	};

	template <>
	struct  FibonacciNumber<0> {
		const static uint64_t value = 0;
	};

	template <>
	struct  FibonacciNumber<1> {
		const static uint64_t value = 1;
	};
} 
