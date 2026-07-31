#pragma once

#include <stdint.h>

class InterruptGuard {
private:
    uint64_t rflags = 0;

    void loadRflags();
    void setRflags(const uint64_t rflags);

public:
    InterruptGuard();
    ~InterruptGuard();

    InterruptGuard(const InterruptGuard&) = delete;
    InterruptGuard& operator=(const InterruptGuard&) = delete;

    InterruptGuard(InterruptGuard&&) = delete;
    InterruptGuard& operator=(InterruptGuard&&) = delete;

    static void setInterrupts();
    static void clearInterrupts();
};