#include "vmm.hpp"

static uint64_t* vmm_kernel_pml4_base = 0;
static uint64_t  vmm_hhdm_offset = 0;

void vmm_init(const uint64_t hhdm_offset) {
	uint64_t cr3_value;
	__asm__ volatile(
			"movq %%cr3, %0"
			: "=r"(cr3_value)
	);

	const uint64_t kernel_pml4_pa = cr3_value & ~(0x0FFF);
	vmm_kernel_pml4_base = (uint64_t*)(vmm_phys_to_virt(kernel_pml4_pa));

	vmm_hhdm_offset = hhdm_offset;
}

uint64_t vmm_phys_to_virt(const uint64_t pa) {
	return pa + vmm_hhdm_offset;
}

uint64_t vmm_virt_to_phys(const uint64_t va) {
	return va - vmm_hhdm_offset;
}

void vmm_map_address(uint64_t* const pml4_va, const uint64_t pa, const uint64_t va, const uint64_t flags) {

}
