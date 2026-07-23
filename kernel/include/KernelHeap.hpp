#pragma once

#include "VirtualMemoryManager.hpp"

namespace KernelHeap {
    struct BlockHeader {
        uint64_t size = 0;
        bool isFree = true;
        BlockHeader* next = nullptr;
    };
    
    void init();
    bool expand(const uint64_t pageCount);

    VirtualAddress alloc(const uint64_t size);
    void free(const VirtualAddress addr);
}