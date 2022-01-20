#include "find_nearest_same.hpp"

ptrdiff_t findNearestSame(const char* s, const char* c) {
	ptrdiff_t it = 1;
	bool is_limp = s == c ? false : true;
	bool is_rimp = true;

	while (is_limp || is_rimp) {
		if (is_rimp) { 
			if (*(c + it) == *c)
				return it;
			else if (*(c + it) == '\0')
				is_rimp = false;
		} 
		if (is_limp) {
			if (*(c - it) == *c)
				return -it;
			else if (c - it == s)
				is_limp = false;
		}

		++it;
	}

	return 0;
}