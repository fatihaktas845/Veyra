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

    uint64_t width = 200;
    uint64_t height = 200;

	for (uint64_t y = 0; y < height; y++) {
		for (uint64_t x = 0; x < width; x++) {
			uint64_t pixel_index = x + (y * (pitch / 4));

			framebuffer_address[pixel_index] = 0x0000FF00;
		}
	}

	Log::print("Welcome to Veyra\n");
	Process::sleep(4000);
	Log::print("Sümeyye");

    while (true)
        __asm__ volatile("hlt");
}