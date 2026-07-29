#include "Process.hpp"
#include "VirtualMemoryManager.hpp"

extern VirtualMemoryManager kernelVMM;

void kernelMainThread();

extern uint64_t kernel_stack_top[];

void Process::init() {
    if (list)
        return;
    
    ControlBlock* kernelProcess = new ControlBlock;
    kernelProcess->vmm = &kernelVMM;
    kernelProcess->rip = reinterpret_cast<uint64_t>(&kernelMainThread);
    kernelProcess->cs  = 0x08ULL;

    __asm__ volatile(
        "pushfq\n"
        "pop %0\n"
        : "=r"(kernelProcess->rflags)
        :
        : "memory"
    );

    kernelProcess->rsp = reinterpret_cast<uint64_t>(kernel_stack_top);
    kernelProcess->ss  = 0x10ULL;

    list = kernelProcess;
}