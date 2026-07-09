#include "idt.hpp"

void idt::set_idt_entry(uint8_t index, uint64_t offset, uint8_t ist, uint8_t type_attribute) {
	idt_entries[index].offset_1 = (uint16_t)(offset & 0xFFFF);
	idt_entries[index].segment_selector = (uint16_t)0x08;
	idt_entries[index].ist = 0;
	idt_entries[index].type_attribute = type_attribute;
	idt_entries[index].offset_2 = (uint16_t)((offset >> 16) & 0xFFFF);
	idt_entries[index].offset_3 = (uint32_t)((offset >> 32) & 0xFFFFFFFF);
	idt_entries[index].reserved = 0;
}

void idt::init_idt() {
	set_idt_entry(0, (uint64_t)divide_error, 0, 0x8F);
	set_idt_entry(13, (uint64_t)general_protection, 0, 0x8F);
	set_idt_entry(14, (uint64_t)page_fault, 0, 0x8F);

	idtr.size = sizeof(idt_entries) - 1;
	idtr.offset = (uint64_t)idt_entries;
}

void idt::divide_error() {
	__asm__ volatile("movq $1, %%rax": : : "rax");

	while (1)
		__asm__ volatile("hlt");
}

void idt::general_protection() {
	__asm__ volatile("movq $14, %%rax" : : : "rax");

	while (1)
		__asm__ volatile("hlt");
}

void idt::page_fault() {
	__asm__ volatile("movq $15, %%rax" : : : "rax");

	while (1)
		__asm__ volatile("hlt");
}
