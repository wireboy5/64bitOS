%include "kernel/utility/asm/macros.s"
global _start

# The start label
_start:
    mov byte [0xb8000], 'H'
    hlt