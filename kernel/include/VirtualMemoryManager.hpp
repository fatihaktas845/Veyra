#pragma once

#include "address.hpp"

#include <cstdint>

#define VMM_PAGE_FLAG_P   (1ULL << 0)
#define VMM_PAGE_FLAG_RW  (1ULL << 1)

class VirtualMemoryManager {
	private:
		static VirtualAddress kernelPML4;
		VirtualAddress currentPML4;
	
	public:
		VirtualMemoryManager();
		VirtualMemoryManager(const VirtualAddress& va);

		// TODO: Not Finished!!!
};
