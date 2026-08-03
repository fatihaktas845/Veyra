#pragma once

#include <stdint.h>

class VirtualMemoryManager;

namespace Process {
    enum class State {
        Running,
        Sleeping,
        Terminated
    };

    struct ControlBlock {
        uint64_t rsp    = 0;

        uint64_t pid = 0;
        VirtualMemoryManager* vmm = nullptr;
        State state = State::Running;

        ControlBlock* next = nullptr;
    } __attribute__((packed));

    inline ControlBlock* list = nullptr;
    inline ControlBlock* lastBlock = nullptr;
    inline uint64_t lastPid = 0;

    void init();
    void create(const uint64_t rspTop, const uint64_t rip, const bool isUser = false);
}