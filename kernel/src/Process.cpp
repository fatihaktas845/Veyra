#include "Process.hpp"
#include "VirtualMemoryManager.hpp"

extern VirtualMemoryManager kernelVMM;

void kernelMainThread();

void Process::init() {
    if (list)
        return;
    
    ControlBlock* kernelProcess = new ControlBlock;
    kernelProcess->vmm = &kernelVMM;
    kernelProcess->rip = reinterpret_cast<uint64_t>(&kernelMainThread);
    kernelProcess->cs  = 0x08ULL;
    list = kernelProcess;
}