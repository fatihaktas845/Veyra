#include <stdint.h>

extern volatile uint32_t* framebuffer_address;
extern uint64_t pitch;

void kernelMainThread() {
    uint64_t width = 200;
    uint64_t height = 200;

	for (uint64_t y = 0; y < height; y++) {
		for (uint64_t x = 0; x < width; x++) {
			uint64_t pixel_index = x + (y * (pitch / 4));

			framebuffer_address[pixel_index] = 0x0000FF00;
		}
	}

    while (true)
        __asm__ volatile("hlt");
}