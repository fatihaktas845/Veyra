#include "kstring.h"

void* memset(void* dest, uint8_t c, uint64_t count) {
	uint8_t* d = (uint8_t*)dest;

	while (count--)
		*d++ = c;
}
