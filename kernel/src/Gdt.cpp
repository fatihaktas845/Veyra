#include "Gdt.hpp"

uint64_t gdtArray[7] = {};
Gdt::GDTR gdtr;
Gdt::TSS globalTss;

extern "C" uint64_t kernel_stack_top[];
extern "C" uint64_t double_fault_stack_top[];

extern "C" void initGdt() {
    globalTss.rsp[0] = reinterpret_cast<uint64_t>(kernel_stack_top);
    globalTss.ist[0] = reinterpret_cast<uint64_t>(double_fault_stack_top);

    uint64_t base = reinterpret_cast<uint64_t>(&globalTss);
    uint64_t limit = sizeof(Gdt::TSS) - 1;

    Gdt::setEntry(1, 0x9B, 0xA); // Kernel-Code Segment - 0x08
    Gdt::setEntry(2, 0x93, 0xA); // Kernel-Data Segment - 0x10
    Gdt::setEntry(3, 0xF3, 0xA); // User-Data Segment - 0x1B
    Gdt::setEntry(4, 0xFB, 0xA); // User-Code Segment - 0x23
    Gdt::setSystemEntry(5, base, limit, 0x89, 0xA); // Kernel-Task State Segment - 0x28

    gdtr.size = sizeof(gdtArray) - 1;
    gdtr.offset = reinterpret_cast<uint64_t>(&gdtArray);
}

void Gdt::setEntry(uint64_t index, uint8_t access, uint8_t flags) {
    SegmentDescriptor sd;

    sd.limitLow = 0xFFFF;
    sd.baseLow = 0;
    sd.baseMiddle = 0;
    sd.access = access;
    sd.flagsAndLimitHigh = (flags << 4) | 0xF;
    sd.baseHigh = 0;

    gdtArray[index] = *reinterpret_cast<uint64_t*>(&sd);
} 

void Gdt::setSystemEntry(uint64_t index, uint64_t base, uint32_t limit, uint8_t access, uint8_t flags) {
    SystemSegmentDescriptor ssd;

    ssd.limitLow = limit & 0xFFFF;
    ssd.baseLow = base & 0xFFFF;
    ssd.baseMiddle0 = (base >> 16) & 0xFF;
    ssd.access = access;
    ssd.flagsAndLimitHigh = (flags << 4) | ((limit >> 16) & 0xF);
    ssd.baseMiddle1 = (base >> 24) & 0xFF;
    ssd.baseHigh = (base >> 32) & 0xFFFFFFFF;

    uint64_t* addr = reinterpret_cast<uint64_t*>(&ssd);
    gdtArray[index] = addr[0];
    gdtArray[index + 1] = addr[1];
}