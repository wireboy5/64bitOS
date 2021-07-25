[bits 32]
%include "kernel/utility/asm/macros.s"
section .text ; Text Section

; Define global labels
global _start
global multiboot_info

; Define external functions
extern check_multiboot
extern check_cpuid
extern check_long_mode
extern initialize_long_mode
extern entry64

; The start label
_start:
    ; We are going to identity map the first 2GB of memory
    ; to 0x0000000000000000 and then also map it to
    ; 0xFFFFFFFF80000000 which is about at the last 2 GB of memory
    
    ; Stash the multiboot info location
    mov [no_offset(multiboot_info)], ebx

    ; Setup new stack
    mov esp, no_offset(stack_end)

    ; Run checks
    call check_multiboot
    call check_cpuid
    call check_long_mode


    ; Initialize long mode
    call initialize_long_mode

    ; Far jump to 64 bit entry point
    jmp 0x08:no_offset(entry64)

section .bss

; The address of the multiboot info structure
multiboot_info: resd 1

; We also want a stack
stack_begin:
    resb 0x1000
stack_end:



