#include "bootInfo.h"
#include "psf.h"
#include "memory.h"
#include "utils.h"

static inline void outb(uint16_t port, uint8_t val)
{
    __asm__ volatile ( "outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

char* hex64_to_str(uint64_t hex) {
    static char buf[17];

    for (int i = 15; i >= 0; i--) {
        uint8_t nibble = hex & 0xF;
        buf[i] = (nibble < 10) ? ('0' + nibble) : ('A' + nibble - 10);
        hex >>= 4;
    }

    buf[16] = '\0';
    return buf;
}

void *bootInfo;

extern uint8_t __kernel_heap_start;

void kmain() {
	BootInfo *bi = (BootInfo *)bootInfo;
	
	initPageAllocator();
	KernelGOPInfo *kgi = (KernelGOPInfo *)allocPage();
	memcpy(kgi, bi->kgi, 4096);

	volatile uint32_t *fbb = (uint32_t*)kgi->FrameBufferBase;
	uint32_t pps = kgi->PixelsPerScanLine;

	initPSFv1(fbb, pps);
	printk("Welcome To Veyra!\n", 0x00FFFFFF);

	printk(hex64_to_str((uint64_t)&__kernel_heap_start), 0x00FFFF00);
	/* while (*str) {
		outb(0x3F8, *str);
		str++;
	} */

	/* int a = 1, b = 0;
	int c = a / b; // To test IDT #DE */

	while (1);
}
