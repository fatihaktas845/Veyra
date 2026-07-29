#pragma once

#include <stdint.h>

#define IA32_APIC_BASE_MSR  0x1B
#define IA32_X2APIC_SIV_MSR 0x80F
#define IA32_X2APIC_EOI_MSR 0x80B

// LAPIC Timer MSRs
#define IA32_X2APIC_LVT_TIMER_MSR  0x832
#define IA32_X2APIC_INIT_COUNT_MSR 0x838
#define IA32_X2APIC_DIV_CONF_MSR   0x83E

namespace msr {
    [[nodiscard]] inline uint64_t read(const uint32_t msr) {
        uint32_t low, high;

        __asm__ volatile(
            "rdmsr"
            : "=a"(low), "=d"(high)
            : "c"(msr)
            : "memory"
        );

        return (static_cast<uint64_t>(high) << 32) | low;
    }

    inline void write(const uint32_t msr, const uint64_t value) {
        const uint32_t low = static_cast<uint32_t>(value);
        const uint32_t high = static_cast<uint32_t>(value >> 32);

        __asm__ volatile(
            "wrmsr"
            :
            : "a"(low), "d"(high), "c"(msr)
            : "memory"
        );
    }
}