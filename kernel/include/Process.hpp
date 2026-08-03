#pragma once

#include <stdint.h>

class VirtualMemoryManager;

namespace Process {
    struct ControlBlock {
        uint64_t rsp    = 0;

        uint64_t pid = 0;
        VirtualMemoryManager* vmm = nullptr;
        uint64_t sleepTime = 0;

        ControlBlock* next = nullptr;
    } __attribute__((packed));

    inline ControlBlock* readyQueue = nullptr;
    inline ControlBlock* sleepQueue = nullptr;

    inline ControlBlock* lastBlock = nullptr;
    inline uint64_t lastPid = 0;

    void init();
    void create(const uint64_t rspTop, const uint64_t rip, const bool isUser = false);
    void sleep(const uint64_t ms);
    void scheduler();
}