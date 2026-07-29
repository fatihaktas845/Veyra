#include <stdint.h>

extern uint64_t fbb;

void kernelMainThread() {
    while (true)
        __asm__ volatile("hlt");
}