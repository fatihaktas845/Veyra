#include "Kstd.hpp"

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

uint64_t kstd::strToUint64(const char* str, uint64_t base) {
	uint64_t sum = 0;
	const char* s = str;

	while (*s != '\0') {
		int digit = -1;
		if (*s >= '0' && *s <= '9')
			digit = *s - '0';
		else if (*s >= 'a' && *s <= 'z')
			digit = *s - 'a' + 10;
		else if (*s >= 'A' && *s <= 'Z')
			digit = *s - 'A' + 10;
		
		if (digit < 0 || digit >= base)
			break;
		
		sum = (sum * base) + digit;
		s++;
	}

	return sum;
}