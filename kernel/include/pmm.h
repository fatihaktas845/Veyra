#pragma once

#include <stdint.h>

struct limine_memmap_response;

void pmm_init(const struct limine_memmap_response* response, const uint64_t hhdm_offset);

void* pmm_alloc_page();
void pmm_free_page(void* physical_address);
