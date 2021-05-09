global entry
extern kmain
extern mboot_info
bits 64
%define offset 0xFFFFFFFF80000000
section .text
align 4096
entry:
    ; Yay! Long Mode!

    ; Reset the data segment registers
    mov ax, 0x0
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    xor rdi, rdi
    mov rdi, [mboot_info]
    
    ; Call the kernel
    call kmain

    ; Hang forever
    jmp $
    
    ; Make sure the last instruction is halt, so we do not execute garbabge.
    hlt

section .bss
align 4096
stack_top:
    resb 4096
stack_end: