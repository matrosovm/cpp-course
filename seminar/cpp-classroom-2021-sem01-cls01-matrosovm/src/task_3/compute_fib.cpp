#include <fstream>
#include "compute_fib.hpp"

int64_t compute_fib() {
	std::ifstream file_input("input.txt");
	
	if (!file_input.is_open())
		return 0;

	int64_t n = 0;
	
	file_input >> n;

	if (n == 0)
		return  0;

	uint64_t current = 1;
	uint64_t prev = 0;

	for (int64_t i = 1; i < n; ++i) {
		uint64_t tmp = current;

		current += prev;
		prev = tmp;
	}

	return current;
}