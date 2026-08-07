#pragma once

#include "Address.hpp"

#include <stdint.h>

#define PAGE_FLAG_P   (1ULL << 0)
#define PAGE_FLAG_RW  (1ULL << 1)
#define PAGE_FLAG_US  (1ULL << 2)
#define PAGE_FLAG_PWT (1ULL << 3)
#define PAGE_FLAG_PCD (1ULL << 4)
#define PAGE_FLAG_XD  (1ULL << 63)

class VirtualMemoryManager {
	private:
		VirtualAddress currentPml4;
	
	public:
		VirtualMemoryManager();

		void setPml4(const VirtualAddress pml4);
		[[nodiscard]] VirtualAddress getPml4() const;

		void mapPage(const PhysicalAddress pa, const VirtualAddress va, const uint64_t flags);
		void unmapPage(const VirtualAddress va);

		PhysicalAddress toPhysicalAddress(const VirtualAddress addr);

		bool allocPage(const VirtualAddress pageAddr, const uint64_t flags);
		void freePage(const VirtualAddress pageAddr);

		void loadCr3();
		[[nodiscard]] static uint64_t getCr3();
};