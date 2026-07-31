#pragma once

#include <stdint.h>

namespace idt {
	struct entry {
		uint16_t offset_1;
		uint16_t segment_selector;
		uint8_t  ist;
		uint8_t  type_attribute;
		uint16_t offset_2;
		uint32_t offset_3;
		uint32_t reserved;
	} __attribute__((packed));
	
	struct descriptor {
		uint16_t size;
		uint64_t offset;
	} __attribute__((packed));

	void setIdtEntry(uint8_t index, uint64_t offset, uint8_t ist, uint8_t type_attribute);

	inline uint64_t rflags = 0;

	inline void clearInterrupts() {
		__asm__ volatile(
			"pushfq\n\t"
			"pop %0"
			: "=r"(rflags)
			:
			: "memory"
		);
	}

	inline void setInterrupts() {
		__asm__ volatile(
			"sti": : : "memory"
		);
	}
}
