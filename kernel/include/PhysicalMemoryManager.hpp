#pragma once

#include <stdint.h>

struct limine_memmap_response;

namespace PhysicalMemoryManager {
	void init(const limine_memmap_response* response);

	void* allocPage();
	void freePage(void* physicalAddress);
}
