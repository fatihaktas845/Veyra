[BITS 64]

global _start

extern __stack_top
extern _ZN3idt4idtrE
extern _ZN3idt8init_idtEv
extern kmain

section .rodata
	align 16
	gdt_start:
		dq 0x0 ; Null Descriptor

		dw 0xFFFF ; Kernel Code Descriptor
		dw 0x0
		db 0x0
		db 0b10011011
		db 0b10101111
		db 0x0

		dw 0xFFFF ; Kernel Data Descriptor
		dw 0x0
		db 0x0
		db 0b10010011
		db 0b10001111
		db 0x0
	gdt_end:

	align 16
	gdtr:
		dw gdt_end - gdt_start - 1
		dq gdt_start

section .text
	_start:
		cld
		cli

		mov rsp, __stack_top

		xor rbp, rbp

		lgdt [rel gdtr]

		push 0x08
		lea rax, [rel after_cs_reload]
		push rax
		retfq

	after_cs_reload:
		mov ax, 0x10
		mov ds, ax
		mov ss, ax
		mov es, ax

		call _ZN3idt8init_idtEv
		lidt [rel _ZN3idt4idtrE]
		
		jmp kmain

		hlt
