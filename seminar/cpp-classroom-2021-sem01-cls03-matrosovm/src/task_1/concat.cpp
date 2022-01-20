#include <cstddef>
#include "concat.hpp"

char const* concat(
    char const a[],
    size_t a_size,
    char const b[],
    size_t b_size,
    size_t& concat_size
) {
	concat_size = a_size + b_size;
	char* cct = new char[concat_size]();

	for (size_t i = 0; i < a_size; ++i)
		cct[i] = a[i];

	for (size_t i = 0; i < b_size; ++i)
		cct[a_size + i] = b[i];

	return cct;  
}