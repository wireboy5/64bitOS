section .multiboot_header
header_start:
    align 8
    ; Multiboot 2 magic number
    dd 0xE85250D6
    dd 0x0
    ; Header length
    dd header_end - header_start

    ; Checksum
    dd -(0xE85250D6 + 0x0 + (header_end - header_start))

    ; End tag
    dw 0
    dw 0
    dd 8
header_end:
