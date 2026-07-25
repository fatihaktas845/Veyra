#pragma once

#include <stdint.h>

namespace gdt {
    struct GDTR {
        uint16_t size;
        uint64_t offset;
    } __attribute__((packed));

    struct SegmentDescriptor {
        uint16_t limitLow;
        uint16_t baseLow;
        uint8_t baseMiddle;
        uint8_t access;
        uint8_t flagsAndLimitHigh;
        uint8_t baseHigh;
    } __attribute__((packed));

    struct SystemSegmentDescriptor {
        uint16_t limitLow;
        uint16_t baseLow;
        uint8_t baseMiddle0;
        uint8_t access;
        uint8_t flagsAndLimitHigh;
        uint8_t baseMiddle1;
        uint32_t baseHigh;
        uint32_t reserved = 0;
    } __attribute__((packed));

    struct TSS {
        uint32_t reserved0 = 0;
        uint64_t rsp[3] = {};
        uint64_t reserved1 = 0;
        uint64_t ist[7] = {};
        uint64_t reserved2 = 0;
        uint16_t reserved3 = 0;
        uint16_t iopb = 104;
    } __attribute__((packed));

    void setEntry(uint64_t index, uint8_t access, uint8_t flags);
    void setSystemEntry(uint64_t index, uint64_t base, uint32_t limit, uint8_t access, uint8_t flags);
}