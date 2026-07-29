#pragma once

#include <stdint.h>

namespace Process {
    struct ControlBlock {
        // iretq
        uint64_t rip    = 0;
        uint64_t cs     = 0;
        uint64_t rflags = 0;
        uint64_t rsp    = 0;
        uint64_t ss     = 0;

        // Registers
        uint64_t rax = 0;
        uint64_t rbx = 0;
        uint64_t rcx = 0;
        uint64_t rdx = 0;
        uint64_t rdi = 0;
        uint64_t rsi = 0;
        uint64_t rbp = 0;
        uint64_t r8  = 0;
        uint64_t r9  = 0;
        uint64_t r10 = 0;
        uint64_t r11 = 0;
        uint64_t r12 = 0;
        uint64_t r13 = 0;
        uint64_t r14 = 0;
        uint64_t r15 = 0;

        uint64_t pid = 0;
        uint64_t cr3 = 0;

        ControlBlock* next = nullptr;
    } __attribute__((packed));

    inline ControlBlock* list = nullptr;
}