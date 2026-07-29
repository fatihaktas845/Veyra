#include "cxxabi.hpp"
#include "KernelHeap.hpp"
#include "Apic.hpp"
#include "Idt.hpp"

#include <limine.h>

__attribute__((used, aligned(8)))
static volatile struct limine_framebuffer_request framebuffer_request = {
	.id = LIMINE_FRAMEBUFFER_REQUEST_ID,
	.revision = 0,
	.response = nullptr
};

extern "C" void kmain() {
	call_global_constructors();

	KernelHeap::init();
	apic::init();
	idt::setInterrupts();

	uint32_t* fbb = new uint32_t; // For testing KernelHeap
	*fbb = 214;
	delete fbb;

	limine_framebuffer* framebuffer = framebuffer_request.response->framebuffers[0];

	volatile uint32_t* framebuffer_address = (volatile uint32_t*)framebuffer->address;
	
	const uint64_t width = framebuffer->width;
	const uint64_t height = framebuffer->height;

	for (uint64_t y = 0; y < height; y++) {
		for (uint64_t x = 0; x < width; x++) {
			uint64_t pixel_index = x + (y * (framebuffer->pitch / 4));

			framebuffer_address[pixel_index] = 0x00FFFFFF;
		}
	}
	
	while (1)
		__asm__ volatile("hlt" : : :);
}
