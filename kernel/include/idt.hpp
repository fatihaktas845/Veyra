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

	entry idt_entries[256] = {0};
	descriptor idtr;

	void set_idt_entry(uint8_t index, uint64_t offset, uint8_t ist, uint8_t type_attribute);
	void init_idt();

	void divide_error();
	void general_protection();
	void page_fault();
}
