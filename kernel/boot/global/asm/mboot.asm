%include "kernel/utility/asm/macros.s"

section .multiboot_header
extern _start

%define MAGIC 0xE85250D6
%define HEADER_LENGTH header_end - header_start


header_start:
align 8 ; Align to 8 bytes

; Multiboot 2 magic number
dd MAGIC

; Architecture
dd 0x0

; The header length
dd HEADER_LENGTH

; The checksum
dd -(MAGIC+HEADER_LENGTH)



; Now for multiboot tags


; The kernel entry address
align 8
dw 3
dw 1
dd 12
dd no_offset(_start)

; We want a 1080x1920x32 framebuffer
align 8
dw 5
dw 0
dd 20
dd 1920
dd 1028
dd 32


; End Tag
align 8
dw 0
dw 0
dd 8


header_end: