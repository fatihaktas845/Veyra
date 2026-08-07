#include "KernelHeap.hpp"
#include "PhysicalMemoryManager.hpp"
#include "InterruptGuard.hpp"
#include "VirtualMemoryManager.hpp"

namespace {
    uint64_t KERNEL_HEAP_START = 0xFFFF'9600'0000'0000ULL;
    uint64_t KERNEL_HEAP_END = KERNEL_HEAP_START;
    // uint64_t KERNEL_HEAP_PML4_INDEX = 300;

    KernelHeap::BlockHeader* linkedList = nullptr;
}

void KernelHeap::init(VirtualMemoryManager* vmm) {
    kernelVmm = vmm;

    expand(4);

    linkedList = reinterpret_cast<BlockHeader*>(KERNEL_HEAP_START);
    linkedList->size = KERNEL_HEAP_END - KERNEL_HEAP_START - sizeof(BlockHeader);
    linkedList->isFree = true;
    linkedList->next = nullptr;
}

bool KernelHeap::expand(const uint64_t pageCount) {
    if (!pageCount)
        return false;
    
    const uint64_t oldEnd = KERNEL_HEAP_END;
    bool result = true;

    for (uint64_t i = 0; i < pageCount; i++) {
        result = kernelVmm->allocPage(VirtualAddress(KERNEL_HEAP_END), PAGE_FLAG_P | PAGE_FLAG_RW | PAGE_FLAG_XD);
        
        if (result)
            KERNEL_HEAP_END += 0x1000ULL;
        else {
            while (KERNEL_HEAP_END > oldEnd) {
                KERNEL_HEAP_END -= 0x1000ULL;
                kernelVmm->freePage(VirtualAddress(KERNEL_HEAP_END));
            }

            break;
        }
    }

    return result;
}

VirtualAddress KernelHeap::alloc(const uint64_t size) {
    [[maybe_unused]] InterruptGuard interruptGuard;

    if (size == 0)
        return VirtualAddress(0);

    const uint64_t alignedSize = (size + 7) & ~7;
    BlockHeader* current = linkedList;

    while (current) {
        if (current->isFree && current->size >= alignedSize) {
            const uint64_t minSplitSize = sizeof(BlockHeader) + 8;

            if (current->size >= alignedSize + minSplitSize) {
                const uint64_t addr = reinterpret_cast<uint64_t>(current) + sizeof(BlockHeader);

                BlockHeader* newBlock = reinterpret_cast<BlockHeader*>(addr + alignedSize);
                newBlock->isFree = true;
                newBlock->size = current->size - alignedSize - sizeof(BlockHeader);
                newBlock->next = current->next;

                current->size = alignedSize;
                current->next = newBlock;
            }

            current->isFree = false;

            const uint64_t allocatedAddr = reinterpret_cast<uint64_t>(current) + sizeof(BlockHeader);

            return VirtualAddress(allocatedAddr);
        }

        current = current->next;
    }

    const uint64_t oldEnd = KERNEL_HEAP_END;
    const uint64_t newPageCount = (alignedSize + sizeof(BlockHeader) + 4095) / 4096;
    bool isExpanded = expand(newPageCount);

    if (!isExpanded)
        return VirtualAddress(0);

    current = linkedList;

    while (current) {
        uint64_t blockEndAddr = reinterpret_cast<uint64_t>(current) + sizeof(BlockHeader) + current->size;
        if (blockEndAddr == oldEnd) {
            BlockHeader* lastBlock = current;

            if (lastBlock->isFree)
                lastBlock->size += newPageCount * 0x1000ULL;
            else {
                BlockHeader* newBlock = reinterpret_cast<BlockHeader*>(oldEnd);
                newBlock->isFree = true;
                newBlock->size = newPageCount * 0x1000ULL - sizeof(BlockHeader);
                newBlock->next = nullptr;

                lastBlock->next = newBlock;
            }

            break;
        }

        current = current->next;
    }

    return alloc(size);
}

void KernelHeap::free(const VirtualAddress addr) {
    [[maybe_unused]] InterruptGuard interruptGuard;
    
    if (addr.raw == 0)
        return;

    BlockHeader* block = reinterpret_cast<BlockHeader*>(addr.raw - sizeof(BlockHeader));
    block->isFree = true;

    // TODO: Add coalescing later!!!
}