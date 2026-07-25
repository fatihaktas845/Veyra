#include "idt.hpp"

static idt::entry idt_entries[256] = {};
idt::descriptor idtr;

void idt::setIdtEntry(uint8_t index, uint64_t offset, uint8_t ist, uint8_t type_attribute) {
	idt_entries[index].offset_1 = (uint16_t)(offset & 0xFFFF);
	idt_entries[index].segment_selector = (uint16_t)0x08;
	idt_entries[index].ist = ist;
	idt_entries[index].type_attribute = type_attribute;
	idt_entries[index].offset_2 = (uint16_t)((offset >> 16) & 0xFFFF);
	idt_entries[index].offset_3 = (uint32_t)((offset >> 32) & 0xFFFFFFFF);
	idt_entries[index].reserved = 0;
}

extern "C" void initIdt() {
	idt::setIdtEntry(0, (uint64_t)idt::divide_error, 0, 0x8F);
	idt::setIdtEntry(13, (uint64_t)idt::general_protection, 0, 0x8F);
	idt::setIdtEntry(14, (uint64_t)idt::page_fault, 0, 0x8F);

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

__attribute__((interrupt))
void idt::page_fault(interrupt_frame* frame, uint64_t errorCode) {
	(void)frame;
	(void)errorCode;
	
	__asm__ volatile("movq $15, %%rax" : : : "rax");

	while (1)
		__asm__ volatile("hlt");
}
