global entry
extern kmain
section .text
bits 64
entry:
    ; Yay! Long Mode!

    ; Reset the data segment registers
    mov ax, 0x0
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Call the kernel
    call kmain

    ; Make sure the last instruction is halt, so we do not execute garbabge.
    hlt