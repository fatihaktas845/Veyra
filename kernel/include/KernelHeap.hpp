#pragma once

#include "VirtualMemoryManager.hpp"

#include <stdint.h>

class VirtualMemoryManager;

namespace KernelHeap {
    constexpr uint64_t KERNEL_HEAP_START = 0xFFFF'9600'0000'0000ULL;
    constinit uint64_t KERNEL_HEAP_END = KERNEL_HEAP_START;
    constexpr uint64_t KERNEL_HEAP_PML4_INDEX = 300;
    VirtualMemoryManager kernelVMM;

    struct Block_t {
        uint64_t size = 0;
        bool isFree = true;
        Block_t* next = nullptr;
    };

    Block_t* linkedList = nullptr;
    
    void init();
    void expand(const uint64_t count);

    VirtualAddress alloc();
    void free(const VirtualAddress addr);
}