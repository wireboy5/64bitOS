section .multiboot_header
extern _start
%define MAGIC 0xE85250D6
%define HEADER_LENGTH header_end - header_start

header_start:
    ; Multiboot 2 magic number
    dd MAGIC
    dd 0x0
    ; Header length
    dd HEADER_LENGTH

    ; Checksum
    dd -(MAGIC+HEADER_LENGTH)


    ; We want a framebuffer
    align 8
    dw 5
    dw 1
    dd 20
    dd 1280 ; 720x1280 24bit color
    dd 720
    dd 24

    align 8
mboot_info_request:
    dw 1
    dw 0
    dd mboot_info_request_end - mboot_info_request
    dd 8
mboot_info_request_end:

    ; Entry address
    align 8
    dw 3
    dw 0
    dd 12
    dd _start
    
    ; End tag
    align 8
    dw 0
    dw 0
    dd 8
header_end:
