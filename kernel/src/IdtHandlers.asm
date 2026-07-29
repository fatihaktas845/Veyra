[BITS 64]
DEFAULT ABS

%include "Macros.asm"

extern divideError
extern timerInterrupt
extern currentProcessControlBlock

global timerInterruptAsm

section .text
    timerInterruptAsm:
        pushall

        mov rax, [currentProcessControlBlock]
        mov [rax], rsp

        call timerInterrupt

        

        popall
        iretq