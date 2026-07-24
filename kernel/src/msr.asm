[BITS 64]

global readMsr
global writeMsr

section .text

; uint64_t(RAX) readMsr(uint32_t msr(RDI))
readMsr:
    mov ecx, edi
    rdmsr
    
    shl rdx, 32
    or rax, rdx

    ret

; void writeMsr(uint32_t msr(RDI), uint64_t value(RSI))
writeMsr:
    mov ecx, edi

    mov eax, esi
    mov rdx, rsi
    shr rdx, 32
    wrmsr

    ret