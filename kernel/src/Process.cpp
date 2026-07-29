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

    kernelProcess->rsp = reinterpret_cast<uint64_t>(kernel_stack_top) - 15ULL * 8ULL;

    list = kernelProcess;
    currentProcessControlBlock = kernelProcess;

    __asm__ volatile (
        "mov %0, %%rsp\n\t"
        "jmp *%1\n\t"
        : 
        : "r"(kernelProcess->rsp), "r"(kernelMainThread)
        : "memory"
    );
}