global entry
extern kmain
section .text
bits 64
entry:
    ; load 0 into all data segment registers
    mov ax, 0
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    ; Call the kernel
    call kmain
    hlt