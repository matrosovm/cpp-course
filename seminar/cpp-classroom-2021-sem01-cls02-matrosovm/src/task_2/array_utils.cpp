#include "array_utils.hpp"

int sum(const_int_pointer_t a, size_t s) {
	int total = 0;

	for (size_t i = 0; i < s; ++i) {
		total += a[i];
	}

	return total;
}

int countNumbers(const_int_pointer_t a, size_t s) {
	bool unique[10] {false};
	int total = 0;

	for (size_t i = 0; i < s; ++i) {
		if (!unique[a[i]]) {
			unique[a[i]] = true;
			++total;
		}
	}

	return total;
}

void popZeros(const_int_pointer_t *a, size_t s) {
	size_t l = 0;
	size_t edge = 0;

	while (l < s) {
		while (l < s && (a[l] == nullptr || *a[l] != 0))
			++l;

		edge = l + 1 > edge ? l + 1 : edge;
		
		while (edge < s && a[edge] != nullptr && *a[edge] == 0)
			++edge;

		if (edge == s)
			break;

		const_int_pointer_t tmp = a[l];
		a[l] = a[edge];
		a[edge] = tmp;
		
		++l;
	}
}
