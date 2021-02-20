[bits 32]

gdt_start:

gdt_null:
    dd 0x0
    dd 0x0

gdt_code:
    dw 0xFFFF       ; Limit
    dw 0x0000       ; Base
    db 0x00         ; Base 2
    db 0b10011010   ; Flags
    db 0b11001111   ; Flags and limit
    db 0x00         ; Base 3

gdt_data:
    dw 0xFFFF       ; Limit
    dw 0x0000       ; Base
    db 0x00         ; Base 2
    db 0b10010010   ; Flags
    db 0b11001111   ; Flags and limit
    db 0x00         ; Base 3

gdt_ptr:
    dw gdt_end - gdt_start - 1
    dd gdt_start    

gdt_end:

code_seg: equ gdt_code - gdt_start
data_seg: equ gdt_data - gdt_start

[GLOBAL load_gdt]
load_gdt:
    cli
    lgdt [gdt_ptr]
    jmp 0x08:.finish_load
.finish_load:
    jmp $