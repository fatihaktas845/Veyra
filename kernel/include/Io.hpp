#pragma once

#include <stdint.h>

namespace io {
    inline void outb(const uint16_t port, const uint8_t value) {
        __asm__ volatile(
            "outb %0, %1"
            :
            : "a"(value), "Nd"(port)
            : "memory"
        );
    }

    [[nodiscard]] inline uint8_t inb(const uint16_t port) {
        uint8_t value;
        __asm__ volatile(
            "inb %1, %0"
            : "=a"(value)
            : "Nd"(port)
            : "memory"
        );
    }
}