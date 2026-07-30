#include "Process.hpp"
#include "VirtualMemoryManager.hpp"

Process::ControlBlock* currentProcessControlBlock;

extern VirtualMemoryManager kernelVMM;
extern uint64_t kernel_stack_top[];

void kernelMainThread();

void Process::init() {
    if (list)
        return;
    
    ControlBlock* kernelProcess = new ControlBlock;
    kernelProcess->vmm = &kernelVMM;
    kernelProcess->rsp = reinterpret_cast<uint64_t>(kernel_stack_top);
    kernelProcess->pid = lastPid++;

    list = kernelProcess;
    currentProcessControlBlock = kernelProcess;
    lastBlock = kernelProcess;
}

void Process::create(const uint64_t rspTop, const uint64_t rip, const bool isUser = false) {
    ControlBlock* newBlock = new ControlBlock;

    uint64_t* pst = reinterpret_cast<uint64_t*>(rspTop);
    *(--pst) = isUser ? 0x1BULL : 0x10ULL; // SS
    *(--pst) = rspTop; // RSP
    uint64_t rflags;
    __asm__ volatile(
        "pushfq\n\t"
        "pop %0\n\t"
        : "=r"(rflags)
        :
        : "memory"
    );
    rflags |= 1ULL << 9;
    *(--pst) = rflags; // RFLAGS
    *(--pst) = isUser ? 0x23ULL : 0x08ULL;
    *(--pst) = rip;

    for (uint64_t i = 0; i < 15; i++)
        *(--pst) = 0ULL;
    
    newBlock->rsp = reinterpret_cast<uint64_t>(pst);
    newBlock->pid = lastPid++;
    lastBlock->next = newBlock;
    lastBlock = newBlock;
}