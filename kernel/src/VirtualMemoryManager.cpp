#include "VirtualMemoryManager.hpp"
#include "PhysicalMemoryManager.hpp"
#include "kstring.hpp"

VirtualAddress VirtualMemoryManager::kernelPml4 = VirtualAddress(0);

VirtualMemoryManager::VirtualMemoryManager() {
	if (this->kernelPml4.raw == 0) {
		PhysicalAddress pa;
		uint64_t cr3;

		__asm__ volatile(
				"movq %%cr3, %0"
				: "=r"(cr3)
		);

		pa.raw = cr3 & ~0xFFFULL;
		
		this->kernelPml4 = pa.toVirtualAddress();
	}
}

VirtualMemoryManager::VirtualMemoryManager(const VirtualAddress pml4) {
	if (this->kernelPml4.raw == 0) {
		PhysicalAddress pa;
		uint64_t cr3;

		__asm__ volatile(
				"movq %%cr3, %0"
				: "=r"(cr3)
		);

		pa.raw = cr3 & ~0xFFFULL;

		this->kernelPml4 = pa.toVirtualAddress();
	}

	this->setPml4(pml4);
}

void VirtualMemoryManager::setPml4(const VirtualAddress pml4) {
	this->currentPml4 = pml4;

	const uint64_t* const kernelPml4Base = this->kernelPml4.asPtr<uint64_t>();
	uint64_t* const currentPml4Base = this->currentPml4.asPtr<uint64_t>();

	for (uint64_t i = 256; i < 512; i++)
		currentPml4Base[i] = kernelPml4Base[i];
}

void VirtualMemoryManager::mapPage(const PhysicalAddress pa, const VirtualAddress va, const uint64_t flags) {
	const uint64_t aligned_pa = pa.raw & ~0xFFFULL;
	const uint64_t aligned_va = va.raw & ~0xFFFULL;

	const uint64_t pml4Index = (aligned_va >> 39) & 0x1FFULL;
	const uint64_t pdptIndex = (aligned_va >> 30) & 0x1FFULL;
	const uint64_t pdIndex   = (aligned_va >> 21) & 0x1FFULL;
	const uint64_t ptIndex   = (aligned_va >> 12) & 0x1FFULL;

	uint64_t* const pml4 = this->currentPml4.asPtr<uint64_t>();
	uint64_t* pdpt = nullptr;
	uint64_t* pd   = nullptr;
	uint64_t* pt   = nullptr;

	const uint64_t tableFlags = PAGE_FLAG_P | PAGE_FLAG_RW | PAGE_FLAG_US;

	if (pml4[pml4Index] & PAGE_FLAG_P) {
		PhysicalAddress p(pml4[pml4Index] & 0x000FFFFFFFFFF000ULL);
		pdpt = p.toVirtualAddress().asPtr<uint64_t>();
	} else {
		VirtualAddress v = this->allocPage();
		pdpt = v.asPtr<uint64_t>();
		kstd::memset(pdpt, 0, 4096);
		pml4[pml4Index] = v.toPhysicalAddress().raw | tableFlags;
	}

	if (pdpt[pdptIndex] & PAGE_FLAG_P) {
		PhysicalAddress p(pdpt[pdptIndex] & 0x000FFFFFFFFFF000ULL);
		pd = p.toVirtualAddress().asPtr<uint64_t>();
	} else {
		VirtualAddress v = this->allocPage();
		pd = v.asPtr<uint64_t>();
		kstd::memset(pd, 0, 4096);
		pdpt[pdptIndex] = v.toPhysicalAddress().raw | tableFlags;
	}

	if (pd[pdIndex] & PAGE_FLAG_P) {
		PhysicalAddress p(pd[pdIndex] & 0x000FFFFFFFFFF000ULL);
		pt = p.toVirtualAddress().asPtr<uint64_t>();
	} else {
		VirtualAddress v = this->allocPage();
		pt = v.asPtr<uint64_t>();
		kstd::memset(pt, 0, 4096);
		pd[pdIndex] = v.toPhysicalAddress().raw | tableFlags;
	}

	pt[ptIndex] = aligned_pa | flags;
}

VirtualAddress VirtualMemoryManager::allocPage() {
	const uint64_t paValue = reinterpret_cast<uint64_t>(PhysicalMemoryManager::allocPage());

	PhysicalAddress pa(paValue);
	return pa.toVirtualAddress();
}

void VirtualMemoryManager::freePage(const VirtualAddress va) {
	void* pa = reinterpret_cast<void*>(va.toPhysicalAddress().raw);

	PhysicalMemoryManager::freePage(pa);
}
