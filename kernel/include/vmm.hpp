#pragma once

#define VMM_PAGE_FLAG_P   (1ULL << 0)
#define VMM_PAGE_FLAG_RW  (1ULL << 1)

#include <stdint.h>

void vmm_init(const uint64_t hhdm_offset);
uint64_t vmm_phys_to_virt(const uint64_t pa);
uint64_t vmm_virt_to_phys(const uint64_t va);
void vmm_map_address(uint64_t* const pml4_va, uint64_t pa, uint64_t va, uint64_t flags);
