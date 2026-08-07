#include "cxxabi.hpp"
#include "KernelHeap.hpp"
#include "Apic.hpp"
#include "InterruptGuard.hpp"
#include "Process.hpp"
#include "PhysicalMemoryManager.hpp"
#include "VirtualMemoryManager.hpp"

#include <limine.h>

__attribute__((used, aligned(8)))
static volatile struct limine_framebuffer_request framebuffer_request = {
	.id = LIMINE_FRAMEBUFFER_REQUEST_ID,
	.revision = 0,
	.response = nullptr
};

volatile uint32_t* framebuffer_address = nullptr;
uint64_t pitch;

void kernelMainThread();

extern "C" void kmain() {
	call_global_constructors();
	
	PhysicalMemoryManager::init();

	VirtualMemoryManager kernelVmm;

	KernelHeap::init(&kernelVmm);

	uint32_t* fbb = new uint32_t; // For testing KernelHeap
	*fbb = 214;
	delete fbb;

	limine_framebuffer* framebuffer = framebuffer_request.response->framebuffers[0];

	framebuffer_address = (volatile uint32_t*)framebuffer->address;
	pitch = framebuffer->pitch;
	
	const uint64_t width = framebuffer->width;
	const uint64_t height = framebuffer->height;

	for (uint64_t y = 0; y < height; y++) {
		for (uint64_t x = 0; x < width; x++) {
			uint64_t pixel_index = x + (y * (pitch / 4));

			framebuffer_address[pixel_index] = 0x00FFFFFF;
		}
	}

	Process::init();
	apic::init();
	InterruptGuard::setInterrupts();
	
	kernelMainThread();
	
	while (1)
		__asm__ volatile("hlt" : : :);
}
