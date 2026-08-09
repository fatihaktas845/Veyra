#pragma once

#include <stdint.h>

namespace Idt {
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
}
