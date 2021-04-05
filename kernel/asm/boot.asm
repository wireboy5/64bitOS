global _start
extern entry
extern kernOffset

%define offset 0xC0000000
;%define offset 0

section .text

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

    

    
    mov eax, pd
    mov ebx, 0b11
    mov edx, pts

    
    
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


pdmap:
    ; This function maps a page directory

    ; eax is the page directory
    ; ebx is the flags
    ; edx is the address where we want to have 512 page tables

    ; Discard any non flags in ebx
    and ebx, 0b111111111111

    ; Initialize the counter
    mov ecx, 0

.map1 ; This loop maps the page directory

    

    ; ESI needs to contain the address of the entry
    mov esi, pts ; Move the address of the array in
    add esi, ecx * 8 ; Add the offset

    ; Now lets map the page table at this address

    ; Save eax, ebx, edx
    push eax
    push ebx
    push edx

    ; Now set the 

    ; Now lets apply the flags
    and esi, ebx

    


    inc ecx ; Increment the counter

    cmp ecx, 512 ; Compare ecx to 512
    jle .map ; <= : jump to .map1

    ;; End .map1 loop

    ret ; Return



ptmap:
    ; This function maps a page table.
    ; This functions takes three arguments, the page table, the flags,
    ; and the starting address
    ; in eax, ebx, and edx respectively

    ; Save EDX
    push edx

    ; Discard anything in EBX that is not a flag
    and ebx, 0b111111111111

    ; Save the counter register
    push ecx

    ; Initialize the counter register
    mov ecx, 0

    ; Lets use EDX to store our current index in the page table
    mov esi, 8

.map1 ; This loop maps the page table
    
    mov edi, eax ; Move the address of the page table into EDI
    add edi, edx ; Add EDX (the offset) to EDI (the address)

    ; Now EDI contains the address of the page table entry
    ; EDX contains the physical address to map to
    ; and ESI contains the offset in the page table to use

    ; Add the flags to EDX
    or edx, ebx

    ; Put EDX into the table
    mov [edi - offset], edx



    ; Add 4096 to the starting address
    add edx, 0x1000

    add esi, 8 ; Add 8 (the size of an entry) to EDX

    inc ecx ; Increment the counter

    cmp ecx, 512 ; Compare ecx to 512

    jle .map1 ; If ecx is <= 512, loop

    ;; End .map1 loop

    ; Get the saved counter
    pop ecx

    ; Get the saved EDX
    pop edx

    ret ; Return



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
    push edx
    cpuid                  ; returns various feature bits in ecx and edx
    test edx, 1 << 29      ; test if the LM-bit is set in the D-register
    pop edx
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
; We want a single PML4
pml4:
    resb 4096
; A single pdpt
pdpt:
    resb 4096
; And a single page directory
pd:
    resb 4096

; we only use one of each of those because we are only mapping a 
; relatively small quantity of memory, enough to get us functioning in long mode

; However, we want 512 page tables, for entry in the page directory
; We treat these almost like an array
pts:
    resb 4096 * 512

align 4096
stack_begin:
    resb 4096
stack_end:

section .rodata

gdt64_start:
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
    dw (gdt64_end - offset) - (gdt64_start - offset) - 1
    dd gdt64_start - offset