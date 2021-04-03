global _start
extern entry
extern kernOffset
section .text
%define offset 0xC0000000
;%define offset 0
bits 32
_start:
    ; Setup stack, Without the kernel offset
    mov esp, stack_end - offset

    
    

    ; Run checks
    call check_multiboot
    call check_cpuid
    call check_long_mode

    ; Initialize paging
    jmp init_paging
    


; Setup Page tables
init_paging:
    ; Map first PML4 Entry
    mov eax, pdpt - offset
    or eax, 0b11
    mov [pml4 - offset], eax

    

    ; Map the higher half entry in PDPT
    mov eax, pd2 - offset
    or eax, 0b11
    mov [pdpt - offset + 3 * 8], eax

    ; Initialize counter at 0
    mov ecx, 0
.map_table
    ; We want eax to contain the size of the page
    ; (2MB) * the index in the page table.
    mov eax, 0x200000
    mul ecx
    ; Now we will set the
    ; present, writable, and huge bits.
    or eax, 0b10000011

    ; And move it into the higher half entry
    mov [pd2 - offset + ecx * 8], eax

    ; Increment the counter
    inc ecx

    ; And if it is 512, we have mapped the entire table
    cmp ecx, 512
    jle .map_table ; If not, map the next

    
    ; Identity map first 2MB

    mov ecx, 0 ; Set counter bit

    ; Map the first PDPT entry
    mov eax, pd - offset
    or eax, 0b11
    mov [pdpt - offset], eax
    
    ; And map the first entry in the page directory
    mov eax, 0x0 ; We want the first page
    or eax, 0b10000011 ; We want huge (2MB) pages, we mark it as present, and as writable
    mov [pd - offset], eax ; And move it into the table.

    ; Load PML4
    mov eax, pml4 - offset
    mov cr3, eax

    ; Enable PAE
    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    ; Enable long mode
    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    ; Enable paging
    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax
    

    ; Load GDT
    lgdt [gdt64_pointer - offset]

    ; Long jump
    jmp 0x08:entry - offset


; Here we have some checks

; First we check for multiboot
check_multiboot:
    cmp eax, 0x36d76289
    jne .no_multiboot
    ret
.no_multiboot:
    mov al, "0"
    mov bl, "M"
    jmp error



check_cpuid:
    ; Check if CPUID is supported by attempting to flip the ID bit (bit 21)
    ; in the FLAGS register. If we can flip it, CPUID is available.

    ; Copy FLAGS in to EAX via stack
    pushfd
    pop eax

    ; Copy to ECX as well for comparing later on
    mov ecx, eax

    ; Flip the ID bit
    xor eax, 1 << 21

    ; Copy EAX to FLAGS via the stack
    push eax
    popfd

    ; Copy FLAGS back to EAX (with the flipped bit if CPUID is supported)
    pushfd
    pop eax

    ; Restore FLAGS from the old version stored in ECX (i.e. flipping the
    ; ID bit back if it was ever flipped).
    push ecx
    popfd

    ; Compare EAX and ECX. If they are equal then that means the bit
    ; wasn't flipped, and CPUID isn't supported.
    cmp eax, ecx
    je .no_cpuid
    ret
.no_cpuid:
    mov al, "0"
    mov bl, "C"
    jmp error


check_long_mode:
    ; test if extended processor info in available
    mov eax, 0x80000000    ; implicit argument for cpuid
    cpuid                  ; get highest supported argument
    cmp eax, 0x80000001    ; it needs to be at least 0x80000001
    jb .no_long_mode       ; if it's less, the CPU is too old for long mode

    ; use extended info to test if long mode is available
    mov eax, 0x80000001    ; argument for extended processor info
    cpuid                  ; returns various feature bits in ecx and edx
    test edx, 1 << 29      ; test if the LM-bit is set in the D-register
    jz .no_long_mode       ; If it's not set, there is no long mode
    ret
.no_long_mode:
    mov al, "0"
    mov bl, "L"
    jmp error


error:
    ; Prints "ERR: {code}" where code is an error code
    ; finally halts
    mov dword [0xb8000], 0x4f524f45
    mov dword [0xb8004], 0x4f3a4f52
    mov dword [0xb8008], 0x4f204f20
    mov byte  [0xb800a], al
    mov byte  [0xb800c], bl
    mov word  [0xb800d], 0x4f

    ; Halt
    hlt




section .bss
pml4:
    resb 4096
pdpt:
    resb 4096
pd:
    resb 4096
pd2:
    resb 4096

align 4096
stack_begin:
    resb 4096
stack_end:
section .rodata
gdt64:

gdt64_null:
    dd 0x00000000
    dd 0x00000000

gdt64_code:
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 0b10011010
    db 0b10101111
    db 0x00

gdt64_data:
    dw 0x0000
    dw 0x0000
    db 0x00
    db 0b10010010
    db 0b10100000
    db 0x00

gdt64_end:


gdt64_pointer:
    dw (gdt64_end - offset) - (gdt64 - offset) - 1
    dw gdt64 - offset