[bits 32]
%include "kernel/utility/asm/macros.s"
; Contains basic long mode GDT setup
section .text

; Define global labels
global load_gdt


; Simply loads the GDT
load_gdt:
    lgdt [no_offset(gdt64_ptr)]
    ret


section .rodata ; Read Only Data

; Align to 4K page boundary
align 4096

gdt64_start:

; Null segment
gdt64_null:
    dd 0x00000000
    dd 0x00000000

; Code segment
gdt64_code:
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 0b10011010
    db 0b10101111
    db 0x00

; Data segment
gdt64_data:
    dw 0x0000
    dw 0x0000
    db 0x00
    db 0b10010010
    db 0b10101111
    db 0x00

gdt64_end:

gdt64_ptr:
    dw no_offset(gdt64_end) - no_offset(gdt64_start) - 1
    dd no_offset(gdt64_start)
