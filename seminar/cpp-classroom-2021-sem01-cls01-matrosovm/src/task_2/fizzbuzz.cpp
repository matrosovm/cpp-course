#include <iostream>
#include "fizzbuzz.hpp"

void fizzbuzz(uint64_t n) {
	uint64_t i = 0;

	while (i < n) {
		++i;

		if (i % 3 == 0)
			std::cout << "Fizz";
		if (i % 5 == 0)
			std::cout << "Buzz";
		else if (i % 3 != 0) 
			std::cout << i;

		std::cout << std::endl;
	}
}