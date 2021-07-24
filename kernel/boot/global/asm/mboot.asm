%include "kernel/utility/asm/macros.s"

section .multiboot_header
extern _start

%define MAGIC 0xE85250D6
%define HEADER_LENGTH header_end - header_start


header_start:
align 8 ; Align to 8 bytes

; Multiboot 2 magic number
dd MAGIC


; The header length
dd HEADER_LENGTH

; The checksum
dd -(MAGIC+HEADER_LENGTH)

; Now for multiboot tags


; The kernel entry address
dw 3
dw 1
dd 12
dd _start - offset


; End Tag
dw 0
dw 0
dd 8


header_end: