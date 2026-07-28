[BITS 64]
DEFAULT ABS

%include "macros.asm"

extern divideError
extern timerInterrupt

global divideErrorAsm
global timerInterruptAsm

section .text
    divideErrorAsm:
        pushall
        call divideError
        popall
        iretq
    
    timerInterruptAsm:
        pushall
        call timerInterrupt
        popall
        iretq