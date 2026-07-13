#pragma once

#include <stdint.h>

namespace PhysicalMemoryManager {
	void init();

	void* allocPage();
	void freePage(void* physicalAddress);
}
