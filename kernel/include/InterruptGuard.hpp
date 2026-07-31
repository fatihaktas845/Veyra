#pragma once

#include <stdint.h>

class InterruptGuard {
private:
    static uint64_t rflags;

    [[nodiscard]] uint64_t loadRflags();
    void setRflags(const uint64_t rflags);

public:
    InterruptGuard();
    ~InterruptGuard();

    InterruptGuard(const InterruptGuard&) = delete;
    InterruptGuard& operator=(const InterruptGuard&) = delete;

    InterruptGuard(InterruptGuard&&) = delete;
    InterruptGuard& operator=(InterruptGuard&&) = delete;

    void setInterrupts();
    void clearInterrupts();
};