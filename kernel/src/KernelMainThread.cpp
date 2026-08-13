#include "Process.hpp"
#include "Kstd.hpp"
#include "Log.hpp"

#include <stdint.h>

void idleThread() {
    while (true)
        __asm__ volatile("hlt");
}

void kernelMainThread() {
	Process::create(reinterpret_cast<uint64_t>(idleThread));
	Log::init();

	Log::print("Welcome to Veyra\n");
	Process::sleep(4000);
	Log::print("Finished!", 0x00FF00FF);

    while (true)
        __asm__ volatile("hlt");
}