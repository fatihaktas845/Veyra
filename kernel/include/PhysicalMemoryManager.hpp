#pragma once

#include <cstdint>

namespace PhysicalMemoryManager {
	void init();

	void* allocPage();
	void freePage(void* physicalAddress);
}
