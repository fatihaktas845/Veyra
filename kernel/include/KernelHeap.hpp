#pragma once

#include "VirtualMemoryManager.hpp"

namespace KernelHeap {
    uint64_t KERNEL_HEAP_START = 0xFFFF'9600'0000'0000ULL;
    uint64_t KERNEL_HEAP_END = KERNEL_HEAP_START;
    uint64_t KERNEL_HEAP_PML4_INDEX = 300;
    VirtualMemoryManager kernelVMM;

    struct BlockHeader {
        uint64_t size = 0;
        bool isFree = true;
        BlockHeader* next = nullptr;
    };

    BlockHeader* linkedList = nullptr;
    
    void init();
    void expand(const uint64_t count);

    VirtualAddress alloc(const uint64_t size);
    void free(const VirtualAddress addr);
}