#include "VirtualMemoryManager.hpp"
#include "PhysicalMemoryManager.hpp"
#include "Kstring.hpp"

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
		
		this->kernelPml4 = pa.toVirtualHhdmAddress();
	}

	this->currentPml4 = this->kernelPml4;
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

		this->kernelPml4 = pa.toVirtualHhdmAddress();
	}

	this->setPml4(pml4);
}

void VirtualMemoryManager::setPml4(const VirtualAddress pml4) {
	this->currentPml4 = pml4;

	const uint64_t* const kernelPml4Base = this->kernelPml4.asPtr<uint64_t>();
	uint64_t* const currentPml4Base = this->currentPml4.asPtr<uint64_t>();

	for (uint64_t i = 256; i < 512; i++)
		currentPml4Base[i] = kernelPml4Base[i];
	
	uint64_t cr3;

	__asm__ volatile(
		"mov %%cr3, %0"
		: "=r"(cr3)
	);

	cr3 |= this->currentPml4.toPhysicalHhdmAddress().raw;

	__asm__ volatile(
		"mov %0, %%cr3"
		:
		: "r"(cr3)
		: "memory"
	);
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
		pdpt = p.toVirtualHhdmAddress().asPtr<uint64_t>();
	} else {
        PhysicalAddress p(reinterpret_cast<uint64_t>(PhysicalMemoryManager::allocPage()));
		pdpt = p.toVirtualHhdmAddress().asPtr<uint64_t>();
		kstd::memset(pdpt, 0, 4096);
		pml4[pml4Index] = p.raw | tableFlags;
	}

	if (pdpt[pdptIndex] & PAGE_FLAG_P) {
		PhysicalAddress p(pdpt[pdptIndex] & 0x000FFFFFFFFFF000ULL);
		pd = p.toVirtualHhdmAddress().asPtr<uint64_t>();
	} else {
        PhysicalAddress p(reinterpret_cast<uint64_t>(PhysicalMemoryManager::allocPage()));
		pd = p.toVirtualHhdmAddress().asPtr<uint64_t>();
		kstd::memset(pd, 0, 4096);
		pdpt[pdptIndex] = p.raw | tableFlags;
	}

	if (pd[pdIndex] & PAGE_FLAG_P) {
		PhysicalAddress p(pd[pdIndex] & 0x000FFFFFFFFFF000ULL);
		pt = p.toVirtualHhdmAddress().asPtr<uint64_t>();
	} else {
        PhysicalAddress p(reinterpret_cast<uint64_t>(PhysicalMemoryManager::allocPage()));
		pt = p.toVirtualHhdmAddress().asPtr<uint64_t>();
		kstd::memset(pt, 0, 4096);
		pd[pdIndex] = p.raw | tableFlags;
	}

	pt[ptIndex] = aligned_pa | flags;

	__asm__ volatile(
		"invlpg (%0)"
		:
		:"r"(aligned_va)
		: "memory"
	);
}

void VirtualMemoryManager::unmapPage(const VirtualAddress va) {
	const uint64_t aligned_va = va.raw & ~0xFFFULL;

	const uint64_t pml4Index = (aligned_va >> 39) & 0x1FFULL;
	const uint64_t pdptIndex = (aligned_va >> 30) & 0x1FFULL;
	const uint64_t pdIndex   = (aligned_va >> 21) & 0x1FFULL;
	const uint64_t ptIndex   = (aligned_va >> 12) & 0x1FFULL;

	uint64_t* const pml4 = this->currentPml4.asPtr<uint64_t>();
	uint64_t* pdpt = nullptr;
	uint64_t* pd   = nullptr;
	uint64_t* pt   = nullptr;

	if (pml4[pml4Index] & PAGE_FLAG_P) {
		PhysicalAddress pa(pml4[pml4Index] & 0x000FFFFFFFFFF000ULL);
		pdpt = reinterpret_cast<uint64_t*>(pa.toVirtualHhdmAddress().raw);
	} else
		return;

	if (pdpt[pdptIndex] & PAGE_FLAG_P) {
		PhysicalAddress pa(pdpt[pdptIndex] & 0x000FFFFFFFFFF000ULL);
		pd = reinterpret_cast<uint64_t*>(pa.toVirtualHhdmAddress().raw);
	} else
		return;

	if (pd[pdIndex] & PAGE_FLAG_P) {
		PhysicalAddress pa(pd[pdIndex] & 0x000FFFFFFFFFF000ULL);
		pt = reinterpret_cast<uint64_t*>(pa.toVirtualHhdmAddress().raw);
	} else
		return;

	pt[ptIndex] = 0;

	__asm__ volatile(
		"invlpg (%0)"
		:
		:"r"(aligned_va)
		: "memory"
	);

	bool isEmpty = true;
	for (uint64_t i = 0; i < 512; i++) {
		if (pt[i]) {
			isEmpty = false;
			break;
		}
	}

	if (isEmpty) {
		VirtualAddress ptVa(reinterpret_cast<uint64_t>(pt));
		this->freePage(ptVa);

		pd[pdIndex] = 0;
	}
}

PhysicalAddress VirtualMemoryManager::toPhysicalAddress(const VirtualAddress addr) {
	const uint64_t va = addr.raw;

	const uint64_t pml4Index = (va >> 39) & 0x1FFULL;
	const uint64_t pdptIndex = (va >> 30) & 0x1FFULL;
	const uint64_t pdIndex   = (va >> 21) & 0x1FFULL;
	const uint64_t ptIndex   = (va >> 12) & 0x1FFULL;
	const uint64_t offset    = va & 0xFFFULL;

	const uint64_t* const pml4 = this->currentPml4.asPtr<uint64_t>();
	const uint64_t* pdpt = nullptr;
	const uint64_t* pd   = nullptr;
	const uint64_t* pt   = nullptr;

	if (pml4[pml4Index] & PAGE_FLAG_P) {
		PhysicalAddress pa(pml4[pml4Index] & 0x000FFFFFFFFFF000ULL);
		pdpt = pa.toVirtualHhdmAddress().asPtr<uint64_t>();
	} else return PhysicalAddress(0);

	if (pdpt[pdptIndex] & PAGE_FLAG_P) {
		PhysicalAddress pa(pdpt[pdptIndex] & 0x000FFFFFFFFFF000ULL);
		pd = pa.toVirtualHhdmAddress().asPtr<uint64_t>();
	} else return PhysicalAddress(0);

	if (pd[pdIndex] & PAGE_FLAG_P) {
		PhysicalAddress pa(pd[pdIndex] & 0x000FFFFFFFFFF000ULL);
		pt = pa.toVirtualHhdmAddress().asPtr<uint64_t>();
	} else return PhysicalAddress(0);

	if (pt[ptIndex] & PAGE_FLAG_P) {
		PhysicalAddress pa((pt[ptIndex] & 0x000FFFFFFFFFF000ULL) + offset);
		return pa;
	} else return PhysicalAddress(0);
}

bool VirtualMemoryManager::allocPage(const VirtualAddress pageAddr, const uint64_t flags) {
	const uint64_t paRaw = reinterpret_cast<uint64_t>(PhysicalMemoryManager::allocPage());

	if (paRaw) {
		this->mapPage(PhysicalAddress(paRaw), pageAddr, flags);
		return true;
	} else 
		return false;
}

void VirtualMemoryManager::freePage(const VirtualAddress pageAddr) {
	PhysicalAddress pa = this->toPhysicalAddress(pageAddr);
	PhysicalMemoryManager::freePage(reinterpret_cast<void*>(pa.raw));
	this->unmapPage(pageAddr);
}