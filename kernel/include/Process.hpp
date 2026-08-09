#pragma once

#include <stdint.h>

class VirtualMemoryManager;

namespace Process {
    struct ControlBlock {
        uint64_t rsp    = 0;

        VirtualMemoryManager* vmm = nullptr;
        uint64_t cr3 = 0;
        uint64_t pid = 0;
        uint64_t sleepTime = 0;

        ControlBlock* readyPrev = nullptr;
        ControlBlock* readyNext = nullptr;
        ControlBlock* sleepPrev = nullptr;
        ControlBlock* sleepNext = nullptr;
    } __attribute__((packed));

    extern ControlBlock* readyQueue;
    extern ControlBlock* sleepQueue;

    extern ControlBlock* lastReadyBlock;
    extern ControlBlock* lastSleepBlock;
    extern uint64_t lastPid;

    void init();
    void create(const uint64_t rspTop, const uint64_t rip, const bool isUser = false);
    void sleep(const uint64_t ms);
    void scheduler();
}