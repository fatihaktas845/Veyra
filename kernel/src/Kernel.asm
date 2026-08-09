[BITS 64]
DEFAULT ABS

global _start
global double_fault_stack_top

extern kernel_stack_top
extern gdtr
extern initGdt
extern idtr
extern initIdt
extern kmain

section .bss
	resb 4096
	double_fault_stack_top:
	resb 8192
	temp_kernel_stack

section .text
	_start:
		cld
		cli

		mov rsp, temp_kernel_stack

		xor rbp, rbp

		call initGdt
		lgdt [gdtr]

		push 0x08
		lea rax, [after_cs_reload]
		push rax
		retfq

	after_cs_reload:
		mov ax, 0x10
		mov ds, ax
		mov ss, ax
		mov es, ax

		mov ax, 0x28
		ltr ax

		call initIdt
		lidt [idtr]

		mov al, 0xFF
		out 0x21, al
		out 0xA1, al
		
		jmp kmain

		hlt