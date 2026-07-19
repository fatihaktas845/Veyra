#pragma once

#include <stdint.h>

namespace PhysicalMemoryManager {
	void init();

	void* allocPage();
	void freePage(void* physicalAddress);

	void* allocPages(uint64_t count);
	void freePages(void* physicalAddress, uint64_t count);
}
