global entry
extern kmain
section .text
bits 64
entry:
    ; Call the kernel
    call kmain
    hlt