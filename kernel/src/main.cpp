#include "PhysicalMemoryManager.hpp"
#include "cxxabi.hpp"

#include <limine.h>

__attribute__((used, aligned(8)))
static volatile struct limine_framebuffer_request framebuffer_request = {
	.id = LIMINE_FRAMEBUFFER_REQUEST_ID,
	.revision = 4,
	.response = nullptr
};

extern "C" void kmain() {
	call_global_constructors();

	limine_framebuffer* framebuffer = framebuffer_request.response->framebuffers[0];

	PhysicalMemoryManager::init();

	volatile uint32_t* framebuffer_address = (volatile uint32_t*)framebuffer->address;

	for (int y = 0; y < 100; y++) {
		for (int x = 0; x < 100; x++) {
			int pixel_index = x + (y * framebuffer->pitch / 4);

			framebuffer_address[pixel_index] = 0x00FF0000;
		}
	}
	
	while (1)
		__asm__ volatile("hlt" : : :);
}
