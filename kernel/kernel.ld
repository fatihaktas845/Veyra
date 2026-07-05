OUTPUT_FORMAT(elf64-x86-64)
ENTRY(_start)

SECTIONS
{
    . = 0xFFFFFFFF80000000;

    .text : ALIGN(0x1000)
    {
        *(.text .text.*)
    }

    .rodata : ALIGN(0x1000)
    {
        *(.rodata .rodata.*)
    }

    .data : ALIGN(0x1000)
    {
        *(.data .data.*)
    }

    .bss : ALIGN(0x1000)
    {
        *(COMMON)
        *(.bss .bss.*)
    }

	.stack (NOLOAD) : ALIGN(0x1000) {
		__stack_bottom = .;
		KEEP(*(.stack))
		KEEP(*(.stack.*))
		. += 32K;
		__stack_top = .;
	}
}
