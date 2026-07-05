#include "pmm.h"

#include <limine.h>

__attribute__((used, aligned(8)))
static volatile struct limine_hhdm_request hhdm_request = {
	.id = LIMINE_HHDM_REQUEST_ID,
	.revision = 4
};

__attribute__((used, aligned(8)))
static volatile struct limine_framebuffer_request framebuffer_request = {
	.id = LIMINE_FRAMEBUFFER_REQUEST_ID,
	.revision = 4
};

__attribute__((used, aligned(8)))
static volatile struct limine_memmap_request memmap_request = {
	.id = LIMINE_MEMMAP_REQUEST_ID,
	.revision = 4
};

void kmain() {
	const uint64_t hhdm_offset = hhdm_request.response->offset;
	struct limine_framebuffer* framebuffer = framebuffer_request.response->framebuffers[0];

	pmm_init(memmap_request.response, hhdm_offset);

	volatile uint32_t* framebuffer_address = (uint32_t*)framebuffer->address;

	for (int y = 0; y < 100; y++) {
		for (int x = 0; x < 100; x++) {
			int pixel_index = x + (y * framebuffer->pitch / 4);

			framebuffer_address[pixel_index] = 0x00FF0000;
		}
	}
	
	__asm__ volatile("hlt" : : :);
}
