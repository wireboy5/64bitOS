global _start
extern entry

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
    mov eax, pd - offset
    or eax, 0b11
    mov [pdpt - offset + 3 * 8], eax

    

    
    mov eax, pd - offset
    mov ebx, 0b11
    mov edx, pts - offset
    call pdmap
    
    
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

    ; Save argument registers
    push eax
    push edx

    mov eax, edx ; the 512 PTs

    ; ebx already has flags

    ; We start at an offset of 0
    mov edx, 0x0

    ; Map every page table
    call map_every_pt

    ; Pop argument registers
    pop edx
    pop eax

    ; Every page table has its entries mapped now.

    ; Now lets Add the flags to each one and move them into a page directory entry

    ; First initialize the counter
    mov ecx, 0

.map1 ; The loop lable

    ; Apply flags to EDX
    or edx, ebx

    ; Move into current PDE
    mov [eax + ecx * 8], edx

    ; Increment EDX by the size of a PT (4096)
    add edx, 0x1000


    ;; LOOP FOOTER
    inc ecx ; Increment ecx
    cmp ecx, 512 ; compare counter to 512
    jle .map1 ; if <= : .map1

    ;; END LOOP
    
    ret ; Return

map_every_pt:
    ; Maps 512 PTs at a given address
    ; EAX : address of 512 PTs
    ; EBX : flags of entries
    ; EDX : offset address

    ; Clear all non flags in EBX
    and ebx, 0b111111111111

    ; Init counter
    mov ecx, 0
    
.map1 ; Loop lable

    call ptmap ; Map the page table. The arguments are already setup :)

    ; We do not even have to increment edx, because ptmap does this for us!
    ; It iterates 512 times, and each time it adds the size of a page for it's own
    ; purposes. This helps us too.

    add eax, 0x1000 ; Add 4096 to eax (4096 is the size of a PT)

    ;; LOOP FOOTER
    inc ecx ; Increment ecx
    cmp ecx, 512 ; compare counter to 512
    jle .map1 ; if <= : .map1

    ;; END LOOP

    ret ; Return

ptmap:
    ; Maps a page table. Takes the following as arguments
    ; EAX : address of page table
    ; EBX : flags of entries
    ; EDX : starting offset address
    
    ; Clear all non flags in EBX
    and ebx, 0b111111111111
    
    ; Initialize the counter
    mov ecx, 0
    

.map1 ; Label for loop

    ; Apply flags (EBX) to EDX
    or edx, ebx

    ; Move entry into page table
    mov [eax + ecx], edx

    ; Increment ECX by the size of an entry (8)
    add ecx, 8

    ; Increment EDX by the size of a page (4096)
    add edx, 0x1000

    cmp ecx, 512 * 8 ; Compare ecx to 512 * 8 (Size of each entry)
    jle .map1 ; if <= : jump to .map1
    
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