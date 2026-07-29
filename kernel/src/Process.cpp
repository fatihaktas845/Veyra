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

    list = kernelProcess;
    currentProcessControlBlock = kernelProcess;
}