#include <cstddef>
#include "int_vector.hpp"

void pushBack(IntVector& v, int value) {
	if (v.size >= v.capacity_) {
		if (!v.data) {
			v.capacity_ = 1;
			v.data = new int[1]();
		} else {
			v.capacity_ *= 2;
			int* tmp = new int[v.capacity_]();

			for (size_t i = 0; i < v.size; ++i)
				tmp[i] = v.data[i];

			delete[] v.data;
			v.data = tmp;
		}
	}

	v.data[v.size] = value;
	++v.size;
}

void popBack(IntVector& v) {
	if (v.size > 0)
		--v.size;
}

void deallocate(IntVector& v) {
	v.size = 0;
	v.capacity_ = 0;
	
	if (v.data) {
		delete[] v.data;
		v.data = nullptr;
	}
}