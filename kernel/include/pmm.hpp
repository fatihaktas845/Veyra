#pragma once

#include <stdint.h>

struct limine_memmap_response;

namespace pmm {
	void init(const struct limine_memmap_response* response, const uint64_t hhdm_offset);

	void* alloc_page();
	void free_page(void* physical_address);
}
