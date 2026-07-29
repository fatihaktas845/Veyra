#include "PhysicalMemoryManager.hpp"
#include "Hhdm.hpp"

#include <limine.h>

__attribute__((used, aligned(8)))
static volatile struct limine_memmap_request memmap_request = {
	.id = LIMINE_MEMMAP_REQUEST_ID,
	.revision = 4,
	.response = nullptr
};

#define PAGE_SIZE 4096
#define ALIGN_UP(addr)   (((addr) + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1))
#define ALIGN_DOWN(addr) ((addr) & ~(PAGE_SIZE - 1))

static uint64_t bitmap_entry_count = 0;
static uint64_t bitmap_physical = 0;
static uint64_t* bitmap_base = 0;
static uint64_t bitmap_size = 0;
static uint64_t highest = 0;
static uint64_t page_count = 0;

void PhysicalMemoryManager::init() {
	hhdm::init();

	const limine_memmap_response* response = memmap_request.response;

	for (uint64_t i = 0; i < response->entry_count; i++) {
		const struct limine_memmap_entry* entry = response->entries[i];
		if (entry->type == LIMINE_MEMMAP_USABLE && entry->base + entry->length > highest)
			highest = ALIGN_UP(entry->base + entry->length);
	}

	page_count = highest / PAGE_SIZE;
	bitmap_entry_count = (page_count + 63) / 64;
	bitmap_size = bitmap_entry_count * 8;

	for (uint64_t i = 0; i < response->entry_count; i++) {
		const struct limine_memmap_entry* entry = response->entries[i];
		if (entry->type == LIMINE_MEMMAP_USABLE) {
			uint64_t aligned_base = ALIGN_UP(entry->base);
			uint64_t aligned_end = ALIGN_DOWN(entry->base + entry->length);
			
			if (aligned_end > aligned_base && (aligned_end - aligned_base) >= bitmap_size) {
				bitmap_physical = aligned_base;
				bitmap_base = (uint64_t*)(aligned_base + hhdm::offset);
				break;
			}
		}
	}

	for (uint64_t i = 0; i < bitmap_size / 8; i++) {
		bitmap_base[i] = 0xFFFFFFFFFFFFFFFFULL;
	}

	for (uint64_t i = 0; i < response->entry_count; i++) {
		const limine_memmap_entry* entry = response->entries[i];

		if (entry->type == LIMINE_MEMMAP_USABLE) {
			uint64_t aligned_base = ALIGN_UP(entry->base);
			uint64_t aligned_end = ALIGN_DOWN(entry->base + entry->length);

			if (aligned_end <= aligned_base)
				continue;

			uint64_t first_page = aligned_base / PAGE_SIZE;
			uint64_t free_page_count = (aligned_end - aligned_base) / PAGE_SIZE;

			for (uint64_t j = first_page; j < first_page + free_page_count; j++) {
				bitmap_base[j / 64] &= ~(1ULL << (j % 64));
			}
		}
	}

	const uint64_t bitmap_first_page = bitmap_physical / PAGE_SIZE;
	const uint64_t bitmap_physical_end = ALIGN_UP(bitmap_physical + bitmap_size);
	const uint64_t bitmap_page_count = (bitmap_physical_end - bitmap_physical) / PAGE_SIZE;

	for (uint64_t i = bitmap_first_page; i < bitmap_first_page + bitmap_page_count; i++)
		bitmap_base[i / 64] |= (1ULL << (i % 64));
}

void* PhysicalMemoryManager::allocPage() {
	for (uint64_t i = 0; i < bitmap_entry_count; i++) {
		if (bitmap_base[i] == 0xFFFFFFFFFFFFFFFFULL)
			continue;

		uint64_t free_bit_index = __builtin_ctzll(~bitmap_base[i]);
		bitmap_base[i] |= (1ULL << free_bit_index);

		return (void*)(4096 * (i * 64 + free_bit_index));
	}

	return 0;
}

void PhysicalMemoryManager::freePage(void* physicalAddress) {
	const uint64_t aligned_physical_address = ALIGN_DOWN((uint64_t)physicalAddress);
	const uint64_t page_number = aligned_physical_address / PAGE_SIZE;

	bitmap_base[page_number / 64] &= ~(1ULL << (page_number % 64));
}