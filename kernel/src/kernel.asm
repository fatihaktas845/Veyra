[BITS 64]
DEFAULT ABS

global _start

extern __stack_top
extern _ZN3idt4idtrE
extern _ZN3idt8init_idtEv
extern kmain

section .bss
	double_fault_stack_bottom:
		resb 4096
	double_fault_stack_top:

section .data
	align 16
	tss_start:
		dd 0x0                    ; Reserved
		dq __stack_top            ; RSP0
		dq 0x0                    ; RSP1
		dq 0x0                    ; RSP2
		dq 0x0                    ; Reserved
		dq double_fault_stack_top ; IST1
		dq 0x0                    ; IST2
		dq 0x0                    ; IST3
		dq 0x0                    ; IST4
		dq 0x0                    ; IST5
		dq 0x0                    ; IST6
		dq 0x0                    ; IST7
		dq 0x0                    ; Reserved
		dw 0x0                    ; Reserved
		dw 0x68                   ; IOPB
	tss_end:

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

		dq 0x0 ; User Code Descriptor
		dq 0x0 ; User Data Descriptor

		dw ((tss_end - tss_start - 1) & 0xFFFF) ; TSS Descriptor
		dw (tss_start & 0xFFFF)
		db ((tss_start >> 16) & 0xFF)
		db 0b10001001
		db ((0b0010 << 4) | (((tss_end - tss_start - 1) >> 16) & 0x0F))
		db ((tss_start >> 24) & 0xFF)
		dd (tss_start >> 32) & 0xFFFFFFFF
		dd 0x0
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

		call _ZN3idt8init_idtEv
		lidt [_ZN3idt4idtrE]
		
		jmp kmain

		hlt