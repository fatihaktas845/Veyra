#include "Process.hpp"

void kernelMainThread();

void Process::init() {
    if (list)
        return;
    
    ControlBlock* newBlock = new ControlBlock;
    list = newBlock;
}