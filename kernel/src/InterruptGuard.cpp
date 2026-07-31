#include "InterruptGuard.hpp"

#define RFLAGS_IF_BIT (1ULL << 9)

InterruptGuard::InterruptGuard() {
    loadRflags();
    clearInterrupts();
}

InterruptGuard::~InterruptGuard() {
    if (this->rflags & RFLAGS_IF_BIT)
        setInterrupts();
}

void InterruptGuard::loadRflags() {
    __asm__ volatile(
        "pushfq\n\t"
        "pop %0\n\t"
        : "=r"(this->rflags)
        :
        : "memory"
    );
}

void InterruptGuard::setRflags(const uint64_t rflags) {
    __asm__ volatile(
        "push %0\n\t"
        "popfq\n\t"
        :
        : "r"(rflags)
        : "memory"
    );

    this->rflags = rflags;
}

void InterruptGuard::clearInterrupts() {
    __asm__ volatile("cli"::: "memory");
}

void InterruptGuard::setInterrupts() {
    __asm__ volatile("sti"::: "memory");
}