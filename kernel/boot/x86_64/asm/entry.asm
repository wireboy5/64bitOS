[bits 64]
%include "kernel/utility/asm/macros.s"
; Entry point into long mode

; Global labels
global entry64

entry64:
    ; Reset the data segment registers
    mov ax, 0x0
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax



    jmp $