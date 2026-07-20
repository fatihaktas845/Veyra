#pragma once

#include <cstdint>

namespace KernelHeap {
    constexpr uint64_t KERNEL_HEAP_START = 0xFFFF'9600'0000'0000ULL;
    constexpr uint64_t KERNEL_HEAP_PML4_INDEX = 300;
}