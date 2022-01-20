#include "extract_exponent.hpp"

int32_t extractExponent(float f)
{
	uint32_t tmp = *reinterpret_cast<uint32_t*>(&f);

	return static_cast<int32_t>(tmp << 1 >> 24);
}
