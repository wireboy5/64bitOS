bits 64
global load_cr3

load_cr3:
    mov cr3, rdi
    ret