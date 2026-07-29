#include "Kstring.hpp"

void* kstd::memset(void* dest, uint8_t c, uint64_t count) {
	uint8_t* d = reinterpret_cast<uint8_t*>(dest);

	while (count--)
		*d++ = c;

	return dest;
}

void* kstd::memcpy(void* dest, const void* src, uint64_t size) {
	uint8_t* d = reinterpret_cast<uint8_t*>(dest);
	const uint8_t* s = reinterpret_cast<const uint8_t*>(src);

	while (size--)
		*d++ = *s++;

	return dest;
}
