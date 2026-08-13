#include "Idt.hpp"
#include "Msr.hpp"
#include "Process.hpp"
#include "Serial.hpp"
#include "Kstd.hpp"
#include "VirtualMemoryManager.hpp"
#include "Gdt.hpp"

static Idt::entry idt_entries[256] = {};
Idt::descriptor idtr;
uint64_t timerInterruptCounter = 0;

extern Process::ControlBlock* currentProcessControlBlock;
extern Gdt::TSS globalTss;

extern "C" {
	void timerInterruptAsm();

	void divideError() {
		__asm__ volatile("movq $1, %%rax": : : "rax");

		while (1)
			__asm__ volatile("hlt");
	}

	void generalProtection() {
		__asm__ volatile("movq $14, %%rax" : : : "rax");

		Serial::write("\nGeneral Protection!!!\n");

		while (1)
			__asm__ volatile("hlt");
	}

	void pageFault() {
		__asm__ volatile("movq $15, %%rax" : : : "rax");

		Serial::write("\nPage Fault!!!\n");

		while (1)
			__asm__ volatile("hlt");
	}

	void doubleFault() {
		__asm__ volatile("movq $15, %%rax" : : : "rax");

		Serial::write("\nDouble Fault!!!\n");

		while (1)
			__asm__ volatile("hlt");
	}

	void timerInterrupt() {
		timerInterruptCounter++;

		Process::ControlBlock* currentSleep = Process::sleepQueue;

		while (currentSleep) {
			if (timerInterruptCounter >= currentSleep->sleepTime) {
				if (currentSleep->sleepPrev)
					currentSleep->sleepPrev->sleepNext = currentSleep->sleepNext;
				else
					Process::sleepQueue = currentSleep->sleepNext;
				
				if (currentSleep->sleepNext)
					currentSleep->sleepNext->sleepPrev = currentSleep->sleepPrev;
				else
					Process::lastSleepBlock = currentSleep->sleepPrev;
				
				if (Process::readyQueue) {
					Process::lastReadyBlock->readyNext = currentSleep;
					currentSleep->readyPrev = Process::lastReadyBlock;
					Process::lastReadyBlock = currentSleep;
				} else
					Process::readyQueue = Process::lastReadyBlock = currentSleep;
			}

			currentSleep = currentProcessControlBlock->sleepNext;
		}

		if (currentProcessControlBlock->readyNext)
			currentProcessControlBlock = currentProcessControlBlock->readyNext;
		else
			currentProcessControlBlock = Process::readyQueue;
		
		globalTss.rsp[0] = currentProcessControlBlock->syscallRsp;
		
		currentProcessControlBlock->vmm->loadCr3();

		Msr::write(IA32_X2APIC_EOI_MSR, 0);
	}
}

void Idt::setIdtEntry(uint8_t index, uint64_t offset, uint8_t ist, uint8_t type_attribute) {
	idt_entries[index].offset_1 = (uint16_t)(offset & 0xFFFF);
	idt_entries[index].segment_selector = (uint16_t)0x08;
	idt_entries[index].ist = ist;
	idt_entries[index].type_attribute = type_attribute;
	idt_entries[index].offset_2 = (uint16_t)((offset >> 16) & 0xFFFF);
	idt_entries[index].offset_3 = (uint32_t)((offset >> 32) & 0xFFFFFFFF);
	idt_entries[index].reserved = 0;
}

extern "C" void initIdt() {
	Idt::setIdtEntry(0, reinterpret_cast<uint64_t>(divideError), 0, 0x8F);
	Idt::setIdtEntry(13, reinterpret_cast<uint64_t>(generalProtection), 0, 0x8F);
	Idt::setIdtEntry(14, reinterpret_cast<uint64_t>(pageFault), 0, 0x8F);
	Idt::setIdtEntry(8, reinterpret_cast<uint64_t>(doubleFault), 1, 0x8F);

	Idt::setIdtEntry(0x20, reinterpret_cast<uint64_t>(timerInterruptAsm), 0, 0x8E);

	idtr.size = sizeof(idt_entries) - 1;
	idtr.offset = reinterpret_cast<uint64_t>(idt_entries);
}
