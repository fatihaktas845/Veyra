#include "Process.hpp"
#include "Kstd.hpp"
#include "Log.hpp"

#include <stdint.h>

extern volatile uint32_t* framebuffer_address;
extern uint64_t pitch;

uint8_t idleThreadStack[4096];

void idleThread() {
    while (true)
        __asm__ volatile("hlt");
}

void kernelMainThread() {
	Process::create(reinterpret_cast<uint64_t>(idleThreadStack + 4096), reinterpret_cast<uint64_t>(idleThread));
	Log::init();

	Log::print("Welcome to Veyra\n");
	Process::sleep(4000);
	Log::print("Finished!");

    while (true)
        __asm__ volatile("hlt");
}