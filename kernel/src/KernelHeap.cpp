#include "KernelHeap.hpp"

void KernelHeap::init() {
    expand(4);

    linkedList = reinterpret_cast<Block_t*>(KERNEL_HEAP_START);
    linkedList->size = KERNEL_HEAP_END - KERNEL_HEAP_START - sizeof(Block_t);
    linkedList->isFree = true;
    linkedList->next = nullptr;
}

void KernelHeap::expand(const uint64_t count) {
    for (uint64_t i = 0; i < count; i++) {
        kernelVMM.allocPage(VirtualAddress(KERNEL_HEAP_END), PAGE_FLAG_P | PAGE_FLAG_RW | PAGE_FLAG_XD);
        KERNEL_HEAP_END += 0x1000ULL;
    }
}

VirtualAddress KernelHeap::alloc() {
    // Not Finished!!!

    return VirtualAddress(0);
}

void KernelHeap::free(const VirtualAddress addr) {
    (void)addr.raw;

    // Not Finished!!!
}