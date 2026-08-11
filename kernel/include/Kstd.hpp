#pragma once

#include <stdint.h>

namespace Kstd {
	void* memset(void* dest, int ch, uint64_t count);
	void* memcpy(void* dest, const void* src, uint64_t size);
	int memcmp(const void* ptr1, const void* ptr2, uint64_t size);
	int strcmp(const char* str1, const char* str2);

	uint64_t strToUint64(const char* str, int base = 10);

	void print(const char* c);
}
