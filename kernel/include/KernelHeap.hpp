#pragma once

#include <cstdint>

namespace KernelHeap {
    static constexpr uint64_t KERNEL_HEAP_START = 0xFFFF'9600'0000'0000ULL;
    static constinit uint64_t KERNEL_HEAP_END = KERNEL_HEAP_START;
    static constexpr uint64_t KERNEL_HEAP_PML4_INDEX = 300;
}