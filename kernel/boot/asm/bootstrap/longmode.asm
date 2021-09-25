[bits 32]
%include "kernel/utility/asm/macros.s"
; This file contains functions to initialize long mode
; requirements

; Define external labels
extern initialize_paging
extern enable_paging
extern load_gdt

; Define global lables
global initialize_long_mode


initialize_long_mode:

    ; Initialize paging
    call initialize_paging
    
    ; Enable lonng mode
    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    ; Enable paging
    call enable_paging

    ; Load long mode GDT
    call load_gdt

    ret