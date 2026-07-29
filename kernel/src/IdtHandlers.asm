[BITS 64]
DEFAULT ABS

%include "Macros.asm"

extern divideError
extern timerInterrupt

global timerInterruptAsm

section .text
    timerInterruptAsm:
        pushall
        call timerInterrupt
        popall
        iretq