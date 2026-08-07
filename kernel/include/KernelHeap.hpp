#pragma once

#include "Address.hpp"

class VirtualMemoryManager;

namespace KernelHeap {
    struct BlockHeader {
        uint64_t size = 0;
        bool isFree = true;
        BlockHeader* next = nullptr;
    };
    
    inline VirtualMemoryManager* kernelVmm = nullptr;

    void init(VirtualMemoryManager* vmm);
    bool expand(const uint64_t pageCount);

    VirtualAddress alloc(const uint64_t size);
    void free(const VirtualAddress addr);
}