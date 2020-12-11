; Specify 32 bit instructions
[BITS 32]

; Make sure that the multiboot section will be accessable from out C++ kernel
[GLOBAL multiboot]

[EXTERN code] ; This is the start of the text section
[EXTERN bss] ; This is the start of the bss section
[EXTERN end] ; This is the end of our kernel

; Now for some macro's
MULTIBOOT_MAGIC equ 0x1BADB002 ; Multiboot magic number

MULTIBOOT_FLAGS equ 1 | 2 ; The multiboot flags

MULTIBOOT_CHECKSUM equ -(MULTIBOOT_FLAGS + MULTIBOOT_MAGIC) ; The checksum
; This is equal to 0-(FLAGS+MAGIC) or (In our case)
; -0x1BADB005

; Define a section for multiboot
multiboot:
    ; Define a Double Word of the multiboot magic
    dd MULTIBOOT_MAGIC

    ; Define the multiboot flags
    dd MULTIBOOT_FLAGS

    ; And define the multiboot checksum
    dd MULTIBOOT_CHECKSUM

    ; We also need to define the location of this section
    dd multiboot

    ; The start of the text section
    dd code

    ; The start of the bss section
    dd bss
    
    ; And the end of our kernel
    dd end

    ; As well as the start of our kernel
    dd start




; We need to make sure we can access the kernel entry point globaly
[GLOBAL start]

; Make sure we can access our kernel's kernel_main cpp function
[EXTERN kernel_main]

; Here is the kernel entry point
start:
    ; Disable interrupts
    cli

    ; Call the kernel_main function
    call kernel_main

    ; And here we loop infinitely
    jmp $