#pragma once

#include <cstdint>

struct limine_memmap_response;

namespace PhysicalMemoryManager {
	void init(const limine_memmap_response* response);

	void* allocPage();
	void freePage(void* physicalAddress);
}
