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

int kstd::memcmp(const void* ptr1, const void* ptr2, uint64_t size) {
	const uint8_t* p1 = reinterpret_cast<const uint8_t*>(ptr1);
	const uint8_t* p2 = reinterpret_cast<const uint8_t*>(ptr2);

	for (uint64_t i = 0; i < size; i++)
		if (p1[i] != p2[i])
			return p1[i] < p2[i] ? -1 : 1;

	return 0;
}

int kstd::strcmp(const char* str1, const char* str2) {
	const uint8_t* s1 = reinterpret_cast<const uint8_t*>(str1);
	const uint8_t* s2 = reinterpret_cast<const uint8_t*>(str2);

	while (*s1 && (*s1 == *s2)) {
		s1++;
		s2++;
	}

	return *s1 - *s2;
}

uint64_t kstd::strToUint64(const char* str, int base) {
	uint64_t sum = 0;
	const char* s = str;

	while (*s != '\0' || *s != ' ') {
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

extern "C" {
	void* memset(void* dest, int c, uint64_t count) {
		return kstd::memset(dest, static_cast<uint8_t>(c), count);
	}
}