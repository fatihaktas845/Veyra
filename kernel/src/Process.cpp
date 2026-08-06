#include "Process.hpp"
#include "VirtualMemoryManager.hpp"
#include "InterruptGuard.hpp"
#include "Msr.hpp"
#include "Kstring.hpp"

Process::ControlBlock* currentProcessControlBlock;

extern VirtualMemoryManager kernelVMM;
extern uint64_t kernel_stack_top[];
extern uint64_t timerInterruptCounter;

void kernelMainThread();

void Process::init() {
    if (readyQueue)
        return;
    
    ControlBlock* kernelProcess = new ControlBlock;
    kernelProcess->vmm = &kernelVMM;
    kernelProcess->rsp = reinterpret_cast<uint64_t>(kernel_stack_top);
    kernelProcess->pid = lastPid++;

    readyQueue = kernelProcess;
    currentProcessControlBlock = kernelProcess;
    lastReadyBlock = kernelProcess;
}

void Process::create(const uint64_t rspTop, const uint64_t rip, const bool isUser) {
    [[maybe_unused]] InterruptGuard interruptGuard;

    ControlBlock* newBlock = new ControlBlock;

    uint64_t* pst = reinterpret_cast<uint64_t*>(rspTop);
    *(--pst) = isUser ? 0x1BULL : 0x10ULL; // SS
    *(--pst) = rspTop; // RSP
    uint64_t rflags;
    __asm__ volatile(
        "pushfq\n\t"
        "popq %0\n\t"
        : "=r"(rflags)
        :
        : "memory"
    );
    rflags |= 1ULL << 9;
    *(--pst) = rflags; // RFLAGS
    *(--pst) = isUser ? 0x23ULL : 0x08ULL; // CS
    *(--pst) = rip; // RIP

    for (uint64_t i = 0; i < 15; i++)
        *(--pst) = 0ULL;
    
    newBlock->rsp = reinterpret_cast<uint64_t>(pst);
    newBlock->pid = lastPid++;

    lastReadyBlock->readyNext = newBlock;
    newBlock->readyPrev = lastReadyBlock;
    newBlock->readyNext = nullptr;
    lastReadyBlock = newBlock;
}

void Process::sleep(const uint64_t ms) {
    [[maybe_unused]] InterruptGuard interruptGuard;

    currentProcessControlBlock->sleepTime = timerInterruptCounter + ms;

    if (currentProcessControlBlock->readyPrev)
        currentProcessControlBlock->readyPrev->readyNext = currentProcessControlBlock->readyNext;
    else
        readyQueue = currentProcessControlBlock->readyNext;
    
    if (currentProcessControlBlock->readyNext)
        currentProcessControlBlock->readyNext->readyPrev = currentProcessControlBlock->readyPrev;
    else
        lastReadyBlock = currentProcessControlBlock->readyPrev;
    
    if (sleepQueue) {
        lastSleepBlock->sleepNext = currentProcessControlBlock;
        currentProcessControlBlock->sleepPrev = lastSleepBlock;
        lastSleepBlock = currentProcessControlBlock;
    } else
        sleepQueue = lastSleepBlock = currentProcessControlBlock;
    
    scheduler();
}

void Process::scheduler() {
    [[maybe_unused]] InterruptGuard interruptGuard;

    msr::write(IA32_X2APIC_SELF_IPI_MSR, 0x20);
}