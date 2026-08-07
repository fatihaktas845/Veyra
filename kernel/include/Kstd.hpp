#pragma once

#include <stdint.h>

namespace kstd {
	void* memset(void* dest, uint8_t c, uint64_t count);
	void* memcpy(void* dest, const void* src, uint64_t size);

	uint64_t strToUint64(const char* str, int base = 10);
}
