#pragma once

#include <stdint.h>

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
        const uint32_t hight = static_cast<uint32_t>(value >> 32);

        __asm__ volatile(
            "wrmsr"
            :
            : "a"(low), "d"(hight), "c"(msr)
            : "memory"
        );
    }
}