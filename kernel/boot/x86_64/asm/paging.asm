[bits 32]
%include "kernel/utility/asm/macros.s"
; Contains basic code for enabling an identity mapped paging system
section .text ; Text section

; Define global labels
global initialize_paging
global enable_paging

; Define external functions


initialize_paging:

    ; Map first PML4 entry
    mov eax, no_offset(pdpt_low)
    or eax, 0b11
    mov [no_offset(pml4)], eax

    ; Map the 511th PML4 entry
    mov eax, no_offset(pdpt_high)
    or eax, 0b11
    mov [no_offset(pml4) + 511 * 8], eax

    ; Map the higher half pdpt
    mov eax, no_offset(pd)
    or eax, 0b11
    mov [no_offset(pdpt_high) + 510 * 8], eax

    ; Map the lower half pdpt
    mov eax, no_offset(pd)
    or eax, 0b11
    mov [no_offset(pdpt_low) + 0 * 8], eax

    ; Map the page tables
    mov eax, no_offset(pts)
    mov ebx, 0b11
    mov ecx, 512*512 ; This is how many entries we have in all tables
    mov edx, 0
    call map_pts

    ; Map the page directory
    mov eax, no_offset(pd)
    mov ebx, 0b11
    mov ecx, 512
    mov edx, no_offset(pts)
    call map_pd

    ; Load the PML4
    mov eax, no_offset(pml4)
    mov cr3, eax

    ; Enable PAE
    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    ; Enable long mode

    mov ecx, 0xC0000080 ; Read the MSR
    rdmsr
    or eax, 1 << 8 ; Enable long mode bit
    wrmsr


    ret

; Function enables paging
enable_paging:

    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax

    ret

; This function maps ecx number of page tables
map_pts:
    ; Map ecx page tables
    ; EAX: address of tables
    ; EBX: flags
    ; ECX: number of tables
    ; EDX: offset address


    ; Clear all non flags
    and ebx, 0xFFF

    ; Apply flags
    or edx, ebx

.map1:
    ; Move address into the table
    mov [eax], edx

    ; Add 4096 to offset
    add edx, 4096

    ; Move to the next table
    add eax, 8

    ; Decrement and loop if ecx is larger than zero
    dec ecx
    cmp ecx, 0
    jg .map1


    ret

map_pd:
    ; Map ecx number of page directory entries
    ; EAX: address of page directory
    ; EBX: flags
    ; ECX: number of entries
    ; EDX: location of page tables

    ; Clear all non flags
    and ebx, 0xFFF

    ; Apply flags
    or edx, ebx

.map1:
    ; Move address into the table
    mov [eax], edx

    ; Add 4096 to offset
    add edx, 4096

    ; Move to the next table
    add eax, 8

    ; Decrement and loop if ecx is larger than zero
    dec ecx
    jnz .map1

    ret

section .bss ; The BSS section

; Align on a page boundary
align 4096

; PML4
pml4:
    resb 4096

; Two PDPTs (one for higherhalf and one for lowerhalf)
pdpt_low:
    resb 4096
pdpt_high:
    resb 4096

; A single page directory
pd:
    resb 4096

; 512 page tables
pts:
    resb 4096 * 512