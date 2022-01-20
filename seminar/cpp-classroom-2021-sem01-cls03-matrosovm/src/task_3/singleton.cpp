#include "singleton.hpp"

int inc() {
	static int i = -1;
	++i;

	return i;
}